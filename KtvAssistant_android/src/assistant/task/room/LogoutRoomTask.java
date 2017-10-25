package assistant.task.room;

import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

/**
 * 登出后，需要清理缓存数据
 * 
 * @author pig
 * 
 */
public class LogoutRoomTask extends AsyncTask<Void, String, String> {

	private Handler handler;
	private String roomid;
	private String roompassword;
	private long userId;

	public LogoutRoomTask(Handler handler, String roomid, String roompassword,
			long userId) {
		super();
		this.handler = handler;
		this.roomid = roomid;
		this.roompassword = roompassword;
		this.userId = userId;
	}

	public String getLogoutUrl() {
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.RoomLogout;
		String param = "?roomid=" + roomid + "&roompassword=" + roompassword
				+ "&userId=" + userId;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		// TODO Auto-generated method stub

		String requestUrl = getLogoutUrl();
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

		try {

			if (null != jsonObject) {
				ShowLog.i("LogoutRoomTask result: " + jsonObject.toString());

				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
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
		msg.what = (0 == nStatus) ? errorcode : MessageDef.WM_LOGOUT_ROOM;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.obj = errorMsg;
		handler.sendMessage(msg);

	}

}
