#ifndef RESPONSEBASE__H__
#define RESPONSEBASE__H__
/*
	注释添加以及修改于 2014-4-8 
	mender: glp

	声明一个接口类ResponseBase，用于对服务器数据返回的处理，
	用户根据自己业务的需要自己实现响应的方法。
*/
#include "ResultData.h"

class ResponseBase
{
public:
	ResponseBase(void){};
	virtual ~ResponseBase(void){};
	/************************************************************************/
	/*			add by lixin 2011/10/13 <begin> 聊天、喇叭功能			*/
	virtual void SendChatFromUser(int ret, int to_idx, const char *content ,int contentlen, int time )=0;
	/*			add by lixin 2011/10/13 <end> 聊天、喇叭功能			*/
	/************************************************************************/

	/************************************************************************/
	/*			add by jinguanfu 2009/12/18 <begin> 聊天、喇叭功能			*/
	//接收私聊
	virtual void RecvChatFromUser(int ret, int from_idx, const char *content ,int contentlen, int time )=0;
	//接收公聊（俱乐部内部）
	virtual void RecvChatFromClub(int ret, int from_idx, const char* content,int contentlen)=0;
	//接收公聊（房间内）
	//ret:1--正常 0--被禁言 -1--不允许公聊
	virtual void RecvChatFromRoom(int ret, int errcode,int from_idx, const char* content,int contentlen, int time )=0;	
	//接收大喇叭（全区）
	virtual void RecvSpeakerFromAll(int ret, int type, int from_id, const char* msg, int msglen)=0;
	//接收小喇叭（大厅）
	//@param isSend : 为true的时候是客户端主动请求的最近的喇叭信息，为false的时候是大厅广播喇叭
	virtual void RecvSpeakerFromLobby(int ret, ResultData_UserSpeakerInfo* spkInfo,int type,bool isSend )=0;
	//接收礼物广播
	virtual void RecvPresentBroad(int ret, ResultData_GiftInfo* guestGiftInfo, ResultData_GiftInfo* commGiftInfo)=0;
	//接受系统文字广播
	virtual void RecvSystemBroad( char type, const char * msg, int len )=0;
	//接受系统文字广播（幸运中奖信息）
	virtual void RecvSystemBroad2(const char* nickname, int len, int idx, int roomid, int giftid, int luckmultiple)=0;
	//接受系统文字公告
	virtual void RecvSystemNotice( const char * msg, int len, bool once, int min )=0;
	//接受系统文字公告（幸运中奖信息）
	virtual void RecvSystemNotice2(const char* nickname, int len, int idx, int roomid, int giftid, int luckmultiple)=0;
	//接收房间临时公告
	virtual void RecvRoomNotice(int ret, const char* content,int contentlen)=0;
	/*			add by jinguanfu 2009/12/28 <end> 聊天、喇叭功能			*/
	/************************************************************************/

	/************************************************************************/
	/*			add by wq  2012/4/28 <begin>								*/
	// 接收小喇叭信息
	// virtual void RecvUserSpeakerInfo( int, ResultData_UserSpeakerInfo * ) = 0;
	/*			add by wq  2012/4/28 <end>									*/
	/************************************************************************/

