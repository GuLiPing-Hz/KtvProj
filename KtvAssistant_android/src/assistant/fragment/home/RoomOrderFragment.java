package assistant.fragment.home;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.webkit.DownloadListener;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ImageButton;
import android.widget.TextView;
import assistant.dialog.CallTellDialog;
import assistant.dialog.CallTellDialog.OnTellDialog;
import assistant.fragment.BaseFragment;
import assistant.fragment.BaseFragmentHelper;
import assistant.global.KtvAssistantAPIConfig;
import assistant.util.ShowLog;
import assistant.util.ToolUtil;

/**
 * @author 宋静
 * @version 创建时间：2013-12-12 下午12:02:24
 * 类说明 包厢预定
 */
public class RoomOrderFragment extends BaseFragment {
	//private Home mHome;
	private View vRoot;
	private ImageButton btn_back;
	private TextView tv_title;
	private WebView webView;
	private View view_process;
	
	private String mUrl = "";

	/* ================ 复写系统方法 =============== */
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		super.onCreateView(inflater, container, savedInstanceState);
		ShowLog.i(TAG, "onCreateView.");
		vRoot = inflater.inflate(R.layout.ac_browse, container, false);
		initView();
		return vRoot;
	}

	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);
		ShowLog.i(TAG, "onActivityCreated.");
		initData();
	}
	
	private void initView() {
		//mHome = (Home)this.getActivity();
		findView();
		setView();
		setListener();
	}

	private void findView() {
		btn_back = (ImageButton) vRoot.findViewById(R.id.iv_back);
		tv_title = (TextView) vRoot.findViewById(R.id.tv_title);
		webView = (WebView) vRoot.findViewById(R.id.web);
		view_process =  vRoot.findViewById(R.id.load_progress);
	}

	@SuppressLint("SetJavaScriptEnabled") 
	private void setView() {
		tv_title.setText(getString(R.string.menuitem_bookroom));
		//btn_back.setImageResource(R.drawable.btn_home_menu);
		
		webView.setScrollBarStyle(View.DRAWING_CACHE_QUALITY_AUTO);// View.SCROLLBARS_OUTSIDE_OVERLAY
//		webView.getSettings().setCacheMode(WebSettings.LOAD_NO_CACHE);
//		webView.clearCache(true);
		
		WebSettings webSettings = webView.getSettings();
		webSettings.setJavaScriptEnabled(true);
		webSettings.setJavaScriptCanOpenWindowsAutomatically(true);
		webSettings.setPluginsEnabled(true);
		webSettings.setSupportZoom(true);
		webSettings.setBuiltInZoomControls(true);
		webSettings.setSupportMultipleWindows(false);
	}

	private void setListener() {
		// TODO Auto-generated method stub
		btn_back.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				if(webView.canGoBack())
					webView.goBack();
				else
				{
					ToolUtil.closeKeyBoard(getActivity());
					//mHome.showMenu();
					getActivity().finish();
				}
			}
		});
		
		webView.setWebViewClient(new WebViewClient() {

			@Override
			public void onPageFinished(WebView view, String url) {
				// TODO Auto-generated method stub
				super.onPageFinished(view, url);
				if (view_process != null && view_process.getVisibility() == View.VISIBLE) {
					view_process.setVisibility(View.GONE);
				}
			}

			@Override
			public void onPageStarted(WebView view, String url, Bitmap favicon) {
				// TODO Auto-generated method stub
				super.onPageStarted(view, url, favicon);
				if (view_process != null && view_process.getVisibility() == View.GONE) {
					view_process.setVisibility(View.VISIBLE);
				}
			}

			@Override
			public void onReceivedError(WebView view, int errorCode,
					String description, String failingUrl) {
				// TODO Auto-generated method stub
				super.onReceivedError(view, errorCode, description, failingUrl);
				if (view_process != null && view_process.getVisibility() == View.VISIBLE) {
					view_process.setVisibility(View.GONE);
				}
			}

			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) {
				// TODO Auto-generated method stub
				updateContent(url);
				return true;
			}

		});

		webView.setWebChromeClient(new WebChromeClient() {

			@Override
			public boolean onJsAlert(WebView view, String url, String message,
					JsResult result) {
				// TODO Auto-generated method stub
				return super.onJsAlert(view, url, message, result);
			}

		});
		
		webView.setDownloadListener(new DownloadListener() {
			
			@Override
			public void onDownloadStart(String url, String userAgent,
					String contentDisposition, String mimetype, long contentLength) {
				// TODO Auto-generated method stub
				//Log.v("shou", "onDownloadStart::"+url);
//				MobclickAgent.onEvent(mHome, "DownloadSinaVoice");
//				Uri uri = Uri.parse(url);          
//		        final Intent it = new Intent(Intent.ACTION_VIEW, uri);
//		        startActivity(it);
			}
		});
	}

	public BaseFragmentHelper fragmentHelper = new BaseFragmentHelper() {

		@Override
		public boolean onKeyDown(int keyCode, KeyEvent event) {
			if (webView.canGoBack()) {
				webView.goBack();
				return true;
			}
			return false;
		}
	};
	
	private void initData() {
		
		mUrl = KtvAssistantAPIConfig.ROOM_ORDER;//PKtvAssistantConfig.KtvAssistantAPIDomain + PKtvAssistantConfig.KtvAssistantAPIActivityNew;
		updateContent(mUrl);
		
		//mHome.baseFragmentHelper = fragmentHelper;
	}
	
	private void updateContent(String paramUrl){
		ShowLog.v("RoomOrderFragment.url::"+paramUrl);
		if(!TextUtils.isEmpty(paramUrl) && paramUrl.startsWith("call:")){
			
			CallTellDialog dialog = new CallTellDialog(getActivity(),paramUrl.substring(5),new OnTellDialog(){

				@Override
				public void onTell(String tell) {
					// TODO Auto-generated method stub
					callTel(tell);
				}
				
			});
			dialog.show();
			return;
		}
		webView.loadUrl(paramUrl);
	}
	
	private void callTel(String phoneNumber){
		try {
			if(TextUtils.isEmpty(phoneNumber) || TextUtils.isEmpty(phoneNumber.trim())){
				ShowLog.e("phoneNumber is null or isEmpty");
				return;
			}
			ShowLog.v("phoneNumber:"+phoneNumber);
			//匹配电话格式
//			Pattern pattern = Pattern.compile("\\d{4}-\\d{8}");
//			Matcher matcher = pattern.matcher(phoneNumber);
//			if(matcher.find()){
				Uri uri = Uri.parse("tel:"+phoneNumber);   
			    Intent intent = new Intent(Intent.ACTION_DIAL, uri);     
			    startActivity(intent);  
//			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	    
	}

}
