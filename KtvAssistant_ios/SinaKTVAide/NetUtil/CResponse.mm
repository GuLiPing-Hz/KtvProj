//
//  CResponse.cpp
//  testnetutil
//
//  Created by guliping_imac on 14-4-21.
//  Copyright (c) 2014年 天格科技. All rights reserved.
//

#include "xingguang.h"
#include "CResponse.h"
#include "NetUtilCallBackProtocol.h"
#import "com_ios_NetUtil.h"
#import "User.h"
#import "UserSessionManager.h"
#include "MessageInfoStruct.h"
#import "AutoLock.h"
#include "NetUtilCallBack.h"

#define KTVAssistantMSG @"ktvassistantSendMessageResult"

CResponse* CResponse::getResponseSingleton()
{
    static NSLock* obj = [[NSLock alloc] init];
    
    AutoLock lock(obj);
    static CResponse s_response;
    return &s_response;
}

CResponse::CResponse():m_bRequestUpdateGroupRoomlist(false),m_nGroupVersionID(0)
,m_nTempCur(0),m_nTempCount(0)
{
    m_callbackprotocol = [[NSMutableArray alloc] init];
    m_callbacklock = [[NSLock alloc] init];
}

CResponse::~CResponse()
{
    [m_callbackprotocol removeAllObjects];
}

void CResponse::initView(id<NetUtilCallBackProtocol> viewController)
{
    AutoLock lock(m_callbacklock);
    
    if(![m_callbackprotocol containsObject:viewController])
        [m_callbackprotocol insertObject:viewController atIndex:0];
}

void CResponse::addListenView(id<NetUtilCallBackProtocol> viewController)
{
    AutoLock lock(m_callbacklock);
    
    if(![m_callbackprotocol containsObject:viewController])
        [m_callbackprotocol addObject:viewController];
}
void CResponse::removeListenView(id<NetUtilCallBackProtocol> viewController)
{
    AutoLock lock(m_callbacklock);
    
    [m_callbackprotocol removeObject:viewController];
}
/************************************************************************/
/*			add by lixin 2011/10/13 <begin> 聊天、喇叭功能			*/
void CResponse::SendChatFromUser(int ret, int to_idx, const char *content ,int contentlen, int time ){}
/*			add by jinguanfu 2009/12/18 <begin> 聊天、喇叭功能			*/
//接收私聊
void CResponse::RecvChatFromUser(int ret, int from_idx, const char *content ,int contentlen, int time ){}
//接收公聊（俱乐部内部）
void CResponse::RecvChatFromClub(int ret, int from_idx, const char* content,int contentlen){}
//接收公聊（房间内）
//ret:1--正常 0--被禁言 -1--不允许公聊
void CResponse::RecvChatFromRoom(int ret, int errcode,int from_idx, const char* content,int contentlen, int time ){}
//接收大喇叭（全区）
void CResponse::RecvSpeakerFromAll(int ret, int type, int from_id, const char* msg, int msglen){}
//接收小喇叭（大厅）
//isSend==true  前5条喇叭
void CResponse::RecvSpeakerFromLobby(int ret, ResultData_UserSpeakerInfo* spkInfo,int type,bool isSend ){
    /*
     
    兼顾没有从本地获取到的用户信息GetUserInfoFromLocal-》GetUserInfoForSpeaker
     
     */
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        if([obj respondsToSelector:@selector(saveErrorcode:)])
            [obj saveErrorcode:ret];
    }
    
    {
        char utfroom[260] = {0};
        int len = sizeof(utfroom);
        GbkToUtf8(spkInfo->roomName, utfroom, len);
        
        AllInfo allInfo = {0};
        if (1 == type){
            allInfo.type = 5;
        }
        
        else if (2 == type){
            allInfo.type = 6;
        }
        else
        {
            allInfo.type = 4;
        }
        allInfo.idx = spkInfo->fromidx;
        allInfo.time = spkInfo->time;
        allInfo.roomid = spkInfo->roomId;
        strcpy(allInfo.roomname,utfroom);
        
        if (allInfo.type == 4) {
            strcpy(allInfo.content, spkInfo->chat);
        }
        else
        {
            char utfmsg[4096] = {0};
            len = sizeof(utfmsg);
            GbkToUtf8(spkInfo->chat, utfmsg, len);
            strcpy(allInfo.content, utfmsg);
        }
        
        USER_DATA_BASE userInfo;
        memset(&userInfo,0,sizeof(userInfo));
        if(1 == GetUserInfoFromLocal(spkInfo->fromidx , &userInfo))  //本地获取成功
        {
            char utfname[51] = {0};
            int len = sizeof(utfname);
            GbkToUtf8(userInfo.user_info.base.m_name, utfname, len);
            strcpy(allInfo.name, utfname);
            
            if (allInfo.type == 5) {
                char tmpbuf[1024] = {0};
                sprintf(tmpbuf, "%s%s",allInfo.name,allInfo.content);
                strcpy(allInfo.content, tmpbuf);
            }
            
            char head[256] = {0};
            NSString *domin = [UserSessionManager getInstance].currentUser.domin;
            char *p = strstr(userInfo.user_info.base.headurl, "http://");
            if(NULL == p)
            {
                strcpy(head, [domin UTF8String]);
                strncat(head, userInfo.user_info.base.headurl, strlen(userInfo.user_info.base.headurl));
                strcpy(allInfo.headphoto, head);
            }
            else
            {
                strcpy(allInfo.headphoto, userInfo.user_info.base.headurl);
            }
            if (isSend) {
                for(id obj in m_callbackprotocol)
                {
                    if([obj respondsToSelector:@selector(pushSpeakInfo:)])
                        [obj pushSpeakInfo:&allInfo];
                    //if([obj respondsToSelector:@selector(onChatUnReadAdd)])
                    //    [obj onChatUnReadAdd];
                }
            }
            else
            {
                for(id obj in m_callbackprotocol)
                {
                    if([obj respondsToSelector:@selector(pushBroadInfo:)])
                        [obj pushBroadInfo:&allInfo];
                    if([obj respondsToSelector:@selector(onChatUnReadAdd)])
                        [obj onChatUnReadAdd];
                }
            }
        }
        else
        {
            allInfo.isSend = isSend;
            for(id obj in m_callbackprotocol)
            {
                if(!obj)
                    continue;
                
                if([obj respondsToSelector:@selector(pushTmpSpeakInfo:)])
                    [obj pushTmpSpeakInfo:&allInfo];
            }
            
            GetUserInfoForSpeaker(&spkInfo->fromidx, 1);
        }
    }
}

//接收礼物广播
void CResponse::RecvPresentBroad(int ret, ResultData_GiftInfo* guestGiftInfo, ResultData_GiftInfo* commGiftInfo){}
//接受系统文字广播
void CResponse::RecvSystemBroad( char type, const char * msg, int len ){}
//接受系统文字广播（幸运中奖信息）
void CResponse::RecvSystemBroad2(const char* nickname, int len, int idx, int roomid, int giftid, int luckmultiple){
    
    AllInfo allInfo = {0};
    allInfo.type = 7;
    allInfo.idx = idx;
    
    char utfname[260] = {0};
    len = sizeof(utfname);
    GbkToUtf8(nickname, utfname, len);
    
    strcpy(allInfo.name, utfname);
    allInfo.roomid = roomid;
    allInfo.time = 0;
    allInfo.gift_id = giftid;
    allInfo.toidx = 0;
    strcpy(allInfo.toname, "0");
    allInfo.recv_roomid = 0;
    strcpy(allInfo.recv_room, "0");
    allInfo.total_number = 0;
    strcpy(allInfo.unit_name, "0");

    allInfo.luckmultiple = luckmultiple;
    
    USER_DATA_BASE userInfo;
    memset(&userInfo, 0, sizeof(userInfo));
    if(GetUserInfoFromLocal(idx, &userInfo))
    {
        char head[256] = {0};
        NSString *domin = [UserSessionManager getInstance].currentUser.domin;
        char *p = strstr(userInfo.user_info.base.headurl, "http://");
        if(NULL == p)
        {
            strcpy(head, [domin UTF8String]);
            strncat(head, userInfo.user_info.base.headurl, strlen(userInfo.user_info.base.headurl));
            strcpy(allInfo.headphoto, head);
        }
        else
        {
            strcpy(allInfo.headphoto, userInfo.user_info.base.headurl);
        }
    }
    
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(pushBroadInfo:)])
            [obj pushBroadInfo:&allInfo];
        if([obj respondsToSelector:@selector(onChatUnReadAdd)])
            [obj onChatUnReadAdd];
    }
}
//接受系统文字公告
void CResponse::RecvSystemNotice( const char * msg, int len, bool once, int min ){}
//接受系统文字公告（幸运中奖信息）
void CResponse::RecvSystemNotice2(const char* nickname, int len, int idx, int roomid, int giftid, int luckmultiple){}
//接收房间临时公告
void CResponse::RecvRoomNotice(int ret, const char* content,int contentlen){}
/*			add by jinguanfu 2009/12/28 <begin> 好友操作				*/
//取得好友位置
void CResponse::RecvFriendLocation(int ret, int errcode,
                                int friend_idx,
                                int lobbyid,
                                int roomid,
                                int cseq){}
