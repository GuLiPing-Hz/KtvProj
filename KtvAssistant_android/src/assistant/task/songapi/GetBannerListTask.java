package assistant.task.songapi;

import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.Banner;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.task.PDataCache;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

/**
 * @author 宋静
 * @version 创建时间：2013-12-9 下午5:01:35 类说明 6.4 获取banner信息列表
 */
public class GetBannerListTask extends AsyncTask<Void, String, String> {
	private Handler handler;
	private long userIdx;
	private String roomId;

	private boolean isreadcache = true;
	private String requestUrl = null;
	private String cacheKey = null;

	public GetBannerListTask(Handler handler, long userIdx, String roomId) {
		super();
		this.handler = handler;
		this.userIdx = userIdx;
		this.roomId = roomId;
	}

	public GetBannerListTask(Handler handler, long userIdx, String roomId,
			boolean isreadcache) {
		super();
		this.handler = handler;
		this.userIdx = userIdx;
		this.roomId = roomId;
		this.isreadcache = isreadcache;
	}

	public String getUrl() {
		if (TextUtils.isEmpty(roomId))
			return "";

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.GetBannerList;
		String param = "?userID=" + userIdx + "&roomid=" + roomId;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		requestUrl = getUrl();
		if (TextUtils.isEmpty(requestUrl))
			return "";

		cacheKey = PCommonUtil.md5Encode(requestUrl);
		PDataCache dataCache = PDataCache.getInstance();
		if (isreadcache && dataCache.hasCacheForKey(cacheKey, 3600)) {
			// MyLog.d("cache..." + cacheKey);
			String cacheData = dataCache.stringForKey(cacheKey);
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

			JSONArray list = jsonObject.optJSONArray("bannerlists");
			ArrayList<Banner> bannerList = new ArrayList<Banner>();

			if (list.length() > 0) {

				for (int i = 0; i < list.length(); i++) {
					bannerList.add(new Banner(list.optJSONObject(i)));
				}
			}

			// 这里加入handler事件，通知刷新界面
			Message msg = handler.obtainMessage();
			msg.what = MessageDef.WM_GET_BANNERLIST;
			msg.arg1 = 0;// 默认0-缓存，1-网络
			msg.obj = bannerList;
			handler.sendMessage(msg);

		} catch (JSONException e) {
			ShowLog.showException(e);
		}
	}

	@Override
	protected void onPostExecute(String result) {
		// TODO Auto-generated method stub
		super.onPostExecute(result);

		if (TextUtils.equals(result, "IS_READ_CACHE")) {
			return;
		}
		
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		ArrayList<Banner> bannerList = new ArrayList<Banner>();
		
		if(result != null)
		{
			JSONObject jsonObject = PCommonUtil.parseString2JsonObject(result);
			
			try {
				if (null != jsonObject) 
				{
					nStatus = jsonObject.optInt("status");
					errorMsg = jsonObject.optString("msg");
					errorcode = jsonObject.optInt("errorcode");
					if (1 == nStatus) {
						JSONObject jsonResultObject = jsonObject
								.optJSONObject("result");

						if (jsonResultObject != null) {
							JSONArray list = jsonResultObject
									.optJSONArray("bannerlists");

							if (list.length() > 0) {

								for (int i = 0; i < list.length(); i++) {
									bannerList
											.add(new Banner(list.optJSONObject(i)));
								}
							}

							// 缓存
							PDataCache.getInstance().setString(cacheKey,
									jsonResultObject.toString());
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
		}
		
		// 这里加入handler事件，通知刷新界面
		Message msg = handler.obtainMessage();
		msg.what = (0 == nStatus) ? errorcode : MessageDef.WM_GET_BANNERLIST;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.obj = (0 == nStatus) ? errorMsg : bannerList;
		handler.sendMessage(msg);
	}

}
