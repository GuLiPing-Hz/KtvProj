package assistant.fragment;

import tiange.ktv.assistant.R;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.TextView;
import assistant.global.AppStatus;
import assistant.util.FaceUtil;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;

public class SendHornFragment extends BaseFragment {

	View m_viewSend;
	View m_viewBack;
	EditText m_editTextHorn;
	TextView m_textViewTitle;
	
	String m_msg;
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		View root = inflater.inflate(R.layout.fragment_send_horn, null);
		
		m_textViewTitle = (TextView) root.findViewById(R.id.tv_title);
		m_viewBack = root.findViewById(R.id.iv_back);
		m_editTextHorn = (EditText) root.findViewById(R.id.editTextHorn);
		m_viewSend = root.findViewById(R.id.textViewSend);
		
		m_editTextHorn.addTextChangedListener(new TextWatcher(){

			@Override
			public void afterTextChanged(Editable arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onTextChanged(CharSequence s, int start, int before,
					int count) {
				// TODO Auto-generated method stub
				if (before == 0)// 说明是字符增加，大于0说明是减少
				{// 字符增加的时候判断下
					CharSequence seq = s.subSequence(start, start + count);
					String newStr = seq.toString();
					if (FaceUtil.containsEmoji(newStr)) {
						m_editTextHorn.getText().delete(start, start + count);
						m_editTextHorn.setSelection(start);
						ShowUtil.showToast(getActivity(), R.string.sorry_emoji);
					}
					else if(TextUtils.equals(newStr, "\n"))
					{
						int index = -1;
						int enterCount = -1;
						do
						{
							index++;
							enterCount++;
							index = s.toString().indexOf("\n",index);
						}while(index != -1);
						
						if(enterCount > 5)
						{
							m_editTextHorn.getText().delete(start, start + count);
							m_editTextHorn.setSelection(start);
							ShowUtil.showToast(getActivity(), R.string.sorry_enter);
						}
					}
				}
			}
			
		});
		
		m_viewBack.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				ToolUtil.closeKeyBoard(getActivity());
				getActivity().finish();
			}
		});
		
		m_viewSend.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if(AppStatus.user == null)
					return ;
				
				//喇叭免费，不再检查金币数
//				if(AppStatus.user.gold < 2000)
//				{
//					DialogManager.showTopUpDialog(getActivity(),new OnDialogListener(){
//
//						@Override
//						public void onOk(int type) {
//							// TODO Auto-generated method stub
//							AppStatus.s_isNeedToTopup = true;
//						}
//
//						@Override
//						public void onCancel() {
//							// TODO Auto-generated method stub
//							
//						}
//						
//					});
//					return ;
//				}
				
				String msg = m_editTextHorn.getText().toString();
				if(TextUtils.isEmpty(msg))
						ShowUtil.showToast(getActivity(), R.string.horn_send_empty);
				else
				{
					if(AppStatus.s_isConnectedLobby)
					{
						String finalMsg = AppStatus.s_NetUtilMgr.m_JniUtil.MFFilter(msg);
						if(0 != AppStatus.s_NetUtilMgr.m_JniUtil.SendSpeakerToLobby(AppStatus.LobbyId, finalMsg))
							ShowUtil.showToast(getActivity(), R.string.horn_send_failed);
						//发送成功之后，清空喇叭字符串
						else
						{
							m_editTextHorn.getText().clear();
							m_viewBack.performClick();
						}
							
					}
					else
						ShowUtil.showToast(getActivity(), R.string.no_networking);
				}
			}
		});
		m_textViewTitle.setText(R.string.chat_send_horn);
		
		return root;
	}
}
