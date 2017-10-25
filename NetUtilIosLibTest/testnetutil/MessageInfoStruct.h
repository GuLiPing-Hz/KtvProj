//
//  MessageDef.h
//  testnetutil
//
//  Created by guliping_imac on 14-4-22.
//  Copyright (c) 2014年 天格科技. All rights reserved.
//

#ifndef testnetutil_MessageInfoStruct_h
#define testnetutil_MessageInfoStruct_h

#include "../include/NetUtil/ResultData.h"
#include <vector>
#include <list>

typedef struct _TokenInfo{
    char token[MAXLEN_TOKEN];
    int length;
}TokenInfo;
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
typedef std::vector<ShowInfo> VECTSHOWINFO;
#endif
