package assistant.dialog;

import tiange.ktv.assistant.R;
import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

public class CallTellDialog extends Dialog {

	public interface OnTellDialog{
		void onTell(String tell);
	}
	
	OnTellDialog mCallback;
	String mTell;
	
	TextView mUiTell;
	
	public CallTellDialog(Context context,String tell,OnTellDialog callback) {
		super(context, R.style.EnterRoom);
		
		mTell = tell;
		mCallback = callback;
		// TODO Auto-generated constructor stub
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.dialog_call);
		mUiTell = (TextView) findViewById(R.id.menu_btn1);
		mUiTell.setText(mTell);
		mUiTell.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				mCallback.onTell(mTell);
				dismiss();
			}
		});
		
		findViewById(R.id.menu_cancel).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				dismiss();
			}
		});
		
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
}
