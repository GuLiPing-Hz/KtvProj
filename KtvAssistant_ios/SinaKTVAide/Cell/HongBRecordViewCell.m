//
//  HongBRecordViewCell.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/22.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "HongBRecordViewCell.h"

@implementation HongBRecordViewCell

- (void)awakeFromNib {
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

-(void)initWithHBInfo:(KTVHongBaoInfo*)info
{
    self.mInfo = info;
    if(self.mInfo)
    {
        if(self.mInfo.hbnclass == 2)
            [self.mUiMissionName setText:[NSString stringWithFormat:@"来自:%@",self.mInfo.hbmission]];
        else
            [self.mUiMissionName setText:self.mInfo.hbmission];
        [self.mUiGold setText:[NSString stringWithFormat:@"%d金币",info.gold]];
        
        //if()
        [self.mUiTime setText:info.hbtime];
    }
}

@end
