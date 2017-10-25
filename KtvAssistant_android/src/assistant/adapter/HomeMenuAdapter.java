package assistant.adapter;

import java.util.ArrayList;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.entity.HomeMenuItem;
import assistant.manager.HomeMenuItemManager;

/**
 * @author GLP
 */
public class HomeMenuAdapter extends BaseAdapter {
	private Context m_context;
	private int select;
	
	private ArrayList<HomeMenuItem> m_HomeMenuItemList;

	public HomeMenuAdapter(Context con) {
		super();
		this.m_context = con;
		
		m_HomeMenuItemList = HomeMenuItemManager.getSingleInstance(con).getHomeMenuItemList();
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return m_HomeMenuItemList.size();
	}

	@Override
	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return m_context.getText(m_HomeMenuItemList.get(arg0).getStringId());
	}
	
	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		
		if(m_HomeMenuItemList == null || m_HomeMenuItemList.size()<=position)
			return null;
		
		if(convertView == null)
			convertView = LayoutInflater.from(m_context).inflate(R.layout.item_home_menu,null);
		
		ImageView icon = (ImageView) convertView.findViewById(R.id.iv_icon);
		TextView text = (TextView) convertView.findViewById(R.id.tv_name);
		
		HomeMenuItem hmi = m_HomeMenuItemList.get(position);
		
		text.setText(hmi.getStringId());
		icon.setImageResource(hmi.getIconId());

		if (position == select) 
			convertView.setBackgroundColor(m_context.getResources().getColor(hmi.getSelectColorId()));
		else
			convertView.setBackgroundResource(hmi.getDrawableId());

		return convertView;
	}

	public void setSelection(int position) {
		select = position;
		this.notifyDataSetChanged();
	}
}
