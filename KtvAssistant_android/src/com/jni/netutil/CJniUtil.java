/*
 * 
 *   by glp
 *   2014-4-9   
 *   封装java native interface，
 *   以及实现JniUtil的方法.
 * 
 */

package com.jni.netutil;

import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;

import com.jni.netutil.out.ChatInfoToUi;
import com.jni.netutil.out.HongBaoDetailInfoToUi;
import com.jni.netutil.out.HongBaoInfoToUi;
import com.jni.netutil.out.HornInfoToUi;
import com.jni.netutil.out.RewardInfoToUi;


public class CJniUtil implements JniNetUtil
{
	//ResultDate_Channel tmp;
	//对于关于DLL的加载情况，低版本的Eclipse好像支持写在这里来调试C++，
	//但是v22.6.2-1085508需要写在Launcher Activity 中，这个例子写在了Activity里面,
	//才能很好的调试，遇到F6按了没用的时候再下面加个断点，点击继续，仍然可以继续调试
//	static {
//		System.loadLibrary("stlport_shared");
//		System.loadLibrary("JniNetUtil");
//	}
//	public ClassLoader getClassLoader(){
//		return this.getClassLoader();
//	}
	//初始化
	public void initJniUtil(Handler h){
		m_handler = h;
		assert(m_handler != null);
	}
	
	/*
	 * @int return ： 
	 * 0 ：成功
	 * -1：失败
	 */
	//启动客户端
    public native int StartClient();
    //停止客户端
    public native void StopClient();
    //取得Token
    public native String GetToken();
    //取得Token长度
    public native int GetTokenlen();

    //认证服务器是否连接
    public native boolean IsConnectAuth();
    //大厅是否连接
    public native boolean IsConnectLobby();
    //房间是否连接
    public native boolean IsConnectRoom();
    //大厅是否token
    public native boolean IsTokenLoginLobby();
    //房间是否token
    public native boolean IsTokenLoginRoom();

    public native int ConnectAuth(String host,short port,int timeout);
    public native int DisConnectAuth();
    public native int ConnectLobby(String host,short port,int timeout);
    public native int DisConnectLobby();
    public native int ConnectRoom(String host,short port,int type,int timeout);
    public native int DisConnectRoom();

    public native int IdPassAuth(String id,String passwd,int again);
    public native int TouristAuth(int again);
    //Room API
    //验证token
    public native int ConfirmToken();
    //进入房间
    public native int EnterRoom(int cseq,int roomid, String passwd, short type);
    //取得房间当前运行信息
    public native int GetRoomAllInfomation(int cseq);
    //取麦序列表
    public native int GetWaitMicList();
    //排麦
    public native int RoomWaitMic( int musicid, short bk, char level, String  name, char speed );
    //取消排麦
    public native int RoomCancleWaitMic( int idx );
    //提升麦序
    public native int RoomUpWaitMic(char tag, int idx, char flag);
    //flag==>MIC_UP_DOWN_FLAG
    //下降麦序
    public native int RoomDownWaitMic(char tag, int idx, char flag);
    //flag==>MIC_UP_DOWN_FLAG
    //上麦准备ok
    //int RoomOnMicReadyOK( short music_len );
    //准备下麦
    //int RoomOffMicReady();
    //把指定用户提出房间
    public native int RoomKickSomeOne(int idx);
    //禁言某人（或取消禁言）
    public native int RoomForbidenSomeOne(int idx, char flag);
    //上主持人麦
    public native int RoomOnvjMic();
    //下主持人麦
    public native int RoomOffvjMic();
    //把主持人麦给某人
    public native int RoomGivOnevjMic(int idx);
    // 是否同意上主持人麦 add by cx 10-6-2
    public native int ApplyGetVJmic( int idx, char flag );
    //让主持人下麦
    public native int RoomGiveOffvjMic(int idx);
    //让普通麦上麦用户下麦
    public native int RoomGiveOffMic(int idx);
    //flag==>ADD_SUB_FLAG
    //跟新房间黑名单列表(单条添加和删除)
    public native int RoomUpdateBlackList(int cseq,int idx, char flag);
    //flag==>ADD_SUB_FLAG
    //跟新管理员列表(单条添加和删除)
    public native int RoomUpdateMgrList(int idx, char flag, char identity);

    //登录大厅
    /*
     @param mac:一般为设备的MAC地址，但是由于IOS，苹果禁止获取MAC地址，所以在IOS设备上使用的是UIDevice Unicode的字符串
     @param loginway:登录的方式，0：PC端，1：ANDROID，2：IOS
     
     @return:1，成功；0，失败。
     */
    public native int LoginLobby( int lobbyId,String  mac,int loginway);
    //登录大厅(重连)
    public native int LoginLobbyAuto( String  mac, int roomid );
    //获取用户资料 参数一用户idx列表,参数二数量
    public native int GetUserInfo_AII(int idxl[],int count);
    //获取好友列表
    public native int GetFriendListWithoutUserInfo();
    //获取黑名单列表
    public native int GetBlackListWithoutUserInfo();
    //添加好友
    public native int AddFriend(int idx);
    //添加黑名单
    public native int AddBlack(int cseq,int idx);
    //删除好友
    public native int DelFriend(int cseq,int idx);
    //删除黑名单
    public native int DelBlack(int cseq,int idx);
    //获取通道列表
    public native int GetChannellist( int version, String  mac );
    //*********************************room manage api*********************************
    //获取当前大厅的房间列表
    public native int GetRoomList(int cseq);
    public native int GetRoomListEx(int cseq, int type);
    //获取房间信息,可以是一组房间
    public native int GetRoomInfo(int cseq,int roomcount, int[] roomlist);
    public native int GetTmpRoomInfo( int roomId );
    //*********************************************************************************
    //用户创建头像信息 
    @Deprecated
    public native int CreateAvatar( String nickname, ResultData_Avatar_Updata avarat );
    //avatarname长度限制20,partlen,decoration大小限制为10
    @Deprecated
    public native int ChangeAvatar( ResultData_Avatar_Updata avarat );
    //修改昵称
    public native int ChangeNickName( int idx, String  name, int len );
    //给好友送花
    public native int SendFlower(int s_idx,int r_idx,int cate_id,int count);
    //发送大喇叭
    public native int BroadAll(String content);
    //发送小喇叭
    public native int BroadLobby(int lobbyidx,String content);
    //发送房间临时公告
    public native int SendRoomNotice(String content);
    //取得指定大厅信息
    public native int GetLobbyInfo(int cseq,int lobbyid);
    //邀请用户进入表演麦麦序
    public native int InviteMic(int useridx);
    /*			add by jinguanfu 2010/3/2 <end> 回调API改造成接口			*/
    /*			add by jinguanfu 2009/12/18 <begin> 聊天、喇叭功能			*/
    //发送用户私聊
    public native int SendChatToUser(int to_idx, String content);
    //发送公聊（俱乐部内部）
    public native int SendChatToClub(String content);
    //发送公聊（房间内）
    public native int SendChatToRoom(String content);
    //发送大喇叭（全区）
    public native int SendSpeakerToAll(String msg);
    //发送小喇叭（大厅）
    public native int SendSpeakerToLobby(int lobbyid, String msg);
    /*			add by jinguanfu 2009/12/28 <begin> 好友操作				*/
    //查询好友位置
    public native int QueryFriendLocation(int cseq,int friend_idx);
    /*			add by jinguanfu 2009/12/28 <begin> 房间管理接口			*/
    //请求取得待审核房间会员列表
    public native int GetRoomApplyList (int cseq);
    //用户申请房间会员
    public native int ApplyRoomMember(int cseq);
    //审核会员加入申请
    public native int AuditAddApply (int cseq,int idx,char type);
    //取房间会员列表
    public native int GetRoomMemberList (int cseq);
    //解除房间会员资格
    public native int UnchainMember (int cseq,int idx);
    //赋予用户房间主持助理权限
    public native int SetRoomPesideAssistant(int cseq,int idx);
    //赋予用户房间主持人权限
    public native int SetRoomPeside (int cseq, int idx);
    //赋予用户房间副室主权限
    public native int SetRoomSubHost(int cseq, int idx);
    //设置为房间会员（由管理员变为会员）	//add by jinguanfu 2010/5/13
    public native int SetRoomMember(int cseq, int idx);
    //取得房间黑名单
    public native int GetBlackList (int cseq);
    //黑名单用户添加/删除
    public native int AddDelBlack (int idx ,char type);
    //查看房间设置面板
    public native int GetRoomPanelInfo (int cseq);
    //设置房间密码
    public native int SetRoomPassword(int cseq,String password);
    //设置房间开启/关闭(上锁)
    public native int SetRoomSwitch(int cseq,char roomflag);
    //设置房间仅对会员开放
    public native int SetRoomPrivate(int cseq,char accessflag);
    //用户进出信息开启/关闭
    public native int SetUserInOutSwitch(int cseq,char inoutflag);
    //设置自由排麦开启/关闭
    public native int SetGetMicSwitch(int cseq,char getmicflag);
    //设置自由上麦开启/关闭
    public native int SetOnMicSwitch(int cseq,char onmicflag);
    //房间名称修改
    public native int ModifyRoomName(int cseq,String roomname);
    //设置房间公告
    public native int SetRoomAffiche (int cseq,String content);
    //设置允许房间公聊
    public native int AllowRoomAllChat (int cseq , char chatflag);
    //设置房间欢迎词
    public native int SetRoomSalutatory (int cseq,String content);
    //设置房间logo
    public native int SetRoomLogo(int cseq, String path);
    /*			add by jinguanfu 2009/12/28 <begin> 角色信息				*/
    //发送取得角色信息请求
    public native int GetPartInfo(int cseq,int idx);
    //取得指定大厅的房间列表
    public native int GetRoomList_II(int cseq,int lobbyid);
    /************************************************************************/
    /*			add by jinguanfu 2010/1/14 <begin> 							*/
    //离开房间（切换房间时使用）
    public native int LeaveRoom();
    /*			add by jinguanfu 2010/1/20 <begin> 							*/
    //是否同意添加好友
    //参数说明：int		idx		请求添加好友的用户
    //			char	flag	0--同意 1--不同意
    public native int ApplyAddFriend(int idx, char flag);
    /*add by jinguanfu 2010/1/27 删除角色信息 ********/
    /*参数说明：int		idx		用户idx	*/
    //int DeleteAvarta(int idx);
    //设置回调类对象
    //public native void SetResponseHandle(ResponseBase* pRb);

