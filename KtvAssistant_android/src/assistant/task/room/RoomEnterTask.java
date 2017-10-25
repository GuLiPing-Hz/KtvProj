package assistant.task.room;

import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import assistant.entity.RoomInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;

import com.tiange.phttprequest.PHttpRequest;

public class RoomEnterTask extends AsyncTask<Void, Void, String> {

	private Handler handler;
	private String password;

	public RoomEnterTask(Handler handler, String password) {
		super();
		this.handler = handler;
		this.password = password;
	}

	public String getLoginUrl(String tPassword) {
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.RoomLoginEx;
		
		String param ="?roompassword="+tPassword+"&idx="+ AppStatus.userIdx
				+"&pwd="+AppStatus.user.xgPwd;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		// TODO Auto-generated method stub
		String requestUrl = getLoginUrl(password);
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
		AppStatus.isInRoom = false;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		int gold = 0;
		try {
			if (null != jsonObject) {
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {

					JSONObject jsonResultObject = jsonObject.optJSONObject("result");
					RoomInfo roominfo = new RoomInfo(jsonResultObject);
					gold = jsonResultObject.optInt("gold");
					
					// global
					AppStatus.s_nRoomId = jsonResultObject.optInt("xgroomid");
					AppStatus.currentRoomInfo = roominfo;
					AppStatus.roomId = roominfo.roomId;
					AppStatus.ktvId = roominfo.ktvId;
					AppStatus.room_password = password;
					
					AppStatus.isInRoom = true;
				}else{
					AppStatus.isInRoom = false;
				}
			}
			else
			{
				errorMsg = "服务器异常";
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			AppStatus.isInRoom = false;
		}

		// 这里加入handler事件，通知刷新界面
		Message msg = handler.obtainMessage();
		msg.what = (0 == nStatus) ? errorcode : MessageDef.WM_LOGIN_ROOM;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.arg2 = gold;
		msg.obj = errorMsg;
		handler.sendMessage(msg);

	}
}
