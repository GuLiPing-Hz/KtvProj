//
//  RoomInfo.h
//  SinaKTVAide
//
//  Created by apple on 13-10-29.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface RoomInfo : NSObject

@property (nonatomic, strong) NSString *ktvId;//店面id
@property (nonatomic, strong) NSString *roomId;//包厢id
@property (nonatomic, strong) NSString *roomName;
@property (nonatomic, strong) NSString *beginTime;//包厢k歌开始时间
@property (nonatomic, strong) NSString *loginTime;//用户登录系统时间
@property (nonatomic, assign) long long periodsTime;//剩余k歌时间，单位秒
@property (nonatomic, strong) NSString *address;//店面地址
@property (nonatomic, strong) NSString *roomPassword;//房间密钥
@property (nonatomic, strong) NSString *xgRoomID;//绑定的星光房间

+(RoomInfo *)initWithDefaultValue;
+(RoomInfo *)initWithDictionary:(NSDictionary *)data;
-(void)Log;

@end
