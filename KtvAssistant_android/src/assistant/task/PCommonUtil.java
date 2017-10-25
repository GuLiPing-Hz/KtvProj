package assistant.task;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.security.MessageDigest;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.text.TextUtils;
import android.util.Log;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.util.ShowLog;

import com.tiange.mobile.common.AES256Cipher;
import com.tiange.mobile.common.Base64;

public class PCommonUtil {
	
	/**
	 * 获取当前的网络信息
	 * @param context
	 * @return
	 */
	public static String getAPNType(Context context){
		String netType = "unknown";
		ConnectivityManager conMgr = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo networkInfo = conMgr.getActiveNetworkInfo();
		
		if(null == networkInfo){
			return netType;
		}
		int nType = networkInfo.getType();
		if(nType == ConnectivityManager.TYPE_MOBILE){
			netType = networkInfo.getExtraInfo();
		} else if(nType == ConnectivityManager.TYPE_WIFI){
			netType = "WiFi";
		}
		return netType;
	}
	
	/**
	 * 获取当前app的版本号
	 * @param context
	 * @param packageName app的包名
	 * @return
	 */
	public static String getAppVersionName(Context context, String packageName){
		String versionName = "";
		
		try {
			PackageManager packageManager = context.getPackageManager();
			PackageInfo packageInfo = packageManager.getPackageInfo(packageName, 0);
			versionName = (null != packageInfo) ? packageInfo.versionName : "";
			
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return versionName;
	}
	
	/**
	 * 对带有空格等特殊字符的url地址做编码
	 * 编码后http://ktv.9158.com/key=value中的部分字符同时被编码，
	 * 需要做替换处理，恢复正常状态.
	 * @param srcurl
	 * @return
	 */
	public static String formatUrlForDownload(String srcurl){
		String dsturl = "" + srcurl;
		if(null != srcurl){
			dsturl = URLEncoder.encode(srcurl)
					.replace("%3A", ":").replace("%2F", "/")
					.replace("%3D", "=").replace("+", "%20");
		}
		return dsturl;
	}
	
	public final static int IMAGE_SIZE_ORIGINAL = 3;
	public final static int IMAGE_SIZE_BIG = 2;
	public final static int IMAGE_SIZE_MIDDLE = 1;
	public final static int IMAGE_SIZE_SMALL = 0;
	
	public final static int START_COVER_ISO_2 = 2;
	public final static int START_COVER_ISO_1 = 1;
	public final static int START_COVER_ANDROID = 0;
	
	
	/**
	 * 转换图片地址，获取不同大小的图片
	 * 1.先将url地址做一次恢复处理，转成原图地址url
	 * 2.再将原图地址处理成指定的缩放图片地址url
	 * @param srcurl
	 * @param scaletype : IMAGE_SIZE_BIG, IMAGE_SIZE_MIDDLE, IMAGE_SIZE_SMALL
	 * @return
	 */
	public static String getUrlByScaleType(String srcurl, int scaletype){
		String dsturl = null;
		if(null == srcurl || srcurl.trim().length() <= 0){
			return dsturl;
		}
		
		String[] imageType = { "_100_100.jpg", "_200_200.jpg", "_320_320.jpg", "_640_640.jpg" };
		//先格式化
		for(int i=0; i<imageType.length; i++){
			srcurl = srcurl.replace(imageType[i], ".jpg");
		}
		dsturl = srcurl.replace(".jpg", imageType[scaletype]);
		//MyLog.d("dsturl: " + dsturl);
		
		return dsturl;
	}
	
	public static String getStartPageCoverUrlScaleType(String srcurl, int scaletype){
		String dsturl = null;
		if(TextUtils.isEmpty(srcurl)){
			return dsturl;
		}
		
		String[] imageType = { "_480_800.jpg", "_640_960.jpg", "_640_1136.jpg" };
		//先格式化
		for(int i=0; i<imageType.length; i++){
			srcurl = srcurl.replace(imageType[i], ".jpg");
		}
		dsturl = srcurl.replace(".jpg", imageType[scaletype]);
		//MyLog.d("dsturl: " + dsturl);
		
		return dsturl;
	}
	
	/**
	 * 做解码，可选是否做aes解码
	 * @param encodeStr
	 * @param doAes
	 * @return
	 */
	//默认使用aes解码，但是对于需要压缩的数据，不使用aes解码
	public static JSONObject parseString2JsonObject(String encodeStr){
		return parseString2JsonObject(encodeStr,true);
	}
	//解析result 返回的是金币量不是json的时候
	public static JSONObject parseString2JsonObjectGold(String encodeStr){
		if(null == encodeStr){
			return null;
		}
		
		JSONObject returnJsonObject = new JSONObject();
		
		try {
			returnJsonObject.put("status", 0);
			returnJsonObject.put("msg", "服务器异常");
			returnJsonObject.put("errorcode", 1);
			
			JSONObject tempJsonObject = new JSONObject(encodeStr);
			int nStatus = tempJsonObject.optInt("status", 0);
			if (1 == nStatus) {
				String strResult = tempJsonObject.optString("result");
				String resultData = AES256Cipher.AES_Decode(strResult, KtvAssistantAPIConfig.KTV_ASSISTANT_APP_SECRET_KEY);
				
				if (null == resultData) {
					returnJsonObject.put("status", 0);
					returnJsonObject.put("msg", tempJsonObject.optString("msg", ""));
					returnJsonObject.put("errorcode", tempJsonObject.optInt("errorcode", 0));
				} else {
					returnJsonObject.put("status", 1);
					returnJsonObject.put("msg", tempJsonObject.optString("msg", ""));
					returnJsonObject.put("errorcode", tempJsonObject.optInt("errorcode", 0));
					returnJsonObject.put("gold", Integer.parseInt(resultData));
				}
			} else {
				returnJsonObject.put("status", 0);
				returnJsonObject.put("msg", tempJsonObject.optString("msg", ""));
				returnJsonObject.put("errorcode", tempJsonObject.optInt("errorcode", 0));
			}
			
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return returnJsonObject;
	}
	public static JSONObject parseString2JsonObject(String encodeStr,boolean needDec){
		
		if(null == encodeStr){
			return null;
		}
		
		JSONObject returnJsonObject = new JSONObject();
		
		try {
			
			returnJsonObject.put("status", 0);
			returnJsonObject.put("msg", "服务器异常");
			returnJsonObject.put("errorcode", 1);
			
			JSONObject tempJsonObject = new JSONObject(encodeStr);
			int nStatus = tempJsonObject.optInt("status", 0);
			if (1 == nStatus) {
				String strResult = tempJsonObject.optString("result");
				String resultData;
				if(needDec)
					resultData = AES256Cipher.AES_Decode(strResult, KtvAssistantAPIConfig.KTV_ASSISTANT_APP_SECRET_KEY);
				else
					resultData = strResult;
				
				if (null == resultData) {
					returnJsonObject.put("status", 0);
					returnJsonObject.put("msg", tempJsonObject.optString("msg", ""));
					returnJsonObject.put("errorcode", tempJsonObject.optInt("errorcode", 0));
				} else {
					returnJsonObject.put("status", 1);
					returnJsonObject.put("msg", tempJsonObject.optString("msg", ""));
					returnJsonObject.put("errorcode", tempJsonObject.optInt("errorcode", 0));
					
					if (resultData.length() > 2) 
						returnJsonObject.put("result", new JSONObject(resultData));
					else
						returnJsonObject.put("result", "");
				}
			} else {
				returnJsonObject.put("status", 0);
				returnJsonObject.put("msg", tempJsonObject.optString("msg", ""));
				returnJsonObject.put("errorcode", tempJsonObject.optInt("errorcode", 0));
			}
			
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			
			e.printStackTrace();
		}
		return returnJsonObject;
	}
	
	/**
	 * 旧的解密方法，对整个数据快做解密，需要debug才能知道错误提示，不方便错误查找
	 * @param encodeStr
	 * @return
	 */
	@Deprecated
	public static JSONObject parserString2JsonObjectOld(String encodeStr){
		
		if(null == encodeStr){
			return null;
		}
		String resultData = AES256Cipher.AES_Decode(encodeStr, KtvAssistantAPIConfig.KTV_ASSISTANT_APP_SECRET_KEY);
		ShowLog.v("resultData: " + resultData);
		JSONObject jsonObject = null;
		try {
			if (null == resultData) {
				
				jsonObject = new JSONObject();
				jsonObject.put("status", 0);
				jsonObject.put("msg", KtvAssistantAPIConfig.ErrorMsgUnknow);
				
			} else {
				jsonObject = new JSONObject(resultData);
			}
			
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return jsonObject;
	}

	public static String generateAPIStringWithSecret(String srcUrl)
	{
		String secret = generateSecretString(srcUrl);
		String strIMEI = encodeBase64(AppStatus.deviceIMEI);
		strIMEI = URLEncoder.encode(strIMEI);
		//secret 必须要放在前面，服务器截取secret之前的数据进行合法性验证。
		String encodeUrl = srcUrl + "&secret=" + secret 
				+ "&versioncode="+AppStatus.s_curVU.versionName+"&client=android&unique=" + strIMEI;
		Log.d("PCommonUtil", encodeUrl);
		return encodeUrl;
	}
	/**
	 * 根据固定url和参数，生成api全地址
	 * @param baseUrl
	 * @param param
	 * @return
	 */
	public static String generateAPIStringWithSecret(String baseUrl, String param){
		String srcUrl = baseUrl + param;
		return generateAPIStringWithSecret(srcUrl);
	}
	
	/**
	 * 结合密钥，将指定的字符串做md5编码，生成服务端校验密钥
	 * @param str
	 * @return
	 */
	public static String generateSecretString(String str){
		String tempStr = "%*(!" + str + "%*(!";
		return md5Encode(tempStr);
	}
	
	/**
	 * 先对字符做url编码，然后再做base64编码
	 * @param str
	 * @return
	 */
	public static String encodeStringOfUrlEncodeAndBase64(String str){
		if(null != str){
			return encodeBase64(URLEncoder.encode(str));
		}
		return null;
	}
	
	/**
	 * 先对字符做base64解码，然后再做url解码
	 * @param str
	 * @return
	 */
	public static String decodeStringOfBase64AndUrlEncode(String str){
		String tempStr = decodeBase64(str);
		if(null != tempStr){
			return URLDecoder.decode(tempStr);
		}
		return null;
	}
	
	/**
	 * 以utf8的形式，对字符串做base64编码
	 * @param str
	 * @return
	 */
	public static String encodeBase64(String str){
		if(null != str){
//			return Base64Encoder.encode(str);
			try {
				byte[] textBytes = str.getBytes("UTF-8");
				return Base64.encodeToString(textBytes, Base64.NO_WRAP);
			} catch (UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return null;
	}
	
	/**
	 * 做字符做base64解码，并转成utf8格式的字符串
	 * @param str
	 * @return
	 */
	public static String decodeBase64(String str){
		if(null != str){
//			return Base64Decoder.decode(str);
			
			try {
				byte[] textBytes =Base64.decode(str,0);
				return new String(textBytes, "UTF-8");
			} catch (UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return null;
	}
	
	/**
	 * 对字符做md5编码
	 * @param str
	 * @return
	 */
	public static String md5Encode(String str) {
		// 返回字符串
		String md5Str = null;
		if(null == str){
			return md5Str;
		}
		try {
			// 操作字符串
			StringBuffer buf = new StringBuffer();
			MessageDigest md = MessageDigest.getInstance("MD5");

			// 添加要进行计算摘要的信息,使用 plainText 的 byte 数组更新摘要。
			md.update(str.getBytes());

			// 计算出摘要,完成哈希计算。
			byte b[] = md.digest();
			int i;

			for (int offset = 0; offset < b.length; offset++) {
				i = b[offset];
				if (i < 0) {
					i += 256;
				}
				if (i < 16) {
					buf.append("0");
				}
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

}
