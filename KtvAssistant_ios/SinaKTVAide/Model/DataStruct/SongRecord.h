//
//  SongRecord.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/13.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SongRecord : NSObject

@property (strong,nonatomic) NSString* songname;
@property (strong,nonatomic) NSString* singername;
@property (assign,nonatomic) int score;
@property (strong,nonatomic) NSString* room;
@property (strong,nonatomic) NSString* time;

+(instancetype) initWithDic:(NSDictionary*)dict;

@end
