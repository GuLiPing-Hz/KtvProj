//
//  BaseViewController.m
//
//  Created by pig on 13-8-15.
//  Copyright (c) 2013年 pig. All rights reserved.
//

#import "BaseViewController.h"
#import "SSImageCategory.h"
#import "KTVBaseConfig.h"
#import <QuartzCore/CoreAnimation.h>
#import "UIFont+PFontCategory.h"
#import "KTVAppDelegate.h"
#import "DDMenuController.h"
#import "KTVAppDelegate.h"
#import "BookedAndCollectedController.h"
#import "iToast.h"
#import "CResponse.h"
#import "NetUtilCallBack.h"

@interface BaseViewController ()

@end

@implementation BaseViewController

@synthesize navView = _navView;

@synthesize bgImageView = _bgImageView;

@synthesize mainContentView = _mainContentView;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        self.isRedCircle = NO;
    }
    return self;
}

- (KTVAppDelegate *)appDelegate
{
    KTVAppDelegate *appDelegate = (KTVAppDelegate *)[UIApplication sharedApplication].delegate;
    return appDelegate;
}

- (void)getAlreadyOrderedSongsNumber
{
    VECTREQSONGLIST vectOrderList = [NetUtilCallBack getInstance]->m_vectreqsonglist;
    int count = vectOrderList.size();
    if(count > 0)
        count -- ;
    [self.navView.rightButton setTitle:[NSString stringWithFormat:@"%d", count] forState:UIControlStateNormal];
}
//设备尺寸
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self.view setFrame:CGRectMake(0, 0, _MainScreen_Width, _MainScreen_Height)];
    
    CResponse::getResponseSingleton()->addListenView(self);
}

-(void)viewWillDisappear:(BOOL)animated
{
    if(!self.mIsRemoveOnDestroy)
        CResponse::getResponseSingleton()->removeListenView(self);
    
    [super viewWillDisappear:animated];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

-(void)dealloc
{
    if(self.mIsRemoveOnDestroy)
        CResponse::getResponseSingleton()->removeListenView(self);
}
#pragma mark View Didload
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.mIsRemoveOnDestroy = NO;
    
    if (IOS7_OR_LATER)
    {
        self.view.backgroundColor = [UIColor grayColor]; // 以前从导航条底部开始计算y值的，现在却是从Status Bar开始计算y值。全屏
        self.edgesForExtendedLayout = UIRectEdgeNone;  // 不延伸布局。默认是 ALL 四周延伸全屏
        self.extendedLayoutIncludesOpaqueBars = NO;    // 不 延伸到不透明(自定义导航条图)的导航条区域。
        self.modalPresentationCapturesStatusBarAppearance = NO;
        // 导航条
        _navView = [[PCustomNavigationBarView alloc] initCustomNavigationBarView:@"Base View"];
        [self.view addSubview:_navView];
        [_navView setHidden:YES];
        // 导航条以下的内容视图
        _mainContentView = [[UIView alloc] initWithFrame:CGRectMake(0, 64, _MainScreen_Width, _MainScreen_Height-44)];//44
        [self.view addSubview:_mainContentView];
    }
    else
    {
        // 导航条
        _navView = [[PCustomNavigationBarView alloc] initCustomNavigationBarView:@"Base View"];
        [self.view addSubview:_navView];
        [_navView setHidden:YES];
        // 导航条以下的内容视图
        _mainContentView = [[UIView alloc] initWithFrame:CGRectMake(0, 44, _MainScreen_Width, _MainScreen_Height-44)];//44
        [self.view addSubview:_mainContentView];
    }

    // 返回左按钮 - 默认返回按钮
    UIImage *backImageNormal = [UIImage imageWithName:@"btn_tback" type:@"png"];
    UIImage *backImage = [UIImage imageWithName:@"btn_tback_c" type:@"png"];
    _navView.backButton.contentMode = UIViewContentModeCenter;
    [_navView.backButton setBackgroundImage:backImageNormal forState:UIControlStateNormal];
    [_navView.backButton setBackgroundImage:backImage forState:UIControlStateHighlighted];
    [_navView.backButton setHidden:YES];
    [_navView.backButton addTarget:self action:@selector(doBack:) forControlEvents:UIControlEventTouchUpInside];
    
    // 左功能按钮 - 默认侧滑
    UIImage *leftImageNormal = [UIImage imageWithName:@"btn_cehua" type:@"png"];
    UIImage *leftImage = [UIImage imageWithName:@"btn_cehua_c" type:@"png"];
    [_navView.leftButton setBackgroundImage:leftImageNormal forState:UIControlStateNormal];
    [_navView.leftButton setBackgroundImage:leftImage forState:UIControlStateHighlighted];
    [_navView.leftButton setHidden:YES];
    [_navView.leftButton addTarget:self action:@selector(leftAction:) forControlEvents:UIControlEventTouchUpInside];
    
    // 右功能按钮 - 默认已点歌曲
    UIImage *rightImageNormal = [UIImage imageNamed:@"panel0_btn_selected0.png"];
    UIImage *rightImage = [UIImage imageNamed:@"panel0_btn_selected1.png"];
    _navView.rightButton.contentMode = UIViewContentModeCenter;
    _navView.rightButton.titleLabel.font = [UIFont systemFontOfSize:11];
    [_navView.rightButton setTitleEdgeInsets:UIEdgeInsetsMake(2, 2, 12, 2)];
    [_navView.rightButton setTitleColor:UIColorFromRGB(0xe4397d) forState:UIControlStateNormal];
    [_navView.rightButton setBackgroundImage:rightImageNormal forState:UIControlStateNormal];
    [_navView.rightButton setBackgroundImage:rightImage forState:UIControlStateHighlighted];
    [_navView.rightButton setHidden:YES];
    [_navView.rightButton addTarget:self action:@selector(rightAction:) forControlEvents:UIControlEventTouchUpInside];
    
    [self creatToolBar]; // 包厢信息条
    [self creatRedCircleActivity]; // 红圈指示器
}

