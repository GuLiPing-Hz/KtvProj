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
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView.LayoutParams;
import android.widget.BaseExpandableListAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.activity.Home;
import assistant.callback.OnOptionClickListener;
import assistant.global.AppStatus;
import assistant.util.ImageUtil;
import assistant.util.ShowUtil;
import assistant.view.RoundedImageView;

import com.jni.netutil.LocalData_UserInfo;

public class OnlineAdapter extends BaseExpandableListAdapter implements View.OnClickListener
{

	public final static int MessagePhoto = 1;
	
	Context m_context;
	List<LocalData_UserInfo> m_users = null;//保存所有的用户信息
	
	//在线天使
	List<LocalData_UserInfo> mAngles = new ArrayList<LocalData_UserInfo>();
	//本包厢用户
	List<LocalData_UserInfo> mRoomUsers = new ArrayList<LocalData_UserInfo>();
	
	//一下列数据放一起
	//身边主播
	List<LocalData_UserInfo> mSideAngles = new ArrayList<LocalData_UserInfo>();
	//身边用户
	List<LocalData_UserInfo> mSideUsers = new ArrayList<LocalData_UserInfo>();
	
	class DisplayUserList{
		//boolean expand = true;
		//boolean islast = false;
		int type = 1;//1 listview ; 2 separater
		String section;
		List<LocalData_UserInfo> list;
	}
	//结构化数据结构
	public List<DisplayUserList> mDataLists = new ArrayList<DisplayUserList>();
	
