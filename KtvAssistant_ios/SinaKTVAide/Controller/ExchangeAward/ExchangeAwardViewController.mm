//
//  ExchangeAwardViewController.m
//  SinaKTVAide
//
//  Created by Li Pan on 13-12-24.
//  Copyright (c) 2013年 Li Pan. All rights reserved.
//

#import "ExchangeAwardViewController.h"
#import "KTVAppDelegate.h"
#import "DDMenuController.h"
@interface ExchangeAwardViewController ()

@end

@implementation ExchangeAwardViewController

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
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.navView.titleLabel.text = @"兑换奖品";
    self.view.backgroundColor = UIColorFromRGB(0xe1e2e6);

    _exchangeAwardWebView = [[UIWebView alloc] init];
    _exchangeAwardWebView.backgroundColor = [UIColor clearColor];
    _exchangeAwardWebView.frame = CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height);
    _exchangeAwardWebView.delegate = self;
    [self.mainContentView insertSubview:_exchangeAwardWebView belowSubview:self.redCircle]; // 红圈指示器显示最上层，webView显示在红圈下面
    
    [self loadData];
}

-(void)loadData
{
    [self loadWebView:kPKtvAssistantAPIExchangeAwardUrl];  // 加载webView
}

#pragma mark - 加载webView

-(void)loadWebView:(NSString *)tRequestUrl
{
    if(![[Reachability reachabilityForInternetConnection] isReachable])
    {
        [self showNetwork];
        return;
    }
    
    NSURL *tempUrl = [NSURL URLWithString:tRequestUrl];
    [_exchangeAwardWebView loadRequest:[[NSURLRequest alloc] initWithURL:tempUrl]];
}

#pragma mark - UIWebViewDelegate

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSString  *urlStr = request.URL.absoluteString;
    PLog(@"request.URL.absoluteString: %@", urlStr);
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


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
