package assistant.global;

import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.io.Writer;
import java.lang.Thread.UncaughtExceptionHandler;
import java.lang.reflect.Field;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Properties;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.app.ActivityManager;
import android.app.NotificationManager;
import android.content.Context;
import android.os.Build;
import android.os.Handler;
import assistant.util.ShowLog;
import assistant.util.ShowUtil;

import com.umeng.analytics.MobclickAgent;

/**
 * @author 宋静
 * @version 创建时间：2013-7-11 下午5:41:03
 * 类说明 崩溃日志收集
 */
public class CrashHandler  implements UncaughtExceptionHandler {

	NotificationManager m_NotificationManager;
	ActivityManager a;
	/** Debug Log tag */
	public static final String TAG = "CrashHandler";

	/** 系统默认的UncaughtException处理类 */
	private Thread.UncaughtExceptionHandler mDefaultHandler;
	/** CrashHandler实例 */
	private static CrashHandler INSTANCE;
	/** 程序的Context对象 */
	private Context mContext;
	/** 文件目录 */
	private File m_fileDir;
	private Handler m_hMsgHandler = new Handler();

	/** 使用Properties来保存设备的信息和错误堆栈信息 */
	private Properties mDeviceCrashInfo = new Properties();
	private static final String VERSION_NAME = "versionName";
	private static final String STACK_TRACE = "STACK_TRACE";
	/** 错误报告文件的扩展名 */
	private static final String CRASH_REPORTER_EXTENSION = ".txt";
	/** 保证只有一个CrashHandler实例 */
	private CrashHandler() {
	}

	/** 获取CrashHandler实例 ,单例模式 */
	public static CrashHandler getInstance() {
		if (INSTANCE == null) {
			INSTANCE = new CrashHandler();
		}
		return INSTANCE;
	}

	/**
	 * 初始化,注册Context对象, 获取系统默认的UncaughtException处理器, 设置该CrashHandler为程序的默认处理器
	 * 
	 * @param ctx
	 */
	public void init(Context ctx) {
		mContext = ctx;
		mDefaultHandler = Thread.getDefaultUncaughtExceptionHandler();
		Thread.setDefaultUncaughtExceptionHandler(this);
		m_fileDir = AppStatus.s_appDirUtil.getDumpDirFile();
	}

	/**
	 * 当UncaughtException发生时会转入该函数来处理
	 */
	@Override
	public void uncaughtException(Thread thread, Throwable ex) {
		if (!handleException(ex) && mDefaultHandler != null) {
			// 如果用户没有处理则让系统默认的异常处理器来处理
			mDefaultHandler.uncaughtException(thread, ex);
			System.exit(0);
		} else {
			// Sleep一会后结束程序
			try {
				m_NotificationManager = (NotificationManager) mContext
						.getSystemService(android.content.Context.NOTIFICATION_SERVICE);
				m_NotificationManager.cancelAll();
				Thread.sleep(3000);
				// m_hMsgHandler.sendMessage(m_hMsgHandler.obtainMessage(GlobalDef.EX_OCCUR,
				// 0, 0, 0));
			} catch (InterruptedException e) {
				ShowLog.showException(e);
			}

			exitClient(mContext);
		}
	}

	/**
	 * 自定义错误处理,收集错误信息 发送错误报告等操作均在此完成. 开发者可以根据自己的情况来自定义异常处理逻辑
	 * 
	 * @param ex
	 * @return true:如果处理了该异常信息;否则返回false
	 */
	private boolean handleException(Throwable ex) {
		if (ex == null) {
			return true;
		}
		// final String msg = ex.getLocalizedMessage();
		// 使用Toast来显示出错信息
		m_hMsgHandler.post(new Runnable(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				ShowUtil.showLongToast(mContext,R.string.soft_error);
			}
			
		});

