package assistant.fragment.adapter;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.entity.GiftExchangeRecord;
import assistant.fragment.pager.GiftDisplayFragment;

public class GiftLogFragmentAdapter extends BaseAdapter {

	Context m_context;
	// ChatDisplayFragment.ChatTypePublic

	// 待处理数据
	List<GiftExchangeRecord> m_display = new ArrayList<GiftExchangeRecord>();

	public GiftLogFragmentAdapter(Context context,
			List<GiftExchangeRecord> GiftLogList) {
		// TODO Auto-generated constructor stub
		m_context = context;
		m_display = GiftLogList;
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return m_display.size();
	}

	@Override
	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return m_display.get(arg0);
	}

	@Override
	public long getItemId(int arg0) {
		// TODO Auto-generated method stub
		return arg0;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		
		if (m_display.size() <= position)
			return null;
		
		GiftExchangeRecord giftlog = m_display.get(position);
		if(giftlog == null)
			return convertView;
		ViewHolder viewHolder = null;
		if (convertView == null) {
			viewHolder = new ViewHolder();
			convertView = LayoutInflater.from(m_context).inflate(
					R.layout.percenter_gift_dhuanlogitem, null);

			viewHolder.gift_Name = (TextView) convertView
					.findViewById(R.id.pc_giftname);
			viewHolder.gift_Count = (TextView) convertView
					.findViewById(R.id.pc_giftcount);
			viewHolder.gift_Gold = (TextView) convertView
					.findViewById(R.id.pc_giftgold);
			viewHolder.gift_Code = (TextView) convertView
					.findViewById(R.id.pc_gift_dhcode);
			viewHolder.gift_KtvName = (TextView) convertView
					.findViewById(R.id.pc_gift_ktvaddr);
			viewHolder.gift_SDate = (TextView) convertView
					.findViewById(R.id.pc_gift_validdate_s);
			viewHolder.gift_EDate = (TextView) convertView
					.findViewById(R.id.pc_gift_validdate_e);
			viewHolder.giftState = (ImageView) convertView
					.findViewById(R.id.pc_gift_imgdh);

			//预先设置，因为所有当前页的convertView标签都一样的
			switch (giftlog.type) {
			case GiftDisplayFragment.GiftType_Nuget:
				viewHolder.giftState.setImageResource(R.drawable.gift_tag_reg);
				break;
			case GiftDisplayFragment.GiftType_Geted:
				viewHolder.giftState.setImageResource(R.drawable.gift_tag_receive);
				break;
			case GiftDisplayFragment.GiftType_Invaild:
				viewHolder.giftState.setImageResource(R.drawable.gift_tag_over);
				break;
			}
			convertView.setTag(viewHolder);

		} else
			viewHolder = (ViewHolder) convertView.getTag();

		
		viewHolder.gift_Name.setText(giftlog.giftname);
		viewHolder.gift_Count.setText("" + giftlog.number);
		viewHolder.gift_Gold.setText("" + giftlog.cost);
		viewHolder.gift_Code.setText("" + giftlog.code);
		viewHolder.gift_KtvName.setText("" + giftlog.shopname);
		viewHolder.gift_SDate.setText("" + giftlog.begin);
		viewHolder.gift_EDate.setText("" + giftlog.end);

		return convertView;
	}

	static class ViewHolder {
		TextView gift_Name;
		TextView gift_Count;
		TextView gift_Gold;
		TextView gift_Code;
		TextView gift_KtvName;
		TextView gift_SDate;
		TextView gift_EDate;
		ImageView giftState;
		int static_type;
		String unit = "";
	}
}
