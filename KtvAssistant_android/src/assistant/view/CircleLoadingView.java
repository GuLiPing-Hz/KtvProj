package assistant.view;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.AnimationSet;
import android.view.animation.LinearInterpolator;
import android.view.animation.RotateAnimation;
import android.view.animation.ScaleAnimation;
import android.widget.RelativeLayout;

/**
 * 
 * @author glp
 * 
 * 为了便于该类使用，我用了偷懒的方式，如果要使用这个View，请把它置于FrameLayout中，不然无法正常工作
 *
 */

public class CircleLoadingView extends RelativeLayout {	
	boolean mIsLoading = false;
	
	View mBgView;
	View mPointView;
	GradientDrawable mShapeEnter;
	GradientDrawable mShapeLoading;
	
	AnimationSet mLoadingBefore;
	Animation mLoading;
	AnimationSet mLoadingAfter;
	
	boolean mFinishSuccess = false;
	
	OnTask mTaskListener = null;
	
	public CircleLoadingView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		
		mShapeEnter = new GradientDrawable();
		mShapeEnter.setShape(GradientDrawable.RECTANGLE);
		mShapeEnter.setColor(Color.parseColor("#ffff3d3d"));
		mShapeEnter.setCornerRadius(35);
		
		mShapeLoading = new GradientDrawable();
		mShapeLoading.setShape(GradientDrawable.OVAL);
		mShapeLoading.setColor(Color.parseColor("#ffff3d3d"));
		mShapeLoading.setCornerRadius(35);
		
		mBgView = new View(context);
		mBgView.setBackgroundDrawable(mShapeEnter);
		mBgView.setVisibility(View.GONE);
		addView(mBgView);
		
		Animation tmpAnim;
		mLoadingBefore = new AnimationSet(context,null);
		tmpAnim = new ScaleAnimation(1.1f, 0.44f, 1, 1, Animation.RELATIVE_TO_SELF, 0.5f
				, Animation.RELATIVE_TO_SELF, 0.5f);
		tmpAnim.setDuration(200);
		mLoadingBefore.addAnimation(tmpAnim);
		tmpAnim = new AlphaAnimation(0.6f, 1.0f);
		tmpAnim.setDuration(50);
		mLoadingBefore.addAnimation(tmpAnim);
		mLoadingBefore.setFillEnabled(true);
		mLoadingBefore.setFillAfter(true);
		
		mLoading = new RotateAnimation(0, 360, Animation.RELATIVE_TO_SELF, 0.5f
				, Animation.RELATIVE_TO_SELF, 0.5f);
		mLoading.setInterpolator(new LinearInterpolator());
		mLoading.setDuration(2000);
		mLoading.setRepeatCount(Animation.INFINITE);
		mLoading.setRepeatMode(Animation.RESTART);
		
		mLoadingAfter = new AnimationSet(context,null);
		tmpAnim = new ScaleAnimation(0.44f, 1, 1, 1, Animation.RELATIVE_TO_SELF, 0.5f
				, Animation.RELATIVE_TO_SELF, 0.5f);
		tmpAnim.setDuration(500);
		tmpAnim.setAnimationListener(new AnimationListener(){

			@Override
			public void onAnimationEnd(Animation animation) {
				// TODO Auto-generated method stub
				if(mFinishSuccess)
					setBackgroundResource(R.drawable.income_btn_2);
				else
				{
					setBackgroundResource(R.drawable.income_btn_3);
					setEnabled(true);
				}
			}

			@Override
			public void onAnimationRepeat(Animation animation) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onAnimationStart(Animation animation) {
				// TODO Auto-generated method stub
				
			}
			
		});
		mLoadingAfter.addAnimation(tmpAnim);
		tmpAnim = new AlphaAnimation(1.0f, 0);
		tmpAnim.setStartOffset(500);
		tmpAnim.setDuration(100);
		mLoadingAfter.setDuration(600);
		mLoadingAfter.addAnimation(tmpAnim);
		mLoadingAfter.setFillEnabled(true);
		mLoadingAfter.setFillAfter(true);
		
