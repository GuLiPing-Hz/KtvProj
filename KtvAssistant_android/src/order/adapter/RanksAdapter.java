package order.adapter;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.entity.TopicInfo;
import assistant.util.ImageUtil;

public class RanksAdapter extends BaseAdapter implements View.OnClickListener{
	public static final int PIC_NOTIFY = 1;
	
	OnRanksClick mCallBack;
	Context mContext;
	List<TopicInfo> mDataList;
	int mListSize;
	
	@SuppressLint("HandlerLeak")
	Handler mHandler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			switch(msg.what)
			{
				case PIC_NOTIFY:
				{
					RanksAdapter.this.notifyDataSetChanged();
				}
			}
		}
		
	};
	
	public RanksAdapter(Context context,OnRanksClick callBack,List<TopicInfo> list){
		mContext = context;
		mCallBack = callBack;
		mDataList = list;
		mListSize = mDataList.size()/2 + (mDataList.size()%2 == 0?0:1);
		
		List<String> downlist = new ArrayList<String>();
		for(int i=0;i<list.size();i++)
			downlist.add(list.get(i).topicImg);
		ImageUtil.showBannerImage(mContext, mHandler, downlist, PIC_NOTIFY, false);
	}
	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return mListSize;
	}

	@Override
	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		ViewHolder viewHolder = null;
		if(convertView == null)
		{
			viewHolder = new ViewHolder();
			convertView = LayoutInflater.from(mContext).inflate(R.layout.item_ranks, null);
			viewHolder.uiImg1 = (ImageView) convertView.findViewById(R.id.imgbtn_ranks1);
			viewHolder.uiTitle1 = (TextView) convertView.findViewById(R.id.tv_ranks_title1);
			viewHolder.uiImg2 = (ImageView) convertView.findViewById(R.id.imgbtn_ranks2);
			viewHolder.uiTitle2 = (TextView) convertView.findViewById(R.id.tv_ranks_title2);
			viewHolder.uiFirst = convertView.findViewById(R.id.rl_ranks_first);
			viewHolder.uiSecond = convertView.findViewById(R.id.rl_ranks_second);
			convertView.setTag(viewHolder);
		}
		else
			viewHolder = (ViewHolder) convertView.getTag();
		
		TopicInfo info = mDataList.get(2*position);
		if(info == null)
			return convertView;
		
		Bitmap bmap = ImageUtil.getBitmapBanner(info.topicImg);
		if(bmap == null)
			viewHolder.uiImg1.setImageResource(R.drawable.load_ic_1);
		else
			viewHolder.uiImg1.setImageBitmap(bmap);
		viewHolder.uiTitle1.setText(info.topicTitle);
		viewHolder.uiFirst.setTag(info);
		viewHolder.uiFirst.setOnClickListener(this);
		
		
		TopicInfo info2 = null;		if(2*position +1 < mDataList.size())
			info2 = mDataList.get(2*position+1);
		if(info2 == null)
			viewHolder.uiSecond.setVisibility(View.INVISIBLE);
		else
		{
			viewHolder.uiSecond.setVisibility(View.VISIBLE);
			bmap = ImageUtil.getBitmapBanner(info2.topicImg);
			if(bmap == null)
				viewHolder.uiImg2.setImageResource(R.drawable.load_ic_1);
			else
				viewHolder.uiImg2.setImageBitmap(bmap);
			viewHolder.uiTitle2.setText(info2.topicTitle);
			viewHolder.uiSecond.setTag(info2);
			viewHolder.uiSecond.setOnClickListener(this);
		}
		
		return convertView;
	}

	class ViewHolder
	{
		ImageView uiImg1;
		ImageView uiImg2;
		TextView uiTitle1;
		TextView uiTitle2;
		View uiFirst;
		View uiSecond;
	}
	
	public interface OnRanksClick{
		public void onClick(TopicInfo info);
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		TopicInfo info = (TopicInfo) v.getTag();
		switch(v.getId())
		{
			//点击第一个排行榜
			case R.id.rl_ranks_first:
			case R.id.rl_ranks_second:
			{
				if(mCallBack != null)
					mCallBack.onClick(info);
				break;
			}
		}
	}
}
