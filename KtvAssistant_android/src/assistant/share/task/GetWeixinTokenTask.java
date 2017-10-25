package assistant.share.task;

import org.json.JSONException;
import org.json.JSONObject;

import tiange.ktv.assistant.wxapi.WXEntryActivity;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import assistant.entity.WeixinToken;

import com.tiange.phttprequest.PHttpRequest;

public class GetWeixinTokenTask extends AsyncTask<Void, Integer, String> {

	public static final int GETTOKEN = 1;
	public static final int GETREFRESHTOKEN = 2;
	
	Handler mHandler;
	String mCodeOrReToken;
	int mType;
	
	String getRequestUrl(){
		if(mType == 1)
		{
			return "https://api.weixin.qq.com/sns/oauth2/access_token?appid="+WXEntryActivity.AppID
					+"&secret="+WXEntryActivity.AppSecret+"&code="+mCodeOrReToken+"&grant_type=authorization_code";
		}
		else if(mType == 2)
		{
			return "https://api.weixin.qq.com/sns/oauth2/refresh_token?appid="+WXEntryActivity.AppID
					+"&grant_type=refresh_token&refresh_token="+mCodeOrReToken;
		}
		return null;
	}
	
	public GetWeixinTokenTask(Handler handler,int type,String codeOrReToken) {
		// TODO Auto-generated constructor stub
		mHandler = handler;
		mCodeOrReToken = codeOrReToken;
		mType = type;
	}

	@Override
	protected String doInBackground(Void... arg0) {
		// TODO Auto-generated method stub
		String requestUrl = getRequestUrl();
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
		
		try {
			boolean success = true;
			
			WeixinToken data = new WeixinToken();
			if(result != null)
			{
				JSONObject tempJsonObject = new JSONObject(result);
				
				if(mType == 1)
				{
					String openId = tempJsonObject.optString("openid");
					String refreshtoken = "";
					if(TextUtils.isEmpty(openId))
						success = false;
					else
						refreshtoken = tempJsonObject.optString("refresh_token");
					data.openId = openId;
					data.reortoken = refreshtoken;
				}
				else if(mType == 2)
				{
					String openId = tempJsonObject.optString("openid");
					String token = "";
					if(TextUtils.isEmpty(openId))
						success = false;
					else
						token = tempJsonObject.optString("access_token");
					data.openId = openId;
					data.reortoken = token;
				}
			}
			else
			{
				success = false;
			}
			
			
			Message msg = mHandler.obtainMessage();
			msg.what = mType;
			msg.arg1 = success?1:0;
			msg.obj = data;
			mHandler.sendMessage(msg);
			
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
