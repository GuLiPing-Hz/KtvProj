package assistant.fragment.home;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import order.adapter.HomeAdapter;
import order.show.DotView;
import order.show.activity.OrderActivity;
import order.show.activity.OrderTypeMenu;
import order.show.activity.OrderTypeRanks;
import order.show.activity.OrderTypeSongs;
import order.show.activity.OrderTypeTopSongs;
import order.show.activity.OrderedListActivity;
import order.show.activity.SongSearch;
import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.OnPageChangeListener;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import assistant.activity.CommonBrowserActivity;
import assistant.activity.GiftLogActivity;
import assistant.activity.Home;
import assistant.activity.home.ChargeActivity;
import assistant.activity.home.ChatActivity;
import assistant.activity.home.HongBListActivity;
import assistant.activity.home.RoomOrderActivity;
import assistant.adapter.BannerAdapter;
import assistant.adapter.BannerAdapter.BannerClickMethod;
import assistant.dialog.LoadingDialog;
import assistant.entity.Banner;
import assistant.entity.Banner.BannerType;
import assistant.entity.TopicInfo;
import assistant.fragment.BaseFragment;
import assistant.fragment.BaseFragmentHelper;
import assistant.global.AppStatus;
import assistant.global.KtvAssistantAPIConfig;
import assistant.global.MessageDef;
import assistant.global.RequestCodeDef;
import assistant.manager.DialogManager;
import assistant.manager.DialogManager.OnDialogListener;
import assistant.manager.NetUtilManager.INetUtilListener;
import assistant.task.gift.GiftExchangeTask;
import assistant.task.songapi.GetBannerListTask;
import assistant.util.ImageUtil;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;
import assistant.view.DrawableClickEditText;
import assistant.view.DrawableClickEditText.DrawableClickListener;

import com.jni.netutil.ResultData_SongList;
import com.jni.netutil.XGMessage;
import com.jni.netutil.out.HongBaoInfoToUi;
import com.zxing.activity.CaptureActivity;

/**
 * @author 宋静
 * @version 创建时间：2013-12-9 下午7:56:15 类说明 点歌首页
 */
