package assistant.dialog;

import tiange.ktv.assistant.R;
import android.app.Dialog;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import assistant.global.AppStatus;
import assistant.util.ImageUtil;
import assistant.view.RoundedImageView;

import com.jni.netutil.LocalData_UserInfo;
import com.jni.netutil.out.HongBaoInfoToUi;

public class HongBaoPersonDialog extends Dialog {
	
	HongBaoInfoToUi mInfo;
	RoundedImageView mUiHead;
	TextView mUiFrom;
	TextView mUiGold;
	
	public HongBaoPersonDialog(Context context,HongBaoInfoToUi info) {
		super(context,R.style.HongBaoDialog);
		// TODO Auto-generated constructor stub
		mInfo = info;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.dialog_hb_person);
		
		findViewById(R.id.hb_btn_close).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				dismiss();
			}
		});
		mUiHead = (RoundedImageView) findViewById(R.id.iv_userhead);
		
		Bitmap bitmap = ImageUtil.getBitmapPhoto(mInfo.photo);
		if(bitmap != null)
			mUiHead.setImageBitmap(bitmap);
		
		
		mUiFrom = (TextView) findViewById(R.id.dialog_hb_mission);
		mUiFrom.setText("来自："+mInfo.name);
		
		mUiGold = (TextView) findViewById(R.id.dialog_hb_gold);
		mUiGold.setText(""+mInfo.gold+this.getContext().getResources().getString(R.string.pc_goldinfo));
	
		setCanceledOnTouchOutside(true);
	}
}
