package assistant.dialog;

import tiange.ktv.assistant.R;
import android.app.Dialog;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.LinearInterpolator;
import android.widget.TextView;
import assistant.entity.ChatDisplayInfo;
import assistant.util.ImageUtil;
import assistant.view.Rotate3dAnimation;
import assistant.view.RoundedImageView;

public class HongBaoVieDialog extends Dialog {

	ChatDisplayInfo mInfo;
	
	RoundedImageView mUiHead;
	TextView mUiFrom;
	TextView mUiTip;
	
	View mUiFail;
	View mUiLookDetail;
	
	View mUiCatchFrameL;
	View mUiHBBtn;
	View mUiHB;
	
	OnHBCatch mListener;
	boolean mIsCanCatch = true;
	
	public String mHBId;
	Rotate3dAnimation mRotateAnima;
	
	public HongBaoVieDialog(Context context,ChatDisplayInfo info,boolean canCatch,OnHBCatch listener) {
		super(context,R.style.HongBaoDialog);
		// TODO Auto-generated constructor stub
		mInfo = info;
		mIsCanCatch = canCatch;
		mListener = listener;
		mHBId = mInfo.hongbao.hbid;
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.dialog_hongbao_vie);
		
		mUiHead = (RoundedImageView) findViewById(R.id.iv_userhead);
		mUiFrom = (TextView) findViewById(R.id.dialog_hb_from);
		mUiTip = (TextView) findViewById(R.id.dialog_hb_introduction);
		
		mUiFail = findViewById(R.id.fl_fail_catch);
		mUiLookDetail = findViewById(R.id.tv_look_detail);
		
		mUiCatchFrameL = findViewById(R.id.rl_catch_hb);
		mUiHBBtn = findViewById(R.id.fl_catch_hb_1);
		mUiHB = findViewById(R.id.img_effect_hb);
		
		if(!mIsCanCatch)
		{
			mUiCatchFrameL.setVisibility(View.GONE);
			mUiFail.setVisibility(View.VISIBLE);
		}
		
		findViewById(R.id.hb_btn_close).setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				dismiss();
			}
		});
		
		if(mInfo.hongbao.idx == 1)
		{
			//系统红包，显示系统红包专用头像
			mUiHead.setImageResource(R.drawable.ic_system_profile);
		}
		else
		{
			Bitmap bm = ImageUtil.getBitmapPhoto(mInfo.hongbao.photo);
			if(bm != null)
				mUiHead.setImageBitmap(bm);
		}
		
		mUiFrom.setText(mInfo.tagName);
		mUiTip.setText(mInfo.tagName+getContext().getResources().getString(R.string.hongbao_giveto_us));
		
		mUiLookDetail.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				dismiss();
				// 查看红包详情
				if(mListener != null)
					mListener.onHBDetail(mInfo.hongbao.hbid);
			}
		});
		
		
		mUiHB.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				mUiHB.setEnabled(false);
				//特效旋转
				mUiCatchFrameL.clearAnimation();
				mUiHBBtn.setVisibility(View.GONE);
				
				mRotateAnima = new Rotate3dAnimation(0, 360, mUiCatchFrameL.getWidth()/2.0f
						, mUiCatchFrameL.getHeight()/2.0f);
				mRotateAnima.setDuration(2000);
				mRotateAnima.setInterpolator(new LinearInterpolator());
				mRotateAnima.setRepeatCount(Animation.INFINITE);
				mUiCatchFrameL.startAnimation(mRotateAnima);
				
				// 领取红包
				if(mListener != null)
					mListener.onHBCatch(mInfo.hongbao.hbid);
				
				//不消失
			}
		});
		
		setCanceledOnTouchOutside(true);
	}
	
	//没抢到，调用这个接口
	public void stopWait(){
		mUiCatchFrameL.clearAnimation();
		mUiCatchFrameL.setVisibility(View.GONE);
		mUiFail.setVisibility(View.VISIBLE);
	}
	
	public interface OnHBCatch{
		public void onHBDetail(String hbid);
		public void onHBCatch(String hbid);
	}
}
