//
//  PDatabaseManager.h
//  SinaKTVAide
//
//  Created by apple on 13-11-6.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "PFileManager.h"
#import "FMDatabase.h"
#import "SongInfo.h"

@interface PDatabaseManager : PFileManager

@property (nonatomic, retain) FMDatabase *db;

+(PDatabaseManager *)GetInstance;

-(void)insertSongInfo:(SongInfo *)tSongInfo;
-(void)deleteSongInfo:(long)songId;
-(SongInfo *)getSongInfoBySongId:(long)songId;
-(NSMutableArray *)getSongInfoList:(int)tCount;

@end
