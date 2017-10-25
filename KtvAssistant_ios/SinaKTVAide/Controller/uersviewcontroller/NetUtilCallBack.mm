//
//  NetUtilCallBack.m
//  SinaKTVAide
//
//  Created by cxp on 14-7-23.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "NetUtilCallBack.h"
#import "ResStrings.h"
#import "AutoLock.h"
#import "User.h"
#import "com_ios_NetUtil.h"
#import <CommonCrypto/CommonDigest.h>
#import <Foundation/NSThread.h>
#include <UIKit/UIDevice.h>
#import "xingguang.h"
#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/NSUserDefaults.h>
#import <QuartzCore/QuartzCore.h>
#import "CResponse.h"
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_dl.h>
#include <sys/sysctl.h>
#import "InteractController.h"
#import "KTVBaseConfig.h"
#import "UserSessionManager.h"
#import "KTVAppDelegate.h"
#import "SongInfo.h"
#include <sys/socket.h>
#include <sys/sysctl.h>
#include "KTVBaseConfig.h"
#include "UserLoginController.h"
#include "PKtvAssistantAPI.h"
#include "iToast.h"
#import "BaseNavigationViewController.h"

#define FIRSTFILE "forbiddenword"
#define SECONDFILE "forbiddenword2"
#define FILEEXTERN "dog"

@interface TransactionObj : NSObject

@property(nonatomic,assign) int transaction;
@property(nonatomic,copy) TransactionListener listener;

@end

@implementation TransactionObj

@end


static NetUtilCallBack *instance = nil;

@implementation NetUtilCallBack
@synthesize m_lockChat = lockChat;
@synthesize m_lockGift = lockGift,m_lockShow=lockShow;
@synthesize m_sysTrack;

+(NetUtilCallBack *)getInstance
{
    @synchronized(self)
    {
        if (instance == nil) {
            instance = [[self alloc] init];
            [instance initcallback];
        }
    }
    return instance;
}

-(instancetype)init
{
    self = [super init];
    
    memset(&m_myInfo, 0, sizeof(m_myInfo));
    
    return self;
}
/*-(void)getTable:(InteractTabView *)tabview
{
    tab = tabview;
}*/

#pragma mark sendNotification
//登录结果消息 type:0失败 1成功
-(void)sendLoginLobbyResult:(NSInteger)type
{
    NSMutableDictionary *dic = [[NSMutableDictionary alloc] init];
    [dic setValue:[NSString stringWithFormat:@"%d",type] forKey:@"result"];
    [[NSNotificationCenter defaultCenter] postNotificationName:KTVAssistantLoginLobbyResult object:nil userInfo:dic];
}

-(void)setRoomId:(int)roomId
{
    m_roomId = roomId;
}

-(void)doCurrentWork:(int) transaction
{
    [self doCurrentWork:transaction listener:nil];
}
-(void)doCurrentWork:(int) transaction listener:(TransactionListener)listener
{
    @synchronized(self){
        if(mCurrentTransaction != 0)
        {
            if(listener!=nil)
                listener(NO);
        }
        
        Reachability *reachability = [Reachability reachabilityForInternetConnection];
        if (![reachability isReachable])
        {
            [SVProgressHUD showErrorWithStatus:@"网络有问题,请检查网络"];
            if(listener!=nil)
                listener(NO);
            return;
        }
        
        mCurrentTransaction = transaction;
        mListener = listener;
        if(mCurrentTransaction == TransactionLobbyLogin || mCurrentTransaction == TransactionLobbyAgain)
        {
            DisConnectRoom();
            DisConnectLobby();
            DisConnectAuth();
            [self beginLobbyTransaction];
        }
        else if(mCurrentTransaction == TransactionRoomLogin)
        {
            if(m_bConnectedLobby)
            {
                DisConnectRoom();
                [self beginRoomTransaction];
            }
            else
            {
                DisConnectLobby();
                DisConnectAuth();
                [self beginLobbyTransaction];
            }
        }
        else if(mCurrentTransaction == TransactionAuthAgain)
        {
            DisConnectAuth();
            [self beginLobbyTransaction];
        }
        else
        {
            [self resetCurrentWork];
        }
    }
}

-(void)resetCurrentWork
{
    @synchronized(self){
        mListener = nil;
        mCurrentTransaction = 0;
    }
}

-(void)reconnectLobby:(TransactionObj*)obj
{
    PLog(@"重连大厅或者重新获取令牌...");
    [self doCurrentWork:obj.transaction listener:obj.listener];
}

-(void)reconnectRoom:(TransactionObj*)obj
{
    PLog(@"重连房间...");
    [self doCurrentWork:obj.transaction listener:obj.listener];
}

-(void)onTransactionFinish:(BOOL)success reconnect:(BOOL)reconnect
{
    if(mCurrentTransaction == 0)
        return;
    
    if(success)
    {
        //获取五条喇叭信息
    }
    else if(reconnect)
    {
        //如果连接大厅，或者重连大厅失败
        if(mCurrentTransaction == TransactionLobbyLogin || mCurrentTransaction == TransactionLobbyAgain)
        {
            //5秒之后去重连大厅
            TransactionObj *obj = [[TransactionObj alloc] init];
            obj.transaction = mCurrentTransaction;
            obj.listener = mListener;
            [self performSelector:@selector(reconnectLobby:) withObject:obj afterDelay:5];
        }
        else if(mCurrentTransaction == TransactionRoomLogin && m_roomId != 0)
        {
            //1秒之后去重连房间
            TransactionObj *obj = [[TransactionObj alloc] init];
            obj.transaction = mCurrentTransaction;
            obj.listener = mListener;
            [self performSelector:@selector(reconnectRoom:) withObject:obj afterDelay:1];
        }
        else if(mCurrentTransaction == TransactionAuthAgain)
        {
            //1秒之后去重新获取，借用的是重连大厅的方法
            TransactionObj *obj = [[TransactionObj alloc] init];
            obj.transaction = mCurrentTransaction;
            obj.listener = mListener;
            [self performSelector:@selector(reconnectLobby:) withObject:obj afterDelay:1];
        }
    }
    
    TransactionListener listener = mListener;
    //置为空闲任务
    [self resetCurrentWork];
    
    //如果不需要重连，则需要回调到监听者，我们是否成功
    if(!reconnect)
    {
        if(listener != nil)
            listener(success);
    }
}

- (void)beginLobbyTransaction
{
    PLog(@"连接认证...");
    m_errorCode = 0;
    m_ret = 0;
    m_bWangtong = !!IsWangtong(XG_AUTHHOST, XG_WANGTONG);
    if(ConnectAuth(XG_AUTHHOST, (short)XG_AUTHPORT) != 0)
    {
        PLog(@"连接认证失败!");
        [self onTransactionFinish:NO reconnect:YES];
    }
}

-(void)beginRoomTransaction
{
    if(m_roomId == 0)
    {
        //没有指定包厢，说明事务已经成功完成。
        [self onTransactionFinish:YES reconnect:NO];
    }
    
    ResultData_RoomInfo room = {0};
    GetRoomInfoFromLocal(m_roomId, &room);
    
    int ret = 0;
    if ([NetUtilCallBack getInstance]->m_bWangtong)
        ret = ConnectRoom(room.ip_2, room.port, XG_ROOMTYPE_BASE);
    else
        ret = ConnectRoom(room.ip_1, room.port, XG_ROOMTYPE_BASE);
    
    if(ret == 0)
        printf("房间连接中...\n");
    else
    {
        printf("请求房间连接失败!\n");
        [self onTransactionFinish:NO reconnect:YES];
    }
}

-(KtvUser*)getKtvUserInfo
{
    return &(self->m_myInfo);
}

