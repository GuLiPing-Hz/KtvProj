//
//  TopicInfoCell.m
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-16.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "TopicInfoCell.h"
#import "CommenTools.h"
#import "KTVBaseConfig.h"

@implementation TopicInfoCell

#define RIGHTTO_MARGIN_RIGHT 40

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        // Initialization code ;
        //self.selectionStyle = UITableViewCellSelectionStyleNone;
        self.backgroundColor = UIColorFromRGB(0xffffff);
        self.selectedBackgroundView = [[UIView alloc]init];
        self.selectedBackgroundView.backgroundColor = UIColorFromRGB(0xe0e1e5);
        
        _topicTitle = [[UILabel alloc]init];
        _topicTitle.backgroundColor = [UIColor clearColor];
        _topicTitle.frame = CGRectMake(15, 5, 200, 30);
        _topicTitle.font = [UIFont systemFontOfSize:15];
        _topicTitle.textColor = UIColorFromRGB(0x212121);
        [self.contentView addSubview:_topicTitle];
        
        UIImage* imgRightTo = [UIImage imageNamed:@"list_pic_more.png"];
        self.mUiRightTo = [[UIImageView alloc] initWithImage:imgRightTo];
        
        CGFloat x_center = _MainScreen_Width - ABSOLUTE2RELATIVE(RIGHTTO_MARGIN_RIGHT);
        [self.mUiRightTo setFrame:CGRectMake(x_center-10, self.contentView.center.y-7, 8, 14)];
        [self.contentView addSubview:self.mUiRightTo];//
        
        self.mUiLine = [[UIView alloc] initWithFrame:CGRectMake(0, self.contentView.frame.origin.x+self.contentView.frame.size.height-1, _MainScreen_Width, 1)];
        [self.mUiLine setBackgroundColor:UIColorFromRGB(0xebebeb)];
        [self.contentView addSubview:self.mUiLine];
    }
    return self;
}

- (void)setTopicInfo:(TopicInfo *)topicInfo{
    _topicInfo = topicInfo;
    
    _topicTitle.text = topicInfo.topicTitle;
    
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

@end
