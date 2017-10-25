package assistant.manager;

/***
 * @author GLP
 */

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import tiange.ktv.assistant.R;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import assistant.activity.Home;
import assistant.activity.LoginActivity;
import assistant.entity.ChatDisplayInfo;
import assistant.entity.ChatInfo;
import assistant.entity.GiftDisplayInfo;
import assistant.entity.HornDisplayInfo;
import assistant.entity.RewardInfo;
import assistant.global.AppStatus;
import assistant.global.XGConfig;
import assistant.notify.NormalNotify;
import assistant.util.MobileNetStatUtil;
import assistant.util.ShowUtil;
import assistant.util.ToolUtil;

import com.jni.netutil.CJniUtil;
import com.jni.netutil.GiftInfo;
import com.jni.netutil.JcBridgeResultData_Channel;
import com.jni.netutil.JniMessage;
import com.jni.netutil.LocalData_UserInfo;
import com.jni.netutil.RequestData_IdxWithVersion;
import com.jni.netutil.ResultData_GetChannellist;
import com.jni.netutil.ResultData_RoomHB;
import com.jni.netutil.ResultData_RoomInfo;
import com.jni.netutil.ResultData_SelfData;
import com.jni.netutil.ResultData_SendFlowerInfo;
import com.jni.netutil.ResultData_SongList;
import com.jni.netutil.ResultData_UIdxWithType;
import com.jni.netutil.ResultData_UserSampleInfo;
import com.jni.netutil.XGMessage;
import com.jni.netutil.out.ChatInfoToUi;
import com.jni.netutil.out.HongBaoDetailInfoToUi;
import com.jni.netutil.out.HongBaoInfoToUi;
import com.jni.netutil.out.HornInfoToUi;
import com.jni.netutil.out.RewardInfoToUi;

public class NetUtilManager extends Handler{
	static final String TAG = "NetUtilManager";
	static final int MAX_DISPLAYLIST = 1000;
	static final int MAX_INDEX_DISPLAYLIST = 5;
	
	static final String ROOM_PWD = "888888";
	
	public static boolean IsLoginLobbyAfterList = true;
	//登陆大厅
	public static final int TransactionLobbyLogin = 1;
	//大厅重连，会需要重连房间
	public static final int TransactionLobbyLoginAgain = 2;
	//进入指定房间,如果大厅还未连接，则会先连接大厅
	//@注:房间的transactionObj必须为null
	public static final int TransactionRoomLogin = 3;
	//重新获取认证信息
	public static final int TransactionAuthAgain = 4;
	
	Context m_context;
	
	@SuppressLint("UseSparseArrays") 
	Map<Integer,JcBridgeResultData_Channel> 	m_mapChannellist = new HashMap<Integer,JcBridgeResultData_Channel>();
	//事务处理,同时只处理单个事务
	public class TransactionNet{
		public TransactionNet(){}
		public TransactionNet(TransactionNet transactionNet) {
			// TODO Auto-generated constructor stub
			transactionToDo = transactionNet.transactionToDo;
			transactionObj = transactionNet.transactionObj;
			transactionListener = transactionNet.transactionListener;
		}
		public int transactionToDo = 0;
		public Object transactionObj = null;
		public IOnTransactionListener transactionListener = null;
	}
	TransactionNet m_transactionNet = new TransactionNet();
	boolean m_giftDataFinished = false;
	//互斥锁
	Lock m_lockListener = new ReentrantLock(); 
	ArrayList<INetUtilListener> m_NetUtilListenerList = new ArrayList<INetUtilListener>();
	Lock m_lockDisplay = new ReentrantLock(); 
	List<ChatDisplayInfo> m_chatDisplay = new ArrayList<ChatDisplayInfo>();
	Lock m_lockUser = new ReentrantLock();
	
	Lock m_lockIndexDisplay = new ReentrantLock();
	boolean m_isNeedRequestFiveHorn = true;
	List<ChatDisplayInfo> m_indexDisplay = new ArrayList<ChatDisplayInfo>();
	
	public ChatDisplayInfo m_hornTopDisplay = null;
	
	@SuppressLint("UseSparseArrays")
	Map<Integer,Integer> m_userOnlineType = new HashMap<Integer,Integer>();
	//ktv 手机在线用户列表(没有包含自己)
	@SuppressLint("UseSparseArrays")
	Map<Integer,LocalData_UserInfo> m_userMap = new HashMap<Integer,LocalData_UserInfo>();
	@SuppressLint("UseSparseArrays") 
	Map<Integer,ResultData_UserSampleInfo> m_userHornMap = new HashMap<Integer,ResultData_UserSampleInfo>();
	Lock m_lockCurSonglist = new ReentrantLock();
	List<ResultData_SongList> m_songList = new ArrayList<ResultData_SongList>();
	Set<String> mSetRoomHB = new HashSet<String>();
	public CJniUtil m_JniUtil = null;
	
	private NetUtilManager(Context context){
		m_context = context.getApplicationContext();
		m_JniUtil = new CJniUtil();
		m_JniUtil.initJniUtil(this);
	}
	
	static NetUtilManager s_mgr;
	public static NetUtilManager getInstance(Context context){
		if(s_mgr == null)
			s_mgr = new NetUtilManager(context);
		return s_mgr;
	}
	
	public static String GetHeadUrl(LocalData_UserInfo userInfo){
		String headUrl = "";
		if(userInfo == null)
			return headUrl;
		return GetHeadUrl(userInfo.headurl);
	}
	public static String GetHeadUrl(String headUrl){
		if(headUrl.indexOf("http") == -1)//自己上传的头像
			return (AppStatus.user.xgDomain+headUrl);
		else//第三方头像
			return headUrl;
	}
	
	private void updateSonglist(ResultData_SongList[] aList){
		m_lockCurSonglist.lock();
		m_songList.clear();
		if(aList != null)
		{
			for(int i=0;i<aList.length;i++)
				m_songList.add(aList[i]);
		}
		m_lockCurSonglist.unlock();
	}
	public List<ResultData_SongList> getSonglist(){
		m_lockCurSonglist.lock();
		List<ResultData_SongList> list = new ArrayList<ResultData_SongList>();
		list.addAll(m_songList);
		m_lockCurSonglist.unlock();
		return list;
	}
	
