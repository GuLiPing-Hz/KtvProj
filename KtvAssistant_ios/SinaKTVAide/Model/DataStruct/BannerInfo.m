//
//  BannerInfo.m
//  SinaKTVAide
//
//  Created by sp on 13-12-10.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BannerInfo.h"

@implementation BannerInfo

@synthesize bID;
@synthesize type;
@synthesize imagePhoto;
@synthesize title;
@synthesize songParameter;

+(BannerInfo *)initWithDefaultValue{
    BannerInfo *bannerInfo = [[BannerInfo alloc] init];
    return bannerInfo;
}


+(BannerInfo *)initWithDictionary:(NSDictionary *)data{
    BannerInfo *bannerInfo = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            bannerInfo = [[BannerInfo alloc] init];
            
            bannerInfo.bID = [[data objectForKey:@"id"] longValue];
            bannerInfo.type = [[data objectForKey:@"bannertype"] intValue];
            bannerInfo.imagePhoto = [PCommonUtil decodeBase64:[data objectForKey:@"imgpath"]];
            bannerInfo.title = [PCommonUtil decodeBase64:[data objectForKey:@"title"]];
            
            if([data objectForKey:@"songparameter"]){
                NSDictionary *spData = [data objectForKey:@"songparameter"];
                
                bannerInfo.songParameter = [BannerSongParameter initWithDictionary:spData];
                //[bannerInfo.songParameter log];
            }
            
        }
        
    }@catch (NSException *exception) {
        PLog(@"create BannerInfo failed...please check");
    }
    
    return bannerInfo;
}

- (id)init{
    self = [super init];
    if(self !=nil){
        self.bID = 0L;
        self.type = 0;
        self.title = @"";
        self.imagePhoto = @"";
        self.songParameter = nil;
    }
    
    return self;
}

-(void) log{
    //PLog(@"BannerInfo [id=%ld, type=%d, title=%@, imagePhoto=%@, songParameter=%@]", bID, type, title, imagePhoto, songParameter);
    PLog(@"BannerInfo [id=%ld, type=%d, title=%@, imagePhoto=%@ ]", bID, type, title, imagePhoto);
}

-(NSString *) description {
    
    
    return [NSString stringWithFormat:@"BannerInfo [id=%ld, type=%d, title=%@, imagePhoto=%@ songParameter=%@]", bID, type, title, imagePhoto,
            (songParameter == nil ? @"NULL":songParameter)];
}

@end
