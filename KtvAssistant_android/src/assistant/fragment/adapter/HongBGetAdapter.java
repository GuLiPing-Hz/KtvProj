package assistant.fragment.adapter;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView.LayoutParams;
import android.widget.BaseAdapter;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.entity.KtvHongBaoInfo;

public class HongBGetAdapter extends BaseAdapter{
	public static final int HongBaoGet = 0;
	public static final int HongBaoRecord = 1;
	
	int mType = HongBaoGet;
	Context mContext;
	List<KtvHongBaoInfo> mDataList;
	
	@SuppressLint("UseSparseArrays")
	Map<Integer,KtvHongBaoData> mMapData = new LinkedHashMap<Integer,KtvHongBaoData>();
	int mNumber;
	
	OnGetHBListener mListener;
	
	class KtvHongBaoData{
		boolean isLast = false;//是否是最后一个
		boolean isExpand = true;//默认展开
		int hbnClass;
		String hbClass;
		List<KtvHongBaoInfo> listData = new ArrayList<KtvHongBaoInfo>();
		
		int getDataLength(){
			if(isExpand)
				return listData.size();
			return 0;
		}
		
		int getCount(){
			return listData.size() == 0 ? 0 : getDataLength()+1+(isLast?0:1);
		}
	}
	
	class DisplayHongBData{
		int type;//1 头，2 中，3 尾巴也就是间隔
		boolean isExpand;
		String hbClass;
		int hbnClass;
		KtvHongBaoInfo info;
	}
	
	static class ViewHolder{
		int type;//1 头，2 中，3 尾巴也就是间隔
		
		//section
		TextView uiTitle;
		View uiTip;
		ImageView uiExpand;
		//item
		TextView uiMission;
		TextView uiTime;
		TextView uiGold;
		ImageButton uiBtn;
		
		//data
		KtvHongBaoData data;
	}
	
	public HongBGetAdapter(Context context,OnGetHBListener listener,int type,List<KtvHongBaoInfo>list){
		mContext = context;
		mListener = listener;
		mType = type;
		mDataList = list;
		
		initDataList();
	}
	
	void initDataList(){
		mMapData.clear();
		
		KtvHongBaoData data = null;
		for(KtvHongBaoInfo info : mDataList)
		{
			if(mMapData.containsKey(info.hbnclass))//检查是否有这个map
			{
				data = mMapData.get(info.hbnclass);
				data.listData.add(info);
			}
			else
			{
				data = new KtvHongBaoData();
				data.hbnClass = info.hbnclass;
				data.hbClass = info.hbclass;
				data.listData.add(info);
				mMapData.put(info.hbnclass, data);
			}
		}
		
		int lastType = -1;
		Iterator<Integer> it = mMapData.keySet().iterator();
		while(it.hasNext())
			lastType = it.next();
		
		if(lastType != -1)
			mMapData.get(lastType).isLast = true;
		
		mNumber = 0;
		for(KtvHongBaoData forData : mMapData.values())
		{
			mNumber += forData.getCount();
		}
	}
	
	//不需要重新加载全部数据，只需要执行view的重新加载
	void notifyDataSetChangedEx(){
		
		mNumber = 0;
		for(KtvHongBaoData forData : mMapData.values())
		{
			mNumber += forData.getCount();
		}
		
		super.notifyDataSetChanged();
	}
	@Override
	public void notifyDataSetChanged() {
		// TODO Auto-generated method stub
		initDataList();
		super.notifyDataSetChanged();
	}

	DisplayHongBData getCollectDisplay(int position){
		
		DisplayHongBData result = new DisplayHongBData();
		for(KtvHongBaoData data : mMapData.values())
		{
			if(position == 0)
			{
				result.type = 1;
				result.hbClass = data.hbClass;
				result.info = null;
				result.isExpand = data.isExpand;
				result.hbnClass = data.hbnClass;
				break;
			}
			else if(position <= data.getDataLength())
			{
				result.type = 2;
				result.hbClass = data.hbClass;
				result.info = data.listData.get(position-1);
				break;
			}
			else if(position < data.getCount())
			{
				result.type = 3;
				result.hbClass = data.hbClass;
				result.info = null;
				break;
			}
			else
				position -= data.getCount();
		}
		
		return result;
	}
	
	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return mNumber;
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
		
		DisplayHongBData data = getCollectDisplay(position);
		ViewHolder viewHolder = null;
		
