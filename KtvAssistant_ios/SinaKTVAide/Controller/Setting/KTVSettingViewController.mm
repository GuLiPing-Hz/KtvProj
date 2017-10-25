
//
//  KTVSettingViewController.m
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-10-31.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "KTVSettingViewController.h"
#import "KTVAboutController.h"
#import "MobClick.h"
#import "UMFeedback.h"
#import "PKtvAssistantAPI.h"
#import "KTVAppRecommendedViewController.h"
#import "CMActionSheet.h"
#import "KTVInputSecretController.h"
#import "RegexKitLite.h"
#import "iToast.h"
#import "KTVAppDelegate.h"
//#import "BPush.h"
#import "com_ios_NetUtil.h"
#import "NetUtilCallBack.h"
#import "KTVBaseConfig.h"
#import "UserSessionManager.h"
#import "Reachability.h"
#import "AFHTTPRequestOperation.h"
#import "PCommonUtil.h"
#import "CommenTools.h"
#import "SVProgressHUD.h"
#import "xingguang.h"
#import "UserLoginController.h"
#import "DDMenuController.h"
#import "BaseNavigationViewController.h"

@interface KTVSettingViewController ()

@end

@implementation KTVSettingViewController

@synthesize setTableView = _setTableView;
@synthesize isNeedUpdate = _isNeedUpdate;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

#pragma mark - viewDidLoad

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self.navView initOnlyBackButton:@"设置"];
    
    _setTableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, self.mainContentView.frame.size.width, self.mainContentView.frame.size.height/*-50*/) style:UITableViewStyleGrouped];
    self.mainContentView.backgroundColor = UIColorFromRGB(0xecedf2);
    _setTableView.backgroundColor = [UIColor clearColor];
    _setTableView.delegate = self;
    _setTableView.dataSource = self;
    _setTableView.rowHeight = 46.0f;
    [self.mainContentView addSubview:_setTableView];
    
    _logOut = [UIButton buttonWithType:UIButtonTypeCustom];
    _logOut.frame = CGRectMake(10, 240 , _MainScreen_Width-20, 41);
    _logOut.backgroundColor = [UIColor clearColor];
    _logOut.titleLabel.font = [UIFont systemFontOfSize:18.0f];
    [_logOut setTitle:@"退出登录" forState:UIControlStateNormal];
    [_logOut setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
    [_logOut setBackgroundImage:[UIImage imageNamed:@"btn_login_Out.png"] forState:UIControlStateNormal];
    [_logOut setBackgroundImage:[UIImage imageNamed:@"btn_login_Out_c.png"] forState:UIControlStateHighlighted];
    [_logOut addTarget:self action:@selector(userLogOut:) forControlEvents:UIControlEventTouchUpInside];
    [self.setTableView addSubview:_logOut];
    //[self changeLoginFlag]; // 登录状态
    
	_isNeedUpdate = NO;
    
//    self.toolBar.hidden = NO;
//    self.joinButton.hidden = NO;
//    // 进入包厢
//    [self.joinButton addTarget:self action:@selector(goRoom:) forControlEvents:UIControlEventTouchUpInside];
//    
//    // 退出包厢
//    [self.power addTarget:self action:@selector(exitBox:) forControlEvents:UIControlEventTouchUpInside];
//    
//    [self flashUIWithRoom]; // 退出包厢刷新UI
}

#pragma mark - 注销或者登录

- (void)userLogOut:(UIButton *)button
{
        //注销
    _altOutLogin = [[UIAlertView alloc] initWithTitle:nil message:@"确定要退出当前登录账号吗?" delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"退出", nil];
    [_altOutLogin show];
    
    if ([UserSessionManager isUserAlreadyLogin])
    {
    }
}

#pragma mark - 登录状态

- (void)changeLoginFlag
{
    if ([UserSessionManager isUserAlreadyLogin])
    {
        [_logOut setTitle:@"退出登录" forState:UIControlStateNormal];
    }
    else
    {
        [_logOut setTitle:@"立即登录" forState:UIControlStateNormal];
    }
}

#pragma mark -  UIActionSheet 进入包厢

- (void)goRoom:(UIButton *)btn

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

- (void)exitBox:(UIButton *)button
{
    _altExitRoom = [[UIAlertView alloc] initWithTitle:@"温馨提示" message:@"你确定要退出包厢吗?" delegate:self cancelButtonTitle:@"点错了" otherButtonTitles:@"好", nil];
    [_altExitRoom show];
}

#pragma mark - 退出包厢/注销账号 UIAlertView delegate

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 1)
    {
        /**
         注销账号
         */
        if (alertView == _altOutLogin)
        {
            if ([UserSessionManager isUserAlreadyLogin])
            {
                if(![[Reachability reachabilityForInternetConnection] isReachable])
                {
                    [self showNetwork];
                    return;
                }
                long int userID = [UserSessionManager getInstance].currentUser.uid;
                NSString *roomId = [UserSessionManager getInstance].currentRoomInfo.roomId;
                NSString *encodeUrl = [PKtvAssistantAPI getUserLogoutUrl:userID roomId:roomId];
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
                         BOOL res = [[result objectForKey:@"success"] boolValue];
                         if(res)
                         {
                             PLog(@"注销成功");
                             [[iToast makeText:@"注销成功"] show];
                             [UserSessionManager clearCurrentUserInfo]; // 清空个人信息
                             User *user = [UserSessionManager getInstance].currentUser;
                             UserLoginController *loginController = [[UserLoginController alloc] init];
                             UINavigationController *naviMain = [[BaseNavigationViewController alloc] initWithRootViewController:loginController];
                             self.appDelegate.window.rootViewController = naviMain;
                             DisConnectRoom();
                             DisConnectLobby();
                             [NetUtilCallBack getInstance]->m_listLobbyUserInfo.clear();
                             [NetUtilCallBack getInstance]->m_listPrincessInfo.clear();
                             [NetUtilCallBack getInstance]->m_listRoomUserInfo.clear();
                             [NetUtilCallBack getInstance]->m_vectAllInfo.clear();
                             [NetUtilCallBack getInstance]->m_vectreqsonglist.clear();
                         }
                     }
                     else
                     {
                         PLog(@"注销失败");
                         [[iToast makeText:@"注销失败"] show];
                         NSString *msg = [results objectForKey:@"msg"];
                         PLog(@"doUserLogout failed...%@", msg);
                     }
                 }
                 failure:^(AFHTTPRequestOperation *operation, NSError *error)
                 {
                     PLog(@"--failed...");
                     [CommenTools showNetworkDisconnectedAlertView];
                 }];
                [operation start];
            }
        }

        /**
         退出包厢
         */
        if (alertView == _altExitRoom)
        {
            //[SVProgressHUD showWithStatus:@"正在退出..." maskType:SVProgressHUDMaskTypeBlack];
            long int userID = [UserSessionManager getInstance].currentUser.uid;
            NSString *password  = [UserSessionManager getInstance].currentRoomInfo.roomPassword;
            
            NSString *roomId = [UserSessionManager getInstance].currentRoomInfo.roomId;
            NSString *encodeUrl = [PKtvAssistantAPI getRoomLogoutUrl:userID roomID:roomId password:password];
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
                     PLog(@"退出包厢成功");
                     DisConnectRoom();
//                     NSString * tag = [[UserSessionManager getInstance].currentRoomInfo.roomPassword lowercaseString];
//                     //PLog(@"TAG-->>%@",tag);
//                     [BPush delTag:tag];
//                     [SVProgressHUD dismissWithSuccess:@"成功退出包厢" afterDelay:1.0];
//                     [UserSessionManager clearCurrentRoomInfo];
//                     
//                     [self flashUIWithRoom]; // 退出包厢刷新room
                 }
                 else
                 {
                     PLog(@"退出包厢失败");
                     //[SVProgressHUD dismissWithError:@"退出包厢失败"];
                     NSString *msg = [results objectForKey:@"msg"];
                     NSString *errorCode = [results objectForKey:@"errorcode"];
                     PLog(@"doLoginRoomWithUser failed...errorcode:%@(%@)", errorCode, msg);
                 }
                 
                 
             }
             failure:^(AFHTTPRequestOperation *operation, NSError *error)
             {
                 PLog(@"doLogin failed...");
                 //[SVProgressHUD dismiss];
                 //[CommenTools showNetworkDisconnectedAlertView];
             }];
             [operation start];
            
            NSString * tag = [password lowercaseString];
            //PLog(@"TAG-->>%@",tag);
            //[BPush delTag:tag withCompleteHandler:nil];
            [UserSessionManager clearCurrentRoomInfo];
            [self flashUIWithRoom]; // 退出包厢刷新room
        }
    }
    
    [alertView dismissWithClickedButtonIndex:0 animated:YES];
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

#pragma mark - UIImagePickerController delegate

- (void) imagePickerController: (UIImagePickerController*) reader
 didFinishPickingMediaWithInfo: (NSDictionary*) info
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
    [reader dismissModalViewControllerAnimated: YES];
    
    if (![resultText isMatchedByRegex:@"^[a-zA-Z0-9]+$"] || [resultText length] != 6)
    {
        PLog(@"字符串不符合标准！！！");
        [[iToast makeText:@"该密码密码不符合密码标准，请重试"] show];
        return;
    }
    else
    {
        self.keyWord = resultText;
        [self login];
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
//    if ([UserSessionManager isRoomAlreadyLogin])   // 已进入包厢
//    {
//        NSString *roomName = [UserSessionManager getInstance].currentRoomInfo.roomName;
//        self.inRoomNumber.text = [NSString stringWithFormat:@"所在包厢: %@",roomName];
//        self.joinButton.hidden = YES;
//        self.power.hidden = NO;
//        self.inRoomNumber.hidden = NO;
//    }
//    else  // 未进入包厢
//    {
//        self.joinButton.hidden = NO;
//        self.power.hidden = YES;
//        self.inRoomNumber.hidden = YES;
//    }
}


#pragma mark - viewWillAppear

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    [self.appDelegate.menuController setEnableGesture:YES];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self.appDelegate.menuController setEnableGesture:false];
    
    [MobClick checkUpdateWithDelegate:self selector:@selector(updateMethod:)];
    
    [self flashUIWithRoom]; // 退出包厢刷新UI
    //[self changeLoginFlag]; // 登录状态
    //[self.appDelegate.menuController setEnableGesture:NO];
    
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

#pragma mark UItableView delegate

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (section == 0)
    {
        return 2;
    }
    else
    {
        return 1;
    }
    return 0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *dataCellIdentifier = @"AppSettingCell";
    UITableViewCell *cell = (UITableViewCell *)[tableView  dequeueReusableCellWithIdentifier:dataCellIdentifier];
    if (cell ==nil)
    {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:dataCellIdentifier];
        // 更新标志
        UIImageView *newTipImageView = [[UIImageView alloc] initWithFrame:CGRectMake(230, 14, 33, 21)];
        newTipImageView.tag = 2004;
        newTipImageView.hidden = YES;
        newTipImageView.image = [UIImage imageNamed:@"new.png"];
        [cell.contentView addSubview:newTipImageView];
        
        cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        cell.imageView.transform = CGAffineTransformMakeScale(0.5, 0.5);
        cell.textLabel.backgroundColor = [UIColor clearColor];
        cell.textLabel.textColor = UIColorFromRGB(0x444444);
        cell.textLabel.font = [UIFont systemFontOfSize:17.0f];
    }

    UIImageView *newTipImageView = (UIImageView *)[cell.contentView viewWithTag:2004];
    if (indexPath.section == 0 && indexPath.row == 1)
    {
        if (_isNeedUpdate) //提示有新版本
        {
            newTipImageView.hidden = NO;
        }
        else
        {
            newTipImageView.hidden = YES;
        }
    }
    if (indexPath.section == 0)
    {
        switch (indexPath.row)
        {
            case 0:
            {
                cell.imageView.image = [UIImage imageNamed:@"bth_guanyu.png"];
                cell.textLabel.text = @"关于";
            }
                break;
            case 1:
            {
                cell.imageView.image = [UIImage imageNamed:@"bth_fankui.png"];
                cell.textLabel.text = @"用户反馈";
            }
                break;
            default:
                break;
        }
    }
    else if (indexPath.section == 1)
    {
        switch (indexPath.row)
        {
            case 0:
            {
                cell.imageView.image = [UIImage imageNamed:@"bth_tuijian.png"];
                cell.textLabel.text = @"应用推荐";
            }
                break;
            default:
                break;
        }
    }
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.section == 0)
    {
        if (indexPath.row == 0)
        {
            KTVAboutController *about = [[KTVAboutController alloc] init];
            [self.navigationController pushViewController:about animated:YES];

        }
        else if (indexPath.row == 1)
        {
            [UMFeedback showFeedback:self withAppkey:UMENG_APPKEY];
        }
    }
    else if (indexPath.section == 1)
    {
        KTVAppRecommendedViewController *appRecommended = [[KTVAppRecommendedViewController alloc] init];
        [self.navigationController pushViewController:appRecommended animated:YES];
    
    }
    [self.setTableView deselectRowAtIndexPath:indexPath animated:YES];
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    if (section == 0||section == 1)
    {
        return 20.0f;
    }
    return 1.0;
}

- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section
{
    return 1.0;
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)updateMethod:(NSDictionary *)appInfo
{
    PLog(@"update info %@",appInfo);
    
    if ([appInfo isKindOfClass:[NSDictionary class]])
    {
        _isNeedUpdate = [[appInfo objectForKey:@"update"] boolValue];
        
        [_setTableView reloadData];
    }
}

@end
