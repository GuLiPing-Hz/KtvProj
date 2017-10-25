package assistant.view;

import android.R.integer;
import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.view.animation.AlphaAnimation;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.AnticipateInterpolator;
import android.view.animation.ScaleAnimation;
import android.widget.TextView;

/**
 * @author 肖平平
 * @version 创建时间：2014-1-8 下午3:54:02
 * 类说明  Textview 淡出 缩放动画效果
 */
public class AlphaAndScaleTextView extends TextView{

	public AlphaAndScaleTextView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	public void setAlphaAndScaleAnimation(int points) {
		this.setText("+" + points);
		this.setAnimation(alphaAndScaleAnimation());
	}
	
	public static AnimationSet alphaAndScaleAnimation() {
		//淡出效果
		Animation alph = new AlphaAnimation(1.0f, 0.0f);
		alph.setDuration(600);
		//缩放效果
		Animation scan = new ScaleAnimation(1.0f, 0.0f, 1.0f, 0.0f, 
				Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF, 0.5f);
		scan.setDuration(600);
		AnimationSet set = new AnimationSet(true);
		set.addAnimation(alph);
		set.addAnimation(scan);
		set.setFillAfter(true);
		set.setFillEnabled(true);
		set.setInterpolator(new AnticipateInterpolator());
		return set;
	}

}