/*			add by jinguanfu 2009/12/28 <begin> 房间管理接口			*/
//接收取得待审核房间会员列表
void CResponse::RecvRoomApplyList (int ret ,int errcode,ResultDate_RoomApplyList *applylist,int cseq){}
//接收申请房间会员响应
void CResponse::RecvApplyRoomMember(int ret,int errcode,int cseq){}
//接收审核会员加入申请结果
void CResponse::RecvAuditAddApply (int ret, int errcode,int idx,int cseq){}
//接收拒绝房间申请系统提示
void CResponse::RecvRoomApplyRefuseMsg (int ret, int roomid){}
//接收房间会员列表
void CResponse::RecvRoomMemberList (int ret ,int errcode, ResultDate_RoomMemList* memlist,int cseq){}
//接收解除房间会员资格回应
void CResponse::RecvUnchainMember (int ret, int errcode, int idx,int cseq){}
//接收赋予用户房间主持助理权限回应
void CResponse::RecvRoomPesideAssistant (int ret , int errcode, int idx,int cseq){}
//接收赋予主持助理通知
void CResponse::RecvRoomNotice4PesideAssistant (int ret , int opidx,int bopidx){}
//接收赋予用户房间主持人权限回应
void CResponse::RecvRoomPeside (int ret ,int errcode, int idx,int cseq){}
//接收设置主持人权限房间通知
void CResponse::RecvRoomNotice4Peside (int ret , int opidx,int bopidx){}
//接收赋予用户房间副室主权限回应
void CResponse::RecvRoomSubHost (int ret ,int errcode, int idx,int cseq){}
//接收设置副室主权限房间通知
void CResponse::RecvRoomNotice4SubHost(int ret , int opidx,int bopidx){}
//接收房间黑名单
void CResponse::RecvBlackList(int ret,	int errcode,ResultDate_RoomBlackList* blacklist,int cseq){}
//接收黑名单用户添加/删除回应
void CResponse::RecvAddDelBlack(int ret , int errcode,int idx, int type){}
//接收房间设置面板信息
void CResponse::RecvRoomPanelInfo(int ret ,int errcode, ResultDate_RoomPanel* panelinfo){}
//接收设置房间密码回应
void CResponse::RecvRoomPassword(int ret,int errcode,int cseq){}
//接收设置房间密码消息提示
void CResponse::RecvRoomNotice4Password(int ret, int idx, char pwdflag){}
//接收设置房间开启/关闭(上锁)回应
void CResponse::RecvRoomSwitch(int ret,int errcode,char roomflag,int cseq){}
//接收房间开启/关闭(上锁)消息提示
void CResponse::RecvRoomNotice4RoomSwitch(int ret , int idx , char roomflag){}
//接收设置房间访问权限回应
void CResponse::RecvRoomPrivate(int ret , int errcode,char accessflag,int cseq){}
//接收房间访问权限消息提示
void CResponse::RecvRoomNotice4Private(int ret , int idx , char accessflag){}
//接收设置用户进出信息开启/关闭回应
void CResponse::RecvUserInOutSwitch(int ret , int errcode, char inoutflag,int cseq){}
//接收用户进出信息开启/关闭消息提示
void CResponse::RecvRoomNotice4UserSwitch (int ret , int idx , char inoutflag){}
//接收设置自由排麦开启/关闭回应
void CResponse::RecvGetMicSwitch (int ret, int errcode,char onmicflag,int cseq){}
//接收设置自由上麦开启/关闭回应
void CResponse::RecvOnMicSwitch (int ret, int errcode,char onmicflag,int cseq){}
//接收自由排麦开启/关闭消息提示
void CResponse::RecvRoomNotice4GetMicSwitch(int ret , int idx , char getmicflag){}
//接收自由上麦开启/关闭消息提示
void CResponse::RecvRoomNotice4OnMicSwitch(int ret , int idx , char onmicflag){}
//房间名称修改回应
void CResponse::RecvModifyRoomName(int ret,int errcode,int cseq){}
//接收房间名
void CResponse::RecvRoomName (int ret , int roomid , const char* roomname , int rnlen){}
//设置房间公告响应
void CResponse::RecvRoomAffiche(int ret,int errcode,int cseq){}
//接收房间公告更新事件
void CResponse::RecvRoomAfficheEvent(int ret){}
//设置允许房间公聊响应
void CResponse::RecvAllowRoomAllChat (int ret, int errcode,char chatflag,int cseq){}
//接收房间公聊设置消息提示
void CResponse::RecvRoomNotice4AllChat(int ret , int idx , char chatflag){}
//设置房间欢迎词响应
void CResponse::RecvRoomSalutatory(int ret, int errcode,int cseq){}
//接收房间欢迎词更新通知
void CResponse::RecvRoomNotice4Salutatory(int ret,int idx){}
//设置房间logo响应
void CResponse::RecvRoomLogo (int ret,int errcode ,int cseq){}
//接收房间LOGO更新通知
void CResponse::RecvRoomNotice4Logo(int ret,int idx){}
//接收房间冻结消息提示
void CResponse::RecvRoomNotice4Lock(int idx){}
//接受创建房间回应
void CResponse::RecvCreateRoom( int ret, int errcode, RoomListEx * info ){}
//接受练歌房完全信息
void CResponse::RecvTmpRoomInfo( int ret, int errcode, RoomListEx * info ){}
// 添加歌房广播
void CResponse::RecvRoomNotice4Add( RoomListEx * info ){}
void CResponse::RecvRoomNotice4Add( int idx ){}
// 删除歌房广播
void CResponse::RecvRoomNotice4Del( int idx ){}
// 接收房间易主通知
void CResponse::RecvRoomNotice4ChangeMaster(int idx){}
// 接收房间信息更改通知
void CResponse::RecvRoomNotice4ChangeInfo( int idx ){}
/*			add by jinguanfu 2009/12/28 <begin> 角色信息				*/
//取得他人角色信息
void CResponse::RecvPartInfo(int ret, int errcode, ResultData_PartInfo * part_info, int cseq){}
//取得自己角色信息
void CResponse::RecvPartInfoBySelf(int ret, int errcode, ResultData_PartInfo * part_info, int cseq)
{
    //登陆大厅最后一步，本来是还要获取背包信息，去掉
    KtvUser gUser;
    memset(&gUser,0,sizeof(gUser));
    USER_DATA_BASE my;
    memset(&my,0,sizeof(my));
    ResultData_SelfData sd = {0};
    if((GetMyInfo(&my) != 0) && (GetSelfData(&sd) != 0))
    {
        gUser.fullInfo.ret = my.user_info.base.ret;
        gUser.fullInfo.m_idx = my.user_info.base.m_idx;
        strcpy(gUser.fullInfo.m_id, my.user_info.base.m_id);
        int len = sizeof(gUser.fullInfo.m_name);
        GbkToUtf8(my.user_info.base.m_name,gUser.fullInfo.m_name,len);//GBK to UTF8
        len = sizeof(gUser.fullInfo.m_province);
        GbkToUtf8(my.user_info.base.m_province,gUser.fullInfo.m_province,len);
        //printf("my province is %s ; before is %s\n",gUser.fullInfo.m_province,my.user_info.base.m_province);
        len = sizeof(gUser.fullInfo.m_city);
        GbkToUtf8(my.user_info.base.m_city,gUser.fullInfo.m_city,len);
        strcpy(gUser.fullInfo.m_sex, my.user_info.base.m_sex);
        strcpy(gUser.fullInfo.m_age, my.user_info.base.m_age);
        gUser.fullInfo.m_level = my.user_info.base.m_level;
        strcpy(gUser.fullInfo.m_LevelTime, my.user_info.base.m_LevelTime);
        gUser.fullInfo.m_isVip = my.user_info.base.m_isVip;
        strcpy(gUser.fullInfo.m_stage,my.user_info.base.m_stage);
        len = sizeof(gUser.fullInfo.m_stageName);
        GbkToUtf8(my.user_info.base.m_stageName,gUser.fullInfo.m_stageName,len);
        strcpy(gUser.fullInfo.m_ClubId, my.user_info.base.m_ClubId);
        strcpy(gUser.fullInfo.m_ClubProfession, my.user_info.base.m_ClubProfession);
        len = sizeof(gUser.fullInfo.m_ClubName);
        GbkToUtf8(my.user_info.base.m_ClubName,gUser.fullInfo.m_ClubName,len);
        gUser.fullInfo.m_ExpLevel = my.user_info.base.m_ExpLevel;
        
        //avarata deprecated
        //gUser.fullInfo.m_AvatarId = my.user_info.avarta.avaratid;
        //gUser.fullInfo.m_AvaratCount = my.user_info.avarta.len;
        //gUser.fullInfo.m_AvaratData = new short[fullInfo.m_AvaratCount];
        //memcpy(gUser.fullInfo.m_AvaratData, my.user_info.avarta.goods_id, gUser.fullInfo.m_AvaratCount*sizeof(short));
        
        gUser.fullInfo.m_singerlevel = my.user_info.base.m_singerlevel;
        gUser.fullInfo.m_singer_expvalue = my.user_info.base.m_singer_expvalue;
        gUser.fullInfo.m_wealthlevel = my.user_info.base.m_wealthlevel;
        gUser.fullInfo.m_wealth_expvalue = my.user_info.base.m_wealth_expvalue;
        
        len = sizeof(gUser.fullInfo.headurl);
        GbkToUtf8(my.user_info.base.headurl,gUser.fullInfo.headurl,len);
        len = sizeof(gUser.fullInfo.topsongname);
        GbkToUtf8(my.user_info.base.topsongname,gUser.fullInfo.topsongname,len);
        
        gUser.fullInfo.topsongscore = my.user_info.base.topsongscore;
        gUser.fullInfo.recvcount = my.user_info.base.recvcount;
        gUser.fullInfo.songcount = my.user_info.base.songcount;
        
        gUser.is_adult = sd.is_adult;
        gUser.is_remind = sd.m_remind==1;
        gUser.consume = sd.m_consume;
        gUser.family_idx = 0;//?家族ID
        gUser.peri = false;//是否美女用户
        gUser.gold = sd.gold;
        
        len = sizeof(gUser.fullInfo.songname);
        GbkToUtf8(my.user_info.base.songname, gUser.fullInfo.songname, len);
        len = sizeof(gUser.fullInfo.singername);
        GbkToUtf8(my.user_info.base.singername, gUser.fullInfo.singername, len);
        gUser.fullInfo.score = my.user_info.base.score;
    }
    
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        if([obj respondsToSelector:@selector(saveErrorcode:)])
            [obj saveErrorcode:errcode];
        if([obj respondsToSelector:@selector(saveUserInfo:)])
            [obj saveUserInfo:&gUser];
        if([obj respondsToSelector:@selector(OnLoginLobby)])
            [obj performSelectorOnMainThread:@selector(OnLoginLobby) withObject:nil waitUntilDone:NO];
    }
}
/*			add by jinguanfu 2010/1/5 <begin> 取得房间列表				*/
void CResponse::RecvRoomList(int ret,ResultDate_RoomList* roomlist){}
void CResponse::RecvRoomListEx(int ret,ResultDate_RoomListEx * roomlist){}
//打分通知
//void CResponse::RecvNotice4Score(int onmic_idx,int onmic_score,int pk_idx,int pk_score){}
//void CResponse::RecvNotice4ScoreAdd(int epchange, int silverchange){}
//下麦通知
void CResponse::RecvNotice4OffMic( int onmic_idx, int pk_idx, int exp, const char * scoreinfo, int len ){}
// add by cx 10-6-2
//回应是否邀请上麦成功
void CResponse::RecvRoomGivOnevjMic( int idx, char flag ){}
//是否同意邀请上麦成功
void CResponse::RecvRoomGivOnevjMicApply( int formidx ){}
//add by jinguanfu 2010/1/20 <begin>
//是否同意添加对方为好友
//参数说明：int idx 添加好友者idx
void CResponse::RecvAddFriendApply(int idx){}
//add by jinguanfu 2010/1/20 <end>
//void CResponse::RecvDeleteAvarta(int ret){}
//add by jinguanfu 2010/3/4 原有回调API改成接口<begin>
//连接成功
//成功连接认证服务器
void CResponse::OnAuthTunnelConnectSuccess(){
    //直接访问会导致内存泄露，因为没有为线程创建线程池，而且这样做界面不会相应，跟界面线程是两个线程
    //所以采取更加友好的方式，把这个任务推入主线程的队列中，让它在主线程中执行，安全方便
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(obj && [obj respondsToSelector:@selector(OnAuthTunnelConnectSuccess)])
        {
            [obj performSelectorOnMainThread:@selector(OnAuthTunnelConnectSuccess) withObject:nil waitUntilDone:NO];
        }
    }
    
}
//成功连接大厅服务器
void CResponse::OnLobbyTunnelConnectSuccess(){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(obj && [obj respondsToSelector:@selector(OnLobbyTunnelConnectSuccess)])
        {
            [obj performSelectorOnMainThread:@selector(OnLobbyTunnelConnectSuccess) withObject:nil waitUntilDone:NO];
        }
    }
}
//成功连接房间服务器
void CResponse::OnRoomTunnelConnectSuccess(){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(obj && [obj respondsToSelector:@selector(OnRoomTunnelConnectSuccess)])
        {
            [obj performSelectorOnMainThread:@selector(OnRoomTunnelConnectSuccess) withObject:nil waitUntilDone:NO];
        }
    }
}
//成功连接频道服务器
void CResponse::OnChannellistTunnelConnectSuccess(){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(obj && [obj respondsToSelector:@selector(OnChannellistTunnelConnectSuccess)])
        {
            [obj performSelectorOnMainThread:@selector(OnChannellistTunnelConnectSuccess) withObject:nil waitUntilDone:NO];
        }
    }
}
//连接超时
//连接认证服务器超时
void CResponse::OnAuthTunnelConnectTimeout(){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(obj && [obj respondsToSelector:@selector(OnAuthTunnelConnectTimeout)])
        {
            [obj performSelectorOnMainThread:@selector(OnAuthTunnelConnectTimeout) withObject:nil waitUntilDone:NO];
        }
    }
}
//连接大厅服务器超时
void CResponse::OnLobbyTunnelConnectTimeout(){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(obj && [obj respondsToSelector:@selector(OnLobbyTunnelConnectTimeout)])
        {
            [obj performSelectorOnMainThread:@selector(OnLobbyTunnelConnectTimeout) withObject:nil waitUntilDone:NO];
        }
    }
}
//连接房间服务器超时
void CResponse::OnRoomTunnelConnectTimeout(){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(obj && [obj respondsToSelector:@selector(OnRoomTunnelConnectTimeout)])
        {
            [obj performSelectorOnMainThread:@selector(OnRoomTunnelConnectTimeout) withObject:nil waitUntilDone:NO];
        }
    }
}
//连接频道服务器超时
void CResponse::OnChannellistTunnelConnectTimeout(){}
//连接错误
void CResponse::OnAuthTunnelConnectError( int code ){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        //对数据的传递使用直接存放的方式，切记必须先更新完数据，再存储操作，推入主线程队列
        if([obj respondsToSelector:@selector(saveErrorcode:)])
        {
            [obj saveErrorcode:code];
        }
        
        if([obj respondsToSelector:@selector(OnAuthTunnelConnectError)])
        {
            [obj performSelectorOnMainThread:@selector(OnAuthTunnelConnectError) withObject:nil waitUntilDone:NO];
        }
    }
}
void CResponse::OnLobbyTunnelConnectError( int code ){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        //对数据的传递使用直接存放的方式，切记必须先更新完数据，再存储操作，推入主线程队列
        if([obj respondsToSelector:@selector(saveErrorcode:)])
        {
            [obj saveErrorcode:code];
        }
        
        if([obj respondsToSelector:@selector(OnLobbyTunnelConnectError)])
        {
            [obj performSelectorOnMainThread:@selector(OnLobbyTunnelConnectError) withObject:nil waitUntilDone:NO];
        }
    }
}
void CResponse::OnRoomTunnelConnectError( int code ){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        //对数据的传递使用直接存放的方式，切记必须先更新完数据，再存储操作，推入主线程队列
        if([obj respondsToSelector:@selector(saveErrorcode:)])
        {
            [obj saveErrorcode:code];
        }
        
        if([obj respondsToSelector:@selector(OnRoomTunnelConnectError)])
        {
            [obj performSelectorOnMainThread:@selector(OnRoomTunnelConnectError) withObject:nil waitUntilDone:NO];
        }
    }
}
void CResponse::OnChannellistTunnelConnectError( int code ){}
//服务器主动断开的连接,客户端recv == 0的时候,回调到以下的接口
//断开认证服务器
void CResponse::OnAuthTunnelClose(){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(obj && [obj respondsToSelector:@selector(OnAuthTunnelClose)])
        {
            [obj performSelectorOnMainThread:@selector(OnAuthTunnelClose) withObject:nil waitUntilDone:NO];
        }
    }
}
//断开大厅服务器
void CResponse::OnLobbyTunnelClose(){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(obj && [obj respondsToSelector:@selector(OnLobbyTunnelClose)])
        {
            [obj performSelectorOnMainThread:@selector(OnLobbyTunnelClose) withObject:nil waitUntilDone:NO];
        }
    }
}
//断开房间服务器
void CResponse::OnRoomTunnelClose(){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(obj && [obj respondsToSelector:@selector(OnRoomTunnelClose)])
        {
            [obj performSelectorOnMainThread:@selector(OnRoomTunnelClose) withObject:nil waitUntilDone:NO];
        }
    }
}
//断开频道服务器
void CResponse::OnChannellistTunnelClose(){}
//客户端recv异常,send异常,网络层buf溢出,select出现问题,都会回调到这个以下接口
void CResponse::OnAuthTunnelError( int code ){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        //对数据的传递使用直接存放的方式，切记必须先更新完数据，再存储操作，推入主线程队列
        if([obj respondsToSelector:@selector(saveErrorcode:)])
        {
            [obj saveErrorcode:code];
        }
        
        if([obj respondsToSelector:@selector(OnAuthTunnelError)])
        {
            [obj performSelectorOnMainThread:@selector(OnAuthTunnelError) withObject:nil waitUntilDone:NO];
        }
    }
}
void CResponse::OnLobbyTunnelError( int code ){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        //对数据的传递使用直接存放的方式，切记必须先更新完数据，再存储操作，推入主线程队列
        if([obj respondsToSelector:@selector(saveErrorcode:)])
        {
            [obj saveErrorcode:code];
        }
        
        if([obj respondsToSelector:@selector(OnLobbyTunnelError)])
        {
            [obj performSelectorOnMainThread:@selector(OnLobbyTunnelError) withObject:nil waitUntilDone:NO];
        }
    }
}
void CResponse::OnRoomTunnelError( int code ){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        //对数据的传递使用直接存放的方式，切记必须先更新完数据，再存储操作，推入主线程队列
        if([obj respondsToSelector:@selector(saveErrorcode:)])
        {
            [obj saveErrorcode:code];
        }
        
        if([obj respondsToSelector:@selector(OnRoomTunnelError)])
        {
            [obj performSelectorOnMainThread:@selector(OnRoomTunnelError) withObject:nil waitUntilDone:NO];
        }
    }
}
void CResponse::OnChannellistTunnelError( int code ){}