- (void)initcallback
{
    NSString *unicode;
    if ([[UIDevice currentDevice] respondsToSelector:@selector(identifierForVendor)]) {
        // This will run if it is iOS6 or higher
        unicode = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
    } else {
        unicode = @"sdfsdf";
    }
    
    memcpy(m_cunicode,unicode.UTF8String,strlen(unicode.UTF8String));
    //strcpy(m_cunicode, "40:6c:8f:4f:f4:76");
    m_channellist = NULL;
    m_response = NULL;
    m_bWangtong = false;
    m_bConnectedLobby = false;
    m_bConnectedRoom = false;
    m_isChanged = false;
    //m_roomId = 0; //外部必须设置
    m_errorCode = 0;
    m_clientData = 0;
    
    m_response = CResponse::getResponseSingleton();
    //这里保证是第一个回调监听
    m_response->initView(self);
    SetResponseHandle(m_response);
    StartClient();
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *docDir = [paths objectAtIndex:0];
    
    //NSArray* pListitems = [NSArray arrayWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"DownloadTest" ofType:@"plist"]];
    NSArray* pListitems = [NSArray arrayWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"DownloadUrl" ofType:@"plist"]];
    
    unsigned int i = 0;
    for (NSString* pUrl in pListitems) {
        InitDownLoadUrl(pUrl.UTF8String, i);
        i++;
    }
    /*
     模块内部路径：
     app 资源文件路径存放目录
     docDir
     |-congfig 存放配置文件,比如礼物信息xml，本地房间分组xml
     |
     |-images 存放图片资源文件,暂时想到的是礼物图片
     |
     |-update 存放app临时下载文件，可选删除下载文件，比如包含xml的zip文件
     |
     |
     */
    if(!InitLoaclDatabaseDir(docDir.UTF8String))
        printf("%s : InitLoaclDatabaseDir Failed\n",__FUNCTION__);
    
    [self InitGiftImg];
    bool b = MFInitFilter();
    if(!b)
        printf("%s : Init Message Filter Failed!\n",__FUNCTION__);
}

#pragma mark -
#pragma mark UTIL FUNCTION
+(const char *)md5:(const char *)str
{
    const char *cStr = str;//UTF-8 c string
    unsigned char result[16];
    CC_MD5(cStr, strlen(cStr), result); // This is the md5 call
    static char ret[32] = {0};
    memset(ret,0,sizeof(ret));
    sprintf(ret, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
            result[0], result[1], result[2], result[3],
            result[4], result[5], result[6], result[7],
            result[8], result[9], result[10], result[11],
            result[12], result[13], result[14], result[15]);
    return ret;
}
+(bool)onAuthError:(int) e
{
    switch (e) {
        case 0:
        case AUTH_NORMAL:
            PLog(@"恭喜您,登录成功!");
            return true;
        case AUTH_NOTEXIST:
            PLog(@"帐号不存在!");
            break;
        case AUTH_PWDERROR:
            PLog(@"登陆密码错误!");
            break;
        case AUTH_TOURISTERROR:
            PLog(@"游客登录失败!");
            break;
        case AUTH_CACHEERROR:
            PLog(@"登录不成功!");
            break;
        case AUTH_TOKENERROR:
            PLog(@"取得token失败!");
            break;
        case OPTION_TIMEOUT:
            PLog(@"认证超时...");
            break;
        default:
            break;
    }
    
    return false;
}

+(bool)onLobbyError:(int) e
{
    switch(e)
    {
        case 0:
        case LOBBY_NORMAL:
            return true;
        case LOBBY_LOGIN_IDLOCK:
            [SVProgressHUD showErrorWithStatus:@"账号被冻结"];
            DisConnectLobby();
            break;
        case LOBBY_LOGIN_TOOOLD:
            [SVProgressHUD showErrorWithStatus:@"版本错误"];
            DisConnectLobby();
            break;
        case LOBBY_LOGIN_NOTEXIST:
            [SVProgressHUD showErrorWithStatus:@"用户不存在"];
            DisConnectLobby();
            break;
        case LOBBY_LOGIN_NOAVATAR://认为登陆成功
            PLog(@"还未创建avatar信息");
            return true;
        case LOBBY_USERINFO_NOTEXIST:
            [SVProgressHUD showErrorWithStatus:@"用户信息不存在"];
            DisConnectLobby();
            break;
        case LOBBY_USERINFO_INDULGE://不理防沉迷信息
            //tip.text = @"用户防沉迷中"
            return true;
        case OPTION_TIMEOUT:
            PLog(@"大厅超时...");
            break;
    }
    
    return false;
}

-(bool)onRoomError:(int)e
{
    switch (e) {
        case 0:
        case ROOM_NORMAL://正常
            return true;
//        case ROOM_TOKEN_ERROR:
//            PLog(@"token过程异常");
//            break;
        case ROOM_LOGIN_NONE:
            [SVProgressHUD showErrorWithStatus:@"未登录"];
            break;
        case ROOM_LOGIN_SUCCESS:
        {
            PLog(@"登录成功");
            RequestCurrentRoomHB(m_roomId);
            return true;
        }
        case ROOM_LOGIN_AGAIN:
            [SVProgressHUD showErrorWithStatus:@"重复登录"];
            break;
        case ROOM_LOGIN_NOUIN:
            [SVProgressHUD showErrorWithStatus:@"非法用户"];
            break;
        case ROOM_LOGIN_FULL:
            [SVProgressHUD showErrorWithStatus:@"房间人数满"];
            break;
        case ROOM_LOGIN_IFULL:
            [SVProgressHUD showErrorWithStatus:@"房间已经达到允许人数,用户身份无法登陆"];
            break;
        case ROOM_LOGIN_EPWD:
            [SVProgressHUD showErrorWithStatus:@"房间密码错误"];
            break;
        case ROOM_LOGIN_CLOSE:
            [SVProgressHUD showErrorWithStatus:@"房间关闭"];
            break;
        case ROOM_LOGIN_INBLACK:
            [SVProgressHUD showErrorWithStatus:@"在黑名单中"];
            break;
        case ROOM_LOGIN_INTBLACK:
            [SVProgressHUD showErrorWithStatus:@"在临时黑名单中"];
            break;
        case ROOM_LOGIN_NOTHIS:
            [SVProgressHUD showErrorWithStatus:@"房间不存在或被删除"];
            break;
        case ROOM_LOGIN_LOCK:
            [SVProgressHUD showErrorWithStatus:@"房间冻结"];
            break;
        case ROOM_LOGIN_IP_INVALIED:
            [SVProgressHUD showErrorWithStatus:@"该IP在房间内被禁"];
            break;
    }
    
    return false;
}
-(void)onEnterRoom
{
    //无密码进入包厢房间,如果以后需要增加输入密码，请在此添加
    int roomid = [[UserSessionManager getInstance].currentRoomInfo.xgRoomID intValue];
    std::string pwd = [[UserSessionManager getInstance].currentRoomInfo.roomPassword UTF8String];
    if(EnterRoom(roomid, roomid, "11111", (short)0) != 0)
    {
        PLog(@"进入房间失败");
        DisConnectRoom();
    }
    PLog(@"正在进入房间...");
    
}
#pragma mark -
#pragma mark Save Data Method
//保存线程回调的数据
-(void)saveRet:(int)ret
{
    m_ret = ret;
}
-(void)saveErrorcode:(int)code
{
    m_errorCode = code;
}
-(void)saveToken:(const char*)token length:(int)len
{
    memcpy(m_tokenInfo.token, token,len);
    m_tokenInfo.length = len;
}
-(void)saveChannellist:(ResultData_GetChannellist*)list
{
    if(list)
    {
        //释放之前保存的信息
        if(m_channellist)
        {
            if(m_channellist->pCh)
                delete m_channellist->pCh;
            delete m_channellist;
            m_channellist = NULL;
        }
        
        m_channellist = new ResultData_GetChannellist;
        //保存大厅数量
        m_channellist->count = list->count;
        //保存某个大厅具体信息
        m_channellist->pCh = new Channel[m_channellist->count];
        for(int i=0;i<m_channellist->count;i++)
        {
            //逐个保存
            memcpy(&(m_channellist->pCh[i]),&(list->pCh[i]),sizeof(Channel));
        }
    }
    
}
-(void)saveUserInfo:(KtvUser*)user
{
    memcpy(&m_myInfo, user, sizeof(KtvUser));
    m_myInfo.online_type = mOnlineType;
}

-(void)saveCSeq:(int)cseq
{
    m_clientData = cseq;
}

-(void)saveLoginInfo:(ResultData_LoginInfo*)loginInfo
{
    memcpy(&m_roomLoginInfo,loginInfo,sizeof(ResultData_LoginInfo));
}

#define MAX_CHAT_LENGTH 100

