package assistant.activity;

import tiange.ktv.assistant.R;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.view.KeyEvent;
import assistant.fragment.BaseFragmentHelper;
import assistant.global.AppStatus;
import assistant.util.ShowLog;

import com.umeng.analytics.MobclickAgent;

public abstract class BaseFragmentActivity extends FragmentActivity {
	
	String TAG = "BaseFragmentActivity";
	protected abstract Fragment createFragment();
	
	protected BaseFragmentHelper mFragmentHelper = null;
	//默认都需要检查App是否被资源回收
	public boolean m_isNeedCheck = true;
	@Override
	public void onCreate(Bundle savedInstanceState){
		TAG = this.getClass().getSimpleName();
		ShowLog.d(TAG, "onCreate");
		AppStatus.register(this,m_isNeedCheck);
		
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_fragment);
		
		//if(savedInstanceState == null)
		{
			FragmentManager fm = getSupportFragmentManager();
			Fragment fragment = fm.findFragmentById(R.id.framelayout_activity);
			if(fragment == null)
			{
				fragment = createFragment();
				fm.beginTransaction().add(R.id.framelayout_activity, fragment).commit();
			}
		}
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		
		super.onDestroy();
		AppStatus.unregister(this);
		ShowLog.d(TAG, "onDestroy");
	}

	@Override
	protected void onPause() {
		
		super.onPause();
		//用于统计用户使用行为,友盟提供的SDK
		MobclickAgent.onPause(this);
		ShowLog.d(TAG, "onPause");
	}

	@Override
	protected void onResume() {
		
		super.onResume();
		MobclickAgent.onResume(this);
		ShowLog.d(TAG, "onResume");
	}

	//辅助Fragment监听键盘返回事件
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if(mFragmentHelper!=null && mFragmentHelper.onKeyDown(keyCode, event))
		{
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}
	
	
}
