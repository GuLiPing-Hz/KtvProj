//
//  com_ios_NetUtil.cpp
//  NetUtil
//
//  Created by guliping on 14-4-18.
//  Copyright (c) 2014年 天格科技. All rights reserved.
//

#include "com_ios_NetUtil.h"
#include "netutil/RequestBase.h"
#include "NetUtil/localstorage/ServerData.h"
#include "NetUtil/localstorage/DataMgr.h"
#include "NetUtil/NetApp.h"
#include "NetUtil/xml/tinyxml.h"
#include "NetUtil/MessageFilter.h"

RequestBase g_RB;
CDataMgr g_DM;

//启动客户端
int StartClient(){
    return g_RB.StartClient();
}
//停止客户端
void StopClient(){
    g_RB.StopClient();
}
//取得Token
const char* GetToken(){
    return g_RB.GetToken();
}
//取得Token长度
unsigned int GetTokenlen(){
    return g_RB.GetTokenlen();
}

//认证服务器是否连接
bool IsConnectAuth(){
    return g_RB.IsConnectAuth();
}
//大厅是否连接
bool IsConnectLobby(){
    return g_RB.IsConnectLobby();
}
//房间是否连接
bool IsConnectRoom(){
    return g_RB.IsConnectRoom();
}
//大厅是否token
bool IsTokenLoginLobby(){
    return g_RB.IsTokenLoginLobby();
}
//房间是否token
bool IsTokenLoginRoom(){
    return g_RB.IsTokenLoginRoom();
}

int ConnectAuth(const char* host,short port,int timeout){
    return g_RB.ConnectAuth(host, port,timeout);
}
int DisConnectAuth(){
    return g_RB.DisConnectAuth();
}
int ConnectLobby(const char* host,short port,int timeout){
    return g_RB.ConnectLobby(host, port,timeout);
}
int DisConnectLobby(){
    return g_RB.DisConnectLobby();
}
int ConnectRoom(const char* host,short port,int type,int timeout){
    return g_RB.ConnectRoom(host, port, type,timeout);
}
int DisConnectRoom(){
    return g_RB.DisConnectRoom();
}

int IdPassAuth(const char* id,const char* passwd){
    return g_RB.IdPassAuth(id, passwd);
}
int TouristAuth(){
    return g_RB.TouristAuth();
}
//Room API
//验证token
int ConfirmToken(){
    return g_RB.ConfirmToken();
}
//进入房间
int EnterRoom(int cseq,int roomid, const char* passwd, short type){
    return g_RB.EnterRoom(cseq, roomid, passwd, type);
}
//取得房间当前运行信息
int GetRoomAllInfomation(int cseq){
    return g_RB.GetRoomAllInfomation(cseq);
}
//取麦序列表
int GetWaitMicList(){
    return g_RB.GetWaitMicList();
}
//排麦
int RoomWaitMic( int musicid, short bk, char level, const char * name, char speed ){
    return g_RB.RoomWaitMic(musicid, bk, level, name, speed);
}
//取消排麦
int RoomCancleWaitMic( int idx ){
    return g_RB.RoomCancleWaitMic(idx);
}
//提升麦序
int RoomUpWaitMic(char tag, int idx, char flag){
    return g_RB.RoomUpWaitMic(tag, idx, flag);
}
//flag==>MIC_UP_DOWN_FLAG
//下降麦序
int RoomDownWaitMic(char tag, int idx, char flag){
    return g_RB.RoomDownWaitMic(tag, idx, flag);
}
//flag==>MIC_UP_DOWN_FLAG
//上麦准备ok
//int RoomOnMicReadyOK( short music_len );
//准备下麦
//int RoomOffMicReady();
//把指定用户提出房间
int RoomKickSomeOne(int idx){
    return g_RB.RoomKickSomeOne(idx);
}
//禁言某人（或取消禁言）
int RoomForbidenSomeOne(int idx, char flag){
    return g_RB.RoomForbidenSomeOne(idx, flag);
}
//上主持人麦
int RoomOnvjMic(){
    return g_RB.RoomOnvjMic();
}
//下主持人麦
int RoomOffvjMic(){
    return g_RB.RoomOffvjMic();
}
//把主持人麦给某人
int RoomGivOnevjMic(int idx){
    return g_RB.RoomGivOnevjMic(idx);
}
// 是否同意上主持人麦 add by cx 10-6-2
int ApplyGetVJmic( int idx, char flag ){
    return g_RB.ApplyGetVJmic(idx, flag);
}
//让主持人下麦
int RoomGiveOffvjMic(int idx){
    return g_RB.RoomGiveOffvjMic(idx);
}
//让普通麦上麦用户下麦
int RoomGiveOffMic(int idx){
    return g_RB.RoomGiveOffMic(idx);
}
//flag==>ADD_SUB_FLAG
//跟新房间黑名单列表(单条添加和删除)
int RoomUpdateBlackList(int cseq,int idx, char flag){
    return g_RB.RoomUpdateBlackList(cseq, idx, flag);
}
//flag==>ADD_SUB_FLAG
//跟新管理员列表(单条添加和删除)
int RoomUpdateMgrList(int idx, char flag, char identity){
    return g_RB.RoomUpdateMgrList(idx, flag, identity);
}

