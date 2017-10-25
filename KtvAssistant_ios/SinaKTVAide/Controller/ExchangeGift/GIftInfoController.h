//
//  GIftInfoController.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//



#import "BaseViewController.h"
#import "GiftInfoCell.h"

@interface GIftInfoController : BaseViewController <UITableViewDataSource,UITableViewDelegate,GiftInfoCellDelegate>
{
    NSString *ktvid;
    NSString *ktvname;
}

@property (nonatomic,retain) UITableView    *tableView;
@property (nonatomic,retain) UILabel        *lblTip;
@property (nonatomic,retain) NSMutableArray *arrGiftInfo;

-(id)init:(NSString *)_ktvid  ktvname:(NSString *)_ktvname;

@end
