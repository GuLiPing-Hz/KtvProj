//
//  BaseWaitViewController.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/22.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "BaseWaitViewController.h"

@interface BaseWaitViewController ()

@end

@implementation BaseWaitViewController

#define RE_EMPTYVIEW_MARGIN_TOP -100
#define RE_EMPTYVIEW_HEIGHT 230
-(void)createTip
{
    self.mUiEmptyView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, self.mainContentView.frame.size.width, RE_EMPTYVIEW_HEIGHT)];
    UIImageView* imgView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"pic_sorry"]];
    [imgView setCenter:CGPointMake(self.mUiEmptyView.center.x, self.mUiEmptyView.center.y-20)];
    [self.mUiEmptyView addSubview:imgView];
    self.mUiEmptyTip = [[UILabel alloc] initWithFrame:CGRectMake(0, imgView.frame.origin.y+imgView.frame.size.height+20, self.mUiEmptyView.frame.size.width, 30)];
    [self.mUiEmptyTip setFont:[UIFont systemFontOfSize:18]];
    [self.mUiEmptyTip setTextColor:UIColorFromRGB(0xc0c0c2)];
    [self.mUiEmptyTip setTextAlignment:NSTextAlignmentCenter];
    [self.mUiEmptyView addSubview:self.mUiEmptyTip];
    //居中
    [self.mUiEmptyView setCenter:CGPointMake(self.mainContentView.center.x, self.mainContentView.center.y+RE_EMPTYVIEW_MARGIN_TOP)];
    [self.mUiEmptyView setHidden:YES];
    [self.mainContentView addSubview:self.mUiEmptyView];
    
    self.mUiLoadingView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 100, 100)];
    [self.mUiLoadingView setCenter:CGPointMake(self.mainContentView.center.x, self.mainContentView.center.y+RE_EMPTYVIEW_MARGIN_TOP)];
    [self.mainContentView addSubview:self.mUiLoadingView];
    NSArray* arrayImg = [[NSArray alloc] initWithObjects:[UIImage imageNamed:@"ktv_loading0"],[UIImage imageNamed:@"ktv_loading1"],[UIImage imageNamed:@"ktv_loading2"],[UIImage imageNamed:@"ktv_loading3"],[UIImage imageNamed:@"ktv_loading4"],[UIImage imageNamed:@"ktv_loading5"],[UIImage imageNamed:@"ktv_loading6"],[UIImage imageNamed:@"ktv_loading7"],[UIImage imageNamed:@"ktv_loading8"],[UIImage imageNamed:@"ktv_loading9"],[UIImage imageNamed:@"ktv_loading10"],[UIImage imageNamed:@"ktv_loading11"],[UIImage imageNamed:@"ktv_loading12"], nil];
    [self.mUiLoadingView setAnimationImages:arrayImg];
    [self.mUiLoadingView setAnimationRepeatCount:0];
    [self.mUiLoadingView setAnimationDuration:0.65f];
    [self.mUiLoadingView setHidden:YES];
    
    self.mUiActivityIndicator= [[MONActivityIndicatorView alloc] init];
    self.mUiActivityIndicator.delegate = self;
    self.mUiActivityIndicator.numberOfCircles = 6;
    self.mUiActivityIndicator.radius = 8;
    self.mUiActivityIndicator.internalSpacing = 10;
    self.mUiActivityIndicator.duration = 1;
    //居中
    [self.mUiActivityIndicator setCenter:CGPointMake(self.mainContentView.center.x, self.mainContentView.center.y+RE_EMPTYVIEW_MARGIN_TOP)];
    [self.mainContentView addSubview:self.mUiActivityIndicator];
}

-(void)showEmptyTip:(BOOL)show msg:(NSString*)msg
{
    //OC 里面貌似第二次加载一下XIB文件之后，之前加载的东西就被释放了，然后虽然之前获取到的指针还在，但是指向的东西已经不存在了，所以，这里无法更新到界面UI字符串。如果要写基类的话，还是要靠内存中new出来的才行
    //[[NSBundle mainBundle] loadNibNamed:@"BaseWait" owner:self options:nil];
    //停止动画
    [self stopWait];
    if(show)
    {
        [self.mUiEmptyTip setText:msg];
        [self.mUiEmptyView setHidden:NO];
        [self.mainContentView bringSubviewToFront:self.mUiEmptyView];
    }
    else
    {
        [self.mUiEmptyView setHidden:YES];
    }
}

-(void)startWait
{
    [self.mainContentView bringSubviewToFront:self.mUiLoadingView];
    [self.mUiLoadingView setHidden:NO];
    [self.mUiLoadingView startAnimating];
    //[self.mUiActivityIndicator startAnimating];
}
-(void)stopWait
{
    [self.mUiLoadingView stopAnimating];
    [self.mUiLoadingView setHidden:YES];
    //[self.mUiActivityIndicator stopAnimating];
}
#pragma mark - MONActivityIndicatorViewDelegate Methods

- (UIColor *)activityIndicatorView:(MONActivityIndicatorView *)activityIndicatorView
      circleBackgroundColorAtIndex:(NSUInteger)index {
    return UIColorFromRGB(0xc0c0c2);
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    //[[NSBundle mainBundle] loadNibNamed:@"BaseWait" owner:self options:nil];
    
    [self createTip];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
