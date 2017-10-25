//
//  BoxPreBookViewController.m
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-10-23.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BoxPreBookViewController.h"
#import "PKtvAssistantAPI.h"
#import "CMActionSheet.h"
#import "KTVInputSecretController.h"
#import "RegexKitLite.h"
#import "iToast.h"
#import "KTVAppDelegate.h"
//#import "BPush.h"
#import "xingguang.h"
#import "iosdatadefine.h"
#import "com_ios_NetUtil.h"
#import "NetUtilCallBack.h"
#import "KTVBaseConfig.h"
#import "UserSessionManager.h"
#import "DDMenuController.h"


@interface BoxPreBookViewController ()

@end

@implementation BoxPreBookViewController

@synthesize appRecommendedWebView = _appRecommendedWebView;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

#pragma mark - viewWillAppear

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self flashUIWithRoom]; // 退出包厢刷新UI
    [self.appDelegate.menuController setEnableGesture:NO];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(disconnRoom:) name:KTVASSISTANT_CLOSEROOM_NOTIFICATION object:nil];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:YES];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
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


#pragma mark - viewDidLoad

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self.navView initOnlyBackButton:@"包厢预订"];
    self.view.backgroundColor = [UIColor whiteColor];
    
//    self.toolBar.hidden = NO;
//    self.joinButton.hidden = NO;
//    // 进入包厢
//    [self.joinButton addTarget:self action:@selector(goRoom:) forControlEvents:UIControlEventTouchUpInside];
//    
//    // 退出包厢
//    [self.power addTarget:self action:@selector(exitBox:) forControlEvents:UIControlEventTouchUpInside];
    
    //[self flashUIWithRoom]; // 退出包厢刷新UI
    
    _appRecommendedWebView = [[UIWebView alloc] init];
    _appRecommendedWebView.backgroundColor = [UIColor clearColor];
    _appRecommendedWebView.frame = CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height);
    _appRecommendedWebView.delegate = self;
    [self.mainContentView addSubview:_appRecommendedWebView];
    
    // 红色圈圈指示器
    _circularSpinner = [[TJSpinner alloc] initWithSpinnerType:kTJSpinnerTypeCircular];
    _circularSpinner.frame = CGRectMake(115, 130, 100, 100);
    _circularSpinner.hidesWhenStopped = YES;
    _circularSpinner.radius = 10;
    _circularSpinner.pathColor = [UIColor whiteColor];
    _circularSpinner.fillColor = UIColorFromRGB(0xd23675);
    _circularSpinner.thickness = 7;
    [_circularSpinner startAnimating];
    [self.mainContentView addSubview:_circularSpinner];
    
    [self loadWebView:kPKtvAssistantAPIAppBoxVodUrl];  // 加载webView
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
            //[BPush delTag:tag];
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
             //[BPush setTag:tag];
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

#pragma mark - 加载webView

-(void)loadWebView:(NSString *)tRequestUrl
{
    if(![[Reachability reachabilityForInternetConnection] isReachable])
    {
        [_circularSpinner stopAnimating];
        [self showNetwork];
        return;
    }
    
    NSURL *tempUrl = [NSURL URLWithString:tRequestUrl];
    [_appRecommendedWebView loadRequest:[[NSURLRequest alloc] initWithURL:tempUrl]];
}

#pragma mark - UIWebViewDelegate

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSString  *urlStr = request.URL.absoluteString;
    PLog(@"request.URL.absoluteString: %@", request.URL.absoluteString);
    NSString *frontStr = [urlStr substringToIndex:5];
    if ([frontStr isEqualToString:@"call:"])
    {
        telnum = [urlStr substringFromIndex:5];
        //********* 自定义表单 *********
        CMActionSheet *actionSheet = [[CMActionSheet alloc] init];
        [actionSheet addSeparator];
        
        [actionSheet addButtonWithTitle:telnum type:CMActionSheetButtonTypeWhite block:^{
            [self callAction];
            
        }];
        [actionSheet addButtonWithTitle:@"取消" type:CMActionSheetButtonTypeWhite block:^{
            PLog(@"取消");
        }];
        [actionSheet present];

        return NO;
    }
    
    //处理请求内容，拦劫请求
    return YES;
}

#pragma mark - 打电话

- (void)callAction
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"tel://%@",telnum]]];
}

- (void)webViewDidStartLoad:(UIWebView *)webView
{
    PLog(@"webViewDidStartLoad...");
    [_circularSpinner startAnimating];
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    PLog(@"webViewDidFinishLoad");
    [_circularSpinner stopAnimating];
//    if ([webView canGoBack])
//    {
//        self.navView.leftButton.hidden = YES;
//        self.navView.backButton.hidden = NO;
//    }
//    else
//    {
//        self.navView.leftButton.hidden = NO;
//        self.navView.backButton.hidden = YES;
//    }
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    PLog(@"didFailLoadWithError...");
    [_circularSpinner stopAnimating];
    [SVProgressHUD showErrorWithStatus:kPKtvAssistantErrorMsgSystem];
}

// 返回按钮
-(void)doBack:(id)sender
{
    if ([_appRecommendedWebView canGoBack]) {
         [_appRecommendedWebView goBack];
    }
    else
        [super doBack:sender];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
