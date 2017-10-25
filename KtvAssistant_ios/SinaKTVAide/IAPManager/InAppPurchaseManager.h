//
//  InAppPurchaseManager.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-30.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@protocol InAppPurchaseDelegate <NSObject>

-(void) PurchaseDown;
-(void) PurchaseFail;

@end



@interface InAppPurchaseManager : NSObject<SKProductsRequestDelegate,SKPaymentTransactionObserver>
{
    SKProduct *product;
    SKProductsRequest *productsRequest;
    NSString *productIdentifier;
    Boolean isPaying;
}

@property (nonatomic,retain) id<InAppPurchaseDelegate> delegate;

- (void)loadStore:(NSString *)Identifiers;
- (BOOL)canMakePurchases;
- (void)purchaseProUpgrade;

@end
