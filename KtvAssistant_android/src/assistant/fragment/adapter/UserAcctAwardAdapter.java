package assistant.fragment.adapter;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import assistant.activity.Home;
import assistant.entity.GoldFromInfo;
import assistant.view.RoundedImageView;

public class UserAcctAwardAdapter extends BaseAdapter {

	Context m_context;
	// ChatDisplayFragment.ChatTypePublic
	//private Home mHome;
	// 待处理数据
	List<GoldFromInfo> m_display = new ArrayList<GoldFromInfo>();

	public UserAcctAwardAdapter(Context context,
			List<GoldFromInfo> GiftLogList) {
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
		ViewHolder viewHolder = null;
		if (convertView == null) {
			viewHolder = new ViewHolder();
			convertView = LayoutInflater.from(m_context).inflate(
					R.layout.percenter_msgmodel, null);

			viewHolder.head_Img = (RoundedImageView) convertView
					.findViewById(R.id.pc_msg_head);
			viewHolder.nickname = (TextView) convertView
					.findViewById(R.id.pc_msg_nickname);
			viewHolder.ktvname = (TextView) convertView
					.findViewById(R.id.pc_msg_ktvaddr);
			viewHolder.msg_tstype = (TextView) convertView
					.findViewById(R.id.pc_msg_tstype);
			viewHolder.msg_score = (TextView) convertView
					.findViewById(R.id.pc_msg_gold);
			viewHolder.msg_scoreunit = (TextView) convertView
					.findViewById(R.id.pc_msg_giftinfo);
			viewHolder.msg_tstype2 = (TextView) convertView
					.findViewById(R.id.pc_msg_tstype2);
			viewHolder.msg_gold2 = (TextView) convertView
					.findViewById(R.id.pc_msg_gold2);
			viewHolder.msg_date = (TextView) convertView
					.findViewById(R.id.pc_msg_date);
			viewHolder.msg_time = (TextView) convertView
					.findViewById(R.id.pc_msg_time);
			viewHolder.tv1 = (TextView) convertView
					.findViewById(R.id.pc_msg_tv1);
			convertView.setTag(viewHolder);

		} else
			viewHolder = (ViewHolder) convertView.getTag();

		GoldFromInfo goldinfo = m_display.get(position);
		String _msg_tstype = "";
		switch (goldinfo.type) {
		case 7:
			viewHolder.msg_score.setVisibility(View.VISIBLE);
			viewHolder.msg_scoreunit.setVisibility(View.VISIBLE);
			
			_msg_tstype = "演唱歌曲《" + goldinfo.songname + "》获得";
			viewHolder.msg_score.setText(goldinfo.score);
			viewHolder.msg_scoreunit.setText("分");
			break;
		default:
			viewHolder.msg_score.setVisibility(View.GONE);
			viewHolder.msg_scoreunit.setVisibility(View.GONE);
			
			_msg_tstype = GoldFromInfo.GoldFromMsg.getMsg(goldinfo.type)+ " 得到奖励";
			break;
		}
		
		viewHolder.head_Img.setImageResource(GoldFromInfo.GoldFromMsg.getImage(goldinfo.type));
		
		if(TextUtils.equals(goldinfo.ktvname, ""))
		{
			viewHolder.ktvname.setVisibility(View.GONE);
			viewHolder.tv1.setVisibility(View.GONE);
		}
		else
		{
			viewHolder.ktvname.setVisibility(View.VISIBLE);
			viewHolder.tv1.setVisibility(View.VISIBLE);
			viewHolder.ktvname.setText(goldinfo.ktvname);
		}
		
		viewHolder.nickname.setText(R.string.text_me);
		viewHolder.msg_tstype.setText(_msg_tstype);
		
		viewHolder.msg_tstype2.setText("您得到");
		viewHolder.msg_gold2.setText("" + goldinfo.gold);
		
		String times[] = goldinfo.time.split(" ");
		viewHolder.msg_date.setText("" + times[0]!=null?times[0]:"");
		viewHolder.msg_time.setText("" + times[1]!=null?times[1]:"");

		return convertView;
	}

	static class ViewHolder {
		RoundedImageView head_Img;
		TextView nickname;
		TextView ktvname;
		TextView msg_tstype;
		TextView msg_score;
		TextView msg_scoreunit;
		TextView msg_tstype2;
		TextView msg_gold2;
		TextView msg_date;
		TextView msg_time;
		TextView tv1;
	}
}
