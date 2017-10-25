package assistant.fragment;

import com.umeng.analytics.MobclickAgent;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import assistant.util.ShowLog;

/**
 * @author 宋静
 * @version 创建时间：2013-7-17 下午2:06:40
 * 类说明 首页结构的Fragment的封装基类
 */
public class BaseFragment extends Fragment{
	public BaseFragment instance = null;
	public String TAG = "BaseFragment";
	
	@Override
    public void onCreate(Bundle savedInstanceState) {
		TAG = this.getClass().getSimpleName();
		ShowLog.d(TAG, "onCreate.");
        super.onCreate(savedInstanceState);
        instance = this;
        
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        instance = null;
        
        ShowLog.d(TAG, "onDestroy.");
    }

    @Override
	public void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		MobclickAgent.onPageEnd(TAG);
		
		ShowLog.d(TAG, "onPause.");
	}

	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		MobclickAgent.onPageStart(TAG);
		
		ShowLog.d(TAG, "onResume.");
	}

	public void refreshView(){
    	
    }
}