/******************认证服务器操作接口*****************************/
//接收认证结果
void CResponse::RecvAuth(int ret, int errcode, int again){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        //对数据的传递使用直接存放的方式，切记必须先更新完数据，再存储操作，推入主线程队列
        if([obj respondsToSelector:@selector(saveErrorcode:)])
        {
            [obj saveErrorcode:errcode];
        }
        
        if([obj respondsToSelector:@selector(RecvAuth)])
        {
            [obj performSelectorOnMainThread:@selector(RecvAuth) withObject:nil waitUntilDone:NO];
        }
    }
}
//接收新token
//void CResponse::RecvNewToken(int ret,ReserveDataS* pRD){}
void CResponse::RecvNewToken(int ret,int errcode){}
//接收频道列表
void CResponse::RecvSectionList(int ret,int errcode,ResultData_GetChannellist* pRDc,int cseq){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        //对数据的传递使用直接存放的方式，切记必须先更新完数据，再存储操作，推入主线程队列
        if([obj respondsToSelector:@selector(saveErrorcode:)])
        {
            [obj saveErrorcode:errcode];
            if(ret == 0 && [obj respondsToSelector:@selector(saveChannellist:)])
                [obj saveChannellist: pRDc];
        }
        
        if([obj respondsToSelector:@selector(RecvSectionList)])
        {
            [obj performSelectorOnMainThread:@selector(RecvSectionList) withObject:nil waitUntilDone:NO];
        }
    }
}
//接收频道列表(添加防沉迷信息)
void CResponse::RecvSectionList2(int ret,int errcode,int leftlogintime,ResultData_GetChannellist* pRDc,int cseq){}
/******************大厅服务器操作接口*****************************/
//接收大厅token认证
void CResponse::RecvTokenAuth4Lobby( int ret, int errcode, ResultData_LobbyLogin * pRDl, ResultData_SelfData * pSD, int cseq ){
    
    {
        AutoLock lock(m_callbacklock);
        for(id obj in m_callbackprotocol)
        {
            if(!obj)
                continue;
            
            //对数据的传递使用直接存放的方式，切记必须先更新完数据，再存储操作，推入主线程队列
            if([obj respondsToSelector:@selector(saveErrorcode:)])
            {
                [obj saveErrorcode:errcode];
            }
            
            if([obj respondsToSelector:@selector(RecvTokenAuth4Lobby)])
            {
                [obj performSelectorOnMainThread:@selector(RecvTokenAuth4Lobby) withObject:nil waitUntilDone:NO];
            }
        }
    }

    //大厅认证成功的后续动作
    if(ret == 0)
    {
        //check the group info if need to update
		m_nGroupVersionID = pRDl->group_versionid;
        if(GetServerGroupVersion(XG_LOBBY_ID) < m_nGroupVersionID)
        {
            if(m_bRequestUpdateGroupRoomlist)
                return ;
            ClearRoom();
            ClearGroup();
            
            if(0 != GetGroupInfo())
				PLog(@"%s : GetGroupInfo failed",__FUNCTION__);
        }
        else//don't update the group and use the local data
		{
			if(0 != GetPartInfo(GetMyIdx(),GetMyIdx()))
				PLog(@"%s : GetPartInfo failed",__FUNCTION__);
		}
    }
}
//用户登录大厅通知
void CResponse::RecvUserLoginLobbyNotice(int ret,ResultData_LobbyLoginUser* pRlu){
}
//用户退出大厅通知
void CResponse::RecvUserLogoutLobbyNotice(int ret,ResultData_LobbyLoginUser* pRlu){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(obj && [obj respondsToSelector:@selector(deleteOnlineInfo:)])
            [obj deleteOnlineInfo:pRlu->idx];
    }
}
//接收用户信息
void CResponse::RecvUserInfo(int ret,int errcode,ResultData_UserFullInfo* pRDu)
{
    OnlineInfo online = {0};
    online.idx = pRDu->m_idx;
    
    char utfname[260] = {0};
    int len = sizeof(utfname);
    GbkToUtf8(pRDu->m_name, utfname, len);
    strcpy(online.name, utfname);
    strcpy(online.sex, pRDu->m_sex);
    char head[256] = {0};
    NSString *domin = [UserSessionManager getInstance].currentUser.domin;
    char *p = strstr(pRDu->headurl, "http://");
    if(NULL == p)
    {
        strcpy(head, [domin UTF8String]);
        strncat(head, pRDu->headurl, strlen(pRDu->headurl));
        strcpy(online.headphoto, head);
    }
    else
        strcpy(online.headphoto, pRDu->headurl);
    
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(obj && [obj respondsToSelector:@selector(pushOnlineInfo:)])
            [obj pushOnlineInfo:&online];
    }
}
//接收好友状态
void CResponse::RecvFriendStatus(int ret,int errcode,ResultData_GetFriendlist* pRD){}
//接收黑名单状态
void CResponse::RecvBlackStatus(int ret,int errcode,ResultData_GetFriendlist* pRD){}
//接收俱乐部状态
void CResponse::RecvClubStatus(int ret,int errcode,ResultData_GetClublist* pRD){}
//接收添加好友结果
void CResponse::RecvAddFriend(int ret,int errcode,int idx){}
//接收添加黑名单结果
void CResponse::RecvAddBlack(int ret,int errcode,int cseq){}
//接收删除好友结果
void CResponse::RecvDelFriend(int ret,int errcode,int cseq){}
//接收删除黑名单结果
void CResponse::RecvDelBlack(int ret,int errcode,int cseq){}
//接收申请加入俱乐部结果
void CResponse::RecvApplyClub(int ret,int errcode,int cseq){}
//接收退出俱乐部结果
void CResponse::RecvExitClub(int ret,int errcode,int cseq){}
//接收最近几天内的加入俱乐部申请数量
void CResponse::RecvApplyListCount(int ret,int errcode,int count,int cseq){}
//接收最近几天内的加入俱乐部申请列表
void CResponse::RecvApplyList(int ret,int errcode,ResultData_ClubApplyList* pRD,int cseq){}
//通过加入俱乐部审批
void CResponse::RecvAddClubOk(int ret,ResultData_Approvelresult* pRD){}
//删除申请列表
void CResponse::RecvDelApplyList(int ret,ResultData_Approvelresult* pRD){}
//接收加入俱乐部广播消息(当有人加入俱乐部)
//modify by jinguanfu 2010/4/11
//void CResponse::RecvAddClubNotice(int ret ,int idx){}
//opidx--操作者idx
//addidx--加入者idx
void CResponse::RecvAddClubNoticeByLobby(int ret ,int clubid,const char* clubname,int opidx,int addidx){}
void CResponse::RecvAddClubNoticeByRoom( int opidx, int addidx ){}
//接收申请会员拒绝消息
//opidx--审核者idx
void CResponse::RecvDenyClubNoticeByLobby(int ret ,int clubid,const char* clubname,int opidx){}
//void CResponse::RecvDenyClubNoticeByRoom(int ret ,int clubid,char* clubname,int opidx){}
//接收会员退出俱乐部消息
void CResponse::RecvExitClubNoticeByLobby(int ret,int clubid,const char* clubname,int exitidx){}
void CResponse::RecvExitClubNoticeByRoom( int exitidx ){}
//接收会员被踢出俱乐部消息
//opidx--操作者idx
//kickidx--被踢出者idx
void CResponse::RecvKickClubNoticeByLobby(int ret,int clubid,const char* clubname,int opidx,int kickidx){}
void CResponse::RecvKickClubNoticeByRoom( int opidx, int kickidx ){}
//创建俱乐部
void CResponse::RecvCreateClub(int ret,int errcode,int clubid,int cseq){}
//解散俱乐部
void CResponse::RecvDismissClub(int ret,int errcode,int cseq){}
//接收俱乐部信息
void CResponse::RecvClubInfo(int ret,int errcode,ResultData_ClubFullInfo* pRD,int cseq){}
////接收房间列表
//void CResponse::RecvRoomList(int ret,ResultData_RoomList* pRD){}
//接收房间信息
void CResponse::RecvRoomInfo(int ret,ResultData_RoomInfo* pRD){}
//创建头像
void CResponse::RecvCreateAvatar(int ret,int errcode){}
//修改头像
void CResponse::RecvChangeAvatar(int ret,int errcode){}
//修改昵称
void CResponse::RecvChangeNickName(int ret,int errcode,const char * name, int len){}
//进入房间通知
void CResponse::RecvLobbyEnterRoom(int ret,ResultData_LobbyRoomEnter* pRD){}
//离开房间通知
void CResponse::RecvLobbyLeaveRoom(int ret,ResultData_LobbyRoomEnter* pRD){}
//添加好友通知
void CResponse::RecvAddFriendNotice(int ret,int idx){}
//取得大厅信息
void CResponse::RecvLobbyInfo(int ret,int errcode,ResultData_LobbyName* pRD,int cseq){}
//接收来自大厅的权限更改消息 add by jinguanfu 2010/6/5
void CResponse::RecvChangePower(int optionidx/*操作者ID*/,char identity/*更改后权限*/){}
//取大厅名(CMD_SYSTEMMSG_L2C)
//void CResponse::RecvLobbyName(){}
/******************房间操作接口*****************************/
//接收房间token验证 //
void CResponse::RecvTokenAuth4Room(int ret,int errcode){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        if([obj respondsToSelector:@selector(saveRet:)])
            [obj saveRet:ret];
        
        if([obj respondsToSelector:@selector(saveErrorcode:)])
            [obj saveErrorcode:errcode];
        
        if([obj respondsToSelector:@selector(RecvTokenAuth4Room)])
            [obj performSelectorOnMainThread:@selector(RecvTokenAuth4Room) withObject:nil waitUntilDone:false];
    }
}
//进入房间
void CResponse::RecvEnterRoom(int ret,int errcode,ResultData_LoginInfo* pRDlf,int cseq){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        if([obj respondsToSelector:@selector(saveErrorcode:)])
            [obj saveErrorcode:errcode];
        
        if([obj respondsToSelector:@selector(saveCSeq:)])
            [obj saveCSeq:cseq];
        
        if([obj respondsToSelector:@selector(RecvEnterRoom)])
            [obj performSelectorOnMainThread:@selector(RecvEnterRoom) withObject:nil waitUntilDone:false];
    }
}
//用户进入房间通知
void CResponse::RecvEnterRoomNotice(int ret,ResultData_UserInfo* pRDu){}
//用户离开房间通知
void CResponse::RecvLeaveRoomNotice(int ret,int idx)
{
    if (idx != [UserSessionManager getInstance].currentUser.uid) {
        return;
    }
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        if([obj respondsToSelector:@selector(RecvLeaveRoom)])
            [obj performSelectorOnMainThread:@selector(RecvLeaveRoom) withObject:nil waitUntilDone:false];
    }

}
//接收房间详细信息
void CResponse::RecvRoomAllInfo(int ret,int errcode,ResultData_RoomAllInfo *pRD,int cseq){}
//接收排麦成功通知
void CResponse::RecvWaitMicNotice(int ret,ResultData_MicInfo *pRD){}
////接收上麦准备通知
//void CResponse::RecvWaitMicReadyNotice(int ret,ResultData_MicInfo *pRD){}
//接收上麦准备完成通知
//void CResponse::RecvOnMicReadyOkNotice(int ret,ResultData_MicInfo *pRD){}
//接收上麦通知
void CResponse::RecvOnMicNotice(int ret,ResultData_MicInfo *pRD){}
//取消排麦成功通知
void CResponse::RecvCancelWaitMicNotice(int ret,int opidx,int bopidx){}
//接收加入PK麦通知
void CResponse::RecvWaitPKMicNotice(int ret,ResultData_MicInfo *pRD){}
//接收取消PK麦通知
void CResponse::RecvCancelWaitPKMicNotice(int ret,int opidx,int bopidx){}
//接收升麦成功通知
void CResponse::RecvUpWaitMicNotice(int ret,ResultData_UDMicInfo *pRD){}
//接收降麦成功通知
void CResponse::RecvDownWaitMicNotice(int ret,ResultData_UDMicInfo *pRD){}
//接收踢出房间通知
void CResponse::RecvKickOutNotice(int idx,int bidx){}
//接收上VJ麦（主持麦）通知
void CResponse::RecvOnVJMicNotice(int ret,int idx){}
//接收下VJ麦（主持麦）通知
void CResponse::RecvOffVJMicNotice(int ret,int idx){}
//接收被下麦通知
void CResponse::RecvGiveOffMicNotice(int ret,int idx){}
//接收被禁言通知
void CResponse::RecvForbidenNotice(int ret,ResultData_ForbidenInfo* pRD){}
//接收黑名单更新通知
void CResponse::RecvUpdateBlackListNotice(int ret,ResultData_UpBlackList* pRD){}
//接收管理员更新通知
void CResponse::RecvUpdateMGRListNotice(int ret,ResultData_UpMgrList* pRD){}
//接收赠送礼物通知
void CResponse::RecvSendFlowerNotice(int ret,ResultData_SendFlowerInfo* pRD){
    //处理广播礼物消息
    RenderGiftInfo rgi = {0};
    rgi.send_idx = pRD->s_idx;
    rgi.recv_idx = pRD->r_idx;
    rgi.gift_id = pRD->cate_idx;
    rgi.total_number = pRD->number;
    rgi.remain_number = pRD->leavings;
    rgi.time_span = pRD->time;
    rgi.number_once = pRD->once_num;
    rgi.send_roomid = pRD->s_roomid;
    rgi.recv_roomid = pRD->r_roomid;
    
    USER_DATA_BASE userInfo;
    memset(&userInfo, 0, sizeof(userInfo));
    GetUserInfoFromLocal(pRD->s_idx, &userInfo);
    char sutfname[260] = {0};
    int len = sizeof(sutfname);
    GbkToUtf8(userInfo.user_info.base.m_name, sutfname, len);
    strcpy(rgi.send_name, sutfname);
    len = sizeof(rgi.send_room);
    GbkToUtf8(pRD->s_room,rgi.send_room,len);
    
    memset(&userInfo, 0, sizeof(userInfo));
    GetUserInfoFromLocal(pRD->r_idx, &userInfo);
    char rutfname[260] = {0};
    len = sizeof(rutfname);
    GbkToUtf8(userInfo.user_info.base.m_name, rutfname, len);
    strcpy(rgi.recv_name, rutfname);
    len = sizeof(rgi.recv_room);
    GbkToUtf8(pRD->r_room, rgi.recv_room, len);
    
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        if([obj respondsToSelector:@selector(saveErrorcode:)])
            [obj saveErrorcode:ret];
        if([obj respondsToSelector:@selector(pushRenderGift:)])
            [obj pushRenderGift:&rgi];
        if([obj respondsToSelector:@selector(onChatUnReadAdd)])
            [obj onChatUnReadAdd];
    }
}
//add by jinguanfu 2010/3/4 原有回调API改成接口<end>
//接收房间重复登录消息
void CResponse::RecvRoomKickUser(){}
//接收大厅重复登录消息
void CResponse::RecvLobbyKickUser(){}
//从大厅接收房间简易信息
void CResponse::RecvRoomSimpleInfo(int ret,int errcode,ResultData_RoomSimpleInfoList* pRD){}
void CResponse::RecvRoomSimpleInfoEx(int ret,int errcode,ResultData_RoomSimpleInfoList* pRD, int type){}
//接收用户退出房间会员回应
void CResponse::RecvExitRoomMember(int ret,int errcode){}
//接收WEB服务器上邮件信息
void CResponse::RecvMailInfo(int ret,char* context,int length){}
//接收大厅在线人数
void CResponse::RecvLobbyOnlineNum(int ret,int errcode,ResultData_LobbyOnlineList* pRD){}
//接收送礼回应消息	//add by jinguanfu 2010/4/27
void CResponse::RecvSendFlower(int ret,int errcode,int receive_idx,int item_id,int count,int gold,int silver){
    
    if(ret == 0)
        UpdataMoney(gold,silver);
    
    //消息通知礼物发送结果
    NSMutableDictionary *userinfo =[[NSMutableDictionary alloc] init];
    [userinfo setValue:@"1" forKey:@"type"];//礼物
    [userinfo setValue:[NSString stringWithFormat:@"%d",errcode] forKey:@"result"];
    [[NSNotificationCenter defaultCenter] postNotificationName:KTVAssistantMSG object:nil userInfo:userinfo];
    
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        if([obj respondsToSelector:@selector(saveErrorcode:)])
            [obj saveErrorcode:errcode];
        if([obj respondsToSelector:@selector(CheckGiftSend)])
            [obj performSelectorOnMainThread:@selector(CheckGiftSend) withObject:nil waitUntilDone:NO];
    }
}
//接收发送大喇叭回应 //add by jinguanfu 2010/5/13
void CResponse::RecvSpeakerToAll(int ret,int errcode,int gold,int silver){}
//接收发送小喇叭回应 //add by jinguanfu 2010/5/13
void CResponse::RecvSpeakerToLobby(int ret,int errcode,int gold,int silver){
    NSMutableDictionary *userinfo =[[NSMutableDictionary alloc] init];
    [userinfo setValue:@"2" forKey:@"type"];//喇叭
    if(ret == 0)
    {
        UpdataMoney(gold,silver);
        [userinfo setValue:@"0" forKey:@"result"];//成功
    }
    else
        [userinfo setValue:[NSString stringWithFormat:@"%d",errcode] forKey:@"result"];//失败
    [[NSNotificationCenter defaultCenter] postNotificationName:KTVAssistantMSG object:nil userInfo:userinfo];
    
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        if([obj respondsToSelector:@selector(saveErrorcode:)])
            [obj saveErrorcode:errcode];
        if([obj respondsToSelector:@selector(RecvSpeakerFromLobby)])
            [obj performSelectorOnMainThread:@selector(RecvSpeakerFromLobby) withObject:nil waitUntilDone:NO];
    }
}
//接收由管理员设为一般会员的回应 //add by jinguanfu 2010/5/13
void CResponse::RecvRoomMember (int ret , int errcode, int idx,int cseq){}
//接收由管理员设为一般会员的通知 //add by jinguanfu 2010/5/13
void CResponse::RecvRoomNotice4Member (int ret , int opidx,int bopidx){}
//接收房间内有人改变了avarat的广播 //add by cx 2010-5-20
void CResponse::RecvRoomNotice4ChangeAvarat (int ret, int idx){}
//接收房间内错误操作提示 //add by cx 2010-6-6
void CResponse::RecvOneError( short cmd ){}
//add by cx 2010-7-6 接受购买物品回应
void CResponse::RecvBuyPack( int ret, int errcode, int gold, int silver, short * goods_id, int * global_id, int count, const char * day ){}
// 接受购买物品回应
void CResponse::RecvBuyForSend( int ret, int errcode, int idx, int gold, int silver ){}
//接受获取背包回应
void CResponse::RecvGetPack( int ret, int errcode, ResultData_Pack * pPack, int count, int all_count ){}
//接受删除物品回应
void CResponse::RecvDelPack( int ret, int errcode, int cseq ){}
//接受商品过期通知
void CResponse::RecvTimeoutPack( int count, int * global_id ){}
//接受商品赠送
void CResponse::RecvSendPack( int idx, int global_id, short goods_id, const char * day, const char * msg ){}
//接受使用物品回应
void CResponse::RecvUsePack( int ret, int errcode, int global_id ){}
// 升级广播
void CResponse::RecvNotice4LevelUp( int idx, char level ){}
// 经验提升
void CResponse::RecvExpChange( int nexp, int level ){}
// vip改变
void CResponse::RecvVipChange( int idx, int level, char is_vip ){}
// 钱币改变
void CResponse::UpdataMoney( int r_gold, int r_silver )
{
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(UpdateMoney)])
            [obj performSelectorOnMainThread:@selector(UpdateMoney) withObject:nil waitUntilDone:NO];
    }
}
// 更新自己的信息
void CResponse::RecvUpdataMyInfo(){}
// 接受烟花广播
void CResponse::RecvFireworks( int ret, int errcode, int cate_idx, int count, int gold, int silver ){}
// 接受烟花广播
void CResponse::RecvFireworksNotice( int player_idx, int cate_idx, int count ){}
// 中奖？
void CResponse::RecvLucky( int player_idx, int cate_idx, int lotte ){}
// 中奖？
void CResponse::RecvLucky( int room_idx, int player_idx, int cate_idx, int lotte ){}
// 列表
void CResponse::RecvGiftTopList( ResultData_GiftList * gift_list ){}
// 在线时间
void CResponse::RecvOnlineTime( int idx, int day_time, int online ){}
// 获取新手任务
void CResponse::RecvGetNewUserTask( short step ){}
// 返回新手任务提交结果
void CResponse::RecvTaskStep( int ret, int err_code, int gold, int silver, int total_gold, int total_silver ){}
void CResponse::RecvTaskGift( int ret, int err_code, int item_id, int object_id, const char * time_buf ){}
// 商品信息更改通知
void CResponse::RecvNotice4ChangeGoods(){}
// 服务器请求刷新背包
void CResponse::RecvGetBagAgain(){}
// 返回用户印章
void CResponse::RecvUserSeel( int user_idx, int seel_idx ){}
// 返回用户防沉迷状态
void CResponse::EnterAllMentState( int idx, short state ){}
// room loge
void CResponse::RecvRoomLoge( int room_idx, const char * loge, int len ){}
// 新分组版本号
void CResponse::RecvNewVersionId( int version_id ){
    if(version_id > GetServerGroupVersion(XG_LOBBY_ID))
    {
        //更新本地分组信息
        if(m_bRequestUpdateGroupRoomlist)//已经在更新中，等待完成
			return ;
        
		PLog(@"%s : need to update the group info",__FUNCTION__);
		ClearRoom();
		ClearGroup();
        
		if(0 != GetGroupInfo())
			PLog(@"%s : GetGroupInfo failed",__FUNCTION__);
    }
}
// 分组信息
void CResponse::RecvGroupInfo( int count, ResultData_GroupInfo * pInfo ){
    //对于可能以后出现的多个大厅信息，可以在返回的结构中要求加数据，目前只是默认登陆的大厅ID
	for(int i=0;i<count;i++)
    {
        char utf[300] = {0};
        int size = 300;
        GbkToUtf8(pInfo[i].group_name,utf,size);
		if(0 == AddGroupData(pInfo[i].group_id,pInfo[i].group_name,pInfo[i].disable,XG_LOBBY_ID))
        {
            AddGroupDataEnd();
            return ;
        }
    }
	AddGroupDataEnd();
    
	//记录请求group的次数
	if(count != 0)
		m_bRequestUpdateGroupRoomlist = true;
	m_nTempCount = count;
	m_nTempCur = 0;//当前第几次回应
    
	//向服务器依次请求group数据
	for(int i=0;i<count;i++)
	{
		if(0 != GetRoomListInGroup(pInfo[i].group_id))
		{
			PLog(@"%s : GetRoomListInGroup failed",__FUNCTION__);
			return ;
		}
	}
}
// 获取组里面的房间列表
void CResponse::RecvRoomListInGroup( short group_idx, int count, int * list ){
    for(int i=0;i<count;i++)
    {
		if(0 == AddRoomData(list[i],group_idx,XG_LOBBY_ID))
        {
            AddRoomDataEnd();
            return ;
        }
    }
	AddRoomDataEnd();
    
	m_nTempCur ++;
	//回应已经全部接受
	if(m_nTempCur == m_nTempCount)
	{
		PLog(@"%s : All Group Room data has been recevied",__FUNCTION__);
		SaveServerGroupVersion(XG_LOBBY_ID,m_nGroupVersionID);
		m_bRequestUpdateGroupRoomlist = false;
		//cseq保存idx，用于在服务器返回的时候可以处理该数据
		if(0 != GetPartInfo(GetMyIdx(),GetMyIdx()))
			PLog(@"%s : GetPartInfo failed",__FUNCTION__);
        
		m_nTempCur = 0;
		m_nTempCount = 0;
	}
}
void CResponse::RecvRoomListInGroup(short count, ResultData_GroupRoomInfo* pInfo)
{
    
}
// 上传日志通知
void CResponse::RecvNotice4CollectData( const char * server_ip, short server_port ){}
// 获取歌曲信息
void CResponse::RecvSongInfoList( short totalnum, short curnum, ResultData_SongInfo * info ){}
// 获取音视频服务器列表
void CResponse::RecvAvServerList( ResultData_AvServer * list, int count ){}
// 更新音视频服务器列表
void CResponse::UpdataAvServerList(){}
// 被提出游戏服务器
void CResponse::RecvCheckOutGame( int result ){}
// 获取离线消息
void CResponse::RecvOffLineMsg( int count, ResultData_OffLineMsg * msg ){}
// 返回分组置顶信息
void CResponse::RecvRoomSortInGroup( int idx, int count, ResultData_RoomSort * list ){
    ////////////////////////////
}
// 通知分组置顶信息改变
void CResponse::RecvRoomSortChange( int idx ){}
// 歌手等级
void CResponse::RecvSingerLevel(int idx, short curlevel, int expvalue){}
// 财富等级
void CResponse::RecvWealthLevel(int idx, short curlevel, int expvalue){}

