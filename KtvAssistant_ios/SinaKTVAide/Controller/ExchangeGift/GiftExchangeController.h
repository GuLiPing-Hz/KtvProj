//
//  GiftExchangeController.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "KTVGiftInfo.h"
#import "CustomListBox.h"

@interface GiftExchangeController : BaseViewController <CustomListBoxDelegate,UIAlertViewDelegate,UIGestureRecognizerDelegate>
{
    NSInteger giftNum;
    NSString  *giftKtvID;
}

@property (nonatomic,retain) UIImageView    *imgGift;
@property (nonatomic,retain) UILabel        *lblGiftName;
@property (nonatomic,retain) UILabel        *lblGiftNum;
@property (nonatomic,retain) UILabel        *lblGiftCost;
@property (nonatomic,retain) UIButton       *btnExchange;
@property (nonatomic,retain) UIImageView    *imgGiftNum;
@property (nonatomic,retain) UIView         *contentView;
@property (nonatomic,retain) UIAlertView    *confirmExchange;
@property (nonatomic,retain) CustomListBox  *listBox;
@property (nonatomic,retain) UITapGestureRecognizer *recognzier;

@property (nonatomic,retain) KTVGiftInfo       *gift;

-(id)init:(KTVGiftInfo *)_info ktvid:(NSString *)ktvid;

@end
