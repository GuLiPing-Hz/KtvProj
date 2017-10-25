package assistant.fragment;

import tiange.ktv.assistant.R;
import android.content.Intent;
import android.os.Bundle;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ExpandableListView;
import android.widget.TextView;
import assistant.activity.MoneyBagRedPacketActivity;
import assistant.callback.OnOptionClickListener;
import assistant.fragment.adapter.OnlineAdapter;
import assistant.global.AppStatus;
import assistant.manager.NetUtilManager.INetUtilListener;

import com.jni.netutil.LocalData_UserInfo;
import com.jni.netutil.XGMessage;

public class PhoneOnlineUserFragment extends BaseFragment implements OnOptionClickListener
	,INetUtilListener
{
	View m_viewSend;
	EditText m_editTextHorn;
	TextView m_textViewTitle;
	String m_msg;
	
	View m_viewRoot;
	OnlineAdapter mAdapter;
	
	public static final int RETURN_SEND_PRIVATE_LETTER = 1;
	public static final int RETURN_SEND_GIFT = 2;
	
	public static final String RETURN_DATA_KEY_USERID = "SELECT_ONLINE_USR_ID";
	public static final String RETURN_DATA_KEY_USERNAME = "SELECT_ONLINE_USR_NAME";
	public static final String RESULTCODE_USERINFO = "SELECT_ONLINE_USR_INFO_OBJ";
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,Bundle savedInstanceState)
	{
		// TODO Auto-generated method stub
		View root = inflater.inflate(R.layout.fragment_interactive_chat_onine_member, null);
		m_viewRoot = root;
	
		m_textViewTitle = (TextView) root.findViewById(R.id.tv_title);
		m_textViewTitle.setText("在线人数");
		root.findViewById(R.id.iv_back).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				getActivity().finish();
			}
		});
		
		mAdapter = new OnlineAdapter(getActivity(),this);
        ExpandableListView myExpandableListView = (ExpandableListView)root.findViewById(R.id.current_room_member_list);
        myExpandableListView.setAdapter(mAdapter);
        for(int i=0;i<mAdapter.mDataLists.size();i++)
        	myExpandableListView.expandGroup(i);
		
        // 注册到网络协议监听
     	AppStatus.s_NetUtilMgr.registerListener(this);
		return root;
	}
	
	@Override
	public void onDestroyView() {
		// TODO Auto-generated method stub
		AppStatus.s_NetUtilMgr.unregisterListener(this);
		
		super.onDestroyView();
	}



	@Override
	public void onGiftSendObjectClick(LocalData_UserInfo info) {
		// TODO Auto-generated method stub
		Intent resultIntent = new Intent();
		resultIntent.putExtra(RESULTCODE_USERINFO, info);
		getActivity().setResult(RETURN_SEND_GIFT, resultIntent);
		getActivity().finish();
	}

	@Override
	public void onTalkClick(LocalData_UserInfo info) {
		// TODO Auto-generated method stub
		Intent resultIntent = new Intent();
		resultIntent.putExtra(RESULTCODE_USERINFO, info);
		getActivity().setResult(RETURN_SEND_PRIVATE_LETTER, resultIntent);
		getActivity().finish();
	}

	@Override
	public void onUserInfoClick() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onHongBao(LocalData_UserInfo info) {
		Intent intent = new Intent(getActivity(), MoneyBagRedPacketActivity.class);
		intent.putExtra(HongBaoSendFragment.EXTRA_HONGBAO_TYPE
				, HongBaoSendFragment.HONGBAO_PERSON);
		intent.putExtra(HongBaoSendFragment.EXTRA_HONGBAO_IDX, info.m_idx);
		startActivity(intent);
		getActivity().finish();
	}

	@Override
	public void dealWithMsg(Message msg) {
		// TODO Auto-generated method stub
		switch(msg.what)
		{
			// 更新在线用户数量
			case XGMessage.XGMSG_USER_LOGIN://更新在线状态
			case XGMessage.XGMSG_USER_LOGOUT:
			case XGMessage.XGMSG_USERINFO_NOTIFY:
			case XGMessage.XGMSG_USERPIC_UPDATE:
			case XGMessage.XGMSG_USERNAME_UPDATE:
				if(mAdapter != null)
					mAdapter.notifyDataSetChanged();
				break;
		}
	}
}
