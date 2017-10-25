//
//  GiftInfoCell.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "GiftInfoCell.h"
#import "GiftExchangeController.h"

@implementation GiftInfoCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        
        self.selectionStyle = UITableViewCellSelectionStyleNone;
        
        imgGift = [[UIImageView alloc]initWithFrame:CGRectMake(15, 8, 44, 44)];
        imgGift.backgroundColor = [UIColor clearColor];
        imgGift.layer.cornerRadius = 4;
        imgGift.layer.masksToBounds = true;
        [self addSubview:imgGift];
        
        lblGiftName = [[UILabel alloc] initWithFrame:CGRectMake(74, 10, 65, 17)];
        lblGiftName.backgroundColor = [UIColor clearColor];
        lblGiftName.textColor = [UIColor blackColor];
        lblGiftName.font = [UIFont systemFontOfSize:16];
        [self addSubview:lblGiftName];
        
        lblGiftGold = [[UILabel alloc] initWithFrame:CGRectMake(74, 35, 50, 14)];
        lblGiftGold.backgroundColor = [UIColor clearColor];
        lblGiftGold.textColor = UIColorFromRGB(0xe4397d);
        lblGiftGold.font = [UIFont systemFontOfSize:14];
        [self addSubview:lblGiftGold];
        
        lblGold = [[UILabel alloc] init];
        lblGold.backgroundColor = [UIColor clearColor];
        lblGold.textColor = UIColorFromRGB(0x666666);
        lblGold.font = [UIFont systemFontOfSize:14];
        lblGold.text = @"金币";
        [self addSubview:lblGold];
        
        btnExchange = [[UIButton alloc] initWithFrame:CGRectMake(_MainScreen_Width-54, 15, 50, 30)];
        btnExchange.backgroundColor = UIColorFromRGB(0xe4397d);
        btnExchange.layer.cornerRadius = 2;
        btnExchange.layer.masksToBounds = YES;
        [btnExchange setTitle:@"兑换" forState:UIControlStateNormal];
        [btnExchange addTarget:self action:@selector(exchange) forControlEvents:UIControlEventTouchUpInside];
        
        [self addSubview:btnExchange];
    }
    return self;
}

-(void)setModel:(KTVGiftInfo *)model
{
    _model = model;
    lblGiftName.text = model.giftName;
    [lblGiftName sizeToFit];
    
    lblGiftGold.text = [[NSString alloc] initWithFormat:@"%d",model.giftGold,nil];
    [lblGiftGold sizeToFit];
    
    lblGold.frame=CGRectMake(lblGiftGold.frame.origin.x + lblGiftGold.frame.size.width + 1, 35, 30, 14);
    [lblGold sizeToFit];
    
    [imgGift setImageWithURL:[NSURL URLWithString:model.giftSmallImg]  placeholderImage:[UIImage imageNamed:@"pic_top100_admin.png"]];
}

-(void)exchange
{
    [_delegate ShowExchange:_model];
}

- (void)awakeFromNib
{
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

@end
