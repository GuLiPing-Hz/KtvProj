package assistant.util;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.channels.FileChannel;
import java.security.MessageDigest;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;
import android.app.NotificationManager;
import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.net.wifi.WifiManager;
import android.os.Environment;
import android.os.IBinder;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.webkit.CookieManager;
import android.webkit.CookieSyncManager;
import assistant.global.AppStatus;

import com.umeng.analytics.MobclickAgent;


/**
 * @author 寿鹏
 * @version 创建时间：2013-10-21 下午4:12:39
 * 类说明
 */
public class ToolUtil {
	private static Executor executor = Executors.newSingleThreadExecutor();
	
	public static final void executeInSingleThread(Runnable run){
		executor.execute(run);
	}
	
	public static byte[] bmpToByteArray(final Bitmap bmp, final boolean needRecycle) {
		ByteArrayOutputStream output = new ByteArrayOutputStream();
		bmp.compress(CompressFormat.PNG, 100, output);
		if (needRecycle) {
			bmp.recycle();
		}
		
		byte[] result = output.toByteArray();
		try {
			output.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
	}
	/**
	 * md5加密
	 * 
	 * @param plainText
	 * @return
	 */
	public static String md5(String plainText) {
		// 返回字符串
		String md5Str = "";
		
		if(plainText == null || TextUtils.isEmpty(plainText))
			return md5Str;
		
		try {
			// 操作字符串
			StringBuffer buf = new StringBuffer();
			MessageDigest md = MessageDigest.getInstance("MD5");

			// 添加要进行计算摘要的信息,使用 plainText 的 byte 数组更新摘要。
			md.update(plainText.getBytes());

			// 计算出摘要,完成哈希计算。
			byte b[] = md.digest();
			int i;

			for (int offset = 0; offset < b.length; offset++) {
				i = b[offset];
				if (i < 0) 
					i += 256;
				if (i < 16) 
					buf.append("0");

				// 将整型 十进制 i 转换为16位，用十六进制参数表示的无符号整数值的字符串表示形式。
				buf.append(Integer.toHexString(i));
			}

			// 32位的加密
			md5Str = buf.toString();

			// 16位的加密
			// md5Str = buf.toString().md5Strstring(8,24);

		} catch (Exception e) {
			e.printStackTrace();
		}
		return md5Str;
	}

	/**
	 * 获取IMEI
	 * 
	 * @param context
	 * @return
	 */
	public static String getIMEI(Context context) {
		String res = ((TelephonyManager) context
				.getSystemService(Context.TELEPHONY_SERVICE)).getDeviceId();
		if(TextUtils.isEmpty(res)){
			return "";
		}
		return res;
	}
	
	/**
	 * 获取MacAddress,当取不到时取IMEI码
	 * @param context
	 * @return
	 */
	@Deprecated
	public static String getMacAddress(Context context) {
		String macAddress = ((WifiManager)context.getSystemService(Context.WIFI_SERVICE))
								.getConnectionInfo().getMacAddress();
		if(TextUtils.isEmpty(macAddress)){
			return getIMEI(context);
		}
		return macAddress.replaceAll(":", "").trim();
	}
	
	/**
	 * 将指定格式的日期字符串转化成时间戳
	 * @param str
	 * @param format
	 * @return
	 */
	public static long parseTimeStr2Timestamp(String str, String format) {
		if (!TextUtils.isEmpty(str) && !TextUtils.isEmpty(format)) {
			SimpleDateFormat sdf = new SimpleDateFormat(format,AppStatus.s_timeStyle);
			try {
				synchronized (sdf) {
					// SimpleDateFormat is not thread safe
					return sdf.parse(str).getTime();
				}
			} catch (ParseException pe) {
				pe.printStackTrace();
			}
		}
		return 0l;
	}
	
	/**
	 * 将指定格式的日期字符串解析成日期类
	 * @param str
	 * @param format
	 * @return
	 */
	public static Date parseTimeStr2Date(String str, String format) {
		SimpleDateFormat sdf = new SimpleDateFormat(format,AppStatus.s_timeStyle);
		try {
			synchronized (sdf) {
				// SimpleDateFormat is not thread safe
				return sdf.parse(str);
			}
		} catch (ParseException pe) {
			pe.printStackTrace();
		}
		return null;
	}
	
	/**
	 * 将指定格式的日期字符串解析成界面展示的样式
	 * 
	 * @param str
	 * @param format
	 * @return ("刚刚"、"x分钟前"、"x小时前"、"MM-dd HH:mm"格式)
	 */
	public static String parseDate(String str, String format) {
		SimpleDateFormat sdf = new SimpleDateFormat(format,AppStatus.s_timeStyle);
		try {
			synchronized (sdf) {
				// SimpleDateFormat is not thread safe
				return getTimeDiff(sdf.parse(str));
			}
		} catch (ParseException pe) {
			pe.printStackTrace();
		}
		return "";
	}
	
	public static String getStringDate(String format){
		SimpleDateFormat sdf = new SimpleDateFormat(format,AppStatus.s_timeStyle);
		return sdf.format(new Date());
	}

	/**
	 * 将指定格式的时间字符串转换成"yyyy-MM-dd HH:mm:ss"格式的时间字符串
	 * 
	 * @param str
	 * @param format
	 * @param isSinaData
	 * @return "yyyy-MM-dd HH:mm:ss"格式
	 */
	public static String formatDateToStr(String str, String format,
			boolean isSinaData) {
		SimpleDateFormat sdf = new SimpleDateFormat(format,AppStatus.s_timeStyle);
		if (isSinaData) {
			str = Pattern.compile(" \\+\\d{4} ").matcher(str).replaceAll(" ");
			//sdf.setTimeZone(TimeZone.getTimeZone("GMT"));
		}
		try {
			synchronized (sdf) {
				// SimpleDateFormat is not thread safe
				Date date = sdf.parse(str);
				SimpleDateFormat sdf_other = new SimpleDateFormat(
						"yyyy-MM-dd HH:mm:ss",AppStatus.s_timeStyle);
				return sdf_other.format(date);
			}
		} catch (Exception e) {
			ShowLog.showException(e);
		}
		return "";
	}

	public static String getTimeDiff(Date date) {
		Calendar cal = Calendar.getInstance();
		long diff = 0;
		Date dnow = cal.getTime();
		String str = "";
		diff = dnow.getTime() - date.getTime();

		// System.out.println("diff---->"+diff);
		if (diff > 24 * 60 * 60 * 1000 || diff < 0 ) {
			// System.out.println("1天前");
			// str="1天前";
			SimpleDateFormat dateFormat = new SimpleDateFormat("MM-dd HH:mm",AppStatus.s_timeStyle);
			str = dateFormat.format(date);
		} else if (diff > 1 * 60 * 60 * 1000) {
			Long num = diff / (1 * 60 * 60 * 1000);
			str = String.valueOf(num) + "小时前";
		} else if (diff > 1 * 60 * 1000) {
			Long num = diff / (1 * 60 * 1000);
			str = String.valueOf(num) + "分钟前";
		} else {
			str = "刚刚";
		}
		return str;
	}

	/**
	 * 转换时间（String转化为String）
	 * 
	 * @param time 时间
	 * @param formatBefore
	 *            转换前的时间格式，比如"yyyy-MM-dd HH:mm:ss"
	 * @param formatAfter
	 *            转换后的时间格式，比如"MM-dd"
	 * @return 返回指定格式的时间显示
	 */
	public static String formatTime(String time, String formatBefore, String formatAfter) {
		if (!TextUtils.isEmpty(time) && !TextUtils.isEmpty(formatBefore) && !TextUtils.isEmpty(formatAfter)) {
			try {
				SimpleDateFormat sdf_before = new SimpleDateFormat(formatBefore,AppStatus.s_timeStyle);
				synchronized (sdf_before) {
					// SimpleDateFormat is not thread safe
					Date date = sdf_before.parse(time);
					SimpleDateFormat sdf_after = new SimpleDateFormat(
							formatAfter,AppStatus.s_timeStyle);
					return sdf_after.format(date);
				}
			} catch (Exception e) {
				ShowLog.showException(e);
			}
		}

		return "";
	}

	/**
	 * 将时间戳转换指定的时间字符串
	 * 
	 * @param timeStamp 时间戳(单位：毫秒)
	 * @param format 转换的时间格式，比如"yyyy-MM-dd HH:mm:ss"
	 * 
	 * @return 返回指定格式的时间字符串
	 */
	public static String formatTimestampToStr(long timeStamp, String format) {
		if (!TextUtils.isEmpty(format) && timeStamp >0 ) {
			try {
				SimpleDateFormat dateFormat = new SimpleDateFormat(format,AppStatus.s_timeStyle);
				synchronized (dateFormat) {
					// SimpleDateFormat is not thread safe
					Date date = new Date(timeStamp);
					return dateFormat.format(date);
				}
			} catch (Exception e) {
				ShowLog.showException(e);
			}
		}

		return "";
	}
	
	/**
	 * 编辑框计算字数
	 * 
	 * @param str
	 * @return
	 */
	public static float countWords(String str) {
		if (TextUtils.isEmpty(str) || TextUtils.isEmpty(str.trim())) {
			return 0;
		}
		float len = 0;
		char c;
		for (int i = str.length() - 1; i >= 0; i--) {
			c = str.charAt(i);
			if(isChinese(c)){
				len++;
			}else{
				len += 0.5;
			}  
			/*if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z')
					|| (c >= 'A' && c <= 'Z')) {
				// 字母, 数字
				len += 0.5;
			} else {
				if (Character.isLetter(c)) { // 中文
					len++;
				} else { // 符号或控制字符
					len++;
				}
			}*/
		}
		return len;
	}
	
	/**
	 * 判断是否是中文字符
	 * @param c
	 * @return
	 */
	public static boolean isChinese(char c) {  
        Character.UnicodeBlock ub = Character.UnicodeBlock.of(c);  
        if (ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS  //【4E00-9FFF】 CJK Unified Ideographs 中日韩统一表意文字
                || ub == Character.UnicodeBlock.CJK_COMPATIBILITY_IDEOGRAPHS  //【F900-FAFF】 CJK Compatibility Ideographs 中日韩兼容表意文字
                || ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A  //【3400-4DBF】 CJK Unified Ideographs Extension A 中日韩统一表意文字扩充A
                || ub == Character.UnicodeBlock.GENERAL_PUNCTUATION  	//【2000-206F】 General Punctuation 一般标点符号
                || ub == Character.UnicodeBlock.CJK_SYMBOLS_AND_PUNCTUATION  //【3000-303F】 CJK Symbols and Punctuation 中日韩符号和标点
                || ub == Character.UnicodeBlock.HALFWIDTH_AND_FULLWIDTH_FORMS) //【FF00-FFEF】 Halfwidth and Fullwidth Forms 半角及全角字符
        {  
            return true;  
        }  
        return false;  
    }  

	/**
	 * 判断SD卡是否可用（SD卡被占用算作无SD卡）
	 * 
	 * @return true可用,反之false
	 */
	public static boolean checkSDcardState() {
		return android.os.Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED) ? true : false;
	}
	
