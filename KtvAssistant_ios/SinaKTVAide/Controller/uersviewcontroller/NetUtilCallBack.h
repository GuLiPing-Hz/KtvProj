//
//  NetUtilCallBack.h
//  SinaKTVAide
//
//  Created by cxp on 14-7-23.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NetUtilCallBackProtocol.h"
#import "MessageInfoStruct.h"
#import <UIKit/UIKit.h>
#import "DataType.h"
#import "NetUtilCallBackDelegate.h"

class CResponse;
@class NSThread;
typedef std::vector<GiftInfo> VECTGIFTINFO;

typedef enum _eTransaction{
    TransactionLobbyLogin = 1,
    TransactionLobbyAgain,
    TransactionRoomLogin,
    TransactionAuthAgain
}eTransaction;

typedef void(^TransactionListener)(BOOL success);

@interface NetUtilCallBack : NSObject<NetUtilCallBackProtocol>
{
    
    //回调接口
    CResponse *m_response;
@public
    //ios 手机标识码 c风格字符串保存手机标识码
    char m_cunicode[260];
    //是否是网通用户
    bool m_bWangtong;
    //是否已连接大厅
    bool m_bConnectedLobby;
    //是否已连接房间
    bool m_bConnectedRoom;
    //用户信息是否更改
    bool m_isChanged;
    //令牌信息
    TokenInfo m_tokenInfo;
    //大厅列表信息
    ResultData_GetChannellist *m_channellist;
    //个人信息
    KtvUser m_myInfo;
    char mOnlineType;
    //房间ID即KTV包厢ID
    int m_roomId;
    //返回值
    int m_ret;
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
    LISTONLINEINFO   m_listPrincessInfo;  //主播列表
    LISTONLINEINFO  m_listRoomUserInfo;   //本包厢
    LISTONLINEINFO  m_listLobbyUserInfo;  //身边用户
    
    VECTALLINFO     m_vectAllInfo;
    VECTGIFTINFO    m_localLuckGiftInfo; //礼物类别,幸运
    VECTGIFTINFO    m_localCommonGiftInfo;//礼物类别,普通
    VECTGIFTINFO    m_localLuxuryGiftInfo;//礼物类别,奢侈
    VECTREQSONGLIST m_vectreqsonglist;  //已点歌曲列表
    VECTALLINFO     m_tempBroadInfo;   //喇叭临时队列
    
    VECTTMPIDX      m_tmpidx;
    VECTHBDETAIL     m_hbdetail;   //红包领取详情
    SETREDID     m_tmpredid;  //包厢红包临时列表
    
    BOOL        m_running;
    float       m_fSpeed;
    float       m_fOldSpeed;
    
    int m_reconnectTimes;
    
    //当前处理的事务
    int mCurrentTransaction;//eTransaction
    TransactionListener mListener;
}
@property (nonatomic,retain) NSLock* m_lockGift;
@property (nonatomic,retain) id tableview;
@property (nonatomic,retain) NSLock* m_lockShow;
@property (nonatomic,retain) NSLock* m_lockChat;
@property (nonatomic,retain) NSMutableArray* m_sysTrack;

@property (nonatomic, assign)id<NetUtilCallBackDelegate> thedelegate;

+(NetUtilCallBack *)getInstance;
+(const char *)md5:(const char *)str;

-(void)setRoomId:(int)roomId;

-(void)doCurrentWork:(int) transaction;
-(void)doCurrentWork:(int) transaction listener:(void(^)(BOOL success))listener;

-(KtvUser*)getKtvUserInfo;
-(void)initcallback;
//-(void)getTable:(InteractTabView *)tabview;
-(void)saveRet:(int)ret;
-(void)saveErrorcode:(int)code;
-(void)saveToken:(const char*)token length:(int)len;
-(void)saveChannellist:(ResultData_GetChannellist*)list;
-(void)saveUserInfo:(KtvUser*)user;
//-(void)SaveUserInfoSample:(int)idx length:(const char *)nickName;
-(void)saveCSeq:(int)cseq;
-(void)saveLoginInfo:(ResultData_LoginInfo*)loginInfo;
-(void)pushChatInfo:(AllInfo*)allInfo;
-(void)pushBroadInfo:(AllInfo*)allInfo;
-(void)pushSpeakInfo:(AllInfo*)allInfo;
-(void)pushTmpSpeakInfo:(AllInfo*)broadInfo;
-(void)pushRecvSpeakInfo:(ResultData_UserSampleInfo *)siminfo;
-(void)pushOnline:(const ResultData_UIdxWithType*)list :(int)size;
-(void)pushOnlineInfo:(OnlineInfo*)online;
-(void)noticeEndsong;
-(void)closeRoom;
-(void)loginNotice;
-(void)changehead:(int)idx :(const char*)picture;
-(void)isChanged;
-(void)deleteOnlineInfo:(int)idx;
-(void)pushNotify:(NSString*)str;
-(void)pushRenderGift:(RenderGiftInfo*)rgi;
-(void)pushReqSongInfo:(const ResultData_SongList*)pSonglist :(int)count;

-(void)onRecvUserIdxAndType:(const ResultData_UIdxWithType*)list size:(int)p2;
-(void)onRecvUserLoginNew:(int)idx type:(char)p1;
-(void)onRecvSendHBResult:(int)ret;
-(void)onRecvHongBao:(int)idx hbid:(const char*)p1 mark:(const char*)p2;
-(void)onRecvHongBaoStatus:(const char*)hbid ret:(int)p;
-(void)onRecvVieHongBaoResult:(int)result idx:(int)p1 hbid:(const char*)p2 hbtype:(char)p3 remark:(const char*)p4 gold:(int)p5;
-(void)onRecvHongBaoDetail:(int)ret idx:(int)p1 hbtype:(char)p2 remark:(const char*)p3 totalgold:(int)p4 totalnumber:(int)p5 sendtime:(int)sendtime list:(const ResultData_HBDetail*)p6 number:(int)p7;
-(void)onRecvSingSongChange:(const char*)songname singer:(const char*)p1 score:(int)p2;
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

//按照登录房间的回调顺序排列
-(void)RecvAuth;
-(void)RecvSectionList;
-(void)RecvTokenAuth4Lobby;
-(void)OnLoginLobby;
-(void)RecvTokenAuth4Room;
-(void)RecvEnterRoom;
-(void)RecvLeaveRoom;

-(void)CheckGiftSend;
-(void)UpdateMoney;
-(void)InformRenderGift;
-(void)RecvSpeakerFromLobby;
-(void)InitGiftImg;

+(bool)onLobbyError:(int) e;

@end

