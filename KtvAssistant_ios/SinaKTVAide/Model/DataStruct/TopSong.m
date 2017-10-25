//
//  TopSong.m
//  SinaKTVAide
//
//  Created by sp on 13-12-9.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "TopSong.h"

@implementation TopSong

@synthesize tsID;
@synthesize score;
@synthesize address;
@synthesize song;
@synthesize user;



+(TopSong *)initWithDefaultValue{
    TopSong *topSong = [[TopSong alloc] init];
    topSong.tsID = 0L;
    topSong.score = 0;
    topSong.address = @"";
    topSong.song = nil;
    topSong.user = nil;
    
    return topSong;
}

+(TopSong *)initWithDictionary:(NSDictionary *)data{
    TopSong *topSong = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            topSong = [[TopSong alloc] init];
            
            topSong.tsID = [[data objectForKey:@"workid"] longValue];
            topSong.score = [[data objectForKey:@"score"] intValue];
            topSong.address = [PCommonUtil decodeBase64:[data objectForKey:@"address"]];//[NSString stringWithFormat:@"%@",[data objectForKey:@"headphoto"]];
            
            if([data objectForKey:@"song"]){
                
                NSDictionary *songData = [data objectForKey:@"song"];
                topSong.song = [SongInfo initWithDictionary:songData];
            }
            
            if([data objectForKey:@"user"]){
                
                NSDictionary *userData = [data objectForKey:@"user"];
                topSong.user = [User initWithUserDictionary:userData];
            }
            
            //[topSong log];
        }
        
    }@catch (NSException *exception) {
        PLog(@"create TopSong failed...please check");
    }
    
    return topSong;
}

-(void) log{
    PLog(@"TopSong [TopSongID=%ld, score=%d, address=%@, song=%@, user=%@]", tsID, score, address,
         (song == nil ? @"NULL" :[song description]), (user == nil ? @"NULL" : [user description]) );
}


@end
