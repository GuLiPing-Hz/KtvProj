//
//  KTVHongBaoInfo.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/21.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "KTVHongBaoInfo.h"
#import "PCommonUtil.h"

@implementation KTVHongBaoInfo

-(instancetype)init
{
    self = [super init];
    if(self)
    {
        self.hbid = 0;
        self.hbnclass = -1;
        self.hbclass = @"";
        self.hbmission = @"";
        self.hbstatus = hongbao_todo;
        self.hbtime = @"";
    }
    
    return self;
}

+(instancetype)initWithDic:(NSDictionary*) data
{
    KTVHongBaoInfo* info = [[KTVHongBaoInfo alloc] init];
    if(info)
    {
        info.hbid = [data objectForKey:@"ID"];
        info.hbnclass = [[data objectForKey:@"RewardType"] integerValue];
        switch(info.hbnclass)
        {
            case 0:
                info.hbclass = @"新人奖励";
                break;
            case 1:
                info.hbclass = @"每日奖励";
                break;
            case 2:
                info.hbclass = @"个人红包";
                break;
        }
        info.hbmission = [PCommonUtil decodeBase64:[data objectForKey:@"DescrInfo"]];
        info.hbstatus = (eHongBaoStatus)[[data objectForKey:@"Status"] integerValue];
        
        id obj = [data objectForKey:@"AddTime"];
        if(obj)
            info.hbtime = [PCommonUtil decodeBase64:obj];
        
        info.gold = [[data objectForKey:@"RewardGold"] integerValue];
    }
    
    return info;
}

@end
