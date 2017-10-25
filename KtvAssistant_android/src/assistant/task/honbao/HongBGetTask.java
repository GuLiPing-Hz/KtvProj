package assistant.task.honbao;

import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;

import com.tiange.phttprequest.PHttpRequest;

public class HongBGetTask extends AsyncTask<Void, Integer, String> {

	Handler mHandler;
	String mHBId;
	
	public HongBGetTask(Handler handler,String hbId){
		mHandler = handler;
		mHBId = hbId;
	}
	
	String getUrl(){
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain+KtvAssistantAPIConfig.APIUrl.GetHongBao;
		String param = "?Idx="+AppStatus.userIdx+"&ID="+mHBId;
		
		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}
	
	@Override
	protected String doInBackground(Void... params) {
		// TODO Auto-generated method stub
		String requestUrl = getUrl();
		
		PHttpRequest request = PHttpRequest.requestWithURL(requestUrl);
		String result = request.startSyncRequestString();
		return result;
	}

	@Override
	protected void onPostExecute(String result) {
		// TODO Auto-generated method stub
		super.onPostExecute(result);
		
		int nStatus = 0;
		int errorCode = KtvAssistantAPIConfig.APIErrorCode.Error;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		JSONObject jsonObject = PCommonUtil.parseString2JsonObject(result);
		if(jsonObject != null)
		{
			nStatus = jsonObject.optInt("status");
			errorCode = jsonObject.optInt("errorcode");
			errorMsg = jsonObject.optString("msg");
			if(nStatus != 1)
				Log.e("HongBGetTask",errorMsg);
		}
		else
		{
			errorMsg = "服务器异常";
		}
		
		boolean isError = nStatus == 0;
		Message msg = mHandler.obtainMessage();
		msg.what = MessageDef.WM_GETHONGBAO;
		msg.arg1 = isError?errorCode:KtvAssistantAPIConfig.APIErrorCode.Nothing;
		msg.obj = isError?errorMsg:null;
		mHandler.sendMessage(msg);
	}
}
