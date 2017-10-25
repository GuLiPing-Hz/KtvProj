package assistant.util;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.NetworkInfo.State;
import android.webkit.CookieManager;
import android.webkit.CookieSyncManager;

/**
 * @author shou_peng
 * @version 创建时间：2013-1-6 下午04:07:27 类说明
 */
public class MobileNetStatUtil {
	public static int netState = -1; // -1:默认值 0:未联网 1:移动网络2G/3G 2:WIFI

	//无网络
	public final static int NET_NOTCONNECT = 0;
	//使用手机网络2G,3G,4G
	public final static int NET_MOBILE = 1;
	//使用Wifi网络
	public final static int NET_WIFI = 2;
	/**
	 * 判断是否联网
	 * 
	 * @param cont
	 * @return
	 */
	public static boolean isNetConnection(final Context cont) {
		ConnectivityManager manager = (ConnectivityManager) cont.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo networkinfo = manager.getActiveNetworkInfo();
		if (networkinfo == null || !networkinfo.isAvailable()) {
			return false;
		}
		return true;
	}

	
	public static boolean unNetConnectionTips(final Context cont){
		if(getCurrentNetState(cont) <= 0){
			return true;
		}
		return false;
	}
	
	public static boolean checkCurrentNetState(Context context){
		return checkCurrentNetState(context,false);
	}
	public static boolean checkCurrentNetState(Context context,boolean isSlient){
		boolean isConnect = getCurrentNetState(context,isSlient) > 0;
		return isConnect;
	}
	
	public static int getCurrentNetState(Context cont){
		return getCurrentNetState(cont,false);
	}
	public static int getCurrentNetState(Context cont,boolean isSlient){
		getNetType(cont);
		if(netState <= 0 && !isSlient)
			ShowUtil.showToast(cont, R.string.no_networking);
		return netState;
	}
	
	/**
	 * wifi检测
	 * 
	 * @return 无网络或者2G/3G 返回false; wifi返回true
	 */
	public static boolean isWiFi(final Context con) {
		ConnectivityManager manager = (ConnectivityManager) con.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo networkinfo = manager.getActiveNetworkInfo();
		if (networkinfo == null || !networkinfo.isAvailable()) {
			return false;
		} else {
			State state = manager.getNetworkInfo(ConnectivityManager.TYPE_WIFI).getState();
			if (state.toString().equals("CONNECTED"))// if(wifi == State.CONNECTED)
				return true;
			else
				return false;
		}

	}

	/**
	 * 获得网络连接类型状态
	 * 
	 * @param context
	 */
	public static void getNetType(Context context) {
		ConnectivityManager connManager = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
		// State state = connManager.getActiveNetworkInfo().getState();
		if(connManager == null)
		{
			netState = 0;
			return ;
		}
		
		State state = connManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI).getState(); // 获取网络连接状态
		if (State.CONNECTED == state) { // 判断是否正在使用WIFI网络
			netState = 2;
			return;
		}

		state = connManager.getNetworkInfo(ConnectivityManager.TYPE_MOBILE).getState(); // 获取网络连接状态
		if (State.CONNECTED == state) { // 判断是否正在使用手机移动网络
			netState = 1;
			return;
		}

		netState = 0;
	}

	/**
     * Clear current context cookies .
     * 
     * @param context
     *            : current activity context.
     * 
     * @return void
     */
    public static void clearCookies(Context context) {
        @SuppressWarnings("unused")
        CookieSyncManager cookieSyncMngr = CookieSyncManager.createInstance(context);
        CookieManager cookieManager = CookieManager.getInstance();
        cookieManager.removeAllCookie();
    }
    
}