// 礼物失效（印章）的协议
void CResponse::RecvGiftInvalid( int idx, int GiftID ){}
// 礼物有效（印章）的协议
void CResponse::RecvGiftValid( int idx, int GiftID ){}
// 显示房间可领取收益
void CResponse::RecvShowRoomIncom( int income ){}
// 显示收益已领取记录
void CResponse::RecvShowRoomGetIncom( ResultData_IncomeDate * data, int count ){}
// 室主领取房间内收益
void CResponse::RecvGetRoomIncom( int ret ){}
// 礼物信息更改通知
void CResponse::RecvNotice4ChangeFlower(){}
// 权限配置更新通知
void CResponse::RecvNotice4ChangePower(){}
// 更改音视频服务器广播
void CResponse::RecvNotice4AvserverChange( ResultData_AvChange * info ){}
// 更改gm
void CResponse::RecvChangeGM( int idx, char add ){}
// 上麦的人的pc概况
void CResponse::RecvMicPcInfo( int idx, const char * buf, int len ){}
// 回应是否邀加入房间成功
void CResponse::RecvSendJoinRoomInvite( int wseq, int ret, int err_code ){}
// 回应是否邀加入房间成功
void CResponse::RecvInviteJoinRoom( int idx ){}
//取得当日礼物赠送记录
void CResponse::RecvGiftSend( int count, ResultData_GiftCount * pInfo ){}
//取得当日礼物接收记录
void CResponse::RecvGiftRecv( int count, ResultData_GiftCount * pInfo ){}
//返回封mac结果
void CResponse::RecvKickUserByMac( int ret, int idx ){}
//返回封ip结果
void CResponse::RecvKickUserByIp( int ret, int idx ){}
//返回设置在麦时间结果
void CResponse::RecvSetMicTime( int ret, int idx ){}
//广播在麦时间延长
void CResponse::RecvNotice4SetMicTime( int idx, int time ){}
//取排麦列表
void CResponse::RecvWaitMicList( short count, int* idxs ){}
//防沉迷时间到,断开连接
void CResponse::RecvIndulgeTimeUp(){}
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
void CResponse::RecvFamilyInfo(int flag, int familyNum, ResultData_Family* familys){}

