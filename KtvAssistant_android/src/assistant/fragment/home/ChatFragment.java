package assistant.fragment.home;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import order.show.DotView;
import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.text.Editable;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.text.style.ImageSpan;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.AnimationUtils;
import android.view.animation.TranslateAnimation;
import android.view.inputmethod.EditorInfo;
import android.widget.ArrayAdapter;
import android.widget.ImageButton;
import android.widget.PopupWindow;
import android.widget.PopupWindow.OnDismissListener;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;
import assistant.activity.HongBDetailActivity;
import assistant.activity.MoneyBagRedPacketActivity;
import assistant.activity.PhoneOnlineUserActivity;
import assistant.activity.SendHornActivity;
import assistant.activity.home.ChargeActivity;
import assistant.callback.OnOptionClickListener;
import assistant.dialog.HongBaoPersonDialog;
import assistant.dialog.HongBaoVieDialog;
import assistant.dialog.HongBaoVieDialog.OnHBCatch;
import assistant.dialog.LoadingDialog;
import assistant.entity.ChatDisplayInfo;
import assistant.fragment.BaseFragment;
import assistant.fragment.HongBaoSendFragment;
import assistant.fragment.PhoneOnlineUserFragment;
import assistant.fragment.adapter.ChatDisplayAdapter.OnUserClickListener;
import assistant.fragment.adapter.UserSpinnerAdapter;
import assistant.fragment.pager.ChatDisplayFragment;
import assistant.fragment.pager.ChatDisplayFragment.OnListViewTouchListener;
import assistant.fragment.pager.FaceFragment;
import assistant.fragment.pager.FaceFragment.OnFaceClickListener;
import assistant.fragment.pager.GiftFragment;
import assistant.fragment.pager.GiftFragment.OnGiftClickListener;
import assistant.global.AppStatus;
import assistant.global.RequestCodeDef;
import assistant.manager.DialogManager;
import assistant.manager.DialogManager.OnDialogListener;
import assistant.manager.EnterRoomManager;
import assistant.manager.FaceManager;
import assistant.manager.NetUtilManager.INetUtilListener;
import assistant.manager.OnRoomListener;
import assistant.util.FaceUtil;
import assistant.util.KeyBoardListener;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;
import assistant.view.DrawableClickEditText;
import assistant.view.KeyBoardLayout;
import assistant.view.PngAnimView;

import com.jni.netutil.GiftInfo;
import com.jni.netutil.JniMessage;
import com.jni.netutil.LocalData_UserInfo;
import com.jni.netutil.XGMessage;
import com.jni.netutil.out.HongBaoDetailInfoToUi;
import com.jni.netutil.out.HongBaoInfoToUi;

//互动聊天界面
public class ChatFragment extends BaseFragment implements View.OnClickListener,
		INetUtilListener, OnFaceClickListener, OnListViewTouchListener,
		OnGiftClickListener, OnUserClickListener, OnOptionClickListener,
		OnHBCatch{

	public final static String Tag = "ChatFragment";
	public final static Long AnimaDuration = 100L;
	
	public final static int OffsetYHBWindow = 40;
	public final static int OffsetYUserWindow = 40;
	
	public final static int DelayMillSeconds = 100;

	public static HongBaoDetailInfoToUi sHongBaoDetail;
	//public Home m_Home; // 主页
	private View m_ViewRoot; // 根视图 就是互动聊天视图

	DisplayMetrics mDm;
	
	KeyBoardLayout m_RLChat;
	TextView m_TextViewTitle; // 标题 “弹幕互动”
	TextView m_TextViewOnlineNum; // 在线人数
	ImageButton m_ImgButtonOnline; // 在线 view

	ImageButton m_ImgButtonBack; // 后退 按钮
	ImageButton m_ImgButtonHorn; // 喇叭 按钮
	ImageButton m_ImgButtonGift; // 礼物 按钮
	ImageButton m_ImgButtonFace; // 表情 按钮
	ImageButton m_ImgButtonRedPacket; // 红包
	View mUiSendChat;

	ImageButton m_BtnPrivateLetter;
	ImageButton m_BtnSendGift;
	ImageButton m_BtnRedPacket;

	Animation m_animaDisappear; // 显示喇叭的动画

	View m_viewHornTop; // 动画显示喇叭内容，由下列四个组件组成
	TextView m_textViewHornName; // 喇叭消息的发送者的名称
	TextView m_textViewHornContent; // 喇叭消息内容
	TextView m_textViewHornTime; // 喇叭时间
	TextView m_textViewHornAddress; // 喇叭地址
	
	View m_rlInteractiveChatBottom;

	FragmentPagerAdapter m_adapterFragment; // fagmentPagerAdapter
	ViewPager m_viewPagerChat; // 显示页面

	int m_curGiftType = 0; //
	int m_nextGiftType = 0;
	int m_viewSelectWidth = 0;
	int m_viewGiftSelectWidth = 0;
	long mActionSendTime = 0L;
	long mClickLastTime = 0L;
	DrawableClickEditText m_EditTextChat; // 聊天编辑框
	View mUiEnterRoom;

	boolean m_isShowFace = false; // 是否显示了表情
	boolean m_isShowSend = false; // 是否已经发送
	View m_rl_face_view;// 表情视图 //

	ViewPager m_viewPagerFace;
	DotView m_dotViewFace;

	Animation animationGift = null;

	boolean m_isShowGift = false;
	// 礼物上的内容
	View m_giftNormal;
	TextView m_textViewNormal;
	View m_viewNormal;

	View m_giftLuck;
	TextView m_textViewLuck;
	View m_viewLuck;

	View m_giftLuxury;
	TextView m_textViewLuxury;
	View m_viewLuxury;

	ViewPager m_viewPagerGift;
	DotView m_dotViewGift;
	TextView m_textViewGold;
	View m_viewTopup;
	// List<Integer> m_listNumber = null;
	Spinner m_spinnerNumber;
	List<LocalData_UserInfo> m_listUser = new ArrayList<LocalData_UserInfo>();
	@SuppressLint("UseSparseArrays")
	Map<Integer,Integer> mMapIdexIndex = new HashMap<Integer,Integer>();
	UserSpinnerAdapter m_spinnerAdatperUser = null;
	Spinner m_spinnerSendObject;
	View m_viewSendGift;
	View m_rl_gift_view;
	//私聊对象
	Map<String, Integer> m_mapNameIdx = new HashMap<String, Integer>();

	int m_normalGiftTextColor;
	int m_selectGiftTextColor;

	// NetUtil模块需要的数据
	PrivateChatTo m_privateChatTo = new PrivateChatTo();

	GiftInfo m_preGiftInfo = null;
	GiftInfo m_curGiftInfo = null;
	
	LocalData_UserInfo m_SendGiftToOnlineUser=new LocalData_UserInfo();

	PopupWindow mHBPW;
	PopupWindow m_UserOperationMenu;
	
	LoadingDialog mLoading;
	HongBaoVieDialog mVieDialog;
	
	ChatDisplayInfo mTempInfo;
	
	PngAnimView mUiMoreTip;
	
	ChatDisplayFragment mChatDisplayFragment;
	
	EnterRoomManager mEnterRoomManager;
	OnRoomListener mRoomListener = new OnRoomListener() {
		@Override
		public void EnterRoomSuccess() {
			if(!mEnterRoomManager.m_isSlientEnterRoom)
				ShowUtil.showToast(getActivity(), R.string.login_success);
		}

		@Override
		public void EnterRoomFail(String msg) {
			if (!TextUtils.isEmpty(msg))
				ShowUtil.showToast(getActivity(), msg);
			else
				ShowUtil.showToast(getActivity(), R.string.login_fail);
		}

		@Override
		public void ExitRoomSuccess() {
		}

		@Override
		public void ExitRoomFail(String msg) {
		}
	};
	
	public static boolean sCanAutoScroll = true;
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);

		if (AppStatus.s_faceUtil == null)
		{
			AppStatus.s_faceUtil = new FaceUtil(getActivity());
		}

		mEnterRoomManager = new EnterRoomManager(getActivity(),mRoomListener);
		mLoading = new LoadingDialog(getActivity(),"");
