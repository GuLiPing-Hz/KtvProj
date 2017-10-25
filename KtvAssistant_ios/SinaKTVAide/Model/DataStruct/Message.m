//
//  Message.m
//  SinaKTVAide
//
//  Created by sp on 13-12-10.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "Message.h"

@implementation Message

@synthesize msgID;
@synthesize msgType;
@synthesize content;
@synthesize date;
@synthesize workID;
@synthesize roomID;
@synthesize point;

+(Message *)initWithDefaultValue{
    Message *msg = [[Message alloc] init];
    msg.msgID = 0L;
    msg.msgType = 0;
    msg.content = @"";
    msg.date = @"1970-01-01 00:00:00";
    msg.workID = 0L;
    msg.roomID = @"0";
    msg.point = 0;
    
    return msg;
}


+(Message *)initWithDictionary:(NSDictionary *)data{
    Message *msg = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            msg = [[Message alloc] init];
            
            msg.msgID = [[data objectForKey:@"msgid"] longValue];
            msg.msgType = [[data objectForKey:@"msgtype"] intValue];
            msg.content = [PCommonUtil decodeBase64:[data objectForKey:@"content"]];      //[PCommonUtil decodeBase64:[data objectForKey:@"content"]];
            msg.date = [PCommonUtil decodeBase64:[data objectForKey:@"date"]];       //[NSString stringWithFormat:@"%@",[data objectForKey:@"date"]];
            
            if([data objectForKey:@"workid"]){
                msg.workID = [[data objectForKey:@"workid"] longValue];
            }
            
            if([data objectForKey:@"roomid"]){
                msg.roomID = [NSString stringWithFormat:@"%@",[data objectForKey:@"roomid"]];
            }
            
            if([data objectForKey:@"point"]){
                msg.point = [[data objectForKey:@"point"] intValue];
            }
            //[msg log];
        }
        
    }@catch (NSException *exception) {
        PLog(@"create PointLog failed...please check");
    }
    
    return msg;
}

- (id)init{
    self = [super init];
    if(self !=nil){
        self.msgID = 0L;
        self.msgType = 0;
        self.content = @"";
        self.date = @"1970-01-01 00:00:00";
        self.workID = 0L;
        self.roomID = @"0";
        self.point = 0;
    }
    
    return self;
}

-(void) log{
    PLog(@"Message [msgID=%ld, msgType=%d, content=%@, date=%@, workID=%ld roomID=%@ point=%d]",
         msgID, msgType, content, date, workID, roomID, point);
}

@end
