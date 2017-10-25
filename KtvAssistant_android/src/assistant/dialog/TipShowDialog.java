package assistant.dialog;

import tiange.ktv.assistant.R;
import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;
import assistant.manager.DialogManager;
import assistant.manager.DialogManager.OnDialogListener;

public class TipShowDialog extends Dialog implements View.OnClickListener{

	TextView m_textViewTitle;
	TextView m_textViewContent;
	View m_vCancel;
	View m_vOk;
	
	String m_title;
	String m_content;
	boolean m_isDouble = true;//是否两个按钮都需要显示
	
	OnDialogListener m_listener;
	
	void setShowTip(String title,String content,OnDialogListener listener,boolean isDouble){
		m_title = title;
		m_content = content;
		m_listener = listener;
		m_isDouble = isDouble;
	}
	
	public TipShowDialog(Context context,String title,String content,OnDialogListener listener,boolean isDouble) {
		super(context);
		setShowTip(title,content,listener,isDouble);
	}

	public TipShowDialog(Context context, int theme,String title,String content,OnDialogListener listener,boolean isDouble) {
		super(context, theme);
		setShowTip(title,content,listener,isDouble);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.dialog_tip_show);
		
		m_textViewTitle = (TextView) findViewById(R.id.textViewTitle);
		m_textViewContent = (TextView) findViewById(R.id.textViewContent);
		m_vCancel = findViewById(R.id.tip_cancel);
		m_vOk = findViewById(R.id.tip_ok);
		
		m_vCancel.setOnClickListener(this);
		m_vOk.setOnClickListener(this);
		
		m_textViewTitle.setText(m_title);
		m_textViewContent.setText(m_content);
		
		if(!m_isDouble)
			m_vCancel.setVisibility(View.GONE);
		
		//外部点击消失
		//setCanceledOnTouchOutside(true);
		
		Window w = getWindow();
		WindowManager.LayoutParams lp = w.getAttributes();
		WindowManager wm = (WindowManager) getContext().getSystemService(Context.WINDOW_SERVICE);
		lp.width = wm.getDefaultDisplay().getWidth();//根据屏幕宽度重新定义对话框宽度
		onWindowAttributesChanged(lp);
	}

	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		switch(arg0.getId())
		{
			case R.id.tip_cancel:
			{
				if(m_listener!=null)
					m_listener.onCancel();
				break;
			}
			case R.id.tip_ok:
			{
				if(m_listener!=null)
					m_listener.onOk(DialogManager.TYPE_NOTHING);
				break;
			}
		}
		dismiss();
	}
}
