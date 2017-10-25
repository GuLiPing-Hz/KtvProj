//
//  ExchangeLogCell.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-28.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ExchangeLog.h"

@interface ExchangeLogCell : UITableViewCell
{
    UILabel         *lblGiftName;
    UILabel         *lblNumAndCost;
    UILabel         *lblCode;
    UILabel         *lblKtvName;
    UILabel         *lblbeginDate;
    UILabel         *lblendDate;
    UIImageView     *imgType;
    NSInteger       logType;// 0未领取  1已领取  2过期
}


@property (nonatomic,retain) ExchangeLog          *model;


- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier  cellType:(NSInteger)type;
@end
