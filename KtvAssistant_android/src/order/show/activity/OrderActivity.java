package order.show.activity;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import order.manager.OrderManager;
import order.show.listener.IOrderAnimaListener;
import tiange.ktv.assistant.R;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.EditorInfo;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import assistant.activity.BaseActivity;
import assistant.entity.SongInfo;
import assistant.entity.TopicInfo;
import assistant.entity.WorkInfo;
import assistant.fragment.home.MainHomeFragment;
import assistant.global.AppStatus;
import assistant.global.RequestCodeDef;
import assistant.manager.NetUtilManager.INetUtilListener;
import assistant.task.songapi.GetSongListByTopicTask;
import assistant.task.songapi.SongSearchTask;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;
import assistant.view.DrawableClickEditText;
import assistant.view.DrawableClickEditText.DrawableClickListener;
import assistant.view.PngAnimView;

import com.jni.netutil.ResultData_SongList;
import com.jni.netutil.XGMessage;

/**
 * @author 宋静
 * @version 创建时间：2013-10-29 下午4:05:23 类说明 检查在包厢内全局变量值是否被初始化，如果初始化了退出包厢重新进入
 */
public abstract class OrderActivity extends BaseActivity implements INetUtilListener
,IOrderAnimaListener
{
	public static final String BUNDLE_MODELID = "order.show.activity.OrderActivity.type";
	public static final String BUNDLE_CURTOPIC = "order.show.activity.OrderActivity.topic";
	public static final String BUNDLE_PRETOPIC = "order.show.activity.OrderActivity.singer";
	//如果是置顶点歌
	boolean m_isAddByTop;
	/* view */
	TextView tv_title, tv_number, tv_empty;
	ImageButton btn_back;
	ImageButton btn_order;
	ImageView iv_data_fail;
	//View ly_waiting;
	PngAnimView mLoadingAnim;
	
	View ly_search;
	DrawableClickEditText et_search;
	ListView lv_menu;
	public int moreData = 0; //0 刷新数据， 1 更多数据
	
	/* params */
	int type;//ModelID;
	TopicInfo mPreTopic;//前一个页面的Topic Info 服务于歌星3级页面
	TopicInfo topic;//当前页面的Topic Info
	long clickTime = 0L;

	List<SongInfo> songs = new ArrayList<SongInfo>();
	List<SongInfo> parts = new ArrayList<SongInfo>();
	List<SongInfo> searchSongs = new ArrayList<SongInfo>();
	//总共数据列表
	List<WorkInfo> works = new ArrayList<WorkInfo>();
	//当前显示数据列表
	List<WorkInfo> partsWork = new ArrayList<WorkInfo>();
	List<WorkInfo> searchWorks = new ArrayList<WorkInfo>();
	
	GetSongListByTopicTask searchTask;
	SongSearchTask commonSearchTask;
	boolean isLoading = false;

	static int EVERY_PAGE_NUMS = 20; // 这个数量要和LoadMoreListView里的count统一
	//当前数据的起始序号
	int number = 1;
	static int EVERY_GET_NUMS = 100; // 每次网络获取数量

	protected List<ResultData_SongList> m_songList = null;
	protected Set<Integer> m_songSet = null;
	
	OrderManager orderManager;
	String m_nextSearchText = null;
	String m_curSearchText = null;
	
	Handler m_handler = new Handler();
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		orderManager = new OrderManager();
	}
	
	protected void entryRoomState(boolean isEntryRoom) {
		if (isEntryRoom) {
			tv_number.setVisibility(View.VISIBLE);
			btn_order.setVisibility(View.VISIBLE);
		} else {
			tv_number.setVisibility(View.GONE);
			btn_order.setVisibility(View.INVISIBLE);
		}
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		
		if(RequestCodeDef.REQUEST_ROOM_STATUS == requestCode)
			entryRoomState(AppStatus.isInRoom);
	}

	protected void onRegisterNet(){
		AppStatus.s_NetUtilMgr.registerListener(this);
		updateSongList();
	}
	protected void onUnregisterNet(){
		AppStatus.s_NetUtilMgr.unregisterListener(this);
	}

	//供二级菜单使用
	protected void findView() {
		tv_title = (TextView) findViewById(R.id.tv_title);
		tv_number = (TextView) findViewById(R.id.tv_number);
		btn_back = (ImageButton) findViewById(R.id.iv_back);
		btn_order = (ImageButton) findViewById(R.id.iv_ordermenu);
		et_search = (DrawableClickEditText) findViewById(R.id.et_search);
		ly_search = findViewById(R.id.ly_search);
		
		//ly_waiting = findViewById(R.id.ly_waiting);
		mLoadingAnim = (PngAnimView) findViewById(R.id.ly_waiting);
		mLoadingAnim.initPngList();
	}

	//供一级菜单使用
	protected void findViewList() {
		findView();
		lv_menu = (ListView) findViewById(R.id.lv_menu);
	}

	protected void setView() {
		if (!AppStatus.isInRoom) {
			tv_number.setVisibility(View.GONE);
			btn_order.setVisibility(View.INVISIBLE);
		}
	}
	
	protected void setAnimaEndLocation() {
		int[] location = new int[2];
		btn_order.getLocationOnScreen(location);
		//点歌动画的结束位置
		orderManager.setEndLocation(location);
	}

	protected void setListener() {

		btn_back.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				ToolUtil.closeKeyBoard(OrderActivity.this);
				finish();
			}
		});

		btn_order.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				if (AppStatus.isInRoom)
					OrderActivity.this.startActivity(new Intent(
							OrderActivity.this, OrderedListActivity.class));
			}
		});

		if (ly_search.getVisibility() == View.VISIBLE) 
		{
			et_search.setDrawableClickListener(new DrawableClickListener() {

				@Override
				public void onClick(DrawablePosition target) {
					long time = System.currentTimeMillis();
					if (target == DrawablePosition.RIGHT && time - clickTime > 2000L) {
						clickTime = time;
						search();
					}
				}
			});

			et_search.setOnEditorActionListener(new TextView.OnEditorActionListener() {
				
				@Override
				public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
					// TODO Auto-generated method stub
					if(actionId == EditorInfo.IME_ACTION_SEARCH)
					{
						search();
						return true;
					}
					return false;
				}
			});
		}

	}
	
	protected void showEmptyTips(boolean isShow){
		showEmptyTips(isShow,this.getResources().getString(R.string.chat_pc_nodata_msg));
	}
	/** 空数据提示显示 */
	protected void showEmptyTips(boolean isShow,String tips) {
		if (tv_empty == null) {
			tv_empty = (TextView) findViewById(R.id.tv_empty);
		}
		if (isShow) {
			if(lv_menu != null)
				lv_menu.setVisibility(View.GONE);
			tv_empty.setVisibility(View.VISIBLE);
			tv_empty.setText(tips);
		} else {
			if(lv_menu != null)
				lv_menu.setVisibility(View.VISIBLE);
			tv_empty.setVisibility(View.GONE);
		}
	}

