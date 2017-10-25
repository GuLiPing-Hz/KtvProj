package assistant.fragment.adapter;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import assistant.entity.KtvShopInfo;

public class UserGiftDuihuanAdapter extends BaseAdapter {
	// View[] itemViews;
	private Context mContext;
	List<KtvShopInfo> ktvlist = null;

	public UserGiftDuihuanAdapter(Context context,
			ArrayList<KtvShopInfo> ktvList) {
		ktvlist = ktvList;
		this.mContext = context;

		// this.notifyDataSetChanged();
	}

	@Override
	public int getCount() {
		return ktvlist.size();
	}

	@Override
	public Object getItem(int position) {
		return ktvlist.get(position);
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		ViewHolder viewHolder = null;
		if (convertView == null) {
			viewHolder = new ViewHolder();
			convertView = LayoutInflater.from(mContext).inflate(
					R.layout.percenter_gift_ktvlistitem, null);
			viewHolder.title = (TextView) convertView
					.findViewById(R.id.pc_gift_ktvitem_ktvname);
			viewHolder.text = (TextView) convertView
					.findViewById(R.id.pr_gift_ktvitem_addr);

			convertView.setTag(viewHolder);
		} else
			viewHolder = (ViewHolder) convertView.getTag();

		KtvShopInfo info = ktvlist.get(position);
		viewHolder.title.setText(info.name);
		viewHolder.text.setText(info.address);
		return convertView;
	}

	static class ViewHolder {
		TextView title;
		TextView text;
	}
}
