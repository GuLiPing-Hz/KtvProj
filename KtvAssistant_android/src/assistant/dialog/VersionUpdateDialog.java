package assistant.dialog;

import tiange.ktv.assistant.R;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.widget.ListView;
import android.widget.TextView;
import assistant.adapter.VersionDetailAdapter;
import assistant.global.AppStatus;
import assistant.service.AppUpgradeService;
import assistant.util.ShowUtil;

public class VersionUpdateDialog extends Dialog implements View.OnClickListener{

	TextView m_textViewVersion;
	View m_viewUpgrade;
	View m_viewCancel;
	ListView m_listViewDetails;
	View m_viewForce;
	VersionDetailAdapter m_adapter;
	
	public VersionUpdateDialog(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		m_adapter = new VersionDetailAdapter(context);
	}

	public VersionUpdateDialog(Context context, int theme) {
		super(context, theme);
		// TODO Auto-generated constructor stub
		m_adapter = new VersionDetailAdapter(context);
	}

	public VersionUpdateDialog(Context context, boolean cancelable,
			OnCancelListener cancelListener) {
		super(context, cancelable, cancelListener);
		// TODO Auto-generated constructor stub
		m_adapter = new VersionDetailAdapter(context);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.dialog_version_update);
		
		m_textViewVersion = (TextView) findViewById(R.id.textViewVersionTip);
		m_viewUpgrade = findViewById(R.id.versionUpgrade);
		m_viewCancel = findViewById(R.id.versionCancel);
		m_viewForce = findViewById(R.id.textViewForceTip);
		m_listViewDetails = (ListView) findViewById(R.id.listViewDetails);
		m_listViewDetails.setAdapter(m_adapter);
		
		m_viewUpgrade.setOnClickListener(this);
		m_viewCancel.setOnClickListener(this);
		
		m_textViewVersion.setText("最新版 "+AppStatus.s_newVU.versionName+" 更新啦!");
		
		//如果不是强制更新，则可以外部点击消失
		if(AppStatus.s_newVU.forceUpdate == 0)
			setCanceledOnTouchOutside(true);
		else
			m_viewForce.setVisibility(View.VISIBLE);
	}

	void checkNeedExit(){
		if(AppStatus.s_newVU.forceUpdate == 1)
			AppStatus.exitApp(getContext());
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if(keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0)
		{
			checkNeedExit();
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		switch(arg0.getId())
		{
		case R.id.versionCancel:
			//隐藏对话框
			dismiss();
			checkNeedExit();
			break;
		case R.id.versionUpgrade:
			
			if(AppStatus.s_newVU == null)
			{
				ShowUtil.showToast(getContext(), R.string.soft_error);
				break;
			}
			//启动更新版本服务
			Intent intent = new Intent(getContext(),AppUpgradeService.class);
			intent.putExtra(AppUpgradeService.EXTRA_DOWNLOAD_URL, AppStatus.s_newVU.apkUrl);
			intent.putExtra(AppUpgradeService.EXTRA_NEWVERSION, AppStatus.s_newVU.versionName);
			getContext().startService(intent);
			//隐藏对话框
			dismiss();
			break;
		}
	}

}
