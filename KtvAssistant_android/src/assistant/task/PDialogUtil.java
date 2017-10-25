package assistant.task;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.support.v4.app.FragmentActivity;

public class PDialogUtil {

	public static void normalAlertDialog(Context context, String title, String message, OnClickListener listener){
		
		new AlertDialog.Builder(context)
		.setTitle(title)
		.setMessage(message)
		.setPositiveButton("确定", listener)
		.setNegativeButton("取消", new DialogInterface.OnClickListener() {
			
			@Override
			public void onClick(DialogInterface dialog, int which) {
				// TODO Auto-generated method stub
				dialog.cancel();
			}
		})
		.show();
	}
	
	public static void normalAlertDialog(Context context, String title, String message, String PositiveButtonText, OnClickListener listener){
		
		new AlertDialog.Builder(context)
		.setTitle(title)
		.setMessage(message)
		.setPositiveButton(PositiveButtonText, listener)
		.setNegativeButton("取消", new DialogInterface.OnClickListener() {
			
			@Override
			public void onClick(DialogInterface dialog, int which) {
				// TODO Auto-generated method stub
				dialog.cancel();
			}
		})
		.show();
	}

	public static void normalAlertDialog(FragmentActivity userAccountFragment, String title, String message, OnClickListener listener){
		
		new AlertDialog.Builder(userAccountFragment)
		.setTitle(title)
		.setMessage(message)
		.setPositiveButton("确定", listener)
		.setNegativeButton("取消", new DialogInterface.OnClickListener() {
			
			@Override
			public void onClick(DialogInterface dialog, int which) {
				// TODO Auto-generated method stub
				dialog.cancel();
			}
		})
		.show();
	}

}
