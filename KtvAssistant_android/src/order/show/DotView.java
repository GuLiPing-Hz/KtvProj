package order.show;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.view.View;


public class DotView extends View {
	private int m_dotCount = 0, selection = 0;
	int m_dotNormal,m_dotSelected;
	private Bitmap normalBitmap, selectBitmap;
	private int dotWidth, paddingWidth, leftWidth, topheight;

	void initImage(){
		initImage(0,0);
	}
	private void initImage(int normal,int selected){
		if(normal == 0 || selected == 0)
		{
			normal = R.drawable.point_c;
			selected = R.drawable.point;
		}
		
		normalBitmap = BitmapFactory.decodeResource(getResources(),
				normal);
		selectBitmap = BitmapFactory.decodeResource(getResources(),
				selected);
		topheight = normalBitmap!=null?normalBitmap.getHeight()/2:0;
	}
	public DotView(Context context) {
		super(context);
		initImage();
	}

	public DotView(Context context, AttributeSet attrs) {
		super(context, attrs);
		TypedArray a = context.getTheme().obtainStyledAttributes(attrs
				 ,R.styleable.DotView,0, 0);
	    try {
		   m_dotCount = a.getInteger(R.styleable.DotView_dotCount, 0);
		   m_dotNormal = a.getResourceId(R.styleable.DotView_dotNormal, 0);
		   m_dotSelected = a.getResourceId(R.styleable.DotView_dotSelected, 0);
	    } finally {
	       a.recycle();
	    }
		initImage(m_dotNormal,m_dotSelected);
	}

	public int getDotCount() {
		return m_dotCount;
	}
	public void setDotCount(int dotCount) {
		m_dotCount = dotCount;
		this.requestLayout();
	}
	
	private void initLayout(int viewWidth) {
		if(m_dotCount == 0)
			return ;
		
		dotWidth = normalBitmap.getWidth();
		paddingWidth = dotWidth * 2;
		
		leftWidth = (viewWidth - (m_dotCount * (dotWidth + paddingWidth) - paddingWidth)) / 2;
		if (leftWidth < 0) {
			paddingWidth = dotWidth;
			leftWidth = (viewWidth - (m_dotCount * (dotWidth + paddingWidth) - paddingWidth)) / 2;
			if (leftWidth < 0) {
				paddingWidth = (viewWidth - m_dotCount * dotWidth)/ (m_dotCount - 1);
				leftWidth = 0;
			}
		}
	}

	public void setSelection(int select) {
		if (select >= 0 && select < m_dotCount) {
			selection = select;
			invalidate();
		}
	}
	
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		// TODO Auto-generated method stub
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		//设置视图固定高度
		int height = normalBitmap!=null?normalBitmap.getHeight()*2:20;
		setMeasuredDimension(widthMeasureSpec,height); 
	}
	
	@Override
	protected void onLayout(boolean changed, int left, int top, int right,
			int bottom) {
		// TODO Auto-generated method stub
		super.onLayout(changed, left, top, right, bottom);
		//初始化视图的宽度，来计算点点的起始位置和间距
		initLayout(right-left);
	}
	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.onDraw(canvas);
		try {
			if (m_dotCount > 0) {

				for (int i = 0; i < m_dotCount; i++) {
					if (i == selection)
						canvas.drawBitmap(selectBitmap
								, leftWidth + i* (dotWidth + paddingWidth), topheight, null);
					else
						canvas.drawBitmap(normalBitmap
								, leftWidth + i* (dotWidth + paddingWidth), topheight, null);
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