	private void addUserInfo(int idx){
		if(idx == 0 || idx==(int)AppStatus.userIdx)
			return ;
		
		LocalData_UserInfo user = m_JniUtil.GetUserInfoFromLocal(idx);
		if(user != null)
		{
			m_lockUser.lock();
			user.online_type = 3;//默认大厅
			Integer type = 3;
			if((type = m_userOnlineType.get(user.m_idx)) != null)
				user.online_type = type;
			user.headurl = GetHeadUrl(user);
			m_userMap.put(idx, user);//如果已存在，会替换
			m_lockUser.unlock();
		}
		
	}
	private void subUserInfo(int idx){
		if(idx == 0)
			return ;
		m_lockUser.lock();
		m_userOnlineType.remove(idx);
		m_userMap.remove(idx);
		m_lockUser.unlock();
	}
	private void changeUserPic(int idx,String pic){
		if(idx==0 || pic==null)
			return ;
		
		if(idx==(int)AppStatus.userIdx && AppStatus.s_userMy!=null)
		{
			AppStatus.s_userMy.headurl = GetHeadUrl(pic);
			AppStatus.user.headphoto = AppStatus.s_userMy.headurl;
		}
		else
		{
			m_lockUser.lock();
			if(m_userMap.containsKey(idx))
				m_userMap.get(idx).headurl = GetHeadUrl(pic);
			m_lockUser.unlock();
		}
	}
	private void changeUserName(int idx,String name){
		if(idx==0 || name==null)
			return ;
		
		if(idx==(int)AppStatus.userIdx && AppStatus.s_userMy!=null)
		{
			AppStatus.s_userMy.m_name = name;
			AppStatus.user.nickname = name;
		}
		else
		{
			m_lockUser.lock();
			if(m_userMap.containsKey(idx))
				m_userMap.get(idx).m_name = name;
			m_lockUser.unlock();
		}
	}
	public List<LocalData_UserInfo> getUserInfoList(){
		List<LocalData_UserInfo> list = new ArrayList<LocalData_UserInfo>();
		m_lockUser.lock();
		list.addAll(m_userMap.values());
		m_lockUser.unlock();
		return list;
	}
	@SuppressLint("UseSparseArrays")
	public Map<Integer,LocalData_UserInfo> getUserInfoMap(){
		m_lockUser.lock();
		Map<Integer,LocalData_UserInfo> map = new HashMap<Integer,LocalData_UserInfo>();
		map.putAll(m_userMap);
		m_lockUser.unlock();
		return map;
	}
	
	void addUserInfoForHorn(ResultData_UserSampleInfo info){
		if(info!=null)
		{
			info.headurl = GetHeadUrl(info.headurl);
			m_userHornMap.put(info.idx, info);
		}
	}
	public ResultData_UserSampleInfo getUserInfoForHorn(int idx){
		if(m_userHornMap.containsKey(idx))
			return m_userHornMap.get(idx);
		return null;
	}
	
	private void registerIndexDisplay(ChatDisplayInfo info){
//		if(info != null)
//		{
//			m_lockIndexDisplay.lock();
//			m_indexDisplay.add(info);
//			if(m_indexDisplay.size() > MAX_INDEX_DISPLAYLIST)
//				m_indexDisplay.remove(0);
//			m_lockIndexDisplay.unlock();
//		}
	}
	//获取首页需要显示的信息列表
	public List<ChatDisplayInfo> getIndexDisplayList(){
		
		m_lockIndexDisplay.lock();
		List<ChatDisplayInfo> list = new ArrayList<ChatDisplayInfo>();
		list.addAll(m_indexDisplay);
		m_lockIndexDisplay.unlock();
		
		return list;
	}
	private boolean registerDisplay(ChatDisplayInfo info){
		if(info != null)
		{
			m_lockDisplay.lock();
			
			boolean isNeedAdd = true;
			if(info.type == ChatDisplayInfo.ChatRedPacket)
			{
				if(mSetRoomHB.contains(info.hongbao.hbid))
					isNeedAdd = false;
				else//否则添加到红包Set中，防止重复红包出现
					mSetRoomHB.add(info.hongbao.hbid);
			}
			
			if(isNeedAdd)
			{
				
				m_chatDisplay.add(info);
				if(m_chatDisplay.size() > NetUtilManager.MAX_DISPLAYLIST)
				{
					ChatDisplayInfo deleteInfo = m_chatDisplay.get(0);
					if(deleteInfo.type == ChatDisplayInfo.ChatRedPacket)
					{//看下是否在房间红包Set中，如果有，则删掉
						mSetRoomHB.remove(deleteInfo.hongbao.hbid);
					}
					m_chatDisplay.remove(0);//去掉第一个
				}
			}
			
			m_lockDisplay.unlock();
			return isNeedAdd;
		}
		return false;
	}
	//获取聊天需要显示的内容信息
	public List<ChatDisplayInfo> getDisplayList(){
		
		m_lockDisplay.lock();
		List<ChatDisplayInfo> list = new ArrayList<ChatDisplayInfo>();
		list.addAll(m_chatDisplay);
		m_lockDisplay.unlock();
		
		return list;
	}
	
	public void registerListener(INetUtilListener listener)
	{
		if(listener == null)
			return ;
		m_lockListener.lock();
		m_NetUtilListenerList.add(listener);
		m_lockListener.unlock();
	}
	public void unregisterListener(INetUtilListener listener)
	{
		if(listener == null)
			return ;
		m_lockListener.lock();
		m_NetUtilListenerList.remove(listener);
		m_lockListener.unlock();
	}
	
	private void beginLobbyTransaction(){
		if(0 != AppStatus.s_NetUtilMgr.m_JniUtil.ConnectAuth(XGConfig.tokenhost, XGConfig.tokenport, 60))
		{
			showToast(R.string.authcon_failed);
			onTransactionFinish(false);
			return ;
		}
		Log.i(TAG,"连接认证中...");
	}
	private void beginRoomTransaction(){
		//没有指定房间ID
		if(AppStatus.s_nRoomId == 0 || !AppStatus.s_isNeedConnectedRoom)
		{
			//无需进入星光房间
			Log.i(TAG,"no need to enter room");
			onTransactionFinish(true);
			return;
		}
		//进入指定房间->KTV包厢房间
		ResultData_RoomInfo roomInfo = AppStatus.s_NetUtilMgr.m_JniUtil.GetRoomInfoFromLocal(AppStatus.s_nRoomId);
		if(roomInfo == null)
		{
			Log.e(TAG, "room info get failed");
			onTransactionFinish(false);
			return ;
		}
		
		int ret;
		if(!AppStatus.s_bWangTong)
			ret = AppStatus.s_NetUtilMgr.m_JniUtil.ConnectRoom(roomInfo.ip_1,roomInfo.port,XGConfig.ROOMTYPE_BASE,60);
		else
			ret = AppStatus.s_NetUtilMgr.m_JniUtil.ConnectRoom(roomInfo.ip_2, roomInfo.port, XGConfig.ROOMTYPE_BASE, 60);
		if(ret == 0)
			Log.i(TAG,"房间连接中...");
		else
		{
			showToast(R.string.roomconnect_failed);
			onTransactionFinish(false);
		}
	}
	
