package assistant.fragment.pager;

import tiange.ktv.assistant.R;
import android.os.Bundle;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.AbsListView.OnScrollListener;
import android.widget.ListView;
import assistant.callback.OnOptionClickListener;
import assistant.fragment.adapter.ChatDisplayAdapter;
import assistant.fragment.adapter.ChatDisplayAdapter.OnUserClickListener;
import assistant.fragment.adapter.ChatDisplayUserAdapter;
import assistant.fragment.home.ChatFragment;
import assistant.global.AppStatus;
import assistant.manager.NetUtilManager.INetUtilListener;

import com.jni.netutil.XGMessage;
//聊天显示 - adapter
public class ChatDisplayFragment extends Fragment implements INetUtilListener{

	final static String Tag = "ChatDisplayFragment";
	public final static String FRAGMENT_TYPE_EXT = "assistant.fragment.ChatDisplayFragment.type";
	
	public final static int ChatTypePublic = 0;          //公聊
	public final static int ChatTypePrivate = 1;         //私聊
	public final static int ChatTypeSystem = 2;          //系统消息
	public final static int ChatTypeArround = 3;         //身边
	
	int m_type = ChatDisplayFragment.ChatTypePublic;     //默认聊天类型
	
	ListView m_listViewDisplay;                          //聊天消息显示列表
	View m_viewTip;                                      //信息显示
	
	ChatDisplayAdapter chatInfoAdapter = null;           //聊天显示适配器
	ChatDisplayUserAdapter chatUserAdapter = null;       //“身边” 用户信息
	
	
	OnListViewTouchListener m_onListViewTouchListener = null;   //聊天区域点击响应信息
	OnUserClickListener m_onUserClickListener = null;           //身边用户信息
	OnOptionClickListener m_optionClicListener = null;          //
	
	public boolean mIsMoreInfo = false;
	boolean mIsBottom = true;
	boolean mIsTouchDown = false;
	
	public ChatDisplayFragment(){
		
	}
	
