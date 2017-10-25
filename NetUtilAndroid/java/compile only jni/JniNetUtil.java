/*
 * 
 *   by glp
 *   2014-4-9   
 *   定义提供给c netutil的java回调接口，c netutil内部会调用
 *   具体接口内容请看相应函数说明，需要自己去实现相应业务
 * 
 */

package com.jni.netutil;

public interface JniNetUtil{
	//聊天、喇叭功能
	  void SendChatFromUser(int ret, int to_idx, String content, int time );
	//接收私聊
	  void RecvChatFromUser(int ret, int from_idx, String content, int time );
	//接收公聊（俱乐部内部）
	  void RecvChatFromClub(int ret, int from_idx, String content);
	//接收公聊（房间内）
	//ret:1--正常 0--被禁言 -1--不允许公聊
	  void RecvChatFromRoom(int ret, int errcode,int from_idx, String content, int time );	
	//接收大喇叭（全区）
	  void RecvSpeakerFromAll(int ret, int type, int from_id, String msg);
	//接收小喇叭（大厅）
	  //int ret, int from_idx, int room_id,const char* room_name,const char* msg, int time,int type,bool isSend
	  void RecvSpeakerFromLobby(int ret, int from_idx,String room_name, String msg, int time,int type,boolean isSend );
	//接收礼物广播
	  void RecvPresentBroad(int ret, ResultData_GiftInfo guestGiftInfo, ResultData_GiftInfo commGiftInfo);
	//接受系统文字广播
	  void RecvSystemBroad( int type, String msg);
	//接受系统文字广播（幸运中奖信息）
	  void RecvSystemBroad2(String nickname, int idx, int roomid, int giftid, int luckmultiple);
	//接受系统文字公告
	  void RecvSystemNotice( String msg, boolean once, int min );
	//接受系统文字公告（幸运中奖信息）
	  void RecvSystemNotice2(String nickname, int idx, int roomid, int giftid, int luckmultiple);
	//接收房间临时公告
	  void RecvRoomNotice(int ret, String content);
	//取得好友位置
	  void RecvFriendLocation(int ret, int errcode,int friend_idx,int lobbyid, int roomid,int cseq);
	/*			 好友操作					*/
	/*			房间管理接口			*/
	//接收取得待审核房间会员列表
	  void RecvRoomApplyList (int ret ,int errcode,ResultDate_RoomApplyList applylist,int cseq);
	//接收申请房间会员响应
	  void RecvApplyRoomMember(int ret,int errcode,int cseq);
	//接收审核会员加入申请结果
	  void RecvAuditAddApply (int ret, int errcode,int idx,int cseq);
	//接收拒绝房间申请系统提示
	  void RecvRoomApplyRefuseMsg (int ret, int roomid);
	//接收房间会员列表
	  void RecvRoomMemberList (int ret ,int errcode, ResultDate_RoomMemList memlist,int cseq);
	//接收解除房间会员资格回应
	  void RecvUnchainMember (int ret, int errcode, int idx,int cseq);
	//接收赋予用户房间主持助理权限回应
	  void RecvRoomPesideAssistant (int ret , int errcode, int idx,int cseq);
	//接收赋予主持助理通知
	  void RecvRoomNotice4PesideAssistant (int ret , int opidx,int bopidx);
	//接收赋予用户房间主持人权限回应
	  void RecvRoomPeside (int ret ,int errcode, int idx,int cseq);
	//接收设置主持人权限房间通知
	  void RecvRoomNotice4Peside (int ret , int opidx,int bopidx);
	//接收赋予用户房间副室主权限回应
	  void RecvRoomSubHost (int ret ,int errcode, int idx,int cseq);
	//接收设置副室主权限房间通知
	  void RecvRoomNotice4SubHost(int ret , int opidx,int bopidx);
	//接收房间黑名单
	  void RecvBlackList(int ret,	int errcode,ResultDate_RoomBlackList blacklist,int cseq);
	//接收黑名单用户添加/删除回应
	  void RecvAddDelBlack(int ret , int errcode,int idx, int type);
	//接收房间设置面板信息
	  void RecvRoomPanelInfo(int ret ,int errcode, ResultDate_RoomPanel panelinfo);
	//接收设置房间密码回应
	  void RecvRoomPassword(int ret,int errcode,int cseq);
	//接收设置房间密码消息提示
	  void RecvRoomNotice4Password(int ret, int idx, int pwdflag);
	//接收设置房间开启/关闭(上锁)回应
	  void RecvRoomSwitch(int ret,int errcode,int roomflag,int cseq);
	//接收房间开启/关闭(上锁)消息提示
	  void RecvRoomNotice4RoomSwitch(int ret , int idx , int roomflag);
	//接收设置房间访问权限回应
	  void RecvRoomPrivate(int ret , int errcode,int accessflag,int cseq);
	//接收房间访问权限消息提示
	  void RecvRoomNotice4Private(int ret , int idx , int accessflag);
	//接收设置用户进出信息开启/关闭回应
	  void RecvUserInOutSwitch(int ret , int errcode, int inoutflag,int cseq);
	//接收用户进出信息开启/关闭消息提示
	  void RecvRoomNotice4UserSwitch (int ret , int idx , int inoutflag);
	//接收设置自由排麦开启/关闭回应
	  void RecvGetMicSwitch (int ret, int errcode,int onmicflag,int cseq);
	//接收设置自由上麦开启/关闭回应
	  void RecvOnMicSwitch (int ret, int errcode,int onmicflag,int cseq);
	//接收自由排麦开启/关闭消息提示
	  void RecvRoomNotice4GetMicSwitch(int ret , int idx , int getmicflag);
	//接收自由上麦开启/关闭消息提示
	  void RecvRoomNotice4OnMicSwitch(int ret , int idx , int onmicflag);
	//房间名称修改回应
	  void RecvModifyRoomName(int ret,int errcode,int cseq);
	//接收房间名
	  void RecvRoomName (int ret , int roomid , String roomname);
	//设置房间公告响应
	  void RecvRoomAffiche(int ret,int errcode,int cseq);
	//接收房间公告更新事件
	  void RecvRoomAfficheEvent(int ret);
	//设置允许房间公聊响应
	  void RecvAllowRoomAllChat (int ret, int errcode,int chatflag,int cseq);
	//接收房间公聊设置消息提示
	  void RecvRoomNotice4AllChat(int ret , int idx , int chatflag);
	//设置房间欢迎词响应
	  void RecvRoomSalutatory(int ret, int errcode,int cseq);
	//接收房间欢迎词更新通知
	  void RecvRoomNotice4Salutatory(int ret,int idx);
	//设置房间logo响应
	  void RecvRoomLogo (int ret,int errcode ,int cseq);
	//接收房间LOGO更新通知
	  void RecvRoomNotice4Logo(int ret,int idx);
	//接收房间冻结消息提示
	  void RecvRoomNotice4Lock(int idx);
	//接受创建房间回应
	  void RecvCreateRoom( int ret, int errcode, RoomListEx info );
	//接受练歌房完全信息
	  void RecvTmpRoomInfo( int ret, int errcode, RoomListEx info );
	// 添加歌房广播
	  void RecvRoomNotice4Add( RoomListEx info );
	  void RecvRoomNotice4Add( int idx );
	// 删除歌房广播
	  void RecvRoomNotice4Del( int idx );
	// 接收房间易主通知
	  void RecvRoomNotice4ChangeMaster(int idx);
	// 接收房间信息更改通知
	  void RecvRoomNotice4ChangeInfo( int idx );
	/*			房间管理接口				*/
	/*			 角色信息				*/
	//取得他人角色信息
	  void RecvPartInfo(int ret, int errcode, ResultData_PartInfo part_info, int cseq);
	  
