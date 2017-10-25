package assistant.task.songapi;

import java.net.URLEncoder;

import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

/**
 * @author 宋静
 * @version 创建时间：2013-12-9 下午6:26:12 类说明 高分榜上榜歌曲抢榜
 */
public class PossessRankWorkTask extends AsyncTask<Void, String, String> {
	private Handler handler;
	private long userIdx;
	private String roomId;
	private long workId;
	private int score;
	private String date;

	public PossessRankWorkTask(Handler handler, long userIdx, String roomId,
			long workId, int score, String date) {
		super();
		this.handler = handler;
		this.userIdx = userIdx;
		this.roomId = roomId;
		this.workId = workId;
		this.score = score;
		this.date = date;
	}

	public String getUrl() {
		if (userIdx <= 0 || TextUtils.isEmpty(roomId) || workId <= 0
				|| score < 0)
			return "";

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.PossessRankWork;
		String param = "?userID=" + userIdx + "&roomid=" + roomId + "&WorkID="
				+ workId + "&score=" + score + "&date=" + URLEncoder.encode(date);

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		String requestUrl = getUrl();
		if (TextUtils.isEmpty(requestUrl))
			return "";

		PHttpRequest request = PHttpRequest.requestWithURL(requestUrl);
		String result = request.startSyncRequestString();
		return result;
	}

	@Override
	protected void onPostExecute(String result) {
		// TODO Auto-generated method stub
		super.onPostExecute(result);

		JSONObject jsonObject = PCommonUtil.parseString2JsonObject(result);
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		int success = 0;
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
						success = jsonResultObject.optInt("success");
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
		Message msg = handler.obtainMessage();
		msg.what = (0 == nStatus) ? errorcode : MessageDef.WM_POSSESS_RANKWORK;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.obj = (0 == nStatus) ? errorMsg : success;
		handler.sendMessage(msg);
	}

}