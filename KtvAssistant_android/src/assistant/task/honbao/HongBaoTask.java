package assistant.task.honbao;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import assistant.entity.KtvHongBaoInfo;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.PCommonUtil;

import com.tiange.phttprequest.PHttpRequest;

public class HongBaoTask extends AsyncTask<Void, Integer, String> {

	Handler mHandler;
	int mType;
	
	public HongBaoTask(Handler handler,int type){
		mHandler= handler;
		mType = type;
	}
	
	String getUrl(){
		String baseUrl = KtvAssistantAPIConfig.KtvAssistantAPIDomain+KtvAssistantAPIConfig.APIUrl.GetHongBaoList;
		String param = "?idx="+AppStatus.userIdx+"&type="+mType;
		
		return PCommonUtil.generateAPIStringWithSecret(baseUrl, param);
	}
	
	@Override
	protected String doInBackground(Void... arg0) {
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
		
		JSONObject jsonObject = PCommonUtil.parseString2JsonObject(result);
		int nStatus = 0, errorcode = KtvAssistantAPIConfig.APIErrorCode.Error;
		String errorMsg = KtvAssistantAPIConfig.ErrorMsgUnknow;
		List<KtvHongBaoInfo> resultList = new ArrayList<KtvHongBaoInfo>();
		
		if(jsonObject != null)
		{
			nStatus = jsonObject.optInt("status");
			errorcode = jsonObject.optInt("errorcode");
			errorMsg = jsonObject.optString("msg");
			
			if(nStatus == 1)
			{
				JSONObject jsonResult = jsonObject.optJSONObject("result");
				if(jsonResult != null)
				{
					JSONArray array = jsonResult.optJSONArray("RewardInfolist");
					for(int i=0;i<array.length();i++)
					{
						resultList.add(new KtvHongBaoInfo(array.optJSONObject(i)));
					}
				}
			}
		}
		else
		{
			errorMsg = "服务器异常";
		}
		
		boolean isError = nStatus == 0;
		Message msg = mHandler.obtainMessage();
		msg.what = MessageDef.WM_GETHONGBAO_LIST;
		msg.arg1 = isError?errorcode:KtvAssistantAPIConfig.APIErrorCode.Nothing;
		msg.obj = isError?errorMsg:resultList;
		mHandler.sendMessage(msg);
	}

}
