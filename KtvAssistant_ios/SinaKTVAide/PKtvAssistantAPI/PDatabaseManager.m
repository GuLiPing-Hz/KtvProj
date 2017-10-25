//
//  PDatabaseManager.m
//  SinaKTVAide
//
//  Created by apple on 13-11-6.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "PDatabaseManager.h"

@implementation PDatabaseManager

@synthesize db = _db;

+(PDatabaseManager *)GetInstance{
    
    static PDatabaseManager *instance = nil;
    @synchronized(self){
        if (nil == instance) {
            instance = [[PDatabaseManager alloc] init];
        }
    }
    return instance;
    
}

-(id)init{
    
    self = [super init];
    if (self) {
        
        NSString *cacheHomeDir = [self getCacheHomeDirectory];
        NSString *dbpath = [cacheHomeDir stringByAppendingPathComponent:@"user.sqlite"];
        PLog(@"dbpath: %@", dbpath);
        
        _db = [FMDatabase databaseWithPath:dbpath];
        [_db open];
        
        //user info
        [_db executeUpdate:@"create table if not exists SONG_INFO (songid integer not null, songname text, songartist text, songisscore integer, isordered integer, createtime integer)"];
        
        [_db close];
    }
    
    return self;
}

-(void)insertSongInfo:(SongInfo *)tSongInfo{
    
    NSString *sql = @"insert into SONG_INFO (songid, songname, songartist, songisscore, isordered, createtime) values (?, ?, ?, ?, ?, ?)";
    PLog(@"sql: %@", sql);
    
    [_db open];
    
    NSString *checksql = [NSString stringWithFormat:@"select * from SONG_INFO where songid = %ld ", tSongInfo.songId];
    PLog(@"checksql: %@", checksql);
    
    //
    BOOL isExists = NO;
    FMResultSet *rs = [_db executeQuery:checksql];
    while ([rs next]) {
        
        isExists = YES;
        break;
    }
    
    if (!isExists) {
        
        NSNumber *numSongId = [NSNumber numberWithLongLong:tSongInfo.songId];
        NSNumber *numSongIsScore = [NSNumber numberWithInt:tSongInfo.songIsScore];
        NSNumber *numIsOrdered = [NSNumber numberWithBool:tSongInfo.isOrdered];
        NSDate *nowDate = [NSDate date];
        NSNumber *numCreateTime = [NSNumber numberWithLong:[nowDate timeIntervalSince1970]];
        
        [_db executeUpdate:sql, numSongId, tSongInfo.songName, tSongInfo.songArtist, numSongIsScore, numIsOrdered, numCreateTime];
        
    }
    
    [_db close];
    
}

-(void)deleteSongInfo:(long)songId{
    
    NSString *sql = [NSString stringWithFormat:@"delete from SONG_INFO where songid = %ld ", songId];
    PLog(@"sql: %@", sql);
    
    [_db open];
    [_db executeUpdate:sql];
    [_db close];
    
}

-(SongInfo *)getSongInfoBySongId:(long)songId{
    
    SongInfo *psonginfo = nil;
    
    NSString *sql = [NSString stringWithFormat:@"select * from SONG_INFO where songid ＝ '%ld' ", songId];
    PLog(@"sql: %@", sql);
    
    [_db open];
    
    FMResultSet *rs = [_db executeQuery:sql];
    while ([rs next]) {
        
        long psongid = [rs longForColumn:@"songid"];
        NSString *psongname = [rs stringForColumn:@"songname"];
        NSString *psongartist = [rs stringForColumn:@"songartist"];
        int nsongisscore = [rs intForColumn:@"songisscore"];
        BOOL bisordered = [rs boolForColumn:@"isordered"];
        
        SongInfo *psonginfo = [[SongInfo alloc] init];
        psonginfo.songId = psongid;
        psonginfo.songName = psongname;
        psonginfo.songArtist = psongartist;
        psonginfo.songIsScore = nsongisscore;
        psonginfo.isOrdered = bisordered;
        
        [psonginfo Log];
        
    }
    
    [_db close];
    
    return psonginfo;
    
}

-(NSMutableArray *)getSongInfoList:(int)tCount{
    
    NSString *sql = [NSString stringWithFormat:@"select * from SONG_INFO order by createtime asc limit %d ", tCount];
    PLog(@"sql: %@", sql);
    
    NSMutableArray *songInfoList = [[NSMutableArray alloc] init];
    
    [_db open];
    
    FMResultSet *rs = [_db executeQuery:sql];
    while ([rs next]) {
        
        long psongid = [rs longForColumn:@"songid"];
        NSString *psongname = [rs stringForColumn:@"songname"];
        NSString *psongartist = [rs stringForColumn:@"songartist"];
        int nsongisscore = [rs intForColumn:@"songisscore"];
        BOOL bisordered = [rs boolForColumn:@"isordered"];
        
        SongInfo *psonginfo = [[SongInfo alloc] init];
        psonginfo.songId = psongid;
        psonginfo.songName = psongname;
        psonginfo.songArtist = psongartist;
        psonginfo.songIsScore = nsongisscore;
        psonginfo.isOrdered = bisordered;
        
//        [psonginfo Log];
        
        [songInfoList addObject:psonginfo];
    }
    
    [_db close];
    
    return songInfoList;
    
}

@end
