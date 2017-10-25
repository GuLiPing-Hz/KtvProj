package assistant.task.songapi;

import java.net.URLEncoder;
import java.util.ArrayList;

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
 * 
 * @author apple 歌曲搜索 Assistant/Search 参数：int roomid, string Keyword, int
 *         Modelid, （若为0，则查询全部歌曲） int? Topicid, int begin, int num
 */
public class SongSearchTask extends AsyncTask<Void, String, String> {

	private Handler handler;
	private String roomid;
	private String keyword;
	private int modelid;
	private long topicid = 0;
	private int begin = 1;
	private int num = 200;
	private boolean isreadcache = true;

	private String requestUrl = null;
	private String cacheKey = null;

	public SongSearchTask(Handler tHandler, String tRoomId, String tKeyword,
			int tModelId, int tBegin, int tNum) {
		this.handler = tHandler;
		this.roomid = tRoomId;
		this.keyword = tKeyword;
		this.modelid = tModelId;
		this.begin = tBegin;
		this.num = tNum;
	}

	public SongSearchTask(Handler tHandler, String tRoomId, String tKeyword,
			int tModelId, long tTopicId, int tBegin, int tNum) {
		this.handler = tHandler;
		this.roomid = tRoomId;
		this.keyword = tKeyword;
		this.modelid = tModelId;
		this.topicid = tTopicId;
		this.begin = tBegin;
		this.num = tNum;
	}

	public SongSearchTask(Handler tHandler, String tRoomId, String tKeyword,
			int tModelId, long tTopicId) {
		this.handler = tHandler;
		this.roomid = tRoomId;
		this.keyword = tKeyword;
		this.modelid = tModelId;
		this.topicid = tTopicId;
	}

	protected String getSongSearchUrl(String tRoomId, String tKeyword,
			int tModelId, int tBegin, int tNum) {

		String encodeKeyword = URLEncoder.encode(tKeyword);

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.SongSearch;
		String param = "?roomid=" + tRoomId + "&Keyword=" + encodeKeyword
				+ "&Modelid=" + tModelId + "&Begin=" + tBegin + "&Num=" + tNum;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	protected String getSongSearchUrl(String tRoomId, String tKeyword,
			int tModelId, long tTopicId, int tBegin, int tNum) {

		String encodeKeyword = URLEncoder.encode(tKeyword);

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.SongSearch;
		String param = "?roomid=" + tRoomId + "&Keyword=" + encodeKeyword
				+ "&Modelid=" + tModelId + "&Topicid=" + tTopicId + "&Begin="
				+ tBegin + "&Num=" + tNum;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		// TODO Auto-generated method stub
		if (topicid > 0) 
			requestUrl = getSongSearchUrl(roomid, keyword, modelid, topicid,begin, num);
		else
			requestUrl = getSongSearchUrl(roomid, keyword, modelid, begin, num);
		
		cacheKey = PCommonUtil.md5Encode(requestUrl);
		PDataCache dataCache = PDataCache.getInstance();
		if (isreadcache && dataCache.hasCacheForKey(cacheKey, 3600))
		{
			String cacheData = dataCache.stringForKey(cacheKey);
			if (null != cacheData) 
				publishProgress(cacheData);
			return "IS_READ_CACHE";
		} 
		else 
		{
			PHttpRequest request = PHttpRequest.requestWithURL(requestUrl);
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
			ArrayList<SongInfo> resultList = new ArrayList<SongInfo>();
			JSONArray jsonResultList = new JSONArray(values[0]);
			int resultListLen = jsonResultList.length();
			for (int i = 0; i < resultListLen; i++) {
				SongInfo songinfo = new SongInfo(
						jsonResultList.getJSONObject(i));
				songinfo.log();
				resultList.add(songinfo);
			}

			if (resultListLen > 0) 
			{
				// 这里加入handler事件，通知刷新界面
				Message msg = handler.obtainMessage();
				msg.what = MessageDef.WM_SONG_SEARCH;
				msg.arg1 = 0;// 默认0-缓存，1-网络
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

		if (TextUtils.equals(result, "IS_READ_CACHE"))
			return;

		JSONObject jsonObject = PCommonUtil.parseString2JsonObject(result);
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		ArrayList<SongInfo> resultList = new ArrayList<SongInfo>();

		try {

			if (null != jsonObject) 
			{
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) 
				{
					JSONObject jsonResultObject = jsonObject.optJSONObject("result");
					JSONArray jsonDataList = (null != jsonResultObject) ? jsonResultObject
							.optJSONArray("Songlist") : new JSONArray();
					int songTopicListLen = jsonDataList.length();
					for (int i = 0; i < songTopicListLen; i++) 
					{
						SongInfo songinfo = new SongInfo(
								jsonDataList.optJSONObject(i));
						songinfo.log();
						resultList.add(songinfo);
					}

					if (songTopicListLen > 0)// 缓存
						PDataCache.getInstance().setString(cacheKey,jsonDataList.toString());

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
		msg.what = MessageDef.WM_SONG_SEARCH;// command
		msg.arg1 = isError ? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;// 默认0-缓存，1-网络
		msg.arg2 = begin; // 判断是加载更多还是刷新
		msg.obj = isError ? errorMsg : resultList;
		handler.sendMessage(msg);

	}

}