//登录大厅
int LoginLobby( const char * mac,int loginway){
    return g_RB.LoginLobby(mac,loginway);
}
//登录大厅(重连)
int LoginLobbyAuto( const char * mac, int roomid ){
    return g_RB.LoginLobbyAuto(mac, roomid);
}
//获取用户资料 参数一用户idx列表,参数二数量
int GetUserInfo_AII(int idxl[],int count){
    return g_RB.GetUserInfo(idxl, count);
}
//获取好友列表
int GetFriendListWithoutUserInfo(){
    return g_RB.GetFriendListWithoutUserInfo();
}
//获取黑名单列表
int GetBlackListWithoutUserInfo(){
    return g_RB.GetBlackListWithoutUserInfo();
}
//添加好友
int AddFriend(int idx){
    return g_RB.AddFriend(idx);
}
//添加黑名单
int AddBlack(int cseq,int idx){
    return g_RB.AddBlack(cseq, idx);
}
//删除好友
int DelFriend(int cseq,int idx){
    return g_RB.DelFriend(cseq, idx);
}
//删除黑名单
int DelBlack(int cseq,int idx){
    return g_RB.DelBlack(cseq, idx);
}
//获取通道列表
int GetChannellist( int version, const char * mac ){
    return g_RB.GetChannellist(version, mac);
}
//*********************************room manage api*********************************
//获取当前大厅的房间列表
int GetRoomList(int cseq){
    return g_RB.GetRoomList(cseq);
}
int GetRoomListEx(int cseq, int type){
    return g_RB.GetRoomListEx(cseq, type);
}
//获取房间信息,可以是一组房间
int GetRoomInfo(int cseq,int roomcount, int *roomlist){
    return g_RB.GetRoomInfo(cseq, roomcount, roomlist);
}
int GetTmpRoomInfo( int idx ){
    return g_RB.GetTmpRoomInfo(idx);
}
//*********************************************************************************
//用户创建头像信息
int CreateAvatar( char * nickname, ResultData_Avatar_Updata * avarat ){
    return g_RB.CreateAvatar(nickname, avarat);
}
//avatarname长度限制20,partlen,decoration大小限制为10
int ChangeAvatar( ResultData_Avatar_Updata * avarat ){
    return g_RB.ChangeAvatar(avarat);
}
//修改昵称
int ChangeNickName( int idx, char * name, int len ){
    return g_RB.ChangeNickName(idx, name);
}
//给好友送花
int SendFlower(int s_idx,int r_idx,int cate_id,int number){
    return g_RB.SendFlower(s_idx, r_idx, cate_id, number);
}
//发送大喇叭
int BroadAll(const char *content){
    return g_RB.BroadAll(content);
}
//发送小喇叭
int BroadLobby(int lobbyidx,const char *content){
    return g_RB.BroadLobby(lobbyidx, content);
}
//发送房间临时公告
int SendRoomNotice_SI(const char *content){
    return g_RB.SendRoomNotice(content);
}
//取得指定大厅信息
int GetLobbyInfo(int cseq,int lobbyid){
    return g_RB.GetLobbyInfo(cseq, lobbyid);
}
//邀请用户进入表演麦麦序
int InviteMic(int useridx){
    return g_RB.InviteMic(useridx);
}
/*			add by jinguanfu 2010/3/2 <end> 回调API改造成接口			*/
/*			add by jinguanfu 2009/12/18 <begin> 聊天、喇叭功能			*/
//发送用户私聊
int SendChatToUser(int to_idx, const char *content, int contentlen){
    return g_RB.SendChatToUser(to_idx, content);
}
//发送公聊（俱乐部内部）
int SendChatToClub(const char *content,int contentlen){
    return g_RB.SendChatToClub(content);
}
//发送公聊（房间内）
int SendChatToRoom(const char* content,int contentlen){
    return g_RB.SendChatToRoom(content);
}
//发送大喇叭（全区）
int SendSpeakerToAll(const char* msg,int msglen){
    return g_RB.SendSpeakerToAll(msg);
}
//发送小喇叭（大厅）
int SendSpeakerToLobby(int lobbyid, const char* msg,int msglen){
    return g_RB.SendSpeakerToLobby(lobbyid, msg);
}
//发送房间临时公告
int SendRoomNotice(const char *content,int contentlen){
    return g_RB.SendRoomNotice(content);
}
/*			add by jinguanfu 2009/12/28 <begin> 好友操作				*/
//查询好友位置
int QueryFriendLocation(int cseq,int friend_idx){
    return g_RB.QueryFriendLocation(cseq, friend_idx);
}
/*			add by jinguanfu 2009/12/28 <begin> 房间管理接口			*/
//请求取得待审核房间会员列表
int GetRoomApplyList (int cseq){
    return g_RB.GetRoomApplyList(cseq);
}
//用户申请房间会员
int ApplyRoomMember(int cseq){
    return g_RB.ApplyRoomMember(cseq);
}
//审核会员加入申请
int AuditAddApply (int cseq,int idx,char type){
    return g_RB.AuditAddApply(cseq, idx, type);
}
//取房间会员列表
int GetRoomMemberList (int cseq){
    return g_RB.GetRoomMemberList(cseq);
}
//解除房间会员资格
int UnchainMember (int cseq,int idx){
    return g_RB.UnchainMember(cseq, idx);
}
//赋予用户房间主持助理权限
int SetRoomPesideAssistant(int cseq,int idx){
    return g_RB.SetRoomPesideAssistant(cseq, idx);
}
//赋予用户房间主持人权限
int SetRoomPeside (int cseq, int idx)
{
    return g_RB.SetRoomPeside(cseq, idx);
}
//赋予用户房间副室主权限
int SetRoomSubHost(int cseq, int idx){
    return g_RB.SetRoomSubHost(cseq, idx);
}
//设置为房间会员（由管理员变为会员）	//add by jinguanfu 2010/5/13
int SetRoomMember(int cseq, int idx){
    return g_RB.SetRoomMember(cseq, idx);
}
//取得房间黑名单
int GetBlackList (int cseq){
    return g_RB.GetBlackList(cseq);
}
//黑名单用户添加/删除
int AddDelBlack (int idx ,char type){
    return g_RB.AddDelBlack(idx, type);
}
//查看房间设置面板
int GetRoomPanelInfo (int cseq){
    return g_RB.GetRoomPanelInfo(cseq);
}
//设置房间密码
int SetRoomPassword(int cseq,const char *password , int passwordlen){
    return g_RB.SetRoomPassword(cseq, password);
}
//设置房间开启/关闭(上锁)
int SetRoomSwitch(int cseq,char roomflag){
    return g_RB.SetRoomSwitch(cseq, roomflag);
}
//设置房间仅对会员开放
int SetRoomPrivate(int cseq,char accessflag){
    return g_RB.SetRoomPrivate(cseq, accessflag);
}
//用户进出信息开启/关闭
int SetUserInOutSwitch(int cseq,char inoutflag){
    return g_RB.SetUserInOutSwitch(cseq, inoutflag);
}
//设置自由排麦开启/关闭
int SetGetMicSwitch(int cseq,char getmicflag){
    return g_RB.SetGetMicSwitch(cseq, getmicflag);
}
//设置自由上麦开启/关闭
int SetOnMicSwitch(int cseq,char onmicflag){
    return g_RB.SetOnMicSwitch(cseq, onmicflag);
}
//房间名称修改
int ModifyRoomName(int cseq,const char* roomname, int namelen){
    return g_RB.ModifyRoomName(cseq, roomname);
}
//设置房间公告
int SetRoomAffiche (int cseq,const char* content, int contentlen){
    return g_RB.SetRoomAffiche(cseq, content);
}
//设置允许房间公聊
int AllowRoomAllChat (int cseq , char chatflag){
    return g_RB.AllowRoomAllChat(cseq, chatflag);
}
//设置房间欢迎词
int SetRoomSalutatory (int cseq,const char* content , int contentlen){
    return g_RB.SetRoomSalutatory(cseq, content);
}
//设置房间logo
int SetRoomLogo(int cseq, const char* path, int pathlen){
    return g_RB.SetRoomLogo(cseq, path);
}
/*			add by jinguanfu 2009/12/28 <begin> 角色信息				*/
//发送取得角色信息请求
int GetPartInfo(int cseq,int idx){
    return g_RB.GetPartInfo(cseq, idx);
}
//取得指定大厅的房间列表
int GetRoomList_II(int cseq,int lobbyid){
    return g_RB.GetRoomList(cseq,lobbyid);
}
/************************************************************************/
/*			add by jinguanfu 2010/1/14 <begin> 							*/
//离开房间（切换房间时使用）
int LeaveRoom(){
    return g_RB.LeaveRoom();
}
/*			add by jinguanfu 2010/1/20 <begin> 							*/
//是否同意添加好友
//参数说明：int		idx		请求添加好友的用户
//			char	flag	0--同意 1--不同意
int ApplyAddFriend(int idx, char flag){
    return g_RB.ApplyAddFriend(idx, flag);
}
/*add by jinguanfu 2010/1/27 删除角色信息 ********/
/*参数说明：int		idx		用户idx	*/
//int DeleteAvarta(int idx);
//设置回调类对象
void SetResponseHandle(ResponseBase* pRb){
    g_RB.SetResponseHandle(pRb);
}