	//默认展开第一个
	int m_groupPos = -1;
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
				OnlineAdapter.this.notifyDataSetChangedEx();
				break;
			}
		}
	};
	
	public OnlineAdapter(Context context,OnOptionClickListener listener) {
		// TODO Auto-generated constructor stub
		m_context = context;
		m_onOptionListener = listener;
		updateUsers();
	}
	
	public void updateUsers()
	{
		m_users = AppStatus.s_NetUtilMgr.getUserInfoList();
		mAngles.clear();
		mRoomUsers.clear();
		mSideAngles.clear();
		mSideUsers.clear();
		
		List<String> photoUrls = new ArrayList<String>();
		
		//确保自己在第一个
		if(AppStatus.s_userMy.online_type == 1)
			mRoomUsers.add(AppStatus.s_userMy);
		else if(AppStatus.s_userMy.online_type == 3)
			mSideUsers.add(AppStatus.s_userMy);
		
		for(int i=0;i<m_users.size();i++)
		{
			LocalData_UserInfo info = m_users.get(i);
			Bitmap bitmap = ImageUtil.getBitmapPhoto(info.headurl);
			if(bitmap == null)
				photoUrls.add(info.headurl);
			
			if(info.online_type == 0)
				this.mAngles.add(info);
			else if(info.online_type == 1)
				this.mRoomUsers.add(info);
			else if(info.online_type == 2)
				this.mSideAngles.add(info);
			else if(info.online_type == 3)
				this.mSideUsers.add(info);
		}
		
		Bitmap bitmap = ImageUtil.getBitmapPhoto(AppStatus.s_userMy.headurl);
		if(bitmap == null)
			photoUrls.add(AppStatus.s_userMy.headurl);
		//尝试下载本地没有保存的头像图片
		ImageUtil.showPhotoImage(m_context, photoHandler, photoUrls, MessagePhoto);//
		
		mDataLists.clear();
		if(mAngles.size() > 0)//如果有在线天使
		{
			DisplayUserList data = new DisplayUserList();
			data.section = "当前包厢娱乐天使";
			data.list = mAngles;
			mDataLists.add(data);
		}
		if(mRoomUsers.size() > 0)//如果有本包厢用户
		{
			DisplayUserList data = new DisplayUserList();
			data.section = "当前包厢成员";
			data.list = mRoomUsers;
			
			if(mDataLists.size() > 0)//已经有数据加入分隔
			{
				DisplayUserList separater = new DisplayUserList();
				separater.type = 2;
				separater.list = new ArrayList<LocalData_UserInfo>();
				mDataLists.add(separater);
			}
			mDataLists.add(data);
		}
		List<LocalData_UserInfo> tempList = new ArrayList<LocalData_UserInfo>();
		tempList.addAll(mSideAngles);
		tempList.addAll(mSideUsers);
		if(tempList.size() > 0)//如果有身边用户
		{
			DisplayUserList data = new DisplayUserList();
			data.section = "身边用户";
			data.list = tempList;
			
			if(mDataLists.size() > 0)//加入分隔
			{
				DisplayUserList separater = new DisplayUserList();
				separater.type = 2;
				separater.list = new ArrayList<LocalData_UserInfo>();
				mDataLists.add(separater);
			}
			mDataLists.add(data);
		}
	}
	
	//只是更新View
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
	public Object getChild(int groupPosition, int childPosition) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public long getChildId(int groupPosition, int childPosition) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public View getChildView(int groupPosition, int childPosition,
			boolean isLastChild, View convertView, ViewGroup parent) {
		ViewHolderChild viewHolder = null;
		
		List<LocalData_UserInfo> list = mDataLists.get(groupPosition).list;
			
		if(convertView == null)
		{
			viewHolder = new ViewHolderChild();
			convertView = LayoutInflater.from(m_context).inflate(R.layout.adapter_chat_user, null);
			viewHolder.photo = (RoundedImageView) convertView.findViewById(R.id.rimgViewUserPhoto);
			viewHolder.name = (TextView) convertView.findViewById(R.id.tViewChatUserName);
			viewHolder.id = (TextView) convertView.findViewById(R.id.tViewChatUserId);
			viewHolder.angle = convertView.findViewById(R.id.img_angleflag);
			viewHolder.flag = (ImageView) convertView.findViewById(R.id.listitem_flag);
			
			viewHolder.option = convertView.findViewById(R.id.ll_option);
			viewHolder.gift = convertView.findViewById(R.id.tViewChatUserGift);
			viewHolder.talk = convertView.findViewById(R.id.tViewChatUserChat);
			viewHolder.hongbao = convertView.findViewById(R.id.tViewChatUserHongB);
			
			viewHolder.group = groupPosition;
			viewHolder.pos = childPosition;
			convertView.setTag(viewHolder);
		}
		else
			viewHolder = (ViewHolderChild) convertView.getTag();
		
		LocalData_UserInfo info = list.get(childPosition);
		if(info == null)
			return convertView;
		
		//头像
		Bitmap photo = ImageUtil.getBitmapPhoto(info.headurl);
		if(photo != null)
			viewHolder.photo.setImageBitmap(photo);
		else
			viewHolder.photo.setImageResource(R.drawable.ic_user);
		
		if(info.online_type == 0 || info.online_type == 2)
			viewHolder.angle.setVisibility(View.VISIBLE);
		else
			viewHolder.angle.setVisibility(View.GONE);
		
		Drawable right = m_context.getResources().getDrawable(
				info.m_sex.equals("1")?R.drawable.icon_sex2_man:R.drawable.icon_sex2_woman);
		right.setBounds(0, 0, right.getMinimumWidth(), right.getMinimumHeight());
		viewHolder.name.setCompoundDrawables(null, null, right, null);
		viewHolder.name.setText(info.m_name);
		
		viewHolder.id.setText("ID: "+info.m_idx);

		if(groupPosition == m_groupPos && childPosition == m_optionPos)
		{
			viewHolder.flag.setImageResource(R.drawable.btn_arrow_top);
			viewHolder.option.setVisibility(View.VISIBLE);
			viewHolder.gift.setTag(info);
			viewHolder.gift.setOnClickListener(this);
			viewHolder.talk.setTag(info);
			viewHolder.talk.setOnClickListener(this);
			viewHolder.hongbao.setTag(info);
			viewHolder.hongbao.setOnClickListener(this);
		}
		else
		{
			viewHolder.option.setVisibility(View.GONE);
			viewHolder.flag.setImageResource(R.drawable.btn_arrow_bottom);
		}
		
		convertView.setOnClickListener(convertViewListener);
		
		return convertView;
	}

	//监听操作栏是否展开
	View.OnClickListener convertViewListener = new View.OnClickListener() {
		
		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			ViewHolderChild viewHolder = (ViewHolderChild) v.getTag();
			
			//如果点击当前项
			if(m_groupPos == viewHolder.group && m_optionPos == viewHolder.pos)
			{
				m_groupPos = -1;
				m_optionPos = -1;
			}
			else
			{
				m_groupPos = viewHolder.group;
				m_optionPos = viewHolder.pos;
			}
			OnlineAdapter.this.notifyDataSetChangedEx();
		}
	};
	
	@Override
	public int getChildrenCount(int groupPosition) {
		// TODO Auto-generated method stub
		return mDataLists.get(groupPosition).list.size();
	}

	@Override
	public Object getGroup(int groupPosition) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public int getGroupCount() {
		// TODO Auto-generated method stub
		return mDataLists.size();
	}

	@Override
	public long getGroupId(int groupPosition) {
		// TODO Auto-generated method stub
		return groupPosition;
	}

	@Override
	public View getGroupView(int groupPosition, boolean isExpanded,
			View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		ViewHolderSection viewHolder = null;
		
		DisplayUserList info = mDataLists.get(groupPosition);
		do
		{
			if(convertView != null)
			{
				viewHolder = (ViewHolderSection) convertView.getTag();
				
				//如果可以重用
				if(viewHolder!=null && viewHolder.type == info.type)
					break;
			}
			
			viewHolder = new ViewHolderSection();
			viewHolder.type = info.type;
			if(info.type == 1)
			{
				convertView = LayoutInflater.from(m_context).inflate(R.layout.adapter_chat_user_section, null);
				viewHolder.img = (ImageView) convertView.findViewById(R.id.img_flag);
				viewHolder.section = (TextView) convertView.findViewById(R.id.tv_section);
				viewHolder.number = (TextView) convertView.findViewById(R.id.tv_number);
				convertView.setTag(viewHolder);
			}
			else if(info.type == 2)
			{
				convertView = new View(m_context);
				convertView.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT,20));
				convertView.setBackgroundResource(R.color.bg_gift);
				convertView.setTag(viewHolder);
				return convertView;
			}
			else
				return null;
		}while(false);
		
		if(info.type == 1)//1的时候需要设置
		{
			if(isExpanded)
				viewHolder.img.setImageResource(R.drawable.phone_show_online_user_pushdown_1);
			else
				viewHolder.img.setImageResource(R.drawable.phone_show_online_user_pushdown_0);
			viewHolder.section.setText(info.section);
			viewHolder.number.setText(""+info.list.size()+"人");
		}
		
		return convertView;
	}

	@Override
	public boolean hasStableIds() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean isChildSelectable(int groupPosition, int childPosition) {
		// TODO Auto-generated method stub
		return false;
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
		case R.id.tViewChatUserHongB:
			if(info.m_idx == (int)AppStatus.userIdx)
				ShowUtil.showToast(m_context, R.string.chat_hongbao_my);
			else
			{
				if(m_onOptionListener != null)
					m_onOptionListener.onHongBao(info);
			}
			break;
		}
	}
	
	static class ViewHolderChild{
		RoundedImageView photo = null;
		TextView name = null;
		TextView id = null;
		ImageView flag;
		View angle;
		
		View option = null;
		View gift = null;
		View talk = null;
		View hongbao = null;
		
		int group = -1;
		int pos = -1;
	}
	
	static class ViewHolderSection{
		ImageView img;
		TextView section;
		TextView number;
		
		int type = 1;//同DisplayUserList中的type
	}
}
