package assistant.activity;

import java.util.ArrayList;
import java.util.List;

import order.show.DotView;
import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
import android.net.ConnectivityManager;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.OnPageChangeListener;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewTreeObserver;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.Animation;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.TextView;
import assistant.activity.home.ChargeActivity;
import assistant.activity.home.GiftKtvListActivity;
import assistant.activity.home.SetActivity;
import assistant.activity.home.SingedSongActivity;
import assistant.activity.home.UserAccountActivity;
import assistant.adapter.HomeMenuAdapter;
import assistant.dialog.VersionUpdateDialog;
import assistant.entity.Banner;
import assistant.fragment.BaseFragment;
import assistant.fragment.BaseFragmentHelper;
import assistant.fragment.home.ChargeFragment;
import assistant.fragment.home.ChatFragment;
import assistant.fragment.home.GiftKtvListFragment;
import assistant.fragment.home.MainHomeFragment;
import assistant.fragment.home.RoomOrderFragment;
import assistant.fragment.home.SetFragment;
import assistant.fragment.home.UserAccountFrag;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.global.RequestCodeDef;
import assistant.manager.DialogManager;
import assistant.manager.EnterRoomManager;
import assistant.manager.HomeMenuItemManager;
import assistant.manager.HttpPlayControlManager;
import assistant.manager.NetUtilManager.INetUtilListener;
import assistant.manager.OnRoomListener;
import assistant.pager.fragment.PlayControlBaseFragment;
import assistant.pager.fragment.PlayControlExFragment;
import assistant.share.LoginManager;
import assistant.util.ImageUtil;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowLog;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;
import assistant.util.VersionControl;
import assistant.view.Rotate3dAnimation;
import assistant.view.RoundedImageView;

import com.jni.netutil.JniMessage;
import com.jni.netutil.ResultData_RecentSong;
import com.jni.netutil.XGMessage;
import com.sliding.SlidingActivity;
import com.sliding.SlidingMenu;
import com.sliding.SlidingMenu.OnOpenListener;
import com.umeng.analytics.MobclickAgent;

/**
 * @author 宋静
 * @version 创建时间：2013-11-29 下午5:10:35 类说明 1.1版以后的主页，带侧拉的菜单
 */
public class Home extends SlidingActivity implements INetUtilListener{
	
	private final static String TAG = "Home";
	private static final String OUTSTATE_ROOM_PWD = "assistant.activity.Home.roompwd";
	
	public final static int MSG_VERSION = 1;
	boolean m_isFromStart = true;
	
	public static ArrayList<Banner> bannerList = new ArrayList<Banner>();
	
	private long exitTime = 0;
	/* ================界面绘制=============== */
	private View vMiddle;
	private View vLeft;
	private FragmentManager fragmentManager;
	private FragmentTransaction fragmentTransaction;
	private DisplayMetrics screenDpx; // 屏幕分辨率
	public BaseFragmentHelper baseFragmentHelper;
	//View m_ViewBottom;
	
	/* ================左侧菜单=============== */
	public RoundedImageView ivHeadPhoto;
	public TextView tvUserName, tvUserCash, tvIdx;
	TextView mTextViewRecentSong;
	private View vUserinfo;
	TextView m_textViewSet;
	private ListView lvMenu;
	private HomeMenuAdapter menuAdapter;
	private int selection = HomeMenuItemManager.MENU_MAIN_INDEX;
	private int[] menuUserLayoutParams = new int[] { 362, 181 };
	public LoginManager loginUtil;
	private final static int REQ_LOGIN = 6000;
	
	/* =============头像旋转模块============== */
	private static int whichImg = 0;// 判断是哪个图片
	private static final int[] pic = { R.drawable.ic_user,
			R.drawable.ic_present };

	private boolean hasMeasured;// 是否测量过头像
	// 初始化头像高宽
	private int fmHeight = 101;
	private int fmWidth = 101;
	public boolean isRotation = false;
	