	  //取得自己角色信息 专用于java的接口 登陆大厅最后的返回结果 @glp
	  //@注：Android独有的回调函数用于在大厅登陆最后一步的回调
	  void OnLoginLobby(int ret, int errcode, LocalData_UserInfo userInfo);
	//取得自己角色信息
	  void RecvPartInfoBySelf(int ret, int errcode, ResultData_PartInfo part_info, int cseq);
	//角色信息
	//取得房间列表
	  void RecvRoomList(int ret,ResultDate_RoomList roomlist);
	  void RecvRoomListEx(int ret,ResultDate_RoomListEx roomlist);
	//下麦通知
	  void RecvNotice4OffMic( int onmic_idx, int pk_idx, int exp, String scoreinfo);
	// add by cx 10-6-2
	//回应是否邀请上麦成功
	  void RecvRoomGivOnevjMic( int idx, int flag );
	//是否同意邀请上麦成功
	  void RecvRoomGivOnevjMicApply( int formidx );
	//是否同意添加对方为好友
	//参数说明：int idx 添加好友者idx
	  void RecvAddFriendApply(int idx);
	//  void RecvDeleteAvarta(int ret);
	//add by jinguanfu 2010/3/4 原有回调API改成接口<begin>
	//连接成功
	  void OnAuthTunnelConnectSuccess();			//成功连接认证服务器
	  void OnLobbyTunnelConnectSuccess();		//成功连接大厅服务器
	  void OnRoomTunnelConnectSuccess();			//成功连接房间服务器
	  void OnChannellistTunnelConnectSuccess();	//成功连接频道服务器
	//连接超时
	  void OnAuthTunnelConnectTimeout();		//连接认证服务器超时
	  void OnLobbyTunnelConnectTimeout();		//连接大厅服务器超时
	  void OnRoomTunnelConnectTimeout();		//连接房间服务器超时
	  void OnChannellistTunnelConnectTimeout();//连接频道服务器超时
	//连接错误
	  void OnAuthTunnelConnectError( int code );	
	  void OnLobbyTunnelConnectError( int code );
	  void OnRoomTunnelConnectError( int code );
	  void OnChannellistTunnelConnectError( int code );
	//服务器主动断开的连接,客户端recv == 0的时候,回调到以下的接口
	  void OnAuthTunnelClose();	//断开认证服务器
	  void OnLobbyTunnelClose();	//断开大厅服务器
	  void OnRoomTunnelClose();	//断开房间服务器
	  void OnChannellistTunnelClose();//断开频道服务器
	//客户端recv异常,send异常,网络层buf溢出,select出现问题,都会回调到这个以下接口
	  void OnAuthTunnelError( int code );	
	  void OnLobbyTunnelError( int code );
	  void OnRoomTunnelError( int code );
	  void OnChannellistTunnelError( int code );

