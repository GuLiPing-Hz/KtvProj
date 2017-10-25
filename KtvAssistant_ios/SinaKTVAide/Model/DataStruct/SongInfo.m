//
//  SongInfo.m
//  SinaKTVAide
//
//  Created by apple on 13-10-29.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "SongInfo.h"
#import "PCommonUtil.h"

@implementation SongInfo

@synthesize songId = _songId;
@synthesize songName = _songName;
@synthesize songArtist = _songArtist;
@synthesize songArtistPhoto = _songArtistPhoto;
@synthesize songIsScore = _songIsScore;
@synthesize isOrdered = _isOrdered;
@synthesize isPreOrder = _isPreOrder;

+(SongInfo *)initWithDictionary:(NSDictionary *)data{
    
    SongInfo *songInfo = nil;
    
    @try {
        if (data && [data isKindOfClass:[NSDictionary class]]) {
            songInfo = [[SongInfo alloc] init];
            songInfo.songId = [[data objectForKey:@"music_id"] longValue];
            songInfo.songName = [PCommonUtil decodeBase64:[data objectForKey:@"music_name"]];
            songInfo.songArtist = [PCommonUtil decodeBase64:[data objectForKey:@"music_singer"]];
            if([data objectForKey:@"music_photo"]){
                songInfo.songArtistPhoto = [PCommonUtil decodeBase64:[data objectForKey:@"music_photo"]];
            }
            songInfo.songIsScore = [[data objectForKey:@"music_isScore"] intValue];
            songInfo.isPreOrder = NO;
        }
    }
    @catch (NSException *exception) {
        PLog(@"create SongInfo failed...please check");
    }
    
    return songInfo;
}

-(void)Log{
    PLog(@"Print SongInfo: _songId(%ld), _songName(%@), _songArtist(%@), _songArtistPhoto(%@), _songIsScore(%d)", _songId, _songName, _songArtist, _songArtistPhoto ,_songIsScore);
}

-(NSString *)description{
    return [NSString stringWithFormat:@"Song [songID=%ld, songName=%@, songArtist=%@ songArtistPhoto=%@]", _songId, _songName, _songArtist, _songArtistPhoto];
}

@end
