#include "StdAfx.h"
#include "TestResponse.h"
#include "NetUtil/RequestBase.h"

extern RequestBase request;
CTestResponse::CTestResponse(void)
{
}


CTestResponse::~CTestResponse(void)
{
}

/*			add by lixin 2011/10/13 <begin> 聊天、喇叭功能			*/
void CTestResponse::SendChatFromUser(int ret, int to_idx, const char *content ,int contentlen, int time ){}
//接收私聊
void CTestResponse::RecvChatFromUser(int ret, int from_idx, const char *content ,int contentlen, int time ){}
//接收公聊（俱乐部内部）
void CTestResponse::RecvChatFromClub(int ret, int from_idx, const char* content,int contentlen){}
//接收公聊（房间内）
//ret:1--正常 0--被禁言 -1--不允许公聊
void CTestResponse::RecvChatFromRoom(int ret, int errcode,int from_idx, const char* content,int contentlen, int time ){}	
//接收大喇叭（全区）
void CTestResponse::RecvSpeakerFromAll(int ret, int type, int from_id, const char* msg, int msglen){}
//接收小喇叭（大厅）
void CTestResponse::RecvSpeakerFromLobby(int ret, int from_idx, int room_id,const char* room_name,const char* msg, int msglen, int time,int type,bool isSend){}
//接收礼物广播
void CTestResponse::RecvPresentBroad(int ret, ResultData_GiftInfo* guestGiftInfo, ResultData_GiftInfo* commGiftInfo){}
//接受系统文字广播
void CTestResponse::RecvSystemBroad( char type, const char * msg, int len ){}
//接受系统文字广播（幸运中奖信息）
void CTestResponse::RecvSystemBroad2(const char* nickname, int len, int idx, int roomid, int giftid, int luckmultiple){}
//接受系统文字公告
void CTestResponse::RecvSystemNotice( const char * msg, int len, bool once, int min ){}
//接受系统文字公告（幸运中奖信息）
void CTestResponse::RecvSystemNotice2(const char* nickname, int len, int idx, int roomid, int giftid, int luckmultiple){}
//接收房间临时公告
void CTestResponse::RecvRoomNotice(int ret, const char* content,int contentlen){}
/************************************************************************/
/*			add by jinguanfu 2009/12/28 <begin> 好友操作				*/
//取得好友位置
void CTestResponse::RecvFriendLocation(int ret, int errcode,
							int friend_idx,
							int lobbyid, 
							int roomid,
							int cseq){}
/*			add by jinguanfu 2009/12/28 <begin> 房间管理接口			*/
//接收取得待审核房间会员列表
void CTestResponse::RecvRoomApplyList (int ret ,int errcode,ResultDate_RoomApplyList *applylist,int cseq){}
//接收申请房间会员响应
void CTestResponse::RecvApplyRoomMember(int ret,int errcode,int cseq){}
//接收审核会员加入申请结果
void CTestResponse::RecvAuditAddApply (int ret, int errcode,int idx,int cseq){}
//接收拒绝房间申请系统提示
void CTestResponse::RecvRoomApplyRefuseMsg (int ret, int roomid){}
//接收房间会员列表
void CTestResponse::RecvRoomMemberList (int ret ,int errcode, ResultDate_RoomMemList* memlist,int cseq){}
//接收解除房间会员资格回应
void CTestResponse::RecvUnchainMember (int ret, int errcode, int idx,int cseq){}
//接收赋予用户房间主持助理权限回应
void CTestResponse::RecvRoomPesideAssistant (int ret , int errcode, int idx,int cseq){}
//接收赋予主持助理通知
void CTestResponse::RecvRoomNotice4PesideAssistant (int ret , int opidx,int bopidx){}
//接收赋予用户房间主持人权限回应
void CTestResponse::RecvRoomPeside (int ret ,int errcode, int idx,int cseq){}
//接收设置主持人权限房间通知
void CTestResponse::RecvRoomNotice4Peside (int ret , int opidx,int bopidx){}
//接收赋予用户房间副室主权限回应
void CTestResponse::RecvRoomSubHost (int ret ,int errcode, int idx,int cseq){}
//接收设置副室主权限房间通知
void CTestResponse::RecvRoomNotice4SubHost(int ret , int opidx,int bopidx){}
//接收房间黑名单
void CTestResponse::RecvBlackList(int ret,	int errcode,ResultDate_RoomBlackList* blacklist,int cseq){}
//接收黑名单用户添加/删除回应
void CTestResponse::RecvAddDelBlack(int ret , int errcode,int idx, int type){}
//接收房间设置面板信息
void CTestResponse::RecvRoomPanelInfo(int ret ,int errcode, ResultDate_RoomPanel* panelinfo){}
//接收设置房间密码回应
void CTestResponse::RecvRoomPassword(int ret,int errcode,int cseq){}
//接收设置房间密码消息提示
void CTestResponse::RecvRoomNotice4Password(int ret, int idx, char pwdflag){}
//接收设置房间开启/关闭(上锁)回应
void CTestResponse::RecvRoomSwitch(int ret,int errcode,char roomflag,int cseq){}
//接收房间开启/关闭(上锁)消息提示
void CTestResponse::RecvRoomNotice4RoomSwitch(int ret , int idx , char roomflag){}
//接收设置房间访问权限回应
void CTestResponse::RecvRoomPrivate(int ret , int errcode,char accessflag,int cseq){}
//接收房间访问权限消息提示
void CTestResponse::RecvRoomNotice4Private(int ret , int idx , char accessflag){}
//接收设置用户进出信息开启/关闭回应
void CTestResponse::RecvUserInOutSwitch(int ret , int errcode, char inoutflag,int cseq){}
//接收用户进出信息开启/关闭消息提示
void CTestResponse::RecvRoomNotice4UserSwitch (int ret , int idx , char inoutflag){}
//接收设置自由排麦开启/关闭回应
void CTestResponse::RecvGetMicSwitch (int ret, int errcode,char onmicflag,int cseq){}
//接收设置自由上麦开启/关闭回应
void CTestResponse::RecvOnMicSwitch (int ret, int errcode,char onmicflag,int cseq){}
//接收自由排麦开启/关闭消息提示
void CTestResponse::RecvRoomNotice4GetMicSwitch(int ret , int idx , char getmicflag){}
//接收自由上麦开启/关闭消息提示
void CTestResponse::RecvRoomNotice4OnMicSwitch(int ret , int idx , char onmicflag){}
//房间名称修改回应
void CTestResponse::RecvModifyRoomName(int ret,int errcode,int cseq){}
//接收房间名
void CTestResponse::RecvRoomName (int ret , int roomid , const char* roomname , int rnlen){}
//设置房间公告响应
void CTestResponse::RecvRoomAffiche(int ret,int errcode,int cseq){}
//接收房间公告更新事件
void CTestResponse::RecvRoomAfficheEvent(int ret){}
//设置允许房间公聊响应
void CTestResponse::RecvAllowRoomAllChat (int ret, int errcode,char chatflag,int cseq){}
//接收房间公聊设置消息提示
void CTestResponse::RecvRoomNotice4AllChat(int ret , int idx , char chatflag){}
//设置房间欢迎词响应
void CTestResponse::RecvRoomSalutatory(int ret, int errcode,int cseq){}
//接收房间欢迎词更新通知
void CTestResponse::RecvRoomNotice4Salutatory(int ret,int idx){}
//设置房间logo响应
void CTestResponse::RecvRoomLogo (int ret,int errcode ,int cseq){}
//接收房间LOGO更新通知
void CTestResponse::RecvRoomNotice4Logo(int ret,int idx){}
//接收房间冻结消息提示
void CTestResponse::RecvRoomNotice4Lock(int idx){}
//接受创建房间回应
void CTestResponse::RecvCreateRoom( int ret, int errcode, RoomListEx * info ){}
//接受练歌房完全信息
void CTestResponse::RecvTmpRoomInfo( int ret, int errcode, RoomListEx * info ){}
// 添加歌房广播
void CTestResponse::RecvRoomNotice4Add( RoomListEx * info ){}
void CTestResponse::RecvRoomNotice4Add( int idx ){}
// 删除歌房广播
void CTestResponse::RecvRoomNotice4Del( int idx ){}
// 接收房间易主通知
void CTestResponse::RecvRoomNotice4ChangeMaster(int idx){}
// 接收房间信息更改通知
void CTestResponse::RecvRoomNotice4ChangeInfo( int idx ){}
/*			add by jinguanfu 2009/12/28 <begin> 角色信息				*/
//取得他人角色信息
void CTestResponse::RecvPartInfo(int ret, int errcode, ResultData_PartInfo * part_info, int cseq){
	//m_pDataMgr->savePartInfo(part_info);
}
//取得自己角色信息,
void CTestResponse::RecvPartInfoBySelf(int ret, int errcode, ResultData_PartInfo * part_info, int cseq){
	
}
/*			add by jinguanfu 2010/1/5 <begin> 取得房间列表				*/
void CTestResponse::RecvRoomList(int ret,ResultDate_RoomList* roomlist){}
void CTestResponse::RecvRoomListEx(int ret,ResultDate_RoomListEx * roomlist){}
//打分通知
//void CTestResponse::RecvNotice4Score(int onmic_idx,int onmic_score,int pk_idx,int pk_score){}
//void CTestResponse::RecvNotice4ScoreAdd(int epchange, int silverchange){}
//下麦通知
void CTestResponse::RecvNotice4OffMic( int onmic_idx, int pk_idx, int exp, const char * scoreinfo, int len ){}
// add by cx 10-6-2
//回应是否邀请上麦成功
void CTestResponse::RecvRoomGivOnevjMic( int idx, char flag ){}
//是否同意邀请上麦成功
void CTestResponse::RecvRoomGivOnevjMicApply( int formidx ){}
//add by jinguanfu 2010/1/20 <begin> 
//是否同意添加对方为好友
//参数说明：int idx 添加好友者idx
void CTestResponse::RecvAddFriendApply(int idx){}
//add by jinguanfu 2010/1/20 <end> 
//void CTestResponse::RecvDeleteAvarta(int ret){}
//add by jinguanfu 2010/3/4 原有回调API改成接口<begin>
//连接成功
//成功连接认证服务器
void CTestResponse::OnAuthTunnelConnectSuccess(){
	printf("CTestResponse::OnAuthTunnelConnectSuccess\n");
}
//成功连接大厅服务器
void CTestResponse::OnLobbyTunnelConnectSuccess(){
	printf("CTestResponse::OnLobbyTunnelConnectSuccess\n");
}	
//成功连接房间服务器
void CTestResponse::OnRoomTunnelConnectSuccess(){
}	
//成功连接频道服务器
void CTestResponse::OnChannellistTunnelConnectSuccess(){
}	
//连接超时
//连接认证服务器超时
void CTestResponse::OnAuthTunnelConnectTimeout(){
	printf("CTestResponse::OnAuthTunnelConnectTimeout\n");
}		
//连接大厅服务器超时
void CTestResponse::OnLobbyTunnelConnectTimeout(){
}		
//连接房间服务器超时
void CTestResponse::OnRoomTunnelConnectTimeout(){
}	
//连接频道服务器超时
void CTestResponse::OnChannellistTunnelConnectTimeout(){
}
//连接错误
void CTestResponse::OnAuthTunnelConnectError( int code ){
	printf("CTestResponse::OnAuthTunnelConnectError : %d\n",code);
}	
void CTestResponse::OnLobbyTunnelConnectError( int code ){
}
void CTestResponse::OnRoomTunnelConnectError( int code ){
}
void CTestResponse::OnChannellistTunnelConnectError( int code ){
}
//服务器主动断开的连接,客户端recv == 0的时候,回调到以下的接口
//断开认证服务器
void CTestResponse::OnAuthTunnelClose(){
	printf("CTestResponse::OnAuthTunnelClose\n");
}	
//断开大厅服务器
void CTestResponse::OnLobbyTunnelClose(){
}	
//断开房间服务器
void CTestResponse::OnRoomTunnelClose(){
}	
//断开频道服务器
void CTestResponse::OnChannellistTunnelClose(){
}
//客户端recv异常,send异常,网络层buf溢出,select出现问题,都会回调到这个以下接口
void CTestResponse::OnAuthTunnelError( int code ){
	printf("CTestResponse::OnAuthTunnelError %d\n",code);
}	
void CTestResponse::OnLobbyTunnelError( int code ){

}
void CTestResponse::OnRoomTunnelError( int code ){

}
void CTestResponse::OnChannellistTunnelError( int code ){

}

/******************认证服务器操作接口*****************************/
//接收认证结果
void CTestResponse::RecvAuth(int ret, int errcode, const char * token, int len){

}
//接收新token
//void CTestResponse::RecvNewToken(int ret,ReserveDataS* pRD){}
void CTestResponse::RecvNewToken(int ret,int errcode){}
//接收频道列表
void CTestResponse::RecvSectionList(int ret,int errcode,ResultData_GetChannellist* pRDc,int cseq){
	
}
//接收频道列表(添加防沉迷信息)
void CTestResponse::RecvSectionList2(int ret,int errcode,int leftlogintime,ResultData_GetChannellist* pRDc,int cseq){}
/******************大厅服务器操作接口*****************************/
//接收大厅token认证
void CTestResponse::RecvTokenAuth4Lobby( int ret, int errcode, ResultData_LobbyLogin * pRDl, ResultData_SelfData * pSD, int cseq ){
	
}
//用户登录大厅通知
void CTestResponse::RecvUserLoginLobbyNotice(int ret,ResultData_LobbyLoginUser* pRlu){}
//用户退出大厅通知
void CTestResponse::RecvUserLogoutLobbyNotice(int ret,ResultData_LobbyLoginUser* pRlu){}
//接收用户信息
void CTestResponse::RecvUserInfo(int ret,int errcode,ResultData_UserFullInfo* pRDu){}
//接收好友状态
void CTestResponse::RecvFriendStatus(int ret,int errcode,ResultData_GetFriendlist* pRD){}
//接收黑名单状态
void CTestResponse::RecvBlackStatus(int ret,int errcode,ResultData_GetFriendlist* pRD){}
//接收俱乐部状态
void CTestResponse::RecvClubStatus(int ret,int errcode,ResultData_GetClublist* pRD){}
//接收添加好友结果
void CTestResponse::RecvAddFriend(int ret,int errcode,int idx){}
//接收添加黑名单结果
void CTestResponse::RecvAddBlack(int ret,int errcode,int cseq){}
//接收删除好友结果
void CTestResponse::RecvDelFriend(int ret,int errcode,int cseq){}
//接收删除黑名单结果
void CTestResponse::RecvDelBlack(int ret,int errcode,int cseq){}
//接收申请加入俱乐部结果
void CTestResponse::RecvApplyClub(int ret,int errcode,int cseq){}
//接收退出俱乐部结果
void CTestResponse::RecvExitClub(int ret,int errcode,int cseq){}
//接收最近几天内的加入俱乐部申请数量
void CTestResponse::RecvApplyListCount(int ret,int errcode,int count,int cseq){}
//接收最近几天内的加入俱乐部申请列表
void CTestResponse::RecvApplyList(int ret,int errcode,ResultData_ClubApplyList* pRD,int cseq){}
//通过加入俱乐部审批
void CTestResponse::RecvAddClubOk(int ret,ResultData_Approvelresult* pRD){}
//删除申请列表
void CTestResponse::RecvDelApplyList(int ret,ResultData_Approvelresult* pRD){}
//接收加入俱乐部广播消息(当有人加入俱乐部)
//modify by jinguanfu 2010/4/11
//void CTestResponse::RecvAddClubNotice(int ret ,int idx){}
//opidx--操作者idx
//addidx--加入者idx
void CTestResponse::RecvAddClubNoticeByLobby(int ret ,int clubid,const char* clubname,int opidx,int addidx){}
void CTestResponse::RecvAddClubNoticeByRoom( int opidx, int addidx ){}
//接收申请会员拒绝消息
//opidx--审核者idx
void CTestResponse::RecvDenyClubNoticeByLobby(int ret ,int clubid,const char* clubname,int opidx){}
//void CTestResponse::RecvDenyClubNoticeByRoom(int ret ,int clubid,char* clubname,int opidx){}
//接收会员退出俱乐部消息
void CTestResponse::RecvExitClubNoticeByLobby(int ret,int clubid,const char* clubname,int exitidx){}
void CTestResponse::RecvExitClubNoticeByRoom( int exitidx ){}
//接收会员被踢出俱乐部消息
//opidx--操作者idx
//kickidx--被踢出者idx
void CTestResponse::RecvKickClubNoticeByLobby(int ret,int clubid,const char* clubname,int opidx,int kickidx){}
void CTestResponse::RecvKickClubNoticeByRoom( int opidx, int kickidx ){}
//创建俱乐部
void CTestResponse::RecvCreateClub(int ret,int errcode,int clubid,int cseq){}
//解散俱乐部
void CTestResponse::RecvDismissClub(int ret,int errcode,int cseq){}
//接收俱乐部信息
void CTestResponse::RecvClubInfo(int ret,int errcode,ResultData_ClubFullInfo* pRD,int cseq){}
////接收房间列表
//void CTestResponse::RecvRoomList(int ret,ResultData_RoomList* pRD){}
//接收房间信息
void CTestResponse::RecvRoomInfo(int ret,ResultData_RoomInfo* pRD){}
//创建头像
void CTestResponse::RecvCreateAvatar(int ret,int errcode){}
//修改头像
void CTestResponse::RecvChangeAvatar(int ret,int errcode){}
//修改昵称
void CTestResponse::RecvChangeNickName(int ret,int errcode,const char * name, int len){}
//进入房间通知
void CTestResponse::RecvLobbyEnterRoom(int ret,ResultData_LobbyRoomEnter* pRD){}
//离开房间通知
void CTestResponse::RecvLobbyLeaveRoom(int ret,ResultData_LobbyRoomEnter* pRD){}
//添加好友通知
void CTestResponse::RecvAddFriendNotice(int ret,int idx){}
//取得大厅信息
void CTestResponse::RecvLobbyInfo(int ret,int errcode,ResultData_LobbyName* pRD,int cseq){}
//接收来自大厅的权限更改消息 add by jinguanfu 2010/6/5
void CTestResponse::RecvChangePower(int optionidx/*操作者ID*/,char identity/*更改后权限*/){}
//取大厅名(CMD_SYSTEMMSG_L2C)
//void CTestResponse::RecvLobbyName(){}
/******************房间操作接口*****************************/
//接收房间token验证 //
void CTestResponse::RecvTokenAuth4Room(int ret,int errcode){

}
//进入房间
void CTestResponse::RecvEnterRoom(int ret,int errcode,ResultData_LoginInfo* pRDlf,int cseq){
	
}
//用户进入房间通知
void CTestResponse::RecvEnterRoomNotice(int ret,ResultData_UserInfo* pRDu){}
//用户离开房间通知
void CTestResponse::RecvLeaveRoomNotice(int ret,int idx){}
//接收房间详细信息
void CTestResponse::RecvRoomAllInfo(int ret,int errcode,ResultData_RoomAllInfo *pRD,int cseq){}
//接收排麦成功通知
void CTestResponse::RecvWaitMicNotice(int ret,ResultData_MicInfo *pRD){}
////接收上麦准备通知
//void CTestResponse::RecvWaitMicReadyNotice(int ret,ResultData_MicInfo *pRD){}
//接收上麦准备完成通知
//void CTestResponse::RecvOnMicReadyOkNotice(int ret,ResultData_MicInfo *pRD){}
//接收上麦通知
void CTestResponse::RecvOnMicNotice(int ret,ResultData_MicInfo *pRD){}
//取消排麦成功通知
void CTestResponse::RecvCancelWaitMicNotice(int ret,int opidx,int bopidx){}
//接收加入PK麦通知
void CTestResponse::RecvWaitPKMicNotice(int ret,ResultData_MicInfo *pRD){}
//接收取消PK麦通知
void CTestResponse::RecvCancelWaitPKMicNotice(int ret,int opidx,int bopidx){}
//接收升麦成功通知
void CTestResponse::RecvUpWaitMicNotice(int ret,ResultData_UDMicInfo *pRD){}
//接收降麦成功通知
void CTestResponse::RecvDownWaitMicNotice(int ret,ResultData_UDMicInfo *pRD){}
//接收踢出房间通知
void CTestResponse::RecvKickOutNotice(int idx,int bidx){}
//接收上VJ麦（主持麦）通知
void CTestResponse::RecvOnVJMicNotice(int ret,int idx){}
//接收下VJ麦（主持麦）通知
void CTestResponse::RecvOffVJMicNotice(int ret,int idx){}
//接收被下麦通知
void CTestResponse::RecvGiveOffMicNotice(int ret,int idx){}
//接收被禁言通知
void CTestResponse::RecvForbidenNotice(int ret,ResultData_ForbidenInfo* pRD){}
//接收黑名单更新通知
void CTestResponse::RecvUpdateBlackListNotice(int ret,ResultData_UpBlackList* pRD){}
//接收管理员更新通知
void CTestResponse::RecvUpdateMGRListNotice(int ret,ResultData_UpMgrList* pRD){}
//接收赠送礼物通知
void CTestResponse::RecvSendFlowerNotice(int ret,ResultData_SendFlowerInfo* pRD){
	
}
//add by jinguanfu 2010/3/4 原有回调API改成接口<end>
//接收房间重复登录消息
void CTestResponse::RecvRoomKickUser(){}
//接收大厅重复登录消息
void CTestResponse::RecvLobbyKickUser(){}
//从大厅接收房间简易信息
void CTestResponse::RecvRoomSimpleInfo(int ret,int errcode,ResultData_RoomSimpleInfoList* pRD){}
void CTestResponse::RecvRoomSimpleInfoEx(int ret,int errcode,ResultData_RoomSimpleInfoList* pRD, int type){}
//接收用户退出房间会员回应
void CTestResponse::RecvExitRoomMember(int ret,int errcode){}
//接收WEB服务器上邮件信息
void CTestResponse::RecvMailInfo(int ret,char* context,int length){}
//接收大厅在线人数
void CTestResponse::RecvLobbyOnlineNum(int ret,int errcode,ResultData_LobbyOnlineList* pRD){}
//接收送礼回应消息	//add by jinguanfu 2010/4/27
void CTestResponse::RecvSendFlower(int ret,int errcode,int recevie_idx,int item_id,int count,int gold,int silver){

}
//接收发送大喇叭回应 //add by jinguanfu 2010/5/13
void CTestResponse::RecvSpeakerToAll(int ret,int errcode,int gold,int silver){}
//接收发送小喇叭回应 //add by jinguanfu 2010/5/13
void CTestResponse::RecvSpeakerToLobby(int ret,int errcode,int gold,int silver){

}
//接收由管理员设为一般会员的回应 //add by jinguanfu 2010/5/13
void CTestResponse::RecvRoomMember (int ret , int errcode, int idx,int cseq){}
//接收由管理员设为一般会员的通知 //add by jinguanfu 2010/5/13
void CTestResponse::RecvRoomNotice4Member (int ret , int opidx,int bopidx){}
//接收房间内有人改变了avarat的广播 //add by cx 2010-5-20
void CTestResponse::RecvRoomNotice4ChangeAvarat (int ret, int idx){}
//接收房间内错误操作提示 //add by cx 2010-6-6
void CTestResponse::RecvOneError( short cmd ){}
//add by cx 2010-7-6 接受购买物品回应
void CTestResponse::RecvBuyPack( int ret, int errcode, int gold, int silver, short * goods_id, int * global_id, int count, const char * day ){}
// 接受购买物品回应
void CTestResponse::RecvBuyForSend( int ret, int errcode, int idx, int gold, int silver ){}
//接受获取背包回应
void CTestResponse::RecvGetPack( int ret, int errcode, ResultData_Pack * pPack, int count, int all_count ){}
//接受删除物品回应
void CTestResponse::RecvDelPack( int ret, int errcode, int cseq ){}
//接受商品过期通知
void CTestResponse::RecvTimeoutPack( int count, int * global_id ){}
//接受商品赠送
void CTestResponse::RecvSendPack( int idx, int global_id, short goods_id, const char * day, const char * msg ){}
//接受使用物品回应
void CTestResponse::RecvUsePack( int ret, int errcode, int global_id ){}
// 升级广播
void CTestResponse::RecvNotice4LevelUp( int idx, char level ){}
// 经验提升
void CTestResponse::RecvExpChange( int nexp, int level ){}
// vip改变
void CTestResponse::RecvVipChange( int idx, int level, char is_vip ){}
// 钱币改变
void CTestResponse::UpdataMoney( int r_gold, int r_silver ){
	
}
// 更新自己的信息
void CTestResponse::RecvUpdataMyInfo(){}
// 接受烟花广播
void CTestResponse::RecvFireworks( int ret, int errcode, int cate_idx, int count, int gold, int silver ){}
// 接受烟花广播
void CTestResponse::RecvFireworksNotice( int player_idx, int cate_idx, int count ){}
// 中奖？
void CTestResponse::RecvLucky( int player_idx, int cate_idx, int lotte ){}
// 中奖？
void CTestResponse::RecvLucky( int room_idx, int player_idx, int cate_idx, int lotte ){}
// 列表
void CTestResponse::RecvGiftTopList( ResultData_GiftList * gift_list ){}
// 在线时间
void CTestResponse::RecvOnlineTime( int idx, int day_time, int online ){}
// 获取新手任务
void CTestResponse::RecvGetNewUserTask( short step ){}
// 返回新手任务提交结果
void CTestResponse::RecvTaskStep( int ret, int err_code, int gold, int silver, int total_gold, int total_silver ){}
void CTestResponse::RecvTaskGift( int ret, int err_code, int item_id, int object_id, const char * time_buf ){}
// 商品信息更改通知
void CTestResponse::RecvNotice4ChangeGoods(){}
// 服务器请求刷新背包
void CTestResponse::RecvGetBagAgain(){}
// 返回用户印章
void CTestResponse::RecvUserSeel( int user_idx, int seel_idx ){}
// 返回用户防沉迷状态
void CTestResponse::EnterAllMentState( int idx, short state ){}
// room loge
void CTestResponse::RecvRoomLoge( int room_idx, const char * loge, int len ){}
// 新分组版本号
void CTestResponse::RecvNewVersionId( int version_id ){
	
}
// 分组信息
void CTestResponse::RecvGroupInfo( int count, ResultData_GroupInfo * pInfo ){
	
}
// 获取组里面的房间列表
void CTestResponse::RecvRoomListInGroup( short group_idx, int count, int * list ){
	
}
void CTestResponse::RecvRoomListInGroup(short count, ResultData_GroupRoomInfo* pInfo)
{
	//空
}
// 上传日志通知
void CTestResponse::RecvNotice4CollectData( const char * server_ip, short server_port ){}
// 获取歌曲信息
void CTestResponse::RecvSongInfoList( short totalnum, short curnum, ResultData_SongInfo * info ){}
// 获取音视频服务器列表
void CTestResponse::RecvAvServerList( ResultData_AvServer * list, int count ){}
// 更新音视频服务器列表
void CTestResponse::UpdataAvServerList(){}
// 被提出游戏服务器
void CTestResponse::RecvCheckOutGame( int result ){}
// 获取离线消息
void CTestResponse::RecvOffLineMsg( int count, ResultData_OffLineMsg * msg ){}
// 返回分组置顶信息
void CTestResponse::RecvRoomSortInGroup( int idx, int count, ResultData_RoomSort * list ){}
// 通知分组置顶信息改变
void CTestResponse::RecvRoomSortChange( int idx ){}
// 歌手等级
void CTestResponse::RecvSingerLevel(int idx, short curlevel, int expvalue){}
// 财富等级
void CTestResponse::RecvWealthLevel(int idx, short curlevel, int expvalue){}

//////////////////////////////////////////////////////////////////////////

// 礼物失效（印章）的协议
void CTestResponse::RecvGiftInvalid( int idx, int GiftID ){}
// 礼物有效（印章）的协议
void CTestResponse::RecvGiftValid( int idx, int GiftID ){}
// 显示房间可领取收益
void CTestResponse::RecvShowRoomIncom( int income ){}
// 显示收益已领取记录
void CTestResponse::RecvShowRoomGetIncom( ResultData_IncomeDate * data, int count ){}
// 室主领取房间内收益
void CTestResponse::RecvGetRoomIncom( int ret ){}
// 礼物信息更改通知
void CTestResponse::RecvNotice4ChangeFlower(){}
// 权限配置更新通知
void CTestResponse::RecvNotice4ChangePower(){}
// 更改音视频服务器广播
void CTestResponse::RecvNotice4AvserverChange( ResultData_AvChange * info ){}
// 更改gm
void CTestResponse::RecvChangeGM( int idx, char add ){}
// 上麦的人的pc概况
void CTestResponse::RecvMicPcInfo( int idx, const char * buf, int len ){}
// 回应是否邀加入房间成功
void CTestResponse::RecvSendJoinRoomInvite( int wseq, int ret, int err_code ){}
// 回应是否邀加入房间成功
void CTestResponse::RecvInviteJoinRoom( int idx ){}
//取得当日礼物赠送记录
void CTestResponse::RecvGiftSend( int count, ResultData_GiftCount * pInfo ){}
//取得当日礼物接收记录
void CTestResponse::RecvGiftRecv( int count, ResultData_GiftCount * pInfo ){}
//返回封mac结果
void CTestResponse::RecvKickUserByMac( int ret, int idx ){}
//返回封ip结果
void CTestResponse::RecvKickUserByIp( int ret, int idx ){}
//返回设置在麦时间结果
void CTestResponse::RecvSetMicTime( int ret, int idx ){}
//广播在麦时间延长
void CTestResponse::RecvNotice4SetMicTime( int idx, int time ){}
//取排麦列表
void CTestResponse::RecvWaitMicList( short count, int* idxs ){}
//防沉迷时间到,断开连接
void CTestResponse::RecvIndulgeTimeUp(){}
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
void CTestResponse::RecvFamilyInfo(int flag, int familyNum, ResultData_Family* familys){}

/*
av配置信息
*/
void CTestResponse::RecvAVConfigInfo(int infolen, const char* info){}

/* 私麦                                                                 */
/************************************************************************/
/*上私麦*/
void CTestResponse::RecvOpenPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
void CTestResponse::RecvOpenPrivateMicFailed(int errorcode, int idx, int bidx){}
void CTestResponse::RecvOpenPrivateMicNotice(int idx, int mictype){} /*不包括自己*/

/*下私麦*/
void CTestResponse::RecvClosePrivateMicFailed(int errorcode, int idx, int bidx){}
void CTestResponse::RecvClosePriveteMicNotice(int idx){} /*包括自己*/

/*被抱下私麦*/
void CTestResponse::RecvClosedPrivateMicFailed(int errorcode, int idx, int bidx){}
void CTestResponse::RecvClosedPriveteMicNotice(int idx, int bidx){} /*包括自己*/

/*观看私麦*/
void CTestResponse::RecvWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
void CTestResponse::RecvWatchPrivateMicFailed(int errorcode, int idx, int bidx){}
void CTestResponse::RecvRequestWatchedPrivateMic(int idx){} /*idx=请求者, 是否同意该idx观看你的私麦*/
void CTestResponse::RecvWatchedPrivateMicFailed(int errorcode, int idx, int bidx){}

void CTestResponse::RecvWatchedPrivateMicReject(int idx){} //对方拒绝，idx=谁拒绝的

/*邀请私麦*/
void CTestResponse::RecvInviteWatchPrivateMicSuccess(){}
void CTestResponse::RecvInviteWatchPrivateMicFailed(int errorcode, int idx, int bidx){}
void CTestResponse::RecvRequestInvitedWatchPrivateMic(int idx){} /*idx=邀请者,是否同意观看idx的私麦*/
void CTestResponse::RecvInvitedWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
void CTestResponse::RecvInvitedWatchPrivateMicFailed(int errorcode, int idx, int bidx){}

void CTestResponse::RecvInvitedWatchPrivateMicReject(int idx){}//对方拒绝，idx=谁拒绝的

/*管理房间私麦*/
void CTestResponse::RecvOpenRoomPrivateMicFlag(int ret, char flag){}
void CTestResponse::RecvOpenRoomPrivateMicFlagNotice(int idx, char flag){} /*包括自己*/

/*用户登录房间时，回调房间的私麦信息*/
void CTestResponse::RecvPrivateMicWithLoginRoom(int idx, short mictype, int posindex, int roomid){}
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
void CTestResponse::RecvCrownKickoutSomeOne(int ret, int kickuse, int totalkick, int idx){}
/*室主禁止用户ip
int ret{}
 0：成功
 -1：该IP已被禁用
 -2：该用户已离开房间

int idx{}
被禁用者idx
*/
void CTestResponse::RecvForbidUserIp(int ret, int idx){}

//要求上传歌曲
void CTestResponse::RecvUpdateSong(){}
//回应服务器的返回用户歌唱信息
//void CTestResponse::RecvUserSingInfo(int ret,int idx,const ResultData_UserSingInfo* pData){}
//获取用户接受礼物个数
void CTestResponse::RecvGiftCount(int recvcount){}

//获取用户最高分歌曲信息及演唱歌曲个数
void CTestResponse::RecvTopSongInfo(int songcount, int topsongscore, const char* topsongname, int len){}

//http请求完成的返回接口
// void CTestResponse::OnFinish(_TaskDownload* downloadInfo,bool success)
// {
// 	//回调到java
// 	CTestResponse* pJniResponse = getSingleton();
// 	//返回URL,并且返回saveBuf里面的内容，如果download为ture，则是空字符串
// }
//手机ktv聊天信息
void CTestResponse::RecvKtvChatInfo(int ret,const int fromIdx,int roomid,const char* roomName,const char* content
	,int chatTime,bool isPublic,bool isSend){
	
}
//手机接收到当前的播放列表，即当前ktv的点歌列表，用于更新
/*
 @param pSonglist:信息结构体
 @param listcount:歌曲数量
 */
void CTestResponse::RecvKtvSongList(const ResultData_SongList* pSonglist,int listCount){
	
}
//手机获取当前包厢的开关状态
/*
 @param isOpen:true,包厢开启；false，包厢关闭，用户退出房间
 */
void CTestResponse::RecvKtvRoomState(bool isOpen)
{

}
//手机收到唱歌上榜通知,提示用户到个人信息里面去抢榜
void CTestResponse::RecvKtvSongRanking(){
}
/*
	收到的在线手机用户IDX列表更新数据
	只有IDX
*/
void CTestResponse::RecvKtvIdxList(int* pIdx,int number)
{

}
/*
通知用户头像更新
@param [in] idx：指定的用户IDX
@param [in] picture：用户的新头像
*/
void CTestResponse::RecvUserPicChange(int idx,const char* picture)
{

}
//单个礼物图片下载完成通知
void CTestResponse::RecvImageDownladFinished(const char* pictureName){
	printf("%s is downloaded!\n",pictureName?pictureName:"");
}

void CTestResponse::RecvLocalGiftDatabaseFinished()
{

}

