package assistant.activity;

import android.app.Activity;
import android.os.Bundle;
import assistant.global.AppStatus;
import assistant.util.ShowLog;

import com.umeng.analytics.MobclickAgent;

/**
 * @author glp
 */
public class BaseActivity extends Activity{
	public String TAG = "BaseActivity";
	public BaseActivity instance = null;
	//默认都需要检查App是否被资源回收
	public boolean m_isNeedCheck = true;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		//GLP:
		//在register 里面会去检查是否应用程序是recreate(资源被系统回收)，这个时候需要重新执行Start页面
		
		
		TAG = this.getClass().getSimpleName();
		ShowLog.d(TAG, "onCreate");
		
		AppStatus.register(this,m_isNeedCheck);
		
		super.onCreate(savedInstanceState);
		
		instance = this;
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		
		ShowLog.d(TAG, "onDestroy");
		
		instance = null;
		AppStatus.unregister(this);
		
	}

	@Override
	protected void onPause() {
		super.onPause();
		ShowLog.d(TAG, "onPause");
		MobclickAgent.onPageStart(TAG);
		MobclickAgent.onPause(this);
		
	}

	@Override
	protected void onResume() {
		ShowLog.d(TAG, "onResume");
		super.onResume();
		MobclickAgent.onPageStart(TAG);
		MobclickAgent.onResume(this);
		
	}

	@Override
	protected void onStart() {
		super.onStart();
		ShowLog.d(TAG, "onStart");
	}

}
