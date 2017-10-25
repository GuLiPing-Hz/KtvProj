package assistant.task.userapi;

import java.net.URLEncoder;

import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.ThirdAccount;
import assistant.entity.UserInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.share.AccountManager;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;
import assistant.util.ToolUtil;

import com.tiange.phttprequest.PHttpRequest;

/**
 * @author 宋静
 * @version 创建时间：2013-12-5 上午10:22:49 类说明 4.1 第三方平台用户登录
 */
public class ThirdLoginTask extends AsyncTask<Void, Void, String> {
	private Handler handler;
	private ThirdAccount account;
	private String roomId;
	private String roomPwd;

	public ThirdLoginTask(Handler tHandler, ThirdAccount tAccount,
			String tRoomId, String tRoomPwd) {
		handler = tHandler;
		account = tAccount;
		roomId = tRoomId;
		roomPwd = tRoomPwd;
	}

	public String getUrl() {
		if (account == null || TextUtils.isEmpty(account.openid)
				|| TextUtils.isEmpty(account.token) || account.type == null 
				|| account.type.getType() == 0)
			return "";
		
		String urlDateStr = "";
		if(account.expires != 0)
			urlDateStr = URLEncoder.encode(ToolUtil.formatTimestampToStr(
				account.expires, "yyyy-MM-dd HH:mm:ss"));

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.ThirdLogin;
		String param = "?openID=" + account.openid + "&accesstoken="
				+ account.token + "&expirationdate=" + urlDateStr
				+ "&atype=" + account.type.getType() + "&roomid=" + roomId
				+ "&roompwd=" + roomPwd;

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
				if (1 == nStatus)
				{
					JSONObject jsonResultObject = jsonObject.optJSONObject("result");
					JSONObject jsonUser = jsonResultObject.optJSONObject("User");
					if (jsonUser != null) {
						AppStatus.user = new UserInfo(jsonUser);
						account.idx = AppStatus.user.userIdx;
						// 保存当前登录的第三方账户
						AppStatus.user.bindAccount(account);
						AppStatus.userIdx = (int)AppStatus.user.userIdx;
						AccountManager.writeMyLocalAccount();
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
		msg.what = MessageDef.WM_THIRD_LOGIN_RESULT;
		msg.arg1 = (0 == nStatus) ? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;
		msg.obj = errorMsg;
		handler.sendMessage(msg);
	}
}
