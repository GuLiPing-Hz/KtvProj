//
//  BannerSongParameter.m
//  SinaKTVAide
//
//  Created by sp on 13-12-10.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BannerSongParameter.h"

@implementation BannerSongParameter

@synthesize modelID;
@synthesize topicID;
@synthesize artistID;

+(BannerSongParameter *)initWithDefaultValue{
    BannerSongParameter *songParameter = [[BannerSongParameter alloc] init];
    return songParameter;
}


+(BannerSongParameter *)initWithDictionary:(NSDictionary *)data{
    BannerSongParameter *songParameter = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            songParameter = [[BannerSongParameter alloc] init];
            
            songParameter.modelID = [[data objectForKey:@"modelid"] intValue];
            songParameter.topicID = [[data objectForKey:@"topicid"] intValue];
            songParameter.artistID = [[data objectForKey:@"artistid"] longValue];;
            
        }
        
    }@catch (NSException *exception) {
        PLog(@"create BannerSongParameter failed...please check");
    }
    
    return songParameter;
}

- (id)init{
    self = [super init];
    if(self !=nil){
        self.modelID = 0;
        self.topicID = 0;
        self.artistID = 0L;
    }
    
    return self;
}

-(void) log{
    PLog(@"BannerSongParameter [modelID=%d, topicID=%d, artistID=%ld ]", modelID, topicID, artistID);
}

-(NSString *) description {
    return [NSString stringWithFormat:@"BannerSongParameter [modelID=%d, topicID=%d, artistID=%ld ]", modelID, topicID, artistID];
}

@end
