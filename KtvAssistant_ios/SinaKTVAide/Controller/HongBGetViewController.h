//
//  KTVViewController.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/22.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "BaseWaitViewController.h"
#import "GetHongBaoViewCell.h"
#import "CommonDialogView.h"
#import "DeformationButton.h"

@interface HongBGetViewController : BaseWaitViewController<UITableViewDataSource,UITableViewDelegate,HongBaoCellProtocol,CustomDialogDelegate>

@property(nonatomic,strong) UITableView* mUiTableView;
//保存所有数据
@property(nonatomic,strong) NSMutableArray* mDataList;

@property (strong, nonatomic) IBOutlet GetHongBaoViewCell *mUiTableCell;
@property (strong, nonatomic) IBOutlet UIView *mUiSectionView;
@property (weak, nonatomic) IBOutlet UILabel *mUIClassName;
@property (weak, nonatomic) IBOutlet UIButton *mUiSectionBtn;
@property (weak, nonatomic) IBOutlet UILabel *mUiSectionTip;
@property (weak, nonatomic) IBOutlet UIImageView *mUiSectionStatus;
- (IBAction)onTouchSection:(id)sender;

@property (strong, nonatomic) UIView *mUiHongBao;
@property (strong, nonatomic) UILabel *mUiHbName;
@property (strong, nonatomic) UILabel *mUiHbMission;
@property (strong, nonatomic) UILabel *mUiHbGold;

@property (strong, nonatomic) DeformationButton *mUiGetHBBtn;
@property(strong,nonatomic) CommonDialogView* mDialogView;
@end
