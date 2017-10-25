//
//  GiftInfoCell.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "KTVGiftInfo.h"

@protocol GiftInfoCellDelegate <NSObject>

-(void)ShowExchange:(KTVGiftInfo *)info;

@end

@interface GiftInfoCell : UITableViewCell
{
    UIImageView     *imgGift;
    UILabel         *lblGiftName;
    UILabel         *lblGiftGold;
    UILabel         *lblGold;
    UIButton        *btnExchange;
}


@property (nonatomic,retain) KTVGiftInfo                *model;
@property (nonatomic,retain) id<GiftInfoCellDelegate>   delegate;

@end
