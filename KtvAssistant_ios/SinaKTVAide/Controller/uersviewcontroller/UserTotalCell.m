//
//  UserTotalCell.m
//  SinaKTVAide
//
//  Created by Li Pan on 13-12-6.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "UserTotalCell.h"

@implementation UserTotalCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self)
    {
        // cell背景图
        _backgroundImage.image = [UIImage imageNamed:@"list_blue.png"];
        _backgroundImage.backgroundColor  =[UIColor clearColor];
        [_backgroundImage addSubview:_labKind];
        [_backgroundImage addSubview:_labTotal];
        [_backgroundImage addSubview:_labReceive];
        [self.contentView addSubview:_backgroundImage];
        
        self.labKind.textColor = UIColorFromRGB(0x474747);
        self.labKind.font = [UIFont systemFontOfSize:14.0f];
        
        self.labTotal.textColor = UIColorFromRGB(0x474747);
        self.labTotal.font = [UIFont systemFontOfSize:14.0f];
        
        self.labReceive.textColor = UIColorFromRGB(0xea9111);
        self.labReceive.font = [UIFont systemFontOfSize:13.0f];
    }
    return self;
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    self.labKind.frame = CGRectMake(30, 8, 100, 30);
    self.labTotal.frame = CGRectMake(152 , 8, 100, 30);
    self.labReceive.frame = CGRectMake(230, 8, 100, 30);
    self.backgroundImage.frame = CGRectMake(16, 0, _MainScreen_Width-16-10, 44);
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

@end
