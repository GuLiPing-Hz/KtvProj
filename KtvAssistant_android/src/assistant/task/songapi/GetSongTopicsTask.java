package assistant.task.songapi;

import java.util.ArrayList;

import order.adapter.HomeAdapter;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.TopicInfo;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.task.PDataCache;

import com.tiange.phttprequest.PHttpRequest;

/**
 * @glp
	获取一级菜单列表,一般一级菜单列表不会多余20个，这里默认请求100个，如果以后超过100个（应该不大可能）
 */
public class GetSongTopicsTask extends AsyncTask<Void, String, String> {

	protected Handler handler;
	protected int modelid;
	
	boolean isNeedTopicId = false;
	int topicid;
	protected long begin = 1;
	protected int num = 100;
	protected boolean isreadcache = true;

	protected String requestUrl = null;
	protected String cacheKey = null;

	public GetSongTopicsTask(Handler tHandler, int tModelId) {
		this.handler = tHandler;
		this.modelid = tModelId;
	}
	
	public GetSongTopicsTask(Handler tHandler, int tModelId,int topicid,long begin,int num) {
		this.handler = tHandler;
		this.modelid = tModelId;
		this.topicid = topicid;
		isNeedTopicId = true;
		this.begin = begin;
		this.num = num;
	}

	protected String getGetSongTopicsUrl( int tModelId,long tBegin, int tNum) {
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.SongGetSongTopics;
		
		String param = "?Modelid=" + tModelId+ "&keyword=&Begin=" + tBegin + "&Num=" + tNum;
		if(isNeedTopicId)
			param += ("&topicid="+topicid);

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		// TODO Auto-generated method stub
		requestUrl = getGetSongTopicsUrl(modelid, begin, num);
		cacheKey = PCommonUtil.md5Encode(requestUrl);
		PDataCache dataCache = PDataCache.getInstance();
		if (isreadcache && dataCache.hasCacheForKey(cacheKey, 3600*24)) {
			String cacheData = dataCache.stringForKey(cacheKey);
			if (null != cacheData) {
				publishProgress(cacheData);
			}
			return "IS_READ_CACHE";
		} else {

			if (isreadcache && dataCache.hasCacheForKey(cacheKey)) {
				String cacheData = dataCache.stringForKey(cacheKey);
				if (null != cacheData) {
					publishProgress(cacheData);
				}
			}

			PHttpRequest request = PHttpRequest.requestWithURL(requestUrl);
			if(isNeedTopicId && modelid == HomeAdapter.ModelType.SINGERS.getType())
				request.setRequestSocketTimeOut(20000);
			String result = request.startSyncRequestString();
			return result;
		}
	}

	@Override
	protected void onProgressUpdate(String... values) {
		// TODO Auto-generated method stub
		super.onProgressUpdate(values);

		try {
			ArrayList<TopicInfo> resultList = new ArrayList<TopicInfo>();
			JSONArray jsonResultList = new JSONArray(values[0]);
			int resultListLen = jsonResultList.length();
			for (int i = 0; i < resultListLen; i++) {
				TopicInfo topicinfo = new TopicInfo(jsonResultList.getJSONObject(i));
				topicinfo.toString();
				resultList.add(topicinfo);
			}

			if (resultListLen > 0) {
				// 这里加入handler事件，通知刷新界面
				Message msg = handler.obtainMessage();
				msg.what = MessageDef.WM_GET_SONG_TOPICS;
				msg.arg1 = 0;// 默认0-缓存，1-网络
				msg.obj = resultList;
				handler.sendMessage(msg);
			}
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

		JSONObject jsonObject = null;
		if(isNeedTopicId && modelid == HomeAdapter.ModelType.SINGERS.getType())
			jsonObject = PCommonUtil.parseString2JsonObject(result,false);
		else
			jsonObject = PCommonUtil.parseString2JsonObject(result);
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		ArrayList<TopicInfo> resultList = new ArrayList<TopicInfo>();

		try {
			if (null != jsonObject) {
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {

					JSONObject jsonResultObject = jsonObject
							.optJSONObject("result");
					JSONArray jsonDataList = (null != jsonResultObject) ? jsonResultObject
							.optJSONArray("songtopics") : new JSONArray();
					int songTopicListLen = jsonDataList.length();
					for (int i = 0; i < songTopicListLen; i++) {
						TopicInfo topicinfo = new TopicInfo(jsonDataList.optJSONObject(i));
						topicinfo.toString();
						resultList.add(topicinfo);
					}

					if (songTopicListLen > 0) {
						// 缓存
						PDataCache.getInstance().setString(cacheKey,
								jsonDataList.toString());
					}

				}
			}
			else
			{
				errorMsg = "服务器异常";
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		// 这里加入handler事件，通知刷新界面
		boolean isError = (0 == nStatus);
		Message msg = handler.obtainMessage();
		msg.what = MessageDef.WM_GET_SONG_TOPICS;// command
		msg.arg1 =  isError? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;
		msg.arg2 = 1;// 默认0-缓存，1-网络
		msg.obj = isError ? errorMsg : resultList;
		handler.sendMessage(msg);
	}
}
