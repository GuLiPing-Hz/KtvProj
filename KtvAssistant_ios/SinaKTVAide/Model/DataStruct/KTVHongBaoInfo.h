//
//  KTVHongBaoInfo.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/21.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum {
    hongbao_todo,//未完成
    hongbao_done,//已完成，待领取
    hongbao_finish//已领取
}eHongBaoStatus;

@interface KTVHongBaoInfo : NSObject

//红包所属分类
//0新人奖励 ; 1每日奖励 ; 2个人红包
@property(nonatomic,assign) int hbnclass;
@property(nonatomic,strong) NSString* hbclass;
//红包任务说明
@property(nonatomic,strong) NSString* hbmission;
//红包状态
@property(nonatomic,assign) eHongBaoStatus hbstatus;
//红包ID
@property(nonatomic,strong) NSString* hbid;
//红包领取时间
@property(nonatomic,strong) NSString* hbtime;
//红包金币信息
@property(nonatomic,assign) int gold;

+(instancetype)initWithDic:(NSDictionary*) data;
@end
