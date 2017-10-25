//
//  Message.h
//  SinaKTVAide
//
//  Created by sp on 13-12-10.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Message : NSObject

@property (nonatomic, assign) long msgID;            //消息ID
@property (nonatomic, assign) int msgType;           //类型（1：；    2：歌曲上榜      3：XX）
@property (nonatomic, strong) NSString *content;     //内容
@property (nonatomic, strong) NSString *date;        //时间
@property (nonatomic, assign) long workID;           //演唱记录ID，（只有当msgType=2时才有）
@property (nonatomic, strong) NSString *roomID;       //房间ID，（只有当msgType=2时才有）
@property (nonatomic, assign) int point;             //演唱记录得分，（只有当msgType=2时才有）

+(Message *)initWithDefaultValue;
+(Message *)initWithDictionary:(NSDictionary *)data;

-(void) log;


@end
