package assistant.activity;

import tiange.ktv.assistant.R;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.KeyEvent;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;
import assistant.dialog.VersionUpdateDialog;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowUtil;

import com.umeng.fb.FeedbackAgent;

/**
 * @author 赵龙权
 * @version 创建时间：2013-10-30 下午5:30:07
 * 类说明
 */

public class AboutPageActivity extends BaseActivity {
	
	FeedbackAgent m_feedbackAgent;
	TextView tv_version;
	ImageButton back;

	View m_update, m_feedback;
	View m_viewGrade;
	View m_viewIntroduce;
	View m_viewNew;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.ac_aboutpage);
		getView();
		setInitView();
		
		m_feedbackAgent = new FeedbackAgent(this);
		// check if the app developer has replied to the feedback or not.
		m_feedbackAgent.sync();
	}

	private void getView() {
		TextView tv = (TextView)findViewById(R.id.tv_title);
		tv.setText(R.string.more_about_text);
		back = (ImageButton)findViewById(R.id.iv_back);
		tv_version = (TextView)findViewById(R.id.about_version);
		
		m_viewGrade = findViewById(R.id.more_menu_grade_layout);
		m_viewIntroduce = findViewById(R.id.function_introduce);
		m_feedback = findViewById(R.id.rl_feedback);
		m_update = findViewById(R.id.rl_update);
		m_viewNew = findViewById(R.id.version_new);
		//评分
		m_viewGrade.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				try
				{
					Uri uri = Uri.parse("market://details?id="+getPackageName());
					Intent intent = new Intent(Intent.ACTION_VIEW,uri);
					startActivity(intent);
				}
				catch(ActivityNotFoundException e)
				{
					ShowUtil.showToast(AboutPageActivity.this, R.string.error_nomarket);
				}
			}
		});
		
		//功能介绍
		m_viewIntroduce.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				Intent intent = new Intent(AboutPageActivity.this,CommonBrowserActivity.class);
				intent.putExtra(CommonBrowserActivity.EXTRA_TITLE, AboutPageActivity.this.getString(R.string.fun_introduce));
				intent.putExtra(CommonBrowserActivity.EXTRA_URL, KtvAssistantAPIConfig.APP_INTRODUCE);
				
				startActivity(intent);
			}
		});
		// 更新版本
		m_update.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				if (MobileNetStatUtil.unNetConnectionTips(AboutPageActivity.this) || AppStatus.s_newVU == null) {
					return;
				}
				
				int newVersion = AppStatus.s_newVU.versionCode;
				int curVersion = AppStatus.s_curVU.versionCode;
				
				if(newVersion > curVersion)
				{
					VersionUpdateDialog dialog = new VersionUpdateDialog(AboutPageActivity.this
							,R.style.EnterRoom);
					dialog.show();
				}
				else
				{
					ShowUtil.showToast(AboutPageActivity.this, R.string.app_update_latest_message);
				}
			}
		});

		// 反馈意见
		m_feedback.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				m_feedbackAgent.startFeedbackActivity();
//				UMFeedbackService.setGoBackButtonVisible();
//				UMFeedbackService.setFeedBackListener(mHome);
//				UMFeedbackService.openUmengFeedbackSDK(mHome);
			}
		});
				
		back.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				finish();
			}
		});
	}

	private void setInitView() {
		// TODO Auto-generated method stub
		if(AppStatus.s_newVU!=null && AppStatus.s_newVU.versionCode > AppStatus.s_curVU.versionCode)
			m_viewNew.setVisibility(View.VISIBLE);
		
		try {
			String versionName = getPackageManager().getPackageInfo(getPackageName(), PackageManager.COMPONENT_ENABLED_STATE_DEFAULT).versionName;
			if(!TextUtils.isEmpty(versionName)){
				String text = getString(R.string.app_name) + "V" + versionName;
				tv_version.setText(text);
			}
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		return super.onKeyDown(keyCode, event);
	}
}
