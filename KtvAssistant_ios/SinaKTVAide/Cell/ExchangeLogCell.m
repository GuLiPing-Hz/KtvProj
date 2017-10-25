//
//  ExchangeLogCell.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-28.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "ExchangeLogCell.h"

@implementation ExchangeLogCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier  cellType:(NSInteger)type
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        self.backgroundColor = [UIColor whiteColor];
        CGFloat height = self.frame.size.height;
        [self setFrame:CGRectMake(0, 0, _MainScreen_Width, height)];
        logType = type;
        lblGiftName = [[UILabel alloc] init];
        lblGiftName.textColor = UIColorFromRGB(0xe4397d);
        lblGiftName.backgroundColor = [UIColor clearColor];
        lblGiftName.font = [UIFont systemFontOfSize:16];
        [self  addSubview:lblGiftName];
        
        lblNumAndCost = [[UILabel alloc] init];
        lblNumAndCost.textColor = [UIColor blackColor];
        lblNumAndCost.backgroundColor = [UIColor clearColor];
        lblNumAndCost.font = [UIFont systemFontOfSize:16];
        [self  addSubview:lblNumAndCost];
        
        lblCode = [[UILabel alloc] init];
        lblCode.textColor = UIColorFromRGB(0x666666);
        lblCode.backgroundColor = [UIColor clearColor];
        lblCode.font = [UIFont systemFontOfSize:16];
        [self  addSubview:lblCode];
        
        lblKtvName = [[UILabel alloc] init];
        lblKtvName.textColor = UIColorFromRGB(0x666666);
        lblKtvName.backgroundColor = [UIColor clearColor];
        lblKtvName.font = [UIFont systemFontOfSize:14];
        [self  addSubview:lblKtvName];
        
        lblbeginDate = [[UILabel alloc] init];
        lblbeginDate.textColor = UIColorFromRGB(0x9c9c9c);
        lblbeginDate.backgroundColor = [UIColor clearColor];
        lblbeginDate.font = [UIFont systemFontOfSize:12];
        [self  addSubview:lblbeginDate];
        
        lblendDate = [[UILabel alloc] init];
        lblendDate.textColor = UIColorFromRGB(0xe4397d);
        lblendDate.backgroundColor = [UIColor clearColor];
        lblendDate.font = [UIFont systemFontOfSize:12];
        [self  addSubview:lblendDate];
        
        
        UIImageView *line = [[UIImageView alloc] initWithFrame:CGRectMake(0, 119, self.frame.size.width, 1)];
        line.backgroundColor = UIColorFromRGB(0xc6c6c7);
        [self  addSubview:line];
        
        imgType = [[UIImageView alloc] initWithFrame:CGRectMake(0, 57, 63, 63)];
        imgType.backgroundColor = [UIColor clearColor];
        if (type == 0)
            [imgType setImage:[UIImage imageNamed:@"gift_tag_reg.png"]];
        else if (type == 1)
            [imgType setImage:[UIImage imageNamed:@"gift_tag_receive.png"]];
        else
            [imgType setImage:[UIImage imageNamed:@"gift_tag_over.png"]];
        [self  addSubview:imgType];
    }
    return self;
}

-(void)setModel:(ExchangeLog *)model
{
    CGSize size;
    
    lblGiftName.text = model.giftName;
    size = [lblGiftName.text sizeWithFont:[UIFont systemFontOfSize:16] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
    lblGiftName.frame = CGRectMake(20, 13, size.width, size.height);
    
    lblNumAndCost.text = [[NSString alloc] initWithFormat:@"%@%@  价值%@金币",model.giftNum,model.unit,model.costGold,nil];
    size = [lblNumAndCost.text sizeWithFont:[UIFont systemFontOfSize:16] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
    lblNumAndCost.frame = CGRectMake(lblGiftName.frame.origin.x + lblGiftName.frame.size.width, 13, size.width, size.height);
    
    lblCode.text = [[NSString alloc]initWithFormat:@"兑换码:  %@",model.code,nil];
    size = [lblCode.text sizeWithFont:[UIFont systemFontOfSize:16] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
    lblCode.frame = CGRectMake(20, 39, size.width, size.height);
    
    lblKtvName.text = [[NSString alloc]initWithFormat:@"新浪好声音%@",model.ktvName,nil];
    size = [lblKtvName.text sizeWithFont:[UIFont systemFontOfSize:14] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
    lblKtvName.frame = CGRectMake(self.frame.size.width -15 -size.width, 71, size.width, size.height);
    
    
    lblendDate.text = model.endDate;
    size = [lblendDate.text sizeWithFont:[UIFont systemFontOfSize:12] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
    lblendDate.frame = CGRectMake(self.frame.size.width -15 -size.width, 96, size.width, size.height);
    
    lblbeginDate.text = [[NSString alloc]initWithFormat:@"有效期: %@ - ",model.beginDate,nil];
    size = [lblbeginDate.text sizeWithFont:[UIFont systemFontOfSize:12] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
    lblbeginDate.frame = CGRectMake(lblendDate.frame.origin.x - size.width, 96, size.width, size.height);
    
    
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

@end
