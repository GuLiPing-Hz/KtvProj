//
//  UserCenterLogInfo.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-21.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "UserCenterLogInfo.h"

@implementation UserCenterLogInfo

+(UserCenterLogInfo *)initWithDictionaryGift:(NSDictionary *)data
{
    UserCenterLogInfo *info = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            info = [[UserCenterLogInfo alloc]init];
            info.nickName = [PCommonUtil decodeBase64:[data objectForKey:@"name"]];
            info.headPhoto = [PCommonUtil decodeBase64:[data objectForKey:@"photo"]];
            info.tradeDate = [PCommonUtil decodeBase64:[data objectForKey:@"tdate"]];
            info.giftName = [PCommonUtil decodeBase64:[data objectForKey:@"gname"]];
            info.giftNum = [[data objectForKey:@"gnum"] integerValue];
            info.gold = [[data objectForKey:@"gold"] integerValue];
            info.ktvName = [PCommonUtil decodeBase64:[data objectForKey:@"kname"]];
            info.roomName = [PCommonUtil decodeBase64:[data objectForKey:@"rname"]];
            info.unit = [PCommonUtil decodeBase64:[data objectForKey:@"unit"]];
            
        }
    }
    @catch (NSException *exception) {
        PLog(@"create UserCenterLogInfo failed...please check");
    }
    
    return info;
    
}


-(void)LogGift
{
    PLog(@"Print sourceName:(%@), targetName(%@), tradeDate(%@), giftName(%@), giftNum(%d) , gold(%d) , ktvName(%@) , roomName(%@) ",
          _nickName, _headPhoto, _tradeDate, _giftName, _giftNum, _gold, _ktvName, _roomName);
    
}

+(UserCenterLogInfo *)initWithDictionaryReward:(NSDictionary *)data
{
    UserCenterLogInfo *info = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            info = [[UserCenterLogInfo alloc]init];
            info.gold = [[data objectForKey:@"gold"] integerValue];
            info.rewardName = [PCommonUtil decodeBase64:[data objectForKey:@"reward_name"]];
            info.type = [[data objectForKey:@"type"] integerValue];
            info.detail = [PCommonUtil decodeBase64:[data objectForKey:@"detail"]];
            info.addTime = [PCommonUtil decodeBase64:[data objectForKey:@"addTime"]];
            info.ktvName = [PCommonUtil decodeBase64:[data objectForKey:@"kname"]];
            info.roomName = [PCommonUtil decodeBase64:[data objectForKey:@"rname"]];
            
        }
    }
    @catch (NSException *exception) {
        PLog(@"create UserCenterLogInfo failed...please check");
    }
    
    return info;
    
}


-(void)LogReward
{
    PLog(@"Print gold:(%d), rewardName(%@), type(%d), detail(%@), addTime(%@) , ktvName(%@) , roomName(%@)",
          _gold, _rewardName, _type, _detail, _addTime, _ktvName, _roomName);
}


@end