// 底部工具条
- (void)creatToolBar
{
    CGFloat offset_y = _MainScreen_Height-HEIGHT_BOTTOM-44;
    _toolBar = [[UIView alloc] initWithFrame:CGRectMake(0, offset_y, _MainScreen_Width, HEIGHT_BOTTOM)];
    _toolBar.backgroundColor = [UIColor clearColor];
    UIImageView *toolBarImage = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"bottom_bar.png"]];
    toolBarImage.frame = _toolBar.bounds;
    [_toolBar addSubview:toolBarImage];
    [_toolBar setHidden:YES];
    [self.mainContentView addSubview:_toolBar];
    
    // 进入包厢
    _joinButton = [UIButton buttonWithType:UIButtonTypeCustom];
    _joinButton.frame = CGRectMake((_MainScreen_Width-150)/2, offset_y+9, 150, 35);
    _joinButton.backgroundColor = [UIColor clearColor];
    [_joinButton setTitle:@"进入包厢" forState:UIControlStateNormal];
    _joinButton.titleLabel.font = [UIFont systemFontOfSize:16.0f];
    [_joinButton setTitleColor:UIColorFromRGB(0xd23674) forState:UIControlStateNormal];
    [_joinButton setBackgroundImage:[UIImage imageNamed:@"btn_baoxiang.png"] forState:UIControlStateNormal];
    [_joinButton setBackgroundImage:[UIImage imageNamed:@"btn_baoxiang_c.png"] forState:UIControlStateHighlighted];
    [_joinButton setHidden:YES];
    [self.mainContentView addSubview:_joinButton];
    
    // 包厢号  改成UILabel不滚动了 by liric 2014.1.2
    _inRoomNumber = [[UILabel alloc] initWithFrame:CGRectMake((_MainScreen_Width-220)/2, offset_y+8, 220, 35)];
    _inRoomNumber.numberOfLines = 1;
    _inRoomNumber.hidden = YES;
    _inRoomNumber.opaque = NO;
    _inRoomNumber.enabled = YES;
    _inRoomNumber.shadowOffset = CGSizeMake(0.0, -1.0);
    _inRoomNumber.textAlignment = NSTextAlignmentCenter;
    _inRoomNumber.textColor = [UIColor colorWithRed:0.234 green:0.234 blue:0.234 alpha:1.000];
    _inRoomNumber.backgroundColor = [UIColor clearColor];
    _inRoomNumber.font = [UIFont fontWithName:@"Helvetica-Bold" size:17.000];
    _inRoomNumber.textColor = UIColorFromRGB(0xd23674);
    [self.mainContentView addSubview:_inRoomNumber];
    
    // 电源按钮
    _power = [UIButton buttonWithType:UIButtonTypeCustom];
    _power.frame = CGRectMake(_MainScreen_Width-40, offset_y+8, 35, 35);
    _power.backgroundColor = [UIColor clearColor];
    [_power setBackgroundImage:[UIImage imageNamed:@"btn_off.png"] forState:UIControlStateNormal];
    [_power setBackgroundImage:[UIImage imageNamed:@"btn_off_c.png"] forState:UIControlStateHighlighted];
    [_power setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [_power setHidden:YES];
    [self.mainContentView addSubview:_power];
}

