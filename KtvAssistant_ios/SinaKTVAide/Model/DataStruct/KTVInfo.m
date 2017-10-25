//
//  KTVInfo.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-23.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "KTVInfo.h"

@implementation KTVInfo


+(KTVInfo *)initWithDictionary:(NSDictionary *)data
{
    KTVInfo *info = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            info = [[KTVInfo alloc]init];
            info.ktvID = [data objectForKey:@"id"];
            info.ktvName = [PCommonUtil decodeBase64:[data objectForKey:@"name"]];
            info.ktvAddress = [PCommonUtil decodeBase64:[data objectForKey:@"address"]];
        }
    }
    @catch (NSException *exception) {
        PLog(@"create KTVInfo failed...please check");
    }
    
    return info;

}

-(void)Log
{
    PLog(@"Print KTVInfo ktvID:(%@), ktvName(%@), ktvAddress(%@),",
          _ktvID, _ktvName, _ktvAddress);

}
@end
