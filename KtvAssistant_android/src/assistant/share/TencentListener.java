package assistant.share;

import org.json.JSONException;
import org.json.JSONObject;

import assistant.util.ShowLog;

import com.tencent.tauth.IUiListener;
import com.tencent.tauth.UiError;

/**
 * @author song
 * @version 创建时间：2013-4-10 下午4:23:56
 * 类说明 调用SDK已经封装好的接口时，登录回调
 */
public class TencentListener  implements IUiListener{

	protected static String accesstoken;
	protected static String openId;
	protected static long expires_in;
	
	@Override
	public void onCancel() {
		// TODO Auto-generated method stub
		ShowLog.e("TencentListener", "onCancel.");
	}

	@Override
	public void onComplete(JSONObject response) {
		// TODO Auto-generated method stub
		
		try {
			openId = response.getString("openid");
			accesstoken = response.getString("access_token");
			String expires = response.getString("expires_in");
			expires_in = System.currentTimeMillis()+ Long.parseLong(expires) * 1000;
		} catch (JSONException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	}

	@Override
	public void onError(UiError arg0) {
		// TODO Auto-generated method stub
		ShowLog.e("TencentListener", "onError:" + arg0.toString());
	}
}