// 红色圈圈指示器
- (void)creatRedCircleActivity
{
    if (self.isRedCircle == YES)
    {
        _redCircle = [[TJSpinner alloc] initWithSpinnerType:kTJSpinnerTypeCircular];
        _redCircle.frame = CGRectMake(115, 130, 100, 100);
        _redCircle.hidesWhenStopped = YES;
        _redCircle.radius = 10;
        _redCircle.pathColor = [UIColor whiteColor];
        _redCircle.fillColor = UIColorFromRGB(0xd23675);
        _redCircle.thickness = 7;
        [_redCircle startAnimating];
        [self.mainContentView addSubview:_redCircle];
        [self.mainContentView bringSubviewToFront:_redCircle];
    }
}

// 开启红圈转
- (void)startRedCircle
{
    [_redCircle startAnimating];
}

// 关闭红圈转
- (void)stopRedCircle
{
    [_redCircle stopAnimating];
}

// 返回按钮
-(void)doBack:(id)sender
{
    NSArray *viewControllers = self.navigationController.viewControllers;
    if (viewControllers.count > 1)
    {
        [self.navigationController popViewControllerAnimated:YES];
    }
}
// 左按钮
-(void)leftAction:(id)sender
{
    KTVAppDelegate *appDelegate = (KTVAppDelegate *)[UIApplication sharedApplication].delegate;
    [appDelegate.menuController showLeftController:YES];
}

- (void)rightAction:(id)sender
{
    if ([UserSessionManager isRoomAlreadyLogin])
    {
        BookedAndCollectedController * orderedSongsList = [[BookedAndCollectedController alloc]init];
        [self.navigationController pushViewController:orderedSongsList animated:YES];
    }
    else
    {
        UIAlertView * alert = [[UIAlertView alloc]initWithTitle:nil message:@"请先进入包厢" delegate:self cancelButtonTitle:@"确定" otherButtonTitles: nil];
        [alert show];
    }
}

- (void)showNetwork
{
    [SVProgressHUD showErrorWithStatus:@"网络不给力哦"];
}

- (void)showErrorAlert:(NSString *)content
{
    [SVProgressHUD showErrorWithStatus:content];
}

- (void)showNormalAlert:(NSString *)content
{
    [SVProgressHUD showWithTips:content];
}

- (void)showSuccessAlert:(NSString *)content
{
    [SVProgressHUD showSuccessWithStatus:content];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)setRightAndBadgeButton{
    if ([UserSessionManager isRoomAlreadyLogin]) {
        self.navView.rightButton.hidden = NO;
    }else{
        self.navView.rightButton.hidden = YES;
    }
}

-(void)pushReqSongInfo:(const ResultData_SongList*)pSonglist :(int)count
{
    [self performSelectorOnMainThread:@selector(getAlreadyOrderedSongsNumber) withObject:nil waitUntilDone:NO];
}

-(Boolean)isShowDefault
{
    NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];
    NSString *defaultKey = KTVASSISTANT_USERDEFAULTS_ISSHOW;
    NSString *value = [defaults objectForKey:defaultKey];
    if (value != nil)
        return [value compare:@"1"] == NSOrderedSame;
    else
        return false;
}

@end
