//
//  RankingsViewCell.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/21.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TopicInfo.h"
#import "RankingTouchProtocol.h"
#import "UIButton+AFNetworking.h"

@interface RankingsViewCell : UITableViewCell

@property (weak, nonatomic) IBOutlet UIButton *mUiBtn1;
@property (weak, nonatomic) IBOutlet UILabel *mUiText1;
@property (weak, nonatomic) IBOutlet UIButton *mUiBtn2;
@property (weak, nonatomic) IBOutlet UILabel *mUiText2;

@property(weak,nonatomic) TopicInfo* mInfo1;
@property(weak,nonatomic) TopicInfo* mInfo2;

@property(weak,nonatomic) id<RankingTouchProtocol> mCallBack;

- (IBAction)onTouch1:(id)sender;
- (IBAction)onTouch2:(id)sender;

-(void)initWithDelegate:(id<RankingTouchProtocol>)callBack;
-(void)initWithTopicInfo:(TopicInfo*)info1 info2:(TopicInfo*)p2;
@end