//		m_Home = (Home) this.getActivity();
//		m_Home.hidePlayControl();
//		m_Home.showMiddle(false);
		//switchSoftInputMode(false);
	}

	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		//m_Home.showMiddle(true);
		//switchSoftInputMode(true);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		m_ViewRoot = inflater.inflate(R.layout.ac_chat, null);

		Display display = getActivity().getWindowManager().getDefaultDisplay();
		mDm = new DisplayMetrics();
		display.getMetrics(mDm);
		
		findView(m_ViewRoot);
		setListener();
		initView();
		// 注册到网络协议监听
		AppStatus.s_NetUtilMgr.registerListener(this);
		return m_ViewRoot;
	}

	@Override
	public void onDestroyView() {
		// TODO Auto-generated method stub
		AppStatus.s_NetUtilMgr.unregisterListener(this);

		// 解除屏蔽表情滑动
//		m_Home.removeIgnoredAboveView(m_viewPagerFace);
//		// 解除屏蔽聊天内容滑动
//		m_Home.removeIgnoredAboveView(m_viewPagerChat);
//		// 解除屏蔽礼物滑动
//		m_Home.removeIgnoredAboveView(m_viewPagerGift);
//
//		m_Home.clearFragmentInMem();

		super.onDestroyView();
	}

	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		super.onResume();

		if (AppStatus.s_isNeedToTopup) {
			//m_Home.switchMenu(HomeMenuItemManager.MENU_ACCOUNT_RECHARGE);
			
			AppStatus.s_isNeedToTopup = false;
		}
	}
	
	void findView(View v) {
		// find resource
		
		m_normalGiftTextColor = getActivity().getResources().getColor(
				R.color.chat_user_id);
		m_selectGiftTextColor = getActivity().getResources().getColor(
				R.color.chat_btn_text_pressed);
		
		mUiEnterRoom = v.findViewById(R.id.btn_enterroom);
		m_rlInteractiveChatBottom=v.findViewById(R.id.rl_interactive_chat_bottom);
		m_viewHornTop = v.findViewById(R.id.rl_horn_top);
		m_textViewHornName = (TextView) v.findViewById(R.id.textViewHornName);
		m_textViewHornContent = (TextView) v
				.findViewById(R.id.textViewHornContent);
		m_textViewHornTime = (TextView) v.findViewById(R.id.textViewHornTime);
		m_textViewHornAddress = (TextView) v
				.findViewById(R.id.textViewHornAddress);
		m_animaDisappear = AnimationUtils.loadAnimation(getActivity(),
				R.anim.disappear);

		mUiMoreTip = (PngAnimView) v.findViewById(R.id.pa_more_tip);
		List<Integer> listMore = new ArrayList<Integer>();
		listMore.add(R.drawable.instruction_ic_0);
		listMore.add(R.drawable.instruction_ic_1);
		listMore.add(R.drawable.instruction_ic_2);
		listMore.add(R.drawable.instruction_ic_3);
		mUiMoreTip.initPngList(listMore, 100);
		//聊天主要UI
		m_RLChat = (KeyBoardLayout)v.findViewById(R.id.rl_chat);
		// 界面Title
		m_TextViewTitle = (TextView) v.findViewById(R.id.tv_title);
		// 在线用户数量包含自己
		m_TextViewOnlineNum = (TextView) v.findViewById(R.id.tv_online);
		m_ImgButtonOnline = (ImageButton) v.findViewById(R.id.tv_onlineview);
		// 回退
		m_ImgButtonBack = (ImageButton) v.findViewById(R.id.iv_back);
		// 喇叭
		m_ImgButtonHorn = (ImageButton) v.findViewById(R.id.imgBtnHorn);
		mUiSendChat = v.findViewById(R.id.ibtn_chat_send);

		// 聊天底部的三个初始化控件
		m_ImgButtonFace = (ImageButton) v.findViewById(R.id.ibtn_chat_face);
		m_ImgButtonGift = (ImageButton) v.findViewById(R.id.ibtn_chat_gift);
		m_EditTextChat = (DrawableClickEditText) v.findViewById(R.id.et_chat);
		m_ImgButtonRedPacket = (ImageButton) v.findViewById(R.id.ibtn_small_red_packet);
		
		// 聊天内容显示Pager
		m_viewPagerChat = (ViewPager) v.findViewById(R.id.view_pager_chat);

		// 表情界面UI
		m_rl_face_view = v.findViewById(R.id.rl_face_view);
		m_viewPagerFace = (ViewPager) v.findViewById(R.id.viewPagerFace);
		m_dotViewFace = (DotView) v.findViewById(R.id.dotViewFace);

		// 礼物界面UI
		m_rl_gift_view = m_ViewRoot.findViewById(R.id.rl_gift_view);

		m_giftNormal = m_ViewRoot.findViewById(R.id.ll_gift_normal);
		m_textViewNormal = (TextView) m_ViewRoot.findViewById(R.id.textViewNormal);
		m_giftLuck = m_ViewRoot.findViewById(R.id.ll_gift_luck);
		m_textViewLuck = (TextView) m_ViewRoot.findViewById(R.id.textViewLuck);
		m_giftLuxury = m_ViewRoot.findViewById(R.id.ll_gift_luxury);
		m_textViewLuxury = (TextView) m_ViewRoot.findViewById(R.id.textViewLuxury);
		m_viewNormal = m_ViewRoot.findViewById(R.id.viewNormal);

		m_textViewGold = (TextView) m_ViewRoot.findViewById(R.id.textViewGold);
		m_viewTopup = m_ViewRoot.findViewById(R.id.textViewTopup);
		m_viewSendGift = m_ViewRoot.findViewById(R.id.textViewSendGift);

		m_spinnerNumber = (Spinner) m_ViewRoot.findViewById(R.id.spinnerNumber);
		m_spinnerSendObject = (Spinner) m_ViewRoot.findViewById(R.id.spinnerObject);
		// 具体礼物展示Pager
		m_viewPagerGift = (ViewPager) m_ViewRoot.findViewById(R.id.viewPagerGift);
		m_dotViewGift = (DotView) m_ViewRoot.findViewById(R.id.dotViewGift);
	}

	@SuppressLint("ClickableViewAccessibility")
	void setListener() {
		
		mUiMoreTip.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				mChatDisplayFragment.scrollBottomWithUpdate(false,true);
				onInfoRead();
			}
		});
		
		m_animaDisappear.setAnimationListener(new AnimationListener() {

			@Override
			public void onAnimationEnd(Animation arg0) {
				// TODO Auto-generated method stub
				m_viewHornTop.setVisibility(View.GONE);
			}

			@Override
			public void onAnimationRepeat(Animation arg0) {
				// TODO Auto-generated method stub

			}

			@Override
			public void onAnimationStart(Animation arg0) {
				// TODO Auto-generated method stub

			}

		});

		m_viewPagerFace
				.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {

					@Override
					public void onPageSelected(int arg0) {
						// TODO Auto-generated method stub
						m_dotViewFace.setSelection(arg0);
					}

					@Override
					public void onPageScrolled(int arg0, float arg1, int arg2) {
						// TODO Auto-generated method stub

					}

					@Override
					public void onPageScrollStateChanged(int arg0) {
						// TODO Auto-generated method stub

					}
				});

		m_viewPagerChat
				.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {

					@Override
					public void onPageSelected(int position) {
						// TODO Auto-generated method stub
					}

					@Override
					public void onPageScrolled(int arg0, float arg1, int arg2) {
						// TODO Auto-generated method stub
					}

					@Override
					public void onPageScrollStateChanged(int arg0) {
						// TODO Auto-generated method stub

					}
				});
		m_RLChat.SetKeyBoardListener(new KeyBoardListener(){

			@Override
			public void OnKeyboardClose() {
				// TODO Auto-generated method stub
				//m_Home.showMiddle(true);
			}

			@Override
			public void OnKeyBoardOpen() {
				// TODO Auto-generated method stub
				//m_Home.showMiddle(false);
			}
			
		});
		// 回退
		m_ImgButtonBack.setOnClickListener(this);
		// 表情
		m_ImgButtonFace.setOnClickListener(this);
		// 礼物
		m_ImgButtonGift.setOnClickListener(this);
		//红包
		m_ImgButtonRedPacket.setOnClickListener(this);
		//进入房间
		mUiEnterRoom.setOnClickListener(this);
		//发送聊天
		mUiSendChat.setOnClickListener(this);
		// 更多操作
		m_ImgButtonHorn.setOnClickListener(this);
		//在线用户
		m_ImgButtonOnline.setOnClickListener(this);
		// 文本框
		m_EditTextChat.setOnTouchListener(new View.OnTouchListener() {

			@Override
			public boolean onTouch(View v, MotionEvent arg1) {
				// TODO Auto-generated method stub
				if (m_isShowFace)
					switchFaceShow();
				else if (m_isShowGift)
					switchGiftShow();
				else
				{
					ToolUtil.showKeyBoard(getActivity());
					
					m_handlerUi.postDelayed(new Runnable(){

						@Override
						public void run() {
							// TODO Auto-generated method stub
							mChatDisplayFragment.scrollBottom(true);
						}
						
					}, DelayMillSeconds);
					//ToolUtil.showKeyBoard(getActivity(),InputMethodManager.SHOW_FORCED);
					//mChatDisplayFragment.notifyChanged();
				}
				return false;
			}
		});  
		m_EditTextChat.setOnEditorActionListener(new TextView.OnEditorActionListener()
		{  
		    public boolean onEditorAction(TextView v, int actionId,KeyEvent event)
		    {
		        if (actionId==EditorInfo.IME_ACTION_SEND ||
		        		(event!=null&&event.getKeyCode()== KeyEvent.KEYCODE_ENTER))
		        {    
		        	long sysTime = System.currentTimeMillis();
		    		if(sysTime - mActionSendTime >500)
		        	{
		    			mActionSendTime = sysTime;
		    			return sendChatToKtv();
		        	}
		        	return true;
		        }    
		        return false;    
		       }    
		   }); 
		m_EditTextChat.addTextChangedListener(new TextWatcher() {

			@Override
			public void afterTextChanged(Editable arg0) {
				// TODO Auto-generated method stub

				if (m_EditTextChat.getText().length() == 0) {
					mUiSendChat.setEnabled(false);
//					m_ImgButtonGift.setVisibility(View.GONE);
//					m_EditTextChat.setImeOptions(EditorInfo.IME_ACTION_NONE);
//					m_ImgButtonGift.setImageResource(R.drawable.btn_chat_gift);
//					m_isShowSend = false;
				} else {
					mUiSendChat.setEnabled(true);
//					m_ImgButtonGift.setVisibility(View.VISIBLE);
//					m_EditTextChat.setImeOptions(EditorInfo.IME_ACTION_SEND);
//					m_ImgButtonGift.setImageResource(R.drawable.btn_chat_send);
//					m_isShowSend = true;
				}
			}

			@Override
			public void beforeTextChanged(CharSequence arg0, int start,
					int count, int after) {
				// TODO Auto-generated method stub
			}

			@Override
			public void onTextChanged(CharSequence arg0, int start, int before,
					int count) {
				// TODO Auto-generated method stub
				if (before == 0)// 说明是字符增加，大于0说明是减少
				{// 字符增加的时候判断下
					CharSequence seq = arg0.subSequence(start, start + count);

					if (FaceUtil.containsEmoji(seq.toString())) {
						m_EditTextChat.getText().delete(start, start + count);
						m_EditTextChat.setSelection(start);
						ShowUtil.showToast(getActivity(), R.string.sorry_emoji);
						m_ImgButtonFace.performClick();
					}
				}

			}

		});
		m_giftLuck.setOnClickListener(m_giftClickListener);
		m_giftNormal.setOnClickListener(m_giftClickListener);
		m_giftLuxury.setOnClickListener(m_giftClickListener);

		m_viewPagerGift
				.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {

					@Override
					public void onPageScrollStateChanged(int arg0) {
						// TODO Auto-generated method stub

					}

					@Override
					public void onPageScrolled(int arg0, float arg1, int arg2) {
						// TODO Auto-generated method stub

					}

					@Override
					public void onPageSelected(int pos) {
						// TODO Auto-generated method stub
						m_dotViewGift.setSelection(pos);
						if (pos < AppStatus.s_giftMgr.m_pageIndexNormal) {
							// 显示普通礼物
							switchGiftAnimation(GiftFragment.Gift_Normal);
						} else if (pos < AppStatus.s_giftMgr.m_pageIndexLuck) {
							// 显示幸运礼物
							switchGiftAnimation(GiftFragment.Gift_Luck);
						} else {
							// 显示奢侈礼物
							switchGiftAnimation(GiftFragment.Gift_Luxury);
						}
						// 页面切换，需要重新选择礼物
						//m_curGiftInfo = null;
					}

				});

		m_viewTopup.setOnClickListener(this);
		m_viewSendGift.setOnClickListener(this);
	}

	void switchTIAndEnterRoom(){
		if(AppStatus.isInRoom)//如果在房间
		{
			mUiEnterRoom.setVisibility(View.GONE);
			m_EditTextChat.setEnabled(true);
			m_EditTextChat.setVisibility(View.VISIBLE);
			m_ImgButtonFace.setVisibility(View.VISIBLE);
		}
		else
		{
			m_ImgButtonFace.setVisibility(View.GONE);
			m_EditTextChat.setEnabled(false);
			m_EditTextChat.setVisibility(View.GONE);
			m_EditTextChat.setText("");
			m_EditTextChat.setHint(R.string.chat_edittext_tip);
			mUiEnterRoom.setVisibility(View.VISIBLE);
		}
	}
	
	void initView() {
		switchTIAndEnterRoom();
		
		mUiSendChat.setEnabled(false);
		// 设置在线人数
		updateOnlineUserList();

		m_viewSelectWidth = mDm.widthPixels / 4;

		// 根据父视图的类型
		// RelativeLayout.LayoutParams params = new
		// RelativeLayout.LayoutParams(viewWidth,3);

		//m_ImgButtonBack.setImageResource(R.drawable.btn_home_menu);
		m_TextViewOnlineNum.setVisibility(View.VISIBLE);
		m_TextViewTitle.setText(R.string.chat_title);
		m_ImgButtonHorn.setVisibility(View.VISIBLE);
		m_ImgButtonOnline.setVisibility(View.VISIBLE);

		FragmentManager fm = getActivity().getSupportFragmentManager();
		m_dotViewFace.setDotCount(FaceManager.PAGE);
		m_viewPagerFace.setAdapter(new FragmentPagerAdapter(fm) {

			@Override
			public Fragment getItem(int arg0) {
				// TODO Auto-generated method stub
				return FaceFragment.newInstance(arg0, ChatFragment.this);
			}

			@Override
			public int getCount() {
				// TODO Auto-generated method stub
				return FaceManager.PAGE;
			}
		});

		mChatDisplayFragment = (ChatDisplayFragment) ChatDisplayFragment.newInstance(0, this,this, this);
		m_adapterFragment = new FragmentPagerAdapter(fm) {

			@Override
			public Fragment getItem(int arg0) {
				// TODO Auto-generated method stub
				return mChatDisplayFragment;
			}

			@Override
			public int getCount() {
				// TODO Auto-generated method stub
				// 互动聊天
				return 1;
			}
		};
		m_viewPagerChat.setAdapter(m_adapterFragment);

		initGift();
		// 屏蔽表情滑动
//		m_Home.addIgnoredAboveView(m_viewPagerFace);
//		// 屏蔽聊天内容滑动
//		m_Home.addIgnoredAboveView(m_viewPagerChat);
//		// 屏蔽礼物滑动
//		m_Home.addIgnoredAboveView(m_viewPagerGift);
	}

	void initGift() {
		
		if (AppStatus.user != null)
			m_textViewGold.setText("" + AppStatus.user.gold);
		// 设置礼物数量的Spinner
		List<Integer> listNumber = new ArrayList<Integer>();
		int[] arrayNumber = this.getActivity().getResources()
				.getIntArray(R.array.gift_number);
		for (int i = 0; i < arrayNumber.length; i++)
			listNumber.add(arrayNumber[i]);
		ArrayAdapter<Integer> adapterNumber = new ArrayAdapter<Integer>(
				getActivity(), R.layout.adapter_spinner_normal, listNumber) {

			@Override
			public View getDropDownView(int position, View convertView,
					ViewGroup parent) {
				// TODO Auto-generated method stub
				TextView text = null;
				if (convertView == null) {
					convertView = LayoutInflater.from(this.getContext())
							.inflate(R.layout.adapter_spinner_popup, null);
					text = (TextView) convertView
							.findViewById(R.id.textViewAdapterSpinnerPop);
					convertView.setTag(text);
				} else
					text = (TextView) convertView.getTag();

				text.setText("" + getItem(position));

				return convertView;
			}

		};
		m_spinnerNumber.setAdapter(adapterNumber);
		// 设置赠送对象的Spinner
		m_spinnerAdatperUser = new UserSpinnerAdapter(getActivity(), m_listUser);
		m_spinnerSendObject.setAdapter(m_spinnerAdatperUser);
		m_dotViewGift.setDotCount(AppStatus.s_giftMgr.getGiftTotal());
		FragmentManager fm = getActivity().getSupportFragmentManager();
		m_viewPagerGift.setAdapter(new FragmentPagerAdapter(fm) {

			@Override
			public Fragment getItem(int pos) {
				// TODO Auto-generated method stub
				return GiftFragment.newInstance(pos, ChatFragment.this);
			}

			@Override
			public int getCount() {
				// TODO Auto-generated method stub
				return AppStatus.s_giftMgr.getGiftTotal();
			}

		});

		// 默认普通礼物
		switchGiftType(GiftFragment.Gift_Normal);
	}

	View.OnClickListener m_giftClickListener = new View.OnClickListener() {

		@Override
		public void onClick(View v) {
			// TODO Auto-generated method stub
			switch (v.getId()) {
			case R.id.ll_gift_normal:
				ChatFragment.this.m_viewPagerGift.setCurrentItem(0);
				break;
			case R.id.ll_gift_luck:
				ChatFragment.this.m_viewPagerGift
						.setCurrentItem(AppStatus.s_giftMgr.m_pageIndexNormal);
				break;
			case R.id.ll_gift_luxury:
				ChatFragment.this.m_viewPagerGift
						.setCurrentItem(AppStatus.s_giftMgr.m_pageIndexLuck);
				break;
			}
		}
	};
	
