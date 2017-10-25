package assistant.fragment;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.TextView;
import assistant.dialog.LoadingDialog;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.manager.DialogManager;
import assistant.task.userapi.RegisterTask;
import assistant.util.ShowUtil;

public class RegisterFragment extends BaseFragment implements View.OnClickListener{

	//public static final String ACCOUNT = "assistant.fragment.RegisterFragment.account";
	//public static final String PASSWORD= "assistant.fragment.RegisterFragment.pwd";
	//public static final String SEX = "assistant.fragment.RegisterFragment.sex";
	
	View m_viewBack;
	TextView m_textviewTitle;
	EditText m_editAccount;
	EditText m_editPwd;
	View m_viewMale;
	View m_viewSelectMale;
	View m_viewFemale;
	View m_viewSelectFemale;
	View m_viewRegister;
	
	int m_sex = -1;//0:女；1:男
	
	LoadingDialog m_dialog;
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		View root = inflater.inflate(R.layout.fragment_register, null);
		
		m_viewBack = root.findViewById(R.id.iv_back);
		m_textviewTitle = (TextView) root.findViewById(R.id.tv_title);
		m_editAccount = (EditText) root.findViewById(R.id.edittext_account);
		m_editPwd = (EditText) root.findViewById(R.id.edittext_pwd);
		m_viewMale = root.findViewById(R.id.framelayout_male);
		m_viewSelectMale = root.findViewById(R.id.imgview_male);
		m_viewFemale = root.findViewById(R.id.framelayout_female);
		m_viewSelectFemale = root.findViewById(R.id.imgview_female);
		m_viewRegister = root.findViewById(R.id.textview_register);
		
		m_viewBack.setOnClickListener(this);
		m_viewMale.setOnClickListener(this);
		m_viewFemale.setOnClickListener(this);
		m_viewRegister.setOnClickListener(this);
		
		m_dialog = new LoadingDialog(getActivity(),"注册中");
		m_textviewTitle.setText(getActivity().getString(R.string.txt_register));
		
		getActivity().setResult(Activity.RESULT_CANCELED);
		return root;
	}
	
	private boolean checkSpecil(String str) {
		Pattern pattern = Pattern.compile("^[A-Za-z]+\\w*$");
		Matcher matcher = pattern.matcher(str);
		boolean flag = matcher.matches();
		if (flag) {
			return true;
		} else {
			return false;
		}
	}
	
	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		switch(arg0.getId())
		{
		case R.id.iv_back:
			getActivity().finish();
			break;
		case R.id.framelayout_male:
			m_sex = 1;
			m_viewSelectFemale.setVisibility(View.INVISIBLE);
			m_viewSelectMale.setVisibility(View.VISIBLE);
			break;
		case R.id.framelayout_female:
			m_sex = 0;
			m_viewSelectMale.setVisibility(View.INVISIBLE);
			m_viewSelectFemale.setVisibility(View.VISIBLE);
			break;
		case R.id.textview_register:
			
			String strAccount = m_editAccount.getText().toString();
			if(strAccount.length() == 0)
			{
				ShowUtil.showToast(getActivity(), R.string.register_account_empty);
				return ;
			}
			else if(strAccount.length()<5 || strAccount.length()>10)
			{
				ShowUtil.showToast(getActivity(), R.string.register_account_size_illgel);
				m_editAccount.setText("");
				return ;
			}
			
			if(!checkSpecil(strAccount))
			{
				ShowUtil.showToast(getActivity()	, R.string.register_zhenze);
				m_editAccount.setText("");
				return ;
			}
			
			String strPwd = m_editPwd.getText().toString();
			if(strPwd.length() == 0)
			{
				ShowUtil.showToast(getActivity(), R.string.register_pwd_empty);
				return ;
			}
			if(strPwd.length() < 6)
			{
				ShowUtil.showToast(getActivity(), R.string.register_pwd_size_illgel);
				return ;
			}
			
			if(m_sex == -1)
			{
				ShowUtil.showToast(getActivity(), R.string.register_sex_no);
				return ;
			}
			
			m_dialog.show();
			
			RegisterTask task = new RegisterTask(m_handler,strAccount,strPwd,m_sex);
			task.execute();
			break;
		}
	}

	@SuppressLint("HandlerLeak")
	Handler m_handler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			
			if(m_dialog.isShowing())
				m_dialog.dismiss();
			switch(msg.what)
			{
				case MessageDef.WM_REGISTER_SUCCESS:
				{
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
					{
						DialogManager.showTipDialog(getActivity(), getString(R.string.txt_register)
								, getString(R.string.register_tip_ok), new DialogManager.OnDialogListener() {
									
									@Override
									public void onOk(int type) {
										// TODO Auto-generated method stub
										getActivity().setResult(Activity.RESULT_OK);
										getActivity().finish();
									}
									
									@Override
									public void onCancel() {
										// TODO Auto-generated method stub
										
									}
								}, false);
					}
					else
						ShowUtil.showToast(getActivity(), ((String)msg.obj));
					break;
				}
			}
		}
		
	};
}