-(void)pushChatInfo:(AllInfo*)allInfo
{
    if (allInfo->redtype == 6 && allInfo->type == 8) {
        if (m_tmpredid.size() > MAX_CHAT_LENGTH) {
            SETREDID::iterator rit = m_tmpredid.begin();
            m_tmpredid.erase(rit);
        }
        m_tmpredid.insert(allInfo->redid);
    }
    if (m_vectAllInfo.size() > MAX_CHAT_LENGTH) {
        VECTALLINFO::iterator it = m_vectAllInfo.begin();
        it = m_vectAllInfo.erase(it);
    }
    m_vectAllInfo.push_back(*allInfo);
    [self performSelectorOnMainThread:@selector(reloadPublicChatTable) withObject:nil waitUntilDone:NO];
}

- (void)reloadPublicChatTable
{
    if([(NSObject *)_thedelegate respondsToSelector:@selector(delegateInfo:)] == YES )
    {
        [_thedelegate delegateInfo:0];
    }
}
-(void)reloadPrivateChatTable
{
    if([(NSObject *)_thedelegate respondsToSelector:@selector(delegateInfo:)] == YES )
    {
        [_thedelegate delegateInfo:1];
    }
}
-(void)pushTmpSpeakInfo:(AllInfo*)broadInfo
{
    m_tempBroadInfo.push_back(*broadInfo);
}

-(void)pushRecvSpeakInfo:(ResultData_UserSampleInfo *)saminfo
{
    VECTALLINFO::iterator it = [NetUtilCallBack getInstance]->m_tempBroadInfo.begin();
    for (; it!=[NetUtilCallBack getInstance]->m_tempBroadInfo.end(); it++) {
        AllInfo allinfo = (AllInfo)(*it);
        if (allinfo.idx == saminfo->idx) {
            //gbk -> utf8
            char utfname[260] = {0};
            int len = sizeof(utfname);
            GbkToUtf8(saminfo->nickname, utfname, len);
            strcpy(allinfo.name, utfname);
            
            char fullhead[256] = {0};
            NSString *domin = [UserSessionManager getInstance].currentUser.domin;
            char *p = strstr(saminfo->headurl, "http://");
            if(NULL == p)
            {
                strcpy(fullhead, [domin UTF8String]);
                strncat(fullhead, saminfo->headurl, strlen(saminfo->headurl));
                strcpy(allinfo.headphoto, fullhead);
            }
            else
            {
                strcpy(allinfo.headphoto, saminfo->headurl);
            }
            if (allinfo.isSend) {
                [self pushSpeakInfo:&allinfo];
            }
            [NetUtilCallBack getInstance]->m_tempBroadInfo.erase(it);
            break;
        }
    }
}

-(void)pushSpeakInfo:(AllInfo*)allInfo
{
    if (m_reconnectTimes == 2) {
        return;
    }
    m_vectAllInfo.push_back(*allInfo);
    
    if([(NSObject *)_thedelegate respondsToSelector:@selector(delegateRedNum)] == YES )
    {
        [_thedelegate delegateRedNum];
    }
    [self performSelectorOnMainThread:@selector(reloadbroadTable) withObject:nil waitUntilDone:NO];
}

-(void)pushBroadInfo:(AllInfo *)allInfo
{
    if (m_vectAllInfo.size()>60) {
        VECTALLINFO::iterator ita = m_vectAllInfo.begin();
        m_vectAllInfo.erase(ita);
    }
    if (allInfo->type == 7) {
        char msg[2048] = {0};
        VECTGIFTINFO::iterator it = [NetUtilCallBack getInstance]->m_localLuckGiftInfo.begin();
        for (; it!=[NetUtilCallBack getInstance]->m_localLuckGiftInfo.end(); it++) {
            GiftInfo giftinfo = (GiftInfo)(*it);
            if (giftinfo.id == allInfo->gift_id) {
                sprintf(msg, "恭喜！用户%s(%d)送出%s后，得到%d倍奖金共%d金币。送出幸运礼品，礼金翻倍来！", allInfo->name, allInfo->idx, giftinfo.name, allInfo->luckmultiple, allInfo->luckmultiple*giftinfo.cash);
                strcpy(allInfo->content, msg);
                break;
            }
        }
    }
    m_vectAllInfo.push_back(*allInfo);
    
    [self performSelectorOnMainThread:@selector(reloadbroadTable) withObject:nil waitUntilDone:NO];
    if (allInfo->type == 4) {
        NSMutableDictionary *horeDic = [[NSMutableDictionary alloc]init];
        [horeDic setValue:[NSString stringWithFormat:@"%d",allInfo->idx] forKey:@"idx"];
        [horeDic setValue:[NSString stringWithUTF8String:allInfo->name] forKey:@"name"];
        [horeDic setValue:[NSString stringWithFormat:@"%d",allInfo->roomid] forKey:@"roomid"];
        [horeDic setValue:[NSString stringWithUTF8String:allInfo->roomname] forKey:@"roomname"];
        [horeDic setValue:[NSString stringWithFormat:@"%d",allInfo->time] forKey:@"time"];
        [horeDic setValue:[NSString stringWithUTF8String:allInfo->content] forKey:@"content"];
        
        [self performSelectorOnMainThread:@selector(showHore:) withObject:horeDic waitUntilDone:NO];
    }
    
}

-(void)showHore:(NSMutableDictionary*)dic
{
    if([(NSObject *)_thedelegate respondsToSelector:@selector(delegateHoreInfo:)] == YES )
    {
        [_thedelegate delegateHoreInfo:dic];
    }
}

-(void)reloadbroadTable
{
    if([(NSObject *)_thedelegate respondsToSelector:@selector(delegateInfo:)] == YES )
    {
        [_thedelegate delegateInfo:2];
    }
}

-(void)onNameChange:(int)idx name:(const char *)name
{
    LISTONLINEINFO::iterator princessit;
    for(princessit = m_listPrincessInfo.begin(); princessit != m_listPrincessInfo.end(); princessit++)
    {
        OnlineInfo onlineinfo = (OnlineInfo )(*princessit);
        if (onlineinfo.idx == idx) {
            strcpy(princessit->name, name);
            return ;
        }
    }
    
    LISTONLINEINFO::iterator roomuserit;
    for(roomuserit = m_listRoomUserInfo.begin(); roomuserit != m_listRoomUserInfo.end(); roomuserit++)
    {
        OnlineInfo onlineinfo = (OnlineInfo )(*roomuserit);
        if (onlineinfo.idx == idx) {
            strcpy(roomuserit->name, name);
            return ;
        }
    }
    
    LISTONLINEINFO::iterator lobbyit;
    for(lobbyit = m_listLobbyUserInfo.begin(); lobbyit != m_listLobbyUserInfo.end(); lobbyit++)
    {
        OnlineInfo onlineinfo = (OnlineInfo )(*lobbyit);
        if (onlineinfo.idx == idx) {
            strcpy(lobbyit->name, name);
            return ;
        }
    }
}

-(void)changehead:(int)idx :(const char*)picture
{
    //聊天过的信息头像不需要更新
//    VECTALLINFO::iterator it;
//    for(it = m_vectAllInfo.begin(); it != m_vectAllInfo.end(); it++)
//    {
//        AllInfo allinfo = (AllInfo)(*it);
//        if (allinfo.idx == idx) {
//            strcpy(it->headphoto, picture);
//            continue;
//        }
//    }
    
    LISTONLINEINFO::iterator princessit;
    for(princessit = m_listPrincessInfo.begin(); princessit != m_listPrincessInfo.end(); princessit++)
    {
        OnlineInfo onlineinfo = (OnlineInfo )(*princessit);
        if (onlineinfo.idx == idx) {
            strcpy(princessit->headphoto, picture);
            continue;
        }
    }
    
    LISTONLINEINFO::iterator roomuserit;
    for(roomuserit = m_listRoomUserInfo.begin(); roomuserit != m_listRoomUserInfo.end(); roomuserit++)
    {
        OnlineInfo onlineinfo = (OnlineInfo )(*roomuserit);
        if (onlineinfo.idx == idx) {
            strcpy(roomuserit->headphoto, picture);
            continue;
        }
    }
    
    LISTONLINEINFO::iterator lobbyit;
    for(lobbyit = m_listLobbyUserInfo.begin(); lobbyit != m_listLobbyUserInfo.end(); lobbyit++)
    {
        OnlineInfo onlineinfo = (OnlineInfo )(*lobbyit);
        if (onlineinfo.idx == idx) {
            strcpy(lobbyit->headphoto, picture);
            continue;
        }
    }
    
    [self performSelectorOnMainThread:@selector(reloadheadphoto) withObject:nil waitUntilDone:NO];
}
-(void)reloadheadphoto
{
    if (_thedelegate !=nil) {
        
        if([(NSObject *)_thedelegate respondsToSelector:@selector(delegateInfo:)] == YES )
        {
            [_thedelegate delegateInfo:4];
        }
    }
}

