//
//  NetUtilCallBackProtocol.h
//  testnetutil
//
//  Created by guliping_imac on 14-4-28.
//  Copyright (c) 2014年 天格科技. All rights reserved.
//

#include "iosdatadefine.h"
#include "MessageInfoStruct.h"
@protocol NetUtilCallBackProtocol


//保存数据函数方法，
/*
 @注：由于调用保存函数都在一个线程中回调，所以不需要考虑线程保护的问题；
     但是考虑到要更新主界面，所以一般不带参数的直接采用performSelectorOnMainThread的方式回调，
     如果涉及到C++数据结构，基本都是直接网络线程直接回调，实现方法有需要更新界面的，请自行采用performSelectorOnMainThread的方式。
 */
@optional
-(void)saveRet:(int)ret;
-(void)saveErrorcode:(int)code;
-(void)saveToken:(const char*)token length:(int)len;
-(void)saveChannellist:(ResultData_GetChannellist*)list;
-(void)saveUserInfo:(KtvUser*)user;
-(void)saveCSeq:(int)cseq;
-(void)saveLoginInfo:(ResultData_LoginInfo*)loginInfo;
-(void)pushChatInfo:(AllInfo*)allInfo;
-(void)pushOnlineInfo:(OnlineInfo*)online;
-(void)pushOnline:(const ResultData_UIdxWithType*)list :(int)size;
-(void)noticeEndsong;
-(void)closeRoom;
-(void)changehead:(int)idx :(const char*)picture;
-(void)deleteOnlineInfo:(int)idx;
-(void)isChanged;
-(void)pushBroadInfo:(AllInfo*)allInfo;
-(void)pushSpeakInfo:(AllInfo*)allInfo;
-(void)pushTmpSpeakInfo:(AllInfo*)broadInfo;
-(void)pushRecvSpeakInfo:(ResultData_UserSampleInfo *)siminfo;
-(void)pushNotify:(NSString*)str;
-(void)pushRenderGift:(RenderGiftInfo*)rgi;
-(void)pushReqSongInfo:(const ResultData_SongList*)pSonglist :(int)count;
-(void)loginNotice;
-(void)onNameChange:(int)idx name:(const char*)name;


-(void)onRecvUserIdxAndType:(const ResultData_UIdxWithType*)list size:(int)p2;
-(void)onRecvUserLoginNew:(int)idx type:(char)p1;
-(void)onRecvSendHBResult:(int)ret;
-(void)onRecvHongBao:(int)idx hbid:(const char*)p1 mark:(const char*)p2;
-(void)onRecvHongBaoStatus:(const char*)hbid ret:(int)p;
-(void)onRecvVieHongBaoResult:(int)result idx:(int)p1 hbid:(const char*)p2 hbtype:(char)p3 remark:(const char*)p4 gold:(int)p5;
-(void)onRecvHongBaoDetail:(int)ret idx:(int)p1 hbtype:(char)p2 remark:(const char*)p3 totalgold:(int)p4 totalnumber:(int)p5 sendtime:(int)sendtime list:(const ResultData_HBDetail*)p6 number:(int)p7;
-(void)onRecvSingSongChange:(const char*)songname singer:(const char*)p1 score:(int)p2;
-(void)onRecvHongBNumber:(int)number;
-(void)onHongBUnReadAdd;
-(void)onChatUnReadAdd;
-(void)onRecvRoomHB:(int) number list:(const ResultData_RoomHB*)list;

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

-(void)CheckGiftSend;
-(void)UpdateMoney;
-(void)InformRenderGift;
-(void)RecvSpeakerFromLobby;
-(void)InitGiftImg;
@end
