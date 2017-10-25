package assistant.fragment.adapter;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import assistant.entity.GiftSendRecv;
import assistant.global.AppStatus;
import assistant.util.ImageUtil;
import assistant.view.RoundedImageView;

public class UserAcctGiftAdapter extends BaseAdapter {

	Context m_context;
	// ChatDisplayFragment.ChatTypePublic

	// 待处理数据
	List<GiftSendRecv> m_display = new ArrayList<GiftSendRecv>();

	public final static int MSG_UserHead = 1;
	@SuppressLint("HandlerLeak") 
	Handler m_handler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			switch (msg.what) {
			case MSG_UserHead:
				UserAcctGiftAdapter.this.notifyDataSetChanged();
				break;
			}
		}

	};

	public UserAcctGiftAdapter(Context context,
			List<GiftSendRecv> GiftLogList) {
		// TODO Auto-generated constructor stub
		m_context = context;
		m_display = GiftLogList;

		if (GiftLogList != null) {
			List<String> images = new ArrayList<String>();
			for (int i = 0; i < GiftLogList.size(); i++) {
				Bitmap bitmap = ImageUtil
						.getBitmapGift(GiftLogList.get(i).photo);
				if (bitmap == null)
					images.add(GiftLogList.get(i).photo);
				bitmap = ImageUtil.getBitmapGift(GiftLogList.get(i).photo);
				if (bitmap == null)
					images.add(GiftLogList.get(i).photo);
			}

			ImageUtil.showGiftImage(m_context, m_handler, images, MSG_UserHead);
		}
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
			viewHolder.msg_giftnumber = (TextView) convertView
					.findViewById(R.id.pc_msg_gold);
			viewHolder.msg_giftunitinfo = (TextView) convertView
					.findViewById(R.id.pc_msg_giftinfo);
			viewHolder.msg_tstype2 = (TextView) convertView
					.findViewById(R.id.pc_msg_tstype2);
			viewHolder.msg_gold2 = (TextView) convertView
					.findViewById(R.id.pc_msg_gold2);
			viewHolder.msg_date = (TextView) convertView
					.findViewById(R.id.pc_msg_date);
			viewHolder.msg_time = (TextView) convertView
					.findViewById(R.id.pc_msg_time);
			viewHolder.tv1= (TextView) convertView
					.findViewById(R.id.pc_msg_tv1);
			convertView.setTag(viewHolder);

		} else
			viewHolder = (ViewHolder) convertView.getTag();

		GiftSendRecv giftlog = m_display.get(position);
		boolean isMe = (TextUtils.isEmpty(giftlog.name)  || TextUtils.equals(giftlog.name,AppStatus.user.nickname));
		
		String _msg_tstype = "";
		String _msg_tstype2 = "";
		switch (giftlog.type) {
		case 1:
			if(isMe)
			{
				_msg_tstype = "送给我自己";
				_msg_tstype2 = "我自己得到";
			}
			else
			{
				_msg_tstype = "送给您";
				_msg_tstype2 = "您得到";
			}
			break;
		case 2:
			if(isMe)
			{
				_msg_tstype = "收到我自己";
				_msg_tstype2 = "我自己消费";
			}
			else
			{
				_msg_tstype = "收到您";
				_msg_tstype2 = "您消费";
			}
			break;
		}
		// AppStatus.user
		Bitmap bitmap = ImageUtil.getBitmapGift(giftlog.photo);
		if (bitmap != null)
			viewHolder.head_Img.setImageBitmap(bitmap);

		
		if(TextUtils.equals(giftlog.shopname,""))
		{
			viewHolder.tv1.setVisibility(View.GONE);
			viewHolder.ktvname.setVisibility(View.GONE);
		}
		else
		{
			viewHolder.tv1.setVisibility(View.VISIBLE);
			viewHolder.ktvname.setVisibility(View.VISIBLE);
			viewHolder.ktvname.setText(giftlog.shopname);
		}
		
		if(isMe)
			viewHolder.nickname.setText("我");
		else
			viewHolder.nickname.setText(giftlog.name);
		viewHolder.msg_tstype.setText(_msg_tstype);
		viewHolder.msg_tstype2.setText(_msg_tstype2);
		
		viewHolder.msg_giftnumber.setText("" + giftlog.giftnum);
		viewHolder.msg_giftunitinfo.setText(giftlog.giftUnit + giftlog.giftname);
		
		viewHolder.msg_gold2.setText(""+giftlog.gold);
		String times[] = giftlog.date.split(" ");
		viewHolder.msg_date.setText("" + times[0]!=null?times[0]:"");
		viewHolder.msg_time.setText("" + times[1]!=null?times[1]:"");
		return convertView;
	}

	static class ViewHolder {
		RoundedImageView head_Img;
		TextView nickname;
		TextView ktvname;
		TextView msg_tstype;
		TextView msg_giftnumber;
		TextView msg_giftunitinfo;
		TextView msg_tstype2;
		TextView msg_gold2;
		TextView msg_date;
		TextView msg_time;
		TextView tv1;
	}
}