-(void)loginNotice
{
    m_listLobbyUserInfo.clear();
    m_listPrincessInfo.clear();
    m_listRoomUserInfo.clear();
}
-(void)closeRoom
{
    m_roomId = 0;
    m_tmpredid.clear();
    m_vectreqsonglist.clear();
    [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_CLOSEROOM_NOTIFICATION object:nil userInfo:nil];
}
-(void)noticeEndsong
{
    [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_USERINFO_UPDATE_NOTIFICATION object:nil userInfo:nil];
}
-(void)pushOnlineInfo:(OnlineInfo*)online
{
    if (m_isChanged) {
        [self changehead:online->idx :online->headphoto];
        [self isChanged];
    }
    else
    {
        VECTTMPIDX::iterator tmpit = m_tmpidx.begin();
        for (; tmpit != m_tmpidx.end(); tmpit++){
            ResultData_UIdxWithType idtype = (*tmpit);
            if (online->idx == idtype.idx) {
                if (idtype.type == '0')
                    m_listPrincessInfo.push_back(*online);
                else if(idtype.type == '1')
                    m_listRoomUserInfo.push_back(*online);
                else if(idtype.type == '2')
                {
                    LISTONLINEINFO::iterator lbit = m_listLobbyUserInfo.begin();
                    if (lbit->idx == [UserSessionManager getInstance].currentUser.uid) {
                        lbit++;
                        m_listLobbyUserInfo.insert(lbit, *online);
                    }
                    else
                        m_listLobbyUserInfo.push_front(*online);
                }
                else
                    m_listLobbyUserInfo.push_back(*online);
                
                [self performSelectorOnMainThread:@selector(reloadOnline) withObject:nil waitUntilDone:NO];
                m_tmpidx.erase(tmpit);
                break;
            }
        }
    }
}

-(void)pushOnline:(const ResultData_UIdxWithType*)list :(int)size
{
    OnlineInfo myinfo = {0};
    for(int i=0; i<size; i++)
    {
        USER_DATA_BASE userInfo;
        memset(&userInfo,0,sizeof(userInfo));
        if (GetUserInfoFromLocal(list[i].idx, &userInfo) == 1)
        {
            OnlineInfo onlineInfo = {0};
            onlineInfo.idx = list[i].idx;
            onlineInfo.type = list[i].type;
        
            //gbk -> utf8
            char utfname[260] = {0};
            int len = sizeof(utfname);
            GbkToUtf8(userInfo.user_info.base.m_name, utfname, len);
            strcpy(onlineInfo.name,utfname);//昵称
            strcpy(onlineInfo.sex,userInfo.user_info.base.m_sex);
        
            char head[256] = {0};
            NSString *domin = [UserSessionManager getInstance].currentUser.domin;
            char *p = strstr(userInfo.user_info.base.headurl, "http://");
            if(NULL == p)
            {
                strcpy(head, [domin UTF8String]);
                strncat(head, userInfo.user_info.base.headurl, strlen(userInfo.user_info.base.headurl));
                strcpy(onlineInfo.headphoto, head);
            }
            else
                strcpy(onlineInfo.headphoto, userInfo.user_info.base.headurl);
            PLog(@"%s", onlineInfo.headphoto);
            
            if (list[i].idx == [UserSessionManager getInstance].currentUser.uid) {
                myinfo = onlineInfo;
                continue;
            }
            
            if (onlineInfo.type == '0') {
                m_listPrincessInfo.push_back(onlineInfo);
            }
            else if (onlineInfo.type == '1')
            {
                m_listRoomUserInfo.push_back(onlineInfo);
            }
            else if (onlineInfo.type == '2')
            {
                LISTONLINEINFO::iterator lbit = m_listLobbyUserInfo.begin();
                if (lbit->idx == [UserSessionManager getInstance].currentUser.uid) {
                    lbit++;
                    m_listLobbyUserInfo.insert(lbit, onlineInfo);
                }
                else
                    m_listLobbyUserInfo.push_front(onlineInfo);
            }
            else
                m_listLobbyUserInfo.push_back(onlineInfo);
        }
        else
        {
            ResultData_UIdxWithType idxtype;
            idxtype.idx = list[i].idx;
            idxtype.type = list[i].type;
            m_tmpidx.push_back(idxtype);
        }
    }
    if (myinfo.idx != 0) {
        if (myinfo.type == '0') {
            m_listPrincessInfo.push_front(myinfo);
        }
        else if (myinfo.type == '1')
        {
            m_listRoomUserInfo.push_front(myinfo);
        }
        else if (myinfo.type == '2')
        {
            m_listLobbyUserInfo.push_front(myinfo);
        }
        else
            m_listLobbyUserInfo.push_front(myinfo);
    }
    [self performSelectorOnMainThread:@selector(reloadOnline) withObject:nil waitUntilDone:NO];
}

-(void)reloadOnline
{
    if([_thedelegate respondsToSelector:@selector(delegateOnlineInfo)] == YES )
    {
        [_thedelegate delegateOnlineInfo];
    }
    
    
    if([(NSObject *)_thedelegate respondsToSelector:@selector(delegateInfo:)] == YES )
    {
        [_thedelegate delegateInfo:3];
    }
}
-(void)isChanged
{
    m_isChanged = !m_isChanged;
}

-(void)deleteOnlineInfo:(int)idx
{
    LISTONLINEINFO::iterator it;
    for(it= m_listPrincessInfo.begin(); it != m_listPrincessInfo.end(); )
    {
        if(it->idx == idx)
        {
            m_listPrincessInfo.erase(it);
            [self performSelectorOnMainThread:@selector(reloadOnline) withObject:nil waitUntilDone:NO];
            return;
        }
        else
        {
            it++;
        }
    }
    
    for(it= m_listRoomUserInfo.begin(); it != m_listRoomUserInfo.end(); )
    {
        if(it->idx == idx)
        {
            m_listRoomUserInfo.erase(it);
            [self performSelectorOnMainThread:@selector(reloadOnline) withObject:nil waitUntilDone:NO];
            return;
        }
        else
        {
            it++;
        }
    }

    for(it= m_listLobbyUserInfo.begin(); it != m_listLobbyUserInfo.end(); )
    {
        if(it->idx == idx)
        {
            m_listLobbyUserInfo.erase(it);
            [self performSelectorOnMainThread:@selector(reloadOnline) withObject:nil waitUntilDone:NO];
            return;
        }
        else
        {
            it++;
        }
    }
    
}

-(void) pushNotify:(NSString*)str
{
    //加入线程保护
    AutoLock lock(lockChat);
    [m_sysTrack addObject:str];
}
-(void)pushRenderGift:(RenderGiftInfo*)rgi
{
    AutoLock lock(lockGift);
    //m_listRGI.push_back(*rgi);//保存刷礼物的信息
    
    AllInfo allinfo;
    allinfo.idx = rgi->send_idx;
    strcpy(allinfo.name, rgi->send_name);
    allinfo.toidx = rgi->recv_idx;
    strcpy(allinfo.toname, rgi->recv_name);
    allinfo.roomid = rgi->send_roomid;
    strcpy(allinfo.roomname, rgi->send_room);
    allinfo.recv_roomid = rgi->recv_roomid;
    strcpy(allinfo.recv_room, rgi->recv_room);
    allinfo.gift_id = rgi->gift_id;
    allinfo.total_number = rgi->total_number;
    allinfo.type = 3;
    GiftInfo *giftinfo = new GiftInfo();
    GetGiftInfoFromLocal(rgi->gift_id, giftinfo);
    
    
    strcpy(allinfo.unit_name,giftinfo->unit_name);
    
    char content[2048] = {0};
    sprintf(content, "%s送给%s%d%s%s",rgi->send_name, rgi->recv_name,rgi->total_number,giftinfo->unit_name,giftinfo->name);
    
    strcpy(allinfo.content,content);
    
    USER_DATA_BASE userInfo;
    memset(&userInfo, 0, sizeof(userInfo));
    if(GetUserInfoFromLocal(rgi->send_idx, &userInfo))
    {
        char head[256] = {0};
        NSString *domin = [UserSessionManager getInstance].currentUser.domin;
        char *p = strstr(userInfo.user_info.base.headurl, "http://");
        if(NULL == p)
        {
            strcpy(head, [domin UTF8String]);
            strncat(head, userInfo.user_info.base.headurl, strlen(userInfo.user_info.base.headurl));
            strcpy(allinfo.headphoto, head);
        }
        else
        {
            strcpy(allinfo.headphoto, userInfo.user_info.base.headurl);
        }
    }
    
    m_vectAllInfo.push_back(allinfo);
    [self performSelectorOnMainThread:@selector(reloadbroadTable) withObject:nil waitUntilDone:NO];
}