		do
		{
			if(convertView != null)
			{
				viewHolder = (ViewHolder) convertView.getTag();
				if(data.type == viewHolder.type)//如果符合，则重用该View
					break;
			}
			
			viewHolder = new ViewHolder();
			viewHolder.type = data.type;
			//创建View
			switch(data.type)
			{
				case 1:
				{
					convertView = LayoutInflater.from(mContext).inflate(R.layout.item_hongbao_section, null);
					viewHolder.uiTitle = (TextView) convertView.findViewById(R.id.tv_hongbao_class);
					viewHolder.uiTip = convertView.findViewById(R.id.tv_section_tip);
					viewHolder.uiExpand = (ImageView) convertView.findViewById(R.id.iv_section_expand);
					break;
				}
				case 2:
				{
					if(mType == HongBaoGet)//红包领取界面
					{
						convertView = LayoutInflater.from(mContext).inflate(R.layout.item_hongbao_get, null);
						viewHolder.uiMission = (TextView) convertView.findViewById(R.id.tv_hongbao_mission);
						viewHolder.uiTime = (TextView) convertView.findViewById(R.id.tv_hongbao_time);
						viewHolder.uiBtn = (ImageButton) convertView.findViewById(R.id.btn_hongbao_do);
					}
					else//红包记录界面
					{
						convertView = LayoutInflater.from(mContext).inflate(R.layout.item_hongbao_record, null);
						viewHolder.uiMission = (TextView) convertView.findViewById(R.id.tv_hbrecord_mission);
						viewHolder.uiTime = (TextView) convertView.findViewById(R.id.tv_hbrecord_time);
						viewHolder.uiGold =  (TextView) convertView.findViewById(R.id.tv_hbrecord_gold);
					}
					break;
				}
				case 3:
				{
					convertView = new View(mContext);
					convertView.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT,20));
					convertView.setBackgroundResource(R.color.transparent);
					break;
				}
			}
			
			convertView.setTag(viewHolder);
		}while(false);
		
		
		//设置View
		switch(data.type)
		{
			case 1:
			{
				if(data.isExpand)
					viewHolder.uiExpand.setImageResource(R.drawable.btn_arrow_top);
				else
					viewHolder.uiExpand.setImageResource(R.drawable.btn_arrow_bottom);
				if(data.hbnClass == 1)
					viewHolder.uiTip.setVisibility(View.VISIBLE);
				else
					viewHolder.uiTip.setVisibility(View.GONE);
				viewHolder.uiTitle.setText(data.hbClass);
				viewHolder.data = mMapData.get(data.hbnClass);
				
				convertView.setOnClickListener(new View.OnClickListener() {
					
					@Override
					public void onClick(View v) {
						// TODO Auto-generated method stub
						ViewHolder vh = (ViewHolder) v.getTag();
						KtvHongBaoData info = vh.data;
						info.isExpand = !info.isExpand;
						HongBGetAdapter.this.notifyDataSetChangedEx();
					}
				});
				break;
			}
			case 2:
			{
				if(data.info.hbnclass == 2)
					viewHolder.uiMission.setText("来自:"+data.info.hbmission);
		        else
		        	viewHolder.uiMission.setText(data.info.hbmission);
				
				viewHolder.uiTime.setText(data.info.hbtime);
				
				if(mType == HongBaoGet)
				{
					switch (data.info.hbstatus) {
			            case 0:
			            	viewHolder.uiTime.setVisibility(View.GONE);
			            	viewHolder.uiBtn.setImageResource(R.drawable.unfinished_btn_0);
			            	viewHolder.uiBtn.setEnabled(false);
			                break;
			            case 1:
			            	viewHolder.uiTime.setVisibility(View.VISIBLE);
			            	viewHolder.uiBtn.setImageResource(R.drawable.btn_hongbao_get);
			            	viewHolder.uiBtn.setTag(data.info);
			            	viewHolder.uiBtn.setOnClickListener(new View.OnClickListener() {
								
								@Override
								public void onClick(View v) {
									// TODO Auto-generated method stub
									KtvHongBaoInfo info = (KtvHongBaoInfo) v.getTag();
									//弹出对话框
									if(mListener != null)
										mListener.onClick(info);
									
								}
							});
			            	viewHolder.uiBtn.setEnabled(true);
			                break;
			            case 2://这个一般不会再这个界面上显示
			            	viewHolder.uiTime.setVisibility(View.GONE);
			                break;
			        }
				}
				else
				{
					viewHolder.uiGold.setText(""+data.info.gold+"金币");
				}
		        
				break;
			}
			case 3:
			{
				//convertView = mHeadView;
				break;
			}
		}
		
		return convertView;
	}
	
	public interface OnGetHBListener{
		public void onClick(KtvHongBaoInfo info);
	}
}
