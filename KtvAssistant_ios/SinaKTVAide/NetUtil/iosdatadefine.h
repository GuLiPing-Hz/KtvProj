//
//  iosdatadefine.h
//  SinaKTVAide
//
//  Created by cxp on 15/5/27.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#ifndef SinaKTVAide_iosdatadefine_h
#define SinaKTVAide_iosdatadefine_h
#import "ResultData.h"
#import <map>

typedef struct _HBGetDetail
{
    int idx;//idx
    int gold;//金币
    int time;  //领取时间
    char name[51];  //名称
    char headurl[256];  //头像
    bool isStar;//最佳手气
}HBGetDetail;

typedef struct _HBSendDetail{
    int sendidx; //发送者idx
    char name[200]; //发送者名字
    char mark[256]; //备注
    int  gold;    //红包金币
    char type;   //红包类型
    int num;    //红包总个数
    char headurl[256];  //头像url
    int sendtime;   //红包发送时间
}HBSendDetail;

typedef struct _RedInfo
{
    char redid[51];         //红包id
    int  idx;          //发送者idx
    char headurl[256];			//发送者头像
    char name[200];             //名称
    int gold;               //抢到金币数
    char mark[256];         //红包备注
    char type;      //红包类型
}RedInfo;

typedef std::map<int, HBGetDetail> MAPHBDETAIL;
#endif
