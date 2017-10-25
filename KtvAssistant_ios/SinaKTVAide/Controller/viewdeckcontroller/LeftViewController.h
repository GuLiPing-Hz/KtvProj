//
//  LeftViewController.h
//  SinaKTVAide
//
//  Created by Li Pan on 13-11-30.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "DDMenuController.h"

@class KTVMainViewController;
@interface LeftViewController : BaseViewController<UITableViewDataSource,UITableViewDelegate>

@property (nonatomic,retain) UITableView *menuTableView;
@property (nonatomic,retain) UIButton *headView;
@property (nonatomic,retain) UIImageView *headImageView;
@property (nonatomic,retain) UIImageView *genderImageView;
@property (nonatomic,retain) UILabel *titleName;
@property (nonatomic,retain) UILabel *titleIdx;
@property (nonatomic,retain) UILabel *titleGold;
@property (nonatomic,retain) UILabel* mUiLabelSongLast;
@property (nonatomic,retain) UIImageView *bgImage;
@property (nonatomic,retain) NSString *point;
@property (nonatomic,retain) NSString *standardHeadPhotoString;
@property (nonatomic,retain) NSString *is_standardHeadPhotoString;
@property (nonatomic,weak) KTVMainViewController* mIndexPage;

-(void)setIndexPage:(KTVMainViewController*) page;
@end
