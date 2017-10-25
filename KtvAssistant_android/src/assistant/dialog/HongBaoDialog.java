package assistant.dialog;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.animation.RotateAnimation;
import android.widget.TextView;
import assistant.entity.KtvHongBaoInfo;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.task.honbao.HongBGetTask;
import assistant.view.CircleLoadingView;
import assistant.view.CircleLoadingView.OnTask;

public class HongBaoDialog extends Dialog implements OnTask
{

	TextView mUiHBClass;
	TextView mUiHBMission;
	TextView mUiHBGold;
	CircleLoadingView mUiHBGet;
	
	KtvHongBaoInfo mInfo;
	OnTaskSuccess mTaskListener;
	
	@SuppressLint("HandlerLeak")
	Handler mHandler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			switch(msg.what)
			{
			case MessageDef.WM_GETHONGBAO:
				if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
				{
					mUiHBGet.stopLoading(true);
					if(mTaskListener != null)
						mTaskListener.onTaskSuccess();
				}
				else
					mUiHBGet.stopLoading(false);
				break;
			}
		}
		
	};
	
	void setShowTip(KtvHongBaoInfo info,OnTaskSuccess listener){
		mInfo = info;
		mTaskListener = listener;
	}
	
	public HongBaoDialog(Context context,KtvHongBaoInfo info,OnTaskSuccess listener) {
		super(context,R.style.HongBaoDialog);
		setShowTip(info,listener);
	}

	public HongBaoDialog(Context context,int theme,KtvHongBaoInfo info,OnTaskSuccess listener) {
		super(context, theme);
		setShowTip(info,listener);
	}
	
	RotateAnimation rotateAnimation;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.dialog_hb);
		
		mUiHBClass = (TextView) findViewById(R.id.dialog_hb_class);
		mUiHBMission = (TextView) findViewById(R.id.dialog_hb_mission);
		mUiHBGold = (TextView) findViewById(R.id.dialog_hb_gold);
		mUiHBGet = (CircleLoadingView) findViewById(R.id.hb_btn_get);
		
		mUiHBGet.initOnTask(this);
		mUiHBClass.setText(mInfo.hbclass);
		if(mInfo.hbnclass == 2)
			mUiHBMission.setText("来自:"+mInfo.hbmission);
		else
			mUiHBMission.setText(mInfo.hbmission);
		mUiHBGold.setText(""+mInfo.gold+"金币");
		
		this.findViewById(R.id.hb_btn_close).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				dismiss();
			}
		});
		
		//自动点击
		mUiHBGet.beginTask();
		//setCanceledOnTouchOutside(false);
	}

	@Override
	public void onTaskBegin() {
		// TODO Auto-generated method stub
		mHandler.postDelayed(new Runnable(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				HongBGetTask task = new HongBGetTask(mHandler,mInfo.hbid);
				task.execute();
			}
			
		}, 1000);
	}
	
	public interface OnTaskSuccess{
		public void onTaskSuccess();
	}
}