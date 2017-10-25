//
//  PointLog.m
//  SinaKTVAide
//
//  Created by sp on 13-12-9.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "PointLog.h"

@implementation PointLog

@synthesize sourceID;
@synthesize scoreType;
@synthesize source;
@synthesize points;
@synthesize flag;

+(PointLog *)initWithDefaultValue{
    PointLog *log = [[PointLog alloc] init];
    log.sourceID = 0L;
    log.scoreType = 0;
    log.source = @"";
    log.points = 0;
    log.flag = 0;
    
    return log;
}

+(PointLog *)initWithDictionary:(NSDictionary *)data{
    PointLog *log = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            log = [[PointLog alloc] init];
            
            log.sourceID = [[data objectForKey:@"sourceid"] longValue];
            log.scoreType = [[data objectForKey:@"sourcetype"] intValue];
            log.source = [PCommonUtil decodeBase64:[data objectForKey:@"source"]];//[NSString stringWithFormat:@"%@",[data objectForKey:@"headphoto"]];
            log.points = [[data objectForKey:@"points"] intValue];
            log.flag = [[data objectForKey:@"flag"] intValue];
            
            //[log log];
        }
        
    }@catch (NSException *exception) {
        PLog(@"create PointLog failed...please check");
    }
    
    return log;
}

-(void) log{
    PLog(@"PointLog [sid=%ld, type=%d, source=%@, points=%d, flag=%d]", sourceID, scoreType, source, points, flag);
}

@end
