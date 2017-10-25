package assistant.task.playcontrol;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.SongInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

/**
 * @author 宋静
 * @version 创建时间：2013-12-9 下午7:15:39 类说明 切歌
 */
public class SwitchSongTask extends AsyncTask<Void, String, String> {
	private Handler handler;
	
	private int musicId = 0;

	public SwitchSongTask(Handler handler,int musicid) {
		super();
		this.handler = handler;
		musicId = musicid;
	}

	public String getUrl() {
		if (TextUtils.isEmpty(AppStatus.roomId))
			return "";

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.ChangeSingingSong;
		String param = "?ktvid=" + AppStatus.ktvId + "&roomid=" + AppStatus.roomId + "&Musicid="
				+ musicId + "&roompassword=" + AppStatus.room_password;

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
		List<SongInfo> resultList = new ArrayList<SongInfo>();
		try {

			if (null != jsonObject) {
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {
					JSONObject jsonResultObject = jsonObject.optJSONObject("result");

					if (jsonResultObject != null) {
						JSONArray jsonDataList = (null != jsonResultObject) ? jsonResultObject
								.optJSONArray("Songlist") : new JSONArray();
						int songLen = jsonDataList.length();
						for (int i = 0; i < songLen; i++) {
							SongInfo songinfo = new SongInfo(jsonDataList.optJSONObject(i));
							resultList.add(songinfo);
						}
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
		msg.what = (0 == nStatus) ? errorcode : MessageDef.WM_CHANGE_SONG;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.obj = (0 == nStatus) ? errorMsg : resultList;
		handler.sendMessage(msg);
	}
}