	/*----------------------------认证服务器操作接口--------------------------------------*/
	//接收认证结果
	  void RecvAuth(int ret, int errcode,int again);
	//接收新token
	//  void RecvNewToken(int ret,ReserveDataS pRD);
	  void RecvNewToken(int ret,int errcode);
	//接收频道列表
	  void RecvSectionList(int ret,int errcode,ResultData_GetChannellist RDc,int cseq);
	//接收频道列表(添加防沉迷信息)
	  void RecvSectionList2(int ret,int errcode,int leftlogintime,ResultData_GetChannellist RDc,int cseq);
	/*-------------------------大厅服务器操作接口------------------*/
	//接收大厅token认证
	  void RecvTokenAuth4Lobby( int ret, int errcode, ResultData_LobbyLogin pRDl, ResultData_SelfData pSD, int cseq );
	//用户登录大厅通知
	  void RecvUserLoginLobbyNotice(int ret,/*ResultData_LobbyLoginUser pRlu*/
				int idx,char relation,int lobbyid);
	//用户退出大厅通知
	  void RecvUserLogoutLobbyNotice(int ret,/*ResultData_LobbyLoginUser pRlu*/
				int idx,char relation,int lobbyid);
	//接收用户信息
	  void RecvUserInfo(int ret,int idx,ResultData_UserFullInfo RDu);
	//接收好友状态
	  void RecvFriendStatus(int ret,int errcode,ResultData_GetFriendlist RD);
	//接收黑名单状态
	  void RecvBlackStatus(int ret,int errcode,ResultData_GetFriendlist RD);
	//接收俱乐部状态
	  void RecvClubStatus(int ret,int errcode,ResultData_GetClublist RD);
	//接收添加好友结果
	  void RecvAddFriend(int ret,int errcode,int idx);
	//接收添加黑名单结果
	  void RecvAddBlack(int ret,int errcode,int cseq);
	//接收删除好友结果
	  void RecvDelFriend(int ret,int errcode,int cseq);
	//接收删除黑名单结果
	  void RecvDelBlack(int ret,int errcode,int cseq);
	//接收申请加入俱乐部结果
	  void RecvApplyClub(int ret,int errcode,int cseq);
	//接收退出俱乐部结果
	  void RecvExitClub(int ret,int errcode,int cseq);
	//接收最近几天内的加入俱乐部申请数量
	  void RecvApplyListCount(int ret,int errcode,int count,int cseq);
	//接收最近几天内的加入俱乐部申请列表
	  void RecvApplyList(int ret,int errcode,ResultData_ClubApplyList RD,int cseq);
	//通过加入俱乐部审批
	  void RecvAddClubOk(int ret,ResultData_Approvelresult RD);
	//删除申请列表
	  void RecvDelApplyList(int ret,ResultData_Approvelresult RD);
	//接收加入俱乐部广播消息(当有人加入俱乐部)
	//modify by jinguanfu 2010/4/11
	//  void RecvAddClubNotice(int ret ,int idx);
	//opidx--操作者idx
	//addidx--加入者idx
	  void RecvAddClubNoticeByLobby(int ret ,int clubid,String clubname,int opidx,int addidx);
	  void RecvAddClubNoticeByRoom( int opidx, int addidx );
	//接收申请会员拒绝消息
	//opidx--审核者idx
	  void RecvDenyClubNoticeByLobby(int ret ,int clubid,String clubname,int opidx);
	//  void RecvDenyClubNoticeByRoom(int ret ,int clubid,String clubname,int opidx);
	//接收会员退出俱乐部消息
	  void RecvExitClubNoticeByLobby(int ret,int clubid,String clubname,int exitidx);
	  void RecvExitClubNoticeByRoom( int exitidx );
	//接收会员被踢出俱乐部消息
	//opidx--操作者idx
	//kickidx--被踢出者idx
	  void RecvKickClubNoticeByLobby(int ret,int clubid,String clubname,int opidx,int kickidx);
	  void RecvKickClubNoticeByRoom( int opidx, int kickidx );
	//创建俱乐部
	  void RecvCreateClub(int ret,int errcode,int clubid,int cseq);
	//解散俱乐部
	  void RecvDismissClub(int ret,int errcode,int cseq);
	//接收俱乐部信息
	  void RecvClubInfo(int ret,int errcode,ResultData_ClubFullInfo RD,int cseq);
	////接收房间列表
	//  void RecvRoomList(int ret,ResultData_RoomList pRD);
	//接收房间信息
	  void RecvRoomInfo(int ret,ResultData_RoomInfo RD);
	//创建头像
	  void RecvCreateAvatar(int ret,int errcode);
	//修改头像
	  void RecvChangeAvatar(int ret,int errcode);
	//修改昵称
	  void RecvChangeNickName(int ret,int errcode, String name);
	//进入房间通知
	  void RecvLobbyEnterRoom(int ret,ResultData_LobbyRoomEnter RD);
	//离开房间通知
	  void RecvLobbyLeaveRoom(int ret,ResultData_LobbyRoomEnter RD);
	//添加好友通知
	  void RecvAddFriendNotice(int ret,int idx);
	//取得大厅信息
	  void RecvLobbyInfo(int ret,int errcode,ResultData_LobbyName RD,int cseq);
	//接收来自大厅的权限更改消息 add by jinguanfu 2010/6/5
	  void RecvChangePower(int optionidx/*操作者ID*/,int identity/*更改后权限*/);
	//取大厅名(CMD_SYSTEMMSG_L2C)
	//  void RecvLobbyName();
	/*------------------------房间操作接口------------------------------*/
	//接收房间token验证 //
	  void RecvTokenAuth4Room(int ret,int errcode);
	//进入房间
	  void RecvEnterRoom(int ret,int errcode,/*ResultData_LoginInfo RDlf*/byte retlogin,byte identity,int cseq);
	//用户进入房间通知
	  void RecvEnterRoomNotice(int ret,ResultData_UserInfo RDu);
	//用户离开房间通知
	  void RecvLeaveRoomNotice(int ret,int idx);
	//接收房间详细信息
	  void RecvRoomAllInfo(int ret,int errcode,ResultData_RoomAllInfo RD,int cseq);
	//接收排麦成功通知
	  void RecvWaitMicNotice(int ret,ResultData_MicInfo RD);
	////接收上麦准备通知
	//  void RecvWaitMicReadyNotice(int ret,ResultData_MicInfo pRD);
	//接收上麦准备完成通知
	//  void RecvOnMicReadyOkNotice(int ret,ResultData_MicInfo pRD);
	//接收上麦通知
	  void RecvOnMicNotice(int ret,ResultData_MicInfo RD);
	//取消排麦成功通知
	  void RecvCancelWaitMicNotice(int ret,int opidx,int bopidx);
	//接收加入PK麦通知
	  void RecvWaitPKMicNotice(int ret,ResultData_MicInfo RD);
	//接收取消PK麦通知
	  void RecvCancelWaitPKMicNotice(int ret,int opidx,int bopidx);
	//接收升麦成功通知
	  void RecvUpWaitMicNotice(int ret,ResultData_UDMicInfo RD);
	//接收降麦成功通知
	  void RecvDownWaitMicNotice(int ret,ResultData_UDMicInfo RD);
	//接收踢出房间通知
	  void RecvKickOutNotice(int idx,int bidx);
	//接收上VJ麦（主持麦）通知
	  void RecvOnVJMicNotice(int ret,int idx);
	//接收下VJ麦（主持麦）通知
	  void RecvOffVJMicNotice(int ret,int idx);
	//接收被下麦通知
	  void RecvGiveOffMicNotice(int ret,int idx);
	//接收被禁言通知
	  void RecvForbidenNotice(int ret,ResultData_ForbidenInfo RD);
	//接收黑名单更新通知
	  void RecvUpdateBlackListNotice(int ret,ResultData_UpBlackList RD);
	//接收管理员更新通知
	  void RecvUpdateMGRListNotice(int ret,ResultData_UpMgrList RD);
	//接收赠送礼物通知
	  void RecvSendFlowerNotice(int ret,ResultData_SendFlowerInfo RD);
	//add by jinguanfu 2010/3/4 原有回调API改成接口<end>
	//接收房间重复登录消息
	  void RecvRoomKickUser();
	//接收大厅重复登录消息
	  void RecvLobbyKickUser();
	//从大厅接收房间简易信息
	  void RecvRoomSimpleInfo(int ret,int errcode,ResultData_RoomSimpleInfoList RD);
	  void RecvRoomSimpleInfoEx(int ret,int errcode,ResultData_RoomSimpleInfoList RD, int type);
	//接收用户退出房间会员回应
	  void RecvExitRoomMember(int ret,int errcode);
	//接收WEB服务器上邮件信息
	  void RecvMailInfo(int ret,String context);
	//接收大厅在线人数
	  void RecvLobbyOnlineNum(int ret,int errcode,ResultData_LobbyOnlineList RD);
	//接收送礼回应消息	//add by jinguanfu 2010/4/27
	  void RecvSendFlower(int ret,int errcode,int recevie_idx,int item_id,int count,int gold,int silver);
	//接收发送大喇叭回应 //add by jinguanfu 2010/5/13
	  void RecvSpeakerToAll(int ret,int errcode,int gold,int silver);
	//接收发送小喇叭回应 //add by jinguanfu 2010/5/13
	  void RecvSpeakerToLobby(int ret,int errcode,int gold,int silver);
	//接收由管理员设为一般会员的回应 //add by jinguanfu 2010/5/13
	  void RecvRoomMember (int ret , int errcode, int idx,int cseq);
	//接收由管理员设为一般会员的通知 //add by jinguanfu 2010/5/13
	  void RecvRoomNotice4Member (int ret , int opidx,int bopidx);
	//接收房间内有人改变了avarat的广播 //add by cx 2010-5-20
	  void RecvRoomNotice4ChangeAvarat (int ret, int idx);
	//接收房间内错误操作提示 //add by cx 2010-6-6
	  void RecvOneError( short cmd );
	//add by cx 2010-7-6 接受购买物品回应
	  void RecvBuyPack( int ret, int errcode, int gold, int silver, short [] goods_id, int [] global_id, int count, String day );
	// 接受购买物品回应
	  void RecvBuyForSend( int ret, int errcode, int idx, int gold, int silver );
	//接受获取背包回应
	  void RecvGetPack( int ret, int errcode, ResultData_Pack [] pPack, int count, int all_count );
	//接受删除物品回应
	  void RecvDelPack( int ret, int errcode, int cseq );
	//接受商品过期通知
	  void RecvTimeoutPack( int count, int [] global_id );
	//接受商品赠送
	  void RecvSendPack( int idx, int global_id, short goods_id, String day, String msg );
	//接受使用物品回应
	  void RecvUsePack( int ret, int errcode, int global_id );
	// 升级广播
	  void RecvNotice4LevelUp( int idx, int level );
	// 经验提升
	  void RecvExpChange( int nexp, int level );
	// vip改变
	  void RecvVipChange( int idx, int level, int is_vip );
	// 钱币改变
	  void UpdataMoney( int r_gold, int r_silver );
	// 更新自己的信息
	  void RecvUpdataMyInfo();
	// 接受烟花广播
	  void RecvFireworks( int ret, int errcode, int cate_idx, int count, int gold, int silver );
	// 接受烟花广播
	  void RecvFireworksNotice( int player_idx, int cate_idx, int count );
	// 中奖？
	  void RecvLucky( int player_idx, int cate_idx, int lotte );
	// 中奖？
	  void RecvLucky( int room_idx, int player_idx, int cate_idx, int lotte );
	// 列表
	  void RecvGiftTopList( ResultData_GiftList gift_list );
	// 在线时间
	  void RecvOnlineTime( int idx, int day_time, int online );
	// 获取新手任务
	  void RecvGetNewUserTask( short step );
	// 返回新手任务提交结果
	  void RecvTaskStep( int ret, int err_code, int gold, int silver, int total_gold, int total_silver );
	  void RecvTaskGift( int ret, int err_code, int item_id, int object_id, String time_buf );
	// 商品信息更改通知
	  void RecvNotice4ChangeGoods();
	// 服务器请求刷新背包
	  void RecvGetBagAgain();
	// 返回用户印章
	  void RecvUserSeel( int user_idx, int seel_idx );
	// 返回用户防沉迷状态
	  void EnterAllMentState( int idx, short state );
	// room loge
	  void RecvRoomLoge( int room_idx, String loge, int len );
	// 新分组版本号
	  void RecvNewVersionId( int version_id );
	// 分组信息
	  void RecvGroupInfo( int count, ResultData_GroupInfo Info );
	// 获取组里面的房间列表
	  void RecvRoomListInGroup( short group_idx, int count, int [] list );
	  void RecvRoomListInGroup(short count, ResultData_GroupRoomInfo[] pInfo);
	// 上传日志通知
	  void RecvNotice4CollectData( String server_ip, short server_port );
	// 获取歌曲信息
	  void RecvSongInfoList( short totalnum, short curnum, ResultData_SongInfo info );
	// 获取音视频服务器列表
	  void RecvAvServerList( ResultData_AvServer [] list, int count );
	// 更新音视频服务器列表
	  void UpdataAvServerList();
	// 被提出游戏服务器
	  void RecvCheckOutGame( int result );
	// 获取离线消息
	  void RecvOffLineMsg( int count, ResultData_OffLineMsg [] msg );
	// 返回分组置顶信息
	  void RecvRoomSortInGroup( int idx, int count, ResultData_RoomSort [] list );
	// 通知分组置顶信息改变
	  void RecvRoomSortChange( int idx );
	// 歌手等级
	  void RecvSingerLevel(int idx, short curlevel, int expvalue);
	// 财富等级
	  void RecvWealthLevel(int idx, short curlevel, int expvalue);

