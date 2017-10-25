//
//  TopSongCellModel.h
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-13.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TopSong.h"

@interface TopSongCellModel : NSObject

@property (nonatomic, strong) TopSong * topSongInfo;
@property (nonatomic, assign) BOOL isHigher;

@end
