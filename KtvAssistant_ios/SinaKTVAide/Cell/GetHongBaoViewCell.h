//
//  GetHongBaoViewCell.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/21.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "HongBaoCellProtocol.h"

@interface GetHongBaoViewCell : UITableViewCell

@property (weak, nonatomic) IBOutlet UILabel *mUiMission;
@property (weak, nonatomic) IBOutlet UILabel *mUiTime;
@property (weak, nonatomic) IBOutlet UIButton *mUiButton;

- (IBAction)onGetHongBao:(id)sender;

@property(weak,nonatomic) KTVHongBaoInfo* mInfo;
@property(weak,nonatomic) id<HongBaoCellProtocol> mCallBack;

-(void)initWithHBInfo:(KTVHongBaoInfo*)info;
@end