	//////////////////////////////////////////////////////////////////////////

	// 礼物失效（印章）的协议
	  void RecvGiftInvalid( int idx, int GiftID );
	// 礼物有效（印章）的协议
	  void RecvGiftValid( int idx, int GiftID );
	// 显示房间可领取收益
	  void RecvShowRoomIncom( int income );
	// 显示收益已领取记录
	  void RecvShowRoomGetIncom( ResultData_IncomeDate [] data, int count );
	// 室主领取房间内收益
	  void RecvGetRoomIncom( int ret );
	// 礼物信息更改通知
	  void RecvNotice4ChangeFlower();
	// 权限配置更新通知
	  void RecvNotice4ChangePower();
	// 更改音视频服务器广播
	  void RecvNotice4AvserverChange( ResultData_AvChange info );
	// 更改gm
	  void RecvChangeGM( int idx, int add );
	// 上麦的人的pc概况
	  void RecvMicPcInfo( int idx, String buf );
	// 回应是否邀加入房间成功
	  void RecvSendJoinRoomInvite( int wseq, int ret, int err_code );
	// 回应是否邀加入房间成功
	  void RecvInviteJoinRoom( int idx );
	//取得当日礼物赠送记录
	  void RecvGiftSend( int count, ResultData_GiftCount [] pInfo );
	//取得当日礼物接收记录
	  void RecvGiftRecv( int count, ResultData_GiftCount [] pInfo );
	//返回封mac结果
	  void RecvKickUserByMac( int ret, int idx );
	//返回封ip结果
	  void RecvKickUserByIp( int ret, int idx );
	//返回设置在麦时间结果
	  void RecvSetMicTime( int ret, int idx );
	//广播在麦时间延长
	  void RecvNotice4SetMicTime( int idx, int time );
	//取排麦列表
	  void RecvWaitMicList( short count, int[] idxs );
	//防沉迷时间到,断开连接
	  void RecvIndulgeTimeUp();
	/*家族信息回调
	参数：
	flag：
	1:删除家族
	2:添加或更新家族
	familyNum：
	家族数量(N)
	familys：
	家族信息，是一个数组familys[N-1];
	注意：当flag为1时，familyNum必定为1，同时familys[0]里只有familyid是可用的(因为删除的时候，一次只能删除一个，只有家族id信息)
	*/
	  void RecvFamilyInfo(int flag, int familyNum, ResultData_Family[] familys);

