package order.show.activity;

import java.util.ArrayList;
import java.util.List;

import order.adapter.TopSongsAdapter;
import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.view.KeyEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.entity.TopicInfo;
import assistant.entity.WorkInfo;
import assistant.global.AppStatus;
import assistant.global.MessageDef;
import assistant.manager.EnterRoomManager;
import assistant.manager.OnRoomListener;
import assistant.task.songapi.GetTopScoreListTask;
import assistant.util.ImageUtil;
import assistant.util.ShowLog;
import assistant.util.ShowUtil;
import assistant.view.LoadMoreListView;
import assistant.view.LoadMoreListView.LoadMoreCallBack;

/**
 * 
 * @author Administrator
 * @
 */
public class OrderTypeTopSongs extends OrderActivity {

	private final static int IMAGE_DOWNLOAD = 0x8888;
	
	boolean isInited;
	boolean isSearched;
	TopSongsAdapter adapter;
	String searchText;
	LoadMoreListView listview_topSong;
	List<String> imageUrl = new ArrayList<String>();
	public List<Bitmap> headPhoto = new ArrayList<Bitmap>();

	/**************** 点歌飞盘特效 ***********************/
	private ImageView cdView;
	/**************** 点歌时判断是否登录包厢 ***********************/
	private EnterRoomManager enterRoomManager;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.ac_ordertopsongs);
		init();
		onRegisterNet();
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		onUnregisterNet();
		super.onDestroy();
	}

	private void init() {
		//获取当前类型，当前主题
		initPreData();
		findView();
		setView();
		setListener();
		initData();
	}

	private void initPreData() {
		Bundle b = getIntent().getExtras();
		if (b != null) {
			type = b.getInt(OrderActivity.BUNDLE_MODELID);
			topic = (TopicInfo) b.getSerializable(OrderActivity.BUNDLE_CURTOPIC);
		}
	}

	@Override
	protected void findView() {
		super.findView();
		//点歌特效图片
		cdView = (ImageView) findViewById(R.id.ordertype_view);
		listview_topSong = (LoadMoreListView) findViewById(R.id.item_order_top_songs);
		tv_empty = (TextView) findViewById(R.id.tv_empty);
	}

	@Override
	protected void setView() {
		super.setView();
		if (topic != null && !TextUtils.isEmpty(topic.topicTitle))
			tv_title.setText(topic.topicTitle);
	}

	@Override
	protected void setListener() {
		// TODO Auto-generated method stub
		super.setListener();
		listview_topSong.setloadMoreListener(new LoadMoreCallBack() {
			@Override
			public void loadMore() {
				addMoreData();
			}
		});
	}

	protected void initData() {
		setCDView();
		
		if (!checkNetWork()) {
			showEmptyTips(true);
			return;
		}

		if (topic != null) {
			mLoadingAnim.setVisibility(View.VISIBLE);
			GetTopScoreListTask getTopWorkListTask = new GetTopScoreListTask(
					topSonghandler, type,topic.topicId, 1, EVERY_GET_NUMS);
			getTopWorkListTask.execute();
			isLoading = true;
		}else{
			showEmptyTips(true);
		}
	}

	@SuppressLint("HandlerLeak")
	private Handler topSonghandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			try {
				switch (msg.what) {
				case MessageDef.WM_GET_TOP_SCORELIST:
				{
					if (isLoading)
					{
						mLoadingAnim.setVisibility(View.GONE);
						isLoading = false;
					}
					
					if (msg.arg2 == 1) //第一次获取到数据
					{
						@SuppressWarnings("unchecked")
						List<WorkInfo> list = (List<WorkInfo>) msg.obj;
						showWorkSongList(list);
						setAnimaEndLocation();
					} 
					else //下拉列表获取更多数据
					{
						// 更多数据
						@SuppressWarnings("unchecked")
						List<WorkInfo> list = (List<WorkInfo>) msg.obj;
						if (checkTopSongList(list,1)) 
						{
							works.addAll(list);
							addMoreData();
						} 
						else //如果服务器返回数据已经为空，则清楚掉footer
							listview_topSong.removeFooter();
					}
					break;
				}
				case IMAGE_DOWNLOAD:
					// 头像下载成功
					if (adapter != null)
						adapter.notifyDataSetChanged();
					break;
				default:
					// 获取失败
					if (isLoading) {
						mLoadingAnim.setVisibility(View.GONE);
						showEmptyTips(true);
						isLoading = false;
					}

					ShowLog.e(TAG, "handler error：" + msg.what + "," + msg.obj);
					break;
				}
			} catch (Exception e) {
				e.printStackTrace();
			}

		}
	};

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();