//add by jinguanfu 2010/4/8 取得简单房间信息
int GetRoomSimpleInfo(){
    return g_RB.GetRoomSimpleInfo();
}
int GetRoomSimpleInfoEx( int type ){
    return g_RB.GetRoomSimpleInfoEx(type);
}
//add by jinguanfu 2010/4/11 用户退出房间会员
int ExitRoomMember(){
    return g_RB.ExitRoomMember();
}
//add by jinguanfu 2010/4/15 取得大厅在线人数
int GetLobbyOnlineNum(){
    return g_RB.GetLobbyOnlineNum();
}
//add by cx 10-7-6 购买
int BuyPack( ResultData_Buy * pPack, int count, int type, int idx ){
    return g_RB.BuyPack(pPack, count, type, idx);
}
int GetPack(){
    return g_RB.GetPack();
}
int DelPack( int cseq, int * idx, int count ){
    return g_RB.DelPack(cseq, idx, count);
}
int UsePack( int idx ){
    return g_RB.UsePack(idx);
}
int CreateRoom( ResultData_CreateRoom * room ){
    return g_RB.CreateRoom(room);
}
//烟花
int SendFireworks( int cate_idx, int count ){
    return g_RB.SendFireworks(cate_idx, count);
}

//////////////////////////////////////////////////////////////////////////

