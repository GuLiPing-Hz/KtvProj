package order.adapter;

import java.util.List;

import order.show.TextExView;
import tiange.ktv.assistant.R;
import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.entity.GradeSongTopInfo;

public class GradeSongTopAdapter extends BaseAdapter {

	final static String Tag = "GradeSongTopAdapter";
	List<GradeSongTopInfo> m_list;
	Context m_context;
	
	public GradeSongTopAdapter(Context context,List<GradeSongTopInfo> list) {
		super();
		this.m_context = context;
		this.m_list = list;
		
		if(m_list.size() < 12)
			Log.e(Tag, "list is not enough");
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return (m_list.size()==12)?4:0;//4个分类
	}

	@Override
	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public long getItemId(int arg0) {
		// TODO Auto-generated method stub
		return arg0;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		
		ViewHolder holder = null;
		if (convertView == null) {
			convertView = LayoutInflater.from(m_context).inflate(R.layout.adapter_gradesong_top, null);
			holder = new ViewHolder();
			holder.icon = (ImageView) convertView.findViewById(R.id.imgViewTip);
			holder.songf = (TextExView) convertView.findViewById(R.id.textViewFSong);
			holder.singerf = (TextExView) convertView.findViewById(R.id.textViewFSinger);
			holder.scoref = (TextView) convertView.findViewById(R.id.textViewFGrade);
			holder.songs = (TextExView) convertView.findViewById(R.id.textViewSSong);
			holder.singers = (TextExView) convertView.findViewById(R.id.textViewSSinger);
			holder.scores = (TextView) convertView.findViewById(R.id.textViewSGrade);
			holder.songt = (TextExView) convertView.findViewById(R.id.textViewTSong);
			holder.singert = (TextExView) convertView.findViewById(R.id.textViewTSinger);
			holder.scoret = (TextView) convertView.findViewById(R.id.textViewTGrade);
			convertView.setTag(holder);
		} else {
			holder = (ViewHolder) convertView.getTag();
		}
		
		switch(position)
		{
		case 0:
			setViewInfo(holder,0,R.drawable.rank_pic_top100);
			break;
		case 1:
			setViewInfo(holder,1,R.drawable.rank_pic_today);
			break;
		case 2:
			setViewInfo(holder,2,R.drawable.rank_pic_week);
			break;
		case 3:
			setViewInfo(holder,3,R.drawable.rank_pic_month);
			break;
		}
		
		return convertView;
	}
	
	void setViewInfo(ViewHolder holder,int index,int icon){
		
		holder.icon.setImageResource(icon);
		
		index = index*3;
		holder.scoref.setText(""+m_list.get(index).score);
		holder.singerf.setText(m_list.get(index).songinfo.songArtist);
		holder.songf.setText(m_list.get(index).songinfo.songName);
		
		holder.scores.setText(""+m_list.get(index+1).score);
		holder.singers.setText(m_list.get(index+1).songinfo.songArtist);
		holder.songs.setText(m_list.get(index+1).songinfo.songName);
		
		holder.scoret.setText(""+m_list.get(index+2).score);
		holder.singert.setText(m_list.get(index+2).songinfo.songArtist);
		holder.songt.setText(m_list.get(index+2).songinfo.songName);
	}

	static class ViewHolder{
		ImageView icon;
		//first
		TextExView songf;
		TextExView singerf;
		TextView scoref;
		//second
		TextExView songs;
		TextExView singers;
		TextView scores;
		//third
		TextExView songt;
		TextExView singert;
		TextView scoret;
	}
}
