package assistant.task.gift;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.GiftExchangeRecord;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;

import com.tiange.phttprequest.PHttpRequest;
//礼物兑换记录，无需缓存

//需要增加新的http协议，获取已领取，已过期的兑换记录并按照时间排序
public class GetGiftExchangeRecordTask extends AsyncTask<Void, Integer, String>{

	private Handler handler;
	
	int m_idx = 0;
	int m_type = 0;
	int m_page = 1;
	int m_size = 20;

	public GetGiftExchangeRecordTask(Handler handler) {
		super();
		this.handler = handler;
	}
	public void setArgu(int idx,int type){
		m_idx = idx;
		m_type = type;
	}
	public void setArguBase(int page,int size){
		m_page = page;
		m_size = size;
	}

	public String getUrl() {
		
		if(m_idx == 0)
			return "";
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.GiftExchangeInfo;
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
		List<GiftExchangeRecord> itemList = new ArrayList<GiftExchangeRecord>();
		try {

			if (null != jsonObject) {
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {
					JSONObject jsonResultObject = jsonObject.optJSONObject("result");

					if (jsonResultObject != null) {
						JSONArray list = jsonResultObject.optJSONArray("ktvgiftloglist");

						for (int i = 0; i < list.length(); i++) {
							itemList.add(new GiftExchangeRecord(list.optJSONObject(i),m_type));
						}
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
		Message msg = handler.obtainMessage();
		msg.what = (0 == nStatus) ? errorcode : MessageDef.HTTP_GETGIFTRECORD;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.arg2 = m_page;
		msg.obj = (0 == nStatus) ? errorMsg : itemList;
		handler.sendMessage(msg);
	}
}
