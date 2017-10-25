//
//  NetUtilCallBackProtocol.h
//  testnetutil
//
//  Created by guliping_imac on 14-4-28.
//  Copyright (c) 2014年 天格科技. All rights reserved.
//
@protocol NetUtilCallBackProtocol


//保存数据函数方法，
//@注：由于调用保存函数都在一个线程中回调，所以不需要考虑线程保护的问题
@required
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

//按照登录房间的回调顺序排列
-(void)RecvAuth;
-(void)RecvSectionList;
-(void)RecvTokenAuth4Lobby;
-(void)OnLoginLobby;
-(void)RecvTokenAuth4Room;
-(void)RecvEnterRoom;

-(void)OnReloadTable;
-(void)CheckGiftSend;
-(void)UpdateMoney;
-(void)InformRenderGift;
-(void)RecvSpeakerFromLobby;

//for debug
-(void)OnSetImage;
@end
