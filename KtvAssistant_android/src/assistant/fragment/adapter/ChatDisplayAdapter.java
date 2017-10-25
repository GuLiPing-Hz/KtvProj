package assistant.fragment.adapter;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Message;
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.TextUtils;
import android.text.style.ForegroundColorSpan;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.entity.ChatDisplayInfo;
import assistant.fragment.pager.ChatDisplayFragment;
import assistant.global.AppStatus;
import assistant.util.ImageUtil;
import assistant.view.RoundedImageView;

import com.jni.netutil.ResultData_UserSampleInfo;

public class ChatDisplayAdapter extends BaseAdapter implements View.OnClickListener {

	final static String Tag = "ChatDisplayAdapter";
	public final static int MSG_PHOTO = 1;
	
	Context m_context;                          //ChatDisplayFragment Context
	
	String RoomTxt;                             //
	//ChatDisplayFragment.ChatTypePublic
	int m_type;                                 //ChatDisplayFragment的type
	//待添加下载头像
	List<ChatDisplayInfo> m_display = new ArrayList<ChatDisplayInfo>();
	OnUserClickListener m_onUserClickListener;     //点击用户图像响应函数
	@SuppressLint("HandlerLeak")
	Handler m_handler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			switch(msg.what)
			{
			case MSG_PHOTO:
				//通知使用新的头像
				ChatDisplayAdapter.this.notifyDataSetChangedEx();
				break;
			}
		}
		
	};
	
	public ChatDisplayAdapter(Context context,int type,OnUserClickListener listener) {
		// TODO Auto-generated constructor stub
		m_context = context;
		m_type = type;
		m_onUserClickListener = listener;
		catchCorrectDisplay();
		
		RoomTxt = m_context.getString(R.string.text_room);
	}
	
	String getPhoto(ChatDisplayInfo info){
		if(info.position == ChatDisplayInfo.PosCenter)
			return null;
		if(info.type==ChatDisplayInfo.ChatPublic || info.type==ChatDisplayInfo.ChatPrivate)
			return info.chat.sayphoto;
		else if(info.type == ChatDisplayInfo.ChatHorn)
			return TextUtils.isEmpty(info.horn.photo)?null:info.horn.photo;
		else if(info.type == ChatDisplayInfo.ChatGift)
			return info.gift.s_photo;
		else if(info.type == ChatDisplayInfo.ChatRedPacket)
		{
			//系统发送的红包，不需要下载头像
			if(info.hongbao.hbtype <= 4)
				return info.hongbao.photo;
			return null;
		}
		return null;
	}
	
	/*
	 * 查看这个信息是否归属本Adapter 
	 */
	boolean isBelongtoHere(ChatDisplayInfo info){
		if(info == null)
			return false;
		
		if(m_type == ChatDisplayFragment.ChatTypePublic)
		{
			//处理下发喇叭用户取得信息的时候
			if(info.type == ChatDisplayInfo.ChatHorn && info.position == ChatDisplayInfo.PosLeft)
			{
				ResultData_UserSampleInfo sampleInfo = AppStatus.s_NetUtilMgr.getUserInfoForHorn(info.tagIdx);
				if(sampleInfo != null)
				{
					info.tagName = sampleInfo.nickname;
					info.horn.photo = sampleInfo.headurl;
				}
			}
			return true;
		}
		else if(m_type == ChatDisplayFragment.ChatTypePrivate)
		{
			if(info.type == ChatDisplayInfo.ChatPrivate)
				return true;
		}
		else if(m_type == ChatDisplayFragment.ChatTypeSystem)
		{
			if(info.type == ChatDisplayInfo.ChatPublic || info.type == ChatDisplayInfo.ChatPrivate)
				return false;
			
			//处理下发喇叭用户取得信息的时候
			if(info.type == ChatDisplayInfo.ChatHorn && info.position == ChatDisplayInfo.PosLeft)
			{
				ResultData_UserSampleInfo sampleInfo = AppStatus.s_NetUtilMgr.getUserInfoForHorn(info.tagIdx);
				if(sampleInfo != null)
				{
					info.tagName = sampleInfo.nickname;
					info.horn.photo = sampleInfo.headurl;
				}
			}
			return true;
		}
		return false;
	}

	public boolean isEmpty(){
		return m_display.isEmpty();
	}
	void catchCorrectDisplay(){
		if(AppStatus.s_NetUtilMgr != null)
		{
			List<String> images = new ArrayList<String>();
			m_display.clear();
			List<ChatDisplayInfo> displayList = AppStatus.s_NetUtilMgr.getDisplayList();
			for(int i=0;i<displayList.size();i++)
			{
				ChatDisplayInfo info = displayList.get(i);
				if(isBelongtoHere(info))
					m_display.add(info);
				String photo = getPhoto(info);
				if(photo == null)
					continue;
				//查看需要下载的头像
				Bitmap bitmap = ImageUtil.getBitmapPhoto(photo);
				if(bitmap == null)
					images.add(photo);
			}
			ImageUtil.showPhotoImage(m_context, m_handler, images, MSG_PHOTO);
		}
	}
	
	public void notifyDataSetChangedEx(){
		super.notifyDataSetChanged();
	}
	@Override
	public void notifyDataSetChanged() {
		// TODO Auto-generated method stub
		catchCorrectDisplay();
		
		super.notifyDataSetChanged();
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

	private int getCorrectLayout(ChatDisplayInfo info){

		switch(info.type)
		{
		case ChatDisplayInfo.ChatPublic:
		{
			if(info.position == ChatDisplayInfo.PosLeft)
				return R.layout.adapter_chat_public_left;
			else
				return R.layout.adapter_chat_public_right;
		}
		case ChatDisplayInfo.ChatPrivate:
		{
			if(info.position == ChatDisplayInfo.PosLeft)
				return R.layout.adapter_chat_private_left;
			else
				return R.layout.adapter_chat_private_right;
		}
		case ChatDisplayInfo.ChatGift:
		{
			if(info.position == ChatDisplayInfo.PosLeft)
				return R.layout.adapter_chat_public_left;
			else
				return R.layout.adapter_chat_public_right;
		}
		case ChatDisplayInfo.ChatHorn:
		{
			if(info.position == ChatDisplayInfo.PosLeft)
				return R.layout.adapter_chat_horn_left;
			else
				return R.layout.adapter_chat_horn_right;
		}
		case ChatDisplayInfo.ChatExchange:
		case ChatDisplayInfo.ChatRank:
		case ChatDisplayInfo.ChatReword:
		{
			return R.layout.adapter_chat_system;
		}
		case ChatDisplayInfo.ChatRedPacket:
		{
			if(info.position == ChatDisplayInfo.PosLeft)
				return R.layout.adapter_red_packet_left;
			else
				return R.layout.adapter_red_packet_right;
		}
		}
		return 0;
	}
	
	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		if(m_display.size()<=position)
			return null;
		
		ViewHolder viewHolder = null;
		ChatDisplayInfo info = m_display.get(position);
		
		do
		{
			if(convertView != null)
			{
				viewHolder = (ViewHolder) convertView.getTag();
				//如果可以复用
				if(viewHolder.type == info.type && viewHolder.position == info.position)
					break;
			}
			
			//重新创建一个View
			convertView = LayoutInflater.from(m_context).inflate(getCorrectLayout(info),null);
			viewHolder = new ViewHolder();
			viewHolder.photo = (RoundedImageView) convertView.findViewById(R.id.rimgViewPhoto);
			viewHolder.address = (TextView) convertView.findViewById(R.id.textViewAddress);
			if(info.position == ChatDisplayInfo.PosLeft)
			{
				viewHolder.name = (TextView) convertView.findViewById(R.id.textViewName);
			}
			
			viewHolder.content = (TextView) convertView.findViewById(R.id.textViewContent);
		    if(info.position != ChatDisplayInfo.PosCenter)
		    {
			    if(info.type == ChatDisplayInfo.ChatGift)
				    viewHolder.gift = (ImageView) convertView.findViewById(R.id.imgViewGiftPic);
			    else if(info.type == ChatDisplayInfo.ChatRedPacket)
			    	viewHolder.redpacket = convertView.findViewById(R.id.rl_chat_hongbao);
		    }
			viewHolder.type = info.type;
			viewHolder.position = info.position;
			convertView.setTag(viewHolder);
		}while(false);
		
		//如果需要在中间提示系统信息
		if(info.position == ChatDisplayInfo.PosCenter)
		{
			String msg = "";
			if(info.type == ChatDisplayInfo.ChatRank || info.type == ChatDisplayInfo.ChatExchange)
			{
				String name = info.horn.name+"("+info.horn.from_idx+") ";//添加点击事件 聊天 to be continue
				//String room = (info.horn.room_name.equals("")?"":("  在"+info.horn.room_name+"包厢 "));//添加点击事件 进入包厢
				msg = "【系统消息】恭喜,"+name+info.horn.msg;
			}
			else
			{
				String name = info.reward.name+"("+info.reward.idx+")";//添加点击事件 聊天 to be continue
				msg = "【系统消息】恭喜,"+name+"! 在送出"+info.reward.giftname+"之后,幸运中得"+info.reward.times+"倍大奖,获得"
						+info.reward.gold+"金币,赠送幸运礼物,礼金翻倍来!";
			}
			viewHolder.content.setText(msg);
		}
		else
		{
			switch(info.type)
			{
			//如果是公聊
			case ChatDisplayInfo.ChatPublic:
			{
				//如果显示在左边的话，因为是别人发起的，需要显示名字，如果名字没取到，在NetUtilManager会增加Idx做为名字
				if(info.position == ChatDisplayInfo.PosLeft)
					viewHolder.name.setText(info.tagName);//添加点击事件 聊天 to be continue
				
				//头像
				Bitmap photo = ImageUtil.getBitmapPhoto(info.chat.sayphoto);
				if(photo != null)
					viewHolder.photo.setImageBitmap(photo);
				else
					viewHolder.photo.setImageResource(R.drawable.ic_user);
				
				//内容
				if(AppStatus.s_faceUtil != null)
					viewHolder.content.setText(AppStatus.s_faceUtil.getRichString(info.chat.content));
				else
					viewHolder.content.setText("");
				
				//包厢
				if(info.chat.roomid != 0)
				{
					viewHolder.address.setVisibility(View.VISIBLE);
					viewHolder.address.setText("—"+info.chat.room+RoomTxt);
				}
				break;
			}
			//如果是私聊
			case ChatDisplayInfo.ChatPrivate:
			{
				//名字
				if(info.position == ChatDisplayInfo.PosLeft)
					viewHolder.name.setText(info.tagName);//添加点击事件 聊天 to be continue
				
				//头像
				Bitmap photo = ImageUtil.getBitmapPhoto(info.chat.sayphoto);
				if(photo != null)
					viewHolder.photo.setImageBitmap(photo);
				else
					viewHolder.photo.setImageResource(R.drawable.ic_user);
				
				//具体内容
				if(AppStatus.s_faceUtil != null)
				{
					String toname = "@"+info.chat.toname;//可能增加点击
					viewHolder.content.setText(AppStatus.s_faceUtil.getRichString(toname+"    "+info.chat.content));
				}
				else
					viewHolder.content.setText("");
				
				//包厢信息
				if(info.chat.roomid != 0)
				{
					viewHolder.address.setVisibility(View.VISIBLE);
					viewHolder.address.setText("—"+info.chat.room+RoomTxt);
				}
				break;
			}
			//如果是喇叭
			case ChatDisplayInfo.ChatHorn:
			{
				if(info.position == ChatDisplayInfo.PosLeft)
					viewHolder.name.setText(info.tagName);//添加点击事件 聊天 to be continue
				
				//头像
				Bitmap photo = ImageUtil.getBitmapPhoto(info.horn.photo);
				if(photo != null)
					viewHolder.photo.setImageBitmap(photo);
				else
					viewHolder.photo.setImageResource(R.drawable.ic_user);
				
				//详细内容
				if(AppStatus.s_faceUtil != null)
					viewHolder.content.setText(AppStatus.s_faceUtil.getRichString(info.horn.msg));
				else
					viewHolder.content.setText("");
				
				//包厢信息
				if(!TextUtils.isEmpty(info.horn.room_name))
				{
					viewHolder.address.setVisibility(View.VISIBLE);
					viewHolder.address.setText("—"+info.horn.room_name+RoomTxt);
				}
				break;
			}
			//如果是送礼
			case ChatDisplayInfo.ChatGift:
			{
				//如果在左边显示需要显示发起者名字
				if(info.position == ChatDisplayInfo.PosLeft)
					viewHolder.name.setText(info.tagName);//添加点击事件 聊天 to be continue
				
				//头像
				Bitmap photo = ImageUtil.getBitmapPhoto(info.gift.s_photo);
				if(photo != null)
					viewHolder.photo.setImageBitmap(photo);
				else
					viewHolder.photo.setImageResource(R.drawable.ic_user);
				
				//礼物图片
				Bitmap giftBitmap = ImageUtil.getBitmap(info.gift.gifturl);
				if(giftBitmap != null)
				{
					viewHolder.gift.setVisibility(View.VISIBLE);
					viewHolder.gift.setImageBitmap(giftBitmap);
				}
				
				//显示信息
				if(AppStatus.s_faceUtil != null)
				{
					String toname = "@"+info.gift.r_name;//可能还有点击事件
					int color = m_context.getResources().getColor(R.color.chat_btn_text_pressed);
					String content = toname+"   "+info.gift.s_name
							+m_context.getString(R.string.text_send)
							+info.gift.r_name+m_context.getString(R.string.text_le)
							+info.gift.number+info.gift.giftunit+info.gift.giftname;
					SpannableStringBuilder spannable = new SpannableStringBuilder(content);
					spannable.setSpan(new ForegroundColorSpan(color), 0, toname.length(), Spannable.SPAN_INCLUSIVE_INCLUSIVE);
					viewHolder.content.setText(spannable);
				}
				else
					viewHolder.content.setText("");
				
				//地址信息
				if(info.gift.s_roomid != 0 && info.gift.r_roomid != 0)
				{
					viewHolder.address.setVisibility(View.VISIBLE);
					viewHolder.address.setText(info.gift.s_room+RoomTxt+"->"+info.gift.r_room+RoomTxt);
				}
				else if(info.gift.s_roomid != 0)
				{
					viewHolder.address.setVisibility(View.VISIBLE);
					viewHolder.address.setText(m_context.getString(R.string.text_where)
							+info.gift.s_room+m_context.getString(R.string.text_room_send));
				}
				else if(info.gift.r_roomid != 0)
				{
					viewHolder.address.setVisibility(View.VISIBLE);
					viewHolder.address.setText(m_context.getString(R.string.text_where)
							+info.gift.r_room+m_context.getString(R.string.text_room_recv));
				}
				break;
			}
			case ChatDisplayInfo.ChatRedPacket:
			{
				//左侧需要显示用户
				if(info.position == ChatDisplayInfo.PosLeft)
					viewHolder.name.setText(info.tagName);//添加点击事件 聊天 to be continue
				
				if(info.hongbao.idx == 1)
				{
					//系统发红包专用头像
					viewHolder.photo.setImageResource(R.drawable.ic_system_profile);
				}
				else
				{
					//头像
					Bitmap photo = ImageUtil.getBitmapPhoto(info.hongbao.photo);
					if(photo != null)
						viewHolder.photo.setImageBitmap(photo);
					else
						viewHolder.photo.setImageResource(R.drawable.ic_user);
				}
				
				viewHolder.content.setText(info.hongbao.remark);
				if(info.hongbao.hbtype == 0)
					viewHolder.address.setText(m_context.getResources().getString(R.string.personal_redpacket_title));
				else if(info.hongbao.hbtype == 1)
					viewHolder.address.setText(m_context.getResources().getString(R.string.room_redpacket_title));
				else if(info.hongbao.hbtype == 3)
					viewHolder.address.setText(m_context.getResources().getString(R.string.moneybag_redpacket_title));
				else if(info.hongbao.hbtype == 6)
					viewHolder.address.setText(m_context.getResources().getString(R.string.text_ktv_hb));
				else
					viewHolder.address.setText(m_context.getResources().getString(R.string.text_hb));
				
				viewHolder.redpacket.setTag(info);
				viewHolder.redpacket.setOnClickListener(new View.OnClickListener() {
					
					@Override
					public void onClick(View v) {
						// TODO Auto-generated method stub
						ChatDisplayInfo info = (ChatDisplayInfo) v.getTag();
						if(m_onUserClickListener!=null)
							m_onUserClickListener.onHongBaoClick(info);
					}
				});
				break;
			}
			}
		}
		
		if(viewHolder.name != null)
		{
			//增加添加私聊点击事件
			viewHolder.name.setTag(info);
			viewHolder.name.setOnClickListener(this);
		}
		
		if(viewHolder.photo != null)
		{
			//增加添加私聊点击事件
			viewHolder.photo.setTag(info);
			viewHolder.photo.setOnClickListener(this);
		}

		return convertView;
	}
	

	static class ViewHolder {
		//option
		public ImageView gift = null;
		public View redpacket=null;
		
		//must
		public RoundedImageView photo = null;
		public TextView name = null;
		public TextView content = null;
		public TextView address = null;
		int type;
		int position;
	}
	
	public void setOnUserClickListener(OnUserClickListener listener){
		m_onUserClickListener = listener;
	}
	
	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		ChatDisplayInfo info = (ChatDisplayInfo) arg0.getTag();
		
		//屏蔽自己，跟系统
		if(info.tagIdx == 0 || info.tagIdx == (int)AppStatus.userIdx || info.tagIdx == 1)
		{
			//ShowUtil.showToast(m_context, R.string.chat_privatetomy);
			return ;
		}
		//点击头像或昵称 发起私聊
		if(m_onUserClickListener != null)
			m_onUserClickListener.onPrivateUserClickListener(info,arg0);
	}
	public interface OnUserClickListener{
		public void onPrivateUserClickListener(ChatDisplayInfo info,View arg0);
		public void onHongBaoClick(ChatDisplayInfo info);
	}
}
