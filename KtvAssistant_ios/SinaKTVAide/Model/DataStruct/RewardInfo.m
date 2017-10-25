//
//  RewardInfo.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-21.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "RewardInfo.h"

@implementation RewardInfo

+(RewardInfo *)initWithDictionary:(NSDictionary *)data
{
    RewardInfo *info = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            info = [[RewardInfo alloc]init];
            info.gold = [[data objectForKey:@"gold"] integerValue];
            info.rewardName = [data objectForKey:@"reward_name"];
            info.type = [[data objectForKey:@"type"] integerValue];
            info.detail = [data objectForKey:@"detail"];
            info.addTime = [data objectForKey:@"addTime"];
            info.ktvName = [data objectForKey:@"kname"];
            info.roomName = [data objectForKey:@"rname"];
            
        }
    }
    @catch (NSException *exception) {
        PLog(@"create RewardInfo failed...please check");
    }
    
    return info;

}


-(void)Log
{
    NSLog(@"Print gold:(%d), rewardName(%@), type(%d), detail(%@), addTime(%@) , ktvName(%@) , roomName(%@)",
          _gold, _rewardName, _type, _detail, _addTime, _ktvName, _roomName);
}

@end
