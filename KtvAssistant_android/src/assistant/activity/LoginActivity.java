package assistant.activity;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.global.RequestCodeDef;
import assistant.manager.NetUtilManager;
import assistant.manager.NetUtilManager.INetUtilListener;
import assistant.share.LoginManager;
import assistant.task.userapi.LoginTask;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;

import com.jni.netutil.XGMessage;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;


/**
 * @author 宋静
 * @version 创建时间：2013-8-15 下午4:43:17 类说明 登录界面
 */
public class LoginActivity extends BaseActivity implements IWXAPIEventHandler,INetUtilListener{
	private View vBack;
	private View vSina, vQQ , vWeixin;
	private LoginManager loginUtil;
	
	TextView  m_textviewRegister;
	EditText   m_edittextAccount;
	EditText   m_edittextPwd;
	View		 m_viewLogin;
	
	@SuppressLint("HandlerLeak")
	private Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			
			switch (msg.what) 
			{
				case MessageDef.WM_LOGIN_RESULT:
				{
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
						doEnterXingGuang();
					else
					{
						loginUtil.stopDialog();
						ShowUtil.showToast(LoginActivity.this, "登陆失败:"+(String)msg.obj);
					}
					break;
				}
				/* 登录成功 */
				case MessageDef.WM_THIRD_LOGIN_RESULT:
				{
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
						doEnterXingGuang();
					else
					{
						loginUtil.stopDialog();
						ShowUtil.showToast(LoginActivity.this, "登陆失败:"+(String)msg.obj);
					}
					break;
				}
				/* 登录失败 */
				case MessageDef.WM_LOGIN_FAIL:
				{
					loginUtil.stopDialog();
					ShowUtil.showToast(LoginActivity.this, R.string.login_fail);
					break;
				}
				/* 登录取消 */
				case MessageDef.WM_LOGIN_CANCEL:
				{
					loginUtil.stopDialog();
					ShowUtil.showToast(LoginActivity.this, R.string.login_cancel);
					break;
				}
			}
		}
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		m_isNeedCheck = false;//no need check
		super.onCreate(savedInstanceState);
		
		Home.bitmap = null;
		setContentView(R.layout.ac_login);
		init();

		AppStatus.s_NetUtilMgr.registerListener(this);
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		
		AppStatus.s_NetUtilMgr.unregisterListener(this);
	}

	/** 初始化 */
	private void init() {
		((TextView) findViewById(R.id.tv_title)).setText(R.string.third_login);
		vBack = findViewById(R.id.iv_back);
		vSina = findViewById(R.id.tv_sina);
		vQQ = findViewById(R.id.tv_qq);
		vWeixin = findViewById(R.id.tv_weixin);
		m_textviewRegister = (TextView) findViewById(R.id.tv_giftlog);
		m_edittextAccount = (EditText) findViewById(R.id.edittext_account);
		m_edittextPwd = (EditText) findViewById(R.id.edittext_pwd);
		m_viewLogin = findViewById(R.id.textview_login);
		
		//检测到用户名改变，清空密码输入框
		m_edittextAccount.addTextChangedListener(new TextWatcher(){

			@Override
			public void afterTextChanged(Editable s) {
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
				m_edittextPwd.setText("");
			}
			
		});
		
		if(AppStatus.user != null && !TextUtils.isEmpty(AppStatus.user.xgId) 
				&& !TextUtils.isEmpty(AppStatus.user.xgPwd))
		{
			if(AppStatus.user.accountType == 3)
			{//如果是自己注册的就显示
				m_edittextAccount.setText(AppStatus.user.xgId);
				m_edittextPwd.setText(AppStatus.user.xgPwd);
			}
		}
		
		m_textviewRegister.setVisibility(View.VISIBLE);
		m_textviewRegister.setText(getString(R.string.txt_register));

		loginUtil = new LoginManager(this, handler);
		setListener();
	}

	private void setListener() {
		vBack.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				AppStatus.exitApp(LoginActivity.this);
			}
		});

		vSina.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				loginUtil.sinaLogin();
			}
		});
		
		vWeixin.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				loginUtil.weixinLogin(LoginActivity.this);
			}
		});

		vQQ.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				loginUtil.qqLogin();
			}
		});
		
		m_textviewRegister.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				Intent intent = new Intent(LoginActivity.this,RegisterActivity.class);
				startActivityForResult(intent, RequestCodeDef.REQUEST_REGISTER_USER);
			}
		});
		
		m_viewLogin.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				ToolUtil.closeKeyBoard(LoginActivity.this);
				
				String account = m_edittextAccount.getText().toString();
				if(TextUtils.isEmpty(account))
				{
					ShowUtil.showToast(LoginActivity.this, R.string.account_tip);
					return ;
				}
				String pwd = m_edittextPwd.getText().toString();
				if(TextUtils.isEmpty(pwd))
				{
					ShowUtil.showToast(LoginActivity.this, R.string.pwd_tip);
					return ;
				}
				
				loginUtil.initDialog();
				
				LoginTask task = new LoginTask(handler,account,pwd);
				task.execute();
			}
		});
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if(keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0)
		{
			vBack.performClick();
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}

	//微信授权页面返回
	@Override
	protected void onNewIntent(Intent intent) {
		// TODO Auto-generated method stub
		super.onNewIntent(intent);
		
		loginUtil.handleIntent(intent);
	}

	//QQ，微博授权页面返回
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		
		//注册页面回调
		if(requestCode == RequestCodeDef.REQUEST_REGISTER_USER)
		{
			if(resultCode == Activity.RESULT_OK)//如果注册成功，直接进入
			{
				loginUtil.initDialog();
				doEnterXingGuang();
			}
			return ;
		}
		
		if (loginUtil != null)
			loginUtil.authorizeCallBack(requestCode, resultCode, data);
		else
			ShowUtil.showToast(this, R.string.soft_error);
	}

	void doEnterXingGuang(){
		//登陆界面登陆成功
		//->连接认证服务器
		AppStatus.s_NetUtilMgr.doCurrentWork(NetUtilManager.TransactionLobbyLogin);
	}

	@Override
	public void onReq(BaseReq arg0) {
		// TODO Auto-generated method stub
		//do nothing
		//微信请求我们的应用
	}

	@Override
	public void onResp(BaseResp arg0) {
		// TODO Auto-generated method stub
		loginUtil.dealWithWeixinAuth(arg0);
	}

	@Override
	public void dealWithMsg(Message msg) {
		// TODO Auto-generated method stub
		switch(msg.what)
		{
		case XGMessage.XGMSG_AUTHCON_TIMEOUT:
		case XGMessage.XGMSG_AUTHCON_ERROR:
		case XGMessage.XGMSG_AUTHCON_FAILED:
		case XGMessage.XGMSG_AUTHLOGIN_FAILED:
		case XGMessage.XGMSG_LOBBYCON_FAILED:
		case XGMessage.XGMSG_LOBBYCON_TIMEOUT:
		case XGMessage.XGMSG_LOBBYCON_ERROR:
		case XGMessage.XGMSG_LOBBYLOGIN_FAILED:
			loginUtil.stopDialog();
			break;
		case XGMessage.XGMSG_LOBBYSECTION_SUCCESS://获取大厅列表成功 - 一般来说这个应该会成功
			if(!NetUtilManager.IsLoginLobbyAfterList)
			{
				loginUtil.stopDialog();
				NetUtilManager.IsLoginLobbyAfterList = true;
			}
			break;
//		case XGMessage.XGMSG_AUTHLOGIN_SUCCESS:
		case XGMessage.XGMSG_LOBBYLOGIN_SUCCESS:
			loginUtil.stopDialog();
			//进入主界面
			ShowUtil.showToast(LoginActivity.this, R.string.login_success);
			startActivity(new Intent(this,Home.class));
			finish();
			break;
		}
	}
}
