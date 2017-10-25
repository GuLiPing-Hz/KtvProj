package assistant.dialog;

import tiange.ktv.assistant.R;
import android.app.Dialog;
import android.content.Context;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import assistant.manager.DialogManager;
import assistant.manager.DialogManager.OnDialogListener;

public class ShareDialog extends Dialog implements View.OnClickListener {

	View m_viewWX;
	View m_viewWXPyq;
	View m_viewSina;
	View m_viewMore;
	View m_viewCancel;

	OnDialogListener m_listener;
	public ShareDialog(Context context, int theme,OnDialogListener listener) {
		super(context, theme);
		// TODO Auto-generated constructor stub
		m_listener = listener;
		
		setContentView(R.layout.dialog_share);
		
		m_viewWX = findViewById(R.id.btn_share_wx);
		m_viewWXPyq = findViewById(R.id.btn_share_wxpyq);
		m_viewSina = findViewById(R.id.btn_share_sina);
		m_viewMore = findViewById(R.id.btn_share_more);
		m_viewCancel = findViewById(R.id.btn_share_cancel);
		
		m_viewWX.setOnClickListener(this);
		m_viewWXPyq.setOnClickListener(this);
		m_viewSina.setOnClickListener(this);
		m_viewMore.setOnClickListener(this);
		m_viewCancel.setOnClickListener(this);
		
		setCanceledOnTouchOutside(true);
		//修改对话框大小以及位置
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
		case R.id.btn_share_wx:
		{
			if(m_listener != null)
				m_listener.onOk(DialogManager.TYPE_WX);
			break;
		}
		case R.id.btn_share_wxpyq:
		{
			if(m_listener != null)
				m_listener.onOk(DialogManager.TYPE_WXPYQ);
			break;
		}
		case R.id.btn_share_sina:
		{
			if(m_listener != null)
				m_listener.onOk(DialogManager.TYPE_SINA);
			break;
		}
		case R.id.btn_share_more:
		{
			if(m_listener != null)
				m_listener.onOk(DialogManager.TYPE_MORE);
			break;
		}
		case R.id.btn_share_cancel:
		{
			if(m_listener != null)
				m_listener.onCancel();
			break;
		}
		}
		
		dismiss();
	}
}
