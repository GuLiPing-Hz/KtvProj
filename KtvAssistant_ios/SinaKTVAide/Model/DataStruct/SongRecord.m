//
//  SongRecord.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/13.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "SongRecord.h"
#import "PCommonUtil.h"

@implementation SongRecord

-(instancetype)init
{
    self = [super init];
    
    self.songname = @"";
    self.singername = @"";
    self.score = 0;
    self.room = @"";
    self.time = @"";
    
    return self;
}

+(instancetype) initWithDic:(NSDictionary*)dict
{
    SongRecord* ret = nil;
    
    if(dict)
    {
        ret = [[SongRecord alloc] init];
        if(ret)
        {
            id obj = [dict objectForKey:@"music_name"];
            if(obj)
                ret.songname = [PCommonUtil decodeBase64:obj];
            
            obj = [dict objectForKey:@"music_singer"];
            if(obj)
                ret.singername = [PCommonUtil decodeBase64:obj];
            
            obj = [dict objectForKey:@"top_score"];
            if(obj)
                ret.score = [obj intValue];
            
            obj = [dict objectForKey:@"Room_name"];
            if(obj)
                ret.room = [PCommonUtil decodeBase64:obj];
            
            obj = [dict objectForKey:@"top_date"];
            if(obj)
                ret.time = [PCommonUtil decodeBase64:obj];
        }
    }
    return ret;
}

@end
