//
//  TopSong.h
//  SinaKTVAide
//
//  Created by sp on 13-12-9.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SongInfo.h"
#import "User.h"

@interface TopSong : NSObject


@property (nonatomic, assign) long tsID;          //演唱记录ID
@property (nonatomic, assign) int score;            //演唱分数
@property (nonatomic, strong) NSString *address;    //演唱地点
@property (nonatomic, strong) SongInfo *song;       //
@property (nonatomic, strong) User *user;           //



+(TopSong *)initWithDefaultValue;
+(TopSong *)initWithDictionary:(NSDictionary *)data;

-(void) log;


@end
