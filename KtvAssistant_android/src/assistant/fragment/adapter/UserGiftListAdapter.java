package assistant.fragment.adapter;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.TextView;
import assistant.activity.GiftGetFragmentActivity;
import assistant.entity.HttpGiftInfo;
import assistant.fragment.GiftGetFragment;
import assistant.util.ImageUtil;
import assistant.view.RoundCornerImageView;

public class UserGiftListAdapter extends BaseAdapter {
	// View[] itemViews;
		private Context mContext;
		List<HttpGiftInfo> giftlist = null;
		private int ktvId;
        //private static  int IO_BUFFER_SIZE;
        
        public final static int MSG_GIFT = 1;
        @SuppressLint("HandlerLeak")
        Handler m_handler = new Handler(){

			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				switch(msg.what)
				{
				case MSG_GIFT:
					UserGiftListAdapter.this.notifyDataSetChanged();
					break;
				}
			}
        	
        };
		public UserGiftListAdapter(Context context,
				ArrayList<HttpGiftInfo> giftList,int ktvId) {
			giftlist = giftList;
			this.mContext = context;
            this.ktvId=ktvId;
            if(giftlist != null)
            {
            	List<String> images = new ArrayList<String>();
                for(int i=0;i<giftlist.size();i++)
                {
                	Bitmap bitmap = ImageUtil.getBitmapGift(giftlist.get(i).simg);
                	if(bitmap == null)
                		images.add(giftlist.get(i).simg);
//                	bitmap = ImageUtil.getBitmapGift(giftlist.get(i).bimg);
//                	if(bitmap == null)
//                		images.add(giftlist.get(i).bimg);
                }
                
                ImageUtil.showGiftImage(mContext, m_handler, images, MSG_GIFT);
            }
		}

		@Override
		public int getCount() {
			return giftlist.size();
		}

		@Override
		public Object getItem(int position) {
			return giftlist.get(position);
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
						R.layout.percenter_gift_listitem, null);
				
				viewHolder.img = (RoundCornerImageView) convertView
						.findViewById(R.id.pc_giftitem_head);
				viewHolder.name = (TextView) convertView
						.findViewById(R.id.pc_giftitem_giftname);
				viewHolder.gold = (TextView) convertView
						.findViewById(R.id.pc_giftitem_gold);
				viewHolder.id = (TextView) convertView
						.findViewById(R.id.pr_gift_ktvitem_giftid);
				viewHolder.btn_duihuan= (Button) convertView
						.findViewById(R.id.pc_giftitem_duihuan);
				viewHolder.btn_duihuan.setOnClickListener(new OnClickListener(){

					@Override
					public void onClick(View Btn) {
						// TODO Auto-generated method stub
					
						String msg=  (String) Btn.getTag();
						String detailInfo[] = msg.split("&");
						int _ktvId= Integer.parseInt(detailInfo[0]) ;
						int _giftId=Integer.parseInt(detailInfo[1]) ;
						String _giftName=detailInfo[2];
						int _giftNum=Integer.parseInt(detailInfo[3]);
						int _giftGold=Integer.parseInt(detailInfo[4]);
						String _giftBimg=detailInfo[5];
						//生成新的页面调用
						// 监听 跳转
						Bundle bundle = new Bundle();  
						bundle.putInt(GiftGetFragment.EXTRA_ARGUMENT_KTVID, _ktvId);  
						bundle.putInt(GiftGetFragment.EXTRA_ARGUMENT_GIFTID, _giftId);
						bundle.putString(GiftGetFragment.EXTRA_ARGUMENT_GIFTNAME, _giftName);
						bundle.putInt(GiftGetFragment.EXTRA_ARGUMENT_GIFTNUM, _giftNum);
						bundle.putInt(GiftGetFragment.EXTRA_ARGUMENT_GIFTGOLD, _giftGold);
						bundle.putString(GiftGetFragment.EXTRA_ARGUMENT_GIFTBIMG, _giftBimg);
						Intent intent = new Intent(mContext,GiftGetFragmentActivity.class);
						intent.putExtras(bundle);
						mContext.startActivity(intent);
					}
					
				});
				convertView.setTag(viewHolder);
			} else
				viewHolder = (ViewHolder) convertView.getTag();

			HttpGiftInfo info = giftlist.get(position);
			Bitmap bitmap = ImageUtil.getBitmapGift(info.simg);
			if(bitmap != null)
				viewHolder.img.setImageBitmap(bitmap);
			else
				viewHolder.img.setImageResource(R.drawable.gift_photo_normal_s);
			viewHolder.name.setText(info.name);
			viewHolder.gold.setText(""+info.price);
			viewHolder.id.setText(""+info.id);
			viewHolder.btn_duihuan.setTag(this.ktvId+"&"+""+info.id+"&"+""+info.name+"&"+""+1+"&"+""+info.price+"&"+""+info.bimg);
			return convertView;
		}

		static class ViewHolder {
			RoundCornerImageView img;
			TextView name;
			TextView gold;
			TextView id;
			Button btn_duihuan;
		}
	}
