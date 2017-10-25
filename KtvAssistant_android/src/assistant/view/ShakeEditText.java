package assistant.view;

import android.content.Context;
import android.util.AttributeSet;
import android.view.animation.Animation;
import android.view.animation.CycleInterpolator;
import android.view.animation.TranslateAnimation;

/**
 * @author 肖平平
 * @version 创建时间：2013-12-30 上午8:39:12
 * 类说明  左右抖动功能的编辑框
 */
public class ShakeEditText extends DrawableClickEditText{
	
	public ShakeEditText(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	/**
	 * 设置晃动动画
	 * */
    public void startShakeAnimation() {
    	setAnimation(shakeAnimation(5));
    }
    
    /**
     * 晃动动画
     * */
    public static Animation shakeAnimation(int counts) {
    	Animation translateAnimation = new TranslateAnimation(0, 8, 0, 0);
    	//设置循环加速器
    	translateAnimation.setInterpolator(new CycleInterpolator(counts));
    	translateAnimation.setDuration(1000);
    	return translateAnimation;
    }
}
