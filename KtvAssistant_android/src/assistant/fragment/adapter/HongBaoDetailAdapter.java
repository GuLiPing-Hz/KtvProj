package assistant.fragment.adapter;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.graphics.Bitmap;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import assistant.global.AppStatus;
import assistant.manager.NetUtilManager;
import assistant.util.ImageUtil;
import assistant.util.ToolUtil;
import assistant.view.RoundedImageView;

import com.jni.netutil.LocalData_UserInfo;
import com.jni.netutil.ResultData_HBDetail;

public class HongBaoDetailAdapter extends BaseAdapter {

	class DisplayHBDetail{
		ResultData_HBDetail info;
		boolean isMax = false;
		String strTime = "00:00";
	}
	
	Context mContext;
	List<DisplayHBDetail> mDataList = new ArrayList<DisplayHBDetail>();
	
	public HongBaoDetailAdapter(Context context,ResultData_HBDetail[] array,boolean needLucky){
		mContext = context;
		
		int maxGold = 0;
		int maxIndex = 0;
		for(int i=0;i<array.length;i++)
		{
			ResultData_HBDetail detail = array[i];
			DisplayHBDetail display = new DisplayHBDetail();
			display.info = detail;
			
//			Calendar cal = Calendar.getInstance();
//			cal.setTimeInMillis(detail.time*1000L);
//			Log.i("Time1", ""+cal.get(Calendar.HOUR)+":"+cal.get(Calendar.MINUTE));
			
			display.strTime = ToolUtil.formatTimestampToStr(detail.time*1000L, "HH:mm");
			
			if(needLucky)
			{
				if(maxGold <= detail.gold)
				{
					maxIndex = i;
					maxGold = detail.gold;
				}
			}
			
			mDataList.add(display);
		}
		if(needLucky)
			mDataList.get(maxIndex).isMax = true;
	}
	
	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return mDataList.size();
	}

	@Override
	public Object getItem(int position) {
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
			convertView = LayoutInflater.from(mContext).inflate(R.layout.adapter_hb_detail, null);
			viewHolder.head = (RoundedImageView) convertView.findViewById(R.id.iv_userhead);
			viewHolder.name = (TextView) convertView.findViewById(R.id.tv_name);
			viewHolder.time = (TextView) convertView.findViewById(R.id.tv_time);
			viewHolder.gold = (TextView) convertView.findViewById(R.id.tv_gold);
			viewHolder.lucky = convertView.findViewById(R.id.tv_lucky);
			
			convertView.setTag(viewHolder);
		}
		else
			viewHolder = (ViewHolder) convertView.getTag();
		
		DisplayHBDetail info = mDataList.get(position);
		if(info == null)
			return convertView;
		
		LocalData_UserInfo userInfo = AppStatus.s_NetUtilMgr.m_JniUtil.GetUserInfoFromLocal(info.info.idx);
		if(userInfo != null)
		{
			Bitmap bm = ImageUtil.getBitmapPhoto(NetUtilManager.GetHeadUrl(userInfo));
			if(bm != null)
				viewHolder.head.setImageBitmap(bm);
			else
				viewHolder.head.setImageResource(R.drawable.ic_user);
			viewHolder.name.setText(userInfo.m_name);
		}
		else
		{
			viewHolder.head.setImageResource(R.drawable.ic_user);
			viewHolder.name.setText(mContext.getResources().getString(R.string.name_unknown));
		}
		viewHolder.time.setText(info.strTime);
		viewHolder.gold.setText(""+info.info.gold+mContext.getResources().getString(R.string.pc_goldinfo));
		
		if(info.isMax)
			viewHolder.lucky.setVisibility(View.VISIBLE);
		else
			viewHolder.lucky.setVisibility(View.GONE);
		
		return convertView;
	}

	static class ViewHolder{
		RoundedImageView head;
		TextView name;
		TextView time;
		TextView gold;
		View lucky;
	}
}
