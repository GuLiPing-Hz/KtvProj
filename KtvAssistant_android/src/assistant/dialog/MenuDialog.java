package assistant.dialog;

import tiange.ktv.assistant.R;
import android.app.Dialog;
import android.content.Context;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

/**
 * @author bill
 * @version 创建时间：2013-10-22 下午04:44:13 类说明
 */
public class MenuDialog {
	public interface OnMenuClick {
		void onClick(View v);
	}

	public static Dialog showChoiceMenu(final Context context, boolean isShowText,
			final OnMenuClick menuClick) {
		final Dialog dlg = new Dialog(context, R.style.EnterRoom);
		LayoutInflater inflater = (LayoutInflater) context
				.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		LinearLayout layout = (LinearLayout) inflater.inflate(
				R.layout.menu_choice_iphone, null);

		final int cFullFillWidth = 10000;
		layout.setMinimumWidth(cFullFillWidth);

		TextView tips = (TextView) layout.findViewById(R.id.menu_text);
		final Button btnMenu1 = (Button) layout.findViewById(R.id.menu_btn1);
		final Button btnMenu2 = (Button) layout.findViewById(R.id.menu_btn2);
		Button btnCancel = (Button) layout.findViewById(R.id.menu_cancel);

		if(isShowText)
			tips.setVisibility(View.VISIBLE);
		
		btnCancel.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				dlg.dismiss();
			}
		});

		btnMenu1.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				menuClick.onClick(v);
				dlg.dismiss();
			}
		});

		btnMenu2.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				menuClick.onClick(v);
				dlg.dismiss();
			}
		});

		// set a large value put it in bottom
		Window w = dlg.getWindow();
		WindowManager.LayoutParams lp = w.getAttributes();
		lp.x = 0;
		final int cMakeBottom = -1000;
		lp.y = cMakeBottom;
		lp.gravity = Gravity.BOTTOM;
		dlg.onWindowAttributesChanged(lp);
		dlg.setCanceledOnTouchOutside(true);
		dlg.setContentView(layout);
		dlg.show();
		return dlg;
	}
}