	/************************************************************************/
	/*			add by jinguanfu 2009/12/28 <begin> 好友操作				*/
	//取得好友位置
	virtual void RecvFriendLocation(int ret, int errcode,
								int friend_idx,
								int lobbyid, 
								int roomid,
								int cseq)=0;
	/*			add by jinguanfu 2009/12/28 <end> 好友操作					*/
	/************************************************************************/
	/************************************************************************/
	/*			add by jinguanfu 2009/12/28 <begin> 房间管理接口			*/
	//接收取得待审核房间会员列表
	virtual void RecvRoomApplyList (int ret ,int errcode,ResultDate_RoomApplyList *applylist,int cseq)=0;
	//接收申请房间会员响应
	virtual void RecvApplyRoomMember(int ret,int errcode,int cseq)=0;
	//接收审核会员加入申请结果
	virtual void RecvAuditAddApply (int ret, int errcode,int idx,int cseq)=0;
	//接收拒绝房间申请系统提示
	virtual void RecvRoomApplyRefuseMsg (int ret, int roomid)=0;
	//接收房间会员列表
	virtual void RecvRoomMemberList (int ret ,int errcode, ResultDate_RoomMemList* memlist,int cseq)=0;
	//接收解除房间会员资格回应
	virtual void RecvUnchainMember (int ret, int errcode, int idx,int cseq)=0;
	//接收赋予用户房间主持助理权限回应
	virtual void RecvRoomPesideAssistant (int ret , int errcode, int idx,int cseq)=0;
	//接收赋予主持助理通知
	virtual void RecvRoomNotice4PesideAssistant (int ret , int opidx,int bopidx)=0;
	//接收赋予用户房间主持人权限回应
	virtual void RecvRoomPeside (int ret ,int errcode, int idx,int cseq)=0;
	//接收设置主持人权限房间通知
	virtual void RecvRoomNotice4Peside (int ret , int opidx,int bopidx)=0;
	//接收赋予用户房间副室主权限回应
	virtual void RecvRoomSubHost (int ret ,int errcode, int idx,int cseq)=0;
	//接收设置副室主权限房间通知
	virtual void RecvRoomNotice4SubHost(int ret , int opidx,int bopidx)=0;
	//接收房间黑名单
	virtual void RecvBlackList(int ret,	int errcode,ResultDate_RoomBlackList* blacklist,int cseq)=0;
	//接收黑名单用户添加/删除回应
	virtual void RecvAddDelBlack(int ret , int errcode,int idx, int type)=0;
	//接收房间设置面板信息
	virtual void RecvRoomPanelInfo(int ret ,int errcode, ResultDate_RoomPanel* panelinfo)=0;
	//接收设置房间密码回应
	virtual void RecvRoomPassword(int ret,int errcode,int cseq)=0;
	//接收设置房间密码消息提示
	virtual void RecvRoomNotice4Password(int ret, int idx, char pwdflag)=0;
	//接收设置房间开启/关闭(上锁)回应
	virtual void RecvRoomSwitch(int ret,int errcode,char roomflag,int cseq)=0;
	//接收房间开启/关闭(上锁)消息提示
	virtual void RecvRoomNotice4RoomSwitch(int ret , int idx , char roomflag)=0;
	//接收设置房间访问权限回应
	virtual void RecvRoomPrivate(int ret , int errcode,char accessflag,int cseq)=0;
	//接收房间访问权限消息提示
	virtual void RecvRoomNotice4Private(int ret , int idx , char accessflag)=0;
	//接收设置用户进出信息开启/关闭回应
	virtual void RecvUserInOutSwitch(int ret , int errcode, char inoutflag,int cseq)=0;
	//接收用户进出信息开启/关闭消息提示
	virtual void RecvRoomNotice4UserSwitch (int ret , int idx , char inoutflag)=0;
	//接收设置自由排麦开启/关闭回应
	virtual void RecvGetMicSwitch (int ret, int errcode,char onmicflag,int cseq)=0;
	//接收设置自由上麦开启/关闭回应
	virtual void RecvOnMicSwitch (int ret, int errcode,char onmicflag,int cseq)=0;
	//接收自由排麦开启/关闭消息提示
	virtual void RecvRoomNotice4GetMicSwitch(int ret , int idx , char getmicflag)=0;
	//接收自由上麦开启/关闭消息提示
	virtual void RecvRoomNotice4OnMicSwitch(int ret , int idx , char onmicflag)=0;
	//房间名称修改回应
	virtual void RecvModifyRoomName(int ret,int errcode,int cseq)=0;
	//接收房间名
	virtual void RecvRoomName (int ret , int roomid , const char* roomname , int rnlen)=0;
	//设置房间公告响应
	virtual void RecvRoomAffiche(int ret,int errcode,int cseq)=0;
	//接收房间公告更新事件
	virtual void RecvRoomAfficheEvent(int ret)=0;
	//设置允许房间公聊响应
	virtual void RecvAllowRoomAllChat (int ret, int errcode,char chatflag,int cseq)=0;
	//接收房间公聊设置消息提示
	virtual void RecvRoomNotice4AllChat(int ret , int idx , char chatflag)=0;
	//设置房间欢迎词响应
	virtual void RecvRoomSalutatory(int ret, int errcode,int cseq)=0;
	//接收房间欢迎词更新通知
	virtual void RecvRoomNotice4Salutatory(int ret,int idx)=0;
	//设置房间logo响应
	virtual void RecvRoomLogo (int ret,int errcode ,int cseq)=0;
	//接收房间LOGO更新通知
	virtual void RecvRoomNotice4Logo(int ret,int idx)=0;
	//接收房间冻结消息提示
	virtual void RecvRoomNotice4Lock(int idx)=0;
	//接受创建房间回应
	virtual void RecvCreateRoom( int ret, int errcode, RoomListEx * info )=0;
	//接受练歌房完全信息
	virtual void RecvTmpRoomInfo( int ret, int errcode, RoomListEx * info )=0;
	// 添加歌房广播
	virtual void RecvRoomNotice4Add( RoomListEx * info )=0;
	virtual void RecvRoomNotice4Add( int idx )=0;
	// 删除歌房广播
	virtual void RecvRoomNotice4Del( int idx )=0;
	// 接收房间易主通知
	virtual void RecvRoomNotice4ChangeMaster(int idx)=0;
	// 接收房间信息更改通知
	virtual void RecvRoomNotice4ChangeInfo( int idx )=0;
	/*			add by jinguanfu 2009/12/28 <end> 房间管理接口				*/
	/************************************************************************/
	/************************************************************************/
	/*			add by jinguanfu 2009/12/28 <begin> 角色信息				*/
	//取得他人角色信息
	virtual void RecvPartInfo(int ret, int errcode, ResultData_PartInfo * part_info, int cseq)=0;
	//取得自己角色信息
	virtual void RecvPartInfoBySelf(int ret, int errcode, ResultData_PartInfo * part_info, int cseq)=0;
	/*			add by jinguanfu 2009/12/28 <end> 角色信息					*/
	/************************************************************************/
	/************************************************************************/
	/*			add by jinguanfu 2010/1/5 <begin> 取得房间列表				*/
	virtual void RecvRoomList(int ret,ResultDate_RoomList* roomlist)=0;
	virtual void RecvRoomListEx(int ret,ResultDate_RoomListEx * roomlist)=0;
	/*			add by jinguanfu 2010/1/5 <end> 取得房间列表				*/
	/************************************************************************/
	//打分通知
	//virtual void RecvNotice4Score(int onmic_idx,int onmic_score,int pk_idx,int pk_score)=0;
	//virtual void RecvNotice4ScoreAdd(int epchange, int silverchange)=0;
	//下麦通知
	virtual void RecvNotice4OffMic( int onmic_idx, int pk_idx, int exp, const char * scoreinfo, int len )=0;
	// add by cx 10-6-2
	//回应是否邀请上麦成功
	virtual void RecvRoomGivOnevjMic( int idx, char flag )=0;
	//是否同意邀请上麦成功
	virtual void RecvRoomGivOnevjMicApply( int formidx )=0;
	//add by jinguanfu 2010/1/20 <begin> 
	//是否同意添加对方为好友
	//参数说明：int idx 添加好友者idx
	virtual void RecvAddFriendApply(int idx)=0;
	//add by jinguanfu 2010/1/20 <end> 
	//virtual void RecvDeleteAvarta(int ret)=0;
	//add by jinguanfu 2010/3/4 原有回调API改成接口<begin>
	//连接成功
	virtual void OnAuthTunnelConnectSuccess()=0;			//成功连接认证服务器
	virtual void OnLobbyTunnelConnectSuccess()=0;		//成功连接大厅服务器
	virtual void OnRoomTunnelConnectSuccess()=0;			//成功连接房间服务器
	virtual void OnChannellistTunnelConnectSuccess()=0;	//成功连接频道服务器
	//连接超时
	virtual void OnAuthTunnelConnectTimeout()=0;		//连接认证服务器超时
	virtual void OnLobbyTunnelConnectTimeout()=0;		//连接大厅服务器超时
	virtual void OnRoomTunnelConnectTimeout()=0;		//连接房间服务器超时
	virtual void OnChannellistTunnelConnectTimeout()=0;//连接频道服务器超时
	//连接错误
	virtual void OnAuthTunnelConnectError( int code )=0;	
	virtual void OnLobbyTunnelConnectError( int code )=0;
	virtual void OnRoomTunnelConnectError( int code )=0;
	virtual void OnChannellistTunnelConnectError( int code )=0;
	//服务器主动断开的连接,客户端recv == 0的时候,回调到以下的接口
	virtual void OnAuthTunnelClose()=0;	//断开认证服务器
	virtual void OnLobbyTunnelClose()=0;	//断开大厅服务器
	virtual void OnRoomTunnelClose()=0;	//断开房间服务器
	virtual void OnChannellistTunnelClose()=0;//断开频道服务器
	//客户端recv异常,send异常,网络层buf溢出,select出现问题,都会回调到这个以下接口
	virtual void OnAuthTunnelError( int code )=0;	
	virtual void OnLobbyTunnelError( int code )=0;
	virtual void OnRoomTunnelError( int code )=0;
	virtual void OnChannellistTunnelError( int code )=0;

	/******************认证服务器操作接口*****************************/
	//接收认证结果
	virtual void RecvAuth(int ret, int errcode, const char * token, int len)=0;
	//接收新token
	//virtual void RecvNewToken(int ret,ReserveDataS* pRD)=0;
	virtual void RecvNewToken(int ret,int errcode)=0;
	//接收频道列表
	virtual void RecvSectionList(int ret,int errcode,ResultData_GetChannellist* pRDc,int cseq)=0;
	//接收频道列表(添加防沉迷信息)
	virtual void RecvSectionList2(int ret,int errcode,int leftlogintime,ResultData_GetChannellist* pRDc,int cseq)=0;
	/******************大厅服务器操作接口*****************************/
	//接收大厅token认证
	virtual void RecvTokenAuth4Lobby( int ret, int errcode, ResultData_LobbyLogin * pRDl, ResultData_SelfData * pSD, int cseq )=0;
	//用户登录大厅通知
	virtual void RecvUserLoginLobbyNotice(int ret,ResultData_LobbyLoginUser* pRlu)=0;
	//用户退出大厅通知
	virtual void RecvUserLogoutLobbyNotice(int ret,ResultData_LobbyLoginUser* pRlu)=0;
	//接收用户信息
	virtual void RecvUserInfo(int ret,int errcode,ResultData_UserFullInfo* pRDu)=0;
	//接收好友状态
	virtual void RecvFriendStatus(int ret,int errcode,ResultData_GetFriendlist* pRD)=0;
	//接收黑名单状态
	virtual void RecvBlackStatus(int ret,int errcode,ResultData_GetFriendlist* pRD)=0;
	//接收俱乐部状态
	virtual void RecvClubStatus(int ret,int errcode,ResultData_GetClublist* pRD)=0;
	//接收添加好友结果
	virtual void RecvAddFriend(int ret,int errcode,int idx)=0;
	//接收添加黑名单结果
	virtual void RecvAddBlack(int ret,int errcode,int cseq)=0;
	//接收删除好友结果
	virtual void RecvDelFriend(int ret,int errcode,int cseq)=0;
	//接收删除黑名单结果
	virtual void RecvDelBlack(int ret,int errcode,int cseq)=0;
	//接收申请加入俱乐部结果
	virtual void RecvApplyClub(int ret,int errcode,int cseq)=0;
	//接收退出俱乐部结果
	virtual void RecvExitClub(int ret,int errcode,int cseq)=0;
	//接收最近几天内的加入俱乐部申请数量
	virtual void RecvApplyListCount(int ret,int errcode,int count,int cseq)=0;
	//接收最近几天内的加入俱乐部申请列表
	virtual void RecvApplyList(int ret,int errcode,ResultData_ClubApplyList* pRD,int cseq)=0;
	//通过加入俱乐部审批
	virtual void RecvAddClubOk(int ret,ResultData_Approvelresult* pRD)=0;
	//删除申请列表
	virtual void RecvDelApplyList(int ret,ResultData_Approvelresult* pRD)=0;
	//接收加入俱乐部广播消息(当有人加入俱乐部)
	//modify by jinguanfu 2010/4/11
	//virtual void RecvAddClubNotice(int ret ,int idx)=0;
	//opidx--操作者idx
	//addidx--加入者idx
	virtual void RecvAddClubNoticeByLobby(int ret ,int clubid,const char* clubname,int opidx,int addidx)=0;
	virtual void RecvAddClubNoticeByRoom( int opidx, int addidx )=0;
	//接收申请会员拒绝消息
	//opidx--审核者idx
	virtual void RecvDenyClubNoticeByLobby(int ret ,int clubid,const char* clubname,int opidx)=0;
	//virtual void RecvDenyClubNoticeByRoom(int ret ,int clubid,char* clubname,int opidx)=0;
	//接收会员退出俱乐部消息
	virtual void RecvExitClubNoticeByLobby(int ret,int clubid,const char* clubname,int exitidx)=0;
	virtual void RecvExitClubNoticeByRoom( int exitidx )=0;
	//接收会员被踢出俱乐部消息
	//opidx--操作者idx
	//kickidx--被踢出者idx
	virtual void RecvKickClubNoticeByLobby(int ret,int clubid,const char* clubname,int opidx,int kickidx)=0;
	virtual void RecvKickClubNoticeByRoom( int opidx, int kickidx )=0;
	//创建俱乐部
	virtual void RecvCreateClub(int ret,int errcode,int clubid,int cseq)=0;
	//解散俱乐部
	virtual void RecvDismissClub(int ret,int errcode,int cseq)=0;
	//接收俱乐部信息
	virtual void RecvClubInfo(int ret,int errcode,ResultData_ClubFullInfo* pRD,int cseq)=0;
	////接收房间列表
	//virtual void RecvRoomList(int ret,ResultData_RoomList* pRD);
	//接收房间信息
	virtual void RecvRoomInfo(int ret,ResultData_RoomInfo* pRD)=0;
	//创建头像
	virtual void RecvCreateAvatar(int ret,int errcode)=0;
	//修改头像
	virtual void RecvChangeAvatar(int ret,int errcode)=0;
	//修改昵称
	virtual void RecvChangeNickName(int ret,int errcode,const char * name, int len)=0;
	//进入房间通知
	virtual void RecvLobbyEnterRoom(int ret,ResultData_LobbyRoomEnter* pRD)=0;
	//离开房间通知
	virtual void RecvLobbyLeaveRoom(int ret,ResultData_LobbyRoomEnter* pRD)=0;
	//添加好友通知
	virtual void RecvAddFriendNotice(int ret,int idx)=0;
	//取得大厅信息
	virtual void RecvLobbyInfo(int ret,int errcode,ResultData_LobbyName* pRD,int cseq)=0;
	//接收来自大厅的权限更改消息 add by jinguanfu 2010/6/5
	virtual void RecvChangePower(int optionidx/*操作者ID*/,char identity/*更改后权限*/)=0;
	//取大厅名(CMD_SYSTEMMSG_L2C)
	//virtual void RecvLobbyName();
	/******************房间操作接口*****************************/
	//接收房间token验证 //
	virtual void RecvTokenAuth4Room(int ret,int errcode)=0;
	//进入房间
	virtual void RecvEnterRoom(int ret,int errcode,ResultData_LoginInfo* pRDlf,int cseq)=0;
	//用户进入房间通知
	virtual void RecvEnterRoomNotice(int ret,ResultData_UserInfo* pRDu)=0;
	//用户离开房间通知
	virtual void RecvLeaveRoomNotice(int ret,int idx)=0;
	//接收房间详细信息
	virtual void RecvRoomAllInfo(int ret,int errcode,ResultData_RoomAllInfo *pRD,int cseq)=0;
	//接收排麦成功通知
	virtual void RecvWaitMicNotice(int ret,ResultData_MicInfo *pRD)=0;
	////接收上麦准备通知
	//virtual void RecvWaitMicReadyNotice(int ret,ResultData_MicInfo *pRD)=0;
	//接收上麦准备完成通知
	//virtual void RecvOnMicReadyOkNotice(int ret,ResultData_MicInfo *pRD)=0;
	//接收上麦通知
	virtual void RecvOnMicNotice(int ret,ResultData_MicInfo *pRD)=0;
	//取消排麦成功通知
	virtual void RecvCancelWaitMicNotice(int ret,int opidx,int bopidx)=0;
	//接收加入PK麦通知
	virtual void RecvWaitPKMicNotice(int ret,ResultData_MicInfo *pRD)=0;
	//接收取消PK麦通知
	virtual void RecvCancelWaitPKMicNotice(int ret,int opidx,int bopidx)=0;
	//接收升麦成功通知
	virtual void RecvUpWaitMicNotice(int ret,ResultData_UDMicInfo *pRD)=0;
	//接收降麦成功通知
	virtual void RecvDownWaitMicNotice(int ret,ResultData_UDMicInfo *pRD)=0;
	//接收踢出房间通知
	virtual void RecvKickOutNotice(int idx,int bidx)=0;
	//接收上VJ麦（主持麦）通知
	virtual void RecvOnVJMicNotice(int ret,int idx)=0;
	//接收下VJ麦（主持麦）通知
	virtual void RecvOffVJMicNotice(int ret,int idx)=0;
	//接收被下麦通知
	virtual void RecvGiveOffMicNotice(int ret,int idx)=0;
	//接收被禁言通知
	virtual void RecvForbidenNotice(int ret,ResultData_ForbidenInfo* pRD)=0;
	//接收黑名单更新通知
	virtual void RecvUpdateBlackListNotice(int ret,ResultData_UpBlackList* pRD)=0;
	//接收管理员更新通知
	virtual void RecvUpdateMGRListNotice(int ret,ResultData_UpMgrList* pRD)=0;
	//接收赠送礼物通知
	virtual void RecvSendFlowerNotice(int ret,ResultData_SendFlowerInfo* pRD)=0;
	//add by jinguanfu 2010/3/4 原有回调API改成接口<end>
	//接收房间重复登录消息
	virtual void RecvRoomKickUser()=0;
	//接收大厅重复登录消息
	virtual void RecvLobbyKickUser()=0;
	//从大厅接收房间简易信息
	virtual void RecvRoomSimpleInfo(int ret,int errcode,ResultData_RoomSimpleInfoList* pRD)=0;
	virtual void RecvRoomSimpleInfoEx(int ret,int errcode,ResultData_RoomSimpleInfoList* pRD, int type)=0;
	//接收用户退出房间会员回应
	virtual void RecvExitRoomMember(int ret,int errcode)=0;
	//接收WEB服务器上邮件信息
	virtual void RecvMailInfo(int ret,char* context,int length)=0;
	//接收大厅在线人数
	virtual void RecvLobbyOnlineNum(int ret,int errcode,ResultData_LobbyOnlineList* pRD)=0;
	//接收送礼回应消息	//add by jinguanfu 2010/4/27
	virtual void RecvSendFlower(int ret,int errcode,int receive_idx,int item_id,int count,int gold,int silver)=0;
	//接收发送大喇叭回应 //add by jinguanfu 2010/5/13
	virtual void RecvSpeakerToAll(int ret,int errcode,int gold,int silver)=0;
	//接收发送小喇叭回应 
	virtual void RecvSpeakerToLobby(int ret,int errcode,int gold,int silver)=0;
	//接收由管理员设为一般会员的回应 //add by jinguanfu 2010/5/13
	virtual void RecvRoomMember (int ret , int errcode, int idx,int cseq)=0;
	//接收由管理员设为一般会员的通知 //add by jinguanfu 2010/5/13
	virtual void RecvRoomNotice4Member (int ret , int opidx,int bopidx)=0;
	//接收房间内有人改变了avarat的广播 //add by cx 2010-5-20
	virtual void RecvRoomNotice4ChangeAvarat (int ret, int idx)=0;
	//接收房间内错误操作提示 //add by cx 2010-6-6
	virtual void RecvOneError( short cmd )=0;
	//add by cx 2010-7-6 接受购买物品回应
	virtual void RecvBuyPack( int ret, int errcode, int gold, int silver, short * goods_id, int * global_id, int count, const char * day )=0;
	// 接受购买物品回应
	virtual void RecvBuyForSend( int ret, int errcode, int idx, int gold, int silver )=0;
	//接受获取背包回应
	virtual void RecvGetPack( int ret, int errcode, ResultData_Pack * pPack, int count, int all_count )=0;
	//接受删除物品回应
	virtual void RecvDelPack( int ret, int errcode, int cseq )=0;
	//接受商品过期通知
	virtual void RecvTimeoutPack( int count, int * global_id )=0;
	//接受商品赠送
	virtual void RecvSendPack( int idx, int global_id, short goods_id, const char * day, const char * msg )=0;
	//接受使用物品回应
	virtual void RecvUsePack( int ret, int errcode, int global_id )=0;
	// 升级广播
	virtual void RecvNotice4LevelUp( int idx, char level )=0;
	// 经验提升
	virtual void RecvExpChange( int nexp, int level )=0;
	// vip改变
	virtual void RecvVipChange( int idx, int level, char is_vip )=0;
	// 钱币改变
	virtual void UpdataMoney( int r_gold, int r_silver )=0;
	// 更新自己的信息
	virtual void RecvUpdataMyInfo()=0;
	// 接受烟花广播
	virtual void RecvFireworks( int ret, int errcode, int cate_idx, int count, int gold, int silver )=0;
	// 接受烟花广播
	virtual void RecvFireworksNotice( int player_idx, int cate_idx, int count )=0;
	// 中奖？
	virtual void RecvLucky( int player_idx, int cate_idx, int lotte )=0;
	// 中奖？
	virtual void RecvLucky( int room_idx, int player_idx, int cate_idx, int lotte )=0;
	// 列表
	virtual void RecvGiftTopList( ResultData_GiftList * gift_list )=0;
	// 在线时间
	virtual void RecvOnlineTime( int idx, int day_time, int online )=0;
	// 获取新手任务
	virtual void RecvGetNewUserTask( short step )=0;
	// 返回新手任务提交结果
	virtual void RecvTaskStep( int ret, int err_code, int gold, int silver, int total_gold, int total_silver )=0;
	virtual void RecvTaskGift( int ret, int err_code, int item_id, int object_id, const char * time_buf )=0;
	// 商品信息更改通知
	virtual void RecvNotice4ChangeGoods()=0;
	// 服务器请求刷新背包
	virtual void RecvGetBagAgain()=0;
	// 返回用户印章
	virtual void RecvUserSeel( int user_idx, int seel_idx )=0;
	// 返回用户防沉迷状态
	virtual void EnterAllMentState( int idx, short state )=0;
	// room loge
	virtual void RecvRoomLoge( int room_idx, const char * loge, int len )=0;
	// 新分组版本号
	virtual void RecvNewVersionId( int version_id ) = 0;
	// 分组信息
	virtual void RecvGroupInfo( int count, ResultData_GroupInfo * pInfo ) = 0;
	// 获取组里面的房间列表
	virtual void RecvRoomListInGroup( short group_idx, int count, int * list ) = 0;
    virtual void RecvRoomListInGroup(short count, ResultData_GroupRoomInfo* pInfo) = 0;
	// 上传日志通知
	virtual void RecvNotice4CollectData( const char * server_ip, short server_port ) = 0;
	// 获取歌曲信息
	virtual void RecvSongInfoList( short totalnum, short curnum, ResultData_SongInfo * info ) = 0;
	// 获取音视频服务器列表
	virtual void RecvAvServerList( ResultData_AvServer * list, int count )=0;
	// 更新音视频服务器列表
	virtual void UpdataAvServerList()=0;
	// 被提出游戏服务器
	virtual void RecvCheckOutGame( int result )=0;
	// 获取离线消息
	virtual void RecvOffLineMsg( int count, ResultData_OffLineMsg * msg ) = 0;
	// 返回分组置顶信息
	virtual void RecvRoomSortInGroup( int idx, int count, ResultData_RoomSort * list ) = 0;
	// 通知分组置顶信息改变
	virtual void RecvRoomSortChange( int idx ) = 0;
	// 歌手等级
	virtual void RecvSingerLevel(int idx, short curlevel, int expvalue) = 0;
	// 财富等级
	virtual void RecvWealthLevel(int idx, short curlevel, int expvalue) = 0;

	//////////////////////////////////////////////////////////////////////////

	// 礼物失效（印章）的协议
	virtual void RecvGiftInvalid( int idx, int GiftID )=0;
	// 礼物有效（印章）的协议
	virtual void RecvGiftValid( int idx, int GiftID )=0;
	// 显示房间可领取收益
	virtual void RecvShowRoomIncom( int income )=0;
	// 显示收益已领取记录
	virtual void RecvShowRoomGetIncom( ResultData_IncomeDate * data, int count )=0;
	// 室主领取房间内收益
	virtual void RecvGetRoomIncom( int ret )=0;
	// 礼物信息更改通知
	virtual void RecvNotice4ChangeFlower()=0;
	// 权限配置更新通知
	virtual void RecvNotice4ChangePower()=0;
	// 更改音视频服务器广播
	virtual void RecvNotice4AvserverChange( ResultData_AvChange * info )=0;
	// 更改gm
	virtual void RecvChangeGM( int idx, char add )=0;
	// 上麦的人的pc概况
	virtual void RecvMicPcInfo( int idx, const char * buf, int len )=0;
	// 回应是否邀加入房间成功
	virtual void RecvSendJoinRoomInvite( int wseq, int ret, int err_code )=0;
	// 回应是否邀加入房间成功
	virtual void RecvInviteJoinRoom( int idx )=0;
	//取得当日礼物赠送记录
	virtual void RecvGiftSend( int count, ResultData_GiftCount * pInfo ) = 0;
	//取得当日礼物接收记录
	virtual void RecvGiftRecv( int count, ResultData_GiftCount * pInfo ) = 0;
	//返回封mac结果
	virtual void RecvKickUserByMac( int ret, int idx ) = 0;
	//返回封ip结果
	virtual void RecvKickUserByIp( int ret, int idx ) = 0;
	//返回设置在麦时间结果
	virtual void RecvSetMicTime( int ret, int idx ) = 0;
	//广播在麦时间延长
	virtual void RecvNotice4SetMicTime( int idx, int time ) = 0;
	//取排麦列表
	virtual void RecvWaitMicList( short count, int* idxs ) = 0;
	//防沉迷时间到,断开连接
	virtual void RecvIndulgeTimeUp() = 0;
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
	virtual void RecvFamilyInfo(int flag, int familyNum, ResultData_Family* familys) = 0;

	/*
	av配置信息
	*/
	virtual void RecvAVConfigInfo(int infolen, const char* info) = 0;

	/************************************************************************/
	/* 私麦                                                                 */
	/************************************************************************/
	/*上私麦*/
	virtual void RecvOpenPrivateMicSuccess(int idx, short mictype, int posindex, int roomid) = 0;
	virtual void RecvOpenPrivateMicFailed(int errorcode, int idx, int bidx) = 0;
	virtual void RecvOpenPrivateMicNotice(int idx, int mictype) = 0; /*不包括自己*/
	
	/*下私麦*/
	virtual void RecvClosePrivateMicFailed(int errorcode, int idx, int bidx) = 0;
	virtual void RecvClosePriveteMicNotice(int idx) = 0; /*包括自己*/
	
	/*被抱下私麦*/
	virtual void RecvClosedPrivateMicFailed(int errorcode, int idx, int bidx) = 0;
	virtual void RecvClosedPriveteMicNotice(int idx, int bidx) = 0; /*包括自己*/
	
	/*观看私麦*/
	virtual void RecvWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid) = 0;
	virtual void RecvWatchPrivateMicFailed(int errorcode, int idx, int bidx) = 0;
	virtual void RecvRequestWatchedPrivateMic(int idx) = 0; /*idx=请求者, 是否同意该idx观看你的私麦*/
	virtual void RecvWatchedPrivateMicFailed(int errorcode, int idx, int bidx) = 0;

	virtual void RecvWatchedPrivateMicReject(int idx) = 0; //对方拒绝，idx=谁拒绝的

	/*邀请私麦*/
	virtual void RecvInviteWatchPrivateMicSuccess() = 0;
	virtual void RecvInviteWatchPrivateMicFailed(int errorcode, int idx, int bidx) = 0;
	virtual void RecvRequestInvitedWatchPrivateMic(int idx) = 0; /*idx=邀请者,是否同意观看idx的私麦*/
	virtual void RecvInvitedWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid) = 0;
	virtual void RecvInvitedWatchPrivateMicFailed(int errorcode, int idx, int bidx) = 0;

	virtual void RecvInvitedWatchPrivateMicReject(int idx) = 0;//对方拒绝，idx=谁拒绝的

	/*管理房间私麦*/
	virtual void RecvOpenRoomPrivateMicFlag(int ret, char flag) = 0;
	virtual void RecvOpenRoomPrivateMicFlagNotice(int idx, char flag) = 0; /*包括自己*/

	/*用户登录房间时，回调房间的私麦信息*/
	virtual void RecvPrivateMicWithLoginRoom(int idx, short mictype, int posindex, int roomid) = 0;
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
	virtual void RecvCrownKickoutSomeOne(int ret, int kickuse, int totalkick, int idx) = 0;
	/*室主禁止用户ip
	int ret;
	 0：成功
	 -1：该IP已被禁用
	 -2：该用户已离开房间
	
	int idx;
	被禁用者idx
	*/
	virtual void RecvForbidUserIp(int ret, int idx) = 0;

	//要求上传歌曲
	virtual void RecvUpdateSong() = 0;
	//回应服务器的返回用户歌唱信息
	//virtual void RecvUserSingInfo(int ret,int idx,const ResultData_UserSingInfo* pData) = 0;
	//获取用户接受礼物个数
	virtual void RecvGiftCount(int recvcount) = 0;

	//获取用户最高分歌曲信息及演唱歌曲个数
	virtual void RecvTopSongInfo(int songcount, int topsongscore, const char* topsongname, int len) = 0;

    //手机ktv聊天信息
    /*
     @param ret:返回值，0,OK;其他值,something error
     @param fromIdx:来自谁,如果发生错误，也有可能指发送的IDX
	 @param roomid: 说话者所在房间ID
     @param roomName:说话者在的房间，如果未在房间则为“0”
     @param content:说的内容
     @param chatTime:说话时间
     @param isPublic:是否为公聊
     @param isSend:是否是我发送的，如果为false，则是别人发给我的消息
     
     @注:chattime
     */
    virtual void RecvKtvChatInfo(int ret,int fromIdx,int roomid,const char* roomName,const char* content,
                                 int chatTime,bool isPublic=true,bool isSend=true) = 0;
    //手机接收到当前的播放列表，即当前ktv的点歌列表，用于更新
    /*
     @param pSonglist:信息结构体
     @param listcount:歌曲数量
     
     @注：pSonglist信息结构体中的数据已经是UTF8编码，无需转码
     */
    virtual void RecvKtvSongList(const ResultData_SongList* pSonglist,int listCount) = 0;
    //手机获取当前包厢的开关状态
    /*
     @param isOpen:true,包厢开启；false，包厢关闭，用户退出房间
     */
    virtual void RecvKtvRoomState(bool isOpen) = 0;
    //手机收到唱歌上榜通知,提示用户到个人信息里面去抢榜
    virtual void RecvKtvSongRanking() = 0;
	/*
	通知歌曲演唱结束
	*/
	virtual void RecvKtvSongNormalEnd(int nMusicId) = 0;
    /*
     收到的在线手机用户IDX列表更新数据
     只有IDX
     */
	virtual void RecvKtvIdxList(const int* pIdx,int number) = 0;
	/*
	通知用户头像更新
	@param [in] idx：指定的用户IDX
	@param [in] picture：用户的新头像
	*/
	virtual void RecvUserPicChange(int idx,const char* picture) = 0;
	/*
	通知获取到的喇叭用户信息
	*/
	virtual void RecvUserInfoForSpeaker(int errcode,ResultData_UserSampleInfo* info) = 0;
    /*
     接受发喇叭者信息超时
     需要判读idx数组中的具体idx
     @param [in] idx:idx数组； 数组中某个IDX如果为0，则原来的IDX已经获取到信息，只是底层把它置为0
     @param [in] count:idx数组大小
    */
	virtual void RecvUserInfoForSpeakerTimeOut(int idx[],int count) = 0;
    /*
     接受用户信息超时
     需要判读idx数组中的具体idx
     @param [in] idx:idx数组； 数组中某个IDX如果为0，则原来的IDX已经获取到信息，只是底层把它置为0
     @param [in] count:idx数组大小
     */
	virtual void RecvUserInfoTimeOut(int idx[],int count) = 0;
    //单个礼物图片下载完成通知
    //可以在这里去对之前没有获取到图片的地方尝试再去获取一下,或许已经下载下来了
    /*
     @param pictureName[in]:图片名称(含路径)

     主线程回调
     */
    
    virtual void RecvImageDownladFinished(const char* pictureName) = 0;
    /*
     主线程回调，在初始化下载地址之后成功的话就会回调，通知本地数据库礼物信息已经完成配置
     @param direct[in]:当为true的时候是直接回调，为false的时候为网络主线程回调。
     */
    virtual void RecvLocalGiftDatabaseFinished(bool direct=true) = 0;
};

#endif//RESPONSEBASE__H__
