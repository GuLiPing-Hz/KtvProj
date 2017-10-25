//
//  RechargeController.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-29.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "InAppPurchaseManager.h"


@interface RechargeController : BaseViewController<InAppPurchaseDelegate>
{
    NSMutableArray *productName;
    InAppPurchaseManager *iap;
    BOOL isPurchase;
}
@property(nonatomic,readonly) UISwipeGestureRecognizer *swipe;

@property (nonatomic,retain) UIView         *purchseBack;

@property (nonatomic,retain) UIView         *userInfo;
@property (nonatomic,retain) UIImageView    *imgHead;
@property (nonatomic,retain) UIImageView    *imgSex;
@property (nonatomic,retain) UILabel        *lblName;
@property (nonatomic,retain) UILabel        *lblMoney;

@property (nonatomic,retain) UIView         *recharge;
@property (nonatomic,retain) NSMutableArray *btnRecharge;
@property (nonatomic,retain) NSMutableArray *lblRecharge;


@property (nonatomic,retain) UIView         *tips;
@property (nonatomic,retain) UILabel        *lblTip;

@end