		mLoadingBefore.setAnimationListener(new AnimationListener(){

			@Override
			public void onAnimationStart(Animation animation) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onAnimationEnd(Animation animation) {
				// TODO Auto-generated method stub
				mBgView.setBackgroundDrawable(mShapeLoading);
				
				if(mPointView == null)
					mPointView = CircleLoadingView.this.findViewById(R.id.img_loading);
				mPointView.setVisibility(View.VISIBLE);
				mPointView.startAnimation(mLoading);
			}

			@Override
			public void onAnimationRepeat(Animation animation) {
				// TODO Auto-generated method stub
				
			}
			
		});
		
		mLoadingAfter.setAnimationListener(new AnimationListener(){

			@Override
			public void onAnimationEnd(Animation animation) {
				// TODO Auto-generated method stub
				mIsLoading = false;//动画的最终结束，认为是Loading的结束
			}

			@Override
			public void onAnimationRepeat(Animation animation) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void onAnimationStart(Animation animation) {
				// TODO Auto-generated method stub
				
			}
			
		});
	}
	public void initOnTask(OnTask listener){
		mTaskListener = listener;
	}
	
	public void beginTask(){
		if(!mIsLoading)
		{
			setEnabled(false);
			this.setBackgroundResource(R.color.transparent);
			mBgView.setVisibility(View.VISIBLE);
			mBgView.clearAnimation();
			mBgView.startAnimation(mLoadingBefore);
			
			if(mTaskListener != null)
				mTaskListener.onTaskBegin();
			mIsLoading = true;
		}
	}
	
	//外部通知加载View停止加载
	public void stopLoading(boolean success){
		this.mFinishSuccess = success;
		
		if(mPointView == null)
			mPointView = CircleLoadingView.this.findViewById(R.id.img_loading);
		mPointView.clearAnimation();
		mPointView.setVisibility(View.INVISIBLE);
		mBgView.clearAnimation();
		mBgView.setBackgroundDrawable(mShapeEnter);
		mBgView.startAnimation(mLoadingAfter);
	}

	public static boolean InRangeOfView(View view, MotionEvent event){
		//int[] location = new int[2];
		//view.getLocationOnScreen(location);//获取到view在屏幕中的位置
//		int x = view.getLeft();//location[0];
//		int y = view.getTop();//location[1];
		int width = view.getRight() - view.getLeft();
		int hegith = view.getBottom() - view.getTop();
		
		float eventX = event.getX();
		float eventY =event.getY();
		
		//getRawX 获取到的是事件发生的屏幕左边；getX 获取到的是相对控件左上的坐标
		//Log.i("onTouchEvent", ""+event.getAction()+",x="+event.getX()+",y="+event.getY()+",raw x="+event.getRawX()+"raw y="+event.getRawY());
		
		if(eventX < 0 || eventX > width || eventY < 0 || eventY > hegith)
		{
			return false;
		}
		return true;
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		Log.i("onTouchEvent", ""+event.getAction()+",x="+event.getX()+",raw x="+event.getRawX());
		
		if(event.getAction() == MotionEvent.ACTION_UP)
		{
			if(InRangeOfView(this, event))
			{
				beginTask();
			}
		}
		else if(event.getAction() == MotionEvent.ACTION_DOWN)
		{
			if(!mIsLoading && !mFinishSuccess)
				setBackgroundResource(R.drawable.hb_btn_income);
		}
		
		//如果正在加载或者获取成功！
		if(mIsLoading || mFinishSuccess)
			return false;
		return true;
	}

	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.onDraw(canvas);
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		// TODO Auto-generated method stub
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		
		//Log.i("onMeasure","widthMeasureSpec="+widthMeasureSpec+"heightMeasureSpec="+heightMeasureSpec);
	}

	@Override
	protected void onLayout(boolean changed, int left, int top, int right,
			int bottom) {
		// TODO Auto-generated method stub
		super.onLayout(changed, left, top, right, bottom);
//		LayoutParams params = new LayoutParams(bottom, bottom);
//		mBgView.setLayoutParams(params);
		//填充整个区域
		mBgView.layout(left, top, right, bottom);
		
//		int way_radius = Math.min(right-left-10, bottom-top-10)/2;
//		//int way_radius = (right-left)/2-50;//(8+2)
//		int point_x = (right-left)/2;
//		int point_y = (bottom-top)/2-way_radius;
//		mPointView.layout(point_x-mPointRadius, point_y, point_x+mPointRadius, point_y+mPointRadius*2);
		
		//Log.i("onLayout", "left="+left+",top="+top+",right="+right+",bottom="+bottom);
	}

	public interface OnTask{
		public void onTaskBegin();
	}
}
