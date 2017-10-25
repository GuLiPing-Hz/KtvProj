package assistant.view;

import tiange.ktv.assistant.R;
import android.app.ProgressDialog;
import android.content.Context;
import android.text.TextUtils;

/**
 * @author shou_peng
 * @version 创建时间：2012-12-6 上午11:44:59
 * 类说明
 */
public class WaitingDialog extends ProgressDialog {
	private boolean cancelable = true;
	private String message;
	
	public WaitingDialog(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		init();
		message = context.getString(R.string.waiting);
	}
	
	public WaitingDialog(Context context, String message, boolean paramCancelable) {
		super(context);
		// TODO Auto-generated constructor stub
		if(!TextUtils.isEmpty(message)){
			this.message = message;
		}else
			this.message = context.getString(R.string.waiting);
		this.cancelable = paramCancelable;
		init();
	}
	
	private void init(){
		setCancelable(cancelable);
		setMessage(message);
	}
	
	public void start() {
		show();
	}

	public void stop() {
		dismiss();
	}
}
