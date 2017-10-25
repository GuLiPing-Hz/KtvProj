package assistant.task.songapi;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.ArrayList;

import order.adapter.HomeAdapter;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.SongInfo;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.task.PDataCache;

import com.tiange.phttprequest.PHttpRequest;

/**
 * @glp
 	1 获取歌曲列表，
 		一般都是一级菜单下面就是歌曲列表，
 		但是歌星例外，歌星的歌曲列表在二级菜单下面。
 		
 	2 提供搜索功能
 		如果传入keyword就是使用了搜索功能，如果不传，则是搜索全部的意思
 	
 */
public class GetSongListByTopicTask extends AsyncTask<Void, String, String> {

	private Handler handler;
	private int modelid;
	private int topicid;
	int mSingerId = 0;
	String keyword = "";
	boolean isSearch = false;
	private int begin = 1;
	private int num = 200;
	private boolean isreadcache = false;

	private String requestUrl = null;
	private String cacheKey = null;

	public GetSongListByTopicTask(Handler tHandler,int tModelId, int tTopicId,  int tBegin, int tNum) {
		this.handler = tHandler;
		this.modelid = tModelId;
		this.topicid = tTopicId;
		this.begin = tBegin;
		this.num = tNum;
	}
	
	public GetSongListByTopicTask(Handler tHandler,int tModelId, int tTopicId,String keyword,  int tBegin, int tNum) {
		this.handler = tHandler;
		this.modelid = tModelId;
		this.topicid = tTopicId;
		this.keyword = keyword;
		this.begin = tBegin;
		this.num = tNum;
		isSearch = true;
	}
	
	public GetSongListByTopicTask(Handler tHandler,int tModelId, int tTopicId
			,int singerId,  int tBegin, int tNum) {
		this.handler = tHandler;
		this.modelid = tModelId;
		this.topicid = tTopicId;
		this.mSingerId = singerId;
		this.begin = tBegin;
		this.num = tNum;
	}
	
	public GetSongListByTopicTask(Handler tHandler,int tModelId, int tTopicId
			,int singerId,  String keyword,int tBegin, int tNum) {
		this.handler = tHandler;
		this.modelid = tModelId;
		this.topicid = tTopicId;
		this.mSingerId = singerId;
		this.keyword = keyword;
		this.begin = tBegin;
		this.num = tNum;
		isSearch =true;
	}

	protected String getGetSongListByTopicUrl(int tModelId,
			long tTopicId, int tBegin, int tNum) {
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.SongGetSongTopics;
		String param;
		try {
			param = "?Modelid=" + tModelId+ "&Topicid=" 
					+ tTopicId +"&singerid="+mSingerId+"&keyword="+URLEncoder.encode(PCommonUtil.encodeBase64(keyword), "UTF-8")+ "&Begin=" + tBegin + "&Num=" + tNum;
			return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return null;
	}

	@Override
	protected String doInBackground(Void... params) {
		// TODO Auto-generated method stub
		requestUrl = getGetSongListByTopicUrl(modelid, topicid, begin,
				num);
		if(requestUrl == null)
			return null;
		cacheKey = PCommonUtil.md5Encode(requestUrl);
		PDataCache dataCache = PDataCache.getInstance();
		//1小时的有效时间
		if (isreadcache && dataCache.hasCacheForKey(cacheKey, 3600)) {
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
			ArrayList<SongInfo> resultList = new ArrayList<SongInfo>();
			JSONArray jsonResultList = new JSONArray(values[0]);
			int resultListLen = jsonResultList.length();
			for (int i = 0; i < resultListLen; i++) {
				SongInfo songinfo = new SongInfo(
						jsonResultList.getJSONObject(i));
				songinfo.log();
				resultList.add(songinfo);
			}

			if (resultListLen > 0) {
				// 这里加入handler事件，通知刷新界面
				Message msg = handler.obtainMessage();
				msg.what = isSearch ? MessageDef.WM_SONG_SEARCH : MessageDef.WM_GET_SONG_LIST_BY_TOPIC;
				msg.arg1 = KtvAssistantAPIConfig.APIErrorCode.Nothing;
				msg.arg2 = begin; // 判断是加载更多还是刷新
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
		//如果是歌星，则不用解密
		if(modelid == HomeAdapter.ModelType.SINGERS.getType())
			jsonObject = PCommonUtil.parseString2JsonObject(result,false);
		else
			jsonObject = PCommonUtil.parseString2JsonObject(result);
		
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		ArrayList<SongInfo> resultList = new ArrayList<SongInfo>();

		try {

			if (null != jsonObject) {
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {

					JSONObject jsonResultObject = jsonObject
							.optJSONObject("result");
					JSONArray jsonDataList = (null != jsonResultObject) ? jsonResultObject
							.optJSONArray("Songlist") : new JSONArray();
					int songTopicListLen = jsonDataList.length();
					for (int i = 0; i < songTopicListLen; i++) {
						SongInfo songinfo = new SongInfo(
								jsonDataList.optJSONObject(i));
						// songinfo.log();
						resultList.add(songinfo);
					}

					if (songTopicListLen > 0) {
						// 缓存
						PDataCache.getInstance().setString(cacheKey,jsonDataList.toString());
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
		msg.what = isSearch ? MessageDef.WM_SONG_SEARCH : MessageDef.WM_GET_SONG_LIST_BY_TOPIC;// command
		msg.arg1 = isError ? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;// 默认0-缓存，1-网络
		msg.arg2 = begin; // 判断是加载更多还是刷新
		msg.obj = isError ? errorMsg : resultList;
		handler.sendMessage(msg);
	}
}
