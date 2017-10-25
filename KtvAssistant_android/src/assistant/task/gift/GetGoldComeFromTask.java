package assistant.task.gift;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.GoldFromInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

//reword_name 可以去掉
public class GetGoldComeFromTask extends AsyncTask<Void, String, String>{
	private Handler m_handler;
	
	int m_userid;
	int m_page;
	int m_num;

	public GetGoldComeFromTask(Handler handler) {
		super();
		this.m_handler = handler;
	}
	public void setArgu(int userid,int page,int num){
		m_userid = userid;
		m_page = page;
		m_num = num;
	}

	public String getUrl() {

		if(m_userid == 0)
			return "";
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.GoldGetInfo;
		String param = "?userid=" + m_userid + "&begin=" + m_page
				+"&num="+m_num;

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
		List<GoldFromInfo> itemList = new ArrayList<GoldFromInfo>();
		try {

			if (null != jsonObject) {
				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {
					JSONObject jsonResultObject = jsonObject.optJSONObject("result");

					if (jsonResultObject != null) {
						JSONArray list = jsonResultObject.optJSONArray("list");

						for (int i = 0; i < list.length(); i++) {
							itemList.add(new GoldFromInfo(list.optJSONObject(i)));
						}
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
		msg.what = (0 == nStatus) ? errorcode : MessageDef.HTTP_GOLDFROM;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.arg2 = m_page;
		msg.obj = (0 == nStatus) ? errorMsg : itemList;
		m_handler.sendMessage(msg);
	}
}