//	@Override
//	protected void onResume() {
//		// TODO Auto-generated method stub
//		super.onResume();
//		if (AppStatus.isEntryRoom && orderManager != null) {
//			// 更新已点歌曲数量
//			orderManager.getOrderData();
//		}
//	}

	//判断网络状态
	protected boolean search() {
		ToolUtil.closeKeyBoard(OrderActivity.this);
		
		if (!checkNetWork())
			return false;
		
		String text = et_search.getText().toString();

		//如果跟上次搜索内容相同
		if(m_curSearchText != null && TextUtils.equals(m_curSearchText,text))
			return false;
		m_nextSearchText = text;
		if (TextUtils.isEmpty(text)) {
			ShowUtil.showToast(this, R.string.search_input_empty_tips);
			return false;
		}

		return true;
	}

	/***
	 * 搜索 - 只是提供给SongSearch页面
	 * @param handler
	 * @param searchText
	 */
	protected void searchTask(Handler handler, String searchText) {
		searchTask(handler, searchText, type, topic.topicId);
	}

	protected void searchTask(Handler handler, String searchText, int modelId,
			long topicId) {
		mLoadingAnim.setVisibility(View.VISIBLE);

		if (commonSearchTask != null
				&& commonSearchTask.getStatus() == AsyncTask.Status.RUNNING) {
			commonSearchTask.cancel(true);
		}

		commonSearchTask = new SongSearchTask(handler, AppStatus.roomId, searchText,
				modelId, topicId);
		commonSearchTask.execute();
		isLoading = true;
	}
	
	protected void searchMoreTask(Handler handler, String searchText,
			int modelId, int topicId, int begin, int num) {
		if (searchTask != null
				&& searchTask.getStatus() == AsyncTask.Status.RUNNING) {
			searchTask.cancel(true);
		}

		searchTask = new GetSongListByTopicTask(handler,modelId, topicId,searchText, begin, num);
		searchTask.execute();
	}
	
	protected void searchMoreTask(Handler handler, String searchText,
			int modelId, int topicId, int singerId,int begin, int num) {
		if (searchTask != null
				&& searchTask.getStatus() == AsyncTask.Status.RUNNING) {
			searchTask.cancel(true);
		}

		searchTask = new GetSongListByTopicTask(handler,modelId, topicId,singerId,searchText, begin, num);
		searchTask.execute();
	}
	
	/**
	 * 检查歌曲是否已点
	 * @param list
	 * @return 返回点歌状态是否变化
	 */
	protected boolean checkWorkListIsOrdered(List<WorkInfo> list){
		boolean ret = false;
		if(list == null || list.isEmpty())
			return ret;
		
		for(WorkInfo info : list)
		{
			if(!m_songSet.isEmpty() && m_songSet.contains(info.song.songId))
			{
				if(!info.song.isOrdered)
					ret = true;
				info.song.isOrdered = true;
			}
			else
			{
				if(info.song.isOrdered)
					ret = true;
				info.song.isOrdered = false;
			}
		}
		return ret;
	}
	/**
	 * 
	 * @param list
	 * @return 返回点歌状态是否变化
	 */
	protected boolean checkSongListIsOrdered(List<SongInfo> list){
		boolean ret = false;
		if(list == null || list.isEmpty())
			return ret;
		
		for(SongInfo info : list)
		{
			if(!m_songSet.isEmpty() && m_songSet.contains(info.songId))
			{
				if(!info.isOrdered)
					ret = true;
				info.isOrdered = true;
			}
			else
			{
				if(info.isOrdered)
					ret = true;
				info.isOrdered = false;
			}
		}
		return ret;
	}
	
	/**
	 * 
	 * @param list [in,out] 检查list，去掉非法数据，并且取出其中的头像数据，添加到imageUrl以供下载
	 * @param isLoadMore 是否是下拉加载更多数据
	 * @return 如果list为空，则返回false
	 */
	protected boolean checkListIsOrdered(List<SongInfo> list,int isLoadMore) {
		if (list == null || list.isEmpty())
		{
			if (1 == isLoadMore)
				showEmptyTips(false);
			else if (0 == isLoadMore)
				showEmptyTips(true);
			return false;
		}

		// 排除错误单元
		for (int i = 0; i < list.size(); i++) 
		{
			SongInfo s = list.get(i);
			if (s == null || s.songId <= 0 || TextUtils.isEmpty(s.songName)) 
			{
				list.remove(s);
				i--;
			} 
			else 
			{
				if(!m_songSet.isEmpty() && m_songSet.contains(s.songId))
					s.isOrdered = true;
				else
					s.isOrdered = false;
			}
		}

		if (list == null || list.isEmpty()) 
		{
			if (1 == isLoadMore)
				showEmptyTips(false);
			else if (0 == isLoadMore)
				showEmptyTips(true);
			return false;
		}

		showEmptyTips(false);
		return true;
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) {
			if (ly_search.getVisibility() == View.VISIBLE) {
				if (et_search.getText().length() > 0) {
					et_search.setText("");
					ly_search.requestFocus();
					return true;
				}
			}
		}
		return super.onKeyDown(keyCode, event);
	}

	/** 判断网络是否存在 */
	protected boolean checkNetWork() {
		return MobileNetStatUtil.checkCurrentNetState(this);
	}
	
	protected abstract void updateSongListInSubClass();
	
	protected void updateSongList(){
		int beforeNumber = m_songList==null?0:m_songList.size();
		m_songList = MainHomeFragment.s_songList;
		m_songSet = MainHomeFragment.s_songSet;
		
		int afterNumber = m_songList.size();
		//如果减少
		if(afterNumber < beforeNumber || beforeNumber == 0)
			updateSongNumber();
		else
		{
			m_handler.postDelayed(new Runnable(){

				@Override
				public void run() {
					// TODO Auto-generated method stub
					updateSongNumber();
				}
				
			}, OrderManager.AnimaDuration);
		}
		
		updateSongListInSubClass();
	}
	void updateSongNumber(){
		int orderNum = m_songList.size()-1;
		orderNum = orderNum<0?0:orderNum;
		tv_number.setText(""+ orderNum);
	}
	@Override
	public void dealWithMsg(Message msg) {
		// TODO Auto-generated method stub
		switch(msg.what)
		{
		case XGMessage.XGMSG_CUR_SONGLIST:
			updateSongList();
			break;
		}
	}
}
