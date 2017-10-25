//
//  RewardInfo.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-21.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface RewardInfo : NSObject

@property (nonatomic, assign) NSInteger gold;
@property (nonatomic, strong) NSString *rewardName;  //
@property (nonatomic, assign) NSInteger type;
@property (nonatomic, strong) NSString *detail;
@property (nonatomic, strong) NSString *addTime;
@property (nonatomic, strong) NSString *ktvName;  //在大厅送礼时 ktv和room都为空
@property (nonatomic, strong) NSString *roomName; //

+(RewardInfo *)initWithDictionary:(NSDictionary *)data;
-(void)Log;

@end
