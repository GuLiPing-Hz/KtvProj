//
//  KTVInfo.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-23.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface KTVInfo : NSObject

@property (nonatomic, strong) NSString *ktvID;
@property (nonatomic, strong) NSString *ktvName;
@property (nonatomic, strong) NSString *ktvAddress;

+(KTVInfo *)initWithDictionary:(NSDictionary *)data;
-(void)Log;

@end
