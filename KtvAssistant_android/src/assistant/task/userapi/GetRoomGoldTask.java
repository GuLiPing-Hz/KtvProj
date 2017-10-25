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

import com.tiange.phttprequest.PHttpRequest;

public class GetRoomGoldTask extends AsyncTask<Void, Integer, String> {

	Handler m_handler;
	String m_key;
	
	public GetRoomGoldTask(Handler handler,String key) {
		// TODO Auto-generated constructor stub
		m_handler = handler;
		m_key = key;
	}

	String getUrl(){
		if(AppStatus.user.userIdx == 0)
			return null;
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+KtvAssistantAPIConfig.APIUrl.GetRoomGold;
		String param = "?idx="+AppStatus.userIdx+"&pwd="+m_key;
		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}
	
	@Override
	protected String doInBackground(Void... arg0) {
		// TODO Auto-generated method stub
		String requestUrl = getUrl();
		if(requestUrl==null || TextUtils.isEmpty(requestUrl))
			return "";
		
		PHttpRequest request = PHttpRequest.requestWithURL(requestUrl);
		String result = request.startSyncRequestString();
		return result;
	}

	@Override
	protected void onPostExecute(String result) {
		// TODO Auto-generated method stub
		super.onPostExecute(result);
		
		JSONObject jsonObject = PCommonUtil.parseString2JsonObjectGold(result);
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		int gold = 0;
		try {

			if (null != jsonObject) {
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus)
					gold = jsonObject.optInt("gold");
			}
			else
			{
				errorMsg = "服务器异常";
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		// 这里加入handler事件，通知刷新界面
		boolean isError = (0 == nStatus);
		Message msg = m_handler.obtainMessage();
		msg.what = MessageDef.WM_GET_ROOM_GOLD;// command
		msg.arg1 = isError? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;
		msg.arg2 = gold;
		msg.obj = errorMsg;
		m_handler.sendMessage(msg);
	}

}