void showHBPopupWindow(){
		
		if(mHBPW == null)
		{
			View popupView = LayoutInflater.from(getActivity()).inflate(R.layout.popup_hb_select, null);
			
			mHBPW = new PopupWindow(popupView,LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT); //后两个参数是width和height
			//menuWindow.showAsDropDown(layout); //设置弹出效果
			//menuWindow.showAsDropDown(null, 0, layout.getHeight());
			//设置如下四条信息，当点击其他区域使其隐藏，要在show之前配置
			mHBPW.setFocusable(true);
			mHBPW.setOutsideTouchable(true);
			mHBPW.update();
			mHBPW.setBackgroundDrawable(new BitmapDrawable());
			
			popupView.findViewById(R.id.tv_large).setOnClickListener(new View.OnClickListener() {
				
				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					Intent intent = new Intent(getActivity(), MoneyBagRedPacketActivity.class);
					intent.putExtra(HongBaoSendFragment.EXTRA_HONGBAO_TYPE	
							, HongBaoSendFragment.HONGBAO_LOBBY);
					startActivity(intent);
					mHBPW.dismiss();
				}
			});
			
			popupView.findViewById(R.id.tv_room).setOnClickListener(new View.OnClickListener() {
				
				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					if(AppStatus.isInRoom)
					{
						Intent intent = new Intent(getActivity(), MoneyBagRedPacketActivity.class);
						intent.putExtra(HongBaoSendFragment.EXTRA_HONGBAO_TYPE	
								, HongBaoSendFragment.HONGBAO_ROOM);
						intent.putExtra(HongBaoSendFragment.EXTRA_HONGBAO_IDX, AppStatus.s_nRoomId);
						startActivity(intent);
					}
					else
					{
						mUiEnterRoom.performClick();
					}
					
					mHBPW.dismiss();
				}
			});
		}
		
		//设置layout在PopupWindow中显示的位置
		mHBPW.showAtLocation(m_ImgButtonRedPacket, Gravity.BOTTOM|Gravity.LEFT, 0,ToolUtil.dip2px(getActivity(), OffsetYHBWindow));
	}
	
	void showUserOptionPopwindow(View v,int[] location)
	{
		if(m_UserOperationMenu == null)
		{
			LayoutInflater inflater = (LayoutInflater)getActivity().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			View viewChatOptionView = inflater.inflate(R.layout.frament_interactive_chat_popupwindow, null);
			
			m_UserOperationMenu = new PopupWindow(viewChatOptionView,WindowManager.LayoutParams.WRAP_CONTENT,WindowManager.LayoutParams.WRAP_CONTENT);
			m_UserOperationMenu.setAnimationStyle(R.style.interactiv_chat_popwindow_anim_style);
			
			m_BtnSendGift = (ImageButton) viewChatOptionView.findViewById(R.id.interactive_chat_option_gift_pic);
			m_BtnSendGift.setOnClickListener(new OnClickListener() {
				@Override
				public void onClick(View v) {
					m_UserOperationMenu.dismiss();
					LocalData_UserInfo userInfo = new LocalData_UserInfo();
					userInfo.m_idx = m_privateChatTo.idx;
					onGiftSendObjectClick(userInfo);
				}
			});
			m_BtnRedPacket = (ImageButton) viewChatOptionView.findViewById(R.id.interactive_chat_option_redpacket_pic);
			m_BtnRedPacket.setOnClickListener(new OnClickListener() {
				@Override
				public void onClick(View v) {
				    m_UserOperationMenu.dismiss();
				    Intent i = new Intent(getActivity(), MoneyBagRedPacketActivity.class);
				    i.putExtra(HongBaoSendFragment.EXTRA_HONGBAO_TYPE
				    		, HongBaoSendFragment.HONGBAO_PERSON);
				    i.putExtra(HongBaoSendFragment.EXTRA_HONGBAO_IDX, m_privateChatTo.idx);
					startActivity(i);
				}
			});
			m_BtnPrivateLetter = (ImageButton) viewChatOptionView.findViewById(R.id.interactive_chat_option_letter_pic);
			m_BtnPrivateLetter.setOnClickListener(new OnClickListener() {
				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					m_UserOperationMenu.dismiss();
				    changePrivateObject();
				    
				    mUiEnterRoom.setVisibility(View.GONE);
					m_EditTextChat.setEnabled(true);
					m_EditTextChat.setVisibility(View.VISIBLE);
					m_ImgButtonFace.setVisibility(View.VISIBLE);
					m_EditTextChat.requestFocus();
				}
			});
			
			// 使其聚集
			m_UserOperationMenu.setFocusable(true);
			// 设置允许在外点击消失
			m_UserOperationMenu.setOutsideTouchable(true);
			m_UserOperationMenu.setBackgroundDrawable(new BitmapDrawable());
			
			m_UserOperationMenu.setOnDismissListener(new OnDismissListener(){

				@Override
				public void onDismiss() {
					// TODO Auto-generated method stub
					ChatFragment.sCanAutoScroll = true;
					if(mChatDisplayFragment.mIsMoreInfo)
					{
						mChatDisplayFragment.scrollBottomWithUpdate(false,true);
						mUiMoreTip.setVisibility(View.GONE);
					}
				}
				
			});
		}
		
		ChatFragment.sCanAutoScroll = false;
	    m_UserOperationMenu.showAtLocation(v, Gravity.NO_GRAVITY, location[0]+v.getWidth()/2, location[1]-ToolUtil.dip2px(getActivity(), OffsetYUserWindow));
	}

	// 更改标题栏的选中状态 1:普通，2:幸运，3:奢侈
	void switchGiftType(int type) {
		switch (type) {
		case GiftFragment.Gift_Normal:
			m_textViewNormal.setTextColor(m_selectGiftTextColor);
			m_textViewLuck.setTextColor(m_normalGiftTextColor);
			m_textViewLuxury.setTextColor(m_normalGiftTextColor);
			break;
		case GiftFragment.Gift_Luck:
			m_textViewNormal.setTextColor(m_normalGiftTextColor);
			m_textViewLuck.setTextColor(m_selectGiftTextColor);
			m_textViewLuxury.setTextColor(m_normalGiftTextColor);
			break;
		case GiftFragment.Gift_Luxury:
			m_textViewNormal.setTextColor(m_normalGiftTextColor);
			m_textViewLuck.setTextColor(m_normalGiftTextColor);
			m_textViewLuxury.setTextColor(m_selectGiftTextColor);
			break;
		}
	}

	void switchSoftInputMode(boolean isAdjustPan) {
		if (isAdjustPan)
			getActivity().getWindow().setSoftInputMode(
					WindowManager.LayoutParams.SOFT_INPUT_ADJUST_PAN);
		else
			getActivity()
					.getWindow()
					.setSoftInputMode(
							WindowManager.LayoutParams.SOFT_INPUT_STATE_HIDDEN
									| WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE);
	}

	void showSoftInput(boolean show) {
		if (show) {
			switchSoftInputMode(true);
			ToolUtil.showKeyBoard(getActivity());
			switchSoftInputMode(false);
		} else {
			switchSoftInputMode(true);
			ToolUtil.closeKeyBoard(getActivity());
			switchSoftInputMode(false);
		}
	}

	void switchFaceShow() {
		// 如果礼物界面正在展示
		if (m_isShowGift) {
			m_rl_gift_view.setVisibility(View.GONE);
			m_curGiftInfo = null;
			m_ImgButtonGift.setImageResource(R.drawable.btn_chat_gift);
			m_rl_face_view.setVisibility(View.VISIBLE);
			m_ImgButtonFace.setImageResource(R.drawable.btn_chat_keybord);
			m_isShowGift = false;
			m_isShowFace = true;
			return;
		}
		else 
		{
			if (m_isShowFace) {
				m_rl_face_view.setVisibility(View.GONE);
				showSoftInput(true);
				m_ImgButtonFace.setImageResource(R.drawable.btn_chat_face);
			} else {
				showSoftInput(false);
				m_rl_face_view.setVisibility(View.VISIBLE);
				m_ImgButtonFace.setImageResource(R.drawable.btn_chat_keybord);
				
				m_handlerUi.postDelayed(new Runnable(){

					@Override
					public void run() {
						// TODO Auto-generated method stub
						mChatDisplayFragment.scrollBottom(true);
					}
					
				}, DelayMillSeconds);
			}
		}
		m_isShowFace = !m_isShowFace;
	}

	void switchGiftShow() {
		// 如果表情界面正在展示
		if (m_isShowFace) {
			m_rl_face_view.setVisibility(View.GONE);
			m_ImgButtonFace.setImageResource(R.drawable.btn_chat_face);
			m_rl_gift_view.setVisibility(View.VISIBLE);
			m_ImgButtonGift.setImageResource(R.drawable.btn_chat_keybord);
			m_isShowFace = false;
			m_isShowGift = true;
			return;
		} else {
			if (m_isShowGift) {
				m_rl_gift_view.setVisibility(View.GONE);
				m_curGiftInfo = null;
				showSoftInput(true);
				m_ImgButtonGift.setImageResource(R.drawable.btn_chat_gift);
			} else {
				showSoftInput(false);
				m_rl_gift_view.setVisibility(View.VISIBLE);
				m_ImgButtonGift.setImageResource(R.drawable.btn_chat_keybord);
				
				m_handlerUi.postDelayed(new Runnable(){

					@Override
					public void run() {
						// TODO Auto-generated method stub
						mChatDisplayFragment.scrollBottom(true);
					}
					
				}, DelayMillSeconds);
			}
		}
		m_isShowGift = !m_isShowGift;
	}

	// 更新礼物选择栏的状态显示
	void switchGiftAnimation(int index) {
		if (m_curGiftType == index || m_nextGiftType == index)
			return;

		if (m_viewGiftSelectWidth == 0)
			m_viewGiftSelectWidth = m_viewNormal.getWidth();
		if (animationGift != null)
			animationGift.cancel();

		m_nextGiftType = index;

		animationGift = new TranslateAnimation(m_viewGiftSelectWidth
				* m_curGiftType, m_viewGiftSelectWidth * index, 0, 0);
		animationGift.setDuration(AnimaDuration);
		animationGift.setFillAfter(true);
		animationGift.setFillEnabled(true);
		animationGift.setAnimationListener(new AnimationListener() {

			@Override
			public void onAnimationEnd(Animation arg0) {
				// TODO Auto-generated method stub
				m_curGiftType = m_nextGiftType;
				switchGiftType(m_curGiftType);
			}

			@Override
			public void onAnimationRepeat(Animation arg0) {
				// TODO Auto-generated method stub

			}

			@Override
			public void onAnimationStart(Animation arg0) {
				// TODO Auto-generated method stub

			}

		});
		m_viewNormal.startAnimation(animationGift);
	}

	void clearInputFaceGift() {
		if (m_isShowFace) {
			m_rl_face_view.setVisibility(View.GONE);
			m_ImgButtonFace.setImageResource(R.drawable.btn_chat_face);
			m_isShowFace = false;
		}
		if (m_isShowGift) {
			m_rl_gift_view.setVisibility(View.GONE);
			m_ImgButtonGift.setImageResource(R.drawable.btn_chat_gift);
			m_isShowGift = false;
		}
		ToolUtil.closeKeyBoard(getActivity());
	}

	boolean sendChatToKtv(){
		//if (m_isShowSend)
		long sysTime = System.currentTimeMillis();
		if(sysTime - mClickLastTime >MainHomeFragment.SEARCH_TIME)
    	{
			mClickLastTime = sysTime;
			String textPlain = m_EditTextChat.getText().toString();// 字符串
			
			if (TextUtils.isEmpty(textPlain))
			{
				ShowUtil.showToast(getActivity(),R.string.chat_no_input);
				return false;
			}
			else if(textPlain.length() >= 1000)
			{
				ShowUtil.showToast(getActivity(),R.string.text_toolong);
				return false;
			}
			
			if (AppStatus.s_isConnectedLobby) 
			{
				int toIdx = 0;

				String name = getPrivateChatObjectName();
				// 如果存在私聊对象，则去查找是否是合法对象

				if (!TextUtils.isEmpty(name))
				{
					if (m_mapNameIdx.containsKey(name))
					{
						toIdx = m_mapNameIdx.get(name);
					} 
					else 
					{
						// 聊天对象非法
						ShowUtil.showToast(getActivity(),R.string.char_privateobject_ilegal);
						return false;
					}
				} 
				else 
				{
					if(!AppStatus.isInRoom)
					{
						//需要绑定到包厢
						ShowUtil.showToast(getActivity(),R.string.should_bind_room);
						return false;
					}
				}

				String finalMsg = AppStatus.s_NetUtilMgr.m_JniUtil.MFFilter(textPlain);
				// 如果toIdx为0，则默认公聊发送
				if (0 == AppStatus.s_NetUtilMgr.m_JniUtil.SendChatKTV(toIdx, finalMsg))
				{
					if(!AppStatus.isInRoom)
					{
						this.switchTIAndEnterRoom();
					}
					else
					{
						m_EditTextChat.setHint(R.string.chat_edittext_tip);
						m_EditTextChat.setText("");
					}
				}
				else
				{
					ShowUtil.showToast(getActivity(),R.string.chat_send_failed);
					return false;
				}
			}
			else
			{
			    ShowUtil.showToast(getActivity(), R.string.no_networking);
			    return false;
			}
			return true;
		} 
		else
		{
			ShowUtil.showToast(getActivity(), R.string.tip_chat_ofen);
			return false;
		}
	}
	
	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		switch (arg0.getId()) {
		//进入房间
		case R.id.btn_enterroom:
		{
			if (mEnterRoomManager != null) {
				mEnterRoomManager.showEnterRoomMenuDialog(true);
			}
			break;
		}
		//发送聊天
		case R.id.ibtn_chat_send:
		{
			sendChatToKtv();
			break;
		}
		case R.id.iv_back:// 回退
			clearInputFaceGift();
			//m_Home.showMenu();
			getActivity().finish();
			break;
		case R.id.imgBtnHorn:// 发喇叭
			// 跳转到发喇叭界面
			Intent intent = new Intent(getActivity(), SendHornActivity.class);
			startActivity(intent);
			break;
		case R.id.ibtn_chat_face:// 表情键盘显示
			switchFaceShow();
			break;
		case R.id.ibtn_chat_gift:// 礼物键盘显示
			if (m_isShowSend) {
				sendChatToKtv();
			} else {
				// 显示送礼
				switchGiftShow();
			}
			break;
		case R.id.textViewSendGift:// 送礼物
			if (m_curGiftInfo == null) {
				ShowUtil.showToast(getActivity(), R.string.gift_send_null);
				break;
			}
			// 清空当前的礼物信息
			m_preGiftInfo = m_curGiftInfo;
			int giftId = m_curGiftInfo.id;
			//m_curGiftInfo = null;

			LocalData_UserInfo user = m_SendGiftToOnlineUser;
			if (user == null || user.m_idx == 0) {
				ShowUtil.showToast(getActivity(), R.string.gift_send_tonull);
				break;
			}

			if (checkLobbyConnect()) {
				int giftNumber = (Integer) m_spinnerNumber.getSelectedItem();
				AppStatus.s_NetUtilMgr.m_JniUtil.SendGiftInLobby(user.m_idx,
						giftId, giftNumber, AppStatus.s_nRoomId);
			}
			break;
		case R.id.textViewTopup:
		{// 去充值界面
			//m_Home.switchMenu(HomeMenuItemManager.MENU_ACCOUNT_RECHARGE);
			Intent i = new Intent(getActivity(),ChargeActivity.class);
			startActivity(i);
			getActivity().finish();
			break;
		}
			//发红包选择栏
		case R.id.ibtn_small_red_packet:
			clearInputFaceGift();
			this.showHBPopupWindow();
			break;
		case R.id.tv_onlineview:
		{
			// 跳转到在线人数界面
			clearInputFaceGift();
			Intent i = new Intent(getActivity(), PhoneOnlineUserActivity.class);
			startActivityForResult(i, RequestCodeDef.REQUEST_ONLINE_USERS);
			break;
		}
		case R.layout.frament_interactive_chat_popupwindow:
			Toast.makeText(getActivity().getApplicationContext(), "click gift option", Toast.LENGTH_SHORT).show();
		default:
			break;
		}
	}
	
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		if(requestCode == RequestCodeDef.REQUEST_ONLINE_USERS)
		{
			switch (resultCode)
			{
			case PhoneOnlineUserFragment.RETURN_SEND_GIFT:
			{
				LocalData_UserInfo dataUser =(LocalData_UserInfo)data.getSerializableExtra(
			    		PhoneOnlineUserFragment.RESULTCODE_USERINFO);
			    m_SendGiftToOnlineUser = dataUser;
			    onGiftSendObjectClick(dataUser);
			    break;
			}
			case PhoneOnlineUserFragment.RETURN_SEND_PRIVATE_LETTER:
			{
				LocalData_UserInfo dataUser =(LocalData_UserInfo)data.getSerializableExtra(
			    		PhoneOnlineUserFragment.RESULTCODE_USERINFO);
				onTalkClick(dataUser);
			    break;
			}
			default:
			    break;
			}   
			return ;
		}
		else if(requestCode == RequestCodeDef.REQUEST_SCAN_CODE
				|| requestCode == RequestCodeDef.REQUEST_PWD_ENTER)
		{
			mEnterRoomManager.activityResult(requestCode, resultCode, data);
			return ;
		}
		
		super.onActivityResult(requestCode, resultCode, data);
	}

	// 更新在线用户列表
	void updateOnlineUserList() {
		m_listUser.clear();
		mMapIdexIndex.clear();
		m_mapNameIdx.clear();
		LocalData_UserInfo empty = new LocalData_UserInfo();
		empty.m_name = "请选择";
		m_listUser.add(empty);
		if (AppStatus.s_userMy != null)
			m_listUser.add(AppStatus.s_userMy);
		List<LocalData_UserInfo> listUser = AppStatus.s_NetUtilMgr.getUserInfoList();
		m_listUser.addAll(listUser);

		mMapIdexIndex.put(AppStatus.s_userMy.m_idx, 0);
		for (int i = 0; i < listUser.size(); i++) {
			LocalData_UserInfo info = listUser.get(i);
			m_mapNameIdx.put(info.m_name, info.m_idx);
			mMapIdexIndex.put(info.m_idx, i+1);
		}
		m_TextViewOnlineNum.setText("(" + (m_listUser.size() - 1) + "人)");

		if (m_spinnerAdatperUser != null)
			m_spinnerAdatperUser.notifyDataSetChanged();
	}

	// 喇叭消失动画
	Runnable m_runDisappear = new Runnable() {

		@Override
		public void run() {
			// TODO Auto-generated method stub
			m_viewHornTop.startAnimation(m_animaDisappear);
		}

	};

	Handler m_handlerUi = new Handler();

	void deleteEditTextAChar() {
		int selection = m_EditTextChat.getSelectionStart();// 光标位置
		String textPlain = m_EditTextChat.getText().toString();// 字符串
		if (selection > 0) {
			int selectionBefore = selection - 1;
			String endStr = textPlain.substring(selectionBefore, selection);// 取出光标前面一个字符
			if ("]".equals(endStr))// 如果是"]"
			{
				int start = textPlain.lastIndexOf("[", selectionBefore);
				if (start == -1)// 如果前面没有"["
					m_EditTextChat.getText().delete(selectionBefore, selection);// 去掉一个字
				else {
					int end = selection;
					Spanned textSpanned = m_EditTextChat.getEditableText();
					ImageSpan[] imgSpans = textSpanned.getSpans(start, end,
							ImageSpan.class);
					if (imgSpans.length == 0)// 没有ImageSpan
						m_EditTextChat.getText().delete(selectionBefore,
								selection);// 去掉一个字
					else
						m_EditTextChat.getText().delete(start, end);// 去掉一个表情
				}
			} else
				m_EditTextChat.getText().delete(selectionBefore, selection);// 去掉一个字
		}
	}

	@Override
	public void onFaceClick(int page, int pos) {
		// TODO Auto-generated method stub
		if (AppStatus.s_faceMgr.isRemoveButton(page, pos)) {
			// 删除输入框中的一个字 （包含表情，文字，字母）
			deleteEditTextAChar();
		} else {
			// 添加一个表情到输入框中
			SpannableString str = AppStatus.s_faceMgr.getFaceSpannableString(
					page, pos);
			int start = m_EditTextChat.getSelectionStart();
			m_EditTextChat.getText().insert(start, str);
		}
	}

	// 如果点击聊天区域，将使表情、礼物、键盘不可可用
	@Override
	public void onListViewTouchDown() {
		// TODO Auto-generated method stub
		//mUiMoreTip.setVisibility(View.GONE);
		if(!AppStatus.isInRoom)
			switchTIAndEnterRoom();
		else
			m_EditTextChat.setHint(R.string.chat_edittext_tip);
		clearInputFaceGift();
	}
	
	@Override
	public void onInfoNew(){
		mUiMoreTip.setVisibility(View.VISIBLE);
	}

	@Override
	public void onInfoRead(){
		mUiMoreTip.setVisibility(View.GONE);
	}
	
	@Override
	public void onGiftClick(GiftInfo info) {
		// TODO Auto-generated method stub
		// 礼物GridView的回调 - 选中当前礼物信息
		m_curGiftInfo = info;
	}

	@Override
	public void onPrivateUserClickListener(ChatDisplayInfo info,View v) {
		// TODO Auto-generated method stub
		// 点击用户头像、昵称的回调 - 添加到私聊
		// TODO Auto-generated method stub
		m_privateChatTo.idx = info.tagIdx;
		m_privateChatTo.name = info.tagName;
		m_SendGiftToOnlineUser.m_idx = info.tagIdx;
		
		int[] photoLocation = new int[2];
        v.getLocationInWindow(photoLocation);
        
        //先隐藏可能打开的键盘
        showSoftInput(false);
        showUserOptionPopwindow(v, photoLocation);
	}
	
	@Override
	public void onHongBaoClick(ChatDisplayInfo info){
		
		mLoading.show();
		if(info.hongbao.hbtype == 0)//如果是个人红包，则直接获取
		{
			if(info.tagIdx == AppStatus.s_userMy.m_idx || info.tagIdx == 0)//自己发的红包
				onHBDetail(info.hongbao.hbid);
			else//别人发的红包
				AppStatus.s_NetUtilMgr.m_JniUtil.RequestVieHB(info.hongbao.hbid);
		}
		else//其他红包需要先弹出一个对话框
		{
			mTempInfo = info;
			//需要查询红包当前的状态
			AppStatus.s_NetUtilMgr.m_JniUtil.RequestHBStatus(info.hongbao.hbid);
		}
	}

	@Override
	public void onGiftSendObjectClick(LocalData_UserInfo info) {
		// TODO Auto-generated method stub
		// 点击用户操作界面送礼 - 显示送礼界面，并赋值该对象
		if(mMapIdexIndex.containsKey(info.m_idx))
		{
			int index = mMapIdexIndex.get(info.m_idx);
			if (index != -1)
				m_spinnerSendObject.setSelection(index+1);
			else
				m_spinnerSendObject.setSelection(0);
		}

		if (m_isShowGift)
			return;
		switchGiftShow();
	}

	@Override
	public void onTalkClick(LocalData_UserInfo info) {
		// TODO Auto-generated method stub
		// 点击用户操作栏聊天 - 添加到私聊
		m_privateChatTo.idx = info.m_idx;
		m_privateChatTo.name = info.m_name;
		changePrivateObject();
		
		mUiEnterRoom.setVisibility(View.GONE);
		m_EditTextChat.setEnabled(true);
		m_EditTextChat.setVisibility(View.VISIBLE);
		m_ImgButtonFace.setVisibility(View.VISIBLE);
		m_EditTextChat.requestFocus();
	}

	@Override
	public void onUserInfoClick() {
		// 隐藏表情，礼物，键盘等
		if(!AppStatus.isInRoom)
			switchTIAndEnterRoom();
		else
			m_EditTextChat.setHint(R.string.chat_edittext_tip);
		clearInputFaceGift();
	}
	
	@Override
	public void onHongBao(LocalData_UserInfo info) {
		// TODO Auto-generated method stub
		Intent intent = new Intent(getActivity(), MoneyBagRedPacketActivity.class);
		intent.putExtra(HongBaoSendFragment.EXTRA_HONGBAO_TYPE
				, HongBaoSendFragment.HONGBAO_PERSON);
		intent.putExtra(HongBaoSendFragment.EXTRA_HONGBAO_IDX, info.m_idx);
		startActivity(intent);
	}
	
	@Override
	public void onHBDetail(String hbid) {
		// TODO Auto-generated method stub
		if(!mLoading.isShowing())
			mLoading.show();//等待对话框
		AppStatus.s_NetUtilMgr.m_JniUtil.RequestHBDetail(hbid);
	}

	@Override
	public void onHBCatch(String hbid) {
		// TODO Auto-generated method stub
		AppStatus.s_NetUtilMgr.m_JniUtil.RequestVieHB(hbid);
	}

	String getPrivateChatObjectName() {
		String name = "";
		String text = m_EditTextChat.getHint().toString();
		if (text.length() != 0) {
			int end = text.indexOf("说:");
			if (text.charAt(0) == '对' && end != -1)
				name = text.substring(1, end);
		}
		return name;
	}

	void changePrivateObject() {
		// 如果目前还没有聊天对象
		String toName = "对" + m_privateChatTo.name + "说:";
		m_EditTextChat.setHint(toName);
	}

	boolean checkLobbyConnect() {
		if (!AppStatus.s_isConnectedLobby) {
			ShowUtil.showToast(getActivity(), R.string.no_networking);
			return false;
		}
		return true;
	}

	class PrivateChatTo {
		int idx = 0;// 0 默认公聊
		String name = "";
	}
	
	@Override
	public void dealWithMsg(Message msg) {
		// TODO Auto-generated method stub
		switch (msg.what) {
		//重连房间成功
		case XGMessage.XGMSG_ROOMENTER_SUCCESS:
		{
			switchTIAndEnterRoom();
			break;
		}
		//房间断开连接
		case XGMessage.XGMSG_ROOMCON_ERROR:
		{
			switchTIAndEnterRoom();
			break;
		}
		//查询红包结果返回
		case XGMessage.XGMSG_HONGBAO_STATUS:
		{
			if(msg.arg1 == JniMessage.OPTION_TIMEOUT)
			{
				mLoading.dismiss();
				ShowUtil.showToast(getActivity(), R.string.server_timeout);
				break;
			}
			
			String hbid = (String) msg.obj;
			if(!TextUtils.equals(mTempInfo.hongbao.hbid, hbid))
				break;
			
			if(msg.arg1 == 0)//还有机会抢
			{
				mLoading.dismiss();
				mVieDialog = new HongBaoVieDialog(getActivity(),mTempInfo,true,this);
				mVieDialog.show();
			}
			else if(msg.arg1 == 1)//已经抢完
			{
				mLoading.dismiss();
				mVieDialog = new HongBaoVieDialog(getActivity(),mTempInfo,false,this);
				mVieDialog.show();
			}
			else if(msg.arg1 == 2)//已抢过红包
			{
				//直接查询红包详细结果
				onHBDetail(mTempInfo.hongbao.hbid);
			}
			break;
		}
		//抢红包结果返回
		case XGMessage.XGMSG_HONGBAO_VIERESULT:
		{
			//针对个人红包，显示的加载框
			if(mLoading.isShowing())
				mLoading.dismiss();
			
			if(msg.arg1 == JniMessage.OPTION_TIMEOUT)
			{
				//如果抢红包的对话框正在显示，则隐藏
				if(mVieDialog == null || !mVieDialog.isShowing())
					mVieDialog.dismiss();
				ShowUtil.showToast(getActivity(), R.string.hognbao_vie_timeout);
				break;
			}
			
			HongBaoInfoToUi info = (HongBaoInfoToUi) msg.obj;
			
			if(info.hbtype == 0)//个人红包
			{
				if(msg.arg1 == 1)
				{
					HongBaoPersonDialog dialog = new HongBaoPersonDialog(getActivity(),info);
					dialog.show();
				}
				else if(msg.arg1 == 2)
				{
					//直接查询红包详细结果
					onHBDetail(info.hbid);
				}
			}
			else//争抢红包
			{
				//如果返回一个异常结果，则跳过这个
				if(mVieDialog == null || !mVieDialog.isShowing() 
						|| !TextUtils.equals(mVieDialog.mHBId, info.hbid))
					break;
					
				if(msg.arg1 == 0)//没抢到
				{
					//停止特效，显示一些信息
					mVieDialog.stopWait();
				}
				else //if(msg.arg1==1 || msg.arg1==2)//抢到了 或者 已经抢过了
				{
					//隐藏对话框
					mVieDialog.dismiss();
					
					//直接查询红包详细结果
					onHBDetail(info.hbid);
				}
			}
			break;
		}
		//查看红包详细结果返回
		case XGMessage.XGMSG_HONGBAO_DETAIL:
		{
			mLoading.dismiss();
			if(msg.arg1 == 0)
			{
				ChatFragment.sHongBaoDetail = (HongBaoDetailInfoToUi) msg.obj;
				//跳转到红包领取详情页面
				Intent i = new Intent(getActivity(),HongBDetailActivity.class);
				startActivity(i);
			}
			else if(msg.arg1 == JniMessage.OPTION_TIMEOUT)
				ShowUtil.showToast(getActivity(), R.string.server_timeout);
			break;
		}
		//喇叭信息提醒
		case XGMessage.XGMSG_HORN_NOTIFY: 
		{
			ChatDisplayInfo info = AppStatus.s_NetUtilMgr.m_hornTopDisplay;
			if (msg.arg1 == 1 && info != null) {
				m_viewHornTop.clearAnimation();
				m_handlerUi.removeCallbacks(m_runDisappear);

				m_viewHornTop.setVisibility(View.VISIBLE);
				m_handlerUi.postDelayed(m_runDisappear, 4000);

				if (TextUtils.isEmpty(info.tagName))
					m_textViewHornName.setText("我");
				else
					m_textViewHornName.setText(info.tagName);
				m_textViewHornContent.setText(AppStatus.s_faceUtil.getRichString(info.horn.msg));

				String strTime = ToolUtil.formatTimestampToStr(info.horn.time*1000L, "HH:mm");
				m_textViewHornTime.setText(strTime);
				if (TextUtils.isEmpty(info.horn.room_name)) {
					m_textViewHornAddress.setVisibility(View.GONE);
					m_textViewHornAddress.setText("");
				} else {
					m_textViewHornAddress.setText("—" + info.horn.room_name
							+ "包厢");
					m_textViewHornAddress.setVisibility(View.VISIBLE);
				}
			}
			break;
		}
		// 更新在线用户数量
		case XGMessage.XGMSG_USER_LOGOUT:
		case XGMessage.XGMSG_USERINFO_NOTIFY:
			updateOnlineUserList();
			break;
		// 送礼结果
			
		case XGMessage.XGMSG_GIFTRESULT_NOTIFY: 
		{
			if(msg.arg1 != 0)
			{
				if(msg.arg2 == JniMessage.OPTION_TIMEOUT)
					ShowUtil.showToast(getActivity(), R.string.gift_send_timeout);
				else if(msg.arg2 == JniMessage.ROOM_ERROR_GIFT_BALANCE)
				{
					//金币不足，显示充值提示框
					DialogManager.showTopUpDialog(getActivity(),
							new OnDialogListener() {

								@Override
								public void onOk(int type) {
									// TODO Auto-generated method stub
									m_preGiftInfo = null;
									AppStatus.s_isNeedToTopup = true;
									//m_Home.switchMenu(HomeMenuItemManager.MENU_ACCOUNT_RECHARGE);
								}

								@Override
								public void onCancel() {
									// TODO Auto-generated method stub
									m_curGiftInfo = m_preGiftInfo;
								}
							});
				}
				else if(msg.arg2 == JniMessage.ROOM_ERROR_GIFT_UNKOWN)
					ShowUtil.showToast(getActivity(), R.string.gift_gold_0);
				else if(msg.arg2 == JniMessage.ROOM_ERROR_GIFT_NOTEXIST)
					ShowUtil.showToast(getActivity(), R.string.gift_no_business);
				else if(msg.arg2 == JniMessage.ROOM_ERROR_GIFT_OFFLINE)
					ShowUtil.showToast(getActivity(), R.string.gift_send_offline);
			}
			else
				m_textViewGold.setText("" + AppStatus.user.gold);
			break;
		}
		case XGMessage.XGMSG_USERGOLD_UPDATE:
			m_textViewGold.setText("" + AppStatus.user.gold);
			break;
		// 更新礼物信息
		case XGMessage.XGMSG_GIFTDATA_FINISHED:
			if (m_viewPagerGift != null)
				m_viewPagerGift.getAdapter().notifyDataSetChanged();
			break;
		case XGMessage.XGMSG_HORNRESULT_NOTIFY:
			if (msg.arg1 == 0)
				m_textViewGold.setText("" + AppStatus.user.gold);
			else
			{
				if(msg.arg2 == JniMessage.OPTION_TIMEOUT)
					ShowUtil.showToast(getActivity(), R.string.horn_send_timeout);
				else if(msg.arg2 == JniMessage.LOBBY_SPEAKER_BALANCE)
				{
					// 金币不足，显示充值提示框
					DialogManager.showTopUpDialog(getActivity());
				}
			}
			break;
		}
	}
}
