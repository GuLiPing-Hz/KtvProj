package assistant.fragment.adapter;

import tiange.ktv.assistant.R;
import android.content.Context;
import android.graphics.Bitmap;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.fragment.pager.GiftFragment;
import assistant.global.AppStatus;
import assistant.manager.GiftManager;
import assistant.util.ImageUtil;

import com.jni.netutil.GiftInfo;

public class GiftFragmentAdapter extends BaseAdapter {

	Context m_context;
	
	//指定第几页
	int m_page;
	
	GiftManager m_giftMgr;
	
	public GiftFragmentAdapter(Context context,int page){
		m_context = context;
		m_page = page;
		m_giftMgr = AppStatus.s_giftMgr;
	}
	
	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return m_giftMgr.getGiftSize(m_page);
	}

	@Override
	public Object getItem(int pos) {
		// TODO Auto-generated method stub
		return m_giftMgr.getGiftInfo(m_page, pos);
	}

	@Override
	public long getItemId(int arg0) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public View getView(int pos, View convertView, ViewGroup arg2) {
		// TODO Auto-generated method stub
		ViewHolder viewHolder = null;
		if(convertView == null)
		{
			viewHolder = new ViewHolder();
			convertView = LayoutInflater.from(m_context).inflate(R.layout.adapter_gift_single, null);
			viewHolder.image = (ImageView) convertView.findViewById(R.id.imgViewGift);
			viewHolder.name = (TextView) convertView.findViewById(R.id.textViewGiftName);
			viewHolder.price = (TextView) convertView.findViewById(R.id.textViewPrice);
			convertView.setTag(viewHolder);
		}
		else
			viewHolder = (ViewHolder) convertView.getTag();
		
		GiftInfo info = m_giftMgr.getGiftInfo(m_page,pos);
		if(info == null)
			return convertView;
		
		if(GiftFragment.sCurPage==m_page && GiftFragment.sCurSelect == pos)
			convertView.setBackgroundResource(R.color.bg_gift_item_pressed);
		else
			convertView.setBackgroundResource(R.color.transparent);
		
		Bitmap bitmap = ImageUtil.getBitmap(AppStatus.getLocalImage(info.image_name));
		if(bitmap!=null)
			viewHolder.image.setImageBitmap(bitmap);
		viewHolder.name.setText(info.name);
		viewHolder.price.setText(""+info.cash);
		
		return convertView;
	}

	static class ViewHolder{
		ImageView image;
		TextView name;
		TextView price;
	}
}
