package order.show;

import android.content.Context;
import android.support.v4.view.ViewPager;
import android.util.AttributeSet;
import android.view.MotionEvent;

public class ViewPagerEx extends ViewPager {
    
	public ViewPagerEx(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}

	public ViewPagerEx(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}

//	@Override  
//    public boolean onInterceptTouchEvent(MotionEvent arg0) {  
//        // TODO Auto-generated method stub  
//        //当拦截触摸事件到达此位置的时候，返回true，  
//        //说明将onTouch拦截在此控件，进而执行此控件的onTouchEvent  
//        return true;  
//    } 
	
	@Override  
    public boolean onTouchEvent(MotionEvent arg0) {  
        // TODO Auto-generated method stub  
  
        if(arg0.getAction() == MotionEvent.ACTION_DOWN){  
            //此句代码是为了通知他的父ViewPager现在进行的是本控件的操作，不要对我的操作进行干扰  
            getParent().requestDisallowInterceptTouchEvent(true);  
        }  
  
        if(arg0.getAction() == MotionEvent.ACTION_MOVE){  
            //此句代码是为了通知他的父ViewPager现在进行的是本控件的操作，不要对我的操作进行干扰  
            getParent().requestDisallowInterceptTouchEvent(true);  
        }  
  
        if(arg0.getAction() == MotionEvent.ACTION_UP){  
        }  
  
        return super.onTouchEvent(arg0);  
    }  
}