-(void)pushReqSongInfo:(const ResultData_SongList*)pSonglist :(int)count
{
    m_vectreqsonglist.clear();
    for (int i = 0; i < count; i++) {
        ResultData_SongList songinfo;
        songinfo.songid = pSonglist[i].songid;
        strcpy(songinfo.singername, pSonglist[i].singername);
        strcpy(songinfo.songname, pSonglist[i].songname);
        m_vectreqsonglist.push_back(songinfo);
    }
    [self performSelectorOnMainThread:@selector(reloadreqSongTable) withObject:nil waitUntilDone:NO];
}

-(void)reloadreqSongTable
{
    
}

-(void)onRecvUserIdxAndType:(const ResultData_UIdxWithType*)list size:(int)p2
{
}
-(void)onRecvUserLoginNew:(int)idx type:(char)p1
{
    if (idx == [UserSessionManager getInstance].currentUser.uid) {
        [UserSessionManager getInstance].currentUser.type = p1;
    }

    BOOL isfind = NO;
    OnlineInfo info;
    LISTONLINEINFO::iterator it = m_listPrincessInfo.begin();
    for (; it!= m_listPrincessInfo.end(); ) {
        if (it->idx == idx) {
            if (it->type == p1) {
                return;
            }
            else
            {
                info = (*it);
                isfind = YES;
                m_listPrincessInfo.erase(it);
                break;
            }
        }
        else
            it++;
    }
    
    for (it=m_listRoomUserInfo.begin(); it!= m_listRoomUserInfo.end();) {
        if (it->idx == idx) {
            if (it->type == p1) {
                return;
            }
            else
            {
                info = (*it);
                isfind = YES;
                m_listRoomUserInfo.erase(it);
                break;
            }
        }
        else
            it++;
    }

    for (it=m_listLobbyUserInfo.begin(); it!= m_listLobbyUserInfo.end(); ) {
        if (it->idx == idx) {
            if (it->type == p1) {
                return;
            }
            else
            {
                info = (*it);
                isfind = YES;
                m_listLobbyUserInfo.erase(it);
                break;
            }
        }
        else
            it++;
    }

    if (isfind) {
        info.type = p1;
        if (p1 == '0') {
            m_listPrincessInfo.push_back(info);
        }
        else if (p1 == '1')
        {
            m_listRoomUserInfo.push_back(info);
        }
        else if (p1 == '2')
        {
            LISTONLINEINFO::iterator lbit = m_listLobbyUserInfo.begin();
            if (lbit->idx == [UserSessionManager getInstance].currentUser.uid) {
                lbit++;
                m_listLobbyUserInfo.insert(lbit, info);
            }
            else
                m_listLobbyUserInfo.push_front(info);
        }
        else
            m_listLobbyUserInfo.push_back(info);
        [self performSelectorOnMainThread:@selector(reloadOnline) withObject:nil waitUntilDone:NO];
        return;
    }
    
    RequestData_IdxWithVersion* reqIdx = new RequestData_IdxWithVersion;
    if(!reqIdx)
        return ;
       
    USER_DATA_BASE userInfo;
    memset(&userInfo,0,sizeof(userInfo));
    if(0 == GetUserInfoFromLocal(idx, &userInfo))  //本地没有保存
    {
        reqIdx->id = idx;
        reqIdx->version = -1;
        ResultData_UIdxWithType idxtype;
        idxtype.idx = idx;
        idxtype.type = p1;
        m_tmpidx.push_back(idxtype);
    }
    else
    {
        if(idx == GetMyIdx())
            return;
                
        reqIdx->id = idx;
        reqIdx->version = userInfo.version;
    }
    GetUserInfoWithVersion(reqIdx, 1);
    delete reqIdx;
        
    USER_DATA_BASE userInfo2;
    memset(&userInfo2,0,sizeof(userInfo2));
    if (GetUserInfoFromLocal(idx, &userInfo2) == 1)
    {
        OnlineInfo onlineInfo = {0};
        onlineInfo.idx = idx;
        onlineInfo.type = p1;
        char utfname[260] = {0};
        int len = sizeof(utfname);
        GbkToUtf8(userInfo2.user_info.base.m_name, utfname, len);
        strcpy(onlineInfo.name,utfname);//昵称
        strcpy(onlineInfo.sex,userInfo2.user_info.base.m_sex);
            
        char head[256] = {0};
        NSString *domin = [UserSessionManager getInstance].currentUser.domin;
        char *p = strstr(userInfo2.user_info.base.headurl, "http://");
        if(NULL == p)
        {
            strcpy(head, [domin UTF8String]);
            strncat(head, userInfo2.user_info.base.headurl, strlen(userInfo2.user_info.base.headurl));
            strcpy(onlineInfo.headphoto, head);
        }
        else
            strcpy(onlineInfo.headphoto, userInfo2.user_info.base.headurl);
        if (p1 == '0')
            m_listPrincessInfo.push_back(onlineInfo);
        else if (p1 == '1')
            m_listRoomUserInfo.push_back(onlineInfo);
        else if (p1 == '2')
        {
            LISTONLINEINFO::iterator lboit = m_listLobbyUserInfo.begin();
            if (lboit->idx == [UserSessionManager getInstance].currentUser.uid) {
                lboit++;
                m_listLobbyUserInfo.insert(lboit, onlineInfo);
            }
            else
                m_listLobbyUserInfo.push_front(onlineInfo);
        }
        else
            m_listLobbyUserInfo.push_back(onlineInfo);
        
        [self performSelectorOnMainThread:@selector(reloadOnline) withObject:nil waitUntilDone:NO];
    }
}
-(void)onRecvSendHBResult:(int)ret
{
    
}
-(void)onRecvHongBao:(int)idx hbid:(const char*)p1 mark:(const char*)p2
{
    
}
-(void)reloadRedState:(NSMutableDictionary*)dic{
    if([_thedelegate respondsToSelector:@selector(delegateRedStatue:)] == YES )
    {
        [_thedelegate delegateRedStatue:dic];
    }
}

-(void)onRecvHongBaoStatus:(const char*)hbid ret:(int)p
{
    //0-可抢 1-已抢完 2-已抢过
    NSMutableDictionary *redDic = [[NSMutableDictionary alloc]init];
    [redDic setValue:[NSString stringWithFormat:@"%d",p] forKey:@"ret"];
    [redDic setValue:[NSString stringWithUTF8String:hbid] forKey:@"redid"];
    [self performSelectorOnMainThread:@selector(reloadRedState:) withObject:redDic waitUntilDone:NO];
}