//取得大厅礼物排行榜信息
int GetGiftTopList(){
    return g_RB.GetGiftTopList();
}
//取得在线时间
int GetOnlineTime( int idx ){
    return g_RB.GetOnlineTime(idx);
}
//取得新手任务
int GetNewUserTask(){
    return g_RB.GetNewUserTask();
}
//提交新手任务
int TaskStep( short step ){
    return g_RB.TaskStep(step);
}
int TaskGift( short step ){
    return g_RB.TaskGift(step);
}
//获取礼物滚动广播
int GetGiftInfo(){
    return g_RB.GetGiftInfo();
}
//获取系统文字公告
int GetSystemBroad(){
    return g_RB.GetSystemBroad();
}
//获取印章信息
int GetUserSeel( int * idx, int count ){
    return g_RB.GetUserSeel(idx, count);
}
//发送用户pc配置
int SendSysteInfo( char * data, int len ){
    return g_RB.SendSysteInfo(data);
}
//获取分组信息
int GetGroupInfo(){
    return g_RB.GetGroupInfo();
}
//获取组里面的房间列表
int GetRoomListInGroup( int group_idx ){
    return g_RB.GetRoomListInGroup(group_idx);
}
int GetRoomListInGroupBatch(short* groupId,short count){
    return g_RB.GetRoomListInGroupBatch(groupId,count);
}
//获取歌曲列表
int GetSongInfoList(){
    return g_RB.GetSongInfoList();
}
//获取音视频服务器列表
int GetAvServerList( short net_type ){
    return g_RB.GetAvServerList(net_type);
}
//获取离线消息
int GetOffLineMsg(){
    return g_RB.GetOffLineMsg();
}
//获取分组置顶信息
int GetRoomSortInGroup( short group_idx ){
    return g_RB.GetRoomSortInGroup(group_idx);
}
//获取小喇叭信息
int GetUserSpeakerInfo(){
    return g_RB.GetUserSpeakerInfo();
}
// 获取家族信息
int GetFamilyInfo(){
    return g_RB.GetFamilyInfo();
}