	public interface IOnTransactionListener{
		public void onTransactionComplete(Object obj);
		public void onTransactionFailed(Object obj);
	}
	
	private synchronized void resetWork(){
		m_transactionNet.transactionToDo = 0;
		m_transactionNet.transactionObj = null;
		m_transactionNet.transactionListener = null;
	}
	private synchronized void reconnect(final boolean isLobby){
		//先判断是否是网络突然不好导致的连接断开
		if(m_transactionNet.transactionToDo == 0)
		{
			this.postDelayed(new Runnable(){

				@Override
				public void run() {
					// TODO Auto-generated method stub
					if(isLobby)
					{
						Log.i(TAG,"大厅重连...");
						NetUtilManager.this.doCurrentWork(TransactionLobbyLoginAgain);
					}
					else
					{
						Log.i(TAG,"房间重连...");
						//NetUtilManager.this.doCurrentWork(TransactionLobbyLoginAgain);
						NetUtilManager.this.doCurrentWork(TransactionRoomLogin);
					}
				}
				
			}, 10000);//10秒之后短线重连
		}
	}
	
	void onTransactionFinish(boolean complete){
		onTransactionFinish(complete,true);
	}
	
	void onTransactionFinish(boolean complete,boolean reconnect){
		
		if(m_transactionNet.transactionToDo == 0)
			return ;
		
		if(complete)
		{	
			//如果连接大厅成功
			if(m_transactionNet.transactionToDo == TransactionLobbyLogin)
			{
				if(m_isNeedRequestFiveHorn)
				{
					m_JniUtil.GetUserSpeakerInfo();
					m_isNeedRequestFiveHorn = false;
				}
			}
		}
		else if(reconnect)
		{
			//如果连接大厅，或者重连大厅失败
			if(m_transactionNet.transactionToDo == TransactionLobbyLogin
					|| m_transactionNet.transactionToDo == TransactionLobbyLoginAgain)
			{
				final TransactionNet transaction = new TransactionNet(m_transactionNet);
				NetUtilManager.this.postDelayed(new Runnable(){

					@Override
					public void run() {
						// TODO Auto-generated method stub
						Log.i(TAG,"大厅重连...");
						NetUtilManager.this.doCurrentWork(transaction.transactionToDo
								, transaction.transactionListener, transaction.transactionObj);
					}
					
				}, 5000);
			}
			else if(m_transactionNet.transactionToDo == TransactionRoomLogin 
					&& AppStatus.s_isNeedConnectedRoom)
			{
				final TransactionNet transaction = new TransactionNet(m_transactionNet);
				NetUtilManager.this.postDelayed(new Runnable(){

					@Override
					public void run() {
						// TODO Auto-generated method stub
						Log.i(TAG,"房间重连...");
						NetUtilManager.this.doCurrentWork(transaction.transactionToDo
								, transaction.transactionListener, transaction.transactionObj);
					}
					
				}, 1000);
			}
			else if(m_transactionNet.transactionToDo == TransactionAuthAgain)
			{
				final TransactionNet transaction = new TransactionNet(m_transactionNet);
				NetUtilManager.this.postDelayed(new Runnable(){

					@Override
					public void run() {
						// TODO Auto-generated method stub
						Log.i(TAG,"令牌重新获取...");
						NetUtilManager.this.doCurrentWork(transaction.transactionToDo
								, transaction.transactionListener, transaction.transactionObj);
					}
					
				}, 1000);
			}
		}
		//保存事务处理的监听
		IOnTransactionListener listener = m_transactionNet.transactionListener;
		Object obj = m_transactionNet.transactionObj;
		//把状态置为空闲
		resetWork();
		
		//不需要重连，则需要回调到监听者
		if(!reconnect)
		{
			if(listener != null)
			{
				if(complete)
					listener.onTransactionComplete(obj);
				else
					listener.onTransactionFailed(obj);
			}
		}
	}
	public void doCurrentWork(int transaction){
		doCurrentWork(transaction,null);
	}
	public void doCurrentWork(int transaction,IOnTransactionListener listener){
		doCurrentWork(transaction,listener,null);
	}
	public synchronized void doCurrentWork(int transaction,IOnTransactionListener listener,Object obj){
		int netStat = MobileNetStatUtil.getCurrentNetState(m_context,true);
		//如果正在处理事务
		if(m_transactionNet.transactionToDo != 0)
		{
			if(listener != null)
				listener.onTransactionFailed(obj);
			return ;
		}
		
		m_transactionNet.transactionToDo = transaction;
		m_transactionNet.transactionObj = obj;
		m_transactionNet.transactionListener = listener;
		
		if(netStat == MobileNetStatUtil.NET_NOTCONNECT )
		{
			onTransactionFinish(false);
			return ;
		}
		
		switch(m_transactionNet.transactionToDo)
		{
		case TransactionLobbyLogin:
		case TransactionLobbyLoginAgain:
			//->连接认证服务器
			AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectRoom();
			AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectLobby();
			AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectAuth();
			AppStatus.s_isConnectedLobby = false;
			beginLobbyTransaction();
			break;
		case TransactionRoomLogin:
			//如果大厅已经连接，则直接开始登陆房间的事务，如果没有则从登陆大厅的事务开始
			if(AppStatus.s_isConnectedLobby)
			{
				AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectRoom();
				beginRoomTransaction();
			}
			else
			{
				AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectLobby();
				AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectAuth();
				beginLobbyTransaction();
			}
			break;
		case TransactionAuthAgain:
			AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectAuth();
			beginLobbyTransaction();
			break;
		default:
			resetWork();
			break;
		}
	}
    void showToast(int id){
    	ShowUtil.showToast(m_context, id);
    }
    void showToast(String msg){
    	ShowUtil.showToast(m_context, msg);
    }
    public void idxAuth(int again){
		//->认证
		String md5 = ToolUtil.md5(AppStatus.user.xgPwd);
		Log.i("Glp","xgid:"+AppStatus.user.xgId+",xgpwd:"+AppStatus.user.xgPwd+",md5:"+md5);
		if(0 != m_JniUtil.IdPassAuth(AppStatus.user.xgId, md5,again))
			onTransactionFinish(false);
	}
	public void enterRoom(){
		//"888888"密码进入包厢房间,如果以后需要修改密码，请在此添加
		if(0 != AppStatus.s_NetUtilMgr.m_JniUtil.EnterRoom(AppStatus.s_nRoomId
				, AppStatus.s_nRoomId, ROOM_PWD, (short)0)){
			showToast(R.string.roomenter_failed);
			AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectRoom();
			onTransactionFinish(false);
		}
		Log.i(TAG,"房间进入中...");
	}
	
