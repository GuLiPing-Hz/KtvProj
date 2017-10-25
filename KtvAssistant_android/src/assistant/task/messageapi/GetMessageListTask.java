package assistant.task.messageapi;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.MessageInfo;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

/**
 * @author 宋静
 * @version 创建时间：2013-12-9 下午6:47:27 类说明 获取系统消息列表
 */
public class GetMessageListTask extends AsyncTask<Void, String, String> {
	private Handler handler;
	private long userIdx;
	private String roomId;
	private long since = 0;
	private long before = 0;
	private int number = 20;

	public GetMessageListTask(Handler handler, long userIdx, String roomId,
			long since, long before) {
		super();
		this.handler = handler;
		this.userIdx = userIdx;
		this.roomId = roomId;
		this.since = since;
		this.before = before;
	}

	public GetMessageListTask(Handler handler, long userIdx, String roomId,
			long since, long before, int number) {
		super();
		this.handler = handler;
		this.userIdx = userIdx;
		this.roomId = roomId;
		this.since = since;
		this.before = before;
		this.number = number;
	}

	public String getUrl() {
		if (userIdx < 0 || TextUtils.isEmpty(roomId) || number <= 0)
			return "";

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.GetMessageList;
		String param = "?userID=" + userIdx + "&roomid=" + roomId + "&since="
				+ since + "&before=" + before + "&num=" + number;

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
		List<MessageInfo> resultList = new ArrayList<MessageInfo>();
		try {

			if (null != jsonObject) {
				ShowLog.i("GetMessageListTask", "result: " + jsonObject.toString());

				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {
					JSONObject jsonResultObject = jsonObject
							.optJSONObject("result");

					if (jsonResultObject != null) {
						JSONArray list = jsonResultObject.optJSONArray("msglist");

						if (list.length() > 0) {

							for (int i = 0; i < list.length(); i++) {
								resultList
										.add(new MessageInfo(list.optJSONObject(i)));
							}
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
		Message msg = handler.obtainMessage();
		msg.what = (0 == nStatus) ? errorcode : MessageDef.WM_GET_MESSAGE_LIST;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.obj = (0 == nStatus) ? errorMsg : resultList;
		handler.sendMessage(msg);
	}

}
