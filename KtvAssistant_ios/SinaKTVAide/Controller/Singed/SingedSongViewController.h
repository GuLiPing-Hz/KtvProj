//
//  SingedSongViewController.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/12.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "BaseWaitViewController.h"
#import "MONActivityIndicatorView.h"

@interface SingedSongViewController : BaseWaitViewController<UITableViewDataSource,UITableViewDelegate>

@property (strong,nonatomic) UIView* mUiLine;
@property (strong,nonatomic) NSMutableArray* mSongRecord;
@property (strong, nonatomic) IBOutlet UIView *mUiUserInfo;
@property (weak, nonatomic) IBOutlet UIImageView* mUiImgViewHead;
@property (weak, nonatomic) IBOutlet UILabel* mUiLabelName;
@property (weak, nonatomic) IBOutlet UILabel* mUiLabelIdx;
@property (weak, nonatomic) IBOutlet UILabel* mUiLabelTotalScore;

@property (nonatomic,strong) UITableView* mUiTableView;
@property (strong, nonatomic) IBOutlet UITableViewCell *mUiTableCell;

-(void)onUserSongTotalScoreChange:(int) score;

@end
