//
//  ChatGiftLogInfo.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-20.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "ChatGiftLogInfo.h"

@implementation ChatGiftLogInfo

+(ChatGiftLogInfo *)initWithDictionary:(NSDictionary *)data
{
    ChatGiftLogInfo *info = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            info = [[ChatGiftLogInfo alloc]init];
            info.nickName = [data objectForKey:@"name"];
            info.headPhoto = [data objectForKey:@"photo"];
            info.tradeDate = [data objectForKey:@"tdate"];
            info.giftName = [data objectForKey:@"gname"];
            info.giftNum = [[data objectForKey:@"gnum"] integerValue];
            info.gold = [[data objectForKey:@"gold"] integerValue];
            info.ktvName = [data objectForKey:@"kname"];
            info.roomName = [data objectForKey:@"rname"];
            
        }
    }
    @catch (NSException *exception) {
        PLog(@"create ChatGiftLogInfo failed...please check");
    }
    
    return info;

}


-(void)Log
{
    NSLog(@"Print sourceName:(%@), targetName(%@), tradeDate(%@), giftName(%@), giftNum(%d) , gold(%d) , ktvName(%@) , roomName(%@) ",
          _nickName, _headPhoto, _tradeDate, _giftName, _giftNum, _gold, _ktvName, _roomName);

}

@end
