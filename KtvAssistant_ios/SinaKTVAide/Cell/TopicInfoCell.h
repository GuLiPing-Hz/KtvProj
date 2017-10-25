//
//  TopicInfoCell.h
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-16.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TopicInfo.h"

@interface TopicInfoCell : UITableViewCell
{
    
    UILabel * _topicTitle;
}
@property (nonatomic,strong) UIView* mUiLine;
@property (nonatomic,strong) UIImageView* mUiRightTo;//向右指示器

@property (nonatomic, strong) TopicInfo * topicInfo;

@end