	/* ================下方的房间信息=============== */
	private View btnEnterRoom, btnExitRoom;
	ImageButton m_imgBtnPlayControl;
	private TextView tvRoomInfo;
	public static EnterRoomManager enterRoomManager;
	//private boolean isOverdueing = false;
	
	View m_viewContent;
	View m_viewPlayControlPanel;
	View m_viewRoomInfo;
	ViewPager m_viewPagerPopPC;
	DotView m_dotViewPC;
	boolean m_isPlayControlShow;
	
	/* =============上方的用户信息============== */
	private final static int FRESH_ICON_IMAGE = 44000; // 下载图片后更新头像
	public static Bitmap bitmap;// 获取下载的头像
	
	BroadcastReceiver m_NetworkStateRecver = new BroadcastReceiver(){

		@Override
		public void onReceive(Context arg0, Intent arg1) {
			// TODO Auto-generated method stub
			if(MobileNetStatUtil.getCurrentNetState(Home.this) > 0)
			{
				//如果网络连接，并且之前没有检测过最新版本
				if(AppStatus.s_newVU  == null)
				{
					//启动线程检查apk更新
					new Thread(){
		
						@Override
						public void run() {
							VersionControl vp = new VersionControl();
							AppStatus.s_newVU = vp.checkApkVersion();
							//通知到主UI线程
							m_versionHandler.sendEmptyMessage(MSG_VERSION);
						}
						
					}.start();
				}
				//如果还没有初始化完成本地数据库，则在开启网络的时候初始化一下
				if(!AppStatus.s_isInitLocalDataBase)
				{
					Log.i(TAG,"Init LocalDataBase");
					if(0 ==AppStatus.s_NetUtilMgr.m_JniUtil.InitLocalDatabaseDir(AppStatus.s_docDir))
						AppStatus.s_isInitLocalDataBase = true;
				}
			}
		}
		
	};
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		ShowLog.d(TAG, "onCreate");
		super.onCreate(savedInstanceState);
		
		if(savedInstanceState != null)
			AppStatus.room_password = savedInstanceState.getString(OUTSTATE_ROOM_PWD);
		
		AppStatus.register(this);// 将房间Activity加入activityList
		
		init();
		
		m_isFromStart = AppStatus.s_isFromStart;
		if(!m_isFromStart)
			return ;
		
		checkSdDialog();
		//监听包厢的状态
		AppStatus.s_NetUtilMgr.registerListener(this);
		//注册网络监听
		IntentFilter filter = new IntentFilter(); 
		filter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
		registerReceiver(m_NetworkStateRecver, filter);
		
		if(AppStatus.isNeedShowNewVersion())
		{
			VersionUpdateDialog dialog = new VersionUpdateDialog(this,R.style.EnterRoom);
			dialog.show();
			AppStatus.s_isBeenShowVersion = true;
		}
		
