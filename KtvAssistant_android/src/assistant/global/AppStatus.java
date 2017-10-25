package assistant.global;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

import tiange.ktv.assistant.R;
import android.app.Activity;
import android.app.Application;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.SystemClock;
import android.text.TextUtils;
import android.util.Log;
import assistant.activity.Home;
import assistant.activity.LoginActivity;
import assistant.activity.Start;
import assistant.entity.RoomInfo;
import assistant.entity.UserInfo;
import assistant.entity.VersionUpdate;
import assistant.manager.FaceManager;
import assistant.manager.GiftManager;
import assistant.manager.HttpPlayControlManager;
import assistant.manager.NetUtilManager;
import assistant.util.AppDirUtil;
import assistant.util.FaceUtil;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowLog;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;
import assistant.util.VersionControl;

import com.jni.netutil.LocalData_UserInfo;
import com.umeng.analytics.MobclickAgent;

/**
 * @author GLP
 */
public class AppStatus extends Application{
	//引入C底层模块
	static {
		System.loadLibrary("stlport_shared");//引入的模块无需后缀
		System.loadLibrary("JniNetUtil");
	}
	public static final Locale s_timeStyle = Locale.CHINESE;//Locale.getDefault();
	//NetUtil 模块所需要的数据
	public static final String TAG = "AppStatus";
	
	public static NetUtilManager s_NetUtilMgr = null;
	public static boolean s_isInitLocalDataBase = false;
	public static int	LobbyId = 1;

	/*
    模块内部路径：
    app 资源文件路径存放目录
    m_docDir
       |-congfig 存放配置文件,比如礼物信息xml，本地房间分组xml
       |
       |-images 存放图片资源文件,暂时想到的是礼物图片
       |
       |-update 存放app临时下载文件，可选删除下载文件，比如包含xml的zip文件
       |
       |
    */
	public static String				s_docDir = "";
	//这个roomid则指定了星光虚拟包厢ID
	//是否需要连接星光包厢，如果已经进入了ktv包厢，则必须要进入星光包厢
	public static boolean			s_isNeedConnectedRoom = false;
	public static int						s_nRoomId=0;//如果是KTV登陆用户，需要登陆指定包厢
	public static boolean 			s_isConnectedLobby = false;
	public static boolean			s_bWangTong = false;
	public static String 				s_sMacAddr;
	//星光服务器中我的信息数据
	public static LocalData_UserInfo s_userMy = null;
	
	public static FaceUtil s_faceUtil = null;
	public static AppDirUtil s_appDirUtil = null;
	public static HttpPlayControlManager s_playControMgr = null;
	public static GiftManager s_giftMgr = new GiftManager();
	public static FaceManager s_faceMgr = new FaceManager();
	public static boolean s_isBeenShowVersion = false;
	public static VersionUpdate s_newVU = null;
	public static VersionUpdate s_curVU = new VersionUpdate();
	////////////////////////////////////////////
	public static List<Activity> activityList = new ArrayList<Activity>();
	
