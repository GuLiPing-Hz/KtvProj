package assistant.manager;

import tiange.ktv.assistant.R;
import android.app.Dialog;
import android.content.Context;
import assistant.dialog.ShareDialog;
import assistant.dialog.TipShowDialog;
import assistant.dialog.TopUpDialog;


/**
 * 
 * 显示充值对话框 
 */
public class DialogManager {
	
	public static final int TYPE_NOTHING = 0;
	public static final int TYPE_WX = 1;
	public static final int TYPE_WXPYQ = 2;
	public static final int TYPE_SINA = 3;
	public static final int TYPE_MORE = 4;
	
	public interface OnDialogListener{
		void onOk(int type);
		void onCancel();
	}
	
	public static void showTopUpDialog(Context context) {
		showTopUpDialog(context,null);
	}
	public static void showTopUpDialog(Context context,OnDialogListener listener) {
		Dialog dialog = new TopUpDialog(context, R.style.EnterRoom,listener);
		dialog.show();
	}
	
	public static void showTipDialog(Context context,String title,String content,boolean isDouble){
		Dialog dialog = new TipShowDialog(context,R.style.RankMsgDialog,title,content,null,isDouble);
		dialog.show();
	}
	public static void showTipDialog(Context context,String title,String content,OnDialogListener listener,boolean isDouble){
		Dialog dialog = new TipShowDialog(context,R.style.RankMsgDialog,title,content,listener,isDouble);
		dialog.show();
	}
	
	public static void showShareDialog(Context context,OnDialogListener listener){
		Dialog dialog = new ShareDialog(context, R.style.EnterRoom,listener);
		dialog.show();
	}
}
