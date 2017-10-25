//
//  HongBRecordViewCell.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/22.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "KTVHongBaoInfo.h"

@interface HongBRecordViewCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *mUiMissionName;
@property (weak, nonatomic) IBOutlet UILabel *mUiGold;
@property (weak, nonatomic) IBOutlet UILabel *mUiTime;

@property (weak,nonatomic) KTVHongBaoInfo* mInfo;

-(void)initWithHBInfo:(KTVHongBaoInfo*)info;
@end
