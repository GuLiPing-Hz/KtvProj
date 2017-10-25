//
//  KTVTopSongCell.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/20.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SongOperationProtocol.h"
#import "TopSongCellModel.h"

@interface KTVTopSongCell : UITableViewCell

@property (weak, nonatomic) IBOutlet UIImageView *mUISongStatus;
@property (weak, nonatomic) IBOutlet UILabel *mUISongName;
@property (weak, nonatomic) IBOutlet UILabel *mUISongSinger;
@property (weak, nonatomic) IBOutlet UIImageView *mUIPic;
@property (weak, nonatomic) IBOutlet UILabel *mUIUserName;
@property (weak, nonatomic) IBOutlet UILabel *mUIRoom;
@property (weak, nonatomic) IBOutlet UILabel *mUIScore;
@property (weak, nonatomic) IBOutlet UIButton *mUIOrder;
@property (weak, nonatomic) IBOutlet UIView *mUIOperation;

@property(weak,nonatomic) TopSongCellModel* mInfo;
@property(strong,nonatomic) id<SongOperation> mCallBack;

- (IBAction)onTop:(id)sender;
- (IBAction)onCollect:(id)sender;
- (IBAction)onOrderOrCancel:(id)sender;

-(void)setTopSongInfo:(TopSongCellModel*)info;
@end
