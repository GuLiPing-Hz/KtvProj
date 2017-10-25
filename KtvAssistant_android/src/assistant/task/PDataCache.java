package assistant.task;

import java.io.File;
import java.util.Iterator;

import org.json.JSONException;
import org.json.JSONObject;

import android.os.Environment;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.util.ShowLog;

import com.tiange.mobile.common.PFileManager;

public class PDataCache extends PFileManager {
	private final static long CACHE_TIME = 157680000;
	private final static String PDATA_CACHE_JSON = "PDataCache.dat";
	private static JSONObject pDataCacheJson = null;
	
	//private static String PDataCacheDirectory = null;
	private static PDataCache instance = null;
	
	public synchronized static PDataCache getInstance(){
		if(null == instance){
			instance = new PDataCache();
		}
		return instance;
	}
	
	private PDataCache(){
		
		super.secretKey = KtvAssistantAPIConfig.KTV_ASSISTANT_APP_SECRET_KEY;
		
		try {
			String jsonStr = readData(PDataCacheDirectory() + PDATA_CACHE_JSON);
			if(null != jsonStr){
				pDataCacheJson = new JSONObject(jsonStr);
			}else{
				pDataCacheJson = new JSONObject();
			}
			
		} catch (JSONException e) {
			pDataCacheJson = new JSONObject();
		}
		
	}
	
	private static String PDataCacheDirectory(){
		return AppStatus.s_appDirUtil.getCacheDir();
	}
	
	/**
	 * 用key作为文件名称和关键字缓存数据,数据长期有效
	 * @param key
	 * @param value
	 */
	public void setString(String key, String value){
		long timeInterval = System.currentTimeMillis();//当前时间
		setString(key, value, timeInterval);
	}
	
	/**
	 * 缓存数据，并记录数据缓存开始的时间戳
	 * @param key
	 * @param value
	 * @param timeInterval
	 */
	public void setString(String key, String value, long timeInterval){
		
		try {
			if(!android.os.Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED)){
				return;
			}
			
			//保存文件
			writeData(value, PDataCacheDirectory() + key);
			
			//记录时间
			pDataCacheJson.put(key, timeInterval);
			writeData(pDataCacheJson.toString(), PDataCacheDirectory() + PDATA_CACHE_JSON);
			
		} catch (JSONException e) {
			ShowLog.e("PDataCache", "json: put value for " + key);
		}
		
	}
	
	/**
	 * 判断是否有对应key值的缓存数据，有效期默认为5年
	 * @param key
	 * @return
	 */
	public boolean hasCacheForKey(String key){
		return hasCacheForKey(key, CACHE_TIME);
	}
	
	/**
	 * 判断在有效期内，是否有对应key值的缓存数据
	 * @param key
	 * @param timeout	单位:秒
	 * @return
	 */
	public boolean hasCacheForKey(String key, long timeout){
		
		try {
			if(pDataCacheJson.isNull(key)){
				return false;
			}
			long timeInterval = pDataCacheJson.getLong(key);
			//MyLog.i("System.currentTimeMillis(): " + System.currentTimeMillis());
			//MyLog.i("((System.currentTimeMillis() - timeInterval)/1000): " + ((System.currentTimeMillis() - timeInterval)/1000));
			if(((System.currentTimeMillis() - timeInterval)/1000) < timeout){
				return true;
			}
			
		} catch (JSONException e) {
			ShowLog.e("PDataCache", "json: get value for " + key);
		}
		
		return false;
	}
	
	/**
	 * 判断缓存数据是否过期
	 * @param key
	 * @param timeout	单位:秒
	 * @return
	 */
	public boolean isCacheOverdue(String key, long timeout){
		
		try {
			if(pDataCacheJson.isNull(key)){
				return true;
			}
			long timeInterval = pDataCacheJson.getLong(key);
			ShowLog.i("timeout---" + timeout + ": " + ShowLog.showTimeFromMilis(timeout));
			ShowLog.i("timeout---" + timeInterval + ": " + ShowLog.showTimeFromMilis(timeInterval));
			if(timeout <= timeInterval){
				return false;
			}
			
		} catch (JSONException e) {
			ShowLog.e("PDataCache", "json: get value for " + key);
		}
		
		return true;
	}
	
	
	/**
	 * 根据key获取缓存数据
	 * @param key
	 * @return
	 */
	public String stringForKey(String key){
		String value = readData(PDataCacheDirectory() + key);
		return ((null != value) && (value.length() > 0)) ? value : null;
	}
	
	/**
	 * 清理缓存
	 */
	public void clearCache(){
		Iterator<String> jsonKeys = pDataCacheJson.keys();
		while(jsonKeys.hasNext()){
			String tempkey = jsonKeys.next();
			
			File tempFile = new File(PDataCacheDirectory() + tempkey);
			if(tempFile.exists()){
				tempFile.delete();
			}
//			pDataCacheJson.remove(tempkey);
		}
		pDataCacheJson = new JSONObject();
		writeData(pDataCacheJson.toString(), PDataCacheDirectory() + PDATA_CACHE_JSON);
	}
	
	/**
	 * 删除指定key的缓存
	 * @param key
	 */
	public void removeCacheForKey(String key){
		File tempFile = new File(PDataCacheDirectory() + key);
		if(tempFile.exists()){
			tempFile.delete();
		}
		pDataCacheJson.remove(key);
		writeData(pDataCacheJson.toString(), PDataCacheDirectory() + PDATA_CACHE_JSON);
	}

}
