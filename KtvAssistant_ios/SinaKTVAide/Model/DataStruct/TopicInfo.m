//
//  TopicInfo.m
//  SinaKTVAide
//
//  Created by apple on 13-10-28.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "TopicInfo.h"
#import "PCommonUtil.h"

@implementation TopicInfo

@synthesize topicId = _topicId;
@synthesize topicTitle = _topicTitle;
@synthesize topicUrl = _topicUrl;
@synthesize pinyin = _pinyin;
@synthesize artistHotVal = _artistHotVal;

+(TopicInfo *)initWithDictionary:(NSDictionary *)data{
    
    TopicInfo *topicInfo = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            topicInfo = [[TopicInfo alloc] init];
            topicInfo.topicId = [[data objectForKey:@"topicid"] longValue];
            topicInfo.topicTitle = [PCommonUtil decodeBase64:[data objectForKey:@"topicname"]];
            topicInfo.topicUrl = [PCommonUtil decodeBase64:[data objectForKey:@"topicurl"]];
            topicInfo.topicImgIcon = [PCommonUtil decodeBase64:[data objectForKey:@"topicimg"]];
            if([data objectForKey:@"pinyin"]){
                topicInfo.pinyin =   [PCommonUtil decodeBase64:[data objectForKey:@"pinyin"]];
            }
            if([data objectForKey:@"artisthotval"]){
                topicInfo.artistHotVal = [[data objectForKey:@"artisthotval"] intValue];
            }
        }
    }
    @catch (NSException *exception) {
        PLog(@"create TopicInfo failed...please check");
    }
    
    return topicInfo;
}

@end
