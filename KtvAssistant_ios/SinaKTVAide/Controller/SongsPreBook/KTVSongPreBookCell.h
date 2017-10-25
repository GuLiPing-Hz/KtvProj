//
//  KTVSongPreBookCell.h
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-11-1.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SongInfo.h"

@interface KTVSongPreBookCell : UITableViewCell


@property (nonatomic, strong) IBOutlet UIImageView *songHasVodImage;
@property (nonatomic, strong) IBOutlet UILabel     *songNameLabel;
@property (nonatomic, strong) IBOutlet UILabel     *aritstNameLabel;
@property (nonatomic, strong) IBOutlet UIImageView *commentImage;
@property (nonatomic, strong) IBOutlet UIButton    *leftButton;
@property (nonatomic, strong) IBOutlet UIButton    *toTopButton;
@property (nonatomic, strong) IBOutlet UIButton    *deleteSongButton;

@property (nonatomic, strong) SongInfo *song;

-(IBAction)vodSongAction:(id)sender;//点播
-(IBAction)toTopAction:(id)sender;//置顶
-(IBAction)deleteSongAction:(id)sender;//删除
@end
