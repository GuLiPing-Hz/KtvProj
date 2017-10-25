//
//  UserCenterLogInfo.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-21.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface UserCenterLogInfo : NSObject

//共有部分
@property (nonatomic, assign) NSInteger gold;
@property (nonatomic, strong) NSString *ktvName;  //在大厅送礼时 ktv和room都为空
@property (nonatomic, strong) NSString *roomName;

//礼物日志
@property (nonatomic, strong) NSString *nickName;  //
@property (nonatomic, strong) NSString *headPhoto;
@property (nonatomic, strong) NSString *tradeDate;
@property (nonatomic, strong) NSString *giftName;
@property (nonatomic, assign) NSInteger giftNum;
@property (nonatomic, retain) NSString *unit;

//奖励日志
@property (nonatomic, strong) NSString *rewardName;  //
@property (nonatomic, assign) NSInteger type;
@property (nonatomic, strong) NSString *detail;
@property (nonatomic, strong) NSString *addTime;



+(UserCenterLogInfo *)initWithDictionaryGift:(NSDictionary *)data;
-(void)LogGift;


+(UserCenterLogInfo *)initWithDictionaryReward:(NSDictionary *)data;
-(void)LogReward;


@end
