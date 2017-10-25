package order.show.activity;

import java.util.ArrayList;
import java.util.List;

import order.adapter.HomeAdapter.ModelType;
import order.adapter.RanksAdapter;
import order.adapter.RanksAdapter.OnRanksClick;
import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import assistant.entity.TopicInfo;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.songapi.GetSongTopicsTask;
import assistant.util.ShowLog;

public class OrderTypeRanks extends OrderActivity implements OnRanksClick{

	List<TopicInfo> topicRnaks = new ArrayList<TopicInfo>();
	
	@SuppressLint("HandlerLeak")
	Handler mHandler = new Handler(){

		@SuppressWarnings("unchecked")
		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			
			mLoadingAnim.setVisibility(View.GONE);
			switch(msg.what)
			{
				case MessageDef.WM_GET_SONG_TOPICS:
				{
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
					{
						topicRnaks.clear();
						topicRnaks.addAll((List<TopicInfo>)msg.obj);
	
						RanksAdapter adapter = new RanksAdapter(OrderTypeRanks.this
								,OrderTypeRanks.this, topicRnaks);
						lv_menu.setAdapter(adapter);
					}
					else
					{
						ShowLog.e(TAG, "handler error：" + msg.what + "," + msg.obj);
						showEmptyTips(true,(String)msg.obj);
					}
					break;
				}
			}
		}
	};
	
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
			type = b.getInt(OrderActivity.BUNDLE_MODELID, ModelType.RANKLIST.getType());
		}
	}

	@Override
	protected void setView() {		
		super.setView();
		ModelType model = ModelType.getTypeByInt(type);
		tv_title.setText(model.getName());
		ly_search.setVisibility(View.GONE);
	}

	protected void initData() {
		if(checkNetWork()){
			mLoadingAnim.setVisibility(View.VISIBLE);
			GetSongTopicsTask task = new GetSongTopicsTask(mHandler, type);
			task.execute();
		}else{
			showEmptyTips(true);
		}
	}
	
	@Override
	public void onAnimaEnd() {
		// TODO Auto-generated method stub
		updateSongNumber();
	}

	@Override
	protected void updateSongListInSubClass() {
		// TODO Auto-generated method stub
		
	}

	void updateSongNumber(){
		int orderNum = m_songList.size()-1;
		orderNum = orderNum<0?0:orderNum;
		tv_number.setText(""+ orderNum);
	}

	@Override
	public void onClick(TopicInfo info) {
		// TODO Auto-generated method stub
		Intent i = null;
		//情歌对唱榜，新歌榜，得分榜区分开来
		if(info.topicId==1 || info.topicId==2 || info.topicId==8)
			i = new Intent(this,OrderTypeTopSongs.class);
		else
			i = new Intent(this,OrderTypeSongs.class);
		i.putExtra(OrderActivity.BUNDLE_MODELID, type);
		i.putExtra(OrderActivity.BUNDLE_CURTOPIC, info);
		startActivity(i);
	}
}