	/*
	av配置信息
	*/
	  void RecvAVConfigInfo(int infolen, String info);

	//
	/*------------------------------私麦---------------------------------------*/
	/*上私麦*/
	  void RecvOpenPrivateMicSuccess(int idx, short mictype, int posindex, int roomid);
	  void RecvOpenPrivateMicFailed(int errorcode, int idx, int bidx);
	  void RecvOpenPrivateMicNotice(int idx, int mictype); //不包括自己
	
	/*下私麦*/
	  void RecvClosePrivateMicFailed(int errorcode, int idx, int bidx);
	  void RecvClosePriveteMicNotice(int idx); //包括自己
	
	/*被抱下私麦*/
	  void RecvClosedPrivateMicFailed(int errorcode, int idx, int bidx);
	  void RecvClosedPriveteMicNotice(int idx, int bidx); //包括自己
	
	/*观看私麦*/
	  void RecvWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid);
	  void RecvWatchPrivateMicFailed(int errorcode, int idx, int bidx);
	  void RecvRequestWatchedPrivateMic(int idx); /*idx=请求者, 是否同意该idx观看你的私麦*/
	  void RecvWatchedPrivateMicFailed(int errorcode, int idx, int bidx);

	  void RecvWatchedPrivateMicReject(int idx); //对方拒绝，idx=谁拒绝的

	/*邀请私麦*/
	  void RecvInviteWatchPrivateMicSuccess();
	  void RecvInviteWatchPrivateMicFailed(int errorcode, int idx, int bidx);
	  void RecvRequestInvitedWatchPrivateMic(int idx); /*idx=邀请者,是否同意观看idx的私麦*/
	  void RecvInvitedWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid);
	  void RecvInvitedWatchPrivateMicFailed(int errorcode, int idx, int bidx);

	  void RecvInvitedWatchPrivateMicReject(int idx);//对方拒绝，idx=谁拒绝的

	/*管理房间私麦*/
	  void RecvOpenRoomPrivateMicFlag(int ret, int flag);
	  void RecvOpenRoomPrivateMicFlagNotice(int idx, int flag); //包括自己

	/*用户登录房间时，回调房间的私麦信息*/
	  void RecvPrivateMicWithLoginRoom(int idx, short mictype, int posindex, int roomid);
	/*	返回皇冠踢人结果
		int ret
		 0: 成功 
		-1: 失败，非法操作
		-2: 失败，剩余人次数不足
		int kickuse
		本次操作消耗人次
		int totalkick
		剩余消耗人次数
		int idx;	
		被踢者idx
	*/
	  void RecvCrownKickoutSomeOne(int ret, int kickuse, int totalkick, int idx);
	/*室主禁止用户ip
	int ret;
	 0：成功
	 -1：该IP已被禁用
	 -2：该用户已离开房间
	
	int idx;
	被禁用者idx
	*/
	  void RecvForbidUserIp(int ret, int idx);

	//要求上传歌曲
	  void RecvUpdateSong();
	//回应服务器的返回用户歌唱信息
	//  void RecvUserSingInfo(int ret,int idx,const ResultData_UserSingInfo pData);
	//获取用户接受礼物个数
	  void RecvGiftCount(int recvcount);

	//获取用户最高分歌曲信息及演唱歌曲个数
	  void RecvTopSongInfo(int songcount, int topsongscore, String topsongname);
	  
	  ///////////////////////////////////////////////////手机新增回调////////////////////////////////////////////////////////////
	//手机ktv聊天信息
	    /*
	     @param ret:返回值，0,OK;其他值,something error
	     @param fromIdx:来自谁,如果发生错误，也有可能指发送的IDX
	     @param roomName:说话者在的房间，如果未在房间则为“0”
	     @param content:说的内容
	     @param chatTime:说话时间
	     @param isPublic:是否为公聊
	     
	     @注:chattime
	     */
	    void RecvKtvChatInfo(int ret,int fromIdx,int roomid,String roomName,String content,
	                                 int chatTime,boolean isPublic,boolean isSend);
	    //手机接收到当前的播放列表，即当前ktv的点歌列表，用于更新
	    /*
	     @param pSonglist:信息结构体
	     @param listcount:歌曲数量
	     */
	    void RecvKtvSongList(ResultData_SongList[] pSonglist,int listCount);
	    //手机获取当前包厢的开关状态
	    /*
	     @param isOpen:true,包厢开启；false，包厢关闭，用户退出房间
	     */
	    void RecvKtvRoomState(boolean isOpen);
	    //手机收到唱歌上榜通知,提示用户到个人信息里面去抢榜
	    void RecvKtvSongRanking();
	    /*
		通知歌曲演唱结束
		*/
	    void RecvKtvSongNormalEnd(int nMusicId);
	    /*
	     收到的在线手机用户IDX列表更新数据
	     只有IDX
	     */
		void RecvKtvIdxList(int[] pIdx,int number);
		/*
		通知用户头像更新
		@param [in] idx：指定的用户IDX
		@param [in] picture：用户的新头像
		*/
		void RecvUserPicChange(int idx,String picture);
		//通知用户名字修改
		void RecvUserNameChange(int idx,String name);
	    //单个礼物图片下载完成通知
	    //可以在这里去对之前没有获取到图片的地方尝试再去获取一下,或许已经下载下来了
	    /*
	     @param pictureName[in]:图片名称(含路径)
	     
	     @注:这里回调的线程跟上面的回调线程都不一样，是来自HTTP下载线程的回调
	     */
	    
	    void RecvImageDownladFinished(String pictureName);
	    /*
	     主线程或者HTTP线程回调，在初始化下载地址之后成功的话就会回调，通知本地数据库礼物信息已经完成配置
	     */
	    void RecvLocalGiftDatabaseFinished();
}