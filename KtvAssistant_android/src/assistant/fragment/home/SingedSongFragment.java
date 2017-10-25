package assistant.fragment.home;

import java.util.List;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.TextView;
import assistant.activity.Home;
import assistant.entity.SongRecord;
import assistant.fragment.BaseFragment;
import assistant.fragment.adapter.SongRecordAdapter;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.userapi.GetUserInfoTask;
import assistant.task.userapi.GetUserSongRecordTask;
import assistant.view.PngAnimView;
import assistant.view.RoundedImageView;

public class SingedSongFragment extends BaseFragment {

	List<SongRecord> mDataList;
	
	TextView mUiTitle;
	RoundedImageView mUiUserHead;
	View mUiBack;
	TextView mUiUserName;
	TextView mUiUserIdx;
	TextView mUiTotalScore;
	
	SongRecordAdapter mAdapter;
	
	ListView mUiListView;
	View mUiLine;
	PngAnimView mUiLoading;
	TextView mUiEmptyTip;
	
	@SuppressLint("HandlerLeak")
	Handler mHandler = new Handler(){

		@SuppressWarnings("unchecked")
		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			
			mUiLoading.setVisibility(View.GONE);
			switch(msg.what)
			{
			case MessageDef.WM_UPDATE_USER_INFO:
			{
				if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
				{
					//取到最新的用户信息 - 只更新累积总分信息
					mUiTotalScore.setText(String.format("%.1f", AppStatus.user.totalScore/10.0f));
				}
				break;
			}
			case MessageDef.WM_GETUSER_SONGRECORD:
			{
				if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
				{
					//取到最新的用户演唱信息
					if(msg.obj != null)
					{
						mDataList = (List<SongRecord>)msg.obj;
					
						if(mDataList == null || mDataList.size() == 0)
							mUiEmptyTip.setVisibility(View.VISIBLE);
						else
						{
							if(mAdapter == null)
							{
								mAdapter = new SongRecordAdapter(getActivity(),mDataList);
								mUiListView.setAdapter(mAdapter);			
								mUiLine.setVisibility(View.VISIBLE);
							}
							else
							{
								mAdapter.notifyDataSetChanged();
							}
						}
					}
				}
				else
				{
					mUiEmptyTip.setVisibility(View.VISIBLE);
					mUiEmptyTip.setText((String)msg.obj);
				}
				break;
			}
			}
		}
		
	};

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		View root = inflater.inflate(R.layout.fm_songrecord, null);
		
		this.mUiBack = root.findViewById(R.id.iv_back);
		this.mUiTitle = (TextView) root.findViewById(R.id.tv_title);
		this.mUiUserHead = (RoundedImageView) root.findViewById(R.id.iv_userhead);
		this.mUiUserName = (TextView) root.findViewById(R.id.tvName);
		this.mUiTotalScore = (TextView) root.findViewById(R.id.tvTotalScore);
		this.mUiUserIdx = (TextView) root.findViewById(R.id.tvIdx);
		this.mUiLine = root.findViewById(R.id.singsong_line);
		this.mUiListView = (ListView) root.findViewById(R.id.lv_songrecord);
		this.mUiLoading = (PngAnimView) root.findViewById(R.id.ly_waiting);
		this.mUiLoading.initPngList();
		this.mUiEmptyTip = (TextView) root.findViewById(R.id.tv_emptytip);
		
		this.mUiBack.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				getActivity().finish();
			}
		});
		this.mUiTitle.setText(this.getString(R.string.menuitem_songrecord));
		if(Home.bitmap != null)
			this.mUiUserHead.setImageBitmap(Home.bitmap);
		this.mUiUserName.setText(AppStatus.s_userMy.m_name);
		this.mUiUserIdx.setText("ID: "+AppStatus.s_userMy.m_idx);
		
		return root;
	}

	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		
		//获取最新的用户演唱总分
		GetUserInfoTask task1 = new GetUserInfoTask(mHandler,AppStatus.s_userMy.m_idx);
		task1.execute();
		
		//获取用户最新的演唱记录最近20首
		GetUserSongRecordTask task2 = new GetUserSongRecordTask(mHandler,AppStatus.s_userMy.m_idx);
		task2.execute();
	}
	
}
