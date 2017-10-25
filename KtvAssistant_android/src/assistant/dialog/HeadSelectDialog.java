package assistant.dialog;

import tiange.ktv.assistant.R;
import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

public class HeadSelectDialog extends Dialog implements View.OnClickListener{
	public static final int PICTURE = 1;
	public static final int TAKEPHOTO = 2;
	
	OnHeadClick mListener;
	
	public HeadSelectDialog(Context context,OnHeadClick listener) {
		super(context, R.style.RankMsgDialog);
		// TODO Auto-generated constructor stub
		mListener = listener;
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		this.setContentView(R.layout.dialog_headselect);
		
		this.findViewById(R.id.head_picture).setOnClickListener(this);
		this.findViewById(R.id.head_takephoto).setOnClickListener(this);
		this.findViewById(R.id.tip_cancel).setOnClickListener(this);
		
		//外部点击消失
		setCanceledOnTouchOutside(true);
		
		Window w = getWindow();
		WindowManager.LayoutParams lp = w.getAttributes();
		WindowManager wm = (WindowManager) getContext().getSystemService(Context.WINDOW_SERVICE);
		lp.width = wm.getDefaultDisplay().getWidth();//根据屏幕宽度重新定义对话框宽度
		onWindowAttributesChanged(lp);
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch(v.getId())
		{
		case R.id.head_picture:
			if(mListener != null)
				mListener.onSelect(PICTURE);
			break;
		case R.id.head_takephoto:
			if(mListener != null)
				mListener.onSelect(TAKEPHOTO);
			break;
		case R.id.tip_cancel:
			break;
		}
		dismiss();
	}
	
	public interface OnHeadClick{
		/***
		 * 
		 * @param type 1：从相册，2：拍照
		 */
		void onSelect(int type);
	}
}
