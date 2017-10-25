package assistant.view;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnFocusChangeListener;
import android.view.animation.Animation;
import android.view.animation.CycleInterpolator;
import android.view.animation.TranslateAnimation;
import android.widget.EditText;

/**
 * @author 肖平平
 * @version 创建时间：2013-12-30 上午8:39:12
 * 类说明  带清空、左右抖动功能的编辑框
 */
public class ClearEditText extends EditText implements OnFocusChangeListener, TextWatcher{

	private Drawable mClearDrawable;
	private boolean hasFoucs;
	public ClearEditText(Context context) {
		this(context, null);
	}
	
	public ClearEditText(Context context, AttributeSet attrs) {
		this(context, attrs, android.R.attr.editTextStyle);
	}
	
	public ClearEditText(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init();
	}
	
	private void init() {
		mClearDrawable = getCompoundDrawables()[2];
		//如果EditText的DrawableRight没有，则设置默认的图片
		if (null == mClearDrawable) {
			mClearDrawable = getResources().getDrawable(R.drawable.btn_dlt);
		}
		
		mClearDrawable.setBounds(0, 0, mClearDrawable.getIntrinsicWidth(), mClearDrawable.getIntrinsicHeight());
		//默认设置隐藏图标
		setClearIconVisible(false);
		//设置焦点改变的监听
		setOnFocusChangeListener(this);
		//设置输入框里面内容发生改变的监听
		addTextChangedListener(this);
	}
	
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (event.getAction() == MotionEvent.ACTION_UP) {
			if (getCompoundDrawables()[2] != null) {
				
				boolean touchable = event.getX() > (getWidth() - getTotalPaddingRight())
						&& (event.getX() < ((getWidth() - getPaddingRight())));
				
				if (touchable) {
					this.setText("");
				}
			}
		}
		return super.onTouchEvent(event);
	}
	
	

	/**
	 * 设置图标的显示与隐藏
	 * */
	@Override
	protected void onFocusChanged(boolean focused, int direction,
			Rect previouslyFocusedRect) {
		this.hasFoucs = hasFoucs;
		if (hasFoucs) {
			setClearIconVisible(getText().length() > 0);
		} else {
			setClearIconVisible(false);
		}
	}
	
	/**
	 * 绘制图片
	 * */
	protected void setClearIconVisible(boolean visible) {
		Drawable right = visible ? mClearDrawable : null;
		setCompoundDrawables(null, null, right, null);
	}
  
	/**
     * 当输入框里面的内容发生变化的时候回调方法
     * */
	@Override
	public void onTextChanged(CharSequence text, int start,
			int lengthBefore, int lengthAfter) {
		if (hasFoucs) {
			setClearIconVisible(text.length() > 0);
		}
	}
	
	/**
	 * 设置晃动动画
	 * */
    public void setShakeAnimation() {
    	this.setAnimation(shakeAnimation(5));
    }
    
    
    /**
     * 晃动动画
     * */
    public static Animation shakeAnimation(int counts) {
    	Animation translateAnimation = new TranslateAnimation(0, 10, 0, 0);
    	translateAnimation.setInterpolator(new CycleInterpolator(counts));
    	translateAnimation.setDuration(1000);
    	return translateAnimation;
    }
    
	@Override
	public void afterTextChanged(Editable s) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void beforeTextChanged(CharSequence s, int start, int count,
			int after) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onFocusChange(View v, boolean hasFocus) {
		// TODO Auto-generated method stub
		this.hasFoucs = hasFoucs;
		if (hasFoucs) {
			setClearIconVisible(getText().length() > 0);
		} else {
			setClearIconVisible(false);
		}
	}

}