		//静默登陆包厢
		if(!TextUtils.isEmpty(AppStatus.room_password))
			enterRoomManager.startLoginRoom(AppStatus.room_password, true);
	}

	@Override
	public void onLowMemory() {
		// TODO Auto-generated method stub
		super.onLowMemory();
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		// TODO Auto-generated method stub
		super.onSaveInstanceState(outState);
		outState.putString(OUTSTATE_ROOM_PWD, AppStatus.room_password);
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		ShowLog.d(TAG, "onDestroy");
		
		if(m_isFromStart)
		{
			//取消监听
			unregisterReceiver(m_NetworkStateRecver); 
			//注销监听包厢的状态
			AppStatus.s_NetUtilMgr.unregisterListener(this);
		}
		AppStatus.unregister(this);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if ((keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0))
		{

			if (baseFragmentHelper != null && baseFragmentHelper.onKeyDown(keyCode, event))
			{
				switchMenu(HomeMenuItemManager.MENU_MAIN_INDEX);
				return true;
			}
			
			if( selection != HomeMenuItemManager.MENU_MAIN_INDEX )
			{
				selection = HomeMenuItemManager.MENU_MAIN_INDEX;
				switchMenu(HomeMenuItemManager.MENU_MAIN_INDEX);
				return true;
			}
			
			if ((System.currentTimeMillis() - exitTime) > 2000)
			{
				ShowUtil.showToast(this, R.string.exit_tips);
				exitTime = System.currentTimeMillis();
			} 
			else if ((System.currentTimeMillis() - exitTime) > 0)
			{
				ToolUtil.closeKeyBoard(this);
				AppStatus.exitApp(this);
			}
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		
		if(AppStatus.s_isNeedToTopup)
		{
			startActivity(new Intent(this,ChargeActivity.class));
			AppStatus.s_isNeedToTopup = false;
		}
		
		showContent(false);
		ShowLog.d(TAG, "onResume");
		MobclickAgent.onResume(this);
		refreshUserInfo();
		entryRoomState(AppStatus.isInRoom);
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		// whichImg = 0;
		ShowLog.d(TAG, "onPause");
		MobclickAgent.onPause(this);
		// Online.onPause(this);
	}

	@Override
	protected void onNewIntent(Intent intent) {
		// TODO Auto-generated method stub
		super.onNewIntent(intent);
		ShowLog.d(TAG, "onNewIntent");
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);

		if(requestCode == RequestCodeDef.REQUEST_SCAN_CODE
				|| requestCode == RequestCodeDef.REQUEST_PWD_ENTER)
		{
			enterRoomManager.activityResult(requestCode, resultCode, data);
		}
	}

	OnOpenListener menuOpenListener = new OnOpenListener(){

		@Override
		public void onOpen() {
			refreshUserInfo();
		}};

	/** 初始化 */
	private void init() {
		//侧拉菜单
		vLeft = getLayoutInflater().inflate(R.layout.fm_menu, null);
		setBehindContentView(vLeft);
		//右边显示内容
		vMiddle = getLayoutInflater().inflate(R.layout.fm_content, null);
		setContentView(vMiddle);
		//初始化
		initSliding();
		initMenu();
		initBottom();
		setListener();
	}

	/** 初始化侧滑设置 */
	private void initSliding() {
		screenDpx = getResources().getDisplayMetrics(); // 取屏幕分辨率
		menuUserLayoutParams[0] = screenDpx.widthPixels * menuUserLayoutParams[0] / 480;

		SlidingMenu sm = getSlidingMenu();
		sm.setShadowWidth(0);
		// sm.setShadowDrawable(R.drawable.shadow);
		sm.setBehindOffset(screenDpx.widthPixels - menuUserLayoutParams[0]);
		// sm.setBehindOffsetRes(R.dimen.home_shadow);
		sm.setFadeEnabled(true);
		sm.setFadeDegree(0.35f);
		// 设置slding menu的几种手势模式,TOUCHMODE_FULLSCREEN 全屏模式,TOUCHMODE_MARGIN
		// 边缘模式,TOUCHMODE_NONE 无手势
		sm.setTouchModeAbove(SlidingMenu.TOUCHMODE_FULLSCREEN);
		sm.setMode(SlidingMenu.LEFT);
	}

	/** 监听器 */
	private void setListener() {
		// 个人信息点击
		vUserinfo.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				if (AppStatus.isAleadyLogin())
				{
					//switchMenu(HomeMenuItemManager.MENU_MAIN_INDEX);
					Intent intent = new Intent(Home.this,UserAccountActivity.class);
					startActivity(intent);
				}
//				else 
//				{
//					Intent i = new Intent(Home.this, LoginActivity.class);
//					Home.this.startActivityForResult(i, REQ_LOGIN);
//				}
			}
		});

		// 左侧列表点击
		lvMenu.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
					long arg3) {
				if (arg2 >= 0)
					switchMenu(arg2);
			}
		});
		
