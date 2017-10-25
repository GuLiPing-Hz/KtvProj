package assistant.task.gift;

import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.GiftSendRecv;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;

import com.tiange.phttprequest.PHttpRequest;

public class GiftSRRecordTask extends AsyncTask<Void, Integer, String>{

	Handler m_handler;
	int m_idx = 0;
	int m_page = 1;
	int m_size = 8;
	//1 收礼  2 送礼
	int m_type = 0;
	
	public GiftSRRecordTask(Handler handler) {
		super();
		this.m_handler = handler;
	}
	public void setArgu(int idx,int type){
		m_idx = idx;
		m_type = type;
	}
	public void setArguBase(int  page,int size){
		m_page = page;
		m_size = size;
	}
	public String getUrl() {
		
		if(m_idx==0 || m_type==0)
			return "";
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.GiftSendRecvInfo;
		String param = "?idx=" + m_idx + "&type=" + m_type+"&page="+m_page
				+"&size="+m_size;
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

		JSONObject jsonObject = PCommonUtil.parseString2JsonObject(result);
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		ArrayList<GiftSendRecv> itemList = new ArrayList<GiftSendRecv>();
		try {
			if (null != jsonObject) {
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if(nStatus == 1)
				{
					JSONObject jsonResultObject = jsonObject.optJSONObject("result");
					if(jsonResultObject != null)
					{
						JSONArray listObj = jsonResultObject.optJSONArray("list");
						for(int i=0;i<listObj.length();i++)
							itemList.add(new GiftSendRecv(listObj.getJSONObject(i),m_type));
					}
				}
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
		msg.what = (0 == nStatus) ? errorcode : MessageDef.HTTP_GETGIFTRECORD;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.arg2 = m_page;
		msg.obj = (0 == nStatus) ? errorMsg : itemList;
		m_handler.sendMessage(msg);
	}
}
