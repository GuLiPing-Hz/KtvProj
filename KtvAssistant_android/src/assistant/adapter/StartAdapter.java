package assistant.adapter;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.support.v4.view.PagerAdapter;
import android.view.View;
import android.view.ViewGroup;

public class StartAdapter extends PagerAdapter {

	public static final int TotalPage = 3;
	
	List<View> m_listViews = new ArrayList<View>();
	Context m_context;
	public StartAdapter(Context context) {
		// TODO Auto-generated constructor stub
		m_context = context;
		
//		LayoutInflater inflater = LayoutInflater.from(m_context);
//		View v1 = inflater.inflate(R.layout.imgview_start, null);
//		m_listViews.add(v1);
//		View v2 = inflater.inflate(R.layout.imgview_start, null);
//		((ImageView) v2).setImageResource(R.drawable.start_2);
//		m_listViews.add(v2);
//		View v3 = inflater.inflate(R.layout.imgview_start, null);
//		((ImageView) v3).setImageResource(R.drawable.start_3);
//		m_listViews.add(v3);
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return m_listViews.size();
	}

	@Override
	public boolean isViewFromObject(View arg0, Object arg1) {
		// TODO Auto-generated method stub
		return arg0 == arg1;
	}

	@Override
	public void destroyItem(ViewGroup container, int position, Object object) {
		// TODO Auto-generated method stub
		container.removeView(m_listViews.get(position));
	}

	@Override
	public Object instantiateItem(ViewGroup container, int position) {
		// TODO Auto-generated method stub
		container.addView(m_listViews.get(position));
		
		return m_listViews.get(position);
	}
}
