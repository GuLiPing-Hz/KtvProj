package tiange.ktv.assistant.wxapi;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
//微信回调页面
public class WXEntryActivity extends Activity{

	public static final String AppID = "wx301a28495dd54fa6";
	public static final String AppSecret = "8766057ef9c252a9d2c0f42ca9ae13e8";
	public static final int WEIXING_TIMELINE_MINVERSION = 0x21020001;
	
	static final String state = "SinaKTVAssistant";
	Activity mActivity;
	public IWXAPI mApi;
	public static IWXAPIEventHandler sWeixinHandler;
	
	/** 微信回调专用 */
	public WXEntryActivity() {
		
	}
	
	public WXEntryActivity(Activity activity,IWXAPIEventHandler weixinHandler){
		mActivity = activity;
		sWeixinHandler = weixinHandler;
		regToWx();
	}
	
	void regToWx(){
		//正式发布的时候，是否签名改成true
		//mApi = WXAPIFactory.createWXAPI(mActivity, AppID,false);
		mApi = WXAPIFactory.createWXAPI(mActivity, AppID);
		mApi.registerApp(AppID);
	}
	
	public boolean isSupportPYQ(){
		return (mApi.getWXAppSupportAPI() >= WXEntryActivity.WEIXING_TIMELINE_MINVERSION);
	}
	
	public boolean handleIntent(Intent intent) {
		
		try
		{
			if(mApi != null && intent != null)
				return mApi.handleIntent(intent, sWeixinHandler);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
		return false;
    }
	
	public void authWeixin(){
		 // send oauth request 
		Log.i("WeixinUtil", "auth weixin");
	    final SendAuth.Req req = new SendAuth.Req();
	    req.scope = "snsapi_userinfo,snsapi_base";
	    req.state = state;
	    mApi.sendReq(req);
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		if(mApi == null)
			mApi = WXAPIFactory.createWXAPI(this, AppID);
			
		handleIntent(getIntent());
		finish();
	}

	@Override
	protected void onNewIntent(Intent intent) {
		// TODO Auto-generated method stub
		super.onNewIntent(intent);
		
		setIntent(intent);
		if(mApi == null)
			mApi = WXAPIFactory.createWXAPI(this, AppID);
		
		handleIntent(getIntent());
		finish();
	}
	
	
	public boolean isWXInstalled(){
		return mApi.isWXAppInstalled();
	}
}
