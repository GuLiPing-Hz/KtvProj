package assistant.task.gift;

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

public class GiftExchangeTask extends AsyncTask<Void, Integer, String>{

	Handler m_handler;
	int m_ktvid;
	int m_giftid;
	int m_giftnum;
	
	public GiftExchangeTask(Handler handler) {
		super();
		this.m_handler = handler;
	}
	public void setArgu(int ktvid,int giftid,int giftnum){
		m_ktvid = ktvid;
		m_giftid = giftid;
		m_giftnum = giftnum;
	}

	public String getUrl() {
		
		if(m_ktvid==0 || m_giftid==0 || m_giftnum==0)
			return "";
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.GiftExchange;
		String param = "?idx=" + AppStatus.userIdx + "&giftid=" + m_giftid+"&ktvid="
				+m_ktvid+"&giftnum="+m_giftnum;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
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

		JSONObject jsonObject = PCommonUtil.parseString2JsonObjectGold(result);
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		int gold = 0;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		try {

			if (null != jsonObject) {
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
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
		Message msg = m_handler.obtainMessage();
		boolean isError = (0 == nStatus);
		msg.what = MessageDef.HTTP_GIFTEXCHANGE;// command
		msg.arg1 = isError?errorcode:KtvAssistantAPIConfig.APIErrorCode.Nothing;// 默认0-缓存，1-网络
		msg.arg2 = gold;
		msg.obj = isError ? errorMsg : null;
		m_handler.sendMessage(msg);
	}
}
