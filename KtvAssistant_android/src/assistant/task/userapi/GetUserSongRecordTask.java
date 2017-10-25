package assistant.task.userapi;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.SongRecord;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;
import assistant.util.ShowLog;

import com.tiange.phttprequest.PHttpRequest;

public class GetUserSongRecordTask extends AsyncTask<Void, Integer, String> {

	Handler mHandler;
	long mIdx;
	
	public GetUserSongRecordTask(Handler handler,long idx)
	{
		mHandler = handler;
		mIdx = idx;
	}
	
	String getUrl(long idx)
	{
		if(idx == 0)
			return null;
		
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain
				+ KtvAssistantAPIConfig.APIUrl.SongRecordGet;
		
		String param = "?id=" + idx+"&TopNum=20";

		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}
	
	@Override
	protected String doInBackground(Void... params) {
		// TODO Auto-generated method stub
		String requestUrl = getUrl(mIdx);
		if( requestUrl == null)
			return "";
		
		PHttpRequest request = PHttpRequest.requestWithURL(requestUrl);
		String result = request.startSyncRequestString();
		return result;
	}

	@Override
	protected void onPostExecute(String result) {
		// TODO Auto-generated method stub
		super.onPostExecute(result);
		
		List<SongRecord> resultList = new ArrayList<SongRecord>();
		JSONObject jsonObject = PCommonUtil.parseString2JsonObject(result);
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
					JSONArray jsonDataList = (null != jsonResultObject) ? jsonResultObject
							.optJSONArray("UserSongLogList") : new JSONArray();
					int length = jsonDataList.length();
					for (int i = 0; i < length; i++) {
						SongRecord songinfo = new SongRecord(jsonDataList.optJSONObject(i));
						resultList.add(songinfo);
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
		Message msg = mHandler.obtainMessage();
		msg.what = MessageDef.WM_GETUSER_SONGRECORD;// command
		msg.arg1 = isError? errorcode : KtvAssistantAPIConfig.APIErrorCode.Nothing;
		msg.obj = isError?errorMsg:resultList;
		mHandler.sendMessage(msg);
	}

}
