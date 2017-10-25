package assistant.task.messageapi;

import java.net.URLEncoder;

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
 * @version 创建时间：2013-12-9 下午7:42:38 类说明 根据消息ID和消息类型获取消息的详细信息
 */
public class GetMessageDatailTask extends AsyncTask<Void, String, String> {
	private Handler handler;
	private long userIdx;
	private String roomId;
	private MessageInfo info;

	public GetMessageDatailTask(Handler handler, long userIdx, String roomId,
			MessageInfo info) {
		super();
		this.handler = handler;
		this.userIdx = userIdx;
		this.roomId = roomId;
		this.info = info;
	}

	public String getUrl() {
		if (TextUtils.isEmpty(roomId) || info == null || info.id < 0 || TextUtils.isEmpty(info.date))
			return "";

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.GetMessageDetail;
		String param = "?userID=" + userIdx + "&roomid=" + roomId + "&msgid="
				+ info.id + "&workid=" + info.workId + "&msgtype="
				+ info.msgType + "&point=" + info.point + "&date="
				+ URLEncoder.encode(info.date);

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		String requestUrl = getUrl();
		if (TextUtils.isEmpty(requestUrl))
			return "";
		
		if(handler == null)
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
		MessageInfo msgInfo = null;
		try {

			if (null != jsonObject) {
				ShowLog.i("GetMessageListTask",
						"result: " + jsonObject.toString());

				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {
					JSONObject jsonResultObject = jsonObject
							.optJSONObject("result");

					if (jsonResultObject != null) {
						msgInfo = new MessageInfo(
								jsonResultObject.optJSONObject("msgdetail"));
					} else {
						nStatus = 0;
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
		msg.what = (0 == nStatus) ? errorcode
				: MessageDef.WM_GET_MESSAGE_DETAIL;// command
		msg.arg1 = 1;// 默认0-缓存，1-网络
		msg.obj = (0 == nStatus) ? errorMsg : msgInfo;
		handler.sendMessage(msg);

		if (errorcode > 0) {
			handler.sendMessage(handler.obtainMessage(errorcode, errorMsg));
		}
	}

}
