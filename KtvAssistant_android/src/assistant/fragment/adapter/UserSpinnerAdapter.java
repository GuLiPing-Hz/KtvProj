package assistant.fragment.adapter;

import java.util.List;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.jni.netutil.LocalData_UserInfo;

public class UserSpinnerAdapter extends BaseAdapter{
	
	Context m_context;
	List<LocalData_UserInfo> m_list;
	
	public UserSpinnerAdapter(Context context,
			List<LocalData_UserInfo> objects) {
		m_context = context;
		m_list = objects;
	}
	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return m_list.size();
	}
	@Override
	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return m_list.get(arg0);
	}
	@Override
	public long getItemId(int arg0) {
		// TODO Auto-generated method stub
		return 0;
	}
	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		return getCustromView(position,convertView,false);
	}
	@Override
	public View getDropDownView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		return getCustromView(position,convertView,true);
	}
	
	View getCustromView(int position, View convertView,boolean isPop){
		ViewHolder viewHolder = null;
		if(convertView == null)
		{
			viewHolder = new ViewHolder();
			if(isPop)
			{
				convertView = LayoutInflater.from(m_context).inflate(R.layout.adapter_spinner_popup, null);
				viewHolder.text = (TextView) convertView.findViewById(R.id.textViewAdapterSpinnerPop);
			}
			else
			{
				convertView = LayoutInflater.from(m_context).inflate(R.layout.adapter_spinner_normal, null);
				viewHolder.text = (TextView) convertView.findViewById(R.id.textViewAdapterSpinner);
			}
			convertView.setTag(viewHolder);
		}
		else
			viewHolder = (ViewHolder) convertView.getTag();
		
		LocalData_UserInfo info = m_list.get(position);
		viewHolder.text.setText(info.m_name);
		
		return convertView;
	}
	
	static class ViewHolder{
		TextView text;
	}
}
