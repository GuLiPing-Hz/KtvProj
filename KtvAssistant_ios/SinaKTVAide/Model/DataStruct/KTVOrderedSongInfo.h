//
//  KTVOrderedSongInfo.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/7/3.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface KTVOrderedSongInfo : NSObject

@property(nonatomic,assign) int songId;
@property(nonatomic,strong) NSString* songname;
@property(nonatomic,strong) NSString* singername;
@property(nonatomic,assign) BOOL isHigher;

@end