/*
 av配置信息
 */
void CResponse::RecvAVConfigInfo(int infolen, const char* info){}

/************************************************************************/
/* 私麦                                                                 */
/*上私麦*/
void CResponse::RecvOpenPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
void CResponse::RecvOpenPrivateMicFailed(int errorcode, int idx, int bidx){}
void CResponse::RecvOpenPrivateMicNotice(int idx, int mictype){} /*不包括自己*/

/*下私麦*/
void CResponse::RecvClosePrivateMicFailed(int errorcode, int idx, int bidx){}
void CResponse::RecvClosePriveteMicNotice(int idx){} /*包括自己*/

/*被抱下私麦*/
void CResponse::RecvClosedPrivateMicFailed(int errorcode, int idx, int bidx){}
void CResponse::RecvClosedPriveteMicNotice(int idx, int bidx){} /*包括自己*/

/*观看私麦*/
void CResponse::RecvWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
void CResponse::RecvWatchPrivateMicFailed(int errorcode, int idx, int bidx){}
void CResponse::RecvRequestWatchedPrivateMic(int idx){} /*idx=请求者, 是否同意该idx观看你的私麦*/
void CResponse::RecvWatchedPrivateMicFailed(int errorcode, int idx, int bidx){}

void CResponse::RecvWatchedPrivateMicReject(int idx){} //对方拒绝，idx=谁拒绝的

