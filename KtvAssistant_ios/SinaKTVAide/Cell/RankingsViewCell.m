//
//  RankingsViewCell.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/21.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "RankingsViewCell.h"

@implementation RankingsViewCell

- (void)awakeFromNib {
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (IBAction)onTouch1:(id)sender {
    if([self.mCallBack respondsToSelector:@selector(onTouchIn:title:)])
    {
        if(self.mInfo1 != nil)
            [self.mCallBack onTouchIn:self.mInfo1.topicId title:self.mInfo1.topicTitle];
    }
}

- (IBAction)onTouch2:(id)sender {
    if([self.mCallBack respondsToSelector:@selector(onTouchIn:title:)])
    {
        if(self.mInfo2 != nil)
            [self.mCallBack onTouchIn:self.mInfo2.topicId title:self.mInfo2.topicTitle];
    }
}

-(void)initWithDelegate:(id<RankingTouchProtocol>)callBack
{
    self.mCallBack = callBack;
}

-(void)initWithTopicInfo:(TopicInfo*)info1 info2:(TopicInfo*)p2
{
    self.mInfo1 = info1;
    self.mInfo2 = p2;
    
    [self.mUiBtn1 setImageWithURL:[[NSURL alloc] initWithString:self.mInfo1.topicImgIcon] placeholderImage:[UIImage imageNamed:@"load_ic_0"]];
    [self.mUiText1 setText:self.mInfo1.topicTitle];
    
    if(self.mInfo2 == nil)
    {
        [self.mUiBtn2 setHidden:YES];
        [self.mUiText2 setHidden:YES];
    }
    else
    {
        [self.mUiBtn2 setHidden:NO];
        [self.mUiText2 setHidden:NO];
        
        [self.mUiBtn2 setImageWithURL:[[NSURL alloc] initWithString:self.mInfo2.topicImgIcon] placeholderImage:[UIImage imageNamed:@"load_ic_0"]];
        [self.mUiText2 setText:self.mInfo2.topicTitle];
    }
}
@end