public class MainHomeFragment extends BaseFragment 
	implements INetUtilListener,View.OnClickListener,OnDialogListener
{
	public static final int SCAN_UNKNOWN = 0;
	public static final int SCAN_GIFT = 1;
	public static final long SEARCH_TIME = 1000L;
	
	int mScanType = SCAN_UNKNOWN;
	
	private Home mHome;
	private View vRoot;
	
	/* ================ 初始化 =============== */
	/* view */
	private TextView tv_title, tv_number;
	TextView mUiTip;
	private ImageButton btn_menu;
	private ImageButton btn_order;
	private DrawableClickEditText et_search;
	private ViewPager vp_banner;
	private DotView v_dots;
	
	private long clickTime = 0L;
	
	View mUiChat;
	View mUiHongBao;
	View mUiRoomOrder;
	View mUiSinger;
	View mUiTheme;
	View mUiNewSong;
	View mUiRanks;
	View mUiLanguage;
	View mUiCollect;
	
	boolean mIsNeedAddChat = true;
	int mNumChatUnread;
	int mNumHBUnread;
	
	TextView mUiChatUnread;
	TextView mUiHongBaoUnread;
	Lock mLockUnread = new ReentrantLock();
	
	public static List<ResultData_SongList> s_songList = new ArrayList<ResultData_SongList>();
	public static Set<Integer> s_songSet = new HashSet<Integer>();
	
	/* ================ Banner =============== */
	private GetBannerListTask getBannerListTask;
	private ArrayList<Banner> bannerList = new ArrayList<Banner>();
	private BannerAdapter viewpagerAdapter;
	private List<String> images = new ArrayList<String>();
	private final static int FRESH_BANNER_IMAGE = 55500; // 下载横幅图片后更新控件
	private int current = 0, total = 0;
	private final static long CHANGE_BANNER_TIME = 5000; // 2s更新一次
	private boolean isChangeBannerRun = false;
	boolean isBannerDefault = true;
	
	LoadingDialog mDialogLoading;
	Map<String,String> mMapData = new HashMap<String,String>();
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,Bundle savedInstanceState)
	{
		vRoot = inflater.inflate(R.layout.ac_main_home, container, false);
		initView();
		AppStatus.s_NetUtilMgr.registerListener(this);
		return vRoot;
	}
	
	@Override
	public void onDestroyView() {
		// TODO Auto-generated method stub
		AppStatus.s_NetUtilMgr.unregisterListener(this);
		
		mHome.removeIgnoredAboveView(vp_banner);
		super.onDestroyView();
	}
	
	@Override
	public void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		
		//监听聊天信息
		mIsNeedAddChat = true;
	}

	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);
		initData();
		mDialogLoading = new LoadingDialog(getActivity());
	}
	
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		if(requestCode == RequestCodeDef.REQUEST_SCAN_COMMON)
		{
			if(resultCode == Activity.RESULT_OK)
			{
				String scanData = data.getStringExtra(CaptureActivity.EXTRA_RESULT_DATA);
				//String scanData = "scanpay-type:1;ktvid:123456;giftid:123456;giftnum:5;giftname:啤酒;giftunit:瓶;gold:789;";

				mScanType = SCAN_UNKNOWN;
				do
				{
					mMapData.clear();
					String[] strKeyValues = scanData.split(";");
					for(String str : strKeyValues)
					{
						String[] strKeyValue = str.split(":");
						if(strKeyValue == null || strKeyValue.length != 2)
							break ;
						mMapData.put(strKeyValue[0], strKeyValue[1]);
					}
					
					if(mMapData.isEmpty())
						break ;
					
					String payType = mMapData.get("scanpay-type");
					if(TextUtils.equals(payType, "1"))
						mScanType = SCAN_GIFT;
					
				}while(false);
				
				
				switch(mScanType)
				{
					case SCAN_GIFT:
					{
						String content = "确认要兑换"+mMapData.get("giftnum")+mMapData.get("giftunit")
								+mMapData.get("giftname")+"("+mMapData.get("gold")+"金币)?";
						DialogManager.showTipDialog(getActivity()
								,this.getResources().getString(R.string.text_exchangegift) , content, this, true);
						break;
					}
					case SCAN_UNKNOWN:
					default:
					{
						String content = "扫码结果:"+scanData;
						DialogManager.showTipDialog(getActivity()
								,this.getResources().getString(R.string.scan) , content, null, false);
						break;
					}
				}
			}
			return;
		}
		
		super.onActivityResult(requestCode, resultCode, data);
	}

	private void initData()
	{
		// banner
		if (Home.bannerList == null || Home.bannerList.isEmpty())
		{
			if (getBannerListTask != null && getBannerListTask.getStatus() == AsyncTask.Status.RUNNING)
			{
				getBannerListTask.cancel(true);
			}
			getBannerListTask = new GetBannerListTask(mHandler,AppStatus.userIdx, AppStatus.roomId);
			getBannerListTask.execute();
		}
		mHome.baseFragmentHelper = fragmentHelper;
		updateSongList();
	}
	
	void updateSongList()
	{
		s_songList = AppStatus.s_NetUtilMgr.getSonglist();
		s_songSet.clear();
		for(int i=0;i<s_songList.size();i++)
		{
			s_songSet.add(s_songList.get(i).songid);
		}
		
		int number = s_songList.size()-1;
		number = number<0?0:number;
		tv_number.setText(""+number);
	}
	
	private BaseFragmentHelper fragmentHelper = new BaseFragmentHelper()
	{
		@Override
		public boolean onKeyDown(int keyCode, KeyEvent event)
		{
			return false;
		}
	};

	
	protected void initView()
	{
		mHome = (Home) this.getActivity();
		findView();
		setView();
		setListener();
	}
	
	protected void findView()
	{
		// title
		tv_title = (TextView) vRoot.findViewById(R.id.tv_title);
		mUiTip = (TextView) vRoot.findViewById(R.id.tv_online);
		tv_number = (TextView) vRoot.findViewById(R.id.tv_number);
		btn_menu = (ImageButton) vRoot.findViewById(R.id.iv_back);
		btn_order = (ImageButton) vRoot.findViewById(R.id.iv_ordermenu);
		// search
		et_search = (DrawableClickEditText) vRoot.findViewById(R.id.et_search);
		//vRoot.findViewById(R.id.ly_search);
		// banner
		vp_banner = (ViewPager) vRoot.findViewById(R.id.viewpager);
		v_dots = (DotView) vRoot.findViewById(R.id.dots);
		
		
		mUiChat = vRoot.findViewById(R.id.imgbtn_chat);
		mUiHongBao= vRoot.findViewById(R.id.imgbtn_hongbao);
		mUiRoomOrder= vRoot.findViewById(R.id.imgbtn_roomorder);
		mUiSinger= vRoot.findViewById(R.id.imgbtn_singer);
		mUiTheme= vRoot.findViewById(R.id.imgbtn_theme);
		mUiNewSong= vRoot.findViewById(R.id.imgbtn_newsong);
		mUiRanks= vRoot.findViewById(R.id.imgbtn_rankings);
		mUiLanguage= vRoot.findViewById(R.id.imgbtn_language);
		mUiCollect= vRoot.findViewById(R.id.imgbtn_collect);
		
		mUiChatUnread = (TextView) vRoot.findViewById(R.id.tv_chat_unread);
		mUiHongBaoUnread = (TextView) vRoot.findViewById(R.id.tv_hongbao_unread);
	}
	
	protected void setView() {
		// title
		btn_menu.setImageResource(R.drawable.btn_home_menu);
		tv_title.setText(R.string.app_name);
		if (!AppStatus.isInRoom)
		{
			tv_number.setVisibility(View.GONE);
			btn_order.setVisibility(View.GONE);
		}
		// banner
		initBanner(Home.bannerList);
	}
	
	protected void setListener() {
		mUiChat.setOnClickListener(this);
		mUiHongBao.setOnClickListener(this);
		mUiRoomOrder.setOnClickListener(this);
		mUiSinger.setOnClickListener(this);
		mUiTheme.setOnClickListener(this);
		mUiNewSong.setOnClickListener(this);
		mUiRanks.setOnClickListener(this);
		mUiLanguage.setOnClickListener(this);
		mUiCollect.setOnClickListener(this);
		
		btn_menu.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				ToolUtil.closeKeyBoard(mHome);
				mHome.showMenu();
			}
		});

		btn_order.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				if (AppStatus.isInRoom)
				{
					Intent intent = new Intent(mHome, OrderedListActivity.class);
					mHome.startActivity(intent);
					mHome.hidePlayControl();
				}
			}
		});

		et_search.setDrawableClickListener(new DrawableClickListener() {

			@Override
			public void onClick(DrawablePosition target) {
				long time = System.currentTimeMillis();
				if (target == DrawablePosition.RIGHT && time - clickTime > SEARCH_TIME) {
					clickTime = time;
					ToolUtil.closeKeyBoard(mHome);
					search();
				}
			}
		});

		/* 键盘上按回车也能执行搜索 */
		et_search.setOnEditorActionListener(new TextView.OnEditorActionListener() {
			
			@Override
			public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
				// TODO Auto-generated method stub
				//联想手机貌似会脑残的传递 actionId = 0 过来，所以需要去掉这个判断。
				//if(actionId == EditorInfo.IME_ACTION_SEARCH)
				long time = System.currentTimeMillis();
				//搜索间隔时间
				if(time - clickTime > SEARCH_TIME)
				{
					Log.i("MainHomeFragment","Click Search");
					clickTime = time;
					search();
					return true;
				}
				return false;
			}
		});

		et_search.addTextChangedListener(new TextWatcher() {

			@Override
			public void afterTextChanged(Editable s) {
				// MyLog.v(TAG, "afterTextChanged"+s.toString());
				if (s == null || s.length() == 0) {
					ToolUtil.closeKeyBoard(mHome);
				}
			}

			@Override
			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
			}

			@Override
			public void onTextChanged(CharSequence s, int start, int before,
					int count) {
			}
		});
	}
	
	protected boolean search()
	{
		if (!checkNetWork())
			return false;
		
		final String text = et_search.getText().toString();
		if (TextUtils.isEmpty(text))
		{
			ShowUtil.showToast(this.getActivity(),R.string.search_input_empty_tips);
			return false;
		}
		et_search.setText("");
		mHome.hidePlayControl();

		Intent i = new Intent(getActivity(), SongSearch.class);
		i.putExtra("text", text);
		startActivity(i);
		return true;
	}
	
	protected boolean checkNetWork()
	{
		return MobileNetStatUtil.checkCurrentNetState(getActivity());
	}
	
	/** 初始化活动数据 */
	private synchronized void initBanner(ArrayList<Banner> list)
	{
		// 先加载新浪小人图，然后看有没有缓存，有缓存的加载缓存，没缓存的加载另一套默认图
		if (list != null && !list.isEmpty())
		{
			bannerList.clear();
			bannerList.addAll(list);
			images.clear();
			isBannerDefault = false;
			
			try
			{
				for (Banner banner : bannerList)
				{
					images.add(banner.photoUrl);
				}
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}

			if (Home.bannerList == null)
			{
				Home.bannerList = new ArrayList<Banner>();
			}
			if(Home.bannerList.isEmpty())
			{
				Home.bannerList.addAll(bannerList);
			}
		}
		else if(bannerList.isEmpty())
		{
			addDefaultBanner();
		}

		viewpagerAdapter = new BannerAdapter(mHome, bannerList,viewpagerClickMethod, isBannerDefault);
		
		vp_banner.setAdapter(viewpagerAdapter);
		vp_banner.setOnPageChangeListener(new OnPageChangeListener()
		{

			@Override
			public void onPageScrollStateChanged(int arg0)
			{
				// TODO Auto-generated method stub
			}

			@Override
			public void onPageScrolled(int arg0, float arg1, int arg2)
			{
				// TODO Auto-generated method stub
			}

			@Override
			public void onPageSelected(int arg0)
			{
				current = arg0;
				v_dots.setSelection(arg0);
			}
		});

		mHome.addIgnoredAboveView(vp_banner);
		
		total = bannerList.size();
		mHandler.removeCallbacks(bannerChangeRunnable);
		
		if (total > 1)
		{
			// 添加点点
			v_dots.setDotCount(total);
			isChangeBannerRun = true;
			mHandler.removeCallbacks(bannerChangeRunnable);
			mHandler.postDelayed(bannerChangeRunnable, CHANGE_BANNER_TIME);
		}

		ImageUtil.showBannerImage(mHome, mHandler, images, FRESH_BANNER_IMAGE,false, 1);
	}
	
	/** 主要Handler，不显示提示 */
	@SuppressLint("HandlerLeak") 
	private Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			try {
				int nCommand = msg.what;
				switch (nCommand) {
				case MessageDef.HTTP_GIFTEXCHANGE:
				{
					mDialogLoading.dismiss();
					
					if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.Nothing)
					{
						// 提示兑换成功
						ShowUtil.showToast(getActivity(), R.string.pc_duihuan_seccess);
						
						Intent intent = new Intent(getActivity(), GiftLogActivity.class);
						startActivity(intent);
						
						mHome.updateGold(msg.arg2);
					}
					else if(msg.arg1 == KtvAssistantAPIConfig.APIErrorCode.GoldNotEnough)
					{
						DialogManager.showTopUpDialog(getActivity(),new OnDialogListener(){

							@Override
							public void onOk(int type) {
								// TODO Auto-generated method stub
								mHome.startActivity(new Intent(mHome,ChargeActivity.class));
							}

							@Override
							public void onCancel() {
								// TODO Auto-generated method stub
								
							}
							
						});
					}
					else
						ShowUtil.showToast(getActivity(), "兑换失败:" + (String)msg.obj);
					break;
				}
				/* 获得banner列表 */
				case MessageDef.WM_GET_BANNERLIST:
					@SuppressWarnings("unchecked")
					ArrayList<Banner> list = (ArrayList<Banner>) msg.obj;
					initBanner(list);
					break;

				/* 刷新banner图片 */
				case FRESH_BANNER_IMAGE:
					if (viewpagerAdapter == null || bannerList.size() <= 0)
						return;
					viewpagerAdapter.notifyDataSetChanged();
					break;

				/* 错误码显示 ui线程 */
				default:
					break;
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	};
	
	private Runnable bannerChangeRunnable = new Runnable() {

		@Override
		public void run() {
			if (current >= 0 && current < total - 1) {
				current++;
			} else {
				current = 0;
			}
			vp_banner.setCurrentItem(current);

			if (isChangeBannerRun)
				mHandler.postDelayed(this, CHANGE_BANNER_TIME);
		}

	};
	
	private BannerClickMethod viewpagerClickMethod = new BannerClickMethod() {

		@Override
		public void click(ImageView img, Banner info) {
			
			if (info == null || info.bannertype <= 0)
				return;
			img.setTag(info);
			img.setOnClickListener(new View.OnClickListener() {
				
				@Override
				public void onClick(View v) {
					Banner info = (Banner) v.getTag();
					clickBanner(info);
				}
			});

		}

	};
	
	/** 点击横幅 */
	private void clickBanner(Banner info) {
		if (info == null || info.bannertype < 0)
			return;

		if (info.bannertype == BannerType.NORMAL_SONGS) 
		{ // 跳点歌列表
			if (info.songparameter != null) {
				Intent i = new Intent(mHome, OrderTypeSongs.class);
				i.putExtra(OrderActivity.BUNDLE_MODELID, info.songparameter.modelId);
				TopicInfo topic = new TopicInfo(info.songparameter.topicId,
						info.title);
				i.putExtra(OrderActivity.BUNDLE_CURTOPIC, topic);
				startActivity(i);
			}
		} 
		else if (info.bannertype == BannerType.CHART_SONGS) 
		{
			if (info.songparameter != null) {
				Intent i = new Intent(mHome, OrderTypeTopSongs.class);
				i.putExtra(OrderActivity.BUNDLE_MODELID, info.songparameter.modelId);
				TopicInfo topic = new TopicInfo(info.songparameter.topicId,
						info.title);
				i.putExtra(OrderActivity.BUNDLE_CURTOPIC, topic);
				startActivity(i);
			}
		}
		else if(info.bannertype == BannerType.URL_ACTIVITY)
		{
			if(!TextUtils.isEmpty(info.activityUrl))
			{
				Intent i = new Intent(mHome,CommonBrowserActivity.class);
				i.putExtra(CommonBrowserActivity.EXTRA_TITLE, info.title);
				i.putExtra(CommonBrowserActivity.EXTRA_URL, info.activityUrl);
				i.putExtra(CommonBrowserActivity.EXTRA_ACTIVITY, true);
				i.putExtra(CommonBrowserActivity.EXTRA_ACTIVITY_PIC,info.photoUrl);
				startActivity(i);
			}
		}
		else
			return ;
		
		mHome.hidePlayControl();
	}
	
	private void addDefaultBanner()
	{
		if(!bannerList.isEmpty())
		{
			return;
		}

		Banner item1 = new Banner(0, null, "我是歌手第二季", 1, 7, 1);
		bannerList.add(item1);

	}
	
	void changeHBUnread(int num){
		if(num > 99)
		{
			mUiHongBaoUnread.setText("99+");
			mUiHongBaoUnread.setVisibility(View.VISIBLE);
		}
		else if(num > 0)
		{
			mUiHongBaoUnread.setText(""+num);
			mUiHongBaoUnread.setVisibility(View.VISIBLE);
		}
		else
			mUiHongBaoUnread.setVisibility(View.GONE);
	}
	
	void changeChatUnread(int num){
		if(num > 99)
		{
			mUiChatUnread.setText("99+");
			mUiChatUnread.setVisibility(View.VISIBLE);
		}
		else if(num > 0)
		{
			mUiChatUnread.setText(""+num);
			mUiChatUnread.setVisibility(View.VISIBLE);
		}
		else
			mUiChatUnread.setVisibility(View.GONE);
	}
	
	@Override
	public void refreshView() {
		super.refreshView();

		if (AppStatus.isInRoom) 
		{
			updateSongList();
			tv_number.setVisibility(View.VISIBLE);
			btn_order.setVisibility(View.VISIBLE);
		} 
		else 
		{
			tv_number.setVisibility(View.GONE);
			btn_order.setVisibility(View.GONE);
		}
	}

	@Override
	public void onClick(View v) {
		
		mHome.hidePlayControl();
		
		switch (v.getId()) 
		{
			//弹幕互动
			case R.id.imgbtn_chat:
			{
				Intent i = new Intent(getActivity(), ChatActivity.class);
				startActivity(i);
				mLockUnread.lock();
				mIsNeedAddChat = false;
				mNumChatUnread = 0;
				changeChatUnread(0);
				mLockUnread.unlock();
				break;
			}
			//红包
			case R.id.imgbtn_hongbao:
			{
				Intent i = new Intent(getActivity(),HongBListActivity.class);
				startActivity(i);
				mLockUnread.lock();
				mNumHBUnread = 0;
				changeHBUnread(mNumHBUnread);
				mLockUnread.unlock();
				break;
			}
			//包厢预订
			case R.id.imgbtn_roomorder:
			{
				Intent i = new Intent(getActivity(), RoomOrderActivity.class);
				startActivity(i);
				break;
			}
			//歌手
			case R.id.imgbtn_singer:
			{
				Intent i = new Intent(mHome, OrderTypeMenu.class);
				i.putExtra(OrderActivity.BUNDLE_MODELID, HomeAdapter.ModelType.SINGERS.getType());
				startActivity(i);
				break;
			}
			//主题
			case R.id.imgbtn_theme:
			{
				Intent i = new Intent(mHome, OrderTypeMenu.class);
				i.putExtra(OrderActivity.BUNDLE_MODELID, HomeAdapter.ModelType.THEME.getType());
				startActivity(i);
				break;
			}
			//新曲
			case R.id.imgbtn_newsong:
			{
				Intent i = new Intent(getActivity(), OrderTypeSongs.class);
				i.putExtra(OrderActivity.BUNDLE_MODELID, HomeAdapter.ModelType.NEWSONG.getType());
				TopicInfo info = new TopicInfo(0,getString(R.string.text_newsong));
				i.putExtra(OrderActivity.BUNDLE_CURTOPIC, info);
				startActivity(i);
				break;
			}
			//排行榜
			case R.id.imgbtn_rankings:
			{
				Intent i = new Intent(mHome, OrderTypeRanks.class);
				i.putExtra(OrderActivity.BUNDLE_MODELID, HomeAdapter.ModelType.RANKLIST.getType());
				startActivity(i);
				break;
			}
			//语种
			case R.id.imgbtn_language:
			{
				Intent i = new Intent(mHome, OrderTypeMenu.class);
				i.putExtra(OrderActivity.BUNDLE_MODELID, HomeAdapter.ModelType.LANGUAGE.getType());
				startActivity(i);
				break;
			}
			//扫码
			case R.id.imgbtn_collect:
			{
				Intent i = new Intent(mHome,CaptureActivity.class);
				startActivityForResult(i,RequestCodeDef.REQUEST_SCAN_COMMON);
				break;
			}
		}
	}
	
	@Override
	public void onOk(int type) {
		// TODO Auto-generated method stub
		if(mScanType == SCAN_GIFT)
		{
			mHandler.postDelayed(new Runnable(){

				@Override
				public void run() {
					// TODO Auto-generated method stub
					mDialogLoading.show();
					
					GiftExchangeTask task = new GiftExchangeTask(mHandler);
					
					String tempStr = mMapData.get("ktvid");
					int ktvid = Integer.parseInt(tempStr==null?"":tempStr);
					tempStr = mMapData.get("giftid");
					int giftid = Integer.parseInt(tempStr==null?"":tempStr);
					tempStr = mMapData.get("giftnum");
					int giftnum = Integer.parseInt(tempStr==null?"":tempStr);
					task.setArgu( ktvid, giftid, giftnum);
					task.execute();
				}
				
			}, 100);
		}
	}

	@Override
	public void onCancel() {
		// TODO Auto-generated method stub
		
	}
	
	@Override
	public void dealWithMsg(Message msg)
	{
		// TODO Auto-generated method stub
		switch(msg.what)
		{
		//重连房间成功
		case XGMessage.XGMSG_ROOMENTER_SUCCESS:
		{
			mUiTip.setVisibility(View.GONE);
			break;
		}
		//房间断开连接
		case XGMessage.XGMSG_ROOMCON_ERROR:
		{
			mUiTip.setVisibility(View.VISIBLE);
			mUiTip.setText(R.string.text_room_reconnect);
			break;
		}
		//通知播放列表更新了
		case XGMessage.XGMSG_CUR_SONGLIST:
		{
			updateSongList();
			break;
		}
		case XGMessage.XGMSG_CHAT_NOTIFY:
		case XGMessage.XGMSG_GIFT_NOTIFY:
		case XGMessage.XGMSG_HORN_NOTIFY:
		case XGMessage.XGMSG_REWARD_NOTIFY:
		case XGMessage.XGMSG_USERINFO_FORHORN:
		case XGMessage.XGMSG_HONGBAO_RECV:
		case XGMessage.XGMSG_HONGBAO_ROOM:
			
			mLockUnread.lock();
			if(mIsNeedAddChat)
			{
				if(msg.what == XGMessage.XGMSG_HONGBAO_ROOM)
					mNumChatUnread += msg.arg2;
				else
					mNumChatUnread ++;
				changeChatUnread(mNumChatUnread);
			}
			
			if(msg.what == XGMessage.XGMSG_HONGBAO_RECV)
			{
				HongBaoInfoToUi info = (HongBaoInfoToUi) msg.obj;
				if(info.hbtype == 0 && info.idx != AppStatus.s_userMy.m_idx)//如果不是我发送的红包
				{
					mNumHBUnread ++;
					changeHBUnread(mNumHBUnread);
				}
			}
			mLockUnread.unlock();
			
			break;
		}
	}
}