//		m_textViewSet.setOnClickListener(new View.OnClickListener() {
//			
//			@Override
//			public void onClick(View arg0) {
//				switchMenu(HomeMenuItemManager.MENU_SET);
//			}
//		});

		btnEnterRoom.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				if (enterRoomManager != null) {
					enterRoomManager.showEnterRoomMenuDialog(false);
				}
			}
		});

		btnExitRoom.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				if (enterRoomManager != null)
					enterRoomManager.showExitRoomDialog();
			}
		});

		m_imgBtnPlayControl.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				switchPlayCView();
			}
		});
	}

	/** 初始化左侧菜单界面 */
	private void initMenu() {
		initUserInfo();

		//m_textViewSet = (TextView)vLeft.findViewById(R.id.textViewSet); 
		lvMenu = (ListView) vLeft.findViewById(R.id.lv_menu);
		menuAdapter = new HomeMenuAdapter(this);
		lvMenu.setAdapter(menuAdapter);

		switchMenu(selection);
		
		this.setOnLeftOpenListener(menuOpenListener);
	}

	/** 菜单选择 */
	public void switchMenu(int index) {
		baseFragmentHelper = null;
		//不会切换主页
		//selection = index;
		switch (index) {
		case HomeMenuItemManager.MENU_MAIN_INDEX:         //默认主页
			openFragment(new MainHomeFragment(), index);
			break;
		case HomeMenuItemManager.MENU_ACCOUNT_RECHARGE:   //账户充值
		{
			//openFragment(new ChargeFragment(),index);
			Intent i = new Intent(this, ChargeActivity.class);
			startActivity(i);
			break;
		}
		case HomeMenuItemManager.MENU_GIFT_EXCHANGE:      //礼物兑换
		{
			Intent i = new Intent(this, GiftKtvListActivity.class);
			startActivity(i);
			break;
		}
//		case HomeMenuItemManager.MENU_CONSUME_RECORD:      //消费记录
//			Toast.makeText(getApplicationContext(), "我是消费记录", Toast.LENGTH_SHORT).show();
//			break;
		case HomeMenuItemManager.MENU_SONG_RECORD:         //唱过的歌
		{
			Intent i = new Intent(this,SingedSongActivity.class);
			startActivity(i);
			break;
		}
		case HomeMenuItemManager.MENU_SONGS://歌曲收藏
		{
			Intent i = new Intent(this,SongCollectActivity.class);
			startActivity(i);
			break;
		}
		case HomeMenuItemManager.MENU_SETTINGS:            //设置
		{
			//openFragment(new SetFragment(), index);
			Intent i = new Intent(this, SetActivity.class);
			startActivity(i);
			break;
		}
		default:
			selection = HomeMenuItemManager.MENU_MAIN_INDEX;
			break;
		}
	}

	/** 点击菜单的选项，弹出相对应的页面 */
	private void openFragment(BaseFragment frag, int index) {
		if (frag == null)
			return;
		//this.clearIgnoredAboveViews(); ??
		if (fragmentManager == null)
			fragmentManager = getSupportFragmentManager();

		fragmentTransaction = fragmentManager.beginTransaction();
		fragmentTransaction.replace(R.id.content, frag);
		fragmentTransaction.commit();
		showContent();
		//通知菜单栏选择项更新
		notifyMenuSelected(index);
	}

	private void notifyMenuSelected(int index) {
		if (menuAdapter != null) {
			menuAdapter.setSelection(index);
		}
	}

	private void applyRotation(float start, float end) {

		// 计算中心点
		float centerX = fmWidth / 2.0f;
		float centerY = fmHeight / 2.0f;

		ShowLog.v(TAG, "applyRotation fmWidth = " + fmWidth + ", fmHeight = "
				+ fmHeight);

		Rotate3dAnimation rotation = new Rotate3dAnimation(start, end, centerX,
				centerY, 0.0f, true);
		rotation.setDuration(1000);
		rotation.setFillAfter(true);
		rotation.setInterpolator(new AccelerateInterpolator());
		rotation.setAnimationListener(displayNextView);
		ivHeadPhoto.startAnimation(rotation);
	}

	private void refreshUserInfo() {
		if (this.isMenuShowing()) {
			whichImg = 0;
			ivHeadPhoto.setBorder(0);
			if(!isRotation){
				isRotation = true;
				applyRotation(0, 90);
			}
		}

		tvUserName.setText(AppStatus.user.nickname);
		tvUserCash.setText("" + AppStatus.user.gold);
		if(Home.bitmap != null)
			ivHeadPhoto.setImageBitmap(Home.bitmap);
		updateSongInfo();
	}
	
	void updateSongInfo(){
		if(AppStatus.s_userMy != null && !TextUtils.equals(AppStatus.s_userMy.singername,"0"))
		{
			String songInfo = String.format("%s  %s %.1f分", AppStatus.s_userMy.singername
					,AppStatus.s_userMy.songname,AppStatus.s_userMy.score/10.0f);
			mTextViewRecentSong.setText(songInfo);
		}
	}

	private Animation.AnimationListener displayNextView = new Animation.AnimationListener() {

		public void onAnimationStart(Animation animation) {

			if (AppStatus.user != null) {
				ivHeadPhoto.clearAnimation();
				whichImg = 3;
				return;
			}
		}

		// 动画结束
		public void onAnimationEnd(Animation animation) {
			if (AppStatus.user != null) {
				ivHeadPhoto.clearAnimation();
				whichImg = 3;
				return;
			}

			if (0 == whichImg) {
				ivHeadPhoto.setImageResource(pic[++whichImg]);
				ivHeadPhoto.setBorder(0);
				applyRotation(270, 360);

			} else if (1 == whichImg) {
				m_userHandler.postDelayed(swapViewsRunnable, 500);
				whichImg++;
			} else if (2 == whichImg) {
				ivHeadPhoto.setImageResource(pic[0]);
				ivHeadPhoto.setBorder(0);
				applyRotation(270, 360);
				whichImg++;
			} else {
				ivHeadPhoto.clearAnimation();
			}
		}

		public void onAnimationRepeat(Animation animation) {

		}
	};

	// 用于是、翻转剩下的90度
	private Runnable swapViewsRunnable = new Runnable() {

		public void run() {
			applyRotation(0, 90);
		}
	};

	/** 菜单上的用户名称初始化 */
	private void initUserInfo() {
		vUserinfo = vLeft.findViewById(R.id.userinfo_layout);
		ivHeadPhoto = (RoundedImageView) vLeft.findViewById(R.id.iv_userhead);
		tvUserName = (TextView) vLeft.findViewById(R.id.tv_username);
		tvIdx = (TextView) vLeft.findViewById(R.id.tv_idx);
		tvUserCash = (TextView) vLeft.findViewById(R.id.tv_usercash);
		mTextViewRecentSong = (TextView) vLeft.findViewById(R.id.tv_singInfo);

		ViewTreeObserver vto = ivHeadPhoto.getViewTreeObserver();
		vto.addOnPreDrawListener(new ViewTreeObserver.OnPreDrawListener() {

			@Override
			public boolean onPreDraw() {
				// TODO Auto-generated method stub
				if (false == hasMeasured) {
					fmHeight = ivHeadPhoto.getHeight();
					fmWidth = ivHeadPhoto.getWidth();
					hasMeasured = true;
				}
				return true;
			}
		});

		reforlogin();
	}

	//用于在侧拉菜单的切换页,清除Fragment
	public void clearFragmentInMem(){
		try//catch由于内存低，导致应用被强制关闭，fragment已经被销毁的情况
		{
			//如果已经finished，则不用销毁fragment
			if(!isFinishing())
			{
				//去掉在内存中的Fragment,
				FragmentManager fm = getSupportFragmentManager();
				FragmentTransaction ft = fm.beginTransaction();
				int size = fm.getFragments().size();
				for(int i=0;i<size;i++)
				{
					Fragment fragment = fm.getFragments().get(i);
					//排除常驻fragment
					if(fragment == null
							|| PlayControlBaseFragment.class.isInstance(fragment)//排除播放控制第一页
							|| PlayControlExFragment.class.isInstance(fragment)//排除播放控制第二页
							|| ChargeFragment.class.isInstance(fragment)//排除购买金币
							|| ChatFragment.class.isInstance(fragment)//排除聊天
							|| MainHomeFragment.class.isInstance(fragment)//排除主页
							|| RoomOrderFragment.class.isInstance(fragment)//排除包厢预定
							|| SetFragment.class.isInstance(fragment)//排除设置
							|| UserAccountFrag.class.isInstance(fragment)//排除个人中心
							|| GiftKtvListFragment.class.isInstance(fragment))//排除兑换礼物
						continue;
					
					ft.remove(fragment);
				}
				ft.commit();
			}
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
	}
	
	public void showMiddle(boolean show){
		if(show)
		{
			m_viewPlayControlPanel.setVisibility(View.VISIBLE);
			m_viewRoomInfo.setVisibility(View.VISIBLE);
		}
		else
		{
			m_viewPlayControlPanel.setVisibility(View.GONE);
			m_viewRoomInfo.setVisibility(View.GONE);
		}
	}

	public void hidePlayControl(){
		if(m_isPlayControlShow)//如果正在显示
			switchPlayCView();
	}
	public void updateGold(int gold){
		if(AppStatus.user != null)
		{
			AppStatus.user.gold = gold;
			tvUserCash.setText("" + AppStatus.user.gold);
		}
	}
	/** 登陆成功后用户资料界面的刷新 */
	public void reforlogin() {
		if(AppStatus.user == null)
			return ;
		Drawable left = this.getResources().getDrawable(
				AppStatus.user.gender==1?R.drawable.icon_sex2_man:R.drawable.icon_sex2_woman);
		left.setBounds(0, 0, left.getMinimumWidth(), left.getMinimumHeight());
		tvUserName.setCompoundDrawables(left, null, null, null);
		tvIdx.setText("ID: "+AppStatus.userIdx);
		
		if (!TextUtils.isEmpty(AppStatus.user.headphoto)) {
			ImageUtil.showPhotoImage(Home.this, m_userHandler, AppStatus.user.headphoto,FRESH_ICON_IMAGE);
		}
	}

	@SuppressLint("HandlerLeak")
	private Handler m_versionHandler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			switch(msg.what)
			{
			case MSG_VERSION:
			{
				if(AppStatus.isNeedShowNewVersion())
				{
					VersionUpdateDialog dialog = new VersionUpdateDialog(Home.this,R.style.EnterRoom);
					dialog.show();
					AppStatus.s_isBeenShowVersion = true;
				}
				break;
			}
			}
		}
		
	};
	@SuppressLint("HandlerLeak")
	private Handler m_userHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			try {
				switch (msg.what) {
				case FRESH_ICON_IMAGE:
					// 头像下载成功
					bitmap = ImageUtil.getBitmapPhoto(AppStatus.user.headphoto);
					if (bitmap != null) {
						ivHeadPhoto.setImageBitmap(bitmap);
						ivHeadPhoto.setBorder(2);
					}
					break;
				default:
					// 失败
					ShowUtil.showLongToast(Home.this, (String) msg.obj);
					break;
				}
			} catch (Exception e) {
				ShowLog.showException(e);
			}
		}

	};

	void switchPlayCView(){
		m_isPlayControlShow = !m_isPlayControlShow;
		if(m_isPlayControlShow)
		{
			m_imgBtnPlayControl.setImageResource(R.drawable.btn_panel_open_1);
			m_viewPlayControlPanel.setVisibility(View.VISIBLE);
		}
		else
		{
			m_imgBtnPlayControl.setImageResource(R.drawable.btn_panel_open);
			m_viewPlayControlPanel.setVisibility(View.GONE);
		}
	}
	void initBottom(){
		AppStatus.s_playControMgr = new HttpPlayControlManager(this,m_playcHandler);
		
		m_isPlayControlShow = false;
		m_viewContent = vMiddle.findViewById(R.id.content);
		m_viewPlayControlPanel = vMiddle.findViewById(R.id.include_play_control);
		m_viewRoomInfo = vMiddle.findViewById(R.id.roominfo_bottom);
		m_viewPagerPopPC = (ViewPager) vMiddle.findViewById(R.id.viewPagerPlayControl);
		m_dotViewPC = (DotView) vMiddle.findViewById(R.id.dotViewPlayControl);
		
		//阻止对viewpager的拦截滑动
		addIgnoredAboveView(m_viewPagerPopPC);
		
		FragmentManager fm = getSupportFragmentManager();
		//设置播放控制面板
		m_viewPagerPopPC.setAdapter(new FragmentPagerAdapter(fm){

			@Override
			public Fragment getItem(int arg0) {
				// TODO Auto-generated method stub
				if(arg0 == 0)
					return new PlayControlBaseFragment();
				else if(arg0 == 1)
					return new PlayControlExFragment();
				return null;
			}

			@Override
			public int getCount() {
				// TODO Auto-generated method stub
				return 2;//base , ex
			}
			
		});
		
		m_viewContent.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				hidePlayControl();
			}
		});
		m_dotViewPC.setDotCount(2);
		m_viewPagerPopPC.setOnPageChangeListener(new OnPageChangeListener(){

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
				m_dotViewPC.setSelection(arg0);
			}
			
		});
		
		m_imgBtnPlayControl = (ImageButton) vMiddle.findViewById(R.id.imgBtnPlayControl);
		btnEnterRoom = vMiddle.findViewById(R.id.tv_enterroom);
		tvRoomInfo = (TextView) vMiddle.findViewById(R.id.tv_roominfo);
		btnExitRoom = vMiddle.findViewById(R.id.iv_exitroom);

		enterRoomManager = new EnterRoomManager(this, roomListener);
	}

	boolean m_isSlientDo = false;
	private OnRoomListener roomListener = new OnRoomListener() {
		@Override
		public void EnterRoomSuccess() {
			if(!enterRoomManager.m_isSlientEnterRoom)
				ShowUtil.showToast(Home.this, R.string.login_success);
			entryRoomState(true);
		}

		@Override
		public void EnterRoomFail(String msg) {
			if (!TextUtils.isEmpty(msg))
				ShowUtil.showToast(Home.this, msg);
			else
				ShowUtil.showToast(Home.this, R.string.login_fail);
		}

		@Override
		public void ExitRoomSuccess() {
			if(!enterRoomManager.m_isSlientExitRoom)
			{
				ShowUtil.showToast(Home.this, R.string.exit_room_success);
				entryRoomState(false);
			}
		}

		@Override
		public void ExitRoomFail(String msg) {
			if (!TextUtils.isEmpty(msg))
				ShowUtil.showToast(Home.this, msg);
			else
				ShowUtil.showToast(Home.this, R.string.exit_room_fail);
		}
	};

	public void entryRoomState(boolean isEntryRoom) {
		if (isEntryRoom) {
			btnEnterRoom.setVisibility(View.GONE);
			tvRoomInfo.setVisibility(View.VISIBLE);
			tvRoomInfo.setText(getString(R.string.room_info,AppStatus.currentRoomInfo.roomName));
			btnExitRoom.setVisibility(View.VISIBLE);
			m_imgBtnPlayControl.setVisibility(View.VISIBLE);
		} else {
			btnEnterRoom.setVisibility(View.VISIBLE);
			tvRoomInfo.setVisibility(View.GONE);
			btnExitRoom.setVisibility(View.GONE);
			m_imgBtnPlayControl.setVisibility(View.GONE);
			hidePlayControl();
		}

		//首页需要刷新一下
		if (selection == HomeMenuItemManager.MENU_MAIN_INDEX) {
			BaseFragment fragment = (BaseFragment) fragmentManager.findFragmentById(R.id.content);
			fragment.refreshView();
		}
	}

	@SuppressLint("HandlerLeak")
	protected Handler m_playcHandler = new Handler(){
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			try {
				switch (msg.what) {
				//http 播放控制设置的回调 to be continue
				case MessageDef.HTTP_VOICE_PAD:
					break;
				case MessageDef.HTTP_PAUSE_OO:
					break;
				case MessageDef.HTTP_MUTE_OO:
					break;
				case MessageDef.HTTP_SING_MODE:
					break;
				case MessageDef.HTTP_SCORE_STAT:
					break;
				case MessageDef.HTTP_ATMOSPHERE:
					break;
				case MessageDef.WM_CHANGE_SONG:
					break;
				//不同的错误码
				case KtvAssistantAPIConfig.APIErrorCode.RoomClosed: // 房间关闭
				case KtvAssistantAPIConfig.APIErrorCode.RoomRepair: // 房间维护
				case KtvAssistantAPIConfig.APIErrorCode.NoRoom: // 房间不存在
					//忽略
					break;
				default:
						ShowUtil.showToast(Home.this, Home.this.getString(R.string.operation_failed)+msg.obj.toString());
				}
			}
			catch (Exception e) {
				e.printStackTrace();
			}
		}
	};

	/** 包厢过期提示 */
	private void RoomOverdueTips(boolean isTimeout) {
		//isOverdueing = true;
		entryRoomState(false);
		List<Activity> activityList = AppStatus.activityList;
		activityList.remove(this);

		if (!activityList.isEmpty()) {
			for (Activity ac : activityList)
			{
				if (!ac.isFinishing()) 
					ac.finish();
			}
		}

		activityList.clear();
		activityList.add(this);

		String text;

		if (isTimeout)
			text = getString(R.string.room_tip_content1);
		else
			text = getString(R.string.room_tip_content2);

		DialogManager.showTipDialog(this, getString(R.string.room_tip_title), text, null, false);
	}

	private void checkSdDialog() {
		if (android.os.Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED)) {
			return;
		}

		DialogManager.showTipDialog(this, getString(R.string.no_sd_title)
				, getString(R.string.no_sd_content), null, false);
	}

	@Override
	public void dealWithMsg(Message msg) {
		// TODO Auto-generated method stub
		switch(msg.what)
		{
		case XGMessage.XGMSG_LOBBYLOGIN_SUCCESS:
		{
			updateSongInfo();
			break;
		}
		case XGMessage.XGMSG_USERPIC_UPDATE:
		{
			if(Home.bitmap != null)
				ivHeadPhoto.setImageBitmap(Home.bitmap);
			break;
		}
		case XGMessage.XGMSG_USERNAME_UPDATE:
		{
			tvUserName.setText(AppStatus.user.nickname);
			break;
		}
		case XGMessage.XGMSG_SINGSONG_RECENT:
		{
			ResultData_RecentSong info = (ResultData_RecentSong)msg.obj;
			if(info != null)
			{
				AppStatus.s_userMy.singername = info.singername;
				AppStatus.s_userMy.songname = info.songname;
				AppStatus.s_userMy.score = info.score;
			}
			updateSongInfo();
			break;
		}
		case XGMessage.XGMSG_HORNRESULT_NOTIFY:
		case XGMessage.XGMSG_GIFTRESULT_NOTIFY:
		case XGMessage.XGMSG_USERGOLD_UPDATE:
		{
			if(AppStatus.user != null)
				tvUserCash.setText("" + AppStatus.user.gold);
			break;
		}
		case XGMessage.XGMSG_ROOMSTAT_UPDATE:
		{
			//Log.i("Glp","XGMessage.XGMSG_ROOMSTAT_UPDATE "+msg.arg1);
			//包厢关闭
			if(msg.arg1 == 0)
			{
				//静默退出包厢
				enterRoomManager.startExitRoom(true);
				RoomOverdueTips(true);
			}
			break;
		}
		case XGMessage.XGMSG_ROOMENTER_FAILED:
		case XGMessage.XGMSG_ROOMCON_CLOSE:
		{
			if(msg.arg2 != JniMessage.OPTION_TIMEOUT)
			{
				//房间服务器主动断开连接
				//静默退出包厢
				enterRoomManager.startExitRoom(true);
				RoomOverdueTips(false);
			}
			break;
		}
		}
	}
}