-(void)onRecvVieHongBaoResult:(int)result idx:(int)p1 hbid:(const char*)p2 hbtype:(char)p3 remark:(const char*)p4 gold:(int)p5
{
    //0-没抢到  1-抢到  2-已抢过
    if (p3 == (char)0 && result == 2) {  //已抢过个人红包
        RequestHBDetail(p2);
        return;
    }
    if (p3 != (char)0 && result != 0 ) { //已抢到或抢过的非个人红包
        RequestHBDetail(p2);
        return;
    }
    
    RedInfo redinfo = {0};
    USER_DATA_BASE userInfo;
    memset(&userInfo, 0, sizeof(userInfo));
    redinfo.idx = p1;
    
    if(result != OPTION_TIMEOUT)
    {
        if(p1 == 1)//如果是系统发送的红包
        {
            strcpy(redinfo.name, "新浪KTV助手");
        }
        else
        {
            if(GetUserInfoFromLocal(p1, &userInfo))
            {
                //gbk -> utf8
                char utfname[260] = {0};
                int len = sizeof(utfname);
                if(0 == GbkToUtf8(userInfo.user_info.base.m_name, utfname, len))
                {
                    return ;
                }
                strcpy(redinfo.name, utfname);
                
                char head[256] = {0};
                NSString *domin = [UserSessionManager getInstance].currentUser.domin;
                char *p = strstr(userInfo.user_info.base.headurl, "http://");
                if(NULL == p)
                {
                    strcpy(head, [domin UTF8String]);
                    strncat(head, userInfo.user_info.base.headurl, strlen(userInfo.user_info.base.headurl));
                    strcpy(redinfo.headurl, head);
                }
                else
                {
                    strcpy(redinfo.headurl, userInfo.user_info.base.headurl);
                }
            }
        }
    }
    
    redinfo.gold = p5;
    //gbk -> utf8
    char utfmark[260] = {0};
    int marklen = sizeof(utfmark);
    if(0 == GbkToUtf8(p4, utfmark, marklen))
    {
        return ;
    }
    
    strcpy(redinfo.mark, utfmark);
    redinfo.type = p3;
    
    NSMutableDictionary *redDic = [[NSMutableDictionary alloc]init];
    [redDic setValue:[NSString stringWithFormat:@"%d",0] forKey:@"flag"];
    [redDic setValue:[NSString stringWithFormat:@"%d",redinfo.idx] forKey:@"idx"];
    [redDic setValue:[NSString stringWithUTF8String:redinfo.headurl] forKey:@"headurl"];
    [redDic setValue:[NSString stringWithUTF8String:redinfo.name] forKey:@"name"];
    [redDic setValue:[NSString stringWithFormat:@"%d",redinfo.gold] forKey:@"gold"];
    [redDic setValue:[NSString stringWithUTF8String:redinfo.mark] forKey:@"mark"];
    [redDic setValue:[NSString stringWithFormat:@"%d",redinfo.type] forKey:@"type"];
    [redDic setValue:[NSString stringWithFormat:@"%d",result] forKey:@"result"];
    [self performSelectorOnMainThread:@selector(reloadRed:) withObject:redDic waitUntilDone:NO];
}

-(void)reloadRed:(NSMutableDictionary*)dic{
    if([(NSObject *)_thedelegate respondsToSelector:@selector(delegateRedInfo:)] == YES )
    {
        [_thedelegate delegateRedInfo:dic];
    }
}

bool SortByTime( const HBGetDetail &v1, const HBGetDetail &v2)
{
    return v1.time > v2.time;//降序序排列
}

-(void)onRecvHongBaoDetail:(int)ret idx:(int)p1 hbtype:(char)p2 remark:(const char*)p3 totalgold:(int)p4 totalnumber:(int)p5 sendtime:(int)sendtime list:(const ResultData_HBDetail*)p6 number:(int)p7
{
    if (ret == 0) {
        HBSendDetail senddetail = {0};
        senddetail.sendidx = p1;
        senddetail.type = p2;
        
        //gbk -> utf8
        char utfmark[260] = {0};
        int marklen = sizeof(utfmark);
        if(0 == GbkToUtf8(p3, utfmark, marklen))
        {
            return ;
        }
        strcpy(senddetail.mark, utfmark);
        senddetail.gold = p4;
        senddetail.num = p5;
        senddetail.sendtime = sendtime;
        
        USER_DATA_BASE userInfo;
        memset(&userInfo, 0, sizeof(userInfo));
        
        if(p1 == 1)
        {
            strcpy(senddetail.name, "新浪KTV助手");
            strcpy(senddetail.headurl, "ic_system_profile");
        }
        else
        {
            if(GetUserInfoFromLocal(p1, &userInfo))
            {
                //gbk -> utf8
                char utfname[260] = {0};
                int len = sizeof(utfname);
                if(0 == GbkToUtf8(userInfo.user_info.base.m_name, utfname, len))
                {
                    return ;
                }
                strcpy(senddetail.name, utfname);
                
                char head[256] = {0};
                NSString *domin = [UserSessionManager getInstance].currentUser.domin;
                char *p = strstr(userInfo.user_info.base.headurl, "http://");
                if(NULL == p)
                {
                    strcpy(head, [domin UTF8String]);
                    strncat(head, userInfo.user_info.base.headurl, strlen(userInfo.user_info.base.headurl));
                    strcpy(senddetail.headurl, head);
                }
                else
                {
                    strcpy(senddetail.headurl, userInfo.user_info.base.headurl);
                }
            }
        }
        
        [NetUtilCallBack getInstance]->m_hbdetail.clear();
        int getgold = 0;
        for(int i=0; i<p7; i++)
        {
            HBGetDetail getdetail;
            USER_DATA_BASE getUserInfo;
            if (p6[i].idx == [UserSessionManager getInstance].currentUser.uid) {
                getgold = p6[i].gold;
            }
            getdetail.idx = p6[i].idx;
            getdetail.gold = p6[i].gold;
            getdetail.time = p6[i].time;
            memset(&getUserInfo, 0, sizeof(getUserInfo));
            if(GetUserInfoFromLocal(p6[i].idx, &getUserInfo))
            {
                //gbk -> utf8
                char utfname[260] = {0};
                int len = sizeof(utfname);
                if(0 == GbkToUtf8(getUserInfo.user_info.base.m_name, utfname, len))
                {
                    return ;
                }
                strcpy(getdetail.name, utfname);
                
                char head[256] = {0};
                NSString *domin = [UserSessionManager getInstance].currentUser.domin;
                char *p = strstr(getUserInfo.user_info.base.headurl, "http://");
                if(NULL == p)
                {
                    strcpy(head, [domin UTF8String]);
                    strncat(head, getUserInfo.user_info.base.headurl, strlen(getUserInfo.user_info.base.headurl));
                    strcpy(getdetail.headurl, head);
                }
                else
                {
                    strcpy(getdetail.headurl, getUserInfo.user_info.base.headurl);
                }
            }
            getdetail.isStar = NO;
            [NetUtilCallBack getInstance]->m_hbdetail.push_back(getdetail);
        }
        std::sort(m_hbdetail.begin(),m_hbdetail.end(),SortByTime);
        
        NSMutableDictionary *redDic = [[NSMutableDictionary alloc]init];
        [redDic setValue:[NSString stringWithFormat:@"%d",1] forKey:@"flag"];
        [redDic setValue:[NSString stringWithFormat:@"%d",senddetail.sendidx] forKey:@"idx"];
        [redDic setValue:[NSString stringWithFormat:@"%d",senddetail.sendtime] forKey:@"time"];
        [redDic setValue:[NSString stringWithUTF8String:senddetail.headurl] forKey:@"headurl"];
        [redDic setValue:[NSString stringWithUTF8String:senddetail.name] forKey:@"name"];
        [redDic setValue:[NSString stringWithFormat:@"%d",senddetail.gold] forKey:@"gold"];
        [redDic setValue:[NSString stringWithUTF8String:senddetail.mark] forKey:@"mark"];
        [redDic setValue:[NSString stringWithFormat:@"%d",senddetail.type] forKey:@"type"];
        [redDic setValue:[NSString stringWithFormat:@"%d",senddetail.num] forKey:@"num"];
        [redDic setValue:[NSString stringWithFormat:@"%d",p7] forKey:@"getnum"];
        [redDic setValue:[NSString stringWithFormat:@"%d",getgold] forKey:@"getgold"];
        [redDic setValue:[NSString stringWithFormat:@"%d",senddetail.sendtime] forKey:@"sendtime"];
        
        [self performSelectorOnMainThread:@selector(reloadRed:) withObject:redDic waitUntilDone:NO];
    }
    else
    {
        NSMutableDictionary *redDic = [[NSMutableDictionary alloc]init];
        [redDic setValue:[NSString stringWithFormat:@"%d",1] forKey:@"flag"];
        //result == 9999
        [redDic setValue:[NSString stringWithFormat:@"%d",ret] forKey:@"result"];
        [redDic setValue:[NSString stringWithFormat:@"%d",p1] forKey:@"idx"];
        [redDic setValue:[NSString stringWithFormat:@"%d",p2] forKey:@"type"];
        
        [self performSelectorOnMainThread:@selector(reloadRed:) withObject:redDic waitUntilDone:NO];
    }
}

