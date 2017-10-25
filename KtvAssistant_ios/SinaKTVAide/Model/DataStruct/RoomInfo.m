//
//  RoomInfo.m
//  SinaKTVAide
//
//  Created by apple on 13-10-29.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "RoomInfo.h"
#import "PCommonUtil.h"

@implementation RoomInfo

@synthesize ktvId = _ktvId;
@synthesize roomId =  _roomId;
@synthesize roomName = _roomName;
@synthesize beginTime = _beginTime;
@synthesize loginTime = _loginTime;
@synthesize periodsTime = _periodsTime;
@synthesize address = _address;
@synthesize roomPassword = _roomPassword;
@synthesize xgRoomID = _xgRoomID;

+(RoomInfo *)initWithDefaultValue{
    
    RoomInfo *roomInfo = [[RoomInfo alloc] init];
    roomInfo.ktvId = @"0";
    roomInfo.roomId = @"0";
    roomInfo.roomPassword = @"";
    roomInfo.xgRoomID = @"0";
    return roomInfo;
}

+(RoomInfo *)initWithDictionary:(NSDictionary *)data{
    
    RoomInfo *roomInfo = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            roomInfo = [[RoomInfo alloc] init];
            roomInfo.ktvId = [data objectForKey:@"ktvid"];
            roomInfo.roomId = [NSString stringWithFormat:@"%@",[data objectForKey:@"roomid"]];
            roomInfo.roomName = [PCommonUtil decodeBase64:[data objectForKey:@"roomname"]];
            roomInfo.beginTime = [PCommonUtil decodeBase64:[data objectForKey:@"begintime"]];
            roomInfo.loginTime = [PCommonUtil decodeBase64:[data objectForKey:@"logintime"]];
            roomInfo.periodsTime = [[data objectForKey:@"periodstime"] longLongValue];
            roomInfo.address = [PCommonUtil decodeBase64:[data objectForKey:@"address"]];
            roomInfo.xgRoomID = [data objectForKey:@"xgroomid"];
        }
    }
    @catch (NSException *exception) {
        PLog(@"create RoomInfo failed...please check");
    }
    
    return roomInfo;
}

-(void)Log{
    PLog(@"Print RoomInfo: _ktvId(%@), _roomId(%@), _roomPassword(%@), _roomName(%@), _beginTime(%@), _loginTime(%@), _periodsTime(%lld), _address(%@)", _ktvId, _roomId, _roomPassword, _roomName, _beginTime, _loginTime, _periodsTime, _address);
}

@end
