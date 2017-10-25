package assistant.fragment;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import order.adapter.CollectAdapter;
import order.manager.CollectManager;
import order.util.CollectListListener;
import tiange.ktv.assistant.R;
import android.content.Intent;
import android.os.Bundle;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.TextView;
import assistant.entity.SongInfo;
import assistant.global.AppStatus;
import assistant.manager.EnterRoomManager;
import assistant.manager.NetUtilManager.INetUtilListener;
import assistant.manager.OnRoomListener;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowUtil;

import com.jni.netutil.ResultData_SongList;
import com.jni.netutil.XGMessage;

/**
 * @author 宋静
 * @version 创建时间：2013-12-11 下午9:40:11 类说明 我的收藏
 */
public class CollectFragment extends BaseFragment implements
		CollectListListener,INetUtilListener,OnRoomListener
{
	//private Home mHome;
	private View vRoot;

	List<ResultData_SongList> m_songList = null;
	Set<Integer> m_songSet = new HashSet<Integer>();
	
	/* ================ 初始化 =============== */
	/* view */
	private TextView tv_title, tv_empty;
	private ImageButton btn_back;
	private View ly_waiting;
	private ListView lv_menu;
	
	private CollectManager collectManager;
	private CollectAdapter collectAdapter;
	private List<SongInfo> collectList = new ArrayList<SongInfo>();
	
	EnterRoomManager m_enterRoomMgr;

	/* ================ 复写系统方法 =============== */
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		vRoot = inflater.inflate(R.layout.fm_collect, container, false);
		initView();
		
		AppStatus.s_NetUtilMgr.registerListener(this);
		
		initData();
		return vRoot;
	}

	@Override
	public void onDestroyView() {
		// TODO Auto-generated method stub
		AppStatus.s_NetUtilMgr.unregisterListener(this);
		super.onDestroyView();
	}

	public void onEnterRoomActivityResult(int requestCode, int resultCode, Intent data){
		if(m_enterRoomMgr != null)
			m_enterRoomMgr.activityResult(requestCode, resultCode, data);
	}
	/** 初始化 */
	private void initView() {
		//mHome = (Home) this.getActivity();
		findView();
		setView();
	}

	protected void findView() {
		// title
		tv_title = (TextView) vRoot.findViewById(R.id.tv_title);
		btn_back = (ImageButton) vRoot.findViewById(R.id.iv_back);

		// list
		lv_menu = (ListView) vRoot.findViewById(R.id.lv_menu);
		ly_waiting = vRoot.findViewById(R.id.ly_waiting);

		//empty tips
		tv_empty = (TextView) vRoot.findViewById(R.id.tv_empty);
	}

	protected void setView() {
		// title
		tv_title.setText(getTitleString(0));
		
		btn_back.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				getActivity().finish();
			}
		});
	}

	private String getTitleString(int count) {
		String title = getString(R.string.menuitem_favourite) + "（共" + count + "首）";
		return title;
	}

	protected void initData() 
	{
		if (AppStatus.isAleadyLogin()) 
		{
			if(!checkNetWork()){
				showEmptyTips(true);
				return;
			}
			
			collectManager = new CollectManager(this);
			collectManager.getCollectData();

			if (AppStatus.isInRoom) {
				updateSongList();
			}
			else
				m_enterRoomMgr = new EnterRoomManager(getActivity(),this);
		} 
		else
		{
			showEmptyTips(true);
			tv_empty.setText(R.string.nologin);
		}
	}

	@Override
	public void updateCollectList(List<SongInfo> list) {
		ly_waiting.setVisibility(View.GONE);
		tv_title.setText(getTitleString(list.size()));
		showCollectList(list);
	}

	@Override
	public void getCollectListFail(String errorMessage) {
		showEmptyTips(true);
		tv_empty.setText(errorMessage);
	}

	protected void showCollectList(List<SongInfo> list) {
		if (list == null || list.size() <= 0) {
			showEmptyTips(true);
			return;
		}
		showEmptyTips(false);
		
		collectList.clear();
		collectList.addAll(list);
		if (AppStatus.isInRoom)
			filterSongList(collectList);//针对当前点歌列表过滤字段

		if (collectAdapter == null) 
		{	
			collectAdapter = new CollectAdapter(this.getActivity(), collectList,collectManager);
			collectAdapter.setEnterRoomManager(m_enterRoomMgr);
			lv_menu.setAdapter(collectAdapter);
		}
		else
			collectAdapter.notifyDataSetChanged();
	}

	/** 空数据提示显示 */
	protected void showEmptyTips(boolean isShow) {
		if (isShow)
		{
			lv_menu.setVisibility(View.GONE);
			tv_empty.setVisibility(View.VISIBLE);
		} 
		else
		{
			lv_menu.setVisibility(View.VISIBLE);
			tv_empty.setVisibility(View.GONE);
		}
	}

	/** 判断网络是否存在 */
	protected boolean checkNetWork() {
		return MobileNetStatUtil.checkCurrentNetState(getActivity());
	}

	void filterSongList(List<SongInfo> list){
		// 排除错误单元
		for (int i = 0; i < list.size(); i++) {
			SongInfo s = list.get(i);
			//判断是否已点
			if(m_songSet.size()>0 && m_songSet.contains(s.songId))
				s.isOrdered = true;
			else
				s.isOrdered = false;
		}
		
		if (collectAdapter != null)
			collectAdapter.notifyDataSetChanged();
	}
	void updateSongList(){
		m_songList = AppStatus.s_NetUtilMgr.getSonglist();
		m_songSet.clear();
		for(int i=0;i<m_songList.size();i++)
			m_songSet.add(m_songList.get(i).songid);
		
		filterSongList(collectList);
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
	
	public static Fragment newInstance(){
		Fragment fragment = new CollectFragment();
		return fragment;
	}

	@Override
	public void EnterRoomSuccess() {
		// TODO Auto-generated method stub
		if(m_enterRoomMgr != null && !m_enterRoomMgr.m_isSlientEnterRoom)
			ShowUtil.showToast(getActivity(), R.string.login_success);
		
		updateSongList();
	}

	@Override
	public void EnterRoomFail(String msg) {
		// TODO Auto-generated method stub
		if (!TextUtils.isEmpty(msg))
			ShowUtil.showToast(getActivity(), msg);
		else
			ShowUtil.showToast(getActivity(), R.string.login_fail);
	}

	@Override
	public void ExitRoomSuccess() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void ExitRoomFail(String msg) {
		// TODO Auto-generated method stub
		
	}

}