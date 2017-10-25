package assistant.view;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.graphics.Canvas;
import android.os.SystemClock;
import android.util.AttributeSet;
import android.view.View;

public class PngAnimView extends View {
	List<Integer> mResPngList = new ArrayList<Integer>();
	int mCurIndex = 0;
	int mFrameTime;
	long mStartTime = -1;
	long mCurTime;
	
	public void initPngList(List<Integer> list,int frameTime){
		mResPngList.addAll(list);
		mFrameTime = frameTime;
		this.invalidate();
	}
	
	public void initPngList(int frameTime){
		List<Integer> list = new ArrayList<Integer>();
		list.add(R.drawable.loading1);
		list.add(R.drawable.loading2);
		list.add(R.drawable.loading3);
		list.add(R.drawable.loading4);
		list.add(R.drawable.loading5);
		list.add(R.drawable.loading6);
		list.add(R.drawable.loading7);
		list.add(R.drawable.loading8);
		list.add(R.drawable.loading9);
		list.add(R.drawable.loading10);
		list.add(R.drawable.loading11);
		list.add(R.drawable.loading12);
		list.add(R.drawable.loading13);
		
		initPngList(list,frameTime);
	}
	
	public void initPngList(){
		initPngList(50);
	}
	
	public PngAnimView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}

	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.onDraw(canvas);
		
		if(this.getVisibility() == View.VISIBLE)
		{
			mCurTime = SystemClock.uptimeMillis ();
			if(mStartTime == -1)
				mStartTime = mCurTime;
			
			if((mCurTime - mStartTime > mFrameTime) || (mCurTime - mStartTime == 0))
			{
				mStartTime = mCurTime;
				if(mResPngList.size() > 0)
				{
					this.setBackgroundResource(mResPngList.get(mCurIndex++));
					if(mCurIndex == mResPngList.size())
						mCurIndex = 0;
					postDelayed(new Runnable(){

						@Override
						public void run() {
							// TODO Auto-generated method stub
							PngAnimView.this.invalidate();
						}
						
					}, mFrameTime);
				}
			}
		}
	}

}
