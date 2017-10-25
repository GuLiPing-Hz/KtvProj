package assistant.service;

import java.io.File;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.view.View;
import android.widget.RemoteViews;
import assistant.global.AppStatus;
import assistant.util.DownloadUtils;
import assistant.util.ShowUtil;

public class AppUpgradeService extends Service {

	 	public static final int APP_VERSION_LATEST = 0;
	    public static final int APP_VERSION_OLDER = 1;
	    
	    //下载地址
	    public final static String EXTRA_DOWNLOAD_URL = "assistant.service.AppUpgradeService.downloadurl";
	    //新版本号
	    public final static String EXTRA_NEWVERSION = "assistant.service.AppUpgradeService.newversion";

	    // Unique Identification Number for the Notification.
	    // We use it on Notification start, and to cancel it.
	    public static final int NOTIFICATION = R.id.appupgrade_service;
	    
	    String m_newVersion;
	    String mDownloadUrl = null;
	    NotificationManager mNotificationManager = null;
	    Notification mNotification = null;
	    PendingIntent mPendingIntent = null;
	    File destFile = null;

	    private static final int DOWNLOAD_FAIL = -1;
	    private static final int DOWNLOAD_SUCCESS = 0;
	    @SuppressLint("HandlerLeak")
		private Handler mHandler = new Handler(){

	        @Override
	        public void handleMessage(Message msg) {
	            switch (msg.what) {
	            case DOWNLOAD_SUCCESS:
	            	ShowUtil.showToast(getApplicationContext(), R.string.app_upgrade_download_sucess);
	                install(destFile);
	                break;
	            case DOWNLOAD_FAIL:
	            	ShowUtil.showToast(getApplicationContext(), R.string.app_upgrade_download_fail);
	                mNotificationManager.cancel(NOTIFICATION);
	                break;
	            default:
	                break;
	            }
	        }

	    };
	    private DownloadUtils.DownloadListener downloadListener = 
	    		new DownloadUtils.DownloadListener() {
	        @Override
	        public void downloading(int progress) {
	            mNotification.contentView.setProgressBar(R.id.app_upgrade_progressbar, 100, progress, false);
	            mNotification.contentView.setTextViewText(R.id.textViewNotifyTip, "正在下载: "+progress + "%");
	            mNotificationManager.notify(NOTIFICATION, mNotification);
	        }

	        @Override
	        public void downloaded() {
	            mNotification.contentView.setViewVisibility(R.id.app_upgrade_progressbar, View.GONE);
	            mNotification.defaults = Notification.DEFAULT_SOUND;
	            mNotification.contentIntent = mPendingIntent;
	            mNotification.contentView.setTextViewText(R.id.textViewNotifyTip, "下载完成.");
	            mNotificationManager.notify(NOTIFICATION, mNotification);
	            if (destFile.exists() && destFile.isFile() && checkApkFile(destFile.getPath())) {
	                Message msg = mHandler.obtainMessage();
	                msg.what = DOWNLOAD_SUCCESS;
	                mHandler.sendMessage(msg);
	            }
	            mNotificationManager.cancel(NOTIFICATION);
	        }
	    };
	    
	    @Override
		public void onCreate() {
			// TODO Auto-generated method stub
			super.onCreate();
			
			mNotificationManager = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
		}

		@Override
		public void onDestroy() {
			// TODO Auto-generated method stub
			super.onDestroy();
			
			mNotificationManager.cancel(NOTIFICATION);
		}

		@Override
	    public IBinder onBind(Intent intent) {
	        return null;
	    }

	    @Override
	    public int onStartCommand(Intent intent, int flags, int startId) {
	        mDownloadUrl = intent.getStringExtra(EXTRA_DOWNLOAD_URL);
	        m_newVersion = intent.getStringExtra(EXTRA_NEWVERSION);

	       //已经下载完毕
	        String apkName = mDownloadUrl.substring(mDownloadUrl.lastIndexOf("/"));
	        destFile = new File(AppStatus.s_appDirUtil.getUpgradeDir() + apkName);
            if (destFile.exists() && destFile.isFile() && checkApkFile(destFile.getPath())) {
                install(destFile);
                stopSelf();
                return super.onStartCommand(intent, flags, startId);
            }

            // Set the icon, scrolling text and timestamp 自定义RemoteView
	        mNotification = new Notification();
	        mNotification.contentView = new RemoteViews(getApplication().getPackageName()
	        		,R.layout.notification_app_upgrade);

	        // The PendingIntent to launch our activity if the user selects this notification
	        Intent completingIntent = new Intent();
	        completingIntent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
	        completingIntent.setClass(getApplicationContext(), AppUpgradeService.class);

	        mPendingIntent = PendingIntent.getActivity(this, R.string.app_name
	        		,completingIntent, PendingIntent.FLAG_UPDATE_CURRENT);

	        mNotification.icon = R.drawable.icon_notify;
	        mNotification.tickerText = "开始下载";
	        mNotification.contentIntent = mPendingIntent;
	        mNotification.contentView.setTextViewText(R.id.textViewVersion, m_newVersion);
	        mNotification.contentView.setTextViewText(R.id.textViewNotifyTip, "正在下载: 0%");
	        mNotification.contentView.setProgressBar(R.id.app_upgrade_progressbar, 100, 0, false);
	        mNotificationManager.cancel(NOTIFICATION);//取消原来的通知
	        
	        // Set the info for the views that show in the notification panel.
	        //mNotification.setLatestEventInfo(this, getText(R.string.local_service_label),text, contentIntent);
	        mNotificationManager.notify(NOTIFICATION, mNotification);
	        new AppUpgradeThread().start();
	        
	        return START_STICKY;
	    }

	    class AppUpgradeThread extends Thread{

	        @Override
	        public void run() {
	        	
	            if (destFile.exists() && destFile.isFile() && checkApkFile(destFile.getPath())) {
	                install(destFile);
	            }
	            else
	            {
	            	try {
                        DownloadUtils.download(mDownloadUrl, destFile, false, downloadListener);
                    } 
	            	catch (Exception e) 
	            	{
                        Message msg = mHandler.obtainMessage();
                        msg.what = DOWNLOAD_FAIL;
                        mHandler.sendMessage(msg);
                        e.printStackTrace();
                    }
	            }
	            stopSelf();
	        }
	    }

	    public boolean checkApkFile(String apkFilePath) {
	        boolean result = false;
	        try{
	            PackageManager pManager = getPackageManager();
	            PackageInfo pInfo = pManager.getPackageArchiveInfo(apkFilePath
	            		,PackageManager.GET_ACTIVITIES);
	            if (pInfo == null) {
	                result =  false;
	            } else {
	                result =  true;
	            }
	        } catch(Exception e) {
	            result =  false;
	            e.printStackTrace();
	        }
	        return result;
	    }

	    public void install(File apkFile){
	        Uri uri = Uri.fromFile(apkFile);
	        Intent intent = new Intent(Intent.ACTION_VIEW);
	        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	        intent.setDataAndType(uri, "application/vnd.android.package-archive");
	        startActivity(intent);
	    }
	    public void uninstall(String packageName){
	    	Uri uri = Uri.parse("package:" + packageName);
	    	Intent deleteIntent = new Intent();
	    	deleteIntent.setAction(Intent.ACTION_DELETE);
	    	deleteIntent.setData(uri);
	    	startActivity(deleteIntent);
	    }
}
