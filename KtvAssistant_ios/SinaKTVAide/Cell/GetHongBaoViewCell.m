//
//  GetHongBaoViewCell.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/21.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "GetHongBaoViewCell.h"

@implementation GetHongBaoViewCell

- (void)awakeFromNib {
    // Initialization code
    [self.mUiButton setImage:[UIImage imageNamed:@"receive_btn_1"] forState:UIControlStateHighlighted];
    [self.mUiButton setImage:[UIImage imageNamed:@"unfinished_btn_0"] forState:UIControlStateDisabled];
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
            [self.mUiMission setText:[NSString stringWithFormat:@"来自:%@",self.mInfo.hbmission]];
        else
            [self.mUiMission setText:self.mInfo.hbmission];
        [self.mUiTime setText:self.mInfo.hbtime];
        switch (self.mInfo.hbstatus) {
            case hongbao_todo:
                [self.mUiTime  setHidden:YES];
                [self.mUiButton setEnabled:NO];
                break;
            case hongbao_done:
                [self.mUiTime setHidden:NO];
                [self.mUiButton setEnabled:YES];
                break;
            case hongbao_finish:
                [self.mUiTime setHidden:NO];
                break;
                
            default:
                break;
        }
    }
}
- (IBAction)onGetHongBao:(id)sender {
    if(self.mCallBack && [self.mCallBack respondsToSelector:@selector(onTouchGetHB:)])
    {
        [self.mCallBack onTouchGetHB:self.mInfo];
    }
}
@end
