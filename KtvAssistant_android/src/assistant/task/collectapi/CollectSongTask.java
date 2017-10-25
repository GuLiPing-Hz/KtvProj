package assistant.task.collectapi;

import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.ErrorSongInfo;
import assistant.entity.SongInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

/**
 * @author 宋静
 * @version 创建时间：2013-12-5 下午3:46:24 类说明 收藏歌曲
 */
public class CollectSongTask extends AsyncTask<Void, Void, String> {

	private Handler handler;
	private long userId;
	private int songId;
	protected SongInfo song;

	public CollectSongTask(Handler handler, long userId, int song) {
		super();
		this.handler = handler;
		this.userId = userId;
		this.songId = song;
	}

	public String getUrl() {
		if (userId == 0 || songId == 0)
			return "";

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.CollectSong;
		String param = "?userID=" + userId + "&Songid=" + songId;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		String url = getUrl();
		if (TextUtils.isEmpty(url))
			return "";

		PHttpRequest request = PHttpRequest.requestWithURL(url);
		String result = request.startSyncRequestString();
		return result;
	}

	@Override
	protected void onPostExecute(String result) {
		super.onPostExecute(result);

		JSONObject jsonObject = PCommonUtil.parseString2JsonObject(result);
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		try {

			if (null != jsonObject) {
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
			} else {
				nStatus = 0;
				errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
				errorMsg = "服务器异常";
			}
		} catch (Exception e) {
			ShowLog.showException(e);
		}

		// 这里加入handler事件，通知刷新界面
		boolean isError = (0 == nStatus);
		Message msg = handler.obtainMessage();
		msg.what = MessageDef.WM_COLLECT_SONG;// command
		msg.arg1 = isError ? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;
		msg.arg2 = songId;
		msg.obj = isError ?  new ErrorSongInfo(errorMsg, songId) : null;
		handler.sendMessage(msg);

	}
}