		// 收集设备信息
		collectCrashDeviceInfo(mContext);
		// 保存错误报告文件
		saveCrashInfoToFile(ex);
		// 发送错误报告到服务器
		sendCrashReportsToServer();
		return true;
	}

	/**
	 * 在程序启动时候, 可以调用该函数来发送以前没有发送的报告
	 */
	public void sendPreviousReportsToServer() {
		File dumpFile = AppStatus.s_appDirUtil.getDumpDirFile();
		
		if (dumpFile.exists()) {
			File crFile[] = dumpFile.listFiles();
			if (crFile != null) {
				File cr;
				for (int i = 0; i < crFile.length; i++) {
					cr = crFile[i];
					if(postReport(cr))
							cr.delete();
				}
			}
		}
	}

	/**
	 * 把错误报告发送给服务器,新产生的错误报告.
	 * 
	 * @param ctx
	 */
	private void sendCrashReportsToServer() {
		try {
			if (m_fileDir.exists()) {
				File crFile[] = m_fileDir.listFiles();
				if (crFile != null) 
				{
					File cr;
					for (int i = 0; i < crFile.length; i++) {
						cr = crFile[i];
						if(postReport(cr))
							cr.delete();
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private boolean postReport(File file) {
		// TODO 使用HTTP Post 发送错误报告到服务器
		// 这里不再详述,开发者可以根据OPhoneSDN上的其他网络操作
		// 教程来提交错误报告
		// if(isNetworkAvailable(ctx)==true){

		// }
		return false;
	}

	/**
	 * 获取错误报告文件名
	 * 
	 * @param ctx
	 * @return
	 */
	/*
	 * private String[] getCrashReportFiles(Context ctx) { File filesDir =
	 * ctx.getFilesDir();
	 * 
	 * FilenameFilter filter = new FilenameFilter() { public boolean accept(File
	 * dir, String name) { return name.endsWith(CRASH_REPORTER_EXTENSION); } };
	 * return filesDir.list(filter); }
	 */
	/**
	 * 保存错误信息到文件中
	 * 
	 * @param ex
	 * @return
	 */
	@SuppressLint("SimpleDateFormat")
	private String saveCrashInfoToFile(Throwable ex) {
		Writer info = new StringWriter();
		PrintWriter printWriter = new PrintWriter(info);
		ex.printStackTrace(printWriter);

		Throwable cause = ex.getCause();
		while (cause != null) {
			cause.printStackTrace(printWriter);
			cause = cause.getCause();
		}

		String result = info.toString();
		MobclickAgent.reportError(mContext, result);
		printWriter.close();
		try {
			mDeviceCrashInfo.put(STACK_TRACE, result);
		} catch (Exception e) {
			ShowLog.showException(e);
		}
		String fileName = null;
		try {
			// long timestamp = System.currentTimeMillis();
			SimpleDateFormat formatter = new SimpleDateFormat("yyyyMMdd-HHmmss");
			Date curDate = new Date(System.currentTimeMillis());
			String timestamp = formatter.format(curDate);
			File file;

			ShowLog.e(TAG, "timestamp is :" + timestamp);
			fileName = "ktvassistant_crash-" + timestamp + CRASH_REPORTER_EXTENSION;
			file = new File(m_fileDir, fileName);

			ShowLog.e(TAG, "new Exception file occur : " + fileName);
			if (!file.exists()) {
				file.createNewFile();
			}
			FileOutputStream trace = new FileOutputStream(file);
			trace.write(mDeviceCrashInfo.toString().getBytes());
			trace.write(result.getBytes());
			trace.flush();
			trace.close();
			
		} catch (Exception e) {
			ShowLog.showException(e);
		}
		return fileName;
	}

	/**
	 * 收集程序设备的信息
	 * 
	 * @param ctx
	 */
	public void collectCrashDeviceInfo(Context ctx) {
		mDeviceCrashInfo.put(VERSION_NAME,AppStatus.s_curVU.versionName);
		// 使用反射来收集设备信息.在Build类中包含各种设备信息,
		// 例如: 系统版本号,设备生产商 等帮助调试程序的有用信息
		Field[] fields = Build.class.getDeclaredFields();
		for (Field field : fields) {
			try {
				field.setAccessible(true);
				mDeviceCrashInfo.put(field.getName(), field.get(null));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * 检测网络连接是否可用
	 * 
	 * @param ctx
	 * @return true 可用; false 不可用
	 */
	/*
	 * private boolean isNetworkAvailable(Context ctx) { ConnectivityManager cm
	 * = (ConnectivityManager)
	 * ctx.getSystemService(Context.CONNECTIVITY_SERVICE); if(cm == null) {
	 * return false; } NetworkInfo[] netinfo = cm.getAllNetworkInfo();
	 * if(netinfo == null) { return false; } for (int i = 0; i < netinfo.length;
	 * i++) { if(netinfo[i].isConnected()) { return true; } } return false; }
	 */

	public void SetExHandler(Handler hHandler) {
		m_hMsgHandler = hHandler;
	}

	public static void exitClient(Context context) {

		ShowLog.d(TAG, "----- exitClient -----");
		AppStatus.exitApp(context);
//		// finish()函数是在OnDestroy中调用的，按理说应该会清理内存，不过还没有验证
//		ActivityManager activityManager = (ActivityManager) context
//				.getSystemService(Context.ACTIVITY_SERVICE);
//		//activityManager.restartPackage("com.ChatRoom");
//		activityManager.killBackgroundProcesses(context.getPackageName());
//		System.exit(0);
	}

}