-(void)onRecvSingSongChange:(const char*)songname singer:(const char*)p1 score:(int)p2
{
    strcpy(self->m_myInfo.fullInfo.singername,p1);
    strcpy(self->m_myInfo.fullInfo.songname, songname);
    self->m_myInfo.fullInfo.score = p2;
}
-(void)onRecvRoomHB:(int) number list:(const ResultData_RoomHB*)list
{
    if (number <= 0) {
        return;
    }
    for(int i = 0; i<number; i++)
    {
        SETREDID::iterator it = m_tmpredid.find(list[i].hbid);
        if (it != m_tmpredid.end()) {
            continue;
        }
    
        AllInfo allInfo ={0};
        allInfo.type = 8;
        allInfo.redtype = list[i].hbtype;
        strcpy(allInfo.redid, list[i].hbid);
        //gbk -> utf8
        char utfmark[260] = {0};
        int marklen = sizeof(utfmark);
        if(0 == GbkToUtf8(list[i].mark, utfmark, marklen))
        {
            return ;
        }
        strcpy(allInfo.content, utfmark);
        if (list[i].idx == 1) {
            allInfo.idx = 1;
            strcpy(allInfo.name, "新浪ktv助手");
            strcpy(allInfo.headphoto, "ic_system_profile");
        }

        m_tmpredid.insert(list[i].hbid);
        
        if (m_vectAllInfo.size() > MAX_CHAT_LENGTH) {
            VECTALLINFO::iterator it = m_vectAllInfo.begin();
            
            if(it->type == 8)
            {
                std::string tmpHbId = it->redid;
                if(m_tmpredid.find(tmpHbId) != m_tmpredid.end())
                {
                    m_tmpredid.erase(tmpHbId);
                }
            }
            m_vectAllInfo.erase(it);
        }
        m_vectAllInfo.push_back(allInfo);
        
        if([(NSObject *)_thedelegate respondsToSelector:@selector(delegateRedNum)] == YES )
        {
            [_thedelegate delegateRedNum];
        }
    }
    [self performSelectorOnMainThread:@selector(reloadPublicChatTable) withObject:nil waitUntilDone:NO];
}

#pragma mark -
#pragma mark Callback Method
-(void)OnAuthTunnelConnectSuccess
{
    PLog(@"认证连接成功!");
    User *user = [[UserSessionManager getInstance] currentUser];
    const char* md5Password = [NetUtilCallBack md5:user.pwd.UTF8String];
    int again = mCurrentTransaction == TransactionAuthAgain?1:0;
    if(IdPassAuth(user.xgid.UTF8String,md5Password,again)!=0)
        PLog(@"认证请求失败!");
    else
        PLog(@"认证身份...");
}
-(void)OnAuthTunnelConnectTimeout
{
    PLog(@"认证超时!");
    //[self sendLoginLobbyResult:0];
    [self onTransactionFinish:NO reconnect:YES];
}
-(void)OnAuthTunnelConnectError
{
    PLog(@"认证服务器连接出错,errorcode:%d!", m_errorCode);
    [self onTransactionFinish:NO reconnect:YES];
}
-(void)OnAuthTunnelClose
{
    PLog(@"服务器断开连接!");
    [self onTransactionFinish:NO reconnect:NO];
}
-(void)OnAuthTunnelError
{
    PLog(@"认证服务器网络出错,errorcode:%d!", m_errorCode);
    [self onTransactionFinish:NO reconnect:NO];
    [self sendLoginLobbyResult:0];
}

-(void)OnLobbyTunnelConnectSuccess
{
    PLog(@"连接大厅成功");
    if(0 != GetChannellist(XG_VERSION,m_cunicode))
    {
        PLog(@"获取大厅列表失败");
        [self onTransactionFinish:NO reconnect:YES];
    }
    PLog(@"获取大厅列表中...");
}

-(void)OnChannellistTunnelConnectSuccess
{
    //无
}

-(void)OnLobbyTunnelConnectTimeout
{
    m_bConnectedLobby = false;
    m_bConnectedRoom = false;
    
    m_listLobbyUserInfo.clear();
    m_listPrincessInfo.clear();
    m_listRoomUserInfo.clear();
    m_reconnectTimes = 2;
    
    [self onTransactionFinish:NO reconnect:YES];
}

-(void)OnLobbyTunnelConnectError
{
    m_bConnectedLobby = false;
    m_bConnectedRoom = false;
    PLog(@"大厅服务器连接出错,errorcode:%d!", m_errorCode);
    [self onTransactionFinish:NO reconnect:YES];
}

-(void)OnLobbyTunnelClose
{
    m_bConnectedLobby = false;
    m_bConnectedRoom = false;
    PLog(@"大厅服务器主动断开");
    
    [[iToast makeText:@"账号在其他地方登录或协议异常"] show];
    //回到登录界面
    UserLoginController *loginController = [[UserLoginController alloc] init];
    UINavigationController *naviMain = [[BaseNavigationViewController alloc] initWithRootViewController:loginController];
    loginController.appDelegate.window.rootViewController = naviMain;
    DisConnectRoom();
    DisConnectLobby();
    m_listLobbyUserInfo.clear();
    m_listPrincessInfo.clear();
    m_listRoomUserInfo.clear();
    m_vectAllInfo.clear();
    m_vectreqsonglist.clear();
    
    if(![[Reachability reachabilityForInternetConnection] isReachable])
        return;
    //调用下web接口，退出web登录状态，其实只是为了统计数据，可能以后不需要了
    long int userID = [UserSessionManager getInstance].currentUser.uid;
    NSString *roomId = [UserSessionManager getInstance].currentRoomInfo.roomId;
    NSString *encodeUrl = [PKtvAssistantAPI getUserLogoutUrl:userID roomId:roomId];
    NSURL *url = [NSURL URLWithString:encodeUrl];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    //不管成功与否，我们都不管了。
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject)
     {
     }
     failure:^(AFHTTPRequestOperation *operation, NSError *error)
     {
     }];
    [operation start];
}

-(void)OnLobbyTunnelError
{
    m_bConnectedLobby = false;
    m_bConnectedRoom = false;
    PLog(@"大厅服务器网络出错,errorcode:%d!", m_errorCode);
    
    m_listLobbyUserInfo.clear();
    m_listPrincessInfo.clear();
    m_listRoomUserInfo.clear();
    m_reconnectTimes = 2;
    
    if (mCurrentTransaction == 0)
    {
        PLog(@"大厅重连..");
        [self doCurrentWork:TransactionLobbyAgain];
    }
    else
    {
        [self onTransactionFinish:NO reconnect:YES];
    }
}

-(void)OnRoomTunnelConnectSuccess
{
    PLog(@"房间连接成功");
    m_bConnectedRoom = true;
    if(ConfirmToken() == -2)//已经token,直接进入房间
    {
        [self onEnterRoom];
    }
}
-(void)OnRoomTunnelConnectTimeout
{
    m_bConnectedRoom = false;
    [self onTransactionFinish:NO reconnect:YES];
}

-(void)OnRoomTunnelConnectError
{
    m_bConnectedRoom = false;
    PLog(@"房间服务器连接出错,errorcode:%d!", m_errorCode);
    [self onTransactionFinish:NO reconnect:YES];
}
-(void)OnRoomTunnelClose
{
    m_bConnectedRoom = false;
    m_roomId = 0;
    m_vectreqsonglist.clear();
    PLog(@"房间服务器主动断开");
    
    //退出包厢，状态
    [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_CLOSEROOM_NOTIFICATION object:nil userInfo:nil];
}
-(void)OnRoomTunnelError
{
    m_bConnectedRoom = false;
    PLog(@"房间服务器网络出错,errorcode:%d!", m_errorCode);
    
    if(mCurrentTransaction == 0)
    {
        PLog(@"房间重连...");
        [self doCurrentWork:TransactionRoomLogin];
    }
    else
    {
        [self onTransactionFinish:NO reconnect:YES];
    }
}

