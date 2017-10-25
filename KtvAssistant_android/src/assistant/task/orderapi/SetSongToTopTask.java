package assistant.task.orderapi;

import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import assistant.entity.ErrorSongInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

/**
 * 
 * @author apple
 * 
 *         置顶歌曲：Assistant/SetSongToTop 参数int ktvid, int roomid, int Songid
 */
public class SetSongToTopTask extends AsyncTask<Void, String, String> {

	protected Handler handler;
	protected String ktvid;
	protected String roomid;
	protected String password;
	protected int songid;
	protected String requestUrl = null;

	public SetSongToTopTask(Handler tHandler, String tKtvId, String tRoomId,
			String tPassword, int songid) {
		this.handler = tHandler;
		this.ktvid = tKtvId;
		this.roomid = tRoomId;
		this.password = tPassword;
		this.songid = songid;
	}

	protected String getSetSongToTopUrl(String tKtvId, String tRoomId,
			String tPassword, long tSongId) {
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.SongSetSongToTop;
		String param = "?ktvid=" + tKtvId + "&roomid=" + tRoomId
				+ "&roompassword=" + tPassword + "&Songid=" + tSongId;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		// TODO Auto-generated method stub
		requestUrl = getSetSongToTopUrl(ktvid, roomid, password, songid);

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

		try {

			if (null != jsonObject) {
				ShowLog.i("SetSongToTopTask result: " + jsonObject.toString());

				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {
					errorMsg = "";
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
		msg.what = MessageDef.WM_SET_SONG_TO_TOP;// command
		msg.arg1 =  isError ? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;
		msg.obj = isError ? new ErrorSongInfo(errorMsg, songid) : songid;
		handler.sendMessage(msg);
	}
}
