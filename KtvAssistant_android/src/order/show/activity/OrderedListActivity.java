package order.show.activity;

import java.util.List;

import order.adapter.OrderedAdapter;
import tiange.ktv.assistant.R;
import android.os.Bundle;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import assistant.activity.BaseActivity;
import assistant.fragment.home.MainHomeFragment;
import assistant.global.AppStatus;
import assistant.manager.NetUtilManager.INetUtilListener;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowUtil;
import assistant.view.RefreshLayout;
import assistant.view.RefreshLayout.OnHeadRefreshListener;

import com.jni.netutil.ResultData_SongList;
import com.jni.netutil.XGMessage;

/**
 * 已点歌曲列表视图
 */
public class OrderedListActivity extends BaseActivity implements INetUtilListener
{
	private TextView tv_num, tv_empty;
	private View btn_back, btn_daoru;
	private ListView m_listViewSong;
	private RefreshLayout refreshLayout;
	private View ly_waiting;

	List<ResultData_SongList> m_songList = null;
	
	private boolean isPullToRefresh = false;
	private OrderedAdapter adapter;
	
	private TextView tv_songname, tv_artist;
	private ImageView btn_cut;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.ac_ordered);
		init();
		AppStatus.s_NetUtilMgr.registerListener(this);
	}
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		AppStatus.s_NetUtilMgr.unregisterListener(this);
		super.onDestroy();
	}
	/*
	 * 当指定Activity为launchMode为singleTask的时候
	 * ，通过Intent启到一个Activity,如果系统已经存在一个实例
	 * ，系统就会将请求发送到这个实例上，但这个时候
	 * ，系统就不会再调用通常情况下我们处理请求数据的onCreate方法
	 * ，而是调用onNewIntent方法
	 */
//	@Override
//	protected void onNewIntent(Intent intent) {
//		super.onNewIntent(intent);
//		initData(true);
//	}

	private void init() {
		findView();
		setListener();
		initData(true);
	}

	protected void findView() {
		tv_num = (TextView) findViewById(R.id.tv_num);
		btn_back = findViewById(R.id.iv_back);
		btn_daoru = findViewById(R.id.iv_daoru);
		m_listViewSong = (ListView) findViewById(R.id.listViewOrdered);
		refreshLayout = (RefreshLayout) findViewById(R.id.refreshLayout);
		ly_waiting = findViewById(R.id.ly_waiting);
		
		btn_daoru.setVisibility(View.GONE);
		refreshLayout.setFootRefreshable(false);
		
		tv_songname = (TextView) findViewById(R.id.tv_name);
		tv_artist = (TextView) findViewById(R.id.tv_artist);
		
		btn_cut = (ImageView) findViewById(R.id.btn_cut);
	}

	protected void setListener() {
		btn_back.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				finish();
			}
		});

		refreshLayout.setOnHeadRefreshListener(new OnHeadRefreshListener() {

			@Override
			public void onHeadRefresh()
			{
				if (checkNetWork()) 
				{
					isPullToRefresh = true;
					initData(true);//??
				}
			}
		});
		
		btn_cut.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				if(m_songList!=null && m_songList.size() <= 1)
				{
					ShowUtil.showToast(OrderedListActivity.this, R.string.playc_noorder_songlist);
					return;
				}
				AppStatus.s_playControMgr.switchSong(m_songList.get(0).songid);
			}
		});
	}

	protected void initData(boolean isShowingWaiting) {
		if(AppStatus.isInRoom) 
		{
			if (isShowingWaiting)
				ly_waiting.setVisibility(View.VISIBLE);

			updateSongList();
			
			if(isShowingWaiting)
				ly_waiting.setVisibility(View.GONE);
			
			showList();
		}
		else
			showEmptyTips(true);
		
		//如果是下拉刷新
		if (isPullToRefresh)
		{
			refreshLayout.onHeadRefreshComplete();
			isPullToRefresh = false;
		}
	}
	//显示当前点歌列表
	private void showList() {
		if (m_songList.isEmpty()) 
		{
			showEmptyTips(true);
			this.tv_songname.setText(R.string.please_order);
			this.tv_artist.setVisibility(View.INVISIBLE);
			
			return;
		}
		
		showEmptyTips(false);
		ResultData_SongList songInfo = m_songList.get(0);
		tv_artist.setVisibility(View.VISIBLE);
		tv_songname.setText(songInfo.songname);
		tv_artist.setText(songInfo.singername);
		
		int num = m_songList.size()-1;//已点歌曲，去掉正在播放的
		tv_num.setText("" + num);
		if (adapter == null) {
			adapter = new OrderedAdapter(this);
			m_listViewSong.setAdapter(adapter);
		} else
			adapter.notifyDataSetChanged();
	}

	/** 空数据提示显示 */
	protected void showEmptyTips(boolean isShowEmpty, int tipsResource) {
		if (tv_empty == null) {
			tv_empty = (TextView) findViewById(R.id.tv_empty);
		}

		tv_empty.setText(tipsResource);

		if (isShowEmpty) {
			m_listViewSong.setVisibility(View.INVISIBLE);
			tv_empty.setVisibility(View.VISIBLE);
		} else {
			tv_empty.setVisibility(View.GONE);
			m_listViewSong.setVisibility(View.VISIBLE);
		}
	}

	/** 空数据提示显示 */
	protected void showEmptyTips(boolean isShowEmpty) {
		showEmptyTips(isShowEmpty, R.string.empty_ordered_tips);
	}

	boolean checkNetWork() {
		return MobileNetStatUtil.checkCurrentNetState(this);
	}
	
	void updateSongList(){
		m_songList = MainHomeFragment.s_songList;
	}
	@Override
	public void dealWithMsg(Message msg) {
		// TODO Auto-generated method stub
		switch(msg.what)
		{
		//通知播放列表更新了
		case XGMessage.XGMSG_CUR_SONGLIST:
		{
			initData(false);
			break;
		}
		}
	}
}
