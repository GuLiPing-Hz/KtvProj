package assistant.task.userapi;

import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.share.AccountManager;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

/**
 * @author 宋静
 * @version 创建时间：2013-12-5 上午11:36:31 类说明
 */
public class ThirdLogoutTask extends AsyncTask<Void, Void, String> {
	private Handler handler;
	private long userId;
	private String roomId;

	public ThirdLogoutTask(Handler tHandler, long tUserId, String tRoomId) {
		handler = tHandler;
		userId = tUserId;
		roomId = tRoomId;
	}

	public String getUrl() {
		if (userId == 0)
			return "";

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.ThirdLogout;
		String param = "?userID=" + userId + "&roomID=" + roomId;

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
		int nResult = 0;
		try {

			if (null != jsonObject) {
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {

					JSONObject jsonResultObject = jsonObject
							.optJSONObject("result");
					nResult = jsonResultObject.optInt("success");
					AppStatus.user.canAutoLogin = 0;//user信息不清空
					AppStatus.userIdx = 0;
					//AccountManager.clearMyLocalAccount();
					AccountManager.writeMyLocalAccount();//重新写入canAutoLogin字段数据
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
		msg.what = (0 == nStatus) ? errorcode : MessageDef.WM_THIRD_LOGOUT;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.obj = (0 == nStatus) ? errorMsg : nResult;
		handler.sendMessage(msg);

	}

}
