//
//  KTVGiftInfo.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface KTVGiftInfo : NSObject


@property (nonatomic, strong) NSString  *giftID;
@property (nonatomic, strong) NSString  *giftName;
@property (nonatomic, assign) NSInteger  giftGold;
@property (nonatomic, strong) NSString  *giftBigImg;
@property (nonatomic, strong) NSString  *giftSmallImg;

+(KTVGiftInfo *)initWithDictionary:(NSDictionary *)data;
-(void)Log;

@end