	public static boolean isKeyBoardOpen(Activity con,View view ){
			InputMethodManager imm = (InputMethodManager) con.getSystemService(Context.INPUT_METHOD_SERVICE);
			return imm.isActive(view);
	}
	public static boolean isKeyBoardOpen(Activity con){
		InputMethodManager imm = (InputMethodManager) con.getSystemService(Context.INPUT_METHOD_SERVICE);
		return imm.isActive();
	}
	/**
	 * 关闭键盘
	 * 
	 * @param con
	 */
	public static void closeKeyBoard(Activity con) {
		InputMethodManager imm = (InputMethodManager) con.getSystemService(Context.INPUT_METHOD_SERVICE);
		
		try {
			
//			if (con.getWindow().getAttributes().softInputMode != WindowManager.LayoutParams.SOFT_INPUT_STATE_HIDDEN) {
//				if (con.getCurrentFocus() != null)
//					imm.hideSoftInputFromWindow(con.getCurrentFocus().getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
//			}
			
			View view = con.getCurrentFocus();
			if(view == null)
				return ;
			IBinder ibinder= con.getCurrentFocus().getWindowToken();
			if(ibinder != null)
				imm.hideSoftInputFromWindow(ibinder, InputMethodManager.RESULT_UNCHANGED_SHOWN);
			
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}
	
	/**
	 * 显示软键盘
	 * @param con
	 */
	public static void showKeyBoard(Activity con) {
		showKeyBoard(con,InputMethodManager.HIDE_IMPLICIT_ONLY);
	}
	public static void showKeyBoard(Activity con,int flag) {
		try {
			InputMethodManager imm = (InputMethodManager) con.getSystemService(Context.INPUT_METHOD_SERVICE);
			imm.showSoftInput(con.getCurrentFocus(), flag);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
	}
	
	/**
	 * 匹配登陆口令是否合规
	 * @param key
	 * @return true 正确， false 反之
	 */
	public static boolean matcherLoginKey(String key) {
		try {
			if(TextUtils.isEmpty(key) || TextUtils.isEmpty(key.trim())){
				return false;
			}
			//匹配由数字和26个英文字母组成的字符串
			Pattern pattern = Pattern.compile("^[A-Za-z0-9]+$");
			Matcher matcher = pattern.matcher(key);
			if(matcher.find() && key.length() == 6){
				return true;
			}
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return false;
	}
	
	
	/**
	 * 
	 * @param con
	 * @param fileName
	 * @param paramName
	 * @return
	 */
	public static String[] getSharedPreferencesParam(Context con, String fileName, String[] paramName){
		int size = paramName.length;
		String param[] = new String[size]; 
		SharedPreferences sp = con.getSharedPreferences(fileName, Context.MODE_PRIVATE);
		for(int i=0; i< size; i++){
			param[i] = sp.getString(paramName[i], null);
		}
		return param;
	}
	
	/**
	 * paramName的数组长度和 data的数组长度保持一致，并且两者数据要一一对应
	 * @param con
	 * @param fileName
	 * @param paramName
	 * @param data
	 */
	public static void saveSharedPreferencesParam(Context con, String fileName, String[] paramName, String[] data){
		int size = paramName.length;
		SharedPreferences sp = con.getSharedPreferences(fileName, Context.MODE_PRIVATE);
		for(int i=0; i< size; i++){
			sp.edit().putString(paramName[i],data[i]).commit();
		}
	}

	/** 将时间转换为00：00格式 */
	public static String toTime(int times) {
		times /= 1000;
		int minutes = times / 60;
//		int hour = minutes / 60;
		int seconds = times % 60;
		minutes %= 60;
		return String.format("%02d:%02d", minutes, seconds);
	}

	public static DisplayMetrics getDisplayMetrics(Activity con){
		DisplayMetrics dMetrics = new DisplayMetrics();
		if(con!=null && con.getWindowManager()!=null && con.getWindowManager().getDefaultDisplay()!=null){
			con.getWindowManager().getDefaultDisplay().getMetrics(dMetrics);
		}
		/*int screenWidth = dMetrics.widthPixels;
		int screenHeight = dMetrics.heightPixels;*/
		return dMetrics;
	}

	public static void clearCookie(Context context){
		CookieSyncManager cookieSyncMngr = CookieSyncManager.createInstance(context);
        CookieManager cookieManager = CookieManager.getInstance();
        cookieManager.removeAllCookie();
	}
	
	/**
	 * 取消通知栏的通知
	 * @param context
	 * @param notifyId
	 */
	public static void cancelNotification(Context context, int notifyId){
		//isFinishedPlay = true;
		NotificationManager mNotificationManager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);
        mNotificationManager.cancel(notifyId);  
	}
	
	/** 
     * 程序是否在前台运行 
     *  
     * @return 
     */  
    public static boolean isAppOnForeground(Activity activity) {  
            ActivityManager activityManager = (ActivityManager) activity.getApplicationContext().getSystemService(Context.ACTIVITY_SERVICE);  
            String packageName = activity.getApplicationContext().getPackageName();  

            List<RunningAppProcessInfo> appProcesses = activityManager.getRunningAppProcesses();  
            if (appProcesses == null)   return false;  

            for (RunningAppProcessInfo appProcess : appProcesses) {  
                    if (appProcess.processName.equals(packageName) && appProcess.importance == RunningAppProcessInfo.IMPORTANCE_FOREGROUND) {  
                            return true;  
                    } 
            } 
            return false;  
    }

    /** 复制文件，检查SD卡存在与否在外层做 */
    @SuppressWarnings("resource")
	public static boolean copyFile(String oldPath, String newPath){
    	File oldFile = new File(oldPath);
    	if(!oldFile.exists()){
    		ShowLog.e("ToolUtil", "复制文件 旧的文件不存在.");
    		return false;
    	}
    		
    	File newFile = new File(newPath);
    	if(newFile.exists()){
    		newFile.delete();
    		ShowLog.w("ToolUtil", "复制文件 新的文件已存在，删除之.");
    	}
    	
    	//开始复制文件
    	FileInputStream in = null;
    	FileOutputStream out= null;
    	FileChannel inC = null;
    	FileChannel outC = null;
		try {
			in = new FileInputStream(oldFile);
	        out = new FileOutputStream(newFile);
	        inC = in.getChannel();
	        outC = out.getChannel();
	        int length = 1024;
	        while(true){
				if(inC.position() >= inC.size()){
				    inC.close();
				    outC.close();
				    in.close();
					out.close();
				    return true;
				}

	            if((inC.size() - inC.position()) < 1024)
	                length = (int)(inC.size()-inC.position());
	            else
	                length = 1024;
	            
	            inC.transferTo(inC.position(), length, outC);
	            inC.position(inC.position() + length);
	        }
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			ShowLog.e("ToolUtil", "复制文件 文件不存在.");
		}catch (IOException e) {
			// TODO Auto-generated catch block
			ShowLog.e("ToolUtil", "复制文件 IOException.");
		}
		
		try {
			inC.close();
		    outC.close();
			in.close();
			out.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			ShowLog.e("ToolUtil", "复制文件 IOException.");
		}
    	return false;
    }
    
    public static float StrVerToFloat(String strVer)
	{
		float 	ver = 0.0f;
		if( strVer != null )
		{
			String  str = "";
			char	Char;
			boolean	isPoint = false;
			for(int i = 0 ; i < strVer.length() ; ++i)
			{
				Char = strVer.charAt(i);
				if( ( Char >= '0' && Char <= '9') )
					str +=Char;
				else if( !isPoint && Char == '.')
				{
					isPoint = true;
					str += Char;
				}
			}
			try
			{
				return Float.parseFloat(str);
			}
			catch(Exception e){};
		}
		return ver;
	}
    

	/**
	 * 退出程序
	 * @param con
	 */
	public synchronized static void exitApp(Context con){
		//结束activity队列中的所有activity
		MobclickAgent.onKillProcess(con);
		if( AppStatus.activityList !=null){
			for (Activity ac : AppStatus.activityList) {
				if ( !ac.isFinishing() ) { 
					ac.finish();
				}
			}
		}
		//清除通知栏
		((NotificationManager)con.getSystemService(android.content.Context.NOTIFICATION_SERVICE)).cancelAll();
		
		System.exit(0);
	}

	/**
	 * 获得Hash码
	 * @param fileName
	 * @param hashType
	 * @return
	 * @throws Exception
	 */
	public static String getHash(String fileName, String hashType)  
    throws Exception {  
		InputStream fis;  
		fis = new FileInputStream(fileName);  
		byte[] buffer = new byte[1024];  
		MessageDigest md5 = MessageDigest.getInstance(hashType);  
		int numRead = 0;  
		while ((numRead = fis.read(buffer)) > 0) {  
		    md5.update(buffer, 0, numRead);  
		}  
		fis.close();  
		return toHexString(md5.digest());  
	}  
    
	private static String toHexString(byte[] b) {  
		StringBuilder sb = new StringBuilder(b.length * 2);  
		for (int i = 0; i < b.length; i++) {  
			sb.append(hexChar[(b[i] & 0xf0) >>> 4]);  
		    sb.append(hexChar[b[i] & 0x0f]);  
		}  
		return sb.toString();  
	}
	private static char[] hexChar = { '0', '1', '2', '3', '4', '5', '6', '7',  
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' }; 
	
	/**
	 * @author xpp 
	 * create at 2013-12-06 10:23:00
	 * 检查是否有SD卡
	 */
	public static boolean hasSdCard() {
		String state = Environment.getExternalStorageState();
		if(state.equals(Environment.MEDIA_MOUNTED)){
			return true;
		}else{
			return false;
		}
	}

	public static int dip2px(Context context, float dipValue){ 
		final float scale = context.getResources().getDisplayMetrics().density; 
		return (int)(dipValue * scale + 0.5f); 
	} 
	public static int px2dip(Context context, float pxValue){ 
		final float scale = context.getResources().getDisplayMetrics().density; 
		return (int)(pxValue / scale + 0.5f); 
	} 
}
