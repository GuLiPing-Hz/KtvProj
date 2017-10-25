//
//  KTVGiftInfo.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "KTVGiftInfo.h"

@implementation KTVGiftInfo


+(KTVGiftInfo *)initWithDictionary:(NSDictionary *)data
{
    KTVGiftInfo *info = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            info = [[KTVGiftInfo alloc]init];
            info.giftID = [data objectForKey:@"id"];
            info.giftName = [PCommonUtil decodeBase64:[data objectForKey:@"name"]];
            info.giftBigImg = [PCommonUtil decodeBase64:[data objectForKey:@"bimg"]];
            info.giftSmallImg = [PCommonUtil decodeBase64:[data objectForKey:@"simg"]];
            info.giftGold = [[data objectForKey:@"score"] integerValue];
        }
    }
    @catch (NSException *exception) {
        PLog(@"create GiftInfo failed...please check");
    }
    
    return info;
    
}

-(void)Log
{
    PLog(@"Print KTVInfo giftID:(%@), giftName(%@), giftBigImg(%@), giftSmallImg(%@), giftGold(%d),",
          _giftID, _giftName, _giftBigImg, _giftSmallImg, _giftGold);
    
}


@end
