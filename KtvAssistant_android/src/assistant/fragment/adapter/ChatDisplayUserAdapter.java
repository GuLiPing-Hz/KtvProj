package assistant.fragment.adapter;

import java.util.ArrayList;
import java.util.List;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import assistant.callback.OnOptionClickListener;
import assistant.global.AppStatus;
import assistant.util.ImageUtil;
import assistant.util.ShowUtil;
import assistant.view.RoundedImageView;

import com.jni.netutil.LocalData_UserInfo;

public class ChatDisplayUserAdapter extends BaseAdapter implements View.OnClickListener{

	final static String Tag = "ChatDisplayUserAdapter";
	public final static int MessagePhoto = 1;
	
	Context m_context;
	
	List<LocalData_UserInfo> m_users = null;
	//默认展开第一个
	int m_optionPos = 0;
	
	OnOptionClickListener m_onOptionListener = null;
	
	@SuppressLint("HandlerLeak")
	Handler photoHandler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			
			switch(msg.what)
			{
			case MessagePhoto:
				//头像下载完成
				ChatDisplayUserAdapter.this.notifyDataSetChangedEx();
				break;
			}
		}
		
	};
	
	public ChatDisplayUserAdapter(Context context,OnOptionClickListener listener) {
		// TODO Auto-generated constructor stub
		m_context = context;
		m_onOptionListener = listener;
		updateUsers();
	}

	void updateUsers()
	{
		m_users = AppStatus.s_NetUtilMgr.getUserInfoList();
		List<String> photoUrls = new ArrayList<String>();
		for(int i=0;i<m_users.size();i++)
		{
			LocalData_UserInfo info = m_users.get(i);
			Bitmap bitmap = ImageUtil.getBitmapPhoto(info.headurl);
			if(bitmap == null)
				photoUrls.add(info.headurl);
		}
		//尝试下载本地没有保存的头像图片
		ImageUtil.showPhotoImage(m_context, photoHandler, photoUrls, MessagePhoto);//
	}
	
	void notifyDataSetChangedEx(){
		super.notifyDataSetChanged();
	}
	
	@Override
	public void notifyDataSetChanged() {
		//先更新用户数据，这里没有使用引用，而是直接保存了一个副本
		updateUsers();
		super.notifyDataSetChanged();
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return m_users.size()+(AppStatus.s_userMy==null?0:1);
	}

	private LocalData_UserInfo getUserInfo(int pos){
		if(pos == 0)
			return AppStatus.s_userMy;
		else
			return m_users.get(pos-1);
	}
	@Override
	public Object getItem(int arg0) {
		// TODO Auto-generated method stub
		return getUserInfo(arg0);
	}

	@Override
	public long getItemId(int arg0) {
		// TODO Auto-generated method stub
		return arg0;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		ViewHolder viewHolder = null;
		if(convertView == null)
		{
			viewHolder = new ViewHolder();
			convertView = LayoutInflater.from(m_context).inflate(R.layout.adapter_chat_user, null);
			
			viewHolder.photo = (RoundedImageView) convertView.findViewById(R.id.rimgViewUserPhoto);
			viewHolder.name = (TextView) convertView.findViewById(R.id.tViewChatUserName);
			viewHolder.id = (TextView) convertView.findViewById(R.id.tViewChatUserId);
//			viewHolder.score = (TextView) convertView.findViewById(R.id.tViewChatUserTS);
//			viewHolder.topsong = convertView.findViewById(R.id.ll_topsong);
//			viewHolder.song = (TextView) convertView.findViewById(R.id.tViewChatUserSong);
			
			viewHolder.option = convertView.findViewById(R.id.ll_option);
			viewHolder.gift = convertView.findViewById(R.id.tViewChatUserGift);
			viewHolder.talk = convertView.findViewById(R.id.tViewChatUserChat);
			viewHolder.pos = position;
			convertView.setTag(viewHolder);
		}
		else
			viewHolder = (ViewHolder) convertView.getTag();
		
		LocalData_UserInfo info = getUserInfo(position);
		if(info == null)
			return convertView;
		//头像
		Bitmap photo = ImageUtil.getBitmapPhoto(info.headurl);
		if(photo != null)
			viewHolder.photo.setImageBitmap(photo);
		else
			viewHolder.photo.setImageResource(R.drawable.ic_user);
		viewHolder.name.setText(info.m_name);
		
		Drawable right = m_context.getResources().getDrawable(
				info.m_sex.equals("1")?R.drawable.icon_sex2_man:R.drawable.icon_sex2_woman);
		right.setBounds(0, 0, right.getMinimumWidth(), right.getMinimumHeight());
		
		viewHolder.name.setCompoundDrawables(null, null, right, null);
		
		viewHolder.id.setText("ID: "+info.m_idx);
		if(TextUtils.isEmpty(info.topsongname))
		{
			viewHolder.topsong.setVisibility(View.GONE);
			viewHolder.song.setVisibility(View.GONE);
		}
		else
		{
			viewHolder.topsong.setVisibility(View.VISIBLE);
			viewHolder.song.setVisibility(View.VISIBLE);
			viewHolder.song.setText(info.topsongname);
			viewHolder.score.setText(""+info.topsongscore);
		}
		if(position == m_optionPos)
		{
			viewHolder.option.setVisibility(View.VISIBLE);
			viewHolder.gift.setTag(info);
			viewHolder.gift.setOnClickListener(this);
			viewHolder.talk.setTag(info);
			viewHolder.talk.setOnClickListener(this);
		}
		else
			viewHolder.option.setVisibility(View.GONE);
		
		convertView.setOnClickListener(convertViewListener);
		
		return convertView;
	}

	//监听操作栏是否展开
	View.OnClickListener convertViewListener = new View.OnClickListener() {
		
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			if(m_onOptionListener!=null)
				m_onOptionListener.onUserInfoClick();
			
			ViewHolder viewHolder = (ViewHolder) v.getTag();
			m_optionPos = viewHolder.pos;
			ChatDisplayUserAdapter.this.notifyDataSetChanged();
		}
	};
	
	static class ViewHolder{
		public RoundedImageView photo = null;
		public TextView name = null;
		public TextView id = null;
		public View topsong = null;
		public TextView score = null;
		public TextView song = null;
		
		public View option = null;
		public View gift = null;
		public View talk = null;
		
		public int pos = -1;
	}
	
	//点击操作栏的具体操作
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		LocalData_UserInfo info = (LocalData_UserInfo) v.getTag();
		if(info == null)
			return ;
		
		switch(v.getId())
		{
		case R.id.tViewChatUserGift:
			if(m_onOptionListener != null)
				m_onOptionListener.onGiftSendObjectClick(info);
			break;
		case R.id.tViewChatUserChat:
			if(info.m_idx == (int)AppStatus.userIdx)
				ShowUtil.showToast(m_context, R.string.chat_privatetomy);
			else
			{
				if(m_onOptionListener != null)
					m_onOptionListener.onTalkClick(info);
			}
			break;
		}
	}
}