/*邀请私麦*/
void CResponse::RecvInviteWatchPrivateMicSuccess(){}
void CResponse::RecvInviteWatchPrivateMicFailed(int errorcode, int idx, int bidx){}
void CResponse::RecvRequestInvitedWatchPrivateMic(int idx){} /*idx=邀请者,是否同意观看idx的私麦*/
void CResponse::RecvInvitedWatchPrivateMicSuccess(int idx, short mictype, int posindex, int roomid){}
void CResponse::RecvInvitedWatchPrivateMicFailed(int errorcode, int idx, int bidx){}

void CResponse::RecvInvitedWatchPrivateMicReject(int idx){}//对方拒绝，idx=谁拒绝的

/*管理房间私麦*/
void CResponse::RecvOpenRoomPrivateMicFlag(int ret, char flag){}
void CResponse::RecvOpenRoomPrivateMicFlagNotice(int idx, char flag){} /*包括自己*/

/*用户登录房间时，回调房间的私麦信息*/
void CResponse::RecvPrivateMicWithLoginRoom(int idx, short mictype, int posindex, int roomid){}
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
void CResponse::RecvCrownKickoutSomeOne(int ret, int kickuse, int totalkick, int idx){}
/*室主禁止用户ip
 int ret{}
 0：成功
 -1：该IP已被禁用
 -2：该用户已离开房间
 
 int idx{}
 被禁用者idx
 */
