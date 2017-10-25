package assistant.task.collectapi;

import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.SongInfo;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

/**
 * @author 宋静
 * @version 创建时间：2013-12-5 下午3:39:33 类说明 5.1 获取用户收藏列表
 */
public class GetUserCollectListTask extends AsyncTask<Void, Void, String> {

	private Handler handler;
	private long userId;
	private int begin = 1;
	private int num = 200;

	public GetUserCollectListTask(Handler handler, long userId) {
		super();
		this.handler = handler;
		this.userId = userId;
	}

	public String getUrl() {
		if (userId <= 0)
			return "";

		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.GetCollectList;
		String param = "?userID=" + userId + "&begin=" + begin + "&num=" + num;

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}

	@Override
	protected String doInBackground(Void... params) {
		String url = getUrl();
		if (TextUtils.isEmpty(url))
			return "";

		PHttpRequest request = PHttpRequest.requestWithURL(url);
		String result = request.startSyncRequestString();
		return result;
	}

	@Override
	protected void onPostExecute(String result) {
		super.onPostExecute(result);

		JSONObject jsonObject = PCommonUtil.parseString2JsonObject(result);
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		ArrayList<SongInfo> songs = new ArrayList<SongInfo>();
		try {

			if (null != jsonObject) {
				ShowLog.i("GetUserCollectListTask result: "
						+ jsonObject.toString());

				nStatus = jsonObject.optInt("status");
				errorMsg = jsonObject.optString("msg");
				errorcode = jsonObject.optInt("errorcode");
				if (1 == nStatus) {

					JSONObject jsonResultObject = jsonObject
							.optJSONObject("result");
					JSONArray jsonDataList = (null != jsonResultObject) ? jsonResultObject
							.optJSONArray("songlists") : new JSONArray();
					int songLen = jsonDataList.length();
					for (int i = 0; i < songLen; i++) {
						SongInfo songinfo = new SongInfo(
								jsonDataList.optJSONObject(i));
						// songinfo.log();
						songs.add(songinfo);
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
		boolean isError = (0 == nStatus);
		Message msg = handler.obtainMessage();
		msg.what = MessageDef.WM_GET_COLLECT_LIST;// command
		msg.arg1 = isError ? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;
		msg.obj = isError ? errorMsg : songs;
		handler.sendMessage(msg);

	}

}
