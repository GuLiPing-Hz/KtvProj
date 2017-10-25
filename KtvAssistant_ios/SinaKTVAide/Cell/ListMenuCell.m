//
//  ListMenuCell.m
//  SinaKTVAide
//
//  Created by Li Pan on 13-12-10.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "ListMenuCell.h"

@implementation ListMenuCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self)
    {
        self.backgroundColor = UIColorFromRGB(0xebecf1);
        // cell背景
        _cellBackground = [[UIButton alloc] init];
        _cellBackground.backgroundColor = [UIColor clearColor];
        [_cellBackground setImage:[UIImage imageNamed:@"shouye_list.png"] forState:UIControlStateNormal];
        _cellBackground.userInteractionEnabled = NO;
        [self.contentView addSubview:_cellBackground];
        // 榜单头像
        _headImage = [[UIImageView alloc] init];
        _headImage.backgroundColor = [UIColor clearColor];
        [self.contentView addSubview:_headImage];
        // 榜单标题
        _listName = [[UILabel alloc] init];
        _listName.font = [UIFont systemFontOfSize:18.0f];
        _listName.backgroundColor = [UIColor clearColor];
        [self.contentView addSubview:_listName];
        // 榜单子标题
        _detailListName = [[UILabel alloc] init];
        _detailListName.font = [UIFont systemFontOfSize:12.0f];
         _detailListName.backgroundColor = [UIColor clearColor];
        [self.contentView addSubview:_detailListName];
        // 提示下一级
        _detailIcon = [[UIImageView alloc] init];
        _detailIcon.image = [UIImage imageNamed:@"btn_shouye_next.png"];
        _detailIcon.backgroundColor = [UIColor clearColor];
        [self.contentView addSubview:_detailIcon];
    }
    return self;
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    
    _cellBackground.frame = CGRectMake(4, 6, 312,60);
    _headImage.frame = CGRectMake(8, 9, 53, 53);
    _listName.frame = CGRectMake(6+53+10, 10, 160, 30);
    _detailListName.frame = CGRectMake(6+53+10, 38, 250, 20);
    _detailIcon.frame = CGRectMake(self.frame.size.width-8-15, (self.frame.size.height-12)/2+3, 8, 12);
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

@end
