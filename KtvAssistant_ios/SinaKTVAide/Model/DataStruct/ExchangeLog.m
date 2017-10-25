//
//  ExchangeGiftLog.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-28.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "ExchangeLog.h"

@implementation ExchangeLog

+(ExchangeLog *)initWithDictionary:(NSDictionary *)data
{
    ExchangeLog *info = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            info = [[ExchangeLog alloc]init];
            info.code = [PCommonUtil decodeBase64:[data objectForKey:@"code"]];
            info.costGold = [data objectForKey:@"cost"];
            info.beginDate = [PCommonUtil decodeBase64:[data objectForKey:@"begin"]];
            info.endDate = [PCommonUtil decodeBase64:[data objectForKey:@"end"]];
            info.giftName = [PCommonUtil decodeBase64:[data objectForKey:@"gname"]];
            info.ktvName = [PCommonUtil decodeBase64:[data objectForKey:@"kname"]];
            info.unit = [PCommonUtil decodeBase64:[data objectForKey:@"unit"]];
            info.giftNum = [data objectForKey:@"num"];
        }
    }
    @catch (NSException *exception) {
        PLog(@"create ExchangeGiftLog failed...please check");
    }
    
    return info;
    
}

-(void)Log
{
    PLog(@"Print KTVInfo code:(%@), costGold(%@), beginDate(%@), endDate(%@), giftName(%@), giftNum(%@)",
          _code, _costGold, _beginDate, _endDate, _giftName,_giftNum);
    
}

@end
