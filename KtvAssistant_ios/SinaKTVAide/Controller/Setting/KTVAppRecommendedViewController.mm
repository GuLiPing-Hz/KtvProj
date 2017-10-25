//
//  KTVAppRecommendedViewController.m
//  SinaKTVAide
//
//  Created by apple on 13-11-5.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "KTVAppRecommendedViewController.h"
#import "DDMenuController.h"
#import "KTVAppDelegate.h"
@interface KTVAppRecommendedViewController ()

@end

@implementation KTVAppRecommendedViewController

@synthesize appRecommendedWebView = _appRecommendedWebView;
@synthesize indicatorView = _indicatorView;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.isRedCircle = YES;
    }
    return self;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:NO];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:YES];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navView.titleLabel.text = @"应用推荐";
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.mainContentView.backgroundColor = UIColorFromRGB(0xe6e7ee);
    _appRecommendedWebView = [[UIWebView alloc] init];
    _appRecommendedWebView.frame = CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height);
    _appRecommendedWebView.delegate = self;
    [self.mainContentView insertSubview:_appRecommendedWebView belowSubview:self.redCircle]; // 红圈指示器显示最上层，webView显示在红圈下面

    [self loadData];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)loadData
{
    [self loadWebView:kPKtvAssistantAPIAppRecommendedUrl];
}

-(void)loadWebView:(NSString *)tRequestUrl
{
    if(![[Reachability reachabilityForInternetConnection] isReachable])
    {
        [self showNetwork];
        return;
    }
    
    NSURL *tempUrl = [NSURL URLWithString:tRequestUrl];
    [_appRecommendedWebView loadRequest:[[NSURLRequest alloc] initWithURL:tempUrl]];
}

#pragma UIWebViewDelegate

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSString  *urlStr = request.URL.absoluteString;
    PLog(@"request.URL.absoluteString: %@", request.URL.absoluteString);
    NSString *frontStr = [urlStr substringWithRange:NSMakeRange([urlStr length]-4, 4)];
    if ([frontStr isEqualToString:@"mt=8"]) {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:urlStr]];
        return NO;
    }
    
    //处理请求内容，拦劫请求
    
    return YES;
}

- (void)webViewDidStartLoad:(UIWebView *)webView
{
    PLog(@"webViewDidStartLoad...");
    [self startRedCircle];
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    PLog(@"webViewDidFinishLoad");
    [self stopRedCircle];
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    PLog(@"didFailLoadWithError...");
    [self stopRedCircle];
    [SVProgressHUD showErrorWithStatus:kPKtvAssistantErrorMsgSystem];
}

@end