	@Override
	public void  handleMessage (Message msg){
		String strError;
		switch(msg.what)
		{
		case XGMessage.XGMSG_AUTHCON_TIMEOUT:
			showToast(R.string.authcon_timeout);
			onTransactionFinish(false);
			break;
		case XGMessage.XGMSG_AUTHCON_CLOSE:
			showToast(R.string.authcon_close);
			onTransactionFinish(false,false);
			break;
		case XGMessage.XGMSG_AUTHCON_ERROR:
			strError = m_context.getString(R.string.authcon_error);
			showToast(strError+":"+msg.arg1);
			onTransactionFinish(false,false);
			break;
		case XGMessage.XGMSG_AUTHCON_SUCCESS:
			Log.i(TAG,"认证中...");
			//认证
			idxAuth(m_transactionNet.transactionToDo == TransactionAuthAgain?1:0);
			break;
		case XGMessage.XGMSG_AUTHCON_FAILED:
			strError = m_context.getString(R.string.authcon_failed);
			showToast(strError+":"+msg.arg1);
			onTransactionFinish(false);
			break;
		case XGMessage.XGMSG_AUTHLOGIN_SUCCESS:
			Log.i(TAG,"认证成功");
			//->关闭认证服务器连接 短连接
			AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectAuth();
			
			if(m_transactionNet.transactionToDo == TransactionAuthAgain)
			{
				onTransactionFinish(true);//事务已经处理完毕
			}
			else
			{
				//如果是连接大厅事务，则还需要连接大厅服务器
				if(AppStatus.s_NetUtilMgr.m_JniUtil.ConnectLobby(XGConfig.lobbyhost, XGConfig.lobbyport, 60) != 0)
				{
					showToast(R.string.lobbyconnect_failed);
					onTransactionFinish(false);
				}
				else
					Log.i(TAG,"连接大厅中...");
			}
			break;
		case XGMessage.XGMSG_AUTHLOGIN_FAILED:
		{
			switch(msg.arg1)
			{
			case JniMessage.AUTH_NOTEXIST:
				showToast(R.string.AUTH_NOTEXIST);
				break;
			case JniMessage.AUTH_PWDERROR:
				showToast(R.string.AUTH_PWDERROR);
				break;
			case JniMessage.AUTH_TOURISTERROR:
				showToast(R.string.AUTH_TOURISTERROR);
				break;
			case JniMessage.AUTH_CACHEERROR:
				showToast(R.string.AUTH_CACHEERROR);
				break;
			case JniMessage.AUTH_TOKENERROR:
				showToast(R.string.AUTH_TOKENERROR);
				break;
			}
			//->关闭认证服务器连接
			AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectAuth();
			
			if(JniMessage.OPTION_TIMEOUT == msg.arg1)
				onTransactionFinish(false,false);
			else
			{
				//认证返回失败,清空当前任务，所以需要退到登陆界面
				resetWork();
				
				Intent intent = new Intent(m_context,LoginActivity.class);
				intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
				AppStatus.backToLogin();
				m_context.startActivity(intent);
			}
			break;
		}
		case XGMessage.XGMSG_LOBBYCON_SUCCESS:
			AppStatus.s_isConnectedLobby = true;
			Log.i(TAG,"大厅连接成功");
			
			if(0 !=AppStatus.s_NetUtilMgr.m_JniUtil.GetChannellist(XGConfig.version, AppStatus.s_sMacAddr))
			{
				showToast(R.string.lgetchannellist_failed);
				onTransactionFinish(false);
			}
			Log.i(TAG,"获取大厅列表中...");
			break;
		case XGMessage.XGMSG_LOBBYCON_FAILED:
			AppStatus.s_isConnectedLobby = false;
			strError = m_context.getString(R.string.lobbyconnect_failed);
			showToast(strError+":"+msg.arg1);
			onTransactionFinish(false);
			break;
		case XGMessage.XGMSG_LOBBYCON_TIMEOUT:
			AppStatus.s_isConnectedLobby = false;
			showToast(R.string.lobbyconnect_timeout);
			onTransactionFinish(false);
			break;
		case XGMessage.XGMSG_LOBBYCON_CLOSE:
		{
			AppStatus.s_isConnectedLobby = false;
			Home.enterRoomManager.startExitRoom(true);
			showToast(R.string.lobbyconnect_close);
			
			//可能是账号不同客户端登陆，所以需要退到登陆界面
			Intent intent = new Intent(m_context,LoginActivity.class);
			intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			AppStatus.backToLogin();
			m_context.startActivity(intent);
			break;
		}
		case XGMessage.XGMSG_LOBBYCON_ERROR:
			AppStatus.s_isConnectedLobby = false;
			strError = m_context.getString(R.string.lobbyconnect_error);
			showToast(strError+":"+msg.arg1);
			reconnect(true);
			onTransactionFinish(false);
			break;
		case XGMessage.XGMSG_LOBBYSECTION_SUCCESS://获取大厅列表成功
			Log.i(TAG,"大厅列表获取成功");
			
			m_mapChannellist.clear();
			ResultData_GetChannellist rd_channellist = (ResultData_GetChannellist)msg.obj;
			for(int i=0;i<rd_channellist.count;i++)
			{
				//重复的会被覆盖
				JcBridgeResultData_Channel tmp = new JcBridgeResultData_Channel();
				tmp.jConverFromC(rd_channellist.Ch[i]);
				m_mapChannellist.put(tmp.lobbyid,tmp);
			}
			//*********************登入大厅******************************
			//只允许存在一个大厅列表，并且该大厅的id为1 ，目前是这样的。
			//注:以后如果有多个大厅需要再修改代码,现在只是简单粗暴
			if(m_mapChannellist.size()==1 
					&& m_mapChannellist.get(AppStatus.LobbyId)!=null )
			{
				Log.i(TAG,"正在进入大厅。。");
				//登陆大厅
				AppStatus.s_NetUtilMgr.m_JniUtil.LoginLobby(AppStatus.LobbyId,AppStatus.s_sMacAddr,1);
			}
			else
			{
				showToast(R.string.getchannellist_error);
				IsLoginLobbyAfterList = false;
				AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectLobby();
				onTransactionFinish(false);
			}
			break;
		case XGMessage.XGMSG_LOBBYLOGIN_FAILED:
			AppStatus.s_isConnectedLobby = false;
			switch(msg.arg1)
			{
			case JniMessage.LOBBY_LOGIN_IDLOCK:
				showToast(R.string.LOBBY_LOGIN_IDLOCK);
				AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectLobby();
				onTransactionFinish(false,false);
				break;
			case JniMessage.LOBBY_LOGIN_TOOOLD:
				showToast(R.string.LOBBY_LOGIN_TOOOLD);
				AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectLobby();
				onTransactionFinish(false,false);
				break;
			case JniMessage.LOBBY_LOGIN_NOTEXIST:
				showToast(R.string.LOBBY_LOGIN_NOTEXIST);
				AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectLobby();
				onTransactionFinish(false,false);
				break;
			case JniMessage.LOBBY_LOGIN_NOAVATAR://认为登陆成功
				showToast(R.string.LOBBY_LOGIN_NOAVATAR);
				AppStatus.s_isConnectedLobby = true;
				break;
			case JniMessage.LOBBY_USERINFO_NOTEXIST:
				showToast(R.string.LOBBY_USERINFO_NOTEXIST);
				AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectLobby();
				onTransactionFinish(false,false);
				break;
			case JniMessage.LOBBY_USERINFO_INDULGE://不理防沉迷信息
				AppStatus.s_isConnectedLobby = true;
				break;
			case JniMessage.OPTION_TIMEOUT:
				showToast(R.string.lobby_token_timeout);
				AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectLobby();
				onTransactionFinish(false);
				break;
			}
			break;
		case XGMessage.XGMSG_LOBBYLOGIN_SUCCESS:
			AppStatus.s_isConnectedLobby = true;
			Log.i(TAG,"大厅登陆成功");
			//登陆大厅成功，保存自己的信息
			AppStatus.s_userMy = (LocalData_UserInfo)msg.obj;
			
			int online_type = 3;
			this.m_lockUser.lock();
			if(this.m_userOnlineType.containsKey(AppStatus.s_userMy.m_idx))
				online_type = m_userOnlineType.get(AppStatus.s_userMy.m_idx);
			this.m_lockUser.unlock();
			AppStatus.s_userMy.online_type = online_type;
			AppStatus.s_userMy.headurl = GetHeadUrl(AppStatus.s_userMy);
			//只是登陆大厅
			if(m_transactionNet.transactionToDo == NetUtilManager.TransactionLobbyLogin)
			{
				onTransactionFinish(true);
				break;
			}
			
			//重连大厅，需要重新连接房间
			beginRoomTransaction();
			break;
		case XGMessage.XGMSG_ROOMCON_SUCCESS:
			Log.i(TAG,"房间连接成功");
			
			if(-2 == AppStatus.s_NetUtilMgr.m_JniUtil.ConfirmToken())//已经token,直接进入房间
				enterRoom();
			break;
		case XGMessage.XGMSG_ROOMCON_FAILED:
			strError = m_context.getString(R.string.roomconnect_failed);
			showToast(strError+":"+msg.arg1);
			onTransactionFinish(false);
			break;
		case XGMessage.XGMSG_ROOMCON_TIMEOUT:
			showToast(R.string.roomconnect_timeout);
			onTransactionFinish(false);
			break;
		case XGMessage.XGMSG_ROOMCON_CLOSE:
			//showToast(R.string.roomconnect_close);
			
			//房间服务器主动关闭,需要重新扫码进入包厢
			AppStatus.s_nRoomId = 0;
			AppStatus.s_isNeedConnectedRoom = false;
			break;
		case XGMessage.XGMSG_ROOMCON_ERROR:
			strError = m_context.getString(R.string.roomconnect_error);
			showToast(strError+":"+msg.arg1);
			reconnect(false);
			onTransactionFinish(false);
			break;
		case XGMessage.XGMSG_ROOMAUTH_SUCCESS:
			Log.i(TAG,"房间认证成功");
			//第一次认证成功 进入房间
			enterRoom();
			break;
		case XGMessage.XGMSG_ROOMAUTH_FAILED:
			AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectRoom();
			if(msg.arg1 == -1)
			{
				if(msg.arg2 == JniMessage.OPTION_TIMEOUT)
				{
					showToast(R.string.roomauth_timeout);
					onTransactionFinish(false);
				}
				else//msg.arg2 = JniMessage.ROOM_TOKEN_ERROR
				{
					Object transactionObj = null;
					if(m_transactionNet.transactionListener != null)
						transactionObj = m_transactionNet.transactionListener;
					IOnTransactionListener transactionListener = new IOnTransactionListener(){

						@Override
						public void onTransactionComplete(Object obj) {
							// TODO Auto-generated method stub
							//取得新的认证成功后，再执行登陆房间操作
							doCurrentWork(TransactionRoomLogin,(IOnTransactionListener)obj);
						}

						@Override
						public void onTransactionFailed(Object obj) {
							// TODO Auto-generated method stub
							
						}

					};
					resetWork();
					doCurrentWork(TransactionAuthAgain,transactionListener,transactionObj);
				}
			}
			else if(msg.arg1 == -2)
			{
				//如果是提示未在大厅的错误，则需要重连大厅房间
				m_transactionNet.transactionToDo = TransactionLobbyLoginAgain;
				onTransactionFinish(false);
			}
			break;
		case XGMessage.XGMSG_ROOMENTER_FAILED:
			if(AppStatus.s_nRoomId == msg.arg1)
			{
				switch(msg.arg2)
				{
				case JniMessage.ROOM_LOGIN_NONE:
					showToast(R.string.ROOM_LOGIN_NONE);
					break;
				case JniMessage.ROOM_LOGIN_AGAIN:
					showToast(R.string.ROOM_LOGIN_AGAIN);
					break;
				case JniMessage.ROOM_LOGIN_NOUIN:
					showToast(R.string.ROOM_LOGIN_NOUIN);
					break;
				case JniMessage.ROOM_LOGIN_FULL:
					showToast(R.string.ROOM_LOGIN_FULL);
					break;
				case JniMessage.ROOM_LOGIN_IFULL:
					showToast(R.string.ROOM_LOGIN_IFULL);
					break;
				case JniMessage.ROOM_LOGIN_EPWD:
					showToast(R.string.ROOM_LOGIN_EPWD);
					break;
				case JniMessage.ROOM_LOGIN_CLOSE:
					showToast(R.string.ROOM_LOGIN_CLOSE);
					break;
				case JniMessage.ROOM_LOGIN_INBLACK:
					showToast(R.string.ROOM_LOGIN_INBLACK);
					break;
				case JniMessage.ROOM_LOGIN_INTBLACK:
					showToast(R.string.ROOM_LOGIN_INTBLACK);
					break;
				case JniMessage.ROOM_LOGIN_NOTHIS:
					showToast(R.string.ROOM_LOGIN_NOTHIS);
					break;
				case JniMessage.ROOM_LOGIN_LOCK:
					showToast(R.string.ROOM_LOGIN_LOCK);
					break;
				case JniMessage.ROOM_LOGIN_IP_INVALIED:
					showToast(R.string.ROOM_LOGIN_IP_INVALIED);
					break;
				case JniMessage.OPTION_TIMEOUT:
					showToast(R.string.roomenter_timeout);
					break;
				}
				
				AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectRoom();
				
				if(msg.arg2 == JniMessage.OPTION_TIMEOUT)
					onTransactionFinish(false);//超时，则重新连接房间
				else//房间状态异常，无法进入包厢，给予提示
					resetWork();
			}
			else
			{
				AppStatus.s_NetUtilMgr.m_JniUtil.DisConnectRoom();
				resetWork();
				return;//如果进入的不是对的房间，则直接返回
			}
			break;
		case XGMessage.XGMSG_ROOMENTER_SUCCESS:
			if(AppStatus.s_nRoomId == msg.arg1)
			{
				Log.i(TAG,"房间进入成功");
				//AppStatus.s_userMy.online_type = 1;
				m_JniUtil.RequestCurrentRoomHB(AppStatus.s_nRoomId);
			}
			onTransactionFinish(true);
			break;
		case XGMessage.XGMSG_CHAT_NOTIFY:
		{
		    if(msg.arg1 == 0)
		    {
		        ChatInfoToUi chat = (ChatInfoToUi)msg.obj;
		        
		        if(chat == null)
		        	break;
		        
		        ChatDisplayInfo display = new ChatDisplayInfo();
		        display.chat = new ChatInfo(chat);
		        
		        display.type = ChatDisplayInfo.ChatPublic;//默认公聊
		        if(chat.isSend)//回应
		        {
		        	display.chat.sayidx = 0;
		        	display.chat.toidx = chat.fromIdx;
		        	display.chat.sayname = "我";
		        	display.chat.sayphoto = AppStatus.user.headphoto;
		        	
		            if(!chat.isPublic)//不是公聊
		            {
		                //取出聊天对象的名字
		            	LocalData_UserInfo userInfo = m_JniUtil.GetUserInfoFromLocal(chat.fromIdx);
		            	if(userInfo == null)
		            		display.chat.toname = ""+chat.fromIdx;//昵称
		            	else
		            		display.chat.toname = userInfo.m_name;//昵称
		            	
		            	display.type = ChatDisplayInfo.ChatPrivate;//私聊
		            }
		            
		            display.position = ChatDisplayInfo.PosRight;
		        }
		        else//别人发的聊天信息
		        {
		        	display.chat.sayidx = chat.fromIdx;
		        	display.chat.toidx = 0;
		        	
		        	//取出聊天对象的名字
	            	LocalData_UserInfo userInfo = m_JniUtil.GetUserInfoFromLocal(chat.fromIdx);
	            	if(userInfo == null)
	            		display.chat.sayname = ""+chat.fromIdx;
	            	else
	            	{
	            		display.chat.sayphoto = GetHeadUrl(userInfo);
		            	display.chat.sayname = userInfo.m_name;//昵称
	            	}
	            	
		            if(!chat.isPublic)//私聊
		            {
		            	display.chat.toname = "我";
		            	display.type = ChatDisplayInfo.ChatPrivate;//私聊
		            }
		            display.position = ChatDisplayInfo.PosLeft;
		        }
		        display.tagIdx = display.chat.sayidx;
		        display.tagName = display.chat.sayname;
		        registerDisplay(display);
		    }
		    else if(msg.arg1 == JniMessage.OPTION_TIMEOUT)
		    	showToast(R.string.chat_send_timeout);
		    
			break;
		}
		case XGMessage.XGMSG_GIFT_NOTIFY:
		{
			ChatDisplayInfo display = new ChatDisplayInfo();
			ResultData_SendFlowerInfo gift = (ResultData_SendFlowerInfo)msg.obj;
			if(gift == null)
				break;
			display.gift = new GiftDisplayInfo(gift);
			
			if(!m_giftDataFinished)
				break;
			GiftInfo giftInfo = m_JniUtil.GetGiftInfoFromLocal(gift.cate_idx);
			if(giftInfo == null)
			{
				showToast(R.string.gift_not_exist);
				break;
			}
			display.gift.giftunit = giftInfo.unit_name;
			display.gift.gifturl = AppStatus.getLocalImage(giftInfo.image_name);
			display.gift.giftname = giftInfo.name;
			
			if(gift.s_idx == (int)AppStatus.userIdx)
			{
				display.gift.s_name = "我";
				display.gift.s_photo = AppStatus.user.headphoto;
			}
			else
			{
				LocalData_UserInfo userInfos = m_JniUtil.GetUserInfoFromLocal(gift.s_idx);
	        	if(userInfos == null)
	        		display.gift.s_name= ""+gift.s_idx;
	        	else
	        	{
	        		display.gift.s_name = userInfos.m_name;
	    			display.gift.s_photo = GetHeadUrl(userInfos);
	        	}
			}
			
        	if(gift.r_idx == (int)AppStatus.user.userIdx)
        	{
        		if(gift.s_idx == gift.r_idx)
        			display.gift.r_name = "自己";
        		else
        			display.gift.r_name = "我";
        	}
        	else
        	{
    			LocalData_UserInfo userInfor = m_JniUtil.GetUserInfoFromLocal(gift.r_idx);
            	if(userInfor == null)
            		display.gift.r_name = ""+gift.r_idx;
            	else
            		display.gift.r_name = userInfor.m_name;
        	}
			
			display.type = ChatDisplayInfo.ChatGift;
			if(gift.s_idx == (int)AppStatus.user.userIdx)
				display.position = ChatDisplayInfo.PosRight;
			else
				display.position = ChatDisplayInfo.PosLeft;
			
			display.tagIdx = display.gift.s_idx;
	        display.tagName = display.gift.s_name;
			registerDisplay(display);
			break;
		}
		case XGMessage.XGMSG_HORN_NOTIFY:
		{
			ChatDisplayInfo display = new ChatDisplayInfo();
			HornInfoToUi horn = (HornInfoToUi)msg.obj;
			if(horn == null)
				break;
			display.horn = new HornDisplayInfo(horn);
			
			boolean isMe = horn.from_idx == (int)AppStatus.user.userIdx;
			if(isMe)
			{
				display.horn.name = AppStatus.user.nickname;
				display.horn.photo = AppStatus.user.headphoto;
			}
			else
			{
				LocalData_UserInfo userInfo = m_JniUtil.GetUserInfoFromLocal(horn.from_idx);
	        	if(userInfo == null)
	        	{
	        		//向服务器请求用户数据信息
	        		int[] requestIdx = new int[1];
	        		requestIdx[0] = horn.from_idx;
	        		m_JniUtil.GetUserInfoForSpeaker(requestIdx);
	        		
	        		//暂时先用一下IDX作为昵称
	        		display.horn.name = ""+horn.from_idx;
	        	}
	        	else
	        	{
	        		display.horn.name = userInfo.m_name;
					display.horn.photo = GetHeadUrl(userInfo);
	        	}
			}
			
			switch(horn.horn_type)
			{
			case HornInfoToUi.BT_BROAD:
				display.type = ChatDisplayInfo.ChatHorn;
				if(isMe)
					display.position = ChatDisplayInfo.PosRight;
				else
					display.position = ChatDisplayInfo.PosLeft;
				display.tagIdx = display.horn.from_idx;
		        display.tagName = display.horn.name;
		        
		        m_hornTopDisplay = display;
		        msg.arg1 = 1;
				break;
			case HornInfoToUi.BT_EXCHANGE:
				display.type = ChatDisplayInfo.ChatExchange;
				display.position = ChatDisplayInfo.PosCenter;
				break;
			case HornInfoToUi.BT_RANKSONG:
				display.type = ChatDisplayInfo.ChatRank;
				display.position = ChatDisplayInfo.PosCenter;
				break;
			}
			registerIndexDisplay(display);
			registerDisplay(display);
			break;
		}
		case XGMessage.XGMSG_REWARD_NOTIFY:
		{
			ChatDisplayInfo display = new ChatDisplayInfo();
			RewardInfoToUi reward = (RewardInfoToUi)msg.obj;
			
			if(reward != null)
			{
				if(!m_giftDataFinished)
					break;
				display.reward = new RewardInfo(reward);
				GiftInfo giftInfo = m_JniUtil.GetGiftInfoFromLocal(display.reward.giftid);
				if(giftInfo == null)
				{
					showToast(R.string.gift_not_exist);
					break;
				}
				display.reward.giftname = giftInfo.name;
				display.reward.gifturl = AppStatus.getLocalImage(giftInfo.image_name);
				display.reward.gold = giftInfo.cash*reward.luckmultiple;
				display.type = ChatDisplayInfo.ChatReword;
				display.position = ChatDisplayInfo.PosCenter;
				registerDisplay(display);
			}
			break;
		}
		case XGMessage.XGMSG_KTVUSER_ONLINE:
		{
			ResultData_UIdxWithType[] aIdx = (ResultData_UIdxWithType[])msg.obj;
			if(aIdx == null)
				break;
			
			m_lockUser.lock();
			m_userOnlineType.clear();
			for(int i=0;i<aIdx.length;i++)
			{
				//修改下自己的在线状态
				if(aIdx[i].idx == (int)AppStatus.user.userIdx)
				{
					if(AppStatus.s_userMy != null)
						AppStatus.s_userMy.online_type = aIdx[i].type;
				}
				m_userOnlineType.put(aIdx[i].idx, aIdx[i].type);
			}
			m_lockUser.unlock();
			
			//去掉自己，因为自己的信息都是最新的
			int requestLen = aIdx.length-1;//包含自己？
			if(requestLen == 0)
				break;
			
			RequestData_IdxWithVersion[] requestIdx = new RequestData_IdxWithVersion[requestLen];
			int nSize = 0;
			for(int i=0;i<aIdx.length;i++)
			{
				if(aIdx[i].idx == (int)AppStatus.user.userIdx)
					continue;
				LocalData_UserInfo userInfo = m_JniUtil.GetUserInfoFromLocal(aIdx[i].idx);
				RequestData_IdxWithVersion idx = new RequestData_IdxWithVersion();
				idx.id = aIdx[i].idx;
				if(userInfo == null)
					idx.version = -1;
				else
					idx.version = userInfo.version;
				requestIdx[nSize++] = idx;
			}
	        if(0 != m_JniUtil.GetUserInfoWithVersion(requestIdx))
	        	showToast(R.string.user_request_failed);
			break;
		}
		case XGMessage.XGMSG_USERINFO_NOTIFY:
		{
			if(msg.arg1 == 0)
				addUserInfo(msg.arg2);
			else if(msg.arg1 == -1)
				showToast(R.string.user_no_exist);
			break;
		}
		case XGMessage.XGMSG_USERINFO_FORHORN:
		{
			if(msg.arg1 == 0)//获取成功
				addUserInfoForHorn((ResultData_UserSampleInfo)msg.obj);
			break;
		}
		case XGMessage.XGMSG_USER_LOGIN:
		{
			if(msg.arg1 == (int)AppStatus.user.userIdx)
			{
				AppStatus.s_userMy.online_type = msg.arg2;
				break;
			}
			
			boolean isNoNeedRequest = false;
			m_lockUser.lock();
			m_userOnlineType.put(msg.arg1, (int)msg.arg2);
			isNoNeedRequest = m_userMap.containsKey(msg.arg1);
			if(isNoNeedRequest)//如果存在该用户，则需要修改在线状态
				m_userMap.get(msg.arg1).online_type = msg.arg2;
			m_lockUser.unlock();
			
			//不存在该用户，则需要重新向服务器请求最新的数据
			if(!isNoNeedRequest)
			{
				LocalData_UserInfo userInfo = m_JniUtil.GetUserInfoFromLocal(msg.arg1);
				RequestData_IdxWithVersion idx = new RequestData_IdxWithVersion();
				idx.id = msg.arg1;
				if(userInfo == null)
					idx.version = -1;
				else
					idx.version = userInfo.version;
				RequestData_IdxWithVersion[] requestIdx = new RequestData_IdxWithVersion[1];
				requestIdx[0] = idx;
				 if(0 != m_JniUtil.GetUserInfoWithVersion(requestIdx))
			        showToast(R.string.user_request_failed);
			}
			break;
		}
		case XGMessage.XGMSG_USER_LOGOUT:
		{
			subUserInfo(msg.arg1);
			break;
		}
		case XGMessage.XGMSG_USERPIC_UPDATE:
		{
			changeUserPic(msg.arg1,(String)msg.obj);
			break;
		}
		case XGMessage.XGMSG_USERNAME_UPDATE:
		{
			changeUserName(msg.arg1,(String)msg.obj);
			break;
		}
		case XGMessage.XGMSG_HORNRESULT_NOTIFY:
		{
			if(msg.arg1 == 0)
			{
				ResultData_SelfData sd = m_JniUtil.GetSelfData();
				if(sd != null)
					AppStatus.user.gold = sd.gold;
			}
			break;
		}
		case XGMessage.XGMSG_GIFTRESULT_NOTIFY:
		{
			if(msg.arg1 == 0)
			{
				ResultData_SelfData sd = m_JniUtil.GetSelfData();
				if(sd != null)
					AppStatus.user.gold = sd.gold;
			}
			break;
		}
		case XGMessage.XGMSG_USERGOLD_UPDATE:
		{
			AppStatus.user.gold = msg.arg1;
			break;
		}
		//一般是包厢关闭信息
		case XGMessage.XGMSG_ROOMSTAT_UPDATE:
		{
			break;
		}
		case XGMessage.XGMSG_NORMAL_SONGEND:
		{
			//歌曲正常演唱完毕，处理一些逻辑,金币可以通过XGMSG_USERGOLD_UPDATE实现
			break;
		}
		case XGMessage.XGMSG_GIFTDATA_FINISHED:
		{
			m_giftDataFinished = true;
			//礼物配置信息更新来自web
			AppStatus.s_giftMgr.initGiftInfo(true);
			break;
		}
		case XGMessage.XGMSG_CUR_SONGLIST:
		{
			updateSonglist((ResultData_SongList[])msg.obj);
			break;
		}
		case XGMessage.XGMSG_FORCEOUT_LOBBY:
		{
			//禁止用户进入大厅
			AppStatus.s_isConnectedLobby = false;
			if(msg.arg1 == JniMessage.LOBBY_KICK_BY_IDX)
				showToast(R.string.lobby_idx_forbidden);
			else// if(msg.arg1 == JniMessage.LOBBY_KICK_BY_NET)
				showToast(R.string.lobby_mac_forbidden);
			//重新返回登录界面
			Intent intent = new Intent(m_context,LoginActivity.class);
			intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			AppStatus.backToLogin();
			m_context.startActivity(intent);
			break;
		}
		case XGMessage.XGMSG_NOTIFY_MESSAGE:
		{
			NormalNotify notify= new NormalNotify(m_context);
			notify.showNotify(msg.arg1,(String)msg.obj);
			break;
		}
		case XGMessage.XGMSG_HONGBAO_RECV://收到互动红包
		{
			ChatDisplayInfo display = new ChatDisplayInfo();
			display.type = ChatDisplayInfo.ChatRedPacket;
			display.hongbao = (HongBaoInfoToUi) msg.obj;
			if(display.hongbao.idx == AppStatus.s_userMy.m_idx)
			{
				display.position = ChatDisplayInfo.PosRight;
				display.tagIdx = AppStatus.s_userMy.m_idx;
				display.tagName = AppStatus.s_userMy.m_name;
				display.hongbao.name = AppStatus.s_userMy.m_name;
				display.hongbao.photo = GetHeadUrl(AppStatus.s_userMy.headurl);
			}
			else
			{
				display.tagIdx = display.hongbao.idx;
				if(display.tagIdx == 1)
				{
					display.tagName = m_context.getResources().getString(R.string.app_name);
				}
				else
				{
					LocalData_UserInfo userInfo = m_JniUtil.GetUserInfoFromLocal(display.hongbao.idx);
					display.tagName = userInfo.m_name;
					display.hongbao.name = userInfo.m_name;
					display.hongbao.photo = GetHeadUrl(userInfo.headurl);
				}
			}
				
			registerDisplay(display);
			break;
		}
		case XGMessage.XGMSG_HONGBAO_VIERESULT:
		{
			HongBaoInfoToUi info = (HongBaoInfoToUi) msg.obj;
			
			LocalData_UserInfo userInfo = m_JniUtil.GetUserInfoFromLocal(info.idx);
			if(userInfo != null)
			{
				info.name = userInfo.m_name;
				info.photo = GetHeadUrl(userInfo.headurl);
			}
			break;
		}
		case XGMessage.XGMSG_HONGBAO_DETAIL:
		{
			if(msg.arg1 == 0)//如果获取成功，处理下信息
			{
				HongBaoDetailInfoToUi info = (HongBaoDetailInfoToUi) msg.obj;
				if(info.idx == 1)
				{
					info.name = m_context.getResources().getString(R.string.app_name);
				}
				else
				{
					LocalData_UserInfo userInfo = m_JniUtil.GetUserInfoFromLocal(info.idx);
					if(userInfo != null)
					{
						info.name = userInfo.m_name;
						info.photo = GetHeadUrl(userInfo.headurl);
					}
				}
			}
			break;
		}
		case XGMessage.XGMSG_HONGBAO_ROOM:
		{
			if(msg.obj != null)
			{
				ResultData_RoomHB[] list = (ResultData_RoomHB[])msg.obj;
				int newHB = 0;
				for(ResultData_RoomHB hb : list)
				{
					ChatDisplayInfo display = new ChatDisplayInfo();
					display.type = ChatDisplayInfo.ChatRedPacket;
					display.hongbao = new HongBaoInfoToUi();
					display.hongbao.name = m_context.getResources().getString(R.string.app_name);
					display.hongbao.hbid = hb.hbid;
					display.hongbao.hbtype = hb.hbtype;
					display.hongbao.idx = hb.idx;
					display.hongbao.remark = hb.mark;
					display.tagIdx = hb.idx;
					display.tagName = display.hongbao.name;
					
					if(registerDisplay(display))
						newHB ++;
				}
				msg.arg2 = newHB;
			}
			break;
		}
		}
		
		m_lockListener.lock();
		for(int i=0;i<m_NetUtilListenerList.size();i++)
		{
			INetUtilListener listener = m_NetUtilListenerList.get(i);
			if(listener != null)
				listener.dealWithMsg(msg);
		}
		m_lockListener.unlock();
	}
	
	public interface INetUtilListener {
		public void dealWithMsg(Message msg);
	}
}