	//从创建的地方存入 FRAGMENT_TYPE_EXT对应的type
	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		m_type = this.getArguments().getInt(FRAGMENT_TYPE_EXT);
	}

	public interface OnListViewTouchListener{
		public void onListViewTouchDown();
		public void onInfoNew();
		public void onInfoRead();
	}
	
	//显示ChatDisplayFragment调用
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		Log.i(Tag,"onCreateView");
		View root = inflater.inflate(R.layout.fragment_chat_display, null);
		
		m_listViewDisplay = (ListView)root.findViewById(R.id.listViewChat);
		m_viewTip = root.findViewById(R.id.textViewTip);
		
		//m_listViewDisplay.setOverScrollMode();
		//这个事件主要是点击信息内容区域，键盘退出
		m_listViewDisplay.setOnTouchListener(new View.OnTouchListener() {
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				if(event.getAction() == MotionEvent.ACTION_DOWN)
				{
					if(m_onListViewTouchListener != null)
						m_onListViewTouchListener.onListViewTouchDown();
					mIsTouchDown = true;
				}
				else if(event.getAction() == MotionEvent.ACTION_UP)
				{
					mIsTouchDown = false;
				}
				return false;
			}
		});
		m_listViewDisplay.setOnScrollListener(new OnScrollListener(){

			@Override
			public void onScroll(AbsListView view, int firstVisibleItem,
					int visibleItemCount, int totalItemCount) {
				// TODO Auto-generated method stub
				if(firstVisibleItem+visibleItemCount == totalItemCount)
					mIsBottom = true;
				else
					mIsBottom = false;
				
				if(mIsTouchDown && m_onListViewTouchListener!=null)
					m_onListViewTouchListener.onInfoRead();
			}

			@Override
			public void onScrollStateChanged(AbsListView view, int scrollState) {
				// TODO Auto-generated method stub
				
			}
			
		});
		
		//根据不同的页面添加不同的适配器Adapter
		switch(m_type)
		{
		//公聊
		case ChatDisplayFragment.ChatTypePublic:
		{
			chatInfoAdapter = new ChatDisplayAdapter(getActivity(),ChatDisplayFragment.ChatTypePublic,m_onUserClickListener);
			m_listViewDisplay.setAdapter(chatInfoAdapter);
			setEmptyTip(chatInfoAdapter.isEmpty());
			setLast();
			break;
		}
		//私聊
		case ChatDisplayFragment.ChatTypePrivate:
		{
			chatInfoAdapter = new ChatDisplayAdapter(getActivity(),ChatDisplayFragment.ChatTypePrivate,m_onUserClickListener);
			m_listViewDisplay.setAdapter(chatInfoAdapter);
			setEmptyTip(chatInfoAdapter.isEmpty());
			setLast();
			break;
		}
		//系统
		case ChatDisplayFragment.ChatTypeSystem:
		{
			chatInfoAdapter = new ChatDisplayAdapter(getActivity(),ChatDisplayFragment.ChatTypeSystem,null);
			m_listViewDisplay.setAdapter(chatInfoAdapter);
			setEmptyTip(chatInfoAdapter.isEmpty());
			setLast();
			break;
		}
		//身边
		case ChatDisplayFragment.ChatTypeArround:
		{
			chatUserAdapter = new ChatDisplayUserAdapter(getActivity(),m_optionClicListener);
			m_listViewDisplay.setAdapter(chatUserAdapter);
			break;
		}
		}
		
		AppStatus.s_NetUtilMgr.registerListener(this);
		
		return root;
	}
	
	//销毁
	@Override
	public void onDestroyView() {
		// TODO Auto-generated method stub
		Log.i(Tag,"onDestroyView");
		AppStatus.s_NetUtilMgr.unregisterListener(this);
		super.onDestroyView();
		
	}
	
	//消息放在最后
	public void setLast(){
		if(m_listViewDisplay != null && m_listViewDisplay.getCount()!=0)
			m_listViewDisplay.setSelection(m_listViewDisplay.getCount()-1);
	}
	//点击聊天区域响应事件回调
	public void setOnLVTouchDownListener(OnListViewTouchListener listener){
		m_onListViewTouchListener = listener;
	}
	//
	public void setOnUserClickListener(OnUserClickListener onUserClickListener) {
		m_onUserClickListener = onUserClickListener;
	}

	public void setOptionClicListener(OnOptionClickListener optionClicListener) {
		m_optionClicListener = optionClicListener;
	}
	
	public void notifyChanged(){
		chatInfoAdapter.notifyDataSetChanged();
	}

	public void scrollBottomWithUpdate(boolean update,boolean force){
		
		if(update)
		{
			chatInfoAdapter.notifyDataSetChanged();
			setEmptyTip(chatInfoAdapter.isEmpty());
		}
		
		if(ChatFragment.sCanAutoScroll && mIsMoreInfo)
		{
			scrollBottom(force);
		}
		else
		{
			if(mIsMoreInfo && m_onListViewTouchListener != null)
				m_onListViewTouchListener.onInfoNew();
		}
	}
	
	public void scrollBottom(){
		scrollBottom(false);
	}
	
	public void scrollBottom(boolean force){
		if(!force && !mIsBottom)
		{
			if(m_onListViewTouchListener != null)
				m_onListViewTouchListener.onInfoNew();
			return ;
		}
		
		if(force)
		{
			if(m_onListViewTouchListener != null)
				m_onListViewTouchListener.onInfoRead();
		}
		
		if(!chatInfoAdapter.isEmpty())
			m_listViewDisplay.setSelection(chatInfoAdapter.getCount()-1);
		mIsMoreInfo = false;
	}
	//创建一个新的实例，做一些初始化操作
	public static Fragment newInstance(int type,OnListViewTouchListener listViewTouchListener
			,OnUserClickListener userClickListener,OnOptionClickListener optionClicListener){
		
		if(type < 0 || type >3)
			return null;
		
		ChatDisplayFragment fragment = new ChatDisplayFragment();
		fragment.setOnLVTouchDownListener(listViewTouchListener);
		fragment.setOnUserClickListener(userClickListener);
		fragment.setOptionClicListener(optionClicListener);
		Bundle args = new Bundle();
		args.putInt(ChatDisplayFragment.FRAGMENT_TYPE_EXT, type);
		fragment.setArguments(args);
		return fragment;
	}

	@Override
	public void dealWithMsg(Message msg) {
		// TODO Auto-generated method stub
		switch(msg.what)
		{
		//更新在线用户、更新用户头像,更新用户昵称
		case XGMessage.XGMSG_USER_LOGIN://更新在线状态
		case XGMessage.XGMSG_USER_LOGOUT:
		case XGMessage.XGMSG_USERINFO_NOTIFY:
		case XGMessage.XGMSG_USERPIC_UPDATE:
		case XGMessage.XGMSG_USERNAME_UPDATE:
			if(chatUserAdapter != null)
				chatUserAdapter.notifyDataSetChanged();
			break;
		//更新显示聊天广播信息
		case XGMessage.XGMSG_CHAT_NOTIFY:
		case XGMessage.XGMSG_GIFT_NOTIFY:
		case XGMessage.XGMSG_HORN_NOTIFY:
		case XGMessage.XGMSG_REWARD_NOTIFY:
		case XGMessage.XGMSG_USERINFO_FORHORN:
		case XGMessage.XGMSG_HONGBAO_RECV:
		case XGMessage.XGMSG_HONGBAO_ROOM:
		{
			//如果是红包消息，则强制下滑到底部，给用户抢红包。
			boolean isForce = (msg.what == XGMessage.XGMSG_HONGBAO_RECV) ||
					(msg.what == XGMessage.XGMSG_HONGBAO_ROOM);
			mIsMoreInfo = true;
			//刷新内容，如果不为空，并且能自动滚动，则滚动到底部
			scrollBottomWithUpdate(true,isForce);
			break;
		}
		}
	}

	//如果没有信息，给出没有信息的提示
	private void setEmptyTip(boolean empty) {
		if(empty)
			m_viewTip.setVisibility(View.VISIBLE);
		else
			m_viewTip.setVisibility(View.GONE);
	}
}