    //add by jinguanfu 2010/4/8 取得简单房间信息
    public native int GetRoomSimpleInfo();
    public native int GetRoomSimpleInfoEx( int type );
    //add by jinguanfu 2010/4/11 用户退出房间会员
    public native int ExitRoomMember();
    //add by jinguanfu 2010/4/15 取得大厅在线人数
    public native int GetLobbyOnlineNum();
    //add by cx 10-7-6 购买
    @Deprecated
    public native int BuyPack( ResultData_Buy [] pPack, int count, int type, int idx );
    @Deprecated
    public native int GetPack();
    @Deprecated
    public native int DelPack( int cseq, int [] idx, int count );
    @Deprecated
    public native int UsePack( int idx );
    @Deprecated
    public native int CreateRoom( ResultData_CreateRoom room );
    //烟花
    public native int SendFireworks( int cate_idx, int count );

    //////////////////////////////////////////////////////////////////////////

    //取得大厅礼物排行榜信息
    public native int GetGiftTopList();
    //取得在线时间
    public native int GetOnlineTime( int idx );
    //取得新手任务
    @Deprecated
    public native int GetNewUserTask();
    //提交新手任务
    @Deprecated
    public native int TaskStep( short step );
    @Deprecated
    public native int TaskGift( short step );
    //获取礼物滚动广播
    public native int GetGiftInfo();
    //获取系统文字公告
    public native int GetSystemBroad();
    //获取印章信息
    public native int GetUserSeel( int [] idx );
    //发送用户pc配置
    public native int SendSysteInfo( String  data );
    //获取分组信息
    public native int GetGroupInfo();
    //获取组里面的房间列表
    public native int GetRoomListInGroup( int group_idx );
    public native int GetRoomListInGroupBatch(short[] groupId);
    //获取歌曲列表
    public native int GetSongInfoList();
    //获取音视频服务器列表
    public native int GetAvServerList( short net_type );
    //获取离线消息
    public native int GetOffLineMsg();
    //获取分组置顶信息
    public native int GetRoomSortInGroup( short group_id );
    //获取小喇叭信息
    public native int GetUserSpeakerInfo();
    // 获取家族信息
    public native int GetFamilyInfo();

    //////////////////////////////////////////////////////////////////////////

    //客户端房间打分
    //String scoreinfo -- 打分信息结构体指针
    //int length--打分信息结构体长度
    public native int RoomScore(String scoreinfo);
    //显示房间可领取收益
    @Deprecated
    public native int ShowRoomIncome();
    //显示收益已领取记录
    @Deprecated
    public native int ShowRoomGetIncome();
    //室主领取房间内收益
    @Deprecated
    public native int GetRoomIncome();
    //发送用户pc概况
    public native int SendPcData( String  data );
    //邀请用户加入房间会员
    public native int InviteJoinRoom( int idx, int wseq );
    //邀请用户加入房间会员
    public native int ReInviteJoinRoom( char type );
    //取得当日礼物赠送记录
    public native int GetGiftSend();
    //取得当日礼物接收记录
    public native int GetGiftRecv();
    //封mac
    public native int KickUserByMac( int idx );
    //封ip
    public native int KickUserByIp( int idx );
    //设置在麦剩余时间
    public native int SetOnMicTemainTime( int idx, int time );
    //设置在歌曲剩余时间
    public native int SendSongRemnantsTime( short time );

    /* 私麦接口                                                             */
    //上私麦, mictype为私麦的类型(vip以上可观看或需要对方确认可观看)
    public native int OpenPrivateMic(short mictype);
    //下私麦
    public native int ClosePrivateMic();
    //被抱下私麦
    public native int ClosedPrivateMic(int bidx); //bidx=私麦持有者
    //请求观看idx的私麦
    public native int RequestWatchPrivateMic(int idx); //idx=私麦持有者
    //回复观看私麦,只有同意才调用这个接口
    public native int ResponseWatchPrivateMic(int allow, int idx);//idx=观看者,0->同意，-1->拒绝
    //邀请其他人看私麦
    public native int InviteWatchPrivateMic(int idxs[]);
    //回复邀请看私麦
    public native int ResponseInviteWatchPrivateMic(int allow, int idx);
    //房间私麦管理
    public native int OpenRoomPrivateMicFlag(char flag);//0：不开放 1：开放

    // 皇冠踢人
    public native int CrownKickoutSomeOne(int idx);
    // 室主对用户的IP进行禁用
    public native int ForbidUserIp(int idx);

    // 发送句评分
    public native int SingerScores( int idx, short scores[] );
    // 要求上传歌曲
    public native int UpdateSong( int idx );
    
    //2014-09-26 by glp
    /*
     获取发喇叭的用户信息
     */
    public native int GetUserInfoForSpeaker(int idx[]);
    /*
     获取用户信息夹带版本ID
     */
    public native int GetUserInfoWithVersion(RequestData_IdxWithVersion data[]);
    //上面接口都是0成功，-1失败
    
