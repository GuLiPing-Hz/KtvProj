//
//  PointLog.h
//  SinaKTVAide
//
//  Created by sp on 13-12-9.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PointLog : NSObject

@property (nonatomic, assign) long sourceID;            //积分记录ID
@property (nonatomic, assign) int scoreType;            //类型（1：包厢签到；    2：歌曲上榜      3：XX） 
@property (nonatomic, strong) NSString *source;         //内容
@property (nonatomic, assign) int points;               //分值
@property (nonatomic, assign) int flag;                 //是否已领

+(PointLog *)initWithDefaultValue;
+(PointLog *)initWithDictionary:(NSDictionary *)data;

-(void) log;

@end
