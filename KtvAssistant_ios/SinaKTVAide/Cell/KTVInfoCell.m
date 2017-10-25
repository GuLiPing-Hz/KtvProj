//
//  KTVInfoCell.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-23.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "KTVInfoCell.h"

@implementation KTVInfoCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        
        self.selectedBackgroundView = [[UIView alloc]init];
        self.selectedBackgroundView.backgroundColor = UIColorFromRGB(0xe0e0e0);
        
        // Initialization code
        lblKtvName = [[UILabel alloc] initWithFrame:CGRectMake(20, 11, 200, 32)];
        lblKtvName.font = [UIFont systemFontOfSize:16];
        lblKtvName.textColor = [UIColor blackColor];
        lblKtvName.backgroundColor = [UIColor clearColor];
        [self addSubview:lblKtvName];

        lblKtvAddress = [[UILabel alloc] initWithFrame:CGRectMake(20, 35, _MainScreen_Width-45, 30)];
        lblKtvAddress.font = [UIFont systemFontOfSize:14];
        lblKtvAddress.backgroundColor = [UIColor clearColor];
        lblKtvAddress.textColor = UIColorFromRGB(0x9c9c9c);
        lblKtvAddress.lineBreakMode = UILineBreakModeWordWrap;
        [lblKtvAddress setNumberOfLines:0];
        
        [self addSubview:lblKtvAddress];
        
        btnMore = [[UIButton alloc] initWithFrame:CGRectMake(_MainScreen_Width - 17 - 8, 23, 8, 14)];
        btnMore.backgroundColor = [UIColor clearColor];
        [btnMore setImage:[UIImage imageNamed:@"list_pic_more.png"] forState:UIControlStateNormal];
        //[btnMore ]
        [self addSubview:btnMore];
    }
    return self;
}

-(void)setModel:(KTVInfo *)model
{
    _model = model;
    lblKtvName.text = model.ktvName;
    lblKtvAddress.text = model.ktvAddress;
    
    [lblKtvName sizeToFit];
    [lblKtvAddress sizeToFit];
}
-(void)showGift
{
    
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

@end
