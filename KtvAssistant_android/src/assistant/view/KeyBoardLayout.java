package assistant.view;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.RelativeLayout;
import assistant.util.KeyBoardListener;


public class KeyBoardLayout extends RelativeLayout {

	private static final String Tag = "GLP";
	KeyBoardListener m_Listener;
	boolean 				m_First = true;
	
	public KeyBoardLayout(Context context) {
		super(context);
	}

	public KeyBoardLayout(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	public void SetKeyBoardListener(KeyBoardListener listener){
		m_Listener = listener;
	}

	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh) {
		// TODO Auto-generated method stub
		super.onSizeChanged(w, h, oldw, oldh);
		
		if(m_First)
		{
			m_First = false;
		}
		else
		{
			if(Math.abs(oldh - h) > KeyBoardListener.KeyBoardHeight)
			{
				if(m_Listener!=null)
				{
					if(h > oldh)
					{
						m_Listener.OnKeyboardClose();
					}
					else
					{
						m_Listener.OnKeyBoardOpen();
					}
				}
				
			}
		}
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		// TODO Auto-generated method stub
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
	}

	@Override
	protected void onLayout(boolean changed, int l, int t, int r, int b) {
		// TODO Auto-generated method stub
		//Log.e(Tag,"onLayout l=" + l + ", t=" + t + ",r=" + r + ",b="+b);
		super.onLayout(changed, l, t, r, b);
	}

}
