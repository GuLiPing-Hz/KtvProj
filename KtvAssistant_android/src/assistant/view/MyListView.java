package assistant.view;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.ListView;

/**
 * @author song
 * @version 创建时间：2013-5-13 上午10:28:37
 * 类说明 ScrollView下嵌套的listview
 */
public class MyListView extends ListView{

	public MyListView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		int expandSpec = MeasureSpec.makeMeasureSpec(Integer.MAX_VALUE >> 2,  
                MeasureSpec.AT_MOST);  
		super.onMeasure(widthMeasureSpec, expandSpec);
	}
	
}