void CResponse::RecvForbidUserIp(int ret, int idx){}

//要求上传歌曲
void CResponse::RecvUpdateSong(){}
//回应服务器的返回用户歌唱信息
//void CResponse::RecvUserSingInfo(int ret,int idx,const ResultData_UserSingInfo* pData){}
//获取用户接受礼物个数
void CResponse::RecvGiftCount(int recvcount){}

//获取用户最高分歌曲信息及演唱歌曲个数
void CResponse::RecvTopSongInfo(int songcount, int topsongscore, const char* topsongname, int len){}
//手机ktv聊天信息
void CResponse::RecvKtvChatInfo(int ret,const int fromIdx,int roomid,const char* roomName,const char* content,
                                             int chatTime,bool isPublic,bool isSend){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(saveErrorcode:)])
            [obj saveErrorcode:ret];
    }
    
    NSMutableDictionary *userinfo =[[NSMutableDictionary alloc] init];
    [userinfo setValue:@"0" forKey:@"type"];//聊天
    if(ret == 0)
    {
        char utfroom[260] = {0};
        char utfcontent[4096] = {0};
        
        int len = sizeof(utfroom);
        int ret = 1;
        if(roomid == 0)
            strcpy(utfroom, roomName);
        else
            ret = GbkToUtf8(roomName, utfroom, len);

        len = sizeof(utfcontent);
        ret &= GbkToUtf8(content, utfcontent, len);
        if (ret == 0) {
            PLog(@"%s:Gbk to Utf8 failed : RoomName and Content",__FUNCTION__);
            return ;
        }
        
        AllInfo allInfo ={0};
        allInfo.roomid = roomid;
        allInfo.time = chatTime;
        allInfo.isPublic = false;
        strcpy(allInfo.roomname, utfroom);
        strcpy(allInfo.content, content);
        
        
        if(isSend)//回应
        {
            [userinfo setValue:@"0" forKey:@"result"];//成功
            [[NSNotificationCenter defaultCenter] postNotificationName:KTVAssistantMSG object:nil userInfo:userinfo];
            
            allInfo.idx = 0;
            allInfo.toidx = fromIdx;
            strcpy(allInfo.name, "我");
            if(!isPublic)//不是公聊
            {
                allInfo.type = 1;
                //取出聊天对象的名字
                USER_DATA_BASE userInfo;
                memset(&userInfo, 0, sizeof(userInfo));
                if(GetUserInfoFromLocal(fromIdx, &userInfo))//私聊信息
                {
                    //gbk -> utf8
                    char utfname[260] = {0};
                    
                    len = sizeof(utfname);
                    ret = GbkToUtf8(userInfo.user_info.base.m_name, utfname, len);
                    if (ret == 0) {
                        PLog(@"%s:Gbk to Utf8 failed : UserName",__FUNCTION__);
                        return ;
                    }
                    strcpy(allInfo.toname, utfname);
                }
            }
            else
            {
                allInfo.type = 0;
            }
            strcpy(allInfo.headphoto, [[UserSessionManager getInstance].currentUser.headphoto UTF8String]);
        }
        else//别人发的聊天信息
        {
            allInfo.idx = fromIdx;
            allInfo.toidx = 0;
            USER_DATA_BASE userInfo;
            memset(&userInfo, 0, sizeof(userInfo));
            if(GetUserInfoFromLocal(fromIdx, &userInfo))//私聊信息
            {
                //gbk -> utf8
                char utfname[260] = {0};
                
                len = sizeof(utfname);
                ret = GbkToUtf8(userInfo.user_info.base.m_name, utfname, len);
                if (ret == 0) {
                    return ;
                }
                strcpy(allInfo.name, utfname);
            }
            
            if(!isPublic)
            {
                allInfo.type = 1;
                strcpy(allInfo.toname, "我");
            }
            else
            {
                allInfo.type = 0;
            }
            
            allInfo.recv_roomid = 0;
            strcpy(allInfo.recv_room, "0");
            allInfo.gift_id = 0;
            allInfo.total_number = 0;
            strcpy(allInfo.unit_name, "0");
            char head[256] = {0};
            NSString *domin = [UserSessionManager getInstance].currentUser.domin;
            char *p = strstr(userInfo.user_info.base.headurl, "http://");
            if(NULL == p)
            {
                strcpy(head, [domin UTF8String]);
                strncat(head, userInfo.user_info.base.headurl, strlen(userInfo.user_info.base.headurl));
                strcpy(allInfo.headphoto, head);
            }
            else
            {
                strcpy(allInfo.headphoto, userInfo.user_info.base.headurl);
            }
        }
        
        for(id obj in m_callbackprotocol)
        {
            if([obj respondsToSelector:@selector(pushChatInfo:)])
                [obj pushChatInfo:&allInfo];
            if([obj respondsToSelector:@selector(onChatUnReadAdd)])
                [obj onChatUnReadAdd];
        }
    }
    else
    {
        if(isSend)
        {
            [userinfo setValue:@"-1" forKey:@"result"];//失败
            [[NSNotificationCenter defaultCenter] postNotificationName:KTVAssistantMSG object:nil userInfo:userinfo];
        }
    }
}
//手机接收到当前的播放列表，即当前ktv的点歌列表，用于更新
/*
 @param pSonglist:信息结构体
 @param listcount:歌曲数量
 */
void CResponse::RecvKtvSongList(const ResultData_SongList* pSonglist,int listCount){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(pushReqSongInfo::)])
            [obj pushReqSongInfo:pSonglist :listCount];
    }
}
//手机获取当前包厢的开关状态
/*
 @param isOpen:true,包厢开启；false，包厢关闭，用户退出房间
 */
void CResponse::RecvKtvRoomState(bool isOpen){
    if (!isOpen)
    {
        AutoLock lock(m_callbacklock);
        for(id obj in m_callbackprotocol)
        {
            if([obj respondsToSelector:@selector(closeRoom)])
                [obj closeRoom];
        }
    }
}
//手机收到唱歌上榜通知,提示用户到个人信息里面去抢榜
void CResponse::RecvKtvSongRanking(){
    PLog(@"%s",__FUNCTION__);
}

//通知歌曲演唱结束
void CResponse::RecvKtvSongNormalEnd(int nMusicId){
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(noticeEndsong)])
            [obj noticeEndsong];
    }
}

/*
 收到的在线手机用户IDX列表更新数据
 只有IDX
 @Deprecated
 */
void CResponse::RecvKtvIdxList(const int* pIdx,int number)
{
}
/*
 通知用户头像更新
 @param [in] idx：指定的用户IDX
 @param [in] picture：用户的新头像
 */
void CResponse::RecvUserPicChange(int idx,const char* picture)
{
    //gbk -> utf8
    char picname[260] = {0};
    int len = sizeof(picname);
    GbkToUtf8(picture, picname, len);

    USER_DATA_BASE userInfo;
    memset(&userInfo,0,sizeof(userInfo));
 
    GetUserInfoFromLocal(idx, &userInfo);
    RequestData_IdxWithVersion* reqIdx = new RequestData_IdxWithVersion[1];
    reqIdx[0].id = idx;
    reqIdx[0].version = userInfo.version;
    GetUserInfoWithVersion(&reqIdx[0], 1);

    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(isChanged)])
            [obj isChanged];
    }
}

void CResponse::RecvUserNameChange(int idx,const char* name)
{
    if(name == NULL || strlen(name) == 0)
        return ;
    
    char utf8[300] = {0};
    int size = 300;
    if(1 == GbkToUtf8(name, utf8, size))
    {
        AutoLock lock(m_callbacklock);
        for(id obj in m_callbackprotocol)
        {
            if([obj respondsToSelector:@selector(onNameChange:name:)])
                [obj onNameChange:idx name:utf8];
        }
    }
}
/*
 通知获取到的喇叭用户信息
 */
void CResponse::RecvUserInfoForSpeaker(int errcode,ResultData_UserSampleInfo* info)
{
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(pushRecvSpeakInfo:)])
            [obj pushRecvSpeakInfo:info];
    }
}
/*
 接受发喇叭者信息超时
 需要判读idx数组中的具体idx
 @param [in] idx:idx数组； 数组中某个IDX如果为0，则原来的IDX已经获取到信息，只是底层把它置为0
 @param [in] count:idx数组大小
 */
