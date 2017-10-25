package assistant.view;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PorterDuff.Mode;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.NinePatchDrawable;
import android.util.AttributeSet;
import android.widget.ImageView;

/**
 * @author 肖平平
 * @version 创建时间：2013-12-11 上午11:38:36
 * 类说明 绘制正圆
 */
public class RoundedImageView extends ImageView{
	
	private int mBorderThickness = 0;//边框
	private Context mContext;
	private int mBorderColor = 0xffffffff;//颜色
	
	Paint paint = new Paint();
	
	public RoundedImageView(Context context) {
		super(context);
		mContext = context;
	}
	
	public RoundedImageView(Context context, AttributeSet attrs) {
		super(context, attrs);
		mContext = context;
		setCustomAttributes(attrs);
	}
	
	public RoundedImageView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		mContext = context;
		setCustomAttributes(attrs);
	}

	private void setCustomAttributes(AttributeSet attrs) {
		//得到实例，对应的styleable值在values/attrs.xml中设置
		TypedArray array = mContext.obtainStyledAttributes(attrs, R.styleable.roundedimageview);
		//从array中获取对应的值，第二个参数为默认值，如果值在attrs.xml中没有定义，则返回第二个参数的值
		mBorderThickness = array.getDimensionPixelSize(R.styleable.roundedimageview_border_thickness, 0);
		mBorderColor = array.getColor(R.styleable.roundedimageview_border_color, mBorderColor);
		array.recycle();
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		Drawable drawable = getDrawable();
		if(null == drawable) {
			return;
		}
		int width = getWidth();
		int height = getHeight();
		if(0 == width || 0 == height) {
			return;
		}
		this.measure(0, 0);
		if(drawable.getClass() == NinePatchDrawable.class) {
			return;
		}
				
		Bitmap bitmap = ((BitmapDrawable)drawable).getBitmap();
		if(bitmap == null)
			return ;
		Bitmap bitmap2 = bitmap.copy(Bitmap.Config.ARGB_8888, true);

		//半径
		int radius = (width < height ? width : height)/2 - mBorderThickness-1;
		//画里面的小圆
		Bitmap roundBitmap = getCroppedBitmap(bitmap2, radius);
		
		
		if(0 == mBorderThickness){
			//不显示边框
			canvas.drawBitmap(roundBitmap, 0, 0, null);
		}else{
			//显示边框
			paint.setAntiAlias(true);
			paint.setFilterBitmap(true);
			paint.setDither(true);
			paint.setColor(mBorderColor);
			
			//画大圆
			canvas.drawCircle(width / 2,
					height / 2,
					radius + mBorderThickness, 
					paint);
			canvas.drawBitmap(roundBitmap, width / 2 - radius, height / 2 - radius, null);
		}
	}

	private Bitmap getCroppedBitmap(Bitmap bmp, int radius) {
		Bitmap scaledSrcBmp;
		//直径
		int diameter = radius * 2;
		if(bmp.getWidth() != diameter || bmp.getHeight() != diameter) {
			scaledSrcBmp = Bitmap.createScaledBitmap(bmp, diameter, diameter, false);
		}else{
			scaledSrcBmp = bmp;
		}
		Bitmap output = Bitmap.createBitmap(scaledSrcBmp.getWidth(), 
				scaledSrcBmp.getHeight(),Config.ARGB_8888);
		Canvas canvas = new Canvas(output);
		
		Paint paint = new Paint();
		//以图片的长宽画一个方形
		Rect rect = new Rect(0, 0, scaledSrcBmp.getWidth(), scaledSrcBmp.getHeight());
		
        paint.setFilterBitmap(true);
        paint.setDither(true);
        canvas.drawARGB(0, 255, 255, 255);
        paint.setColor(mBorderColor);//画笔颜色
        paint.setAntiAlias(true);
        
        canvas.drawCircle(scaledSrcBmp.getWidth() / 2,
                scaledSrcBmp.getHeight() / 2, scaledSrcBmp.getWidth() / 2, paint);
        //设置图片的相交模式
        paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN));
        canvas.drawBitmap(scaledSrcBmp, rect, rect, paint);
        return output;
	}

	public void setBorder(int border){
		mBorderThickness = border;
		this.invalidate();
	}
}
