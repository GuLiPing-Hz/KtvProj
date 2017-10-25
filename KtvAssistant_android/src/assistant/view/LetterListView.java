package assistant.view;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

/**
 * @author song
 * @version 创建时间：2012-12-6 下午2:15:17
 * 类说明
 */
public class LetterListView extends View {
	
	OnTouchingLetterChangedListener onTouchingLetterChangedListener;
	String[] letters = {"#","A","B","C","D","E","F","G","H","I","J","K","L"
			,"M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
	int choose = -1;
	Paint paint = new Paint();
	boolean showNoticeView = false;
	boolean isFinished = false;

	public LetterListView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}

	public LetterListView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public LetterListView(Context context) {
		super(context);
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		if(isFinished){
			return;
		}
					
		if(showNoticeView){
		    canvas.drawColor(0x3d22262a);
		}else
			canvas.drawColor(0x00000000);
		
	    int height = getHeight();
	    int width = getWidth();
	    int singleHeight = height / letters.length;
	    float size = width > singleHeight ? singleHeight:width;
//	    MyLog.e("LetterListView", "onDraw size:"+size);	    
	    size = size * 0.8f;
	    
	    for(int i=0; i<letters.length; i++){
	       paint.setColor(0xff696c70);
//	       paint.setTypeface(Typeface.DEFAULT_BOLD);
	       paint.setAntiAlias(true);
	       paint.setTextSize(size);
	       if(i == choose){
	    	   paint.setColor(0xff606366);
	    	   paint.setFakeBoldText(true);
	       }
	       float xPos = width/2  - paint.measureText(letters[i])/2;
	       float yPos = singleHeight * i + singleHeight;
	       canvas.drawText(letters[i], xPos, yPos, paint);
	       paint.reset();
	    }
	   
	}
	
	@Override
	public boolean dispatchTouchEvent(MotionEvent event) {
		final int action = event.getAction();
	    final float y = event.getY();
	    final int oldChoose = choose;
	    final OnTouchingLetterChangedListener listener = onTouchingLetterChangedListener;
	    final int c = (int) (y/getHeight()*letters.length);
	    
		switch (action) {
			case MotionEvent.ACTION_DOWN:
				showNoticeView = true;
				if(oldChoose != c && listener != null){
					if(c > 0 && c< letters.length){
						listener.onTouchingLetterChanged(letters[c]);
						choose = c;
						invalidate();
					}
				}
				
				break;
			case MotionEvent.ACTION_MOVE:
				if(oldChoose != c && listener != null){
					showNoticeView = true;
					if(c > 0 && c< letters.length){
						listener.onTouchingLetterChanged(letters[c]);
						choose = c;
						invalidate();
					}
				}
				break;
			case MotionEvent.ACTION_UP:
				showNoticeView = false;			
				invalidate();
//				choose = -1;
				break;
		}
		return true;
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		return super.onTouchEvent(event);
	}

	public void setOnTouchingLetterChangedListener(
			OnTouchingLetterChangedListener onTouchingLetterChangedListener) {
		this.onTouchingLetterChangedListener = onTouchingLetterChangedListener;
	}

	public interface OnTouchingLetterChangedListener{
		public void onTouchingLetterChanged(String s);
	}
	
	public void finished(){
		isFinished = true;
		invalidate();
	}
}