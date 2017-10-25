//
//  ExchangeGiftLog.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-28.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ExchangeLog : NSObject

@property (nonatomic, strong) NSString  *code;
@property (nonatomic, strong) NSString  *costGold;
@property (nonatomic, strong) NSString  *endDate;
@property (nonatomic, strong) NSString  *giftName;
@property (nonatomic, strong) NSString  *ktvName;
@property (nonatomic, strong) NSString  *giftNum;
@property (nonatomic, strong) NSString  *beginDate;
@property (nonatomic, strong) NSString  *unit;

+(ExchangeLog *)initWithDictionary:(NSDictionary *)data;
-(void)Log;

@end
