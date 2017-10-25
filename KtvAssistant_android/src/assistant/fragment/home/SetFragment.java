package assistant.fragment.home;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.RelativeLayout;
import android.widget.TextView;
import assistant.activity.AboutPageActivity;
import assistant.activity.Home;
import assistant.activity.LoginActivity;
import assistant.activity.SimpleRecommendApp;
import assistant.fragment.BaseFragment;
import assistant.global.AppStatus;
import assistant.global.MessageDef;
import assistant.manager.DialogManager;
import assistant.task.userapi.ThirdLogoutTask;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowLog;
import assistant.util.ShowUtil;

import com.umeng.analytics.MobclickAgent;

/**
 * @author 宋静
 * @version 创建时间：2013-12-12 上午11:31:45 类说明 设置
 */
public class SetFragment extends BaseFragment {
	//private Home mHome;
	private View vRoot;
	public boolean isLoading = false;
	View ly_waiting;

	/* ================ 复写系统方法 =============== */
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		ShowLog.i(TAG, "onCreateView.");
		vRoot = inflater.inflate(R.layout.ac_more, container, false);
		initView();
		return vRoot;
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
	}

	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onActivityCreated(savedInstanceState);
	}

	private RelativeLayout m_about, m_recommend;
	private ImageButton vBack;
	private TextView btn_login;
	private final static int REQ_LOGIN = 8000;

	private void initView() {
		//mHome = (Home) this.getActivity();
		findView();
		setView();
		setListener();
	}

	private void findView() {
		TextView tv = (TextView) vRoot.findViewById(R.id.tv_title);
		tv.setText(R.string.menuitem_setting);
		vBack = (ImageButton) vRoot.findViewById(R.id.iv_back);
		m_about = (RelativeLayout) vRoot
				.findViewById(R.id.more_menu_about_layout);
		
		m_recommend = (RelativeLayout) vRoot
				.findViewById(R.id.more_menu_recommend_layout);
		btn_login = (TextView) vRoot.findViewById(R.id.iv_logout);
		ly_waiting = (View) vRoot.findViewById(R.id.ly_waiting);
	}

	private void setView() {
		//vBack.setImageResource(R.drawable.btn_home_menu);
		if (AppStatus.isAleadyLogin()) {
			btn_login.setText(getString(R.string.more_login_text));
		} else {
			btn_login.setText(getString(R.string.more_login_empty_text));
		}
	}

	private void setListener() {
		vBack.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				//ToolUtil.closeKeyBoard(getActivity());
				//mHome.showMenu();
				getActivity().finish();
			}
		});

		// 关于
		m_about.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent intent_about = new Intent();
				intent_about.setClass(getActivity(), AboutPageActivity.class);
				startActivity(intent_about);
			}
		});

		// 应用推荐
		m_recommend.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				MobclickAgent.onEvent(getActivity(), "RecommendBtn");
				Intent intent = new Intent();
				intent.setClass(getActivity(), SimpleRecommendApp.class);
				startActivity(intent);
			}
		});

		// 登陆与退出登录
		btn_login.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				if (AppStatus.isAleadyLogin()) {
					
					DialogManager.showTipDialog(getActivity(), getString(R.string.account_quit)
							, getString(R.string.account_quit_hint)
							, new DialogManager.OnDialogListener() {
								
								@Override
								public void onOk(int type) {
									// TODO Auto-generated method stub
									if (checkNet()) {
										ThirdLogoutTask thirdLogoutTask = new ThirdLogoutTask(
												handler, AppStatus.userIdx,AppStatus.roomId);
										thirdLogoutTask.execute();
										
										Home.enterRoomManager.startExitRoom(true);
									}
								}
								
								@Override
								public void onCancel() {
									// TODO Auto-generated method stub
									
								}
							}, true);
				} 
//				else {
//					Intent i = new Intent(SetFragment.this.getActivity(),
//							LoginActivity.class);
//					mHome.startActivityForResult(i, REQ_LOGIN);
//				}
			}
		});
	}

	@SuppressLint("HandlerLeak") 
	public Handler handler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			try {
				if (isLoading) {
					ly_waiting.setVisibility(View.GONE);
					isLoading = false;
				}
				switch (msg.what) 
				{
					case MessageDef.WM_THIRD_LOGOUT:
						ShowUtil.showToast(getActivity(),
								R.string.account_quit_message);
						
						AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectLobby();
						
						//需要回调到登陆界面
						Context context = getActivity().getApplicationContext();
						Intent intent = new Intent(context,LoginActivity.class);
						intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
						AppStatus.backToLogin();
						context.startActivity(intent);
						break;
				}
			} catch (Exception e) {
				// TODO: handle exception
				ShowLog.showException(e);
			}
		}
	};

	private boolean checkNet() {
		if (MobileNetStatUtil.unNetConnectionTips(SetFragment.this.getActivity())) {
			ShowUtil.showToast(this.getActivity(), R.string.net_none);
			return false;
		}
		return true;
	}
}
