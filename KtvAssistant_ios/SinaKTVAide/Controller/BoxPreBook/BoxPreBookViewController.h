//
//  BoxPreBookViewController.h
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-10-23.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BaseViewController.h"
#import "ZBarReaderViewController.h"
#import "TJSpinner.h"
@interface BoxPreBookViewController:BaseViewController<UIWebViewDelegate,ZBarReaderDelegate,UIAlertViewDelegate>
{
    NSString *keyword_;
    NSString *telnum;
}
@property (nonatomic,strong)  ZBarReaderViewController *reader;
@property (nonatomic, retain) NSString                 *keyWord;
@property (nonatomic,strong)  UIAlertView              *altExitRoom;
@property (nonatomic, retain) UIWebView                *appRecommendedWebView;
@property (nonatomic, retain) TJSpinner                *circularSpinner;

-(void)loadWebView:(NSString *)tRequestUrl;
@end
