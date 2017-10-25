//
//  KTVAppRecommendedViewController.h
//  SinaKTVAide
//
//  Created by apple on 13-11-5.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"

@interface KTVAppRecommendedViewController : BaseViewController<UIWebViewDelegate>

@property (nonatomic, retain) UIWebView *appRecommendedWebView;
@property (nonatomic, retain) UIActivityIndicatorView *indicatorView;

-(void)loadData;
-(void)loadWebView:(NSString *)tRequestUrl;

@end