    //本地数据库 by glp 2014-6-18
    /*
     初始化下载URL地址(包含一些资源地址等等)具体参照原PC星光客户端
     @param url[in]: URL路径
     @param forUse[in]: 使用的用途,下面一一说明,
        0 : 礼物更新地址
     
     @return：1，成功；0，失败
     
     @例:InitDownLoadUrl("http://res2.17xg.com/res/giftxml.html",0);
     
     @注：必须先调用InitDownLoadUrl再调用InitLoaclDatabaseDir.
     */
    public native int InitDownLoadUrl(String url,int forUse);
    //初始化本地数据目录,包含图片资源路径
    /*
    @param path[in]：指定保存数据文件的目录，注意是目录
     
    @return：1，成功；0，失败
    */
    public native int InitLocalDatabaseDir(String path);
    //保存人物部分信息，
    /*
     @param partInfo[in]：具体的信息结构
     
     @return：1，成功；0，失败
     */
    //public native int SavePartInfo(ResultData_PartInfo partInfo);
    //保存自己的部分信息
    /*
     @param partInfo[in]：具体的信息结构
     
     @return：1，成功；0，失败
     */
    //public native int SaveMyPartInfo(ResultData_PartInfo partInfo);
    //获取自己的信息
    public native LocalData_UserInfo GetMyInfo();
    //获取到自己的数据
    public native ResultData_SelfData GetSelfData();
    //保存自己的数据
    //public native int SaveSelfData(ResultData_SelfData pSD);
    //获取本人的ID
    /*
     @return：本人ID
     */
    public native int GetMyIdx();
    //设置本人的ID
    //public native int SaveMyIdx(int idx);
    //保存用户信息，
    /*
    @param userInfo[in]：用户基本信息
         
    @return：1，成功；0，失败
    */
    //public native int SaveUserInfo(USER_DATA_BASE userInfo);
    //提供给手机只是保存当前的在线用户的信息
    /*
     @param idx:IDX
     @param nickName:用户昵称
     
     @return：1，成功；0，失败
     */
    //public native int SaveUserInfoSample(int idx,String nickName);
    //保存房间信息
    //public native int SaveRoomInfo(ROOM_DATA roomInfo);
    //获取房间分组版本号
    /*
     @param lobbyId[in]：大厅ID
     
     @return：分组版本号
     */
    //public native int GetServerGroupVersion(int lobbyId);
    //保存房间分组版本号
    //public native int SaveServerGroupVersion(int lobbyID,int groupVersionId);
    //清除本地所有房间缓存信息
    //public native int ClearRoom();
    //清除本地所有分组缓存信息
    //public native int ClearGroup();
    //增加一个分组信息
    /*
     @param groupId[in]：分组ID
     @param groupName[in]：分组名称
     @param disable[in]：是否禁用
     @param lobbyId[in]：大厅ID
     
     @return：1，成功；0，失败
     注：之后必须调用AddGroupDataEnd函数
     */
    //public native int AddGroupData(int groupId,String groupName,boolean disable,int lobbyId);
    //分组信息添加结束。
    //public native int AddGroupDataEnd();
    //增加一个房间信息
    /*
     @param roomId[in]：房间ID
     @param groupId[in]：分组ID
     @param lobbyId[in]：大厅ID
     
     @return：1，成功；0，失败
     注：之后必须调用AddRoomDataEnd函数
     */
    //public native int AddRoomData(int roomId,int groupId,int lobbyId);
    //房间信息添加结束。
   // public native int AddRoomDataEnd();
    /*
     判断是否是网通用户
     @param host[in]：要连接的域名
     @param strWangtong[in]：指定的网通IP地址，可以把几个IP地址连起来，放进去，通过";"分割。
     
     @return：0，不是网通用户；1，是网通用户。
     */
    public native int IsWangtong(String host,String strWangtong);
    /*
     从本地数据库中获取到指定房间ID的房间信息
     @param roomId[in]：指定的房间ID，或者包厢ID
     @parma roomInfo[out]：指定房间信息的存放位置
     
     @return：1，成功；0，失败。
     */
    public native ResultData_RoomInfo GetRoomInfoFromLocal(int roomId);
    /*
     从本地数据库中获取指定IDX的用户信息
     @param idx[in]:指定IDX
     @param userInfo[out]:获取到的个人信息。
     
     @return：1，成功；0，失败。
     */
    public native LocalData_UserInfo GetUserInfoFromLocal(int idx);
    /*
     从本地数据库获取制定礼物信息
     @param giftId:指定礼物ID
     @param pGI:礼物信息存放位置
     
     @return：1，成功；0，失败。
     */
    public native GiftInfo GetGiftInfoFromLocal(int giftId);
    //获取所有礼物信息的ID
    /*
     @param pId[out]：礼物ID存放的位置
     @param nNum[in out]：数组大小，返回存放了多少个ID
     @return 数量够存放，true；数量不够存放，false
     
     @注：一定要严格判断返回值
     */
    public native int[] GetAllGiftId();
    /*
     专门用于星光物件的更新接口，包含礼物等
     
     @param taskInfo[in]：下载任务信息，具体参见结构说明
     @param priority[in]：是否优先下载
     
     @注1：对于TaskDownload结构中的DownloadInfo结构中的saveBuf，这里已经对其做了管理，用户无需自己申请管理内存，只要在回调的函数里使用即可
     @注2：对于结构中url和download必须根据实际相匹配
     
     @例:
     TaskDownload task;
     strcpy(task.url , "http://xg.91555.com/res/giftxml.html");
     task.onProgress = &OnProgress;//可选，指定进度函数，可以显示下载，访问进度条。
     task.onFinish = &OnFinish;//指定完成函数，
     task.info.download = false;
     strcpy(task.info.fileName , "C:\\Users\\Administrator\\Desktop\\gift\\gift_items.zip");//指定文件名和位置
     task.info.mandatory = false;
     strcpy(task.info.unzipDir , "C:\\Users\\Administrator\\Desktop\\testunzip");//指定解压路径
     b = g_gMgr.addTask(task);
     //上面这个例子中，你还需要在OnFinish中解析你Http请求到的内容，然后进一步添加任务，下载真正的zip文件，详情请咨询@guliping。
     
     @return：0，添加任务成功；-1，添加任务失败；-2，文件已经存在无需下载
     */
    //public native int AddHttpDownLoadTask(TaskDownload taskInfo,boolean priority);
    /*
     手机用户聊天接口
     @param toIdx[in]:发送给谁，如果是0就是公聊。
     @param content[in]:聊天的内容.
     
     @return：0，成功；-1，失败。
     */
    public native int SendChatKTV(int toIdx,String content);
    /*
     手机用户送礼接口
     @param toIdx[in]:发送给谁
     @param giftId[in]:礼物ID
     @param number[in]:礼物数量
     @param roomId[in]:房间ID，如果在大厅则传0
     
     @return：0，成功；-1，失败。
     */
    public native int SendGiftInLobby(int toIdx,int giftId,int number,int roomId);
    
    /*
     务必调用此方法，否则所有MF前缀的函数都将返回false
     */
	public native boolean MFInitFilter();
	/*
     过滤敏感字符串
     @param utf8[in out]：需要对比的UTF8字符串
     @param size[in] : 字符串的长度
     
     @return :
     false，没有初始化或者utf8为NULL
     true，成功过滤完毕
     
     @注：utf8中的字符串必须是UTF8字符串
     */
    public native String MFFilter( String  utf8 );
	/*
     查找是否含有敏感字符串
     
     @param utf8[in]：需要对比的UTF8字符串
     
     @注：utf8中的字符串必须是UTF8字符串
     
     @return ：true，有；false，没有
     */
	public native boolean MFFindSensWord( String utf8 );
	/*
     查找是否含有特殊符号
     
     @param utf8[in]：需要对比的字符串
     
     @return ：true，有；false，没有
     */
	public native boolean MFFindSensWord2( String utf8 );
	
	//add by glp 2015-5-7
	/*
	请求发红包
	@param hbtype : 红包类型 0-个人  1房间手气 2-房间均分  3-大厅手气 4-大厅均分
	@param number : 红包数量
	@param gold : 红包总金额
	@param mark : 红包备注
	@param extradata : 如果是个人红包，则代表接受者的IDX；如果是房间红包或是大厅，则是0

	@return : -1失败；0成功
	*/
	public native int RequestSendHB(byte hbtype,int number,int gold,String mark,int extradata);

	
	/*
	 * 
	 * 请求查询红包的状态，只准对需要抢的红包，个人红包不需要查询
	 * @param hbid : 红包ID
	 * 
	 * @return : -1失败；0成功
	 */
	public native int RequestHBStatus(String hbid);
	/*
	请求抢红包
	@param hbid : 红包ID

	@return : -1失败；0成功
	*/
	public native int RequestVieHB(String hbid);

	/*
	查看红包领取详细情况
	@param hbid : 红包ID

	@return : -1失败；0成功
	*/
	public native int RequestHBDetail(String hbid);
	
