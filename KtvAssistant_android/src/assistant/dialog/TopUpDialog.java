package assistant.dialog;

import tiange.ktv.assistant.R;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import assistant.activity.Home;
import assistant.manager.DialogManager;
import assistant.manager.DialogManager.OnDialogListener;

public class TopUpDialog extends Dialog implements View.OnClickListener{

	Button m_topUp;
	Button m_cancel;
	OnDialogListener m_listener;
	public TopUpDialog(Context context, int theme,OnDialogListener listener) {
		super(context, theme);
		// TODO Auto-generated constructor stub
		m_listener = listener;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.dialog_topup);
		
		m_topUp = (Button)findViewById(R.id.button_topup);
		m_cancel = (Button)findViewById(R.id.button_topup_cancel);
		
		m_topUp.setOnClickListener(this);
		m_cancel.setOnClickListener(this);
		
		setCanceledOnTouchOutside(true);
		
		Window w = getWindow();
		WindowManager.LayoutParams lp = w.getAttributes();
		lp.x = 0;
		lp.y = -1000;
		WindowManager wm = (WindowManager) getContext().getSystemService(Context.WINDOW_SERVICE);
		lp.width = wm.getDefaultDisplay().getWidth();
		lp.gravity = Gravity.BOTTOM;
		onWindowAttributesChanged(lp);
	}

	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		switch(arg0.getId())
		{
		case R.id.button_topup:
			//AppStatus.backToHome();
			Intent intent = new Intent(getContext(),Home.class);
			intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
			getContext().startActivity(intent);
			if(m_listener != null)
				m_listener.onOk(DialogManager.TYPE_NOTHING);
			break;
		case R.id.button_topup_cancel:
			if(m_listener != null)
				m_listener.onCancel();
			break;
		}
		this.dismiss();
	}
}
