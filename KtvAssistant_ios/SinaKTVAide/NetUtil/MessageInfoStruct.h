//
//  MessageDef.h
//  testnetutil
//
//  Created by guliping_imac on 14-4-22.
//  Copyright (c) 2014年 天格科技. All rights reserved.
//

#ifndef testnetutil_MessageInfoStruct_h
#define testnetutil_MessageInfoStruct_h

#import "iosdatadefine.h"
#include <vector>
#include <list>
#include <map>
#include <set>
#define NAME_IN_HORN_SIZE 64
#define HEAD_IN_HORN_SIZE 256

//在线用户信息
typedef struct _OnlineInfo{
    int idx;
    char name[MAXLEN_NICKNAME];
    char sex[2];
    char headphoto[260];    //头像url
    char type;    //0-主播，1-本包厢，2-其他包厢，3-大厅
}OnlineInfo;

typedef struct _TokenInfo{
    char token[MAXLEN_TOKEN];
    int length;
}TokenInfo;
typedef struct _AllInfo{
    int type; //0-公聊 1-私聊 3-礼物 4-喇叭 5-兑换 6-上榜 7-幸运中奖 8-红包
    int idx; //发送者/说话者idx/喇叭发送者/礼物发送者
    char name[260]; //发送者/说话者用户昵称
    int toidx; //被说话者idx/礼物接受者idx
    char toname[260]; //被说话者昵称/礼物接受者昵称
    int roomid; //房间ID 0为在大厅
    char roomname[260]; //房间名
    int  recv_roomid;//接受者房间ID
    char recv_room[260];//接受者所在房间
    int time;  //发送时间
    char content[4096]; //聊天内容/喇叭内容/礼物消息/兑换消息
    int gift_id;//礼物ID
    int total_number;//礼物总数量
    char unit_name[200];			//单位
    char headphoto[256];      //用户头像
    bool isPublic;//是否公聊
    int luckmultiple;   //中奖倍率
    bool isSend;
    char redtype;    //红包类型
    char redid[51];      //红包id
}AllInfo;

//喇叭信息
typedef struct _BroadInfo{
    int type;  //1-喇叭、兑换、上榜 2-送礼
    int idx;  //发送喇叭用户idx
    char nikename[260]; //用户昵称
    char content[4096]; //喇叭内容
    int time;  //发送时间
    int roomid; //房间id
    char roomname[260]; //房间名
    int recvroomid;  //接受房间id
    char recvroomname[260]; //接受房间名
    int giftid;  //礼物id
}BroadInfo;
//聊天信息
typedef struct _ChatInfo{
    int  sayidx;//说话者IDX 0的时候说明是我自己
    char sayname[260];//说话者名字
    int  toidx;//被说话者IDX 0的时候说明是公聊或对我说
    char toname[260];//被说话者名字
    int  roomid;//房间ID 0为在大厅里
    char room[260];//房间名
    char content[4096];//聊天内容
    bool isPublic;//是否公聊
    int chattime;//聊天时间
    char headphoto[256];      //用户头像
}ChatInfo;
//聊天列表
typedef std::vector<ChatInfo> VECTCHAT;
//刷礼物信息
typedef struct _RenderGiftInfo
{
    int  send_idx;//发送者IDX
    char send_name[260];//发送者昵称
    int  send_roomid;//发送者房间ID
    char send_room[260];//发送者所在房间 如果在大厅显示“未绑定”
    int  recv_idx;//接受者IDX
    char recv_name[260];//接受者昵称
    int  recv_roomid;//接受者房间ID
    char recv_room[260];//接受者所在房间
    
    int gift_id;//礼物ID
    int total_number;//礼物总数量
    int remain_number;//礼物剩余数量
    int time_span;//礼物刷的间隔频率
    int number_once;//一条记录包含的礼物数量
}RenderGiftInfo;
//刷礼物列表
typedef std::list<RenderGiftInfo> LISTRENDERGIFT;

//显示信息
typedef struct _ShowInfo{
    char people[260];
    char content[4096];
}ShowInfo;

typedef struct _KtvUser : public ResultData_GUser{
    long gold;
    char online_type;
}KtvUser;

typedef std::vector<ChatInfo> VECTCHATINFO;
typedef std::vector<BroadInfo> VECTBROADINFO;
typedef std::vector<AllInfo> VECTALLINFO;
typedef std::vector<ResultData_SongList> VECTREQSONGLIST;
typedef std::vector<ResultData_UIdxWithType> VECTTMPIDX;
typedef std::list<OnlineInfo> LISTONLINEINFO;
typedef std::vector<HBGetDetail> VECTHBDETAIL;
typedef std::set<std::string> SETREDID;
#endif
