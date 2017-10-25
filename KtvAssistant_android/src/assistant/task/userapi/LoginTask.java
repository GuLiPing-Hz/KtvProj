package assistant.task.userapi;

import java.net.URLEncoder;

import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import assistant.entity.UserInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.share.AccountManager;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;
import assistant.util.ToolUtil;

import com.tiange.phttprequest.PHttpRequest;

public class LoginTask extends AsyncTask<Void, Integer, String> {

	Handler m_handler;
	String m_account;
	String m_pwd;
	boolean m_needMd5;
	
	/**内部会做MD5加密
	 * 
	 * @param handler
	 * @param account
	 * @param pwd
	 */
	public LoginTask(Handler handler,String account,String pwd){
		m_handler = handler;
		m_account = account;
		m_pwd = pwd;
	}
	
	String getUrl(){
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.Login;
		//
		String param = "?id=" + URLEncoder.encode(m_account)+"&pwd="+ToolUtil.md5(m_pwd);

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}
	
	@Override
	protected String doInBackground(Void... arg0) {
		String requestUrl = getUrl();
		
		PHttpRequest request = PHttpRequest.requestWithURL(requestUrl);
		String result = request.startSyncRequestString();
		return result;
	}

	@Override
	protected void onPostExecute(String result) {
		// TODO Auto-generated method stub
		super.onPostExecute(result);
		
		JSONObject jsonObject = PCommonUtil.parseString2JsonObject(result);
		//errorcode无意义
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
						AppStatus.user = new UserInfo(jsonUser);
						AppStatus.user.xgPwd = m_pwd;
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
		Message msg = m_handler.obtainMessage();
		msg.what = MessageDef.WM_LOGIN_RESULT;
		msg.arg1 = (0 == nStatus) ? KtvAssistantAPIConfig.APIErrorCode.Error : KtvAssistantAPIConfig.APIErrorCode.Nothing;
		msg.obj = errorMsg;
		m_handler.sendMessage(msg);
	}

}
