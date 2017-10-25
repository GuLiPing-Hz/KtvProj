package assistant.task.gift;

import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.HttpGiftInfo;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.task.PDataCache;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

public class GetGiftInfoTask extends AsyncTask<Void, String, String> {

	private Handler m_handler;
	
	int m_page = 1;
	int m_size = 20;
	int m_ktvId = 0;

	private boolean m_isReadCache = true;
	private String m_cacheKey = null;

	public GetGiftInfoTask(Handler handler) {
		super();
		this.m_handler = handler;
	}
	public void setArgu(int ktvid){
		m_ktvId = ktvid;
	}
	public void setArguBase(int page,int size,boolean readcache){
		m_page = page;
		m_size = size;
		m_isReadCache = readcache;
	}

	public String getUrl() {

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.GiftInfoList;
		String param = "?page=" + m_page + "&size=" + m_size+"&ktvid="+m_ktvId;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		String requestUrl = getUrl();
		if (TextUtils.isEmpty(requestUrl))
			return "";

		m_cacheKey = PCommonUtil.md5Encode(requestUrl);
		PDataCache dataCache = PDataCache.getInstance();
		if (m_isReadCache && dataCache.hasCacheForKey(m_cacheKey, 3600)) {
			String cacheData = dataCache.stringForKey(m_cacheKey);
			if (null != cacheData) {
				publishProgress(cacheData);
			}
			return "IS_READ_CACHE";
		} else {

			PHttpRequest request = PHttpRequest.requestWithURL(requestUrl);
			String result = request.startSyncRequestString();
			return result;
		}

	}

	@Override
	protected void onProgressUpdate(String... values) {
		// TODO Auto-generated method stub
		super.onProgressUpdate(values);

		try {
			JSONObject jsonObject = new JSONObject(values[0]);
			JSONArray list = jsonObject.optJSONArray("ktvgiftlist");
			ArrayList<HttpGiftInfo> itemList = new ArrayList<HttpGiftInfo>();

			if (list.length() > 0) {
				for (int i = 0; i < list.length(); i++) {
					itemList.add(new HttpGiftInfo(list.optJSONObject(i)));
				}
			}

			// 这里加入handler事件，通知刷新界面
			Message msg = m_handler.obtainMessage();
			msg.what = MessageDef.HTTP_GETGIFTLIST;
			msg.arg1 = 0;// 默认0-缓存，1-网络
			msg.obj = itemList;
			m_handler.sendMessage(msg);

		} catch (JSONException e) {
			e.printStackTrace();
		}
	}

	@Override
	protected void onPostExecute(String result) {
		// TODO Auto-generated method stub
		super.onPostExecute(result);
		
		if (TextUtils.equals(result, "IS_READ_CACHE")) {
			return;
		}
		JSONObject jsonObject = PCommonUtil.parseString2JsonObject(result);
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		ArrayList<HttpGiftInfo> itemList = new ArrayList<HttpGiftInfo>();
		try {

			if (null != jsonObject) {
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {
					JSONObject jsonResultObject = jsonObject.optJSONObject("result");

					if (jsonResultObject != null) {
						JSONArray list = jsonResultObject.optJSONArray("ktvgiftlist");

						for (int i = 0; i < list.length(); i++) {
							itemList.add(new HttpGiftInfo(list.optJSONObject(i)));
						}
						// 缓存
						PDataCache.getInstance().setString(m_cacheKey,jsonResultObject.toString());
					}

				}
			}
			else
			{
				errorMsg = "服务器异常";
			}
		} catch (Exception e) {
			ShowLog.showException(e);
		}

		// 这里加入handler事件，通知刷新界面
		Message msg = m_handler.obtainMessage();
		msg.what = (0 == nStatus) ? errorcode : MessageDef.HTTP_GETGIFTLIST;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.obj = (0 == nStatus) ? errorMsg : itemList;
		m_handler.sendMessage(msg);
	}
}