//		if (isInited) {
//			updateList();
//		}
	}

	/**
	 * 
	 * @param list
	 */
	protected void showWorkSongList(List<WorkInfo> list) {
		if (checkTopSongList(list,0)) 
		{
			works.clear();
			partsWork.clear();
			works.addAll(list);

			number = 0;
			
			if (works.size() <= EVERY_PAGE_NUMS) 
				partsWork.addAll(works);
			else 
			{
				listview_topSong.addFooter();
				partsWork.addAll(works.subList(0, EVERY_PAGE_NUMS));
				number = EVERY_PAGE_NUMS;
			}

			checkWorkListIsOrdered(partsWork);
			if (adapter == null) {
				adapter = new TopSongsAdapter(this, partsWork, orderManager);
				listview_topSong.setAdapter(adapter);
			} 
			else
				adapter.notifyDataSetChanged();

			if (!AppStatus.isInRoom) {//如果不在房间，则需要提供进入房间的入口
				enterRoomManager = new EnterRoomManager(this, roomListener);
				adapter.setEnterRoomManager(enterRoomManager);
			}

			isInited = true;
			// 下载图片
			ImageUtil.showCashImage(this, topSonghandler, imageUrl,IMAGE_DOWNLOAD, true);
		}
	}

	/**
	 * 
	 * @param list [in,out] 检查list，去掉非法数据，并且取出其中的头像数据，添加到imageUrl以供下载
	 * @param isLoadMore 是否是下拉加载更多数据
	 * @return 如果list为空，则返回false
	 */
	protected boolean checkTopSongList(List<WorkInfo> list,int isLoadMore) {
		if (list == null || list.isEmpty()) {
			if (1 == isLoadMore)
				showEmptyTips(false);
			else if (0 == isLoadMore) 
				showEmptyTips(true);
			return false;
		}

		// 排除错误单元,同时取出需要下载的图片URL
		imageUrl.clear();
		for (int i = 0; i < list.size(); i++)
		{
			WorkInfo info = list.get(i);
			if (info == null || info.song == null) {
				list.remove(info);
				i--;
			} 
		    else if (info.user != null && !TextUtils.isEmpty(info.user.headphoto)) // 寻找图片
				imageUrl.add(info.user.headphoto);
			else if (info.song != null && !TextUtils.isEmpty(info.song.photo))
				imageUrl.add(info.song.photo);
		}

		//检查歌曲列表中是否在已点歌曲列表中
		checkWorkListIsOrdered(list);

		if (list.isEmpty())
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
					return true;
				}

				if (isSearched)
				{
					isSearched = false;
					searchSongs.clear();
					searchSongs.addAll(songs);
					searchSongs.clear();
					return true;
				}
			}
		}
		return super.onKeyDown(keyCode, event);
	}

	protected void addMoreData() {
		List<WorkInfo> list;
		list = works;
		
		int count = list.size() - partsWork.size();
		//如果本地缓存取光了
		if (count == 0) 
		{
			//最多显示100的数据
			if(list.size() == 100)
			{
				listview_topSong.removeFooter();
				return ;
			}
			
			if (listview_topSong.isHasFoot()) 
			{
				number = list.size();
				getMoreData(number);
			}
			return;
		}

		if (count >= EVERY_PAGE_NUMS)//每次增加EVERY_PAGE_NUMS条数据
			partsWork.addAll(list.subList(number, number + EVERY_PAGE_NUMS));
		else
			partsWork.addAll(list.subList(number, list.size()));
		
		adapter.notifyDataSetChanged();
		
		number = partsWork.size();
	}

	protected void getMoreData(int begin) {
		if (!checkNetWork())
			return;
		
		GetTopScoreListTask getTopWorkListTask = new GetTopScoreListTask(
				topSonghandler, type,topic.topicId, begin + 1, EVERY_GET_NUMS);
		getTopWorkListTask.execute();
	}

	@Override
	protected void updateSongListInSubClass() {
		// TODO Auto-generated method stub
		if(checkWorkListIsOrdered(partsWork))
			adapter.notifyDataSetChanged();
	}

	/** 空数据提示显示 */
	protected void showEmptyTips(boolean isShow) {
		if (isShow) {
			listview_topSong.setVisibility(View.GONE);
			tv_empty.setVisibility(View.VISIBLE);
		} else {
			listview_topSong.setVisibility(View.VISIBLE);
			tv_empty.setVisibility(View.GONE);
		}
	}

	private void setCDView() {
		orderManager.setCDView(cdView);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		// 处理扫描结果
		if (enterRoomManager != null) 
			enterRoomManager.activityResult(requestCode, resultCode, data);
	}

	private OnRoomListener roomListener = new OnRoomListener() {
		@Override
		public void EnterRoomSuccess() {
			if (enterRoomManager != null && !enterRoomManager.m_isSlientEnterRoom)
				ShowUtil.showToast(OrderTypeTopSongs.this,R.string.login_success);
			entryRoomState(true);
		}

		@Override
		public void EnterRoomFail(String msg) {
			if (!TextUtils.isEmpty(msg))
				ShowUtil.showToast(OrderTypeTopSongs.this, msg);
			else
				ShowUtil.showToast(OrderTypeTopSongs.this, R.string.login_fail);
		}

		@Override
		public void ExitRoomSuccess() {
//			ShowUtil.showToast(OrderTypeTopSongs.this,R.string.exit_room_success);
//			entryRoomState(false);
		}

		@Override
		public void ExitRoomFail(String msg) {
//			if (!TextUtils.isEmpty(msg))
//				ShowUtil.showToast(OrderTypeTopSongs.this, msg);
//			else
//				ShowUtil.showToast(OrderTypeTopSongs.this,R.string.exit_room_fail);
		}
	};

	@Override
	public void onAnimaEnd() {
		// TODO Auto-generated method stub
		//updateSongNumber();
	}

}