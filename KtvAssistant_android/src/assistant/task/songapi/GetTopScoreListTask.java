package assistant.task.songapi;

import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.WorkInfo;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.task.PDataCache;

import com.tiange.phttprequest.PHttpRequest;

/**
	获取榜单数据，榜单数据不提供搜索功能！
 */
public class GetTopScoreListTask extends AsyncTask<Void, String, String> {
	private Handler handler;
	private int model;
	private int topic;
	//起始点
	private int begin = 1;
	//需求数量
	private int num = 100;
	
	private boolean isreadcache = true;
	private String requestUrl = null;
	private String cacheKey = null;

	//获取榜单数据
	public GetTopScoreListTask(Handler handler,
			int model, int topic, int begin, int num) {
		super();
		this.handler = handler;
		this.model = model;
		this.topic = topic;
		this.begin = begin;
		this.num = num;
	}

//	public GetTopScoreListTask(Handler handler,int model, int topic, int begin
//			, int num, boolean isreadcache) {
//		super();
//		this.handler = handler;
//		this.model = model;
//		this.topic = topic;
//		this.begin = begin;
//		this.num = num;
//		this.isreadcache = isreadcache;
//	}

	public String getUrl() {
		if (model <= 0 || topic < 0)
			return "";

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.SongGetSongTopics;
		String param = "?Modelid="+ model + "&topicid=" + topic +"&keyword=&Begin="
				+ begin + "&Num=" + num;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		requestUrl = getUrl();
		if (TextUtils.isEmpty(requestUrl))
			return "";

		cacheKey = PCommonUtil.md5Encode(requestUrl);
		PDataCache dataCache = PDataCache.getInstance();
		//缓存5分钟
		if (isreadcache && dataCache.hasCacheForKey(cacheKey, 60*5)) {
			String cacheData = dataCache.stringForKey(cacheKey);
			if (null != cacheData)
				publishProgress(cacheData);
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
			JSONArray list = jsonObject.optJSONArray("worklists");
			ArrayList<WorkInfo> workList = new ArrayList<WorkInfo>();
			
			if (list.length() > 0) 
			{
				for (int i = 0; i < list.length(); i++)
					workList.add(new WorkInfo(list.optJSONObject(i)));
			}

			// 这里加入handler事件，通知刷新界面
			Message msg = handler.obtainMessage();
			msg.what = MessageDef.WM_GET_TOP_SCORELIST;
			msg.arg1 = KtvAssistantAPIConfig.APIErrorCode.Nothing;
			msg.arg2 = begin; // 判断是加载更多还是刷新
			msg.obj = workList;
			handler.sendMessage(msg);

		} catch (JSONException e) {
			// TODO Auto-generated catch block
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
		ArrayList<WorkInfo> topScoreList = new ArrayList<WorkInfo>();
		try {

			if (null != jsonObject) {
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus)
				{
					JSONObject jsonResultObject = jsonObject.optJSONObject("result");

					if(jsonResultObject != null)
					{
						JSONArray list = jsonResultObject.optJSONArray("worklists");
						if (list.length() > 0) {
							for (int i = 0; i < list.length(); i++) {
								topScoreList.add(new WorkInfo(list.optJSONObject(i)));
							}
						}

						// 缓存
						PDataCache.getInstance().setString(cacheKey,jsonResultObject.toString());
					}
				}
			}
			else
			{
				errorMsg = "服务器异常";
			}
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}

		// 这里加入handler事件，通知刷新界面
		boolean isError = (0 == nStatus);
		Message msg = handler.obtainMessage();
		msg.what = MessageDef.WM_GET_TOP_SCORELIST;// command
		msg.arg1 = isError ? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;// 默认0-缓存，1-网络
		msg.arg2 = begin; // 判断是加载更多还是刷新
		msg.obj = isError ? errorMsg : topScoreList;
		handler.sendMessage(msg);
	}

}