//////////////////////////////////////////////////////////////////////////

//客户端房间打分
//const char *scoreinfo -- 打分信息结构体指针
//int length--打分信息结构体长度
int RoomScore(const char *scoreinfo,int length){
    return g_RB.RoomScore(scoreinfo);
}
//显示房间可领取收益
int ShowRoomIncome(){
    return g_RB.ShowRoomIncome();
}
//显示收益已领取记录
int ShowRoomGetIncome(){
    return g_RB.ShowRoomGetIncome();
}
//室主领取房间内收益
int GetRoomIncome(){
    return g_RB.GetRoomIncome();
}
//发送用户pc概况
int SendPcData( char * data, int len ){
    return g_RB.SendPcData(data);
}
//邀请用户加入房间会员
int InviteJoinRoom( int idx, int wseq ){
    return g_RB.InviteJoinRoom(idx, wseq);
}
//邀请用户加入房间会员
int ReInviteJoinRoom( char type ){
    return g_RB.ReInviteJoinRoom(type);
}
//取得当日礼物赠送记录
int GetGiftSend(){
    return g_RB.GetGiftSend();
}
//取得当日礼物接收记录
int GetGiftRecv(){
    return g_RB.GetGiftRecv();
}
//封mac
int KickUserByMac( int idx ){
    return g_RB.KickUserByMac(idx);
}
//封ip
int KickUserByIp( int idx ){
    return g_RB.KickUserByIp(idx);
}
//设置在麦剩余时间
int SetOnMicTemainTime( int idx, int time ){
    return g_RB.SetOnMicTemainTime(idx, time);
}
//设置在歌曲剩余时间
int SendSongRemnantsTime( short time ){
    return g_RB.SendSongRemnantsTime(time);
}

/* 私麦接口                                                             */
//上私麦, mictype为私麦的类型(vip以上可观看或需要对方确认可观看)
int OpenPrivateMic(short mictype){
    return g_RB.OpenPrivateMic(mictype);
}
//下私麦
int ClosePrivateMic(){
    return g_RB.ClosePrivateMic();
}
//被抱下私麦 //bidx=私麦持有者
int ClosedPrivateMic(int bidx){
    return g_RB.ClosedPrivateMic(bidx);
}

//请求观看idx的私麦 //idx=私麦持有者
int RequestWatchPrivateMic(int idx){
    return g_RB.RequestWatchPrivateMic(idx);
}
//回复观看私麦,只有同意才调用这个接口 //idx=观看者,0->同意，-1->拒绝
int ResponseWatchPrivateMic(int allow, int idx){
    return g_RB.ResponseWatchPrivateMic(allow, idx);
}
//邀请其他人看私麦
int InviteWatchPrivateMic(int num, int idxs[]){
    return g_RB.InviteWatchPrivateMic(num, idxs);
}
//回复邀请看私麦
int ResponseInviteWatchPrivateMic(int allow, int idx){
    return g_RB.ResponseInviteWatchPrivateMic(allow, idx);
}
//房间私麦管理 //0：不开放 1：开放
int OpenRoomPrivateMicFlag(char flag){
    return g_RB.OpenRoomPrivateMicFlag(flag);
}

