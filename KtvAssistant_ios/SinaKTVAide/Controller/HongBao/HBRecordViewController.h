//
//  HongBRecordViewController.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/22.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "BaseWaitViewController.h"
#import "HongBRecordViewCell.h"

@interface HBRecordViewController : BaseWaitViewController<UITableViewDataSource,UITableViewDelegate>

@property (nonatomic,strong) UITableView* mUiTableView;
@property (nonatomic,strong) NSMutableArray* mDataList;

@property (strong, nonatomic) IBOutlet UIView *mUiSectionView;
@property (weak, nonatomic) IBOutlet UILabel *mUiSectionName;
@property (weak, nonatomic) IBOutlet UILabel *mUiNumber;
@property (weak, nonatomic) IBOutlet UIImageView *mUiSectionStatus;
@property (weak, nonatomic) IBOutlet UIButton *mUiSectionBtn;
@property (strong, nonatomic) IBOutlet HongBRecordViewCell *mUiTableViewCell;

- (IBAction)onTouchSection:(id)sender;
@end
