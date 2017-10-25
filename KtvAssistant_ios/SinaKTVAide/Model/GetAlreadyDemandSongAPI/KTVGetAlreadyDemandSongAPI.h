//
//  KTVGetAlreadyDemandSongAPI.h
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-11-4.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>


@protocol KTVGetAlreadyDemandSongAPIDelegate <NSObject>

-(void)getAlreadyDemandSongWithSuccessResult:(id)result;
-(void)getAlreadyDemandSongWithFaildResult:(id)result;

@end

@interface KTVGetAlreadyDemandSongAPI : NSObject
{
    
}

@property (nonatomic, assign) id<KTVGetAlreadyDemandSongAPIDelegate> delegate;
@property (nonatomic, strong) NSMutableArray                        *songdataArray;

-(NSString *)getGetOrderedSongListUrl:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword;
-(void)doGetOrderedSongList:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword;

@end
