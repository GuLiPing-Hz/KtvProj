package order.adapter;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import assistant.entity.TopicInfo;


/**
 * @author 宋静
 * @version 创建时间：2013-10-25 上午11:30:02
 * 类说明 点歌类型的适配器
 */
public class TopicAdapter extends BaseAdapter{
	private Context mContext;
	private List<TopicInfo> list = new ArrayList<TopicInfo>();
	
	public TopicAdapter(Context mContext, List<TopicInfo> names) {
		super();
		this.mContext = mContext;
		this.list = names;
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return list.size();
	}

	@Override
	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return list.get(arg0);
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		//set view
		ViewHolder holder;
		if (convertView == null) {
			convertView = LayoutInflater.from(mContext).inflate(R.layout.item_order_type, null);
			holder = new ViewHolder();
			holder.text = (TextView) convertView.findViewById(R.id.tv_name);
			convertView.setTag(holder);
		} else {
			holder = (ViewHolder) convertView.getTag();
		}
		
		//set data
		holder.text.setText(list.get(position).topicTitle);
		
		return convertView;
	}
	
	static class ViewHolder{
		TextView text;
	}
}