	/*
	请求获取包厢红包信息

	@param roomid : 当前所在的包厢

	@return : -1失败；0成功
	*/
	public native int RequestCurrentRoomHB(int roomid);
	
	
	//------------------------------------------------------JniUtil 实现-glp---------------------------------------------//
  //聊天、喇叭功能
	public void SendChatFromUser(int ret, int to_idx, String content, int time ){}
	//接收私聊
	public void RecvChatFromUser(int ret, int from_idx, String content, int time ){}
	//接收公聊（俱乐部内部）
	public void RecvChatFromClub(int ret, int from_idx, String content){}
	//接收公聊（房间内）
	//ret:1--正常 0--被禁言 -1--不允许公聊
	public void RecvChatFromRoom(int ret, int errcode,int from_idx, String content, int time ){}	
	//接收大喇叭（全区）
	public void RecvSpeakerFromAll(int ret, int type, int from_id, String msg){}
	//接收小喇叭（大厅）
	public void RecvSpeakerFromLobby(int ret, int from_idx,String room_name, String msg, int time,int type,boolean isSend ){
		//ret 肯定为0
		Message hMsg = m_handler.obtainMessage();
		hMsg.what = XGMessage.XGMSG_HORN_NOTIFY;
		
		HornInfoToUi horn = new HornInfoToUi();
		horn.from_idx = from_idx;
		horn.room_name = room_name;
		horn.msg = msg;
		horn.time = time;
		horn.horn_type = type;
		horn.isSend = isSend;
		
		hMsg.arg1 = 0;
		hMsg.obj = horn;
		m_handler.sendMessage(hMsg);
	}
	//接收礼物广播
	public void RecvPresentBroad(int ret, ResultData_GiftInfo guestGiftInfo, ResultData_GiftInfo commGiftInfo){}
	//接受系统文字广播
	public void RecvSystemBroad( int type, String msg){}
	//接受系统文字广播（幸运中奖信息）
	public void RecvSystemBroad2(String nickname, int idx, int roomid, int giftid, int luckmultiple){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_REWARD_NOTIFY;
		RewardInfoToUi reward = new RewardInfoToUi();
		reward.nickname = nickname;
		reward.idx = idx;
		reward.roomid = roomid;
		reward.giftid = giftid;
		reward.luckmultiple = luckmultiple;
		msg.obj = reward;
		m_handler.sendMessage(msg);
	}
	//接受系统文字公告
	public void RecvSystemNotice( String msg, boolean once, int min ){}
	//接受系统文字公告（幸运中奖信息）
	public void RecvSystemNotice2(String nickname, int idx, int roomid, int giftid, int luckmultiple){}
	//接收房间临时公告
	public void RecvRoomNotice(int ret, String content){}
	//取得好友位置
	public void RecvFriendLocation(int ret, int errcode,int friend_idx,int lobbyid, int roomid,int cseq){}
	/*			 好友操作					*/
	/*			房间管理接口			*/
	//接收取得待审核房间会员列表
	public void RecvRoomApplyList (int ret ,int errcode,ResultDate_RoomApplyList applylist,int cseq){}
	//接收申请房间会员响应
	public void RecvApplyRoomMember(int ret,int errcode,int cseq){}
	//接收审核会员加入申请结果
	public void RecvAuditAddApply (int ret, int errcode,int idx,int cseq){}
	//接收拒绝房间申请系统提示
	public void RecvRoomApplyRefuseMsg (int ret, int roomid){}
	//接收房间会员列表
	public void RecvRoomMemberList (int ret ,int errcode, ResultDate_RoomMemList memlist,int cseq){}
	//接收解除房间会员资格回应
	public void RecvUnchainMember (int ret, int errcode, int idx,int cseq){}
	//接收赋予用户房间主持助理权限回应
	public void RecvRoomPesideAssistant (int ret , int errcode, int idx,int cseq){}
	//接收赋予主持助理通知
	public void RecvRoomNotice4PesideAssistant (int ret , int opidx,int bopidx){}
	//接收赋予用户房间主持人权限回应
	public void RecvRoomPeside (int ret ,int errcode, int idx,int cseq){}
	//接收设置主持人权限房间通知
	public void RecvRoomNotice4Peside (int ret , int opidx,int bopidx){}
	//接收赋予用户房间副室主权限回应
	public void RecvRoomSubHost (int ret ,int errcode, int idx,int cseq){}
	//接收设置副室主权限房间通知
	public void RecvRoomNotice4SubHost(int ret , int opidx,int bopidx){}
	//接收房间黑名单
	public void RecvBlackList(int ret,	int errcode,ResultDate_RoomBlackList blacklist,int cseq){}
	//接收黑名单用户添加/删除回应
	public void RecvAddDelBlack(int ret , int errcode,int idx, int type){}
	//接收房间设置面板信息
	public void RecvRoomPanelInfo(int ret ,int errcode, ResultDate_RoomPanel panelinfo){}
	//接收设置房间密码回应
	public void RecvRoomPassword(int ret,int errcode,int cseq){}
	//接收设置房间密码消息提示
	public void RecvRoomNotice4Password(int ret, int idx, int pwdflag){}
	//接收设置房间开启/关闭(上锁)回应
	public void RecvRoomSwitch(int ret,int errcode,int roomflag,int cseq){}
	//接收房间开启/关闭(上锁)消息提示
	public void RecvRoomNotice4RoomSwitch(int ret , int idx , int roomflag){}
	//接收设置房间访问权限回应
	public void RecvRoomPrivate(int ret , int errcode,int accessflag,int cseq){}
	//接收房间访问权限消息提示
	public void RecvRoomNotice4Private(int ret , int idx , int accessflag){}
	//接收设置用户进出信息开启/关闭回应
	public void RecvUserInOutSwitch(int ret , int errcode, int inoutflag,int cseq){}
	//接收用户进出信息开启/关闭消息提示
	public void RecvRoomNotice4UserSwitch (int ret , int idx , int inoutflag){}
	//接收设置自由排麦开启/关闭回应
	public void RecvGetMicSwitch (int ret, int errcode,int onmicflag,int cseq){}
	//接收设置自由上麦开启/关闭回应
	public void RecvOnMicSwitch (int ret, int errcode,int onmicflag,int cseq){}
	//接收自由排麦开启/关闭消息提示
	public void RecvRoomNotice4GetMicSwitch(int ret , int idx , int getmicflag){}
	//接收自由上麦开启/关闭消息提示
	public void RecvRoomNotice4OnMicSwitch(int ret , int idx , int onmicflag){}
	//房间名称修改回应
	public void RecvModifyRoomName(int ret,int errcode,int cseq){}
	//接收房间名
	public void RecvRoomName (int ret , int roomid , String roomname){}
	//设置房间公告响应
	public void RecvRoomAffiche(int ret,int errcode,int cseq){}
	//接收房间公告更新事件
	public void RecvRoomAfficheEvent(int ret){}
	//设置允许房间公聊响应
	public void RecvAllowRoomAllChat (int ret, int errcode,int chatflag,int cseq){}
	//接收房间公聊设置消息提示
	public void RecvRoomNotice4AllChat(int ret , int idx , int chatflag){}
	//设置房间欢迎词响应
	public void RecvRoomSalutatory(int ret, int errcode,int cseq){}
	//接收房间欢迎词更新通知
	public void RecvRoomNotice4Salutatory(int ret,int idx){}
	//设置房间logo响应
	public void RecvRoomLogo (int ret,int errcode ,int cseq){}
	//接收房间LOGO更新通知
	public void RecvRoomNotice4Logo(int ret,int idx){}
	//接收房间冻结消息提示
	public void RecvRoomNotice4Lock(int idx){}
	//接受创建房间回应
	public void RecvCreateRoom( int ret, int errcode, RoomListEx info ){}
	//接受练歌房完全信息
	public void RecvTmpRoomInfo( int ret, int errcode, RoomListEx info ){}
	// 添加歌房广播
	public void RecvRoomNotice4Add( RoomListEx info ){}
	public void RecvRoomNotice4Add( int idx ){}
	// 删除歌房广播
	public void RecvRoomNotice4Del( int idx ){}
	// 接收房间易主通知
	public void RecvRoomNotice4ChangeMaster(int idx){}
	// 接收房间信息更改通知
	public void RecvRoomNotice4ChangeInfo( int idx ){}
	/*			房间管理接口				*/
	/*			 角色信息				*/
	//取得他人角色信息
	public void RecvPartInfo(int ret, int errcode, ResultData_PartInfo part_info, int cseq){
		
	}
	  
	  //取得自己角色信息 专用于java的接口 登陆大厅最后的返回结果 @glp
	  //@注：Android独有的回调函数用于在大厅登陆最后一步的回调
	public void OnLoginLobby(int ret, int errcode, LocalData_UserInfo userInfo){
		Message msg = m_handler.obtainMessage();
		if(ret == 0)
		{
			msg.what = XGMessage.XGMSG_LOBBYLOGIN_SUCCESS;
			msg.obj = userInfo;
		}
		else
		{
			msg.what = XGMessage.XGMSG_LOBBYLOGIN_FAILED;
			msg.arg1 = errcode;
		}
		m_handler.sendMessage(msg);
	}
	//取得自己角色信息
	public void RecvPartInfoBySelf(int ret, int errcode, ResultData_PartInfo part_info, int cseq){}
	//角色信息
	//取得房间列表
	public void RecvRoomList(int ret,ResultDate_RoomList roomlist){}
	public void RecvRoomListEx(int ret,ResultDate_RoomListEx roomlist){}
	//下麦通知
	public void RecvNotice4OffMic( int onmic_idx, int pk_idx, int exp, String scoreinfo){}
	// add by cx 10-6-2
	//回应是否邀请上麦成功
	public void RecvRoomGivOnevjMic( int idx, int flag ){}
	//是否同意邀请上麦成功
	public void RecvRoomGivOnevjMicApply( int formidx ){}
	//是否同意添加对方为好友
	//参数说明：int idx 添加好友者idx
	public void RecvAddFriendApply(int idx){}
	//public void RecvDeleteAvarta(int ret){}
	//成功连接认证服务器
	public void OnAuthTunnelConnectSuccess(){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_AUTHCON_SUCCESS;
		m_handler.sendMessage(msg);
	}			
	//成功连接大厅服务器
	public void OnLobbyTunnelConnectSuccess(){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_LOBBYCON_SUCCESS;
		m_handler.sendMessage(msg);
	}
	//成功连接房间服务器		
	public void OnRoomTunnelConnectSuccess(){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_ROOMCON_SUCCESS;
		m_handler.sendMessage(msg);
	}			
	public void OnChannellistTunnelConnectSuccess(){}	//成功连接频道服务器
	////连接认证服务器超时
	public void OnAuthTunnelConnectTimeout(){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_AUTHCON_TIMEOUT;
		m_handler.sendMessage(msg);
	}		
	//连接大厅服务器超时
	public void OnLobbyTunnelConnectTimeout(){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_LOBBYCON_TIMEOUT;
		m_handler.sendMessage(msg);
	}		
	//连接房间服务器超时
	public void OnRoomTunnelConnectTimeout(){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_ROOMCON_TIMEOUT;
		m_handler.sendMessage(msg);
	}	
	public void OnChannellistTunnelConnectTimeout(){}//连接频道服务器超时
	//连接错误
	public void OnAuthTunnelConnectError( int code ){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_AUTHCON_FAILED;
		msg.arg1 = code;
		m_handler.sendMessage(msg);
	}	
	public void OnLobbyTunnelConnectError( int code ){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_LOBBYCON_FAILED;
		msg.arg1 = code;
		m_handler.sendMessage(msg);
	}
	public void OnRoomTunnelConnectError( int code ){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_ROOMCON_FAILED;
		msg.arg1 = code;
		m_handler.sendMessage(msg);
	}
	public void OnChannellistTunnelConnectError( int code ){}
	//服务器主动断开的连接,客户端recv == 0的时候,回调到以下的接口
	//断开认证服务器
	public void OnAuthTunnelClose(){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_AUTHCON_TIMEOUT;
		m_handler.sendMessage(msg);
	}	
	//断开大厅服务器
	public void OnLobbyTunnelClose(){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_LOBBYCON_CLOSE;
		m_handler.sendMessage(msg);
	}
	//断开房间服务器
	public void OnRoomTunnelClose(){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_ROOMCON_CLOSE;
		m_handler.sendMessage(msg);
	}	
	//断开频道服务器
	public void OnChannellistTunnelClose(){}
	//客户端recv异常,send异常,网络层buf溢出,select出现问题,都会回调到这个以下接口
	public void OnAuthTunnelError( int code ){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_AUTHCON_ERROR;
		msg.arg1 = code;
		m_handler.sendMessage(msg);
	}	
	public void OnLobbyTunnelError( int code ){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_LOBBYCON_ERROR;
		msg.arg1 = code;
		m_handler.sendMessage(msg);
	}
	public void OnRoomTunnelError( int code ){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_ROOMCON_ERROR;
		msg.arg1 = code;
		m_handler.sendMessage(msg);
	}
	public void OnChannellistTunnelError( int code ){}

