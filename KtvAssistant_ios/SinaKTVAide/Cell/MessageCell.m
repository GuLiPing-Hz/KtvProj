//
//  MessageCell.m
//  SinaKTVAide
//
//  Created by Li Pan on 13-12-20.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "MessageCell.h"
#import "KTVBaseConfig.h"

@implementation MessageCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
       
        self.frame = CGRectMake(20, 0, _MainScreen_Width, 50);

        // 消息
        _labMessage = [[UILabel alloc] init];
        _labMessage.textColor = UIColorFromRGB(0x2f2f2f);
        _labMessage.font = [UIFont systemFontOfSize:12.0f];
        _labMessage.backgroundColor = [UIColor clearColor];
        [self.contentView addSubview:_labMessage];
        
        // 未读消息背景
        _badgeImage = [[UIImageView alloc] init];
        _badgeImage.backgroundColor = [UIColor clearColor];
        _badgeImage.image = [UIImage imageNamed:@"red.png"];
        [self.contentView addSubview:_badgeImage];
        
        // 积分领取背景
        _bgImageView = [[UIImageView alloc] init];
        _bgImageView.backgroundColor = [UIColor clearColor];
        _bgImageView.image = [UIImage imageNamed:@"icon_xitongxiaoxi.png"];
        [self.contentView addSubview:_bgImageView];
        
        // 积分领取标签
        _scoreLabel = [[UILabel alloc] init];
        _scoreLabel.textColor = UIColorFromRGB(0xffffff);
        _scoreLabel.font = [UIFont systemFontOfSize:12.0f];
        _scoreLabel.backgroundColor = [UIColor clearColor];
        _scoreLabel.text = @"";
        [self.contentView addSubview:_scoreLabel];
        
        
        // 日期时间
        _labDate = [[UILabel alloc] init];
        _labDate.font = [UIFont systemFontOfSize:9.0f];
        _labDate.backgroundColor = [UIColor clearColor];
        _labDate.textColor = UIColorFromRGB(0x888888);
        [self.contentView addSubview:_labDate];
        
        // 分隔线
//        _separator = [[UIImageView alloc] init];
//        _separator.backgroundColor = [UIColor clearColor];
//        _separator.image = [[UIImage imageNamed:@"list_line.png"] stretchableImageWithLeftCapWidth:1 topCapHeight:0];
//        [self.contentView addSubview:_separator];
        
    }
    return self;
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    _labMessage.frame = CGRectMake(10, 27, self.frame.size.width-20, 20);
    _bgImageView.frame = CGRectMake(10, 5, 54, 20);
    _scoreLabel.frame = CGRectMake(13, 5, 80, 20);
    _labDate.frame = CGRectMake(self.frame.size.width-30-10, 8, 30, 15);
    _badgeImage.frame = CGRectMake(2, 12, 6, 6);
//    _separator.frame = CGRectMake(0, 49, _MainScreen_Width, 1);
}

// 日期格式
- (NSString *)getDate:(NSString *)dateString
{
    NSString *newDateString = [dateString substringWithRange:NSMakeRange(5, 5)];
    return newDateString;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

@end
