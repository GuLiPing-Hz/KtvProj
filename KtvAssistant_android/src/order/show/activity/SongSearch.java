package order.show.activity;

import java.util.List;

import order.adapter.NormalSongAdapter;
import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.entity.SongInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.manager.EnterRoomManager;
import assistant.manager.OnRoomListener;
import assistant.util.ShowUtil;
import assistant.view.LoadMoreListView;
import assistant.view.LoadMoreListView.LoadMoreCallBack;
import assistant.view.PngAnimView;

/**
 * @author 宋静
 * @version 创建时间：2013-10-31 下午4:40:35 类说明 点歌台首页搜索跳转的页面
 */
public class SongSearch extends OrderActivity {

	private String searchText;
	private NormalSongAdapter adapter;
	boolean isInited;
	LoadMoreListView listview_songs;
	
	/**************** 点歌飞盘特效 ***********************/

	private ImageView cdView;
	/**************** 点歌时判断是否登录包厢 ***********************/
	private EnterRoomManager enterRoomManager;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.ac_ordersongs);
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
		initPreData();
		findView();
		setView();
		setListener();
		initData();
	}

	private void initPreData() {
		Bundle b = this.getIntent().getExtras();
		if (b != null)
			searchText = b.getString("text");
	}

	@Override
	protected void findView() {
		super.findView();
		findCDView();
		listview_songs = (LoadMoreListView) findViewById(R.id.lv_songs);
		mLoadingAnim = (PngAnimView) findViewById(R.id.ly_waiting);
		tv_empty = (TextView) findViewById(R.id.tv_empty);
	}
	
	@Override
	protected void setView() {
		super.setView();
		tv_title.setText(R.string.search);
		et_search.setText(searchText);
	}
	
	@Override
	protected void setListener() {
		// TODO Auto-generated method stub
		super.setListener();
		
		listview_songs.setloadMoreListener(new LoadMoreCallBack() {

			@Override
			public void loadMore() {
				addMoreData();
			}
		});
	}

	protected void initData() {
		setCDView();
		
		if(checkNetWork())
		{
			isLoading = true;
			searchTask(handler, searchText, 0, 0);
		}
	}

	@SuppressLint("HandlerLeak")
	private Handler handler = new Handler() {

		@SuppressWarnings("unchecked")
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			try {
				if(isLoading)
					mLoadingAnim.setVisibility(View.GONE);
				
				switch (msg.what) {
				case MessageDef.WM_SONG_SEARCH:
					isLoading = false;
					if (msg.arg2 <= 1)// 获取成功
					{
						if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
						{
							//搜索成功保存当前搜索条件
							if(m_nextSearchText == null)
								m_curSearchText = searchText;
							else
								m_curSearchText = m_nextSearchText;
							showSongsList((List<SongInfo>) msg.obj);
							setAnimaEndLocation();
						}
						else
						{
							showEmptyTips(true);
							isLoading = false;
						}
					}
					else 
					{
						List<SongInfo> list = (List<SongInfo>) msg.obj;
						if (msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing &&checkListIsOrdered(list,1)) 
						{
							songs.addAll(list);
							addMoreData();
						} 
						else 
						{
							listview_songs.removeFooter();
						}
					}
					break;
				}
			} 
			catch (Exception e) {
				e.printStackTrace();
			}
		}
	};

	protected void showSongsList(List<SongInfo> list) {
		if (checkListIsOrdered(list,0)) {
			songs.clear();
			parts.clear();
			songs.addAll(list);
			number = 0;
			
			if (songs.size() <= EVERY_PAGE_NUMS)
				parts.addAll(songs);
			else
			{
				listview_songs.addFooter();
				parts.addAll(songs.subList(0, EVERY_PAGE_NUMS));
				number = EVERY_PAGE_NUMS;
			}

			if (adapter == null) 
			{
				adapter = new NormalSongAdapter(this, parts, this.orderManager);
				listview_songs.setAdapter(adapter);
			} 
			else
			{
				adapter.resetOperationPos();
				adapter.notifyDataSetChanged();
				//重新搜索到的结果，回到第一个
				handler.postDelayed(new Runnable(){

					@Override
					public void run() {
						// TODO Auto-generated method stub
						listview_songs.setSelection(0);
					}
					
				},100);
			}

			if (!AppStatus.isInRoom) 
			{
				enterRoomManager = new EnterRoomManager(this, roomListener);
				adapter.setEnterRoomManager(enterRoomManager);
			}
			
			isInited = true;
		}
	}

	protected void addMoreData() {
		int count = songs.size() - parts.size();

		if (count <= 0) 
		{
			if (listview_songs.isHasFoot()) 
			{
				number = songs.size();
				searchMoreTask(handler, searchText, 0, 0, number + 1,EVERY_GET_NUMS);
			}
			return;
		}

		if (count >= EVERY_PAGE_NUMS)
			parts.addAll(songs.subList(number, number + EVERY_PAGE_NUMS));
		else
			parts.addAll(songs.subList(number, songs.size()));
		adapter.notifyDataSetChanged();
		number = parts.size();
	}

	@Override
	protected boolean search() {
		if (super.search()) 
		{
			searchText = et_search.getText().toString();
			if (!TextUtils.isEmpty(searchText))
			{
				searchTask(handler, searchText, 0, 0);
				number = 1;
				return true;
			}
		}
		return false;
	}

	@Override
	protected void onResume() {
		super.onResume();
		if (isInited)
			updateSongList();
	}

	@Override
	protected void updateSongListInSubClass() {
		// TODO Auto-generated method stub
		if(checkSongListIsOrdered(parts))
			adapter.notifyDataSetChanged();
	}
	
	/** 空数据提示显示 */
	protected void showEmptyTips(boolean isShow) {
		if (isShow) 
		{
			listview_songs.setVisibility(View.GONE);
			tv_empty.setVisibility(View.VISIBLE);
		} 
		else 
		{
			listview_songs.setVisibility(View.VISIBLE);
			tv_empty.setVisibility(View.GONE);
		}
	}
	
	private void findCDView(){
		cdView = (ImageView)findViewById(R.id.ordertype_view);
	} 
	
	private void setCDView(){
		if(orderManager !=null){
			orderManager.setCDView(cdView);
		}
	} 

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		if (enterRoomManager != null) {
			enterRoomManager.activityResult(requestCode, resultCode, data);
		}
	}

	private OnRoomListener roomListener = new OnRoomListener() {
		@Override
		public void EnterRoomSuccess() {
			if(enterRoomManager != null && !enterRoomManager.m_isSlientEnterRoom)
				ShowUtil.showToast(SongSearch.this, R.string.login_success);
			entryRoomState(true);
		}

		@Override
		public void EnterRoomFail(String msg) {
			if (!TextUtils.isEmpty(msg))
				ShowUtil.showToast(SongSearch.this, msg);
			else
				ShowUtil.showToast(SongSearch.this, R.string.login_fail);
		}

		@Override
		public void ExitRoomSuccess() {
//			ShowUtil.showToast(SongSearch.this, R.string.exit_room_success);
//			entryRoomState(false);
		}

		@Override
		public void ExitRoomFail(String msg) {
//			if (!TextUtils.isEmpty(msg))
//				ShowUtil.showToast(SongSearch.this, msg);
//			else
//				ShowUtil.showToast(SongSearch.this, R.string.exit_room_fail);
		}
	};

	@Override
	public void onAnimaEnd() {
		// TODO Auto-generated method stub
		//updateSongNumber();
	}
}