	/*----------------------------认证服务器操作接口--------------------------------------*/
	//接收认证结果
	public void RecvAuth(int ret, int errcode,int again){
		Message msg = m_handler.obtainMessage();
		if(ret == 0)
			msg.what = XGMessage.XGMSG_AUTHLOGIN_SUCCESS;
		else
		{
			msg.what = XGMessage.XGMSG_AUTHLOGIN_FAILED;
			msg.arg1 = errcode;
		}
		msg.arg2 = again;
		m_handler.sendMessage(msg);
	}
	//接收新token
	//public void RecvNewToken(int ret,ReserveDataS pRD){}
	public void RecvNewToken(int ret,int errcode){}
	//接收频道列表
	public void RecvSectionList(int ret,int errcode,ResultData_GetChannellist RDc,int cseq){
		Message msg = m_handler.obtainMessage();
		if(ret == 0)
		{
			
			msg.what = XGMessage.XGMSG_LOBBYSECTION_SUCCESS;
			msg.arg1 = JniMessage.LOBBY_NORMAL;
			msg.obj = RDc;
		}
		else
		{
			msg.what = XGMessage.XGMSG_LOBBYLOGIN_FAILED;
			msg.arg1 = errcode;
		}
		m_handler.sendMessage(msg);
	}
	//接收频道列表(添加防沉迷信息)   ----忽略防沉迷信息
	public void RecvSectionList2(int ret,int errcode,int leftlogintime,ResultData_GetChannellist RDc,int cseq){}
	/*-------------------------大厅服务器操作接口------------------*/
	//接收大厅token认证
	public void RecvTokenAuth4Lobby( int ret, int errcode, ResultData_LobbyLogin pRDl, ResultData_SelfData pSD, int cseq ){
		Message msg = m_handler.obtainMessage();
		if(ret != 0)
		{
			//提示失败
			msg.what = XGMessage.XGMSG_LOBBYLOGIN_FAILED;
			msg.arg1 = errcode;
		}
		//成功的话，继续会在后台执行，所以不显示信息，还处于登陆大厅的状态
		m_handler.sendMessage(msg);
	}
	//用户登录大厅通知
	@Deprecated
	public void RecvUserLoginLobbyNotice(int ret,int idx,char relation,int lobbyid){
//		Message msg = m_handler.obtainMessage();
//		msg.what = XGMessage.XGMSG_USER_LOGIN;
//		msg.arg1 = idx;
//		m_handler.sendMessage(msg);
	}
	//用户退出大厅通知
	public void RecvUserLogoutLobbyNotice(int ret,int idx,char relation,int lobbyid){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_USER_LOGOUT;
		msg.arg1 = idx;
		m_handler.sendMessage(msg);
	}
	//接收用户信息
	//这里并没有用户信息，具体请使用GetUserInfoFromLocal();
	public void RecvUserInfo(int ret,int idx,ResultData_UserFullInfo RDu){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_USERINFO_NOTIFY;
		msg.arg1 = ret;
		msg.arg2 = idx;
		m_handler.sendMessage(msg);
	}
	//接收好友状态
	public void RecvFriendStatus(int ret,int errcode,ResultData_GetFriendlist RD){}
	//接收黑名单状态
	public void RecvBlackStatus(int ret,int errcode,ResultData_GetFriendlist RD){}
	//接收俱乐部状态
	public void RecvClubStatus(int ret,int errcode,ResultData_GetClublist RD){}
	//接收添加好友结果
	public void RecvAddFriend(int ret,int errcode,int idx){}
	//接收添加黑名单结果
	public void RecvAddBlack(int ret,int errcode,int cseq){}
	//接收删除好友结果
	public void RecvDelFriend(int ret,int errcode,int cseq){}
	//接收删除黑名单结果
	public void RecvDelBlack(int ret,int errcode,int cseq){}
	//接收申请加入俱乐部结果
	public void RecvApplyClub(int ret,int errcode,int cseq){}
	//接收退出俱乐部结果
	public void RecvExitClub(int ret,int errcode,int cseq){}
	//接收最近几天内的加入俱乐部申请数量
	public void RecvApplyListCount(int ret,int errcode,int count,int cseq){}
	//接收最近几天内的加入俱乐部申请列表
	public void RecvApplyList(int ret,int errcode,ResultData_ClubApplyList RD,int cseq){}
	//通过加入俱乐部审批
	public void RecvAddClubOk(int ret,ResultData_Approvelresult RD){}
	//删除申请列表
	public void RecvDelApplyList(int ret,ResultData_Approvelresult RD){}
	//接收加入俱乐部广播消息(当有人加入俱乐部)
	//modify by jinguanfu 2010/4/11
	//public void RecvAddClubNotice(int ret ,int idx){}
	//opidx--操作者idx
	//addidx--加入者idx
	public void RecvAddClubNoticeByLobby(int ret ,int clubid,String clubname,int opidx,int addidx){}
	public void RecvAddClubNoticeByRoom( int opidx, int addidx ){}
	//接收申请会员拒绝消息
	//opidx--审核者idx
	public void RecvDenyClubNoticeByLobby(int ret ,int clubid,String clubname,int opidx){}
	//public void RecvDenyClubNoticeByRoom(int ret ,int clubid,String clubname,int opidx){}
	//接收会员退出俱乐部消息
	public void RecvExitClubNoticeByLobby(int ret,int clubid,String clubname,int exitidx){}
	public void RecvExitClubNoticeByRoom( int exitidx ){}
	//接收会员被踢出俱乐部消息
	//opidx--操作者idx
	//kickidx--被踢出者idx
	public void RecvKickClubNoticeByLobby(int ret,int clubid,String clubname,int opidx,int kickidx){}
	public void RecvKickClubNoticeByRoom( int opidx, int kickidx ){}
	//创建俱乐部
	public void RecvCreateClub(int ret,int errcode,int clubid,int cseq){}
	//解散俱乐部
	public void RecvDismissClub(int ret,int errcode,int cseq){}
	//接收俱乐部信息
	public void RecvClubInfo(int ret,int errcode,ResultData_ClubFullInfo RD,int cseq){}
	////接收房间列表
	//public void RecvRoomList(int ret,ResultData_RoomList pRD){}
	//接收房间信息
	public void RecvRoomInfo(int ret,ResultData_RoomInfo RD){}
	//创建头像
	public void RecvCreateAvatar(int ret,int errcode){}
	//修改头像
	public void RecvChangeAvatar(int ret,int errcode){}
	//修改昵称
	public void RecvChangeNickName(int ret,int errcode, String name){}
	//进入房间通知
	public void RecvLobbyEnterRoom(int ret,ResultData_LobbyRoomEnter RD){}
	//离开房间通知
	public void RecvLobbyLeaveRoom(int ret,ResultData_LobbyRoomEnter RD){}
	//添加好友通知
	public void RecvAddFriendNotice(int ret,int idx){}
	//取得大厅信息
	public void RecvLobbyInfo(int ret,int errcode,ResultData_LobbyName RD,int cseq){}
	//接收来自大厅的权限更改消息 add by jinguanfu 2010/6/5
	public void RecvChangePower(int optionidx/*操作者ID*/,int identity/*更改后权限*/){}
	//取大厅名(CMD_SYSTEMMSG_L2C)
	//public void RecvLobbyName(){}
	/*------------------------房间操作接口------------------------------*/
	//接收房间token验证 //
	public void RecvTokenAuth4Room(int ret,int errcode){
		Message msg = m_handler.obtainMessage();
		if(ret == 0)
		{
			msg.what = XGMessage.XGMSG_ROOMAUTH_SUCCESS;
		}
		else
		{
			msg.what = XGMessage.XGMSG_ROOMAUTH_FAILED;
			msg.arg1 = ret;
			msg.arg2 = errcode;
		}
		m_handler.sendMessage(msg);
	}
	//进入房间
	public void RecvEnterRoom(int ret,int errcode,/*ResultData_LoginInfo RDlf*/byte retlogin,byte identity,int cseq){
		Message msg = m_handler.obtainMessage();
		if(ret == 0)
		{
			ResultData_LoginInfo RDlf = new ResultData_LoginInfo();
			RDlf.ret = retlogin;
			RDlf.identity = identity;
			
			msg.what = XGMessage.XGMSG_ROOMENTER_SUCCESS;
			msg.arg1 = cseq;
			msg.arg2 = 0;
			msg.obj = RDlf;
		}
		else
		{
			msg.what = XGMessage.XGMSG_ROOMENTER_FAILED;
			msg.arg1 = cseq;
			msg.arg2 = errcode;
		}
		m_handler.sendMessage(msg);
	}
	//用户进入房间通知
	public void RecvEnterRoomNotice(int ret,ResultData_UserInfo RDu){}
	//用户离开房间通知
	public void RecvLeaveRoomNotice(int ret,int idx){}
	//接收房间详细信息
	public void RecvRoomAllInfo(int ret,int errcode,ResultData_RoomAllInfo RD,int cseq){}
	//接收排麦成功通知
	public void RecvWaitMicNotice(int ret,ResultData_MicInfo RD){}
	////接收上麦准备通知
	//public void RecvWaitMicReadyNotice(int ret,ResultData_MicInfo pRD){}
	//接收上麦准备完成通知
	//public void RecvOnMicReadyOkNotice(int ret,ResultData_MicInfo pRD){}
	//接收上麦通知
	public void RecvOnMicNotice(int ret,ResultData_MicInfo RD){}
	//取消排麦成功通知
	public void RecvCancelWaitMicNotice(int ret,int opidx,int bopidx){}
	//接收加入PK麦通知
	public void RecvWaitPKMicNotice(int ret,ResultData_MicInfo RD){}
	//接收取消PK麦通知
	public void RecvCancelWaitPKMicNotice(int ret,int opidx,int bopidx){}
	//接收升麦成功通知
	public void RecvUpWaitMicNotice(int ret,ResultData_UDMicInfo RD){}
	//接收降麦成功通知
	public void RecvDownWaitMicNotice(int ret,ResultData_UDMicInfo RD){}
	//接收踢出房间通知
	public void RecvKickOutNotice(int idx,int bidx){}
	//接收上VJ麦（主持麦）通知
	public void RecvOnVJMicNotice(int ret,int idx){}
	//接收下VJ麦（主持麦）通知
	public void RecvOffVJMicNotice(int ret,int idx){}
	//接收被下麦通知
	public void RecvGiveOffMicNotice(int ret,int idx){}
	//接收被禁言通知
	public void RecvForbidenNotice(int ret,ResultData_ForbidenInfo RD){}
	//接收黑名单更新通知
	public void RecvUpdateBlackListNotice(int ret,ResultData_UpBlackList RD){}
	//接收管理员更新通知
	public void RecvUpdateMGRListNotice(int ret,ResultData_UpMgrList RD){}
	//接收赠送礼物通知
	public void RecvSendFlowerNotice(int ret,ResultData_SendFlowerInfo RD){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_GIFT_NOTIFY;
		msg.obj = RD;
		m_handler.sendMessage(msg);
	}
	//add by jinguanfu 2010/3/4 原有回调API改成接口<end>
	//接收房间重复登录消息
	public void RecvRoomKickUser(){}
	//接收大厅重复登录消息
	public void RecvLobbyKickUser(){}
	//从大厅接收房间简易信息
	public void RecvRoomSimpleInfo(int ret,int errcode,ResultData_RoomSimpleInfoList RD){}
	public void RecvRoomSimpleInfoEx(int ret,int errcode,ResultData_RoomSimpleInfoList RD, int type){}
	//接收用户退出房间会员回应
	public void RecvExitRoomMember(int ret,int errcode){}
	//接收WEB服务器上邮件信息
	public void RecvMailInfo(int ret,String context){}
	//接收大厅在线人数
	public void RecvLobbyOnlineNum(int ret,int errcode,ResultData_LobbyOnlineList RD){}
	//接收送礼回应消息	//add by jinguanfu 2010/4/27
	public void RecvSendFlower(int ret,int errcode,int recevie_idx,int item_id,int count,int gold,int silver){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_GIFTRESULT_NOTIFY;
		msg.arg1 = ret;
		msg.arg2 = errcode;
		m_handler.sendMessage(msg);
	}
	//接收发送大喇叭回应 //add by jinguanfu 2010/5/13
	public void RecvSpeakerToAll(int ret,int errcode,int gold,int silver){}
	//接收发送小喇叭回应 //add by jinguanfu 2010/5/13
	public void RecvSpeakerToLobby(int ret,int errcode,int gold,int silver){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_HORNRESULT_NOTIFY;
		msg.arg1 = ret;
		msg.arg2 = errcode;
		m_handler.sendMessage(msg);
	}
	//接收由管理员设为一般会员的回应 //add by jinguanfu 2010/5/13
	public void RecvRoomMember (int ret , int errcode, int idx,int cseq){}
	//接收由管理员设为一般会员的通知 //add by jinguanfu 2010/5/13
	public void RecvRoomNotice4Member (int ret , int opidx,int bopidx){}
	//接收房间内有人改变了avarat的广播 //add by cx 2010-5-20
	public void RecvRoomNotice4ChangeAvarat (int ret, int idx){}
	//接收房间内错误操作提示 //add by cx 2010-6-6
	public void RecvOneError( short cmd ){}
	//add by cx 2010-7-6 接受购买物品回应
	public void RecvBuyPack( int ret, int errcode, int gold, int silver, short [] goods_id, int [] global_id, int count, String day ){}
	// 接受购买物品回应
	public void RecvBuyForSend( int ret, int errcode, int idx, int gold, int silver ){}
	//接受获取背包回应
	public void RecvGetPack( int ret, int errcode, ResultData_Pack [] pPack, int count, int all_count ){}
	//接受删除物品回应
	public void RecvDelPack( int ret, int errcode, int cseq ){}
	//接受商品过期通知
	public void RecvTimeoutPack( int count, int [] global_id ){}
	//接受商品赠送
	public void RecvSendPack( int idx, int global_id, short goods_id, String day, String msg ){}
	//接受使用物品回应
	public void RecvUsePack( int ret, int errcode, int global_id ){}
	// 升级广播
	public void RecvNotice4LevelUp( int idx, int level ){}
	// 经验提升
	public void RecvExpChange( int nexp, int level ){}
	// vip改变
	public void RecvVipChange( int idx, int level, int is_vip ){}
	// 钱币改变
	public void UpdataMoney( int r_gold, int r_silver ){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_USERGOLD_UPDATE;
		msg.arg1 = r_gold;
		m_handler.sendMessage(msg);
	}
	// 更新自己的信息
	public void RecvUpdataMyInfo(){}
	// 接受烟花广播
	public void RecvFireworks( int ret, int errcode, int cate_idx, int count, int gold, int silver ){}
	// 接受烟花广播
	public void RecvFireworksNotice( int player_idx, int cate_idx, int count ){}
	// 中奖？
	public void RecvLucky( int player_idx, int cate_idx, int lotte ){}
	// 中奖？
	public void RecvLucky( int room_idx, int player_idx, int cate_idx, int lotte ){}
	// 列表
	public void RecvGiftTopList( ResultData_GiftList gift_list ){}
	// 在线时间
	public void RecvOnlineTime( int idx, int day_time, int online ){}
	// 获取新手任务
	public void RecvGetNewUserTask( short step ){}
	// 返回新手任务提交结果
	public void RecvTaskStep( int ret, int err_code, int gold, int silver, int total_gold, int total_silver ){}
	public void RecvTaskGift( int ret, int err_code, int item_id, int object_id, String time_buf ){}
	// 商品信息更改通知
	public void RecvNotice4ChangeGoods(){}
	// 服务器请求刷新背包
	public void RecvGetBagAgain(){}
	// 返回用户印章
	public void RecvUserSeel( int user_idx, int seel_idx ){}
	// 返回用户防沉迷状态
	public void EnterAllMentState( int idx, short state ){}
	// room loge
	public void RecvRoomLoge( int room_idx, String loge, int len ){}
	// 新分组版本号
	public void RecvNewVersionId( int version_id ){}
	// 分组信息
	public void RecvGroupInfo( int count, ResultData_GroupInfo Info ){}
	// 获取组里面的房间列表
	public void RecvRoomListInGroup( short group_idx, int count, int [] list ){}
	public void RecvRoomListInGroup(short count, ResultData_GroupRoomInfo[] pInfo){}
	// 上传日志通知
	public void RecvNotice4CollectData( String server_ip, short server_port ){}
	// 获取歌曲信息
	public void RecvSongInfoList( short totalnum, short curnum, ResultData_SongInfo info ){}
	// 获取音视频服务器列表
	public void RecvAvServerList( ResultData_AvServer [] list, int count ){}
	// 更新音视频服务器列表
	public void UpdataAvServerList(){}
	// 被提出游戏服务器
	public void RecvCheckOutGame( int result ){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.MSG_BEGIN;
		msg.arg1 = result;
		m_handler.sendMessage(msg);
	}
	// 获取离线消息
	public void RecvOffLineMsg( int count, ResultData_OffLineMsg [] msg ){}
	// 返回分组置顶信息
	public void RecvRoomSortInGroup( int idx, int count, ResultData_RoomSort [] list ){}
	// 通知分组置顶信息改变
	public void RecvRoomSortChange( int idx ){}
	// 歌手等级
	public void RecvSingerLevel(int idx, short curlevel, int expvalue){}
	// 财富等级
	public void RecvWealthLevel(int idx, short curlevel, int expvalue){}

	//////////////////////////////////////////////////////////////////////////

	// 礼物失效（印章）的协议
	public void RecvGiftInvalid( int idx, int GiftID ){}
	// 礼物有效（印章）的协议
	public void RecvGiftValid( int idx, int GiftID ){}
	// 显示房间可领取收益
	public void RecvShowRoomIncom( int income ){}
	// 显示收益已领取记录
	public void RecvShowRoomGetIncom( ResultData_IncomeDate [] data, int count ){}
	// 室主领取房间内收益
	public void RecvGetRoomIncom( int ret ){}
	// 礼物信息更改通知
	public void RecvNotice4ChangeFlower(){
		//m_handler.sendEmptyMessage(XGMessage.XGMSG_GIFT_UPDATE);
	}
	// 权限配置更新通知
	public void RecvNotice4ChangePower(){}
	// 更改音视频服务器广播
	public void RecvNotice4AvserverChange( ResultData_AvChange info ){}
	// 更改gm
	public void RecvChangeGM( int idx, int add ){}
	// 上麦的人的pc概况
	public void RecvMicPcInfo( int idx, String buf ){}
	// 回应是否邀加入房间成功
	public void RecvSendJoinRoomInvite( int wseq, int ret, int err_code ){}
	// 回应是否邀加入房间成功
	public void RecvInviteJoinRoom( int idx ){}
	//取得当日礼物赠送记录
	public void RecvGiftSend( int count, ResultData_GiftCount [] pInfo ){}
	//取得当日礼物接收记录
	public void RecvGiftRecv( int count, ResultData_GiftCount [] pInfo ){}
	//返回封mac结果
	public void RecvKickUserByMac( int ret, int idx ){}
	//返回封ip结果
	public void RecvKickUserByIp( int ret, int idx ){}
	//返回设置在麦时间结果
	public void RecvSetMicTime( int ret, int idx ){}
	//广播在麦时间延长
	public void RecvNotice4SetMicTime( int idx, int time ){}
	//取排麦列表
	public void RecvWaitMicList( short count, int[] idxs ){}
	//防沉迷时间到,断开连接
	public void RecvIndulgeTimeUp(){}
	/*家族信息回调
	参数：
	flag：
	1:删除家族
	2:添加或更新家族
	familyNum：
	家族数量(N)
	familys：
	家族信息，是一个数组familys[N-1]{}
	注意：当flag为1时，familyNum必定为1，同时familys[0]里只有familyid是可用的(因为删除的时候，一次只能删除一个，只有家族id信息)
	*/
	public void RecvFamilyInfo(int flag, int familyNum, ResultData_Family[] familys){}

