//
//  InAppPurchaseManager.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-30.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "InAppPurchaseManager.h"
#import "SVProgressHUD.h"
#import "PKtvAssistantAPI.h"

@implementation InAppPurchaseManager

-(id)init
{
    isPaying = false;
    return  self;
}

-(void)requestProUpgradeProductData
{
    NSSet *productIdentifiers = [NSSet setWithObject:productIdentifier];
    productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
    productsRequest.delegate = self;
    [productsRequest start];
}

#pragma mark SKProductsRequestDelegate
-(void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    product = nil;
    NSArray *products = response.products;
    product = [products count] == 1? [products firstObject]:nil;
    if (product) {
        PLog(@"Product title: %@" , product.localizedTitle);
        //NSLog(@"Product description: %@" , product.localizedDescription);
        //NSLog(@"Product price: %@" , product.price);
        //NSLog(@"Product id: %@" , product.productIdentifier);
    }
    
    for (NSString *invalidProductId in response.invalidProductIdentifiers)
    {
        PLog(@"Invalid product id: %@" , invalidProductId);
    }
    [self purchaseProUpgrade];
    //[[NSNotificationCenter defaultCenter] postNotificationName:kInAppPurchaseManagerProductsFetchedNotification object:self userInfo:nil];
}

-(void)loadStore:(NSString *)Identifiers
{
    PLog(@"^^^^^^^^::loadStore");
    productIdentifier = Identifiers;
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    [self requestProUpgradeProductData];
}

-(BOOL)canMakePurchases
{
    return [SKPaymentQueue canMakePayments];
}

-(void)purchaseProUpgrade
{
    if (product)
    {
        SKPayment *payment = [SKPayment paymentWithProduct:product];
        [[SKPaymentQueue defaultQueue] addPayment:payment];
    }
    else
        [_delegate PurchaseDown];
}

#pragma mark 
- (void)recordTransaction:(SKPaymentTransaction *)transaction
{
    
}
//
// enable pro features
//
- (void)provideContent:(NSString *)productId
{
    
}
//
// removes the transaction from the queue and posts a notification with the transaction result
//
- (void)finishTransaction:(SKPaymentTransaction *)transaction wasSuccessful:(BOOL)wasSuccessful
{
    // remove the transaction from the payment queue.
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    
    NSDictionary *userInfo = [NSDictionary dictionaryWithObjectsAndKeys:transaction, @"transaction" , nil];
    if (wasSuccessful)
    {
        // send out a notification that we’ve finished the transaction
        [[NSNotificationCenter defaultCenter] postNotificationName:@"kInAppPurchaseManagerTransactionSucceededNotification" object:self userInfo:userInfo];
    }
    else
    {
        // send out a notification for the failed transaction
        [[NSNotificationCenter defaultCenter] postNotificationName:@"kInAppPurchaseManagerTransactionFailedNotification" object:self userInfo:userInfo];
    }
}

- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    if (isPaying) {
        [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
        product = nil;
        return;
    }
    isPaying =TRUE;
    NSString *receipt = [[NSString alloc] initWithData:[transaction transactionReceipt] encoding:NSUTF8StringEncoding];
    //[self recordTransaction:transaction];
    //[self provideContent:transaction.payment.productIdentifier];
    [self finishTransaction:transaction wasSuccessful:YES];
    NSString *identifier =[PCommonUtil encodeAesAndBase64StrFromStr:productIdentifier secretKey:KTV_ASSISTANT_API_SECRET_KEY];
    identifier =  [identifier stringByReplacingOccurrencesOfString:@"+" withString:@"%2B"];
    receipt = [PCommonUtil encodeAesAndBase64StrFromStr:receipt secretKey:KTV_ASSISTANT_API_SECRET_KEY];
    receipt =  [receipt stringByReplacingOccurrencesOfString:@"+" withString:@"%2B"];
    //NSLog(@"%@",receipt);
    NSString *data = [NSString stringWithFormat:@"data=%@",receipt];
    NSString *strUrl = [PKtvAssistantAPI getIAPPayUrl:[UserSessionManager getInstance].currentUser.uid ktvID:[UserSessionManager getInstance].currentRoomInfo.ktvId roomID:[UserSessionManager getInstance].currentRoomInfo.roomId identifier:identifier];
    NSData *postData = [NSData dataWithBytes:[data UTF8String] length:[data length]];
    NSURL *url =[[NSURL alloc] initWithString:strUrl];
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc]initWithURL:url];
    [request setHTTPMethod:@"POST"];
    [request setHTTPBody:postData];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation * operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary *html = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        NSString *status = [html objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [html objectForKey:@"result"];
            
            PLog(@"%@",[result objectForKey:@"Gold"]);
            [[UserSessionManager getInstance] updateCurrentUserGold:[[result objectForKey:@"Gold"] longValue]];
        }
        else
        {
            PLog(@"------------%@",status);
            [_delegate PurchaseFail];
        }
        isPaying = false;
        [_delegate PurchaseDown];
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        isPaying = false;
        [_delegate PurchaseFail];
    }];
    [operation start];
    
    product = nil;
}


- (void)restoreTransaction:(SKPaymentTransaction *)transaction
{
    [SVProgressHUD dismiss];
    //[self recordTransaction:transaction.originalTransaction];
    //[self provideContent:transaction.originalTransaction.payment.productIdentifier];
    [self finishTransaction:transaction wasSuccessful:YES];
    product = nil;
}
//
// called when a transaction has failed
//
- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
    [_delegate PurchaseDown];
    PLog(@"transaction.error.code  %d",transaction.error.code);
    if (transaction.error.code != SKErrorPaymentCancelled)
    {
        // error!
        [self finishTransaction:transaction wasSuccessful:NO];
    }
    //else
    //{
        // this is fine, the user just cancelled, so don’t notify
        [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
   // }
    product = nil;
}


-(void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions) {
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
                break;
            default:
                product = nil;
                break;
        }
    }
    
}


@end
