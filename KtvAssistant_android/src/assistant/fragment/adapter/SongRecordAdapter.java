package assistant.fragment.adapter;

import java.util.List;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import assistant.entity.SongRecord;

public class SongRecordAdapter extends BaseAdapter {

	Context mContext;
	List<SongRecord> mList;
	
	public SongRecordAdapter(Context context,List<SongRecord> list)
	{
		mContext = context;
		mList = list;
	}
	
	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return mList.size();
	}

	@Override
	public Object getItem(int position) {
		// TODO Auto-generated method stub
		return mList.get(position);
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		ViewHolder viewHolder = null;
		if(convertView == null)
		{
			convertView = LayoutInflater.from(mContext).inflate(R.layout.item_songrecord, null);
			viewHolder = new ViewHolder();
			viewHolder.uiSongInfo = (TextView) convertView.findViewById(R.id.tvSongInfo);
			viewHolder.uiRoom = (TextView) convertView.findViewById(R.id.tvRoom);
			viewHolder.uiTime = (TextView) convertView.findViewById(R.id.tvTime);
			convertView.setTag(viewHolder);
		}
		else
			viewHolder = (ViewHolder)convertView.getTag();
		
		SongRecord info = mList.get(position);
		if(info == null || viewHolder == null)
			return convertView;
		
		String str = String.format("%s %s %.1f", info.songname,info.singername,info.score/10.0f);
		viewHolder.uiSongInfo.setText(str);
		viewHolder.uiRoom.setText(TextUtils.isEmpty(info.room)?"未在包厢":info.room);
		viewHolder.uiTime.setText(info.time);
		
		return convertView;
	}

	static class ViewHolder {
		TextView uiSongInfo;
		TextView uiRoom;
		TextView uiTime;
	}
}
