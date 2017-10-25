package assistant.activity;

import java.util.Locale;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.webkit.DownloadListener;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import assistant.global.KtvAssistantAPIConfig;

import com.umeng.analytics.MobclickAgent;

/**
 * @author 赵龙权
 * @version 创建时间：2013-10-30 下午5:30:07 类说明
 */

public class SimpleRecommendApp extends BaseActivity {
	private View btn_back, view_process;
	private WebView webView;
	private String mUrl = "";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.ac_recommend_app);
		getView();
		setView();
		setListener();
		initData();
	}

	private void getView() {
		// TODO Auto-generated method stub
		btn_back = findViewById(R.id.iv_back);
		webView = (WebView) findViewById(R.id.web_recommendapp);
		view_process = findViewById(R.id.load_progress);
	}

	@SuppressLint("SetJavaScriptEnabled")
	private void setView() {
		// TODO Auto-generated method stub
		webView.setScrollBarStyle(View.DRAWING_CACHE_QUALITY_AUTO);
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
				// TODO Auto-generated method stub
				finish();
			}
		});

		webView.setWebViewClient(new WebViewClient() {
			@Override
			public void onPageFinished(WebView view, String url) {
				// TODO Auto-generated method stub
				super.onPageFinished(view, url);
				if (view_process != null
						&& view_process.getVisibility() == View.VISIBLE) {
					view_process.setVisibility(View.GONE);
				}
			}

			@Override
			public void onPageStarted(WebView view, String url, Bitmap favicon) {
				// TODO Auto-generated method stub
				super.onPageStarted(view, url, favicon);
				if (view_process != null
						&& view_process.getVisibility() == View.GONE) {
					view_process.setVisibility(View.VISIBLE);
				}
			}

			@Override
			public void onReceivedError(WebView view, int errorCode,
					String description, String failingUrl) {
				// TODO Auto-generated method stub
				super.onReceivedError(view, errorCode, description, failingUrl);
				if (view_process != null
						&& view_process.getVisibility() == View.VISIBLE) {
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
					String contentDisposition, String mimetype,
					long contentLength) {
				// TODO Auto-generated method stub
				MobclickAgent.onEvent(SimpleRecommendApp.this,
						"DownloadRecommendAPK");
				switchEvent(url);
				Uri uri = Uri.parse(url);
				final Intent it = new Intent(Intent.ACTION_VIEW, uri);
				startActivity(it);
			}
		});
	}

	private void initData() {
		// TODO Auto-generated method stub
		mUrl = "http://ktvapi.9158.com/"
				+ KtvAssistantAPIConfig.APIUrl.RecommendApp;
		updateContent(mUrl);
	}

	private void updateContent(String paramUrl) {

		webView.loadUrl(paramUrl);
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		return super.onKeyDown(keyCode, event);
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	}

	private void switchEvent(String url) {
		if (!TextUtils.isEmpty(url)) {
			String t_url = url.toLowerCase(Locale.getDefault());
			if (t_url.contains("sinavoice") && t_url.endsWith(".apk")) {
				MobclickAgent.onEvent(SimpleRecommendApp.this,
						"DownloadSinaVoice");
			}
		}
	}
}
