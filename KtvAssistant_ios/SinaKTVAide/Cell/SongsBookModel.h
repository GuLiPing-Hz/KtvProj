//
//  SongsBookModel.h
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-4.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SongInfo.h"

#define TYPECELL_CommonOrderSong 1
#define TYPECELL_OrderedSong 2
#define TYPECELL_CollectSong 3

@interface SongsBookModel : NSObject

@property (nonatomic, assign) BOOL isHigher;
@property (nonatomic, assign) int cellType;
@property (nonatomic, strong) SongInfo * songInfo;


@end
