package assistant.activity;

import order.show.DotView;
import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.AnimationUtils;
import assistant.adapter.StartAdapter;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.manager.NetUtilManager;
import assistant.manager.NetUtilManager.INetUtilListener;
import assistant.share.LoginManager;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowLog;
import assistant.util.ShowUtil;

import com.jni.netutil.XGMessage;

/**
 * 小米手机会保存应用的图标，所以如果应用更改图标后，需要重启手机才能看到图标的更新
 *
 */
public class Start extends BaseActivity implements INetUtilListener{

	static final int NO_CACHE_USERINFO = 0;
	static final int VERSION_UPDATE_RELOGIN=1;
	static final int NO_NETWORK = 2;
	static final String TAG = "Start";
	
	boolean m_isFirstStart;
	
	View m_viewFirst;
	View m_viewSecond;
	ViewPager m_viewPagerStart;
	DotView m_dotView;
	View m_BtnKtvStart;
	Animation m_animation;
	StartAdapter m_adapterStart;
	@Override
	public void onCreate(Bundle savedInstanceState) {
		Log.i("Start","Begin Time "+System.currentTimeMillis());
		//启动登陆的入口不需要检查是否被资源回收
		m_isNeedCheck = false;
		super.onCreate(savedInstanceState);
		setContentView(R.layout.ac_start);
		
		
		SharedPreferences preferences = getSharedPreferences("phone", Context.MODE_PRIVATE);
		
		//主要是首次登陆的引导页，非美术人士弄的，所以去掉了。
	    //判断是不是首次登录，
		m_isFirstStart = false;//preferences.getBoolean("firststart", true);
		if (m_isFirstStart)
		{
			Editor editor = preferences.edit();
			//将登录标志位设置为false，下次登录时不在显示首次登录界面
			editor.putBoolean("firststart", false);
			editor.commit();
		}
	   
		findView();
		init();
		AppStatus.s_NetUtilMgr.registerListener(this);
		Log.i("Start","End Time "+System.currentTimeMillis());
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		AppStatus.s_NetUtilMgr.unregisterListener(this);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		//getMenuInflater().inflate(R.menu.start, menu);
		return true;
	}

	void findView(){
		m_animation = AnimationUtils.loadAnimation(Start.this, R.anim.start_appear);
		m_animation.setAnimationListener(new AnimationListener(){

			@Override
			public void onAnimationEnd(Animation arg0) {
				// TODO Auto-generated method stub
				m_BtnKtvStart.setVisibility(View.VISIBLE);
			}

			@Override
			public void onAnimationRepeat(Animation arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onAnimationStart(Animation arg0) {
				// TODO Auto-generated method stub
			}
			
		});
		
		m_viewFirst = findViewById(R.id.first_start);
		m_viewSecond = findViewById(R.id.second_start);
		m_viewPagerStart = (ViewPager) findViewById(R.id.viewPagerStart);
		m_dotView = (DotView) findViewById(R.id.dotViewStart);
		m_BtnKtvStart = findViewById(R.id.textViewKtvstart);
		
		m_BtnKtvStart.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				doAutoLogin();
			}
		});
		
		m_adapterStart = new StartAdapter(this);
		m_viewPagerStart.setAdapter(m_adapterStart);
		
		m_viewPagerStart.setOnPageChangeListener(new ViewPager.OnPageChangeListener(){

			@Override
			public void onPageScrollStateChanged(int arg0) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onPageScrolled(int arg0, float arg1, int arg2) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onPageSelected(int arg0) {
				// TODO Auto-generated method stub
				m_dotView.setSelection(arg0);
				if(arg0 == (StartAdapter.TotalPage-1))
				{
					//显示按钮
					uiHandler.post(new Runnable(){

						@Override
						public void run() {
							// TODO Auto-generated method stub
							m_BtnKtvStart.startAnimation(m_animation);
						}
						
					});
					
				}
				else
					m_BtnKtvStart.setVisibility(View.GONE);
			}
			
		});
	}
	
	void doAutoLogin(){
		//如果已经登陆-这里在Start的launchMode 为singleTask的时候有用
		if(AppStatus.isAleadyLogin())
		{
			uiHandler.post(new Runnable(){

				@Override
				public void run() {
					// TODO Auto-generated method stub
					startActivity(new Intent(Start.this, Home.class));
				}
				
			});
		}
		else
		{
			//如果自动登陆失败,则一秒之后去登陆界面
			long millis = m_isFirstStart?0:1000;
			
			if(MobileNetStatUtil.checkCurrentNetState(this,true))
			{
				//自动登录
				LoginManager login = new LoginManager(this, uiHandler);
				int ret = login.autoLogin();
				if(ret == -1)//需要重新登陆
					uiHandler.sendEmptyMessageDelayed(NO_CACHE_USERINFO,millis);
				else if(ret == -2)//版本升级，重新登陆
					uiHandler.sendEmptyMessageDelayed(VERSION_UPDATE_RELOGIN,millis);
			}
			else
				uiHandler.sendEmptyMessageDelayed(NO_NETWORK, millis);
		}
	}
	
	private void init(){
		if(m_isFirstStart)
		{
			m_viewFirst.setVisibility(View.VISIBLE);
			m_viewSecond.setVisibility(View.GONE);
		}
		else
		{
			doAutoLogin();
		}
	}
	
	@SuppressLint("HandlerLeak")
	private Handler uiHandler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			
			//初始化底层
			AppStatus.startClient(Start.this);
			AppStatus.s_isFromStart = true;
			
			super.handleMessage(msg);
			try{
				switch (msg.what) {
				case MessageDef.WM_LOGIN_RESULT:
				case MessageDef.WM_AUTOLOGIN_SUCCESS:
					
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
					{
						//自动登陆完成
						//开始登陆星光大厅
						ShowLog.i(TAG,"开始登陆星光大厅");
						//->连接认证服务器
						AppStatus.s_NetUtilMgr.doCurrentWork(NetUtilManager.TransactionLobbyLogin);
						//startActivity(new Intent(Start.this, Home.class));
					}
					else
					{
						ShowUtil.showLongToast(Start.this, "自动登陆失败:"+(String)msg.obj);
						startActivity(new Intent(Start.this, LoginActivity.class));
					}
					break;
				default:
					//默认需要重新登录才能使用
					if(msg.what == NO_CACHE_USERINFO)
					{
						//ShowUtil.showLongToast(Start.this, R.string.please_login);
					}
					else if(msg.what == VERSION_UPDATE_RELOGIN)
						ShowUtil.showLongToast(Start.this, R.string.version_relogin);
					else
						ShowUtil.showLongToast(Start.this, R.string.start_net_noconnetc);
					startActivity(new Intent(Start.this, LoginActivity.class));
					break;
				}
			}catch (Exception e) {
				e.printStackTrace();
			}
		}
	};
	
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
			startActivity(new Intent(Start.this, LoginActivity.class));
			break;
		case XGMessage.XGMSG_LOBBYSECTION_SUCCESS://获取大厅列表成功 - 一般来说这个应该会成功
			if(!NetUtilManager.IsLoginLobbyAfterList)
			{
				startActivity(new Intent(Start.this, LoginActivity.class));
				NetUtilManager.IsLoginLobbyAfterList = true;
			}
			break;
//		case XGMessage.XGMSG_AUTHLOGIN_SUCCESS:
		case XGMessage.XGMSG_LOBBYLOGIN_SUCCESS:
			//进入主界面
			ShowUtil.showToast(Start.this, R.string.login_success);
			startActivity(new Intent(this,Home.class));
			finish();
			break;
		}
	}
}