void CResponse::RecvUserInfoForSpeakerTimeOut(int idx[],int count)
{
    bool bTimeout = false;
    for (int i=0; i<count; i++) {
        if(idx[i])
            bTimeout = true;
    }
    
    if(bTimeout)
    {
        //有用户信息获取超时
    }
}
/*
 接受用户信息超时
 需要判读idx数组中的具体idx
 @param [in] idx:idx数组； 数组中某个IDX如果为0，则原来的IDX已经获取到信息，只是底层把它置为0
 @param [in] count:idx数组大小
 */
void CResponse::RecvUserInfoTimeOut(int idx[],int count)
{
    PLog(@"%s",__FUNCTION__);
}

void CResponse::RecvNotifyMessage(int msgId,const char* msgContent)
{
    PLog(@"%s",__FUNCTION__);
}

//单个礼物图片下载完成通知
void CResponse::RecvImageDownladFinished(const char* pictureName){
    PLog(@"%s",__FUNCTION__);
}

/*
 收到服务器的通知在线的用户信息IDX for new
 @param list : 用户列表
 @param size : 列表个数的数量
 */
void CResponse::RecvUserIdxAndType(const ResultData_UIdxWithType* list/*[number]*/,int size)
{
    if (list && size)
    {
        RequestData_IdxWithVersion* reqIdx = new RequestData_IdxWithVersion[size];
        if(!reqIdx)
            return ;
        int j = 0;
        for (int i=0; i<size; i++)
        {
            PLog(@"idx = %d,type=%c", list[i].idx,list[i].type);
            USER_DATA_BASE userInfo;
            memset(&userInfo,0,sizeof(userInfo));
            if(0 == GetUserInfoFromLocal(list[i].idx, &userInfo))  //本地没有保存
            {
                reqIdx[j].id = list[i].idx;
                reqIdx[j++].version = -1;
            }
            else
            {
                if(list[i].idx == GetMyIdx())
                {
                    [UserSessionManager getInstance].currentUser.type = list[i].type;
                    continue;
                }
                
                reqIdx[j].id = list[i].idx;
                reqIdx[j++].version = userInfo.version;
            }
        }
        
        int remain = j;
        int cur = 0;
        do {
            if(remain == 0)
                break;
            
            if(remain > 200)
            {
                GetUserInfoWithVersion(&reqIdx[cur], 200);
                cur += 200;
                remain -= 200;
            }
            else
            {
                GetUserInfoWithVersion(&reqIdx[cur], remain);
                remain = 0;
            }
            
        } while (remain > 0);
        
        delete reqIdx;
    }
    
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if(!obj)
            continue;
        
        if([obj respondsToSelector:@selector(loginNotice)])
            [obj loginNotice];
        if([obj respondsToSelector:@selector(pushOnline::)])
            [obj pushOnline:list :size];
    }
}

/*
 收到服务器的通知，有用户登入，这个是新协议，为了兼容老版本，所以新版本的客户端对原来的回调屏蔽了RecvUserLoginLobbyNotice
 @param idx: 用户的idx
 @param type : 用户的在线方式 ： 0-主播，1-本包厢，2-其他包厢主备，3-大厅
 
 @注：这个接口还包含了用户在线方式的改变，比如从大厅到包厢，所以需要先判断用户是否在列表中，如果在列表中则修改，如果不在则添加
 */
void CResponse::RecvUserLoginLobbyNoticeNew(int idx,char type)
{
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(onRecvUserLoginNew:type:)])
            [obj onRecvUserLoginNew:idx type:type];
    }
}

/*
 收到发送红包的结果
 @param ret : 红包发送结果:
 0-成功  1-金币不足  2-对方不在线（个人红包） 3-不在包厢（包厢红包）	9999-服务器相应超时
 */
void CResponse::RecvSendHBResult(int ret)
{
    //消息通知红包发送结果
    NSMutableDictionary *userinfo =[[NSMutableDictionary alloc] init];
    [userinfo setValue:@"8" forKey:@"type"];//红包
    [userinfo setValue:[NSString stringWithFormat:@"%d",ret] forKey:@"result"];
    [[NSNotificationCenter defaultCenter] postNotificationName:KTVAssistantMSG object:nil userInfo:userinfo];
}

/*
 收到互动红包
 @param idx : 发红包的用户的idx
 @param hbid : 红包ID
 @param hbtype : 红包类型 0-个人  1房间手气 2-房间均分  3-大厅手气 4-大厅均分
 @param remark : 发红包用户的备注信息
 */
void CResponse::RecvHongBao(int idx,const char* hbid,char hbtype,const char* remark)
{
    AllInfo allInfo ={0};
    allInfo.type = 8;
    allInfo.redtype = hbtype;
    strcpy(allInfo.redid, hbid);
    
    //gbk -> utf8
    char utfmark[260] = {0};
    int marklen = sizeof(utfmark);
    if(0 == GbkToUtf8(remark, utfmark, marklen))
    {
        return ;
    }

    strcpy(allInfo.content, utfmark);
    if (idx == [UserSessionManager getInstance].currentUser.uid) {
        allInfo.idx = 0;
        strcpy(allInfo.name, "我");
        strcpy(allInfo.headphoto, [[UserSessionManager getInstance].currentUser.headphoto UTF8String]);
    }
    else//别人发的聊天信息
    {
        allInfo.idx = idx;
        USER_DATA_BASE userInfo;
        memset(&userInfo, 0, sizeof(userInfo));
        if(GetUserInfoFromLocal(idx, &userInfo))//私聊信息
        {
            //gbk -> utf8
            char utfname[260] = {0};
            int len = sizeof(utfname);
            if(0 == GbkToUtf8(userInfo.user_info.base.m_name, utfname, len))
            {
                return ;
            }
            strcpy(allInfo.name, utfname);
            char head[256] = {0};
            NSString *domin = [UserSessionManager getInstance].currentUser.domin;
            char *p = strstr(userInfo.user_info.base.headurl, "http://");
            if(NULL == p)
            {
                strcpy(head, [domin UTF8String]);
                strncat(head, userInfo.user_info.base.headurl, strlen(userInfo.user_info.base.headurl));
                strcpy(allInfo.headphoto, head);
            }
            else
            {
                strcpy(allInfo.headphoto, userInfo.user_info.base.headurl);
            }
        }
    }
    
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(pushChatInfo:)])
            [obj pushChatInfo:&allInfo];
        if([obj respondsToSelector:@selector(onChatUnReadAdd)])
            [obj onChatUnReadAdd];
        //只统计个人红包
        if(hbtype == 0 && [obj respondsToSelector:@selector(onHongBUnReadAdd)])
        {
            KtvUser* pMy = [[NetUtilCallBack getInstance] getKtvUserInfo];
            if(idx != pMy->fullInfo.m_idx)//如果发送者不是我，则不需要增加红包提醒
                [obj onHongBUnReadAdd];
        }
    }
}

void CResponse::RecvHongBaoStatus(const char* hbid,int ret)
{
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(onRecvHongBaoStatus:ret:)])
            [obj onRecvHongBaoStatus:hbid ret:ret];
    }
}

/*
 抢红包的结
 @param result : 抢红包的结果: 0-没抢到  1-抢到 9999-服务器相应超时
 @param idx : 发红包用户的IDX
 @param bhid : 红包ID
 @param hbtype : 红包类型
 @param remark : 红包备注
 @param gold : 抢到金额(没抢到发0)
 */


void CResponse::RecvVieHongBaoResult(int result,int idx,const char* hbid,char hbtype,const char* remark,int gold)
{
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(onRecvVieHongBaoResult:idx:hbid:hbtype:remark:gold:)])
            [obj onRecvVieHongBaoResult:result idx:idx hbid:hbid hbtype:hbtype remark:remark gold:gold];
    }
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
void CResponse::RecvHongBaoDetail(int ret,int idx,char hbtype,const char* remark,int totalgold,int totalnum,int sendtime,const ResultData_HBDetail* list/*[num]*/,int num)
{
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(onRecvHongBaoDetail:idx:hbtype:remark:totalgold:totalnumber:sendtime:list:number:)])
            [obj onRecvHongBaoDetail:ret idx:idx hbtype:hbtype remark:remark totalgold:totalgold totalnumber:totalnum sendtime:sendtime list:list number:num];
    }
}

void CResponse::RecvSingSongChange(const char* songname,const char* singername,int score)
{
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(onRecvSingSongChange:singer:score:)])
            [obj onRecvSingSongChange:songname singer:singername score:score];
    }
}

void CResponse::RecvHongBaoNum(int number)
{
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(onRecvHongBNumber:)])
            [obj onRecvHongBNumber:number];
    }
}

void CResponse::RecvRoomHB(int ret,int number,ResultData_RoomHB* list)
{
    AutoLock lock(m_callbacklock);
    if(ret != OPTION_TIMEOUT)//超时的就不给了
    {
        for(id obj in m_callbackprotocol)
        {
            if([obj respondsToSelector:@selector(onRecvRoomHB:list:)])
            {
                [obj onRecvRoomHB:number list:list];
            }
        }
    }
}

void CResponse::RecvLocalGiftDatabaseFinished(bool direct)
{
    AutoLock lock(m_callbacklock);
    for(id obj in m_callbackprotocol)
    {
        if([obj respondsToSelector:@selector(InitGiftImg)])
            [obj InitGiftImg];
    }
}
