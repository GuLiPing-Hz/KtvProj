package assistant.task.playcontrol;

import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

public class VoicePadTask extends AsyncTask<Void, Integer, String> {

	Handler m_handler;
	int m_voice;
	int m_mic;
	int m_tone;
	int m_effect;
	
	public VoicePadTask(Handler m_handler){
		super();
		this.m_handler = m_handler;
	}
	
	public void setArgu(int m_voice, int m_mic, int m_tone,
			int m_effect) {
		this.m_voice = m_voice;
		this.m_mic = m_mic;
		this.m_tone = m_tone;
		this.m_effect = m_effect;
	}

	String getUrl(){
		if (TextUtils.isEmpty(AppStatus.roomId))
			return "";
		
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.VoicePad;
		String param = "?ktvid=" + AppStatus.ktvId + "&roomid=" + AppStatus.roomId + "&roompassword=" 
				+ AppStatus.room_password+ "&voice="+ m_voice +"&mic="+m_mic+"&tone="
				+m_tone+"&effect="+m_effect;
		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}
	@Override
	protected String doInBackground(Void... arg0) {
		// TODO Auto-generated method stub
		String requestUrl = getUrl();
		if (TextUtils.isEmpty(requestUrl))
			return "";
		
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
				ShowLog.i("ChangeSingingSongTask",
						"result: " + jsonObject.toString());

				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
			}
			else
			{
				errorMsg = "服务器异常";
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		Log.i("VoicePadTase", (0 == nStatus) ?"FAILED":"OK");
		// 这里加入handler事件，通知刷新界面
		Message msg = m_handler.obtainMessage();
		msg.what = (0 == nStatus) ? errorcode : MessageDef.HTTP_VOICE_PAD;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.obj = (0 == nStatus) ? errorMsg : null;
		m_handler.sendMessage(msg);
	}
}