	public static String deviceIMEI ="";
	public static boolean isInRoom = false;
	public static String ktvId = "0"; 
	public static String roomId = "0";
	public static String room_password = ""; //登录房间口令
	public static UserInfo user;
	public static int userIdx = 0;	// uid
	//这里保存了KTV实体店里面包厢信息
	public static RoomInfo currentRoomInfo;
	public static boolean s_isFromStart = false;
	public static boolean s_isBeenKilled = false;
	public static boolean s_isNeedToTopup = false;
	@Override
	public void onCreate() {
		
		Log.i("AppStatus","Begin Time "+System.currentTimeMillis());
		super.onCreate();
		s_appDirUtil = new AppDirUtil(this);
		
		//是否开启dump抓取
		CrashHandler.getInstance().init(this);
		s_NetUtilMgr = NetUtilManager.getInstance(this);
		
		deviceIMEI = ToolUtil.getIMEI(this);
		
		//初始化底层星光网络模块
		getWifiMacAddress(this);
		isWangTong("token.xg.91555.com");
		
		//启动线程处理一些逻辑
		new Thread(){

			@Override
			public void run() {
				//初始化字符串过滤
				s_NetUtilMgr.m_JniUtil.MFInitFilter();
				//检测版本更新
				VersionControl vp = new VersionControl();
				s_newVU = vp.checkApkVersion();
			}
			
		}.start();
		
		//获取友盟上自己的发送配置频率，默认启动时发送
		MobclickAgent.updateOnlineConfig( this );
		
		try {
			PackageInfo info = getApplicationContext().getPackageManager().getPackageInfo(getPackageName(), 0);
			
			s_curVU.versionCode = info.versionCode;
			s_curVU.versionName = info.versionName;
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		Log.i("AppStatus","End Time "+System.currentTimeMillis());
	}
	
	@Override
	public void onTerminate() {
		// TODO Auto-generated method stub
		super.onTerminate();
		
		s_NetUtilMgr.m_JniUtil.StopClient();
	}

	public static boolean isNeedShowNewVersion(){
		if(s_newVU == null)
			return false;
		
		//从Start启动，并且没有被系统回收的，没有显示过版本信息的，并且新版本号大于当前
		return s_isFromStart && !s_isBeenKilled && !s_isBeenShowVersion
				&& s_newVU.versionCode > s_curVU.versionCode;
	}
	
	public static boolean checkIfKilledGoStart(Activity activity){
		if(!s_isFromStart)
		{
			s_isBeenKilled = true;
			//先finish其他activity
			backToStart();
			//这里需要保留房间密码等信息
			Intent intent = new Intent(activity,Start.class);
			//intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);//这种方法是可以，但是得到的效果不大好
			activity.startActivity(intent);
		}
		return s_isFromStart;
	}
	
	public static void startClient(Context context){
		if(0 != s_NetUtilMgr.m_JniUtil.StartClient())
			ShowUtil.showToast(context,R.string.initfailed);
		
		for(int i=0;i<XGConfig.DownloadUrl.length;i++)
			s_NetUtilMgr.m_JniUtil.InitDownLoadUrl(XGConfig.DownloadUrl[i], i);
		/*
	     模块内部路径：
	     app 资源文件路径存放目录
	     docDir
	        |-congfig 存放配置文件,比如礼物信息xml，本地房间分组xml
	        |
	        |-images 存放图片资源文件,暂时想到的是礼物图片
	        |
	        |-update 存放app临时下载文件，可选删除下载文件，比如包含xml的zip文件
	        |
	        |
	     */
		s_docDir = s_appDirUtil.getAppDir();
		if(0 != s_NetUtilMgr.m_JniUtil.InitLocalDatabaseDir(s_docDir))
			ShowUtil.showToast(context,R.string.local_init_failed);
		else
			s_isInitLocalDataBase = true;
		
		//MobileNetStatUtil.getCurrentNetState(context);
	}
	public static String getLocalImage(String image_name){
		return s_docDir+"/images/"+image_name;
	}
	
	//如果wifi没有开启启动一个线程获取mac地址，保存到成员变量中
		public void getWifiMacAddress(Context context) {
			  final WifiManager wifi=(WifiManager)context.getSystemService(Context.WIFI_SERVICE);
			  if(wifi==null) return;

			  WifiInfo info=wifi.getConnectionInfo();
			  AppStatus.s_sMacAddr=info.getMacAddress();
			                
			  if(AppStatus.s_sMacAddr==null && !wifi.isWifiEnabled()) {
			    new Thread() {
			      @Override
			      public void run() {
			        wifi.setWifiEnabled(true);
			        for(int i=0;i<10;i++) {
			          WifiInfo _info=wifi.getConnectionInfo();
			          AppStatus.s_sMacAddr=_info.getMacAddress();
			          
			          if(AppStatus.s_sMacAddr!=null) 
			        	  break;
			          SystemClock.sleep(500);
			        }
			        wifi.setWifiEnabled(false);
			      }
			    }.start();
			  }
		}

		public void isWangTong(String name){
		    	
		    	int netStat = MobileNetStatUtil.getCurrentNetState(this,true);
		    	if(netStat == MobileNetStatUtil.NET_NOTCONNECT)
		    		return ;
		    	
		    	final String strName = name;
		    	new Thread() {

					@Override
					public void run() {
						// TODO Auto-generated method stub
						super.run();
						
						try
				    	{
				    		InetAddress address;
				    		address = InetAddress.getByName(strName);
				    		String strIp = address.getHostAddress();
				    		if(strIp == XGConfig.wangtong)//判断是否是网通服务器
				    			s_bWangTong = true;
				    	}
				    	catch(UnknownHostException e)
				    	{
				    		e.printStackTrace();
				    	}
					}
		    		
		    	}.start();
		}
	
	public synchronized static boolean register(Activity activity){
		return register(activity,true);
	}
	public synchronized static boolean register(Activity activity,boolean isNeedCheck) {  
		if(activityList.contains(activity))
		{
			Log.d(TAG,"register has been exsit"+activity.getClass().getSimpleName());
			return true;
		}
		activityList.add(activity);  
		
		if(isNeedCheck)//检查是否是资源被回收了
			return checkIfKilledGoStart(activity);
		
		return true;
    }
	
	/**Activity被销毁时，从Activities中移除*/  
    public synchronized static void unregister(Activity activity) {  
        if ( activityList != null && activityList.size() != 0 ) 
        {  
        	Log.d("AppStatus","unregister "+activity.getClass().getSimpleName());
        	activityList.remove(activity);  
            if (!activity.isFinishing())  
                activity.finish();  
        } 
        else  
        	Log.d("AppStatus","not register "+(activity!=null?activity.getClass().getSimpleName():""));
    }  

    public static Activity getActivityByName(String name) {
		
		for( int i = activityList.size()-1; i>=0 ;i-- ){
			Activity ac = activityList.get(i);
			if(ac.isFinishing())
				continue; 
			if (ac.getClass().getName().indexOf(name) >= 0) {
				return ac;
			}
		}
		return null;
	}
    
    public synchronized static void backToStart(){
    	for(Activity ac : activityList)
    	{
    		if ( !ac.isFinishing() ) { 
    			String className = ac.getClass().getName();
				if(TextUtils.equals(className, Start.class.getName()))
					continue;
				//finish使activity destroy，这个时候会取消在activityList中的注册的自己
				ac.finish();
			}
    	}
    }
    
    public synchronized static void backToHome(){
    	for(Activity ac : activityList)
    	{
    		if ( !ac.isFinishing() ) { 
    			String className = ac.getClass().getName();
				if(TextUtils.equals(className, LoginActivity.class.getName())
						|| TextUtils.equals(className, Home.class.getName()))
					continue;
				ac.finish();
			}
    	}
    }
    
    //清除房间登录密码信息，防止Home create的时候再去自定进入房间
    public synchronized static void backToLogin(){
    	AppStatus.room_password = "";
    	for(Activity ac : activityList)
    	{
    		if ( !ac.isFinishing() ) { 
				if(ac.getClass().getName().equals(LoginActivity.class.getName()))
					continue;
				ac.finish();
			}
    	}
    }
    /**
	 * 退出程序
	 * @param con
	 */
	public synchronized static void exitApp(Context con){
		//结束activity队列中的所有activity
		if( activityList !=null){
			for (Activity ac : activityList) {
				if ( !ac.isFinishing() ) { 
					ac.finish();
				}
			}
		}
		//清除通知栏
		((NotificationManager)con.getSystemService(android.content.Context.NOTIFICATION_SERVICE)).cancelAll();
		android.os.Process.killProcess(android.os.Process.myPid());
	}

	public static void showCurrentOpenActivity(){
		for( int i = activityList.size()-1; i>=0 ;i-- ){
			Activity ac = activityList.get(i);
			if(ac.isFinishing())
				continue; 
			ShowLog.i("AppStatus", "showCurrentOpenActivity ac = "+ac.getClass().getName());
		}
	}

	public static boolean isAleadyLogin(){
		if(user != null && userIdx != 0)
			return true;
		
		return false;
	}
}
