package assistant.activity;

import tiange.ktv.assistant.R;
import tiange.ktv.assistant.wxapi.WXEntryActivity;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.KeyEvent;
import android.view.View;
import android.webkit.JsPromptResult;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.TextView;
import assistant.global.AppStatus;
import assistant.manager.DialogManager;
import assistant.manager.DialogManager.OnDialogListener;
import assistant.share.SinaShare;
import assistant.util.ImageUtil;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;

import com.sina.weibo.sdk.api.WebpageObject;
import com.sina.weibo.sdk.api.WeiboMessage;
import com.sina.weibo.sdk.api.share.BaseResponse;
import com.sina.weibo.sdk.api.share.IWeiboHandler;
import com.sina.weibo.sdk.api.share.IWeiboShareAPI;
import com.sina.weibo.sdk.api.share.SendMessageToWeiboRequest;
import com.sina.weibo.sdk.api.share.WeiboShareSDK;
import com.sina.weibo.sdk.constant.WBConstants;
import com.sina.weibo.sdk.utils.Utility;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelmsg.SendMessageToWX;
import com.tencent.mm.sdk.modelmsg.WXMediaMessage;
import com.tencent.mm.sdk.modelmsg.WXWebpageObject;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;

//launchMode : singTop
public class CommonBrowserActivity extends BaseActivity implements OnDialogListener
	,IWeiboHandler.Response,IWXAPIEventHandler
{
	public static final String EXTRA_TITLE = "assistant.activity.NormalNotifyActivity.title";
	public static final String EXTRA_URL = "assistant.activity.NormalNotifyActivity.url";
	public static final String EXTRA_ACTIVITY = "assistant.activity.NormalNotifyActivity.activity";
	public static final String EXTRA_ACTIVITY_PIC = "assistant.activity.NormalNotifyActivity.activity_pic";
	
	String m_title = "网页";
	String m_shareUrl = "";
	String m_url = "";
	String m_pic = "";
	boolean m_isActivity = false;//是否是活动页面
	
	TextView m_textViewTitle;
	View		m_viewBack;
	View		m_viewProgress;
	WebView webView;
	
	IWeiboShareAPI m_weiboShare;
	WXEntryActivity mWeixinUtl;
	@SuppressLint("SetJavaScriptEnabled")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		m_isNeedCheck = false;
		super.onCreate(savedInstanceState);
		
		m_title = getIntent().getStringExtra(EXTRA_TITLE);
		m_shareUrl = getIntent().getStringExtra(EXTRA_URL);
		m_isActivity = getIntent().getBooleanExtra(EXTRA_ACTIVITY, false);
		if(m_isActivity)
		{
			m_pic = getIntent().getStringExtra(EXTRA_ACTIVITY_PIC);
			m_url = m_shareUrl+"?idx="+AppStatus.userIdx;
		}
		else
		{
			m_url = m_shareUrl;
			m_pic = "";
		}
		
		setContentView(R.layout.ac_browse);
		
		m_textViewTitle = (TextView) findViewById(R.id.tv_title);
		m_viewBack = findViewById(R.id.iv_back);
		m_viewProgress = findViewById(R.id.load_progress);
		webView = (WebView) findViewById(R.id.web);
		
		webView.setScrollBarStyle(View.DRAWING_CACHE_QUALITY_AUTO);
		WebSettings webSettings = webView.getSettings();
		webSettings.setJavaScriptEnabled(true);
		webSettings.setJavaScriptCanOpenWindowsAutomatically(true);
		webSettings.setPluginsEnabled(true);
		webSettings.setSupportZoom(true);
		webSettings.setBuiltInZoomControls(true);
		webSettings.setSupportMultipleWindows(false);
		
		webView.setWebChromeClient(new WebChromeClient(){

			@Override
			public boolean onJsAlert(WebView view, String url, String message,
					JsResult result) {
				//重新改写弹出窗口
				DialogManager.showTipDialog(CommonBrowserActivity.this
						,CommonBrowserActivity.this.getString(R.string.tip) , message, false);
				return true;
				//return super.onJsAlert(view, url, message, result);
			}

			@Override
			public boolean onJsConfirm(WebView view, String url,
					String message, JsResult result) {
				// TODO Auto-generated method stub
				DialogManager.showTipDialog(CommonBrowserActivity.this
						,CommonBrowserActivity.this.getString(R.string.tip) , message, false);
				return true;
				//return super.onJsConfirm(view, url, message, result);
			}

			@Override
			public boolean onJsPrompt(WebView view, String url, String message,
					String defaultValue, JsPromptResult result) {
				// TODO Auto-generated method stub
				DialogManager.showTipDialog(CommonBrowserActivity.this
						,CommonBrowserActivity.this.getString(R.string.tip) , message, false);
				return true;
				//return super.onJsPrompt(view, url, message, defaultValue, result);
			}
			
		});
		
		webView.setWebViewClient(new WebViewClient() {

			@Override
			public void onPageFinished(WebView view, String url) {
				// TODO Auto-generated method stub
				super.onPageFinished(view, url);
				if (m_viewProgress.getVisibility() == View.VISIBLE) {
					m_viewProgress.setVisibility(View.GONE);
				}
			}

			@Override
			public void onPageStarted(WebView view, String url, Bitmap favicon) {
				// TODO Auto-generated method stub
				super.onPageStarted(view, url, favicon);
				if (m_viewProgress.getVisibility() == View.GONE) {
					m_viewProgress.setVisibility(View.VISIBLE);
				}
			}

			@Override
			public void onReceivedError(WebView view, int errorCode,
					String description, String failingUrl) {
				// TODO Auto-generated method stub
				super.onReceivedError(view, errorCode, description, failingUrl);
				if (m_viewProgress.getVisibility() == View.VISIBLE) {
					m_viewProgress.setVisibility(View.GONE);
				}
			}

			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) {
				// TODO Auto-generated method stub
				if(!TextUtils.isEmpty(url) && TextUtils.equals(url, "share:"))
					DialogManager.showShareDialog(CommonBrowserActivity.this, CommonBrowserActivity.this);
				else
					webView.loadUrl(url);
				return true;
			}

		});
		
		m_viewBack.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				if(webView.canGoBack())
					webView.goBack();
				else
					finish();
			}
		});
	}
	
	@Override
	protected void onNewIntent(Intent intent) {
		// TODO Auto-generated method stub
		super.onNewIntent(intent);
		
		boolean isHandleResponse = false;
		
		if(m_weiboShare != null)
		{
			isHandleResponse = m_weiboShare.handleWeiboResponse(intent, this);
			if(isHandleResponse)
				return ;
		}
		
		
		if(mWeixinUtl!=null && mWeixinUtl.handleIntent(intent))
			return ;
		
		//如果不是处理分享的回调，则需要重新加载页面
		m_title = intent.getStringExtra(EXTRA_TITLE);
		m_shareUrl = intent.getStringExtra(EXTRA_URL);
		m_isActivity = intent.getBooleanExtra(EXTRA_ACTIVITY, false);
		if(m_isActivity)
		{
			m_pic = intent.getStringExtra(EXTRA_ACTIVITY_PIC);
			m_url = m_shareUrl+"?idx="+AppStatus.userIdx;
		}
		else
		{
			m_url = m_shareUrl;
			m_pic = "";
		}
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		
		m_textViewTitle.setText(m_title);
		if(m_url!=null && !TextUtils.equals(webView.getUrl() , m_url))
			webView.loadUrl(m_url);
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if(keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0)
		{
			if(webView.canGoBack())
			{
				webView.goBack();
				return true;
			}
		}
		return super.onKeyDown(keyCode, event);
	}
	
	Bitmap getActivityBitmap(){
        Bitmap bitmap = ImageUtil.getBitmapBanner(m_pic);
        if(bitmap == null)//使用默认的banner图片
        	bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.bg_banner_loading);
        
        return bitmap;
	}
	
	String getDescription(){
		return String.format(this.getString(R.string.share_webpage_template), m_title);
	}
	
	private String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis()) : type + System.currentTimeMillis();
	}
	
	//SendMessageToWX.Req.WXSceneTimeline : SendMessageToWX.Req.WXSceneSession
	
	/**
	 *分享页面活动到更多
	 */
	void shareActivityToMore(){
		Intent intent=new Intent(Intent.ACTION_SEND); 
		intent.setType("text/plain");
		//intent.setPackage("com.sina.weibo"); //指定分享到哪里
		intent.putExtra(Intent.EXTRA_SUBJECT, getDescription()); 
		intent.putExtra(Intent.EXTRA_TEXT, m_shareUrl+getDescription());
		intent.putExtra(Intent.EXTRA_TITLE, m_title);
		intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK); 
		startActivity(Intent.createChooser(intent, "请选择")); 
	}
	
	/**
	 *分享页面活动到微信 好友或朋友圈
	 *
	 *@param isPYQ : 是否是发送到朋友圈
	 */
	void shareActivityToWX(boolean isPYQ){
		
		//微信
		if(mWeixinUtl == null)
		{
			mWeixinUtl = new WXEntryActivity(this,this);
		}
		
		if(isPYQ && !mWeixinUtl.isSupportPYQ())
		{
			ShowUtil.showToast(this, R.string.share_nosupport_pyq);
			return ;
		}
		
		WXWebpageObject webpage = new WXWebpageObject();
		webpage.webpageUrl = m_shareUrl;
		WXMediaMessage msg = new WXMediaMessage(webpage);
		msg.title = m_title;
		msg.description = getDescription();
		Bitmap thumb = getActivityBitmap();
		msg.thumbData = ToolUtil.bmpToByteArray(thumb, true);
		
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("webpage");
		req.message = msg;
		req.scene = isPYQ?SendMessageToWX.Req.WXSceneTimeline:SendMessageToWX.Req.WXSceneSession;
		mWeixinUtl.mApi.sendReq(req);
	}
	/**
	 *分享页面活动到微博
	 */
	void shareActivityToWeibo(){
		//微博
		if(m_weiboShare == null)
		{
			m_weiboShare = WeiboShareSDK.createWeiboAPI(this, SinaShare.APP_KEY);
			m_weiboShare.registerApp();
		}
		
		if(m_weiboShare == null)
		{
			ShowUtil.showToast(this, R.string.share_weibo_cant);
			return ;
		}
		
		// 1. 初始化微博的分享消息
		WeiboMessage weiboMessage = new WeiboMessage();
        
        WebpageObject mediaObject = new WebpageObject();
        mediaObject.identify = Utility.generateGUID();
        mediaObject.title = m_title;
        mediaObject.description = getDescription();
        
        mediaObject.setThumbImage(getActivityBitmap());
        mediaObject.actionUrl = m_shareUrl;
        mediaObject.defaultText = m_title;
        
        weiboMessage.mediaObject = mediaObject;
        
        // 2. 初始化从第三方到微博的消息请求
        SendMessageToWeiboRequest request = new SendMessageToWeiboRequest();
        // 用transaction唯一标识一个请求
        request.transaction = buildTransaction(null);
        request.message = weiboMessage;
        
        // 3. 发送请求消息到微博，唤起微博分享界面
        m_weiboShare.sendRequest(request);
	}



	@Override
	public void onOk(int type) {
		// TODO Auto-generated method stub
		switch(type)
		{
			case DialogManager.TYPE_WX:
			{
				shareActivityToWX(false);
				break;
			}
			case DialogManager.TYPE_WXPYQ:
			{
				shareActivityToWX(true);
				break;
			}
			case DialogManager.TYPE_SINA:
			{
				shareActivityToWeibo();
				break;
			}
			case DialogManager.TYPE_MORE:
			{
				shareActivityToMore();
				break;
			}
		}
	}

	@Override
	public void onCancel() {
		// TODO Auto-generated method stub
		
	}

	//微博分享的回调
	@Override
	public void onResponse(BaseResponse baseResp) {
		// TODO Auto-generated method stub
		switch (baseResp.errCode)
		{
		case WBConstants.ErrorCode.ERR_OK:
			ShowUtil.showToast(this, R.string.share_successed);
			break;
		case WBConstants.ErrorCode.ERR_CANCEL:
			ShowUtil.showToast(this, R.string.share_cancel);
			break;
		case WBConstants.ErrorCode.ERR_FAIL:
			ShowUtil.showToast(this, R.string.share_failed);
			break;
		}
		
	}


	//微信请求我们的应用
	@Override
	public void onReq(BaseReq arg0) {
		// TODO Auto-generated method stub
		//不处理
		int glp = 1;
	}


	//微信分享的回调
	@Override
	public void onResp(BaseResp arg0) {
		// TODO Auto-generated method stub
		switch(arg0.errCode)
		{
		case BaseResp.ErrCode.ERR_AUTH_DENIED:
			ShowUtil.showToast(this, R.string.share_failed);
			break;
		case BaseResp.ErrCode.ERR_COMM:
			ShowUtil.showToast(this, R.string.share_failed);
			break;
		case BaseResp.ErrCode.ERR_OK:
			ShowUtil.showToast(this, R.string.share_successed);
			break;
		case BaseResp.ErrCode.ERR_SENT_FAILED:
			ShowUtil.showToast(this, R.string.share_failed);
			break;
		case BaseResp.ErrCode.ERR_UNSUPPORT:
			ShowUtil.showToast(this, R.string.share_failed);
			break;
		case BaseResp.ErrCode.ERR_USER_CANCEL:
			ShowUtil.showToast(this, R.string.share_cancel);
			break;
		}
	}
}
