package assistant.adapter;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import assistant.global.AppStatus;

public class VersionDetailAdapter extends BaseAdapter {
	Context m_context;
	List<String> m_list = new ArrayList<String>();
	public VersionDetailAdapter(Context context) {
		// TODO Auto-generated constructor stub
		m_context = context;
		if(AppStatus.s_newVU != null)
			m_list = AppStatus.s_newVU.details;
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
		return arg0;
	}

	@Override
	public View getView(int pos, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		TextView text = null;
		if(convertView == null)
		{
			convertView = LayoutInflater.from(m_context).inflate(R.layout.adapter_version_detail, null);
			text = (TextView) convertView.findViewById(R.id.textViewVersionDetail);
			convertView.setTag(text);
		}
		else
			text = (TextView) convertView.getTag();
		
		//设置详细更新信息
		text.setText(m_list.get(pos));
		
		return convertView;
	}
}
