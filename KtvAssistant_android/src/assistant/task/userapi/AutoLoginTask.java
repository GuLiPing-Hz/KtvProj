package assistant.task.userapi;

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
import com.tiange.phttprequest.PHttpRequest.RequestMethodType;

@Deprecated
public class AutoLoginTask extends AsyncTask<Void, Integer, String> {

	Handler m_handler;
	
	int m_userIdx;
	public AutoLoginTask(Handler handler,int idx) {
		m_handler = handler;
		m_userIdx = idx;
	}
	
	private String getUrl() {
		if (m_userIdx == 0)
			return "";

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.AutoLogin;
		
		String param = "?id=" + m_userIdx;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}
	@Override
	protected String doInBackground(Void... arg0) {
		// TODO Auto-generated method stub
		String requestUrl = getUrl();
		if(TextUtils.isEmpty(requestUrl))
			return "";
		
		PHttpRequest request = PHttpRequest.requestWithURL(requestUrl);
		request.setRequestMethodType(RequestMethodType.RequestMethodTypePost);
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
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus)
				{
					JSONObject jsonResultObject = jsonObject.optJSONObject("result");
					JSONObject jsonUser = jsonResultObject.optJSONObject("User");
					if (jsonUser != null) 
					{
						AppStatus.user.updateUserInfo(jsonUser);
						AppStatus.userIdx = (int)AppStatus.user.userIdx;
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
		Message msg = m_handler.obtainMessage();
		msg.what = MessageDef.WM_AUTOLOGIN_SUCCESS;// command
		msg.arg1 = (0 == nStatus) ? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;
		msg.obj = errorMsg;
		m_handler.sendMessage(msg);
	}
}
