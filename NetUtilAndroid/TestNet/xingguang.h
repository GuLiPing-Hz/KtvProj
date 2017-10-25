//
//  xingguang.h
//  testnetutil
//
//  Created by guliping_imac on 14-6-18.
//  Copyright (c) 2014年 天格科技. All rights reserved.
//

#ifndef testnetutil_xingguang_h
#define testnetutil_xingguang_h

//连接测试服务器的定义
#define XG_NET_TEST

#ifndef XG_NET_TEST
//星光认证服务器 ip port
#define XG_AUTHHOST "token.xg.91555.com"
#define XG_AUTHPORT 8500
//星光大厅列表服务器 ip port
#define XG_LOBBYHOST "section.xg.91555.com"
#define XG_LOBBYPORT 7200
#else
//星光认证服务器 ip port/Users/tiange01
#define XG_AUTHHOST "60.191.222.65"
#define XG_AUTHPORT 8500
//星光大厅列表服务器 ip port
#define XG_LOBBYHOST "60.191.222.65"
#define XG_LOBBYPORT 7200
#endif//XG_NET_TEST

//星光大厅ID 1
#define XG_LOBBY_ID 1

//网通IP地址
#define XG_WANGTONG "60.12.149.84"
//星光练歌房
#define XG_ROOMTYPE_PRACTICE  2
//星光普通房间
#define XG_ROOMTYPE_BASE  99
//星光版本
#define XG_VERSION  255
//星光登录设备 0：个人电脑；1：ANDROID；2：IOS
#define XG_DEVICE 2


//for debug 100002 406087 732261
#define DEBUG_ROOMID 0
//333151

#endif
