package assistant.task.userapi;

import java.net.URLEncoder;

import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

/**
 * @author 宋静
 * @version 创建时间：2013-12-16 下午1:41:14 类说明 修改昵称
 */
public class ModifyNickNameTask extends AsyncTask<Void, Void, String> {
	private Handler handler;
	private long userId;
	private String openId;
	private String nickname;
	private int gender;

	public ModifyNickNameTask(Handler handler, long userId, String openId,
			String nickname, int gender) {
		super();
		this.handler = handler;
		this.userId = userId;
		this.openId = openId;
		this.nickname = nickname;
		this.gender = gender;
	}

	private String getUrl() {
		if (userId <= 0 || TextUtils.isEmpty(nickname))
			return "";

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.ModifyNickName;
		String name = URLEncoder.encode(nickname);
		
		String param = "?userID=" + userId + "&openID=" + openId + "&nickname="
				+ name + "&sex=" + gender;

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
		int success = 0;
		try {

			if (null != jsonObject) {
				ShowLog.i("ModifyNickNameTask result: " + jsonObject.toString());

				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {

					JSONObject jsonResultObject = jsonObject
							.optJSONObject("result");
					success = jsonResultObject.optInt("success");
					
//					JSONObject jsonUser = jsonResultObject
//							.optJSONObject("User");
//					if (jsonUser != null) {
//						AppStatus.user.update(jsonUser);
//						AccountManager.writeMyLocalAccount();
//					}
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
		msg.what = MessageDef.WM_MODIFT_NICKNAME;// command
		msg.arg1 = (0 == success) ? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;// 默认0-缓存，1-网络
		msg.obj = errorMsg;
		handler.sendMessage(msg);

	}

}