-(void)RecvAuth
{
    PLog(@"认证结果返回!\n");
    DisConnectAuth();
    
    bool bOk = [NetUtilCallBack onAuthError:m_errorCode];
    if(bOk)
    {
        if(mCurrentTransaction == TransactionAuthAgain)
        {
            //事务处理完毕
            [self onTransactionFinish:YES reconnect:NO];
        }
        else
        {
            if(ConnectLobby(XG_LOBBYHOST, (short)XG_LOBBYPORT, 60) != 0)
            {
                PLog(@"连接大厅失败!");
                [self onTransactionFinish:NO reconnect:YES];
            }
            else
                PLog(@"连接大厅中...");
        }
    }
    else
    {
        if(m_errorCode == OPTION_TIMEOUT)
        {
            [self onTransactionFinish:NO reconnect:YES];
        }
        else
        {
            [self onTransactionFinish:NO reconnect:NO];
            [self sendLoginLobbyResult:0];
        }
    }
}
-(void)RecvSectionList
{
    bool bOk = [NetUtilCallBack onLobbyError:m_errorCode];
    if(bOk)
    {
        PLog(@"大厅列表获取成功");
        
        //登录指定大厅，星光早期版本支持选择大厅登录
        if(m_channellist->count == 1 && m_channellist->pCh[0].sectionid == XG_LOBBY_ID)
        {
            PLog(@"正在进入大厅...");
            //登陆大厅
            LoginLobby(m_cunicode,XG_DEVICE);
            
        }
    }
    else
    {
        if(m_errorCode == OPTION_TIMEOUT)
            [self onTransactionFinish:NO reconnect:YES];
        else
        {
            [self onTransactionFinish:NO reconnect:NO];
            [self sendLoginLobbyResult:0];
        }
        
    }
}

-(void)RecvTokenAuth4Lobby
{
    bool bOk = [NetUtilCallBack onLobbyError:m_errorCode];
    if(bOk)
    {
        PLog(@"进入大厅中...");
    }
    else
    {
        if(m_errorCode == OPTION_TIMEOUT)
            [self onTransactionFinish:NO reconnect:YES];
        else
        {
            [self onTransactionFinish:NO reconnect:NO];
            [self sendLoginLobbyResult:0];
        }
    }
}

-(void)OnLoginLobby
{
    bool bOk = [NetUtilCallBack onLobbyError:m_errorCode];
    if(bOk)
    {
        PLog(@"大厅登录成功");
        m_bConnectedLobby = true;
        ResultData_SelfData rd_sd = {0};
        GetSelfData(&rd_sd);
        
        GetUserSpeakerInfo();
        
        [self sendLoginLobbyResult:1];
        if (m_roomId != 0) {
            //如果房间有保存，则还需要进行房间连接的事务
            [self beginRoomTransaction];
        }
        else
        {
            //事务完成
            [self onTransactionFinish:YES reconnect:NO];
        }
    }
    else
    {
        if(m_errorCode == OPTION_TIMEOUT)
            [self onTransactionFinish:NO reconnect:YES];
        else
        {
            [self onTransactionFinish:NO reconnect:NO];
            [self sendLoginLobbyResult:0];
        }
    }
}
-(void)RecvTokenAuth4Room
{
    if(m_errorCode == 0 || m_errorCode == ROOM_NORMAL)
    {
        PLog(@"房间认证通过");
        [self onEnterRoom];
    }
    else
    {
        DisConnectRoom();
        if(m_ret == -1)
        {
            if(m_errorCode == OPTION_TIMEOUT)
            {
                [self onTransactionFinish:NO reconnect:YES];
            }
            else// m_errorCode = ROOM_TOKEN_ERROR
            {
                [self resetCurrentWork];
                [self doCurrentWork:TransactionAuthAgain listener:^(BOOL success) {
                    if(success)//如果重新取得令牌，则开始执行登录房间的事务
                    {
                        [self doCurrentWork:TransactionRoomLogin];
                    }
                }];
            }
        }
        else if(m_ret == -2)
        {
            //不在大厅，需要重新连接大厅才能进房间
            mCurrentTransaction = TransactionLobbyAgain;
            [self onTransactionFinish:NO reconnect:YES];
        }
    }
    
}
-(void)RecvEnterRoom
{
    bool bOk = [self onRoomError:m_errorCode];
    if(bOk)
    {
        if(m_clientData == m_roomId)
        {
            PLog(@"进入房间成功");
            [self onTransactionFinish:YES reconnect:NO];
        }
        else
        {
            //一般不会出现这种情况
            //需要后续人员继续开发，
            //断开当前房间，重新进入房间
        }
    }
    else
    {
        if(m_errorCode == OPTION_TIMEOUT)
            [self onTransactionFinish:NO reconnect:YES];
        else
        {
            //提示用户
            [self onTransactionFinish:NO reconnect:NO];
        }
    }
}

-(void)RecvLeaveRoom
{
    PLog(@"退出房间成功");
    DisConnectRoom();
    m_clientData = 0;
    m_roomId = 0;
    [self performSelectorOnMainThread:@selector(leaveRoom) withObject:nil waitUntilDone:NO];
}

- (void)leaveRoom
{
    if([(NSObject *)_thedelegate respondsToSelector:@selector(delegateLeaveRoom)] == YES )
    {
        [_thedelegate delegateLeaveRoom];
    }
}

-(void)CheckGiftSend
{
    switch (m_errorCode) {
        case ROOM_NORMAL:
        {
            PLog(@"送礼成功");
            //[self start];
            break;
        }
        case ROOM_ERROR_GIFT_BALANCE:		//送礼时余额不足
        {
            //tip.text = @"送礼时余额不足";
            break;
        }
        case ROOM_ERROR_GIFT_UNKOWN:		//交易金额为空
        {
            //tip.text = @"交易金额为空";
            break;
        }
        case ROOM_ERROR_GIFT_NOTEXIST:		//该礼物不存在
        {
            //tip.text = @"该礼物不存在";
            break;
        }
        case ROOM_ERROR_GIFT_OFFLINE:		//礼物接收者不在房间内
        {
            //tip.text = @"礼物接收者不在线";
            break;
        }
    }
}

-(void)UpdateMoney
{
    ResultData_SelfData rd_sd = {0};
    if(GetSelfData(&rd_sd))
    {
        [UserSessionManager getInstance].currentUser.gold = rd_sd.gold;
        NSString *strgold = [NSString stringWithFormat:@"%d",rd_sd.gold];
        
        NSDictionary * userInfo = [NSDictionary dictionaryWithObjectsAndKeys:strgold,@"gold", nil];
        
        [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_GOLD_UPDATE_NOTIFICATION object:nil userInfo:userInfo];
        [[UserSessionManager getInstance] updateCurrentUserGold:rd_sd.gold];
    }
}

-(void)InformRenderGift
{
    /*[self getTimerInterval];
    //启动定时器，定时刷礼物信息，具体参数参见
    if(m_timerGift == nil)
    {
        [self getTimerInterval];
        m_timerGift = [NSTimer scheduledTimerWithTimeInterval:m_timeIntervalGift target:self selector:@selector(onGiftRender:) userInfo:nil repeats:YES];
    }*/
}
-(void)RecvSpeakerFromLobby
{
    
}

void Qsort(VECTGIFTINFO& a,int low,int high)
{
    if(low>=high || a.empty())
        return;
    int first=low;
    int last=high;
    GiftInfo key=a[first];/*用字表的第一个记录作为枢轴*/
    while(first<last)
    {
        while(first<last&&a[last].order>=key.order)
         --last;
        a[first]=a[last];/*将比第一个小的移到低端*/
        while(first<last&&a[first].order<=key.order)
           ++first;
        a[last]=a[first];/*将比第一个大的移到高端*/
    }
    a[first]=key;/*枢轴记录到位*/
    Qsort(a,low,first-1);
    Qsort(a,first+1,high);
}

-(void)InitGiftImg
{
    m_localCommonGiftInfo.clear();
    m_localLuckGiftInfo.clear();
    m_localLuxuryGiftInfo.clear();
    
    int nNum = 100;
    //GetAllGiftId(NULL, nNum);
    int* pId = NULL;
    if(nNum > 0)
        pId = new int[nNum];
    if(!pId)
    {
        printf("OOM\n");
        exit(-1);
    }
    if (GetAllGiftId(pId, nNum)) {
        for (int i =0; i< nNum; i++) {
            GiftInfo info = {0};
            if(GetGiftInfoFromLocal(pId[i], &info)>0)
            {
                if (info.gift_type == 1)
                    m_localLuckGiftInfo.push_back(info);
                else if (info.gift_type == 2)
                    m_localCommonGiftInfo.push_back(info);
                else
                    m_localLuxuryGiftInfo.push_back(info);
            }
        }
    }
    delete pId;
    
    Qsort(m_localCommonGiftInfo,0,m_localCommonGiftInfo.size()-1);
    Qsort(m_localLuckGiftInfo,0,m_localLuckGiftInfo.size()-1);
    Qsort(m_localLuxuryGiftInfo,0,m_localLuxuryGiftInfo.size()-1);
    
}

@end
