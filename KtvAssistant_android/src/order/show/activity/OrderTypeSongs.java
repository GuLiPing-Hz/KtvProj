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
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.LinearInterpolator;
import android.view.animation.RotateAnimation;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.entity.SongInfo;
import assistant.entity.TopicInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.manager.EnterRoomManager;
import assistant.manager.OnRoomListener;
import assistant.task.songapi.GetSongListByTopicTask;
import assistant.util.ShowUtil;
import assistant.view.LoadMoreListView;
import assistant.view.LoadMoreListView.LoadMoreCallBack;

/**
 * @author 宋静
 * @version 创建时间：2013-10-23 下午6:12:38 类说明 点歌歌曲列表页
 */
public class OrderTypeSongs extends OrderActivity {

	boolean isInited;
	boolean isSearched;
	NormalSongAdapter adapter;
	String searchText;
	LoadMoreListView listview_songs;
	
	View mUiLoading;
	Animation mRotateAnimation;
	int mRequestTopicId;
	int mRequestSingerId;
	/**************** 点歌飞盘特效 ***********************/
	private ImageView cdView;
	/**************** 点歌时判断是否登录包厢 ***********************/
	private EnterRoomManager enterRoomManager;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.ac_ordersongs);
		
		mRotateAnimation = new RotateAnimation(0, 360, Animation.RELATIVE_TO_SELF, 0.5f
				, Animation.RELATIVE_TO_SELF, 0.5f);
		mRotateAnimation.setDuration(1000);
		mRotateAnimation.setRepeatCount(Animation.INFINITE);
		mRotateAnimation.setRepeatMode(Animation.RESTART);
		mRotateAnimation.setInterpolator(new LinearInterpolator());
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
		{
			type = b.getInt(OrderActivity.BUNDLE_MODELID);
			mPreTopic = (TopicInfo) b.getSerializable(OrderActivity.BUNDLE_PRETOPIC);
			topic = (TopicInfo) b.getSerializable(OrderActivity.BUNDLE_CURTOPIC);
			
			if(mPreTopic != null)//如果是歌手页面过来的
			{
				mRequestTopicId = mPreTopic.topicId;
				mRequestSingerId = topic.topicId;
			}
			else//否则既是其他页面菜单过来的
			{
				mRequestTopicId = topic==null ? 0 : topic.topicId;
				mRequestSingerId = 0;
			}
		}
	}
	
	void startLoading(){
		mUiLoading.setVisibility(View.VISIBLE);
		mUiLoading.startAnimation(mRotateAnimation);
	}
	
	void stopLoading(){
		
		this.songhandler.postDelayed(new Runnable(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				mUiLoading.setVisibility(View.GONE);
				mUiLoading.clearAnimation();
			}
			
		}, 500);
	}

	@Override
	protected void findView() {
		super.findView();
		findCDView();
		listview_songs = (LoadMoreListView) findViewById(R.id.lv_songs);
		tv_empty = (TextView) findViewById(R.id.tv_empty);
		mUiLoading = findViewById(R.id.imgview_loading);
	}

	@Override
	protected void setView() {
		super.setView();
		if (topic != null && !TextUtils.isEmpty(topic.topicTitle))
			tv_title.setText(topic.topicTitle);

		if (type == 3) 
			et_search.setHint("请输入歌名进行搜索");
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

		if(checkNetWork() && type > 0)
		{
			mLoadingAnim.setVisibility(View.VISIBLE);

			//获取歌曲列表统一规则
			GetSongListByTopicTask task = new GetSongListByTopicTask(
					songhandler, type, this.mRequestTopicId,this.mRequestSingerId,1,EVERY_GET_NUMS);
			task.execute();

			isLoading = true;
		}
		else
			showEmptyTips(true);
	}

	@SuppressLint("HandlerLeak")
	private Handler songhandler = new Handler() {
		@SuppressWarnings("unchecked")
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			try {
				
				if (isLoading) {
					mLoadingAnim.setVisibility(View.GONE);
					isLoading = false;
				}
				
				switch (msg.what) {
				case MessageDef.WM_GET_SONG_LIST_BY_TOPIC:
					if (msg.arg2 <= 1)
					{
						if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
						{
							showSongsList((List<SongInfo>) msg.obj);
							setAnimaEndLocation();
						}
						else
						{
							showEmptyTips(true,(String)msg.obj);
							listview_songs.removeFooter();
						}
					}
					else 
					{
						List<SongInfo> list = (List<SongInfo>) msg.obj;
						if (msg.arg1== KtvAssistantAPIConfig.APIErrorCode.Nothing && checkListIsOrdered(list,1)) 
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
				case MessageDef.WM_SONG_SEARCH:
					stopLoading();
					if (msg.arg2 <= 1)
					{
						if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
						{
							//搜索成功之后，保存当前的搜索内容
							m_curSearchText = m_nextSearchText;
							m_nextSearchText = null;
							showSearchList((List<SongInfo>) msg.obj);
						}
						else
						{
							showEmptyTips(true);
							listview_songs.removeFooter();
						}
					}
					else 
					{
						List<SongInfo> list = (List<SongInfo>) msg.obj;
						if (msg.arg1== KtvAssistantAPIConfig.APIErrorCode.Nothing && checkListIsOrdered(list,1)) 
						{
							searchSongs.addAll(list);
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
			catch (Exception e) 
			{
				e.printStackTrace();
			}
		}
	};

	@Override
	protected boolean search() {
		if (super.search())
		{
			searchText = et_search.getText().toString();
			Log.i("Search", searchText);
			
			GetSongListByTopicTask task = new GetSongListByTopicTask(songhandler,type,this.mRequestTopicId
					,this.mRequestSingerId,searchText,1,EVERY_GET_NUMS);
			task.execute();
			startLoading();
			
			isSearched = true;
			isLoading = true;
			return true;
		}
		return false;
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();

		if (isInited) {
			updateSongList();
		}
	}

	protected void showSongsList(List<SongInfo> list) {
		if (checkListIsOrdered(list,0)) 
		{
			songs.clear();
			parts.clear();
			songs.addAll(list);

			if (songs.size() <= EVERY_PAGE_NUMS)
				parts.addAll(songs);
			else 
			{
				listview_songs.addFooter();
				parts.addAll(songs.subList(0, EVERY_PAGE_NUMS));
				number = EVERY_PAGE_NUMS;
			}

			if (adapter == null) {
				adapter = new NormalSongAdapter(this, parts, orderManager);
				listview_songs.setAdapter(adapter);
			} 
			else
			{
				adapter.resetOperationPos();
				adapter.notifyDataSetChanged();
				//回到第一个Item
				songhandler.postDelayed(new Runnable(){

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

	protected void showSearchList(List<SongInfo> list) {
		if (checkListIsOrdered(list,0)) {
			searchSongs.clear();
			searchSongs.addAll(list);
			parts.clear();

			if (searchSongs.size() <= EVERY_PAGE_NUMS)
				parts.addAll(searchSongs);
			else
			{
				listview_songs.addFooter();
				parts.addAll(searchSongs.subList(0, EVERY_PAGE_NUMS));
				number = EVERY_PAGE_NUMS;
			}

			if (adapter == null) 
			{
				adapter = new NormalSongAdapter(this, parts, orderManager);
				lv_menu.setAdapter(adapter);
			}
			else
			{
				adapter.resetOperationPos();
				adapter.notifyDataSetChanged();
				//回到第一个Item
				songhandler.postDelayed(new Runnable(){

					@Override
					public void run() {
						// TODO Auto-generated method stub
						listview_songs.setSelection(0);
					}
					
				},100);
			}

			isInited = true;
		}
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) {
			if (ly_search.getVisibility() == View.VISIBLE) {
				if (et_search.getText().length() > 0) {
					et_search.setText("");
					return true;
				}

				if (isSearched) {
					isSearched = false;
					searchSongs.clear();
					searchSongs.addAll(songs);
					showSongsList(searchSongs);
					searchSongs.clear();
					return true;
				}
			}
		}
		return super.onKeyDown(keyCode, event);
	}

	protected void addMoreData() {
		List<SongInfo> list;

		if (isSearched)
			list = searchSongs;
		else
			list = songs;

		int count = list.size() - parts.size();

		if (count <= 0) 
		{
			if (listview_songs.isHasFoot()) 
			{
				number = list.size();
				getMoreData();
			}
			return;
		}

		if (count >= EVERY_PAGE_NUMS) 
			parts.addAll(list.subList(number, number + EVERY_PAGE_NUMS));
		else
			parts.addAll(list.subList(number, list.size()));
		adapter.notifyDataSetChanged();
		number = parts.size();
	}

	protected void getMoreData() {
		if (isSearched)
		{
			searchMoreTask(songhandler,searchText,type,mRequestTopicId,mRequestSingerId
					,number+1,EVERY_GET_NUMS);
		}
		else 
		{
			GetSongListByTopicTask task = new GetSongListByTopicTask(
					songhandler, type, mRequestTopicId,mRequestSingerId
					,number + 1, EVERY_GET_NUMS);
			task.execute();
		}
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

	private void findCDView() {
		cdView = (ImageView) findViewById(R.id.ordertype_view);
	}

	private void setCDView() {
		orderManager.setCDView(cdView);
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
			if(enterRoomManager != null && enterRoomManager.m_isSlientEnterRoom)
				ShowUtil.showToast(OrderTypeSongs.this, R.string.login_success);
			entryRoomState(true);			
		}

		@Override
		public void EnterRoomFail(String msg) {
			if (!TextUtils.isEmpty(msg))
				ShowUtil.showToast(OrderTypeSongs.this, msg);
			else
				ShowUtil.showToast(OrderTypeSongs.this, R.string.login_fail);
		}

		@Override
		public void ExitRoomSuccess() {
		}

		@Override
		public void ExitRoomFail(String msg) {
		}
	};

	@Override
	public void onAnimaEnd() {
		// TODO Auto-generated method stub
		//updateSongNumber();
	}
}