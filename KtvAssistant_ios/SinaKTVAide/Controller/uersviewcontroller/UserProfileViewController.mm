//
//  UserProfileViewController.m
//  SinaKTVAide

//  用户个人中心 界面

//  Created by Li Pan on 13-12-5.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "UserProfileViewController.h"
#import "UserTotalCell.h"
#import "KTVAppDelegate.h"
#import "PKtvAssistantAPI.h"
#import "PointLog.h"
#import "ChangeUserInfoViewController.h"
#import "KTVMainViewController.h"
#import "ASIFormDataRequest.h"
#import "ExchangeAwardViewController.h"
#import "iToast.h"
#import "CommenTools.h"
//#import "BPush.h"
#import "CMActionSheet.h"
#import "KTVInputSecretController.h"
#import "RegexKitLite.h"
#import "xingguang.h"
#import "NetUtilCallBack.h"
#import "com_ios_NetUtil.h"
#import "KTVBaseConfig.h"
#import "DDMenuController.h"

@interface UserProfileViewController ()

@end

@implementation UserProfileViewController
@synthesize headButton = _headButton,changeHeadPhotoSheet = _changeHeadPhotoSheet,totalTable = _totalTable;
@synthesize AnimationTotal = _AnimationTotal;
@synthesize arrayTotal = _arrayTotal,separate = _separate;
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

#pragma mark - viewWillAppear

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self startActivity];
    [self getScoreListData]; // 获取积分列表数据
    [self flashUILoginInfo]; // 刷新个人信息界面
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(disconnRoom:) name:KTVASSISTANT_CLOSEROOM_NOTIFICATION object:nil];
}

-(void)disconnRoom:(NSNotification *)notification
{
    [self performSelectorOnMainThread:@selector(closeRoom) withObject:nil waitUntilDone:YES];
}

-(void)closeRoom
{
    DisConnectRoom();
    [UserSessionManager clearCurrentRoomInfo];
    [self flashUIWithRoom]; // 退出包厢刷新room
}


#pragma mark - 开启加载

- (void)startActivity
{
    [_actListMessage startAnimating];
    [_beachBallSpinner startAnimating];
}

- (void)creatActivity
{
    // 加载积分列表指示器
    _actListMessage = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray];
    _actListMessage.backgroundColor = [UIColor clearColor];
    [_actListMessage setHidesWhenStopped:YES];
    _actListMessage.frame = CGRectMake(160, 330, 0, 0);
    [self.view addSubview:_actListMessage];
    
    // 加载个人信息指示器
    _beachBallSpinner = [[TJSpinner alloc] initWithSpinnerType:kTJSpinnerTypeBeachBall];
    _beachBallSpinner.hidesWhenStopped = YES;
    _beachBallSpinner.backgroundColor = [UIColor clearColor];
    [_beachBallSpinner setRadius:13];
    if (IOS7_OR_LATER)
    {
        _beachBallSpinner.frame = CGRectMake(280, 35, 26, 26);
    }
    else
    {
        _beachBallSpinner.frame = CGRectMake(280, 15, 26, 26);
    }
    [self.view addSubview:_beachBallSpinner];

    
}

#pragma mark - viewDidLoad

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navView.titleLabel.text = @"个人中心";
    self.view.backgroundColor = UIColorFromRGB(0xe1e2e6);
    
    _arrayTotal = [[NSMutableArray alloc] init];
    
    [self creatBackgroundImage];
    [self creatChangeHeadPhoto];
    [self creatTableView];
    [self creatLineWithTotal];
    [self creatNoMessageNoticeWithBackgroundImage];
    [self creatActivity];       // 加载指示器
    [self startActivity];
    
    // 手势进入修改昵称
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(changeNikeName:)];
    tap.numberOfTouchesRequired = 1;
    tap.numberOfTapsRequired = 1;
    [self.userName addGestureRecognizer:tap];
    
    self.toolBar.hidden = NO;
    self.joinButton.hidden = NO;
    // 进入包厢
    [self.joinButton addTarget:self action:@selector(jRoom:) forControlEvents:UIControlEventTouchUpInside];
    // 退出包厢
    [self.power addTarget:self action:@selector(outRoom:) forControlEvents:UIControlEventTouchUpInside];
    [self flashUIWithRoom]; // 退出包厢刷新UI
}

#pragma mark - 获取积分列表数据

- (void)getScoreListData
{
    if(![[Reachability reachabilityForInternetConnection] isReachable])
    {
        [_actListMessage stopAnimating];
        [self showNetwork];
        return;
    }
    long int userID = [UserSessionManager getInstance].currentUser.uid;
    NSString *encodeUrl = [PKtvAssistantAPI getUserRewardUrl:userID begin:1 num:(5)];  // 一页  五条
    NSURL *url = [NSURL URLWithString:encodeUrl];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject)
     {
         NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
         NSString *status = [results objectForKey:@"status"];
         if ([status compare:@"1"] == NSOrderedSame)
         {
             [_actListMessage stopAnimating];
             NSDictionary *result = [results objectForKey:@"result"];
             NSDictionary *userData = [result objectForKey:@"user"];

             User *user = [User initWithUserDictionary:userData];
             [[UserSessionManager getInstance] updateCurrentUserInfo:user];
             [user log];
             NSArray *dataList = [result objectForKey:@"pointlist"];
             if (dataList.count > 0)
             {
                 [self.arrayTotal removeAllObjects];
             }
             else
             {
                 if (self.arrayTotal.count == 0) // 列表无积分就显示logo背景
                 {
                      _noMessageView.hidden = NO;
                 }
             }
             
             for (int i=0; i < [dataList count]; i++)
             {
                 PointLog *log = [PointLog initWithDictionary:[dataList objectAtIndex:i]];
                 [log log];
                 [self.arrayTotal addObject:log];
                 [self.totalTable reloadData];
             }
         }
         else
         {
             [_actListMessage stopAnimating];
             NSString *msg = [results objectForKey:@"msg"];
             NSString *errorCode = [results objectForKey:@"errorcode"];
             [CommenTools showAlertViewWithErrorMessage:msg];
             PLog(@"doGetUserPoints failed...errorcode:%@(%@)", errorCode, msg);
         }
     }
    failure:^(AFHTTPRequestOperation *operation, NSError *error)
     {
         [_actListMessage stopAnimating];
         PLog(@"--failed...");
         [CommenTools showNetworkDisconnectedAlertView];
     }];
    
    [operation start];
}

#pragma mark - 展示个人信息

- (void)creatTableView
{
    _totalTable = [[UITableView alloc] initWithFrame:CGRectMake(0, 250, _MainScreen_Width, _MainScreen_Height-250-50) style:UITableViewStylePlain];
    _totalTable.backgroundColor = [UIColor clearColor];
    _totalTable.backgroundView = nil;
    _totalTable.showsVerticalScrollIndicator = NO;
    _totalTable.showsHorizontalScrollIndicator = NO;
    _totalTable.dataSource = self;
    _totalTable.delegate = self;
    _totalTable.rowHeight = 50;
    _totalTable.scrollEnabled = YES;
    _totalTable.separatorStyle = UITableViewCellSeparatorStyleNone;
    _totalTable.tableFooterView = [[UIView alloc] init];//多余的cell屏蔽掉
    [self.view addSubview:_totalTable];
}

- (void)creatBackgroundImage
{
    [_backgroundImage setImage:[UIImage imageNamed:@"bg_zhanghu.png"]];
    _backgroundImage.backgroundColor = [UIColor clearColor];
    _backgroundImage.userInteractionEnabled = YES;
    _backgroundImage.frame = CGRectMake(0, 0, _MainScreen_Width, 220);
    [self.view addSubview:_backgroundImage];
}

- (void)creatChangeHeadPhoto
{
    //头像按钮
    _headButton = [UIButton buttonWithType:UIButtonTypeCustom];
    _headButton.frame = CGRectMake((_MainScreen_Width-74)/2, (220-74)/2, 74, 74);
    _headButton.backgroundColor = [UIColor clearColor];
    _headButton.layer.cornerRadius = 37;
    _headButton.layer.borderWidth = 1;
    _headButton.layer.borderColor = [UIColor whiteColor].CGColor;
    [self.view addSubview:_headButton];
    _headButton.layer.masksToBounds = YES;
    [_headButton addTarget:self action:@selector(changeHeadPhoto:) forControlEvents:UIControlEventTouchUpInside];
    
    //左按钮
    _left = [UIButton buttonWithType:UIButtonTypeCustom];
    if (IOS7_OR_LATER)
    {
        _left.frame = CGRectMake(5, 30, 35, 35);
    }
    else
    {
        _left.frame = CGRectMake(5, 10, 35, 35);
    }
    _left.backgroundColor = [UIColor clearColor];
    [_left setBackgroundImage:[UIImage imageNamed:@"btn_cehua.png"] forState:UIControlStateNormal];
    [_left setBackgroundImage:[UIImage imageNamed:@"btn_cehua_c.png"] forState:UIControlStateHighlighted];
    [_left addTarget:self action:@selector(_leftAction:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:_left];
    
    //右按钮
//    _right = [UIButton buttonWithType:UIButtonTypeCustom];
//    if (IOS7_OR_LATER)
//    {
//        _right.frame = CGRectMake(_MainScreen_Width-10-35, 30, 35, 35);
//    }
//    else
//    {
//        _right.frame = CGRectMake(_MainScreen_Width-10-35, 10, 35, 35);
//    }
//    _right.backgroundColor = [UIColor clearColor];
//    [_right setBackgroundImage:[UIImage imageNamed:@"btn_loginout.png"] forState:UIControlStateNormal];
//    [_right setBackgroundImage:[UIImage imageNamed:@"btn_loginout_c.png"] forState:UIControlStateHighlighted];
//    [_right addTarget:self action:@selector(_rightAction:) forControlEvents:UIControlEventTouchUpInside];
//    [self.view addSubview:_right];
    
    // 男or女
    _sex = [[UIImageView alloc] initWithFrame:CGRectMake(22, 194, 9, 14)];
    [self.view addSubview:_sex];
    
    //用户名 标签
    _userName = [[UILabel alloc] initWithFrame:CGRectMake(40, 185, 100, 30)];
    _userName.textColor = UIColorFromRGB(0xffffff);
    _userName.userInteractionEnabled = YES;
    _userName.shadowColor = UIColorFromRGB(0x1f1f1f);
    _userName.shadowOffset = CGSizeMake(0, 1);
    _userName.font = [UIFont systemFontOfSize:14.0f];
    _userName.backgroundColor = [UIColor clearColor];
    [self.view addSubview:_userName];
    
    //积分 标签
    UILabel *integral = [[UILabel alloc] initWithFrame:CGRectMake(140, 185, 100, 30)];
    integral.textColor = UIColorFromRGB(0xffffff);
    integral.text = @"积分:";
    integral.shadowColor = UIColorFromRGB(0x1f1f1f);
    integral.shadowOffset = CGSizeMake(0, 1);
    integral.font = [UIFont systemFontOfSize:14.0f];
    integral.backgroundColor = [UIColor clearColor];
    [self.view addSubview:integral];
    
    //积分分数
    _total = [[UILabel alloc] initWithFrame:CGRectMake(172, 185, 100, 30)];
    _total.textColor = UIColorFromRGB(0xe9bb15);
    _total.font = [UIFont systemFontOfSize:14.0f];
    _total.backgroundColor = [UIColor clearColor];
    [self.view addSubview:_total];
    
    //立即兑换 按钮
    UIButton *exchange = [UIButton buttonWithType:UIButtonTypeCustom];
    exchange.frame = CGRectMake(230, 189, 70, 22);
    exchange.backgroundColor = [UIColor clearColor];
    exchange.titleLabel.font = [UIFont systemFontOfSize:14.0f];
    [exchange setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
    [exchange setTitle:@"立即兑换" forState:UIControlStateNormal];
    [exchange setBackgroundImage:[UIImage imageNamed:@"btn_duihuan.png"] forState:UIControlStateNormal];
    [exchange setBackgroundImage:[UIImage imageNamed:@"btn_duihuan_c.png"] forState:UIControlStateHighlighted];
    [self.view addSubview:exchange];
    [exchange addTarget:self action:@selector(exchangeAction:) forControlEvents:UIControlEventTouchUpInside];
}

- (void)creatLineWithTotal
{
    // 积分领取一条背景竖线
    UIImage *separate = [[UIImage imageNamed:@"line_wodezhanghu.png"] stretchableImageWithLeftCapWidth:0 topCapHeight:1];;
    _separate = [[UIImageView alloc] initWithImage:separate];
    if (iPhone5)
    {
        _separate.frame = CGRectMake(16, 220, 1, _MainScreen_Height-220-50);
    }
    else
    {
        _separate.frame = CGRectMake(16, 220, 1, _MainScreen_Height-220-50);
    }
    [self.view addSubview:_separate];
    
    UIImageView *iconTotal = [[UIImageView alloc] initWithFrame:CGRectMake(10, 220+10, 12, 14)];
    iconTotal.image = [UIImage imageNamed:@"icon_Bgjifen.png"];
    iconTotal.backgroundColor = [UIColor clearColor];
    [self.view addSubview:iconTotal];
    
    UILabel *headLabel = [[UILabel alloc] initWithFrame:CGRectMake(30, 220+6, 100, 20)];
    headLabel.text = @"积分领取";
    headLabel.font = [UIFont systemFontOfSize:13.0f];
    headLabel.textColor = UIColorFromRGB(0x000000);
    headLabel.backgroundColor = [UIColor clearColor];
    [self.view addSubview:headLabel];
}

- (void)creatNoMessageNoticeWithBackgroundImage
{
    _noMessageView = [[UIView alloc] initWithFrame:CGRectMake(0, 220, _MainScreen_Width, 100)];
    _noMessageView.backgroundColor = [UIColor clearColor];
    _noMessageView.hidden = YES;
    [self.mainContentView addSubview:_noMessageView];
    
    // logo
    UIImageView *logoImage = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"biaoqing.png"]];
    logoImage.backgroundColor =[UIColor clearColor];
    logoImage.frame = CGRectMake(131, 0, 56, 49);
    [_noMessageView addSubview:logoImage];
    
    // notice
    UILabel *noticeTotal = [[UILabel alloc] initWithFrame:CGRectMake(90, 55, 200, 30)];
    noticeTotal.textColor = UIColorFromRGB(0x606060);
    noticeTotal.font = [UIFont systemFontOfSize:14.0f];
    noticeTotal.backgroundColor = [UIColor clearColor];
    noticeTotal.text = @"当前没有可领取积分哦!";
    [_noMessageView addSubview:noticeTotal];
    
    UILabel *noticeRush = [[UILabel alloc] initWithFrame:CGRectMake(55, 80, 280, 30)];
    noticeRush.textColor = UIColorFromRGB(0x606060);
    noticeRush.font = [UIFont systemFontOfSize:14.0f];
    noticeRush.backgroundColor = [UIColor clearColor];
    noticeRush.text = @"歌曲抢榜或包厢签到都可获得积分!";
    [_noMessageView addSubview:noticeRush];
}

#pragma mark -  UIActionSheet 进入包厢

- (void)jRoom:(UIButton *)btn

{
    //********* 自定义表单 *********
    CMActionSheet *actionSheet = [[CMActionSheet alloc] init];
    actionSheet.title = @"请先进入包厢";
    [actionSheet addSeparator];
    
    [actionSheet addButtonWithTitle:@"扫描包厢二维码" type:CMActionSheetButtonTypeWhite block:^{
        PLog(@"扫描二维码进包厢");
        [self openAction];
        
    }];
    [actionSheet addButtonWithTitle:@"输入包厢密码" type:CMActionSheetButtonTypeBlue block:^{
        PLog(@"输入房间密码进包厢");
        KTVInputSecretController *input = [[KTVInputSecretController alloc] init];
        [self.navigationController pushViewController:input animated:YES];
    }];
    [actionSheet addButtonWithTitle:@"取消" type:CMActionSheetButtonTypeRed block:^{
        PLog(@"取消");
    }];
    [actionSheet present];
    //********* ********* *********
}


#pragma mark - 退出包厢

- (void)outRoom:(UIButton *)button
{
    _altExitRoom = [[UIAlertView alloc] initWithTitle:@"温馨提示" message:@"你确定要退出包厢吗?" delegate:self cancelButtonTitle:@"点错了" otherButtonTitles:@"好", nil];
    [_altExitRoom show];
}

#pragma mark - 开启二维码扫描

- (void)openAction
{
    _reader = [ZBarReaderViewController new];
    //设备尺寸 3.5/4.0英寸
    _reader.view.frame = CGRectMake(0, 0, _MainScreen_Width, _MainScreen_Height+(iPhone5?88:0));
    _reader.readerDelegate = self;
    [_reader setShowsZBarControls:NO]; // 禁止 官方的取消和帮助说明按钮
    _reader.supportedOrientationsMask = ZBarOrientationMaskAll;
    _reader.cameraFlashMode = UIImagePickerControllerCameraFlashModeOff;
    ZBarImageScanner *scanner = _reader.scanner;
    [scanner setSymbology: ZBAR_I25
                   config: ZBAR_CFG_ENABLE
                       to: 0];
    
    // 取消二维码扫描
    UIButton *cancel = [UIButton buttonWithType:UIButtonTypeCustom];
    cancel.frame = CGRectMake(20, _reader.view.frame.size.height-60, 60, 60);
    cancel.showsTouchWhenHighlighted = YES;
    cancel.backgroundColor = [UIColor clearColor];
    [cancel setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
    [cancel setTitle:@"取消" forState:UIControlStateNormal];
    [cancel addTarget:self action:@selector(cancelAction:) forControlEvents:UIControlEventTouchUpInside];
    [_reader.view addSubview:cancel];
    
    // 是否开启闪关灯照明
    UIButton *flash = [UIButton buttonWithType:UIButtonTypeCustom];
    flash.frame = CGRectMake(_MainScreen_Width-60-20, _reader.view.frame.size.height-60, 60, 60);
    flash.showsTouchWhenHighlighted = YES;
    flash.backgroundColor = [UIColor clearColor];
    flash.tag = 0;
    [flash setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
    [flash setTitle:@"照明" forState:UIControlStateNormal];
    [flash addTarget:self action:@selector(flashAction:) forControlEvents:UIControlEventTouchUpInside];
    [_reader.view addSubview:flash];
    
    [self presentModalViewController: _reader animated: YES];
}

#pragma mark -  取消二维码扫描

- (void)cancelAction:(UIButton *)button
{
    [_reader dismissModalViewControllerAnimated:YES];
}

#pragma mark -  是否开启闪关灯照明

- (void)flashAction:(UIButton *)button
{
    if (button.tag == 0)
    {
        [button setTag:1];
        _reader.cameraFlashMode = UIImagePickerControllerCameraFlashModeOn; // 后置灯光模式开
    }
    else
    {
        [button setTag:0];
        _reader.cameraFlashMode = UIImagePickerControllerCameraFlashModeOff; // 后置灯光模式关
    }
}

#pragma mark - 扫描二维码登录进入包厢

-(void)login
{
    if(![[Reachability reachabilityForInternetConnection] isReachable])
    {
        [self showNetwork];
        return;
    }
    [SVProgressHUD showWithStatus:@"进入房间中,请稍后" maskType:SVProgressHUDMaskTypeClear];
    
    User * user = [UserSessionManager getInstance].currentUser;
    long int userID = user.uid;
    NSString *encodeUrl =  [PKtvAssistantAPI getRoomLoginUrlEx:userID pwd:user.pwd password:self.keyWord];
    
    NSURL *url = [NSURL URLWithString:encodeUrl];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject)
     {
         NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
         NSString *status = [results objectForKey:@"status"];
         if ([status compare:@"1"] == NSOrderedSame)
         {
             NSDictionary *result = [results objectForKey:@"result"];
             NSInteger gold = [[result objectForKey:@"gold"] integerValue];
             if ([UserSessionManager getInstance].currentUser.gold != gold && gold > 0) {
                 [[UserSessionManager getInstance] updateCurrentUserGold:gold];
             }
             RoomInfo *roominfo = [RoomInfo initWithDictionary:result];
             roominfo.roomPassword = _keyWord;
             [roominfo Log];
             [[UIApplication sharedApplication] cancelAllLocalNotifications]; // 撤销所有的Notification
             UILocalNotification *localNotification = [[UILocalNotification alloc] init];
             // 设置notification的属性
             localNotification.fireDate = [[NSDate new] dateByAddingTimeInterval:roominfo.periodsTime]; //出发时间
             localNotification.alertBody = @"您的包厢已到期，请续费或欢迎下次光临"; // 消息内容
             localNotification.soundName = UILocalNotificationDefaultSoundName; // 触发消息时播放的声音
             //设置随Notification传递的参数
             NSDictionary *infoDict = [NSDictionary dictionaryWithObjectsAndKeys:@"reminder", @"notificationId", self.keyWord, @"roomSecret", nil];
             localNotification.userInfo = infoDict;
             [[UIApplication sharedApplication] scheduleLocalNotification:localNotification];
             [UserSessionManager getInstance].currentRoomInfo = roominfo;
             [UserSessionManager getInstance].isLoggedIn = YES;
             NSString * tag = [roominfo.roomPassword lowercaseString];
             //PLog(@"TAG-->>%@",tag);
             //[BPush setTag:tag withCompleteHandler:nil];
             [SVProgressHUD dismiss];
             
             NetUtilCallBack* pNetUtil = [NetUtilCallBack getInstance];
             [pNetUtil setRoomId:[roominfo.xgRoomID intValue]];
             [pNetUtil doCurrentWork:TransactionRoomLogin listener:^(BOOL success) {
                 if(success)
                 {
                     [self flashUIWithRoom]; // 进入包厢或者退出包厢刷新界面
                 }
                 else
                 {
                     [SVProgressHUD dismiss];
                     NSString *msg = [results objectForKey:@"msg"];
                     [CommenTools showAlertViewWithTitle:msg];
                 }
             }];
         }
         else
         {
             [SVProgressHUD dismiss];
             NSString *msg = [results objectForKey:@"msg"];
             [CommenTools showAlertViewWithTitle:msg];
         }
     } failure:^(AFHTTPRequestOperation *operation, NSError *error)
     {
         [SVProgressHUD dismiss];
         PLog(@"doLogin failed...");
         [CommenTools showNetworkDisconnectedAlertView];
     }];
    [operation start];
}


#pragma mark - 进入包厢或者退出包厢刷新界面

- (void)flashUIWithRoom
{
    if ([UserSessionManager isRoomAlreadyLogin])   // 已进入包厢
    {
        NSString *roomName = [UserSessionManager getInstance].currentRoomInfo.roomName;
        self.inRoomNumber.text = [NSString stringWithFormat:@"所在包厢: %@",roomName];
        self.joinButton.hidden = YES;
        self.power.hidden = NO;
        self.inRoomNumber.hidden = NO;
    }
    else  // 未进入包厢
    {
        self.joinButton.hidden = NO;
        self.power.hidden = YES;
        self.inRoomNumber.hidden = YES;
    }
}

#pragma mark - 修改昵称

- (void)changeNikeName:(UITapGestureRecognizer *)tap
{
    PLog(@"--修改昵称--");
    ChangeUserInfoViewController *change = [[ChangeUserInfoViewController alloc] init];
    [self.navigationController pushViewController:change animated:YES];
}

#pragma mark - 退出包厢

- (void)_exitRoom:(UIButton *)button
{
    _altExitRoom = [[UIAlertView alloc] initWithTitle:@"温馨提示" message:@"你确定要退出包厢吗?" delegate:self cancelButtonTitle:@"点错了" otherButtonTitles:@"好", nil];
    [_altExitRoom show];
}

#pragma mark - 立即兑换

- (void)exchangeAction:(UIButton *)button
{
    ExchangeAwardViewController *exchange = [[ExchangeAwardViewController alloc] init];
    [self.navigationController pushViewController:exchange animated:YES];
}

#pragma mark - 左上角按钮侧拉

- (void)_leftAction:(UIButton *)button
{
    [self.appDelegate.menuController showLeftController:YES];
}

#pragma mark - 换头像按钮

- (void)changeHeadPhoto:(UIButton *)button
{
    _changeHeadPhotoSheet = [[UIActionSheet alloc] initWithTitle:nil delegate:self cancelButtonTitle:@"取消" destructiveButtonTitle:nil otherButtonTitles:@"拍照",@"从相册选择", nil];
    [_changeHeadPhotoSheet showInView:self.view];
}

#pragma mark - 退出包厢/注销账号 UIAlertView delegate

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 1)
    {
        /**
            退出包厢
         */
        if (alertView == _altExitRoom)
        {
            if(![[Reachability reachabilityForInternetConnection] isReachable])
            {
                [self showNetwork];
                return;
            }
            //[SVProgressHUD showWithStatus:@"正在退出..." maskType:SVProgressHUDMaskTypeBlack];
            long int userID = [UserSessionManager getInstance].currentUser.uid;
            NSString *password  = [UserSessionManager getInstance].currentRoomInfo.roomPassword;
            
            NSString *roomId = [UserSessionManager getInstance].currentRoomInfo.roomId;
            NSString *encodeUrl =  [PKtvAssistantAPI getRoomLogoutUrl:userID roomID:roomId password:password];
            NSURL *url = [NSURL URLWithString:encodeUrl];
            NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
            
            [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
            AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
            [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject)
             {
                  DisConnectRoom();
             }
            failure:^(AFHTTPRequestOperation *operation, NSError *error)
             {
             }];
            [operation start];
            
            NSString * tag = [password lowercaseString];
            //[BPush delTag:tag withCompleteHandler:nil];
            [UserSessionManager clearCurrentRoomInfo];
            [self flashUIWithRoom]; // 退出包厢刷新room

        }
    }
    [alertView dismissWithClickedButtonIndex:0 animated:YES];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark -UIActionSheet delegate

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex

{
    if (buttonIndex == 0) // 调用系统拍照
    {
        if (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
        {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil message:@"该设备不支持拍照功能" delegate:nil cancelButtonTitle:nil otherButtonTitles:@"好", nil];
            [alert show];
        }
        else
        {
            UIImagePickerController * imagePickerController = [[UIImagePickerController alloc]init];
            imagePickerController.sourceType = UIImagePickerControllerSourceTypeCamera;
            imagePickerController.delegate = self;
            imagePickerController.allowsEditing = YES;
            [self presentModalViewController:imagePickerController animated:YES];
        }
    }
    else if (buttonIndex == 1) // 调用系统相册库
    {
        UIImagePickerController * imagePickerController = [[UIImagePickerController alloc]init];
        imagePickerController.navigationBar.tintColor = [UIColor whiteColor];
        if (IOS7_OR_LATER) {
            imagePickerController.navigationBar.barTintColor = [UIColor grayColor];
            imagePickerController.navigationBar.tintColor = [UIColor whiteColor];
        }
        else
        {
            imagePickerController.navigationBar.tintColor = [UIColor grayColor];
        }
        imagePickerController.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        imagePickerController.delegate = self;
        imagePickerController.allowsEditing = YES;
        [self presentModalViewController:imagePickerController animated:YES];
    }
}

#pragma mark - 上传头像 UIImagePickerController delegate

// 选完照片的委托
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    
    if ([info objectForKey: ZBarReaderControllerResults])
    {
        id<NSFastEnumeration> results = [info objectForKey: ZBarReaderControllerResults];
        ZBarSymbol *symbol = nil;
        for(symbol in results)
            break;
        NSString *resultText = symbol.data;
        //解决扫描的二维码是汉字生成时会乱码的问题
        if ([resultText canBeConvertedToEncoding:NSShiftJISStringEncoding])
        {
            resultText = [NSString stringWithCString:[resultText cStringUsingEncoding:NSShiftJISStringEncoding] encoding:NSUTF8StringEncoding];
        }
        PLog(@"resultText: %@",resultText);
        [picker dismissModalViewControllerAnimated: YES];
        
        if (![resultText isMatchedByRegex:@"^[a-zA-Z0-9]+$"] || [resultText length] != 6)
        {
            PLog(@"字符串不符合标准！！！");
            [[iToast makeText:@"该密码密码v不符合密码标准，请重试"] show];
            return;
        }
        else
        {
            self.keyWord = resultText;
            [self login];
        }
    }
  
    else
    {
        if(![[Reachability reachabilityForInternetConnection] isReachable])
        {
            [self dismissModalViewControllerAnimated:YES];
            [self showNetwork];
            return;
        }
        [SVProgressHUD showWithStatus:@"上传中..." maskType:SVProgressHUDMaskTypeBlack];
        [picker dismissModalViewControllerAnimated:YES];
        //UIImage *oldImage = [info objectForKey:UIImagePickerControllerOriginalImage];
        //UIImage *newImage = [self imageWithImage:oldImage scaledToSize:CGSizeMake(640, 640)];  // 640*640
        UIImage *oldImage = [info objectForKey:UIImagePickerControllerEditedImage];
        UIImage *newImage = oldImage;
        NSData* imageData;
        imageData = UIImageJPEGRepresentation(newImage, 0.7);
        _changeHeadImage = [UIImage imageWithData:imageData];
        
        // 保存头像到本地
        NSString *photoName = @"UserHeadPhoto.jpg";
        NSString *path = [[NSHomeDirectory() stringByAppendingPathComponent:@"Documents"] stringByAppendingPathComponent:@"HeadPhotoFile"];
        NSString *headPhotoPath = [path stringByAppendingPathComponent:photoName];
        [imageData writeToFile:headPhotoPath atomically:NO];
        
        // 将头像上传服务器
        long userID = [UserSessionManager getInstance].currentUser.uid;
        NSString *roomId = [UserSessionManager getInstance].currentRoomInfo.roomId;
        NSString *encodeUrl = [PKtvAssistantAPI getUserUploadPhotoUrl:userID roomId:roomId photoName:photoName];
        
        NSString *baseURLstr = kPKtvAssistantAPIDomain;
        NSString *pathURLstr = [encodeUrl stringByReplacingOccurrencesOfString:baseURLstr withString:@""];
        //PLog(@"pathURLstr-->%@",pathURLstr);
        
        NSURL *baseURL = [NSURL URLWithString:baseURLstr];
        AFHTTPClient *httpClient = [[AFHTTPClient alloc] initWithBaseURL:baseURL];
        
        NSMutableURLRequest *request = [httpClient multipartFormRequestWithMethod:@"POST" path:pathURLstr parameters:nil constructingBodyWithBlock: ^(id <AFMultipartFormData>formData) {
            [formData appendPartWithFileData:imageData name:@"imagedata" fileName:photoName mimeType:@"image/jpeg"];
        }];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setUploadProgressBlock:^(NSUInteger bytesWritten, long long totalBytesWritten, long long totalBytesExpectedToWrite) {
            PLog(@"上传 %lld of %lld bytes", totalBytesWritten, totalBytesExpectedToWrite);
        }];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject){
            
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame)
            {
                [SVProgressHUD dismissWithSuccess:@"头像上传成功"];
                PLog(@"头像上传成功...");
                // 更新头像
                [_headButton setImage:_changeHeadImage forState:UIControlStateNormal];
                NSDictionary *result = [results objectForKey:@"result"];
                User *user = [User initWithLoginDictionary:result];
                [[UserSessionManager getInstance] updateCurrentUserInfo:user];
            }
            else
            {
                [SVProgressHUD dismissWithError:@"头像上传失败"];
                PLog(@"头像上传失败...");
                NSString *msg = [results objectForKey:@"msg"];
                NSString *errorCode = [results objectForKey:@"errorcode"];
                PLog(@"doUserUploadPhoto failed...errorcode:%@(%@)", errorCode, msg);
            }
            
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            [SVProgressHUD dismiss];
            PLog(@"doLogin failed...");
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        [operation start];
        
        
        /*   // asi 请求会导致崩溃
        NSString *recordPath = [PCommonUtil storeDataToFile:@"UserHeadPhoto.jpg" subDir:@"photo" data:imageData];
        __unsafe_unretained ASIFormDataRequest *request = [ASIFormDataRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
        [request setRequestMethod:@"POST"];
        [request setTimeOutSeconds:20];
        [request setPostFormat:ASIMultipartFormDataPostFormat];
        [request setFile:recordPath forKey:@"imagedata"];
        [request setCompletionBlock:^{
            NSString *response = [request responseString];
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame)
            {
                [SVProgressHUD dismissWithSuccess:@"上传成功"];
                PLog(@"上传成功...照片大小---%llu",[request postLength]);
                // 更新头像
                [_headButton setImage:_changeHeadImage forState:UIControlStateNormal];
                NSDictionary *result = [results objectForKey:@"result"];
                NSDictionary *userData = [result objectForKey:@"User"];
                User *user = [User initWithUserDictionary:userData];
                [user log];
                [[UserSessionManager getInstance] updateCurrentUserInfo:user];
                [[UserSessionManager getInstance].currentUser log];
            }
            else
            {
                [SVProgressHUD dismissWithError:@"上传失败"];
                NSString *msg = [results objectForKey:@"msg"];
                NSString *errorCode = [results objectForKey:@"errorcode"];
                PLog(@"doUserUploadPhoto failed...errorcode:%@(%@)", errorCode, msg);
            }
        }];
        [request setFailedBlock:^{
            [SVProgressHUD dismissWithError:@"上传失败"];
            PLog(@"doUserUploadPhoto failed...");
        }];
        [request startAsynchronous];
         */
    }
}

#pragma mark - 解决push照片库时状态栏颜色改变

- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated
{
    if (IOS7_OR_LATER) {
        [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
    }
}

- (void)navigationController:(UINavigationController *)navigationController didShowViewController:(UIViewController *)viewController animated:(BOOL)animated
{
    if (IOS7_OR_LATER) {
        [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
    }
}

/**
 * 对图片尺寸进行压缩
 */
-(UIImage*)imageWithImage:(UIImage*)image scaledToSize:(CGSize)newSize
{
    // Create a graphics image context
    UIGraphicsBeginImageContext(newSize);
    
    // Tell the old image to draw in this new context, with the desired
    // new size
    [image drawInRect:CGRectMake(0,0,newSize.width,newSize.height)];
    
    // Get the new image from the context
    UIImage* newImage = UIGraphicsGetImageFromCurrentImageContext();
    
    // End the context
    UIGraphicsEndImageContext();
    
    // Return the new image.
    return newImage;
}

/**
 *	取消按钮的委托
 */
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker

{
    [self dismissModalViewControllerAnimated:YES];
}

#pragma mark -UITableView delegate

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [self.arrayTotal count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString * cellIdentifier = @"UserTotalCell";
    UserTotalCell *cell;
    if ([_totalTable isEqual:tableView])
    {
        cell = (UserTotalCell *)[tableView dequeueReusableCellWithIdentifier:cellIdentifier];
        if (cell == nil)
        {
            cell = (UserTotalCell *)[[[NSBundle mainBundle] loadNibNamed:@"UserTotalCell" owner:self options:nil]objectAtIndex:0];
            cell.backgroundView = nil;
            cell.backgroundColor = UIColorFromRGB(0xe1e2e6);
            cell.selectionStyle = UITableViewCellSelectionStyleNone;
        }
        if ([UserSessionManager isUserAlreadyLogin])
        {
            PointLog *point = (PointLog *)([self.arrayTotal objectAtIndex:indexPath.row]);
            cell.labKind.text = point.source; // 1 歌曲上榜 2 包厢签到
            if (point.scoreType == 1)
            {
                cell.backgroundImage.image = [UIImage imageNamed:@"list_blue.png"];
            }
            else if (point.scoreType == 2)
            {
                cell.backgroundImage.image = [UIImage imageNamed:@"list_green.png"];
            }
        }
  
        PointLog *point = (PointLog *)([self.arrayTotal objectAtIndex:indexPath.row]);
        //_points = point.points;
        cell.labTotal.text = [NSString stringWithFormat:@"%d",point.points];
    }
    return cell;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView;
{
    return 1;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if(![[Reachability reachabilityForInternetConnection] isReachable])
    {
        [self showNetwork];
        return;
    }
//    if ([UserSessionManager isRoomAlreadyLogin])  // 登录账号后 是否 在包厢，在就可以领取，不在提示进包厢
//    {
        // 点击cell 删除 积分累加上去
        long int userID = [UserSessionManager getInstance].currentUser.uid;
        NSString *ktvId = [UserSessionManager getInstance].currentRoomInfo.ktvId;
        PointLog *pointLog = (PointLog *)([self.arrayTotal objectAtIndex:indexPath.row]);
        long int sourceID = pointLog.sourceID;
        NSString *encodeUrl = [PKtvAssistantAPI getReceiveRewardUrl:userID sourceID:sourceID ktvid:ktvId];
        NSURL *url = [NSURL URLWithString:encodeUrl];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject)
         {
             NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
             NSString *status = [results objectForKey:@"status"];
             if ([status compare:@"1"] == NSOrderedSame)
             {
                 NSDictionary *result = [results objectForKey:@"result"];
                 long score = [[result objectForKey:@"score"] longValue];
                 PLog(@"领取success，用户最新积分:%ld", score);
                 [[UserSessionManager getInstance] updateCurrentUserGold:score];

                 [_arrayTotal removeObjectAtIndex:indexPath.row];
                 [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
                 [tableView deselectRowAtIndexPath:indexPath animated:YES];
                 
                 // 刷新领取后的积分界面
                 [self flashUIWithAddScore];
                 
                 // 积分累加
                 _AnimationTotal = [[UILabel alloc] initWithFrame:CGRectMake(155, 170, 100, 30)];
                 _AnimationTotal.hidden = YES;
                 _AnimationTotal.alpha = 1.0;
                 _AnimationTotal.text = [NSString stringWithFormat:@"+%d",pointLog.points];
                 _AnimationTotal.textColor = [UIColor whiteColor];
                 _AnimationTotal.backgroundColor = [UIColor clearColor];
                 [self.view addSubview:_AnimationTotal];
                 CGRect frame  =_AnimationTotal.frame;
                 [UIView beginAnimations:@"animation" context:nil];
                 [UIView setAnimationDuration:2];
                 [UIView setAnimationRepeatCount:1.0];
                 [UIView setAnimationCurve:UIViewAnimationCurveEaseInOut];
                 _AnimationTotal.hidden = NO;
                 frame.origin.y -= 15;
                 _AnimationTotal.frame = frame;
                 _AnimationTotal.alpha = 0.0;
                 [UIView commitAnimations];
                 
                 if (self.arrayTotal.count == 0)
                 {
                     _noMessageView.hidden = NO;
                 }
             }
             else
             {
                 NSString *msg = [results objectForKey:@"msg"];
                 NSString *errorCode = [results objectForKey:@"errorcode"];
                 [CommenTools showAlertViewWithErrorMessage:msg];
                 PLog(@"doReceivePoints failed...errorcode:%@(%@)", errorCode, msg);
             }
         }
        failure:^(AFHTTPRequestOperation *operation, NSError *error)
         {
             PLog(@"doLogin failed...");
             [CommenTools showNetworkDisconnectedAlertView];
         }];
        
        [operation start];
//    }
//    else
//    {
//        UIAlertView *noticeJoinRoom = [[UIAlertView alloc] initWithTitle:nil message:@"亲，您还未进入包厢，进包厢即可领取" delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"好", nil];
//        [self.view addSubview:noticeJoinRoom];
//        [noticeJoinRoom show];
//    }
}

#pragma mark - 刷新用户信息UI

- (void)flashUILoginInfo
{
    if ([UserSessionManager isUserAlreadyLogin])
    {
        // 昵称
         _userName.text = [UserSessionManager getInstance].currentUser.name;
        // 积分数据
        [self flashUIWithAddScore];
        // 性别
        int IntSex = [UserSessionManager getInstance].currentUser.gender;
        if (IntSex == 0)
        {
            [_sex setImage:[UIImage imageNamed:@"icon_mm.png"]];
        }
        else
        {
            [_sex setImage:[UIImage imageNamed:@"icon_gg.png"]];
        }
        // 原始分辨率头像
        NSString *headPhotoString = [UserSessionManager getInstance].currentUser.headphoto;
        // 服务器返回的标准分辨率头像
        NSString *standardHeadPhotoString = [CommenTools GetURLWithResolutionScaleTransfered:headPhotoString scale:0];
        
        [self.egoHeadImage setImageWithURL:[NSURL URLWithString:standardHeadPhotoString] placeholderImage:[UIImage imageNamed:@"pic_touxiang2"]];
        
        [_beachBallSpinner stopAnimating];
    }
    else
    {
        // 默认昵称 用户名
         _userName.text = @"用户名";
        // 默认积分 0
        _total.text = [NSString stringWithFormat:@"0"];
        // 默认性别 空
        [_sex setImage:nil];
        // 默认头像
        [_headButton setImage:[UIImage imageNamed:@"pic_touxiang2.png"] forState:UIControlStateNormal];//默认头像
    }
}

#pragma mark - 领取积分后刷新积分UI

- (void)flashUIWithAddScore
{
    // 用户总积分分数
    long int gold = [UserSessionManager getInstance].currentUser.gold;
    self.total.text = [NSString stringWithFormat:@"%ld",gold];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

@end