	/*
	av配置信息
	*/
	public void RecvAVConfigInfo(int infolen, String info){}

	//
	/*------------------------------私麦---------------------------------------*/
	/*上私麦*/
	public void RecvOpenPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
	public void RecvOpenPrivateMicFailed(int errorcode, int idx, int bidx){}
	public void RecvOpenPrivateMicNotice(int idx, int mictype){} //不包括自己
	
	/*下私麦*/
	public void RecvClosePrivateMicFailed(int errorcode, int idx, int bidx){}
	public void RecvClosePriveteMicNotice(int idx){} //包括自己
	
	/*被抱下私麦*/
	public void RecvClosedPrivateMicFailed(int errorcode, int idx, int bidx){}
	public void RecvClosedPriveteMicNotice(int idx, int bidx){} //包括自己
	
	/*观看私麦*/
	public void RecvWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
	public void RecvWatchPrivateMicFailed(int errorcode, int idx, int bidx){}
	public void RecvRequestWatchedPrivateMic(int idx){} /*idx=请求者, 是否同意该idx观看你的私麦*/
	public void RecvWatchedPrivateMicFailed(int errorcode, int idx, int bidx){}

	public void RecvWatchedPrivateMicReject(int idx){} //对方拒绝，idx=谁拒绝的

	/*邀请私麦*/
	public void RecvInviteWatchPrivateMicSuccess(){}
	public void RecvInviteWatchPrivateMicFailed(int errorcode, int idx, int bidx){}
	public void RecvRequestInvitedWatchPrivateMic(int idx){} /*idx=邀请者,是否同意观看idx的私麦*/
	public void RecvInvitedWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
	public void RecvInvitedWatchPrivateMicFailed(int errorcode, int idx, int bidx){}

	public void RecvInvitedWatchPrivateMicReject(int idx){}//对方拒绝，idx=谁拒绝的

	/*管理房间私麦*/
	public void RecvOpenRoomPrivateMicFlag(int ret, int flag){}
	public void RecvOpenRoomPrivateMicFlagNotice(int idx, int flag){} //包括自己

	/*用户登录房间时，回调房间的私麦信息*/
	public void RecvPrivateMicWithLoginRoom(int idx, short mictype, int posindex, int roomid){}
	/*	返回皇冠踢人结果
		int ret
		 0: 成功 
		-1: 失败，非法操作
		-2: 失败，剩余人次数不足
		int kickuse
		本次操作消耗人次
		int totalkick
		剩余消耗人次数
		int idx{}	
		被踢者idx
	*/
	public void RecvCrownKickoutSomeOne(int ret, int kickuse, int totalkick, int idx){}
	/*室主禁止用户ip
	int ret{}
	 0：成功
	 -1：该IP已被禁用
	 -2：该用户已离开房间
	
	int idx{}
	被禁用者idx
	*/
	public void RecvForbidUserIp(int ret, int idx){}

	//要求上传歌曲
	public void RecvUpdateSong(){}
	//回应服务器的返回用户歌唱信息
	//public void RecvUserSingInfo(int ret,int idx,const ResultData_UserSingInfo pData){}
	//获取用户接受礼物个数
	public void RecvGiftCount(int recvcount){}

	//获取用户最高分歌曲信息及演唱歌曲个数
	public void RecvTopSongInfo(int songcount, int topsongscore, String topsongname){}
	  
