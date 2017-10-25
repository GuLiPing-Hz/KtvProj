package assistant.task.orderapi;

import org.json.JSONArray;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import assistant.entity.SongInfo;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

public class GetCurrentSongTask extends AsyncTask<Void, String, String> {

	private Handler handler;
	private String ktvid;
	private String roomid;
	protected String password;
	private String requestUrl = null;

	public GetCurrentSongTask(Handler tHandler, String tKtvId, String tRoomId,
			String tPassword) {
		this.handler = tHandler;
		this.ktvid = tKtvId;
		this.roomid = tRoomId;
		this.password = tPassword;
	}

	protected String getUrl(String tKtvId, String tRoomId,
			String tPassword) {
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.SongGetCurrentSong;
		String param = "?ktvid=" + tKtvId + "&roomid=" + tRoomId
				+ "&roompassword=" + tPassword;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		// TODO Auto-generated method stub
		requestUrl = getUrl(ktvid, roomid, password);

		PHttpRequest request = PHttpRequest.requestWithURL(requestUrl);
		request.setRequestSocketTimeOut(20000);
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
		SongInfo currentSongInfo = null;

		try {

			if (null != jsonObject) {
				ShowLog.i("GetCurrentSongTask result: " + jsonObject.toString());
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {

					JSONObject jsonResultObject = jsonObject
							.optJSONObject("result");
					JSONArray jsonDataList = (null != jsonResultObject) ? jsonResultObject
							.optJSONArray("Songlist") : new JSONArray();
					int songListLen = jsonDataList.length();
					if (songListLen > 0) {
						currentSongInfo = new SongInfo(
								jsonDataList.optJSONObject(0));
						currentSongInfo.log();
					} else {
						nStatus = 0;
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
		Message msg = handler.obtainMessage();
		msg.what = (0 == nStatus) ? errorcode : MessageDef.WM_GET_CURRENT_SONG;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.obj = (0 == nStatus) ? errorMsg : currentSongInfo;
		handler.sendMessage(msg);

	}

}
