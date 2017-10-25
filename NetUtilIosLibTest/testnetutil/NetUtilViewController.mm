//
//  NetUtilViewController.m
//  testnetutil
//
//  Created by guliping_imac on 14-4-21.
//  Copyright (c) 2014年 天格科技. All rights reserved.
//

#import "NetUtilViewController.h"
#include "CResponse.h"
#include "../include/NetUtil/com_ios_NetUtil.h"
#import <Foundation/NSThread.h>
#import <CommonCrypto/CommonDigest.h>
#include "xingguang.h"
#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/NSUserDefaults.h>
#include <UIKit/UIDevice.h>
#import <QuartzCore/QuartzCore.h>
#include "AutoLock.h"
#include "ResStrings.h"

@implementation NetUtilViewController

@synthesize tip,nameText,passwdText,oThread,chatText;
@synthesize m_chatTable,m_chatCell;
@synthesize m_goldLabel,m_trackLabel;
@synthesize m_lockChat = lockChat;
@synthesize m_lockGift = lockGift,m_lockShow=lockShow;
@synthesize m_sysTrack;
@synthesize m_timerGift;
@synthesize m_timeIntervalGift;
@synthesize m_imgView;

- (void)newThread:(id)sender
{
    NSLog(@"new Thread, This is in new thread\n");
    //showLabel.text = [NSString stringWithFormat:@"Hello, from butn."];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    //oThread = [[NSThread alloc] initWithTarget:self selector:@selector(newThread:) object:nil];
    //[oThread start];
    //[oThread release];
    //以下方法每次启动pGlp都是为空，所以不适用本程序
    /*NSString* pGlp = [[NSUserDefaults standardUserDefaults] stringForKey:@"glp"];
    
    NSString *  result;
    CFUUIDRef   uuid;
    CFStringRef uuidStr;
    
    uuid = CFUUIDCreate(NULL);
    assert(uuid != NULL);
    
    uuidStr = CFUUIDCreateString(NULL, uuid);
    assert(uuidStr != NULL);
    
    //每次生成的Guid都不一样
    result = [NSString stringWithFormat:@"%@", uuidStr];
    assert(result != nil);
    
    CFRelease(uuidStr);
    CFRelease(uuid);
    
    NSDictionary* dic = [[NSDictionary alloc] initWithObjectsAndKeys:result,@"glp", nil];
    
    [[NSUserDefaults standardUserDefaults] registerDefaults:dic];
    
    pGlp = [[NSUserDefaults standardUserDefaults] stringForKey:@"glp"];*/
    
    NSString* unicode = [[UIDevice currentDevice] uniqueIdentifier];
    memcpy(m_cunicode,unicode.UTF8String,strlen(unicode.UTF8String));
    //strcpy(m_cunicode, "40:6c:8f:4f:f4:76");
    m_channellist = NULL;
    m_response = NULL;
    memset(&m_myInfo, 0, sizeof(m_myInfo));
    m_bWangtong = false;
    m_bConnectedLobby = false;
    //m_roomId = 0; //外部必须设置
    m_errorCode = 0;
    m_clientData = 0;
    
    m_response = CResponse::getResponseSingleton();
    m_response->initView(self);
    SetResponseHandle(m_response);
    StartClient();
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *docDir = [paths objectAtIndex:0];
    
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
    
    //NSString* file1 = [[NSBundle mainBundle] pathForResource:@"forbiddenword" ofType:@"dog"];
    //NSString* file2 = [[NSBundle mainBundle] pathForResource:@"forbiddenword2" ofType:@"dog"];
    bool b = MFInitFilter();
	// Do any additional setup after loading the view, typically from a nib.
    m_fSpeed = 40.0f;
    m_running = NO;
    m_vectChat.clear();
    m_listRGI.clear();
    m_vectShowInfo.clear();
    m_sysTrack = [[NSMutableArray alloc] initWithCapacity:0];
    m_totalTimes = 0;
    m_curTimes = 0;
    
    [self pushNotify:@"测试kkkkkkkkk"];
    [self start];
}

- (void)viewDidUnload
{
    StopClient();
    
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    self.m_chatCell = nil;
    self.m_chatTable = nil;
    if(m_timerGift)
        [m_timerGift invalidate];
    m_timerGift = nil;
}

- (void)dealloc
{
    [super dealloc];
    [m_chatCell dealloc];
    [m_chatTable dealloc];
    [m_sysTrack dealloc];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
}

-(IBAction)inpudown:(id)sender
{
    [sender resignFirstResponder];
}

-(IBAction)backgroundTab:(id)sender
{
    [nameText resignFirstResponder];
    [passwdText resignFirstResponder];
    [chatText resignFirstResponder];
}

-(IBAction)buttonLogin:(id)sender
{
    tip.text = @"连接认证...";
    m_bWangtong = !!IsWangtong(XG_AUTHHOST, XG_WANGTONG);
    if(ConnectAuth(XG_AUTHHOST, (short)XG_AUTHPORT) != 0)
        tip.text = @"连接认证失败!";
    //else
        
}

-(IBAction)buttonPublic:(id)sender
{
//    ShowInfo info = {0};
//    strcpy(info.people, "wo");
//    static int d;
//    d++;
//    sprintf(info.content, "xxxxxx %d",d);
//    m_vectShowInfo.push_back(info);
//    [self OnReloadTable];
    
    
    if (!m_bConnectedLobby) {
        tip.text = @"请先登录，再聊天";
        return ;
    }
    
    if(strlen(chatText.text.UTF8String) == 0)
    {
        tip.text = @"内容为空!";
        return ; 
    }
    
    char utf8[1024] = {0};
    int len = strlen(chatText.text.UTF8String);
    memcpy(utf8,chatText.text.UTF8String,len);
    
    MFFilter(utf8, len);
    
    if(0 == SendChatKTV(0, utf8))
        tip.text = @"发送成功";
    else
        tip.text = @"发送失败";
}

-(IBAction)buttonPrivate:(id)sender
{
    if (!m_bConnectedLobby) {
        tip.text = @"请先登录，再聊天";
        return ;
    }
    
    if(strlen(chatText.text.UTF8String) == 0)
    {
        tip.text = @"内容为空!";
        return ;
    }
    
    char gbk[1024] = {0};
    int len = sizeof(gbk);
    if(Utf8ToGbk(chatText.text.UTF8String, gbk, len))
    {
        //指定与88888801私聊
        if(0 == SendChatKTV(88888801, gbk))
            tip.text = @"发送成功";
        else
            tip.text = @"发送失败";
    }
    else
        printf("Utf8 to Gbk failed\n");
}
-(IBAction)buttonSend:(id)sender
{
    if (!m_bConnectedLobby) {
        tip.text = @"请先登录，再送礼";
        return ;
    }
    
    SendGiftInLobby(88888801/*GetMyIdx()*/, 100001, 10, DEBUG_ROOMID);
    printf("Send Gift to Lobby\n");
}
-(IBAction)buttonHorn:(id)sender
{
    if (!m_bConnectedLobby)
    {
        tip.text = @"请先登录，再发喇叭";
        return ;
    }
    if(strlen(chatText.text.UTF8String) == 0)
    {
        tip.text = @"内容为空!";
        return ;
    }
    
    char gbk[1024] = {0};
    int len = sizeof(gbk);
    if(Utf8ToGbk(chatText.text.UTF8String, gbk, len))
    {
        SendSpeakerToLobby(XG_LOBBY_ID, gbk, strlen(gbk));
        printf("Send Horn to Lobby\n");
    }
    else
        printf("Utf8 to Gbk failed\n");
}
#pragma mark -
#pragma mark ANIMATION
-(void)animateCurrentTickerString
{
	NSString *currentString = [m_sysTrack objectAtIndex:0];
    
    if(currentString == nil || [currentString isEqualToString:@""])
        return ;
	
	// Calculate the size of the text and update the frame size of the ticker label
    
	CGSize textSize = [currentString sizeWithFont:[m_trackLabel font]  constrainedToSize:CGSizeMake(9999, m_trackLabel.frame.size.height) lineBreakMode:UILineBreakModeWordWrap];
    
    // Setup some starting and end points
	float startingX = 0.0f;
    float endX = 0.0f;

    float widthScreen = [[UIScreen mainScreen] bounds].size.width;
    startingX = widthScreen;
    endX = -textSize.width;
    
	// Set starting position
	[m_trackLabel setFrame:CGRectMake(startingX, m_trackLabel.frame.origin.y, textSize.width, textSize.height)];
	
	// Set the string
	[m_trackLabel setText:currentString];
    
    if(m_fSpeed < 0.001f)
        return ;
	// Calculate a uniform duration for the item
	float duration = (textSize.width + widthScreen) / m_fSpeed;
	
	// Create a UIView animation
	[UIView beginAnimations:@"move" context:nil];
	[UIView setAnimationCurve:UIViewAnimationCurveLinear];
	[UIView setAnimationDuration:duration];
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(tickerMoveAnimationDidStop:finished:context:)];
    
	// Update end position
    CGRect tickerFrame = m_trackLabel.frame;
    tickerFrame.origin.x = endX;
    [m_trackLabel setFrame:tickerFrame];
    
	[UIView commitAnimations];
}

-(void) popNotify
{
    AutoLock lock(lockChat);
    if([m_sysTrack count] >= 2) {
        [m_sysTrack removeObjectAtIndex:0];
    }
}
-(BOOL) checkArrayNotify
{
    AutoLock lock(lockChat);
    if([m_sysTrack count] >= 1) {
        [lockChat unlock];
        return YES;//保持一直飘动
    }
    return NO;
}

-(void)tickerMoveAnimationDidStop:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context
{
    if([animationID isEqualToString:@"move"])
    {
        [self popNotify];
        m_running = [self checkArrayNotify];
        if (m_running) {
            // Animate
            [self animateCurrentTickerString];
        }
    }
}
#pragma mark - Ticker Animation Handling
-(void)start {
	
    if (m_running) {
        return ;
    }
	// Set m_m_running
	m_running = [self checkArrayNotify];
	
    if (m_running) {
        // Start the animation
        [self animateCurrentTickerString];
    }
}

-(void)pause {
	
	// Check if m_m_running
	if(m_running) {
		// Pause the layer
		//[m_trackLabel pauseLayer:m_trackLabel.layer];
        m_fOldSpeed = m_fSpeed;
		m_fSpeed = 0.0f;
        
		m_running = NO;
	}
}

-(void)resume {
    
	// Check not m_running
	if(!m_running) {
		// Resume the layer
		//[m_trackLabel resumeLayer:m_trackLabel.layer];
		m_fSpeed = m_fOldSpeed;
		m_running = YES;
	}
}
#pragma mark - UIView layer animations utilities
-(void)pauseLayer:(CALayer *)layer
{
    CFTimeInterval pausedTime = [layer convertTime:CACurrentMediaTime() fromLayer:nil];
    layer.speed = 0.0;
    layer.timeOffset = pausedTime;
}

-(void)resumeLayer:(CALayer *)layer
{
    CFTimeInterval pausedTime = [layer timeOffset];
    layer.speed = 1.0;
    layer.timeOffset = 0.0;
    layer.beginTime = 0.0;
    CFTimeInterval timeSincePause = [layer convertTime:CACurrentMediaTime() fromLayer:nil] - pausedTime;
    layer.beginTime = timeSincePause;
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
-(BOOL)getTimerInterval
{
    AutoLock lock(lockGift);
    if(m_listRGI.empty())
        return NO;
    RenderGiftInfo& info = m_listRGI.front();
    m_timeIntervalGift = info.time_span / 1000.0f;
    int remain = (info.total_number%info.number_once)?1:0;
    m_totalTimes = info.total_number / info.number_once+remain;
    return YES;
}
-(void)onPushShowInfo
{
    ShowInfo si;
    {
        AutoLock lock(lockGift);
        if(m_listRGI.empty())
            return ;
        RenderGiftInfo& info = m_listRGI.front();
        
        
        char room[260];
        if (info.send_roomid == 0)
            sprintf(room, "——%s",STR_NOTINROOM);
        else
            sprintf(room, "%s%s%s",STR_IN,info.send_room,STR_KTVROOM);
        sprintf(si.people, "%s%s %s %s",info.send_name,room,STR_SENDTO,info.recv_name);
        GiftInfo gi = {0};
        if(GetGiftInfoFromLocal(info.gift_id, &gi))
            sprintf(si.content, "%d%s%s(%s%d%s)",info.number_once,gi.unit_name,gi.name
                    ,STR_TOTAL,info.total_number,gi.unit_name);
        else
            return ;
    }
    AutoLock lock(lockShow);
    m_vectShowInfo.push_back(si);
    
    [self OnReloadTable];
}
-(void)onGiftRender:(id)timer
{

    m_curTimes ++;
    [self performSelectorOnMainThread:@selector(onPushShowInfo) withObject:nil waitUntilDone:NO];
    if (m_curTimes > m_totalTimes) {
        [m_timerGift invalidate];
        m_timerGift = nil;
        m_curTimes = 0;
        m_totalTimes = -1;
        
        {
            AutoLock lock(lockGift);
            if(m_listRGI.empty())
                return ;
            m_listRGI.pop_front();
        }
        if([self getTimerInterval])
            m_timerGift = [NSTimer scheduledTimerWithTimeInterval:m_timeIntervalGift target:self selector:@selector(onGiftRender:) userInfo:nil repeats:YES];
        return ;
    }
}
-(void)setRoomId:(int)roomId
{
    m_roomId = roomId;
}

-(bool)onAuthError:(int) e
{
    switch (e) {
        case 0:
        case AUTH_NORMAL:
            tip.text = @"恭喜您,登录成功!";
            return true;
        case AUTH_NOTEXIST:
            tip.text = @"sorry,帐号不存在!";
            break;
        case AUTH_PWDERROR:
            tip.text = @"sorry,登陆密码错误!";
            break;
        case AUTH_TOURISTERROR:
            tip.text = @"sorry,游客登录失败!";
            break;
        case AUTH_CACHEERROR:
            tip.text = @"sorry,登录不成功!";
            break;
        case AUTH_TOKENERROR:
            tip.text = @"sorry,取得token失败!";
            break;
            
        default:
            break;
    }
    
    return false;
}

-(bool)onLobbyError:(int) e
{
    switch(e)
    {
        case 0:
        case LOBBY_NORMAL:
            return true;
        case LOBBY_LOGIN_IDLOCK:
            tip.text = @"账号被冻结";
            DisConnectLobby();
            break;
        case LOBBY_LOGIN_TOOOLD:
            tip.text = @"版本错误";
            DisConnectLobby();
            break;
        case LOBBY_LOGIN_NOTEXIST:
            tip.text = @"用户不存在";
            DisConnectLobby();
            break;
        case LOBBY_LOGIN_NOAVATAR://认为登陆成功
            tip.text = @"还未创建avatar信息";
            break;
        case LOBBY_USERINFO_NOTEXIST:
            tip.text = @"用户信息不存在";
            DisConnectLobby();
            break;
        case LOBBY_USERINFO_INDULGE://不理防沉迷信息
            //tip.text = @"用户防沉迷中"
            return true;
    }
    
    return false;
}

-(bool)onRoomError:(int)e
{
    switch (e) {
        case 0:
        case ROOM_NORMAL://正常
            return true;
        case ROOM_TOKEN_ERROR:
            tip.text = @"token过程异常";
            break;
        case ROOM_LOGIN_NONE:
            tip.text = @"未登录";
            break;
        case ROOM_LOGIN_SUCCESS:
            tip.text = @"登录成功";
            return true;
        case ROOM_LOGIN_AGAIN:
            tip.text = @"重复登录";
            break;
        case ROOM_LOGIN_NOUIN:
            tip.text = @"非法用户";
            break;
        case ROOM_LOGIN_FULL:
            tip.text = @"房间人数满";
            break;
        case ROOM_LOGIN_IFULL:
            tip.text = @"房间已经达到允许人数,用户身份无法登陆";
            break;
        case ROOM_LOGIN_EPWD:
            tip.text = @"房间密码错误";
            break;
        case ROOM_LOGIN_CLOSE:
            tip.text = @"房间关闭";
            break;
        case ROOM_LOGIN_INBLACK:
            tip.text = @"在黑名单中";
            break;
        case ROOM_LOGIN_INTBLACK:
            tip.text = @"在临时黑名单中";
            break;
        case ROOM_LOGIN_NOTHIS:
            tip.text = @"房间不存在或被删除";
            break;
        case ROOM_LOGIN_LOCK:
            tip.text = @"房间冻结";
            break;
        case ROOM_LOGIN_IP_INVALIED:
            tip.text = @"该IP在房间内被禁";
            break;
    }
    
    return false;
}

-(void)onEnterRoom
{
    //无密码进入包厢房间,如果以后需要增加输入密码，请在此添加
    if(EnterRoom(m_roomId, m_roomId, "888888", (short)0) != 0)
    {
        tip.text = @"进入房间失败";
        DisConnectRoom();
    }
    tip.text = @"正在进入房间...";

}

#pragma mark -
#pragma mark Save Data Method
//保存线程回调的数据
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
-(void)saveUserInfo:(ResultData_GUser*)user
{
    memcpy(&m_myInfo, user, sizeof(ResultData_GUser));
}

-(void)saveCSeq:(int)cseq
{
    m_clientData = cseq;
}

-(void)saveLoginInfo:(ResultData_LoginInfo*)loginInfo
{
    memcpy(&m_roomLoginInfo,loginInfo,sizeof(ResultData_LoginInfo));
}

-(void)pushChatInfo:(ChatInfo*)chatInfo
{
    ShowInfo si = {0};
    char room[260] = {0};
    if (chatInfo->roomid == 0)
        strcpy(room, STR_NOTINROOM);
    else
        sprintf(room, "%s%s%s",STR_IN,chatInfo->room,STR_KTVROOM);
    char pTemp[260] = {0};
    if (chatInfo->isPublic)
    {
        sprintf(pTemp, "%s %s%s",chatInfo->sayname,room,STR_SAY);
    }
    else
    {
        sprintf(pTemp, "%s %s %s%s",chatInfo->sayname,STR_TO
                ,chatInfo->toname,STR_SAY);
    }
    strcpy(si.people, pTemp);
    strcpy(si.content, chatInfo->content);

    AutoLock lock(lockShow);
    m_vectShowInfo.push_back(si);
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
    m_listRGI.push_back(*rgi);//保存刷礼物的信息
}

#pragma mark -
#pragma mark Callback Method
-(void)OnAuthTunnelConnectSuccess
{
    printf("认证连接成功!\n");
    tip.text = @"认证连接成功!";
#define GLP_DEBUG
#ifdef GLP_DEBUG
    //for debug
    NSString* name = @"glp4703";
    NSString* password = @"glp13738024703";
    const char* md5Password = [NetUtilViewController md5:password.UTF8String];
    if(IdPassAuth(name.UTF8String,md5Password)!=0)
        tip.text = @"认证请求失败!";
    else
        tip.text = @"认证身份...";
#else
    NSString* name = @"startest01";
    NSString* password = @"123456";
    const char* md5Password = [NetUtilViewController md5:password.UTF8String];
    if(IdPassAuth(name.UTF8String,md5Password)!=0)
        tip.text = @"认证请求失败!";
    else
        tip.text = @"认证身份...";
//    const char* md5Password = [NetUtilViewController md5:passwdText.text.UTF8String];
//    if(IdPassAuth(nameText.text.UTF8String,md5Password)!=0)
//        tip.text = @"认证请求失败!";
//    else
//        tip.text = @"认证身份...";
#endif
}
-(void)OnAuthTunnelConnectTimeout
{
    tip.text = @"认证超时!";
}
-(void)OnAuthTunnelConnectError
{
    NSString *tipStr = [NSString stringWithFormat:@"认证服务器连接出错,errorcode:%d!\n",m_errorCode];
    tip.text = tipStr;
}
-(void)OnAuthTunnelClose
{
    tip.text = @"服务器断开连接!";
}
-(void)OnAuthTunnelError
{
    NSString *tipStr = [NSString stringWithFormat:@"认证服务器网络出错,errorcode:%d!\n",m_errorCode];
    tip.text = tipStr;
}

-(void)OnLobbyTunnelConnectSuccess
{
    tip.text = @"连接大厅成功";
    if(0 != GetChannellist(XG_VERSION,m_cunicode))
        tip.text = @"获取大厅列表失败";
    tip.text = @"获取大厅列表中...";
}

-(void)OnChannellistTunnelConnectSuccess
{
    
}

-(void)OnLobbyTunnelConnectTimeout
{
    m_bConnectedLobby = false;
    tip.text = @"大厅连接超时";
}

-(void)OnLobbyTunnelConnectError
{
    m_bConnectedLobby = false;
    NSString *tipStr = [NSString stringWithFormat:@"大厅服务器连接出错,errorcode:%d!\n",m_errorCode];
    tip.text = tipStr;
}

-(void)OnLobbyTunnelClose
{
    m_bConnectedLobby = false;
    tip.text = @"大厅服务器主动断开";
}

-(void)OnLobbyTunnelError
{
    m_bConnectedLobby = false;
    NSString *tipStr = [NSString stringWithFormat:@"大厅服务器网络出错,errorcode:%d!\n",m_errorCode];
    tip.text = tipStr;
}

-(void)OnRoomTunnelConnectSuccess
{
    tip.text = @"房间连接成功";
    
    if(ConfirmToken() == -2)//已经token,直接进入房间
    {
        [self onEnterRoom];
    }
}
-(void)OnRoomTunnelConnectTimeout
{
    tip.text = @"房间服务器连接超时";
}
-(void)OnRoomTunnelConnectError
{
    NSString *tipStr = [NSString stringWithFormat:@"房间服务器连接出错,errorcode:%d!\n",m_errorCode];
    tip.text = tipStr;
}
-(void)OnRoomTunnelClose
{
    tip.text = @"房间服务器主动断开";
}
-(void)OnRoomTunnelError
{
    NSString *tipStr = [NSString stringWithFormat:@"房间服务器网络出错,errorcode:%d!\n",m_errorCode];
    tip.text = tipStr;
}

-(void)RecvAuth
{
    printf("认证结果返回!\n");
    DisConnectAuth();
    
    bool bOk = [self onAuthError:m_errorCode];
    if(bOk)
    {
        if(ConnectLobby(XG_LOBBYHOST, (short)XG_LOBBYPORT, 60) != 0)
            tip.text = @"连接大厅失败!";
        else
            tip.text = @"连接大厅中...";
    }
}

-(void)RecvSectionList
{
    bool bOk = [self onLobbyError:m_errorCode];
    if(bOk)
    {
        tip.text = @"大厅列表获取成功";
        
        //登录指定大厅，星光早期版本支持选择大厅登录
        if(m_channellist->count == 1 && m_channellist->pCh[0].sectionid == XG_LOBBY_ID)
        {
            tip.text = @"正在进入大厅...";
            
            //登陆大厅
            LoginLobby(m_cunicode,XG_DEVICE);
        }
    }
}

-(void)RecvTokenAuth4Lobby
{
    bool bOk = [self onLobbyError:m_errorCode];
    if(bOk)
    {
        //tip.text = @"进入大厅中...";
    }
}

-(void)OnLoginLobby
{
    bool bOk = [self onLobbyError:m_errorCode];
    if(bOk)
    {
        tip.text = @"大厅登录成功";
        m_bConnectedLobby = true;
        
        ResultData_SelfData rd_sd = {0};
        if(GetSelfData(&rd_sd))
            m_goldLabel.text = [[NSString alloc] initWithFormat:@"%d 金币",rd_sd.gold];
        
        //进入指定房间 ktv包厢
        if (m_roomId != 0) {
            ResultData_RoomInfo room = {0};
            GetRoomInfoFromLocal(m_roomId, &room);

            int ret = 0;
            if (m_bWangtong)
                ret = ConnectRoom(room.ip_2, room.port, XG_ROOMTYPE_BASE);
            else
                ret = ConnectRoom(room.ip_1, room.port, XG_ROOMTYPE_BASE);
            if(ret == 0)
                tip.text = @"正在连接房间...";
            else
                tip.text = @"连接房间失败";
        }
    }
    
}

-(void)RecvTokenAuth4Room
{
    bool bOk = [self onRoomError:m_errorCode];
    if(bOk)
    {
        tip.text = @"房间认证通过";
        [self onEnterRoom];
    }
}
-(void)RecvEnterRoom
{
    bool bOk = [self onRoomError:m_errorCode];
    if(bOk)
    {
        if(m_clientData == m_roomId)
            tip.text = @"进入房间成功";
        else
        {
            //一般不会出现这种情况
            //需要后续人员继续开发，
            //断开当前房间，重新进入房间
        }
    }
}

-(void)OnReloadTable
{
    if (m_errorCode == 0)
    {
        [m_chatTable reloadData];
        NSIndexPath* path = [NSIndexPath indexPathForRow:(m_vectShowInfo.size()-1) inSection:0];
        [m_chatTable scrollToRowAtIndexPath:path atScrollPosition:UITableViewScrollPositionBottom animated:YES];
    }
    else
        tip.text = @"服务器回应超时!";
}

-(void)CheckGiftSend
{
    switch (m_errorCode) {
        case ROOM_NORMAL:
        {
            tip.text = @"送礼成功";
            [self start];
            break;
        }
        case ROOM_ERROR_GIFT_BALANCE:		//送礼时余额不足
        {
            tip.text = @"送礼时余额不足";
            break;
        }
        case ROOM_ERROR_GIFT_UNKOWN:		//交易金额为空
        {
            tip.text = @"交易金额为空";
            break;
        }
        case ROOM_ERROR_GIFT_NOTEXIST:		//该礼物不存在
        {
            tip.text = @"该礼物不存在";
            break;
        }
        case ROOM_ERROR_GIFT_OFFLINE:		//礼物接收者不在房间内
        {
            tip.text = @"礼物接收者不在线";
            break;
        }
    }
}

-(void)UpdateMoney
{
    ResultData_SelfData rd_sd = {0};
    if(GetSelfData(&rd_sd))
        m_goldLabel.text = [[NSString alloc] initWithFormat:@"%d %@",rd_sd.gold,@STR_GOLD];
}
-(void)InformRenderGift
{
    [self getTimerInterval];
    //启动定时器，定时刷礼物信息，具体参数参见
    if(m_timerGift == nil)
    {
        [self getTimerInterval];
        m_timerGift = [NSTimer scheduledTimerWithTimeInterval:m_timeIntervalGift target:self selector:@selector(onGiftRender:) userInfo:nil repeats:YES];
    }
}
-(void)RecvSpeakerFromLobby
{
    
}

//for debug
-(void)OnSetImage
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *docDir = [paths objectAtIndex:0];
    
    GiftInfo giftInfo;
    int nNum = 0;
    GetAllGiftId(NULL, nNum);
    GetGiftInfoFromLocal(100001, &giftInfo);
    NSString *src = [[NSString alloc] initWithFormat:@"%@/images/%s",docDir,giftInfo.image_name];
    UIImage* image = [[UIImage alloc] initWithContentsOfFile:src];
    [m_imgView setImage:image];
}
#pragma mark -
#pragma mark Table Data Source Methods
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return (int)m_vectShowInfo.size();
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    //随便起的名字，用于所有的单元都使用这个ID
    static NSString *CellTableIdentifier = @"NetutilChatPublic";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellTableIdentifier];
    if(cell == nil)
    {
        NSArray *nib = [[NSBundle mainBundle] loadNibNamed:@"CustomChatCell" owner:self options:nil];
        if ([nib count]>0) {
            cell = self.m_chatCell;
        } else{
            NSLog(@"failed to load CustomCell nib file");
        }
    }
    
    NSUInteger row = [indexPath row];
    if (row >= m_vectShowInfo.size()) {
        return nil;
    }
    
    ShowInfo& info = m_vectShowInfo[row];
    UILabel *ui_name = (UILabel *)[cell.contentView viewWithTag:TAG_CELL_WHO];
    ui_name.text = [[NSString alloc] initWithUTF8String:info.people];
    
    UILabel *ui_say = (UILabel *)[cell.contentView viewWithTag:TAG_CELL_SAY];
    ui_say.text = [[NSString alloc] initWithUTF8String:info.content];
    return cell;
}

@end