// 皇冠踢人
int CrownKickoutSomeOne(int idx){
    return g_RB.CrownKickoutSomeOne(idx);
}
// 室主对用户的IP进行禁用
int ForbidUserIp(int idx){
    return g_RB.ForbidUserIp(idx);
}

// 发送句评分
int SingerScores( int idx, short scores[], int count ){
    return g_RB.SingerScores(idx, scores, count);
}
// 要求上传歌曲
int UpdateSong( int idx ){
    return g_RB.UpdateSong(idx);
}

/*
 获取发喇叭的用户信息
 */
int GetUserInfoForSpeaker(int idx[],int count){
    return g_RB.GetUserInfoForSpeaker(idx, count);
}
/*
 获取用户信息夹带版本ID
 */
int GetUserInfoWithVersion(RequestData_IdxWithVersion data[],int count){
    return g_RB.GetUserInfoWithVersion(data, count);
}
/*
 初始化下载URL地址(包含一些资源地址等等)具体参照原PC星光客户端
 */
int InitDownLoadUrl(const char* url,unsigned int forUse){
    g_DM.initDownloadUrl(url,forUse);
    return 1;
}
//初始化本地数据信息
int InitLoaclDatabaseDir(const char* path){
    return g_DM.initRootDir(path);
}
//保存人物部分信息
int SavePartInfo(ResultData_PartInfo *partInfo){
    g_DM.savePartInfo(partInfo);
    return 1;
}
//保存自己的部分信息
int SaveMyPartInfo(ResultData_PartInfo *partInfo){
    g_DM.saveMyPartInfo(partInfo);
    return 1;
}
//获取自己的信息
int GetMyInfo(USER_DATA_BASE *pMy){
    if(!pMy)
        return 0;
    USER_DATA_BASE* pTmp = g_DM.getMyInfo();
    if(!pTmp)
        return 0;
    memcpy(pMy, pTmp, sizeof(USER_DATA_BASE));
    return 1;
}
//获取到自己的数据
int GetSelfData(ResultData_SelfData *pSD){
    if(!pSD)
        return 0;
    ResultData_SelfData* pTmp = g_DM.getSelfData();
    if(!pTmp)
        return 0;
    memcpy(pSD,pTmp,sizeof(ResultData_SelfData));
    return 1;
}
//保存自己的数据
int SaveSelfData(ResultData_SelfData *pSD){
    if(!pSD)
        return 0;
    g_DM.saveSelfData(pSD);
    return 1;
}
//获取本人的ID
int GetMyIdx(){
    return g_DM.getMyIdx();
}
//设置本人的ID
int SaveMyIdx(int idx){
    g_DM.saveMyIdx(idx);
    return 1;
}
//保存用户信息
int SaveUserInfo(USER_DATA_BASE *userInfo){
    g_DM.saveUserInfo(*userInfo);
    return 1;
}
//提供给手机只是保存当前的在线用户的信息
int SaveUserInfoSample(int idx,const char* nickName)
{
    g_DM.saveUserInfo(idx, nickName);
    return 1;
}
//保存房间信息
int SaveRoomInfo(ROOM_DATA* roomInfo){
    g_DM.saveRoomInfo(*roomInfo);
    return 1;
}
//获取房间分组版本号
int GetServerGroupVersion(int lobbyId){
    if(!g_DM.m_pServerData)
        return 0;
    return g_DM.m_pServerData->getVersion(lobbyId);
}
//保存房间分组版本号
int SaveServerGroupVersion(int lobbyID,int groupVersionId){
    if(!g_DM.m_pServerData)
        return 0;
    g_DM.m_pServerData->setVersion(lobbyID, groupVersionId);
    return 1;
}
//清除本地所有房间缓存信息
int ClearRoom(){
    if(!g_DM.m_pServerData)
        return 0;
    g_DM.m_pServerData->clearRoom();
    return 1;
}
//清除本地所有分组缓存信息
int ClearGroup(){
    if(!g_DM.m_pServerData)
        return 0;
    g_DM.m_pServerData->clearGroup();
    return 1;
}
//增加一个分组信息
int AddGroupData(int groupId,const char* groupName,bool disable,int lobbyId){
    if(!g_DM.m_pServerData)
        return 0;
    g_DM.m_pServerData->addGroupData(groupId, groupName, disable, lobbyId);
    return 1;
}
//分组信息添加结束。
int AddGroupDataEnd(){
    if(!g_DM.m_pServerData)
        return 0;
    g_DM.m_pServerData->addEnd();
    return 1;
}
//增加一个房间信息
int AddRoomData(int roomId,int groupId,int lobbyId){
    if(!g_DM.m_pServerData)
        return 0;
    g_DM.m_pServerData->addRoomData(roomId, groupId, lobbyId);
    return 1;
}
//房间信息添加结束。
int AddRoomDataEnd(){
    return AddGroupDataEnd();
}
//字符集的转换 by glp 2014-9-19
int Utf8ToGbk(const char* utf8,char* gbk,int& size){
    if(!utf8 || !gbk || (size == 0))
        return 0;
    if(strlen(utf8) == 0)
        return 1;
    unsigned short* pUnicode = g_NetApp.m_pCharsetCodecUTF8->convertToUnicode(utf8);
    if(!pUnicode)
        return 0;
    char* pTmp = g_NetApp.m_pCharsetCodecGBK->convertFromUnicode(pUnicode);
    delete pUnicode;
    if(!pTmp)
        return 0;
    int len = strlen(pTmp);
    if(len>size)
        len = size-1;
    else
        size = len;
    memcpy(gbk,pTmp,len);
    gbk[len] = 0;
    
    return 1;
    
}
int GbkToUtf8(const char* gbk,char* utf8,int& size){
    if(!utf8 || !gbk || (size == 0))
        return 0;
    if(strlen(gbk) == 0)
        return 1;
    unsigned short* pUnicode = g_NetApp.m_pCharsetCodecGBK->convertToUnicode(gbk);
    if(!pUnicode)
        return 0;
    char* pTmp = g_NetApp.m_pCharsetCodecUTF8->convertFromUnicode(pUnicode);
    delete pUnicode;
    if(!pTmp)
        return 0;
    int len = strlen(pTmp);
    if(len>size)
        len = size-1;
    else
        size = len;
    memcpy(utf8,pTmp,len);
    utf8[len] = 0;
    
    return 1;
}
int UnicodeToGbk(const unsigned short* unicode,char* gbk,int& size){
    if(!unicode || !gbk || (size == 0))
        return 0;
    if(CCharsetCodec::caculateLength(unicode) == 0)
        return 1;
    char* pTmp = g_NetApp.m_pCharsetCodecGBK->convertFromUnicode(unicode);
    if(!pTmp)
        return 0;
    int len = strlen(pTmp);
    if(len>size)
        len = size-1;
    else
        size = len;
    memcpy(gbk,pTmp,len);
    gbk[len] = 0;
    delete pTmp;
    
    return 1;
}
int GbkToUnicode(const char* gbk,unsigned short* unicode,int& size){
    if(!unicode || !gbk || (size == 0))
        return 0;
    if(strlen(gbk) == 0)
        return 1;
    unsigned short* pTmp = g_NetApp.m_pCharsetCodecGBK->convertToUnicode(gbk);
    if(!pTmp)
        return 0;
    int len = CCharsetCodec::caculateLength(pTmp);
    if(len>size)
        len = size-1;
    else
        size = len;
    memcpy(unicode, pTmp, len*sizeof(unsigned short));
    unicode[len] = 0;
    delete pTmp;
    
    return 1;
}

/*
 判断是否是网通用户
 */
int IsWangtong(const char* host,const char* strWangtong){
    if(!host || !strWangtong)
        return 0;
    const char* pIp = NetworkUtil::ClientSocket::GetIpv4FromHostname(host);
    if (pIp[0] == 0)
        return 0;
    if(strstr(strWangtong, pIp))
       return 1;
    else
       return 0;
}
/*
 从本地数据库中获取到指定房间的房间信息
 */
int GetRoomInfoFromLocal(int roomId,ResultData_RoomInfo* roomInfo)
{
    if(!roomInfo)
        return 0;
    ROOM_DATA* pRoomData = g_DM.getRoomInfo(roomId);
    if(pRoomData)
    {
        memcpy(roomInfo, pRoomData, sizeof(ResultData_RoomInfo));
        return 1;
    }
    return 0;
}

/*
 从本地数据库中获取指定IDX的用户信息
 */
int GetUserInfoFromLocal(int idx,USER_DATA_BASE* userInfo)
{
    if (!userInfo)
        return 0;
    USER_DATA_BASE* pUserInfo = g_DM.getUserInfo(idx);
    if(pUserInfo)
    {
        memcpy(userInfo, pUserInfo, sizeof(USER_DATA_BASE));
        return 1;
    }
    return 0;
}
/*
 从本地数据库获取制定礼物信息
 */
int GetGiftInfoFromLocal(int giftId,GiftInfo* pGI)
{
    return g_DM.getGiftInfoForId(giftId, pGI)?1:0;
}
bool GetAllGiftId(int pId[],int& nNum)
{
    return g_DM.getAllGiftId(pId,nNum);
}
int AddHttpDownLoadTask(TaskDownload* taskInfo,bool priority)
{
    return g_NetApp.m_gMgr.addTask(*taskInfo,priority);
}

TiXmlDocument g_doc;
TiXmlElement* g_pElement = NULL;
/*
 解析给定的Xml buffer
 */
int ParaseXml(const char* buffer,bool utf8)
{
    if(g_doc.Parse(buffer,NULL,utf8?TIXML_ENCODING_UTF8:TIXML_ENCODING_LEGACY))
        return 1;
    return 0;
}
/*
 对上面传入的xml查找第一个子节点
 */
int FindFirstChildElementFromDoc(const char* name)
{
    if(name)
        g_pElement = g_doc.FirstChildElement(name);
    else
        g_pElement = g_doc.FirstChildElement();
    
    if(g_pElement)
        return 1;
    else
        return 0;
}
/*
 对当前查找到的element继续查找子element
 */
int FindFirstChildElementFromCur(const char* name)
{
    if(!g_pElement)
        return 0;
    if(!name)
        g_pElement = g_pElement->FirstChildElement();
    else
        g_pElement = g_pElement->FirstChildElement(name);
    
    if(g_pElement)
        return 1;
    else
        return 0;
}
/*
 获取到当前element的名字为name的值。对于有些值可以转int类型
 */
int GetElementAttribute(const char* name,char* buffer,int& size)
{
    if(!g_pElement || !name || !buffer || (size == 0))
        return 0;
    const char* p = g_pElement->Attribute(name);
    int len = strlen(p);
    if(len > size)
        len = size-1;
    else
        size = len;
    memcpy(buffer, p, len);
    buffer[len] = 0;
    
    return 1;
}

/*
 手机用户聊天接口
 */
int SendChatKTV(int toIdx,const char* content)
{
    return g_RB.SendChatKTV(toIdx, content);
}
/*
 手机用户送礼接口
 */
int SendGiftInLobby(int toIdx,int giftId,int number,int roomId)
{
    return g_RB.SendGiftInLobby(toIdx, giftId, number,roomId);
}
/*
 务必调用此方法，否则所有的函数都将返回false
 */
bool MFInitFilter()
{
    return CMessageFilter::getSingle()->initFilterEx();
}
/*
 过滤敏感字符串
 */
bool MFFilter( char * utf8, int size )
{
    return CMessageFilter::getSingle()->filter_first(utf8, size);
}
/*
 查找是否含有敏感字符串
 */
bool MFFindSensWord( const char* utf8 )
{
    return CMessageFilter::getSingle()->find_sens_word(utf8);
}
/*
 查找是否含有特殊符号
 */
bool MFFindSensWord2( const char* utf8 )
{
    return CMessageFilter::getSingle()->find_sens_word2(utf8);
}

