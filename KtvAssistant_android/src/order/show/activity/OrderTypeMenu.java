package order.show.activity;

import java.util.ArrayList;
import java.util.List;

import order.adapter.GradeSongTopAdapter;
import order.adapter.HomeAdapter.ModelType;
import order.adapter.TopicAdapter;
import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import assistant.entity.GradeSongTopInfo;
import assistant.entity.TopicInfo;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.global.RequestCodeDef;
import assistant.task.songapi.GetSongTopicsTask;
import assistant.util.ShowLog;


/**
 * @author 宋静
 * @version 创建时间：2013-10-23 下午5:55:46 类说明 分类点歌（大类）
 */
public class OrderTypeMenu extends OrderActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.ac_ordertype);
		init();
		onRegisterNet();
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		onUnregisterNet();
		super.onDestroy();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		updateSongNumber();
	}

	private void init() {
		initPreData();
		findViewList();
		setView();
		setListener();
		initData();
	}

	private void initPreData() {
		Bundle b = this.getIntent().getExtras();
		if(b != null){
			type = b.getInt(OrderActivity.BUNDLE_MODELID, ModelType.THEME.getType());
		}
	}

	@Override
	protected void setView() {		
		super.setView();
		ModelType model = ModelType.getTypeByInt(type);
		tv_title.setText(model.getName());
		ly_search.setVisibility(View.GONE);
	}

	@Override
	protected void setListener() {
		super.setListener();
		
		lv_menu.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,long arg3) {
				entrySongList(arg2);
			}
		});
	}

	protected void initData() {
		if(checkNetWork()){
			mLoadingAnim.setVisibility(View.VISIBLE);
			GetSongTopicsTask task = new GetSongTopicsTask(handler, type);
			task.execute();
			
		}else{
			showEmptyTips(true);
		}
	}

	/*
	 * @author xpp
	 * 2013-12-18  13:25
	 * 添加了ModleType.TOPIC.getTYPE()和ModleType.TOPIC.getTYPE()
	 * */
	private void entrySongList(int position) {
		Intent i = new Intent();
		if (type == ModelType.SINGERS.getType()) 
			i.setClass(this, OrderTypeArtist.class);
		else if (type == ModelType.RANKLIST.getType()) 
			i.setClass(this, OrderTypeTopSongs.class);
		else if (type == ModelType.TOPIC.getType() || type == ModelType.THEME.getType()
				|| type == ModelType.LANGUAGE.getType())
			i.setClass(this, OrderTypeSongs.class);
		else
			return;
		
		i.putExtra(OrderActivity.BUNDLE_MODELID, type);
		i.putExtra(OrderActivity.BUNDLE_CURTOPIC, topics.get(position));
		startActivityForResult(i, RequestCodeDef.REQUEST_ROOM_STATUS);
	}
	
	@SuppressLint("HandlerLeak")
	private Handler handler = new Handler() {

		@SuppressWarnings("unchecked")
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			try {
				mLoadingAnim.setVisibility(View.GONE);
				switch (msg.what) {
					case MessageDef.WM_GET_SONG_TOPICS:
					{
						// 获取成功
						if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
							showList((List<TopicInfo>) msg.obj);
						else
						{
							ShowLog.e(TAG, "handler error：" + msg.what + "," + msg.obj);
							showEmptyTips(true,(String)msg.obj);
						}
						break;
					}
				}
			} catch (Exception e) {
				ShowLog.showException(e);
			}

		}
	};

	private List<TopicInfo> topics = new ArrayList<TopicInfo>();

	private void showGradeInfo(List<GradeSongTopInfo> obj){
		if (obj == null || obj.size() <= 0){
			showEmptyTips(true);
			return;
		}

		GradeSongTopAdapter adapter = new GradeSongTopAdapter(this,obj);
		lv_menu.setAdapter(adapter);
	}
	
	@Override
	protected void updateSongListInSubClass() {
		// TODO Auto-generated method stub
		
	}

	private void showList(List<TopicInfo> list) {
		if (list == null || list.size() <= 0){
			showEmptyTips(true);
			return;
		}

		// 排除错误单元
		for (int i = 0; i < list.size(); i++) {
			TopicInfo a = list.get(i);
			if (a == null || a.topicId < 0 || TextUtils.isEmpty(a.topicTitle)) {
				list.remove(a);
				i--;
			}
		}

		topics.clear();
		topics.addAll(list);

		TopicAdapter adapter = new TopicAdapter(this, topics);
		lv_menu.setAdapter(adapter);
	}

	@Override
	public void onAnimaEnd() {
		// TODO Auto-generated method stub
		updateSongNumber();
	}
}