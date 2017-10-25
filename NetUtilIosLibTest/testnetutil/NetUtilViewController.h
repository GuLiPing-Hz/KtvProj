//
//  NetUtilViewController.h
//  testnetutil
//
//  Created by guliping_imac on 14-4-21.
//  Copyright (c) 2014年 天格科技. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "MessageInfoStruct.h"
#include "NetUtilCallBackProtocol.h"

class CResponse;
@class NSThread;


#define TAG_CELL_WHO 1
#define TAG_CELL_SAY 2
@interface NetUtilViewController : UIViewController <UITableViewDelegate,UITableViewDataSource,NetUtilCallBackProtocol>
{
    //ios 手机标识码 c风格字符串保存手机标识码
    char m_cunicode[260];
    //回调接口
    CResponse *m_response;
    //是否是网通用户
    bool m_bWangtong;
    //是否已连接大厅
    bool m_bConnectedLobby;
    //令牌信息
    TokenInfo m_tokenInfo;
    //大厅列表信息
    ResultData_GetChannellist *m_channellist;
    //个人信息
    ResultData_GUser m_myInfo;
    //房间ID即KTV包厢ID
    int m_roomId;
    //错误码
    int m_errorCode;
    //客户端clientData,保存向服务器发送是返回来的有用数据
    int m_clientData;
    //登录房间信息
    ResultData_LoginInfo m_roomLoginInfo;
    //公聊信息列表
    NSLock*         m_lockChat;
    VECTCHAT        m_vectChat;
    //刷礼物信息
    NSLock*         m_lockGift;
    LISTRENDERGIFT  m_listRGI;
    
    int             m_totalTimes;
    int             m_curTimes;
    //显示信息
    NSLock*         m_lockShow;
    VECTSHOWINFO    m_vectShowInfo;
    
    BOOL        m_running;
    float       m_fSpeed;
    float       m_fOldSpeed;

}

@property (nonatomic,retain) IBOutlet UIImageView* m_imgView;
@property (nonatomic) NSTimeInterval m_timeIntervalGift;
@property (nonatomic,retain) NSTimer* m_timerGift;

@property (nonatomic,retain) NSLock* m_lockGift;
@property (nonatomic,retain) NSLock* m_lockShow;

@property (nonatomic,retain) NSLock* m_lockChat;
@property (nonatomic,retain) NSMutableArray* m_sysTrack;
@property (nonatomic,retain) NSThread* oThread;
@property (nonatomic,retain) IBOutlet UITableView* m_chatTable;
@property (nonatomic,retain) IBOutlet UITableViewCell* m_chatCell;
@property (nonatomic,retain) IBOutlet UILabel* tip;
@property (nonatomic,retain) IBOutlet UILabel* m_goldLabel;
@property (nonatomic,retain) IBOutlet UILabel* m_trackLabel;
@property (nonatomic,retain) IBOutlet UITextField* nameText;
@property (nonatomic,retain) IBOutlet UITextField* passwdText;
@property (nonatomic,retain) IBOutlet UITextField* chatText;

//@param str: UTF-8 c string
//@return: UTF-8 c string
+(const char *)md5:(const char *)str;

-(IBAction)inpudown:(id)sender;
-(IBAction)backgroundTab:(id)sender;
-(IBAction)buttonLogin:(id)sender;
-(IBAction)buttonPublic:(id)sender;
-(IBAction)buttonPrivate:(id)sender;
-(IBAction)buttonSend:(id)sender;
-(IBAction)buttonHorn:(id)sender;

-(void)newThread:(id)sender;
-(void)onGiftRender:(id)timer;
-(void)setRoomId:(int)roomId;

//集中处理对各个服务器返回值的判断。
-(bool)onAuthError:(int) e;
-(bool)onLobbyError:(int) e;
-(bool)onRoomError:(int) e;

-(void)onEnterRoom;
//@protocol NetUtilCallBackProtocol;
//保存数据函数方法，
//@注：由于调用保存函数都在一个线程中回调，所以不需要考虑线程保护的问题
-(void)saveErrorcode:(int)code;
-(void)saveToken:(const char*)token length:(int)len;
-(void)saveChannellist:(ResultData_GetChannellist*)list;
-(void)saveUserInfo:(ResultData_GUser*)user;
-(void)saveCSeq:(int)cseq;
-(void)saveLoginInfo:(ResultData_LoginInfo*)loginInfo;
-(void)pushChatInfo:(ChatInfo*)chatInfo;
-(void)pushNotify:(NSString*)str;
-(void)pushRenderGift:(RenderGiftInfo*)rgi;

//主线程执行函数方法
//认证服务器
-(void)OnAuthTunnelConnectSuccess;
-(void)OnAuthTunnelConnectTimeout;
-(void)OnAuthTunnelConnectError;
-(void)OnAuthTunnelClose;
-(void)OnAuthTunnelError;
//大厅服务器
-(void)OnLobbyTunnelConnectSuccess;
-(void)OnLobbyTunnelConnectTimeout;
-(void)OnLobbyTunnelConnectError;
-(void)OnLobbyTunnelClose;
-(void)OnLobbyTunnelError;
//房间服务器
-(void)OnRoomTunnelConnectSuccess;
-(void)OnRoomTunnelConnectTimeout;
-(void)OnRoomTunnelConnectError;
-(void)OnRoomTunnelClose;
-(void)OnRoomTunnelError;
//频道服务器
-(void)OnChannellistTunnelConnectSuccess;

-(void)RecvAuth;
-(void)RecvSectionList;
-(void)RecvTokenAuth4Lobby;
-(void)OnLoginLobby;
-(void)RecvTokenAuth4Room;

-(void)OnReloadTable;
-(void)CheckGiftSend;
-(void)UpdateMoney;
-(void)InformRenderGift;
-(void)RecvSpeakerFromLobby;

//for debug
-(void)OnSetImage;
@end