	  ///////////////////////////////////////////////////手机新增回调////////////////////////////////////////////////////////////
	//手机ktv聊天信息
	    /*
	     @param ret:返回值，0,OK{}其他值,something error
	     @param fromIdx:来自谁,如果发生错误，也有可能指发送的IDX
	     @param roomName:说话者在的房间，如果未在房间则为“0”
	     @param content:说的内容
	     @param chatTime:说话时间
	     @param isPublic:是否为公聊
	     
	     @注:chattime
	     */
	public void RecvKtvChatInfo(int ret,int fromIdx,int roomid,String roomName,String content,
	                                 int chatTime,boolean isPublic,boolean isSend){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_CHAT_NOTIFY;
		msg.arg1 = ret;
		if(ret == 0)
	    {
	        ChatInfoToUi chatInfo = new ChatInfoToUi();
	        chatInfo.fromIdx = fromIdx;
	        chatInfo.roomid = roomid;
	        chatInfo.room = roomName;
	        chatInfo.content = content;
	        chatInfo.chatTime = chatTime;
	        chatInfo.isPublic = isPublic;
	        chatInfo.isSend = isSend;
	        msg.obj = chatInfo;
	    }
	    m_handler.sendMessage(msg);
	}
	    //手机接收到当前的播放列表，即当前ktv的点歌列表，用于更新
	    /*
	     @param pSonglist:信息结构体
	     @param listcount:歌曲数量
	     */
	public void RecvKtvSongList(ResultData_SongList[] pSonglist,int listCount){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_CUR_SONGLIST;
		msg.obj = pSonglist;
		m_handler.sendMessage(msg);
	}
	    //手机获取当前包厢的开关状态
	    /*
	     @param isOpen:true,包厢开启；false，包厢关闭，用户退出房间
	     */
	public void RecvKtvRoomState(boolean isOpen){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_ROOMSTAT_UPDATE;
		msg.arg1 = isOpen?1:0;
		m_handler.sendMessage(msg);
	}
	    //手机收到唱歌上榜通知,提示用户到个人信息里面去抢榜
	@Deprecated
	public void RecvKtvSongRanking(){}
    /*
	通知歌曲演唱结束
	*/
	public void RecvKtvSongNormalEnd(int nMusicId){
		m_handler.sendEmptyMessage(XGMessage.XGMSG_NORMAL_SONGEND);
	}
    /*
     收到的在线手机用户IDX列表更新数据
     只有IDX
     */
	@Deprecated
	public void RecvKtvIdxList(int[] pIdx,int number){
		
	}
		/*
		通知用户头像更新
		@param [in] idx：指定的用户IDX
		@param [in] picture：用户的新头像
		*/
	public void RecvUserPicChange(int idx,String picture){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_USERPIC_UPDATE;
		msg.arg1 = idx;
		msg.obj = picture;
		m_handler.sendMessage(msg);
	}
	public void RecvUserNameChange(int idx,String name){
		if(TextUtils.isEmpty(name))
			return ;
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_USERNAME_UPDATE;
		msg.arg1 = idx;
		msg.obj = name;
		m_handler.sendMessage(msg);
	}
	/*
	通知获取到的喇叭用户信息
	*/
	public void RecvUserInfoForSpeaker(int errcode,ResultData_UserSampleInfo info){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_USERINFO_FORHORN;
		msg.arg1 = errcode;
		msg.obj = info;
		m_handler.sendMessage(msg);
	}
    /*
     接受发喇叭者信息超时
     需要判读idx数组中的具体idx
     @param [in] idx:idx数组； 数组中某个IDX如果为0，则原来的IDX已经获取到信息，只是底层把它置为0
     @param [in] count:idx数组大小
    */
	public void RecvUserInfoForSpeakerTimeOut(int idx[],int count){}
    /*
     接受用户信息超时
     需要判读idx数组中的具体idx
     @param [in] idx:idx数组； 数组中某个IDX如果为0，则原来的IDX已经获取到信息，只是底层把它置为0
     @param [in] count:idx数组大小
     */
	public void RecvUserInfoTimeOut(int idx[],int count){}
	/*
	 * 接收到通知消息
	 * @param [in] msgId:通知ID
	 * @param [in] msgContent:通知内容 以'|'分隔 目前暂定这样 title|content|url
	 */
	public void RecvNotifyMessage(int msgId,String msgContent){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_NOTIFY_MESSAGE;
		msg.arg1 = msgId;
		msg.obj = msgContent;
		m_handler.sendMessage(msg);
	}
    //单个礼物图片下载完成通知
    //可以在这里去对之前没有获取到图片的地方尝试再去获取一下,或许已经下载下来了
    /*
     @param pictureName[in]:图片名称(含路径)
     */
	public void RecvImageDownladFinished(String pictureName){
		Log.i("CJniUtil", pictureName+" is downloaded.");
	}
	
	public void RecvUserIdxAndType(ResultData_UIdxWithType[] list){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_KTVUSER_ONLINE;
		msg.obj = list;
		m_handler.sendMessage(msg);
	}

	/*
	收到服务器的通知，有用户登入，这个是新协议，为了兼容老版本，所以新版本的客户端对原来的回调屏蔽了RecvUserLoginLobbyNotice
	@param idx: 用户的idx
	@param type : 用户的在线方式 ： 0-主播，1-本包厢，2-其他包厢主备，3-大厅

	@注：这个接口还包含了用户在线方式的改变，比如从大厅到包厢，所以需要先判断用户是否在列表中，如果在列表中则修改，如果不在则添加
	*/
	public void RecvUserLoginLobbyNoticeNew(int idx,byte type){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_USER_LOGIN;
		msg.arg1 = idx;
		msg.arg2 = type;
		m_handler.sendMessage(msg);
	}

	/*
	收到发送红包的结果
	@param ret : 红包发送结果: 
						0-成功  1-金币不足  2-对方不在线（个人红包） 3-不在包厢（包厢红包）	9999-服务器相应超时
	*/
	public void RecvSendHBResult(int ret){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_HONGBAO_SENDRESULT;
		msg.arg1 = ret;
		m_handler.sendMessage(msg);
	}

	/*
	收到互动红包
	@param idx : 发红包的用户的idx
	@param hbid : 红包ID
	@param hbtype : 红包类型
			0-个人  1房间手气 2-房间均分  3-大厅手气 4-大厅均分 5-新人 6-K歌 7-消费
	@param remark : 发红包用户的备注信息
	*/
	public void RecvHongBao(int idx,String hbid,byte hbtype,String remark){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_HONGBAO_RECV;
		HongBaoInfoToUi info = new HongBaoInfoToUi();
		info.idx = idx;
		info.hbid = hbid;
		info.hbtype = hbtype;
		info.remark = remark;
		msg.obj = info;
		m_handler.sendMessage(msg);
	}

	/*
	 * 查询红包的状态结果的返回
	 *
	 * @param hbid : 红包ID
	 * @param ret : 0-可抢 1-已抢完 2-已抢过 9999-服务器回应超时
	 */
	public void RecvHongBaoStatus(String hbid,int ret){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_HONGBAO_STATUS;
		msg.arg1 = ret;
		msg.obj = hbid;
		m_handler.sendMessage(msg);
	}
	
	/*
	抢红包的结
	@param result : 抢红包的结果: 0-没抢到  1-抢到 2-已抢 9999-服务器相应超时 OPTION_TIMEOUT
	@param idx : 发红包用户的IDX
	@param bhid : 红包ID
	@param hbtype : 红包类型
	@param remark : 红包备注
	@param gold : 抢到金额(没抢到发0)
	*/
	public void RecvVieHongBaoResult(int result,int idx,String hbid,byte hbtype,String remark,int gold){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_HONGBAO_VIERESULT;
		msg.arg1 = result;
		
		HongBaoInfoToUi info = new HongBaoInfoToUi();
		info.idx = idx;
		info.hbid = hbid;
		info.hbtype = hbtype;
		info.remark = remark;
		info.gold = gold;
		msg.obj = info;
		
		m_handler.sendMessage(msg);
	}

	/*
	红包详情
	@param ret : 结果返回，0-成功；9999-服务器相应超时
	@param idx : 发红包的用户IDX
	@param hbtype : 红包类型
	@param remark : 红包备注
	@param totalgold : 金币总数
	@param totalnum : 总的红包个数
	@param list : 抢到红包的用户列表[num]
	@param num : list列表的数量
	*/
	public void RecvHongBaoDetail(int ret,int idx,byte hbtype,String remark,int totalgold,int totalnum
			,int sendtime,ResultData_HBDetail[] list){
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_HONGBAO_DETAIL;
		
		HongBaoDetailInfoToUi info = null;
		if(ret == 0)
		{
			info = new HongBaoDetailInfoToUi();
			info.idx = idx;
			info.hbtype = hbtype;
			info.remark = remark;
			info.totalgold = totalgold;
			info.totalnum = totalnum;
			info.sendtime = sendtime;
			info.list = list;
		}
		
		msg.obj = info;
		msg.arg1 = ret;
		m_handler.sendMessage(msg);
	}
	
	/*
	通知最近一次唱歌记录
	@param songname : 歌曲名
	@param singername : 歌手名
	@param score : 分数
	*/
	public void RecvSingSongChange(String songname,String singername,int score)
	{
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_SINGSONG_RECENT;
		ResultData_RecentSong info = new ResultData_RecentSong();
		info.songname = songname;
		info.singername = singername;
		info.score = score;
		msg.obj = info;
		m_handler.sendMessage(msg);
	}

	/*
	通知红包数量更新
	@param number : 可领取的红包数量
	*/
	public void RecvHongBaoNum(int number)
	{
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_HONGBAO_CANGET;
		msg.arg1 = number;
		m_handler.sendMessage(msg);
	}
	
	/*
	当前包厢红包返回

	@param ret : 0-正常获取 9999-服务器超时
	@param number : list[number];
	@param list : 红包列表
	*/
	public void RecvRoomHB(int ret,ResultData_RoomHB[] list)
	{
		//超时的就不给了！
		if(ret == JniMessage.OPTION_TIMEOUT)
			return ;
		
		Message msg = m_handler.obtainMessage();
		msg.what = XGMessage.XGMSG_HONGBAO_ROOM;
		msg.obj = list;
		m_handler.sendMessage(msg);
	}
	
	/*
	     主线程或回调，在初始化下载地址之后成功的话就会回调，通知本地数据库礼物信息已经完成配置
	*/
	public void RecvLocalGiftDatabaseFinished(){
		m_handler.sendEmptyMessage(XGMessage.XGMSG_GIFTDATA_FINISHED);
	}
	
	private Handler m_handler = null;
}
