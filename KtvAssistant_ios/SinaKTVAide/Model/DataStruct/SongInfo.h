//
//  SongInfo.h
//  SinaKTVAide
//
//  Created by apple on 13-10-29.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SongInfo : NSObject

@property (nonatomic, assign) long songId;
@property (nonatomic, strong) NSString *songName;
@property (nonatomic, strong) NSString *songArtist;
@property (nonatomic, strong) NSString *songArtistPhoto;
@property (nonatomic, assign) int songIsScore;//1-可评分歌曲，0-不可评分歌曲
@property (nonatomic, assign) BOOL isOrdered;//是否是已点歌曲
@property (nonatomic, assign) BOOL isPreOrder;//是否已预点

+(SongInfo *)initWithDictionary:(NSDictionary *)data;
-(void)Log;

@end
