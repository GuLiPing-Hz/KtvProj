//
//  KTVAppDelegate.m
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-10-23.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "KTVAppDelegate.h"
#import "LeftViewController.h"
#import "BaseNavigationViewController.h"
#import "DDMenuController.h"
#import "PKtvAssistantAPI.h"
#import "UserSessionManager.h"
#import "MobClick.h"
#import "KTVMainViewController.h"
#import "UserLoginController.h"
//#import "BPush.h"
#import "User.h"
#import "MessageViewController.h"
#import "com_ios_NetUtil.h"
#import "NetUtilCallBack.h"
#import "KTVBaseConfig.h"
#import "AFHTTPRequestOperation.h"
#import "PCommonUtil.h"
#import "CommenTools.h"
#import "Reachability.h"
#import "xingguang.h"
#import "UserLoginController.h"
#import <TencentOpenAPI/TencentOAuth.h>
#import "iToast.h"


@implementation KTVAppDelegate

@synthesize naviMain =_naviMain;
@synthesize menuController = _menuController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions

{
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0)
    {
        [[UIApplication sharedApplication] registerUserNotificationSettings:[UIUserNotificationSettings
                                                                             settingsForTypes:(UIUserNotificationTypeSound | UIUserNotificationTypeAlert | UIUserNotificationTypeBadge)
                                                                             categories:nil]];
        
        
        [[UIApplication sharedApplication] registerForRemoteNotifications];
    }

    /************************************/
    //微博注册
    [WeiboSDK enableDebugMode:YES];
    [WeiboSDK registerApp:kWeiboAppKey];
    
    /************************************/
    //微信注册
    [WXApi registerApp:kWeiXinAppID withDescription:APP_NAME];
    /*********************/
    //[BPush setupChannel:launchOptions]; // 必须
    
    //[BPush setDelegate:self]; // 必须。参数对象必须实现onMethod: response:方法，本示例中为self
    
    // [BPush setAccessToken:@"3.ad0c16fa2c6aa378f450f54adb08039.2	592000.1367133742.282335-602025"];  // 可选。api key绑定时不需要，也可在其它时机调用
    
    if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 8.0) {
        [[UIApplication sharedApplication] registerUserNotificationSettings:[UIUserNotificationSettings settingsForTypes:
            UIUserNotificationTypeSound
            | UIUserNotificationTypeBadge
            | UIUserNotificationTypeAlert
    categories:nil]];
        [[UIApplication sharedApplication] registerForRemoteNotifications];
    }
    else{
        [application registerForRemoteNotificationTypes:
         UIRemoteNotificationTypeAlert
         | UIRemoteNotificationTypeBadge
         | UIRemoteNotificationTypeSound];
    }
    
    /*********************/
    /**
     自动登陆
     */
     User *user = [[UserSessionManager getInstance] readUserInfoFromFile];
    
    /*********************/
    
    [self umengTrack];
    [self isShow];
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.backgroundColor = [UIColor whiteColor];
    self.bannerListArray = [[NSMutableArray alloc]init];
    self.bannerImageArray = [[NSMutableArray alloc]init];
    self.singerList = [[NSMutableArray alloc] init];
    
    UserLoginController *loginController = [[UserLoginController alloc] init];
    _naviMain = [[BaseNavigationViewController alloc] initWithRootViewController:loginController];
    self.window.rootViewController = _naviMain;
    if (user)
    {
        [SVProgressHUD showWithStatus:@"正在登录"  maskType:SVProgressHUDMaskTypeClear];
        if(![[Reachability  reachabilityForInternetConnection] isReachable])
        {
            [SVProgressHUD showErrorWithStatus:@"网络有问题,请重新连接"];
        }
        else
        {
            
            [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(recvLoginLobbyMsg:) name:KTVAssistantLoginLobbyResult object:nil];
            
            User* user = [UserSessionManager getInstance].currentUser;
            [self relogin:user.xgid pwd:user.pwd];
            
//            [NetUtilCallBack getInstance]->m_listLobbyUserInfo.clear();
//            [NetUtilCallBack getInstance]->m_listPrincessInfo.clear();
//            [NetUtilCallBack getInstance]->m_listRoomUserInfo.clear();
//            [[NetUtilCallBack getInstance] doCurrentWork:TransactionLobbyLogin];
        }
    }
    
    // 将info里 add key 名叫View controller-based status bar appearance，并将其值设置为NO就是亮色、YES就是暗色。
    // 下面这句可以随意改亮色和暗色
    if (IOS7_OR_LATER)
    {
        [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
    }
    else
    {
        [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleDefault];
    }
    
    UILocalNotification *localNotification =
    [launchOptions objectForKey:UIApplicationLaunchOptionsLocalNotificationKey];
    if (localNotification)
    {
        PLog(@"Notification Body: %@",localNotification.alertBody);
        PLog(@"%@", localNotification.userInfo);
        NSDictionary *infoDic = [localNotification userInfo];
        NSString *roomSecret = [infoDic objectForKey:@"roomSecret"];
        [self loginVerify:roomSecret];
    }
    [self.window makeKeyAndVisible];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(updateUserGoldFromInternal) name:KTVASSISTANT_USERINFO_UPDATE_NOTIFICATION object:nil];
    return YES;
}

#pragma mark recv
-(void)recvLoginLobbyMsg:(NSNotification *)center
{
    NSDictionary *dic =[center userInfo];
    NSInteger type = [[dic objectForKey:@"result"] integerValue];
    if (type)
    {
        [SVProgressHUD dismiss];
        [self loginMain];
    }
    else
    {
        [SVProgressHUD showErrorWithStatus:@"网络有问题,请重新连接"];
    }
    [[NSNotificationCenter defaultCenter]removeObserver:self name:KTVAssistantLoginLobbyResult object:nil];
}


-(void)updateUserGoldFromInternal
{
    long uid = [UserSessionManager getInstance].currentUser.uid;
    NSString * strUrl = [PKtvAssistantAPI getUserGetInfoUrl:uid];
    NSURL *url = [[NSURL alloc] initWithString:strUrl];
    NSURLRequest *request = [[NSURLRequest alloc]initWithURL:url];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc]initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary *html = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        NSString *status = [html objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [html objectForKey:@"result"];
            NSDictionary *userDic = [result objectForKey:@"User"];
            User *user = [User initWithUserDictionary:userDic];
            [[UserSessionManager getInstance] updateCurrentUserGold:user.gold];
            [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_GOLD_UPDATE_NOTIFICATION object:nil userInfo:nil];
        }
        else
        {
            
        }
        
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        
    }];
    [operation start];
}

//隐藏审核问题模块
-(void)isShow
{
    NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];
    NSString *defaultKey = KTVASSISTANT_USERDEFAULTS_ISSHOW;
    NSString *value = [defaults objectForKey:defaultKey];
    if (value && [value compare:@"1"] == NSOrderedSame)
    {
       return;
    }
    else
    {
        NSString *strUrl = [PKtvAssistantAPI getShow];
        NSURL *url = [NSURL URLWithString:strUrl];
        NSURLRequest *request = [NSMutableURLRequest requestWithURL:url];
        
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject)
         {
             NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData2:responseObject];
             NSString *isShow = [results objectForKey:@"result"];
             [defaults setObject:isShow forKey:defaultKey];
             [defaults synchronize];
             [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_ISSHOW_NOTIFICATION object:nil userInfo:nil];
         }
         failure:^(AFHTTPRequestOperation *operation, NSError *error)
         {
             
         }
        ];
        
        [operation start];
    }
}

-(void)GetShow
{
    

}


-(void)loginMain
{
    /* 中视图--KTV首页 */
    KTVMainViewController *centerController = [[KTVMainViewController alloc] init];
    
    //MessageDetailViewController *centerController = [[MessageDetailViewController alloc] init];
    _naviMain = [[BaseNavigationViewController alloc] initWithRootViewController:centerController];
    
    /* 抽屉视图类 */
    _menuController = [[DDMenuController alloc] initWithRootViewController:_naviMain];
    
    /* 左视图 */
    LeftViewController* leftController = [[LeftViewController alloc] init];
    [leftController setIndexPage:centerController];
    _menuController.leftViewController = leftController;
    
    /* 右视图 */
    _menuController.rightViewController = nil;
    self.window.rootViewController = _menuController;
    
}

-(void)relogin:(NSString*)kid pwd:(NSString*)pwd
{
    NSString *strUrl = [PKtvAssistantAPI getUserLoginUrl:kid pwd:pwd];
    NSURL *url = [NSURL URLWithString:strUrl];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url];
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject)
     {
         NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
         
         NSString *status = [results objectForKey:@"status"];
         if ([status compare:@"1"] == NSOrderedSame)
         {
             NSDictionary *result = [results objectForKey:@"result"];
             User *user = [User initWithLoginDictionary:result];
             user.pwd = pwd;
             [UserSessionManager getInstance].currentUser = user;
             [[UserSessionManager getInstance] writeUserInfoToFile];
             //星光模块进行登录操作
             [NetUtilCallBack getInstance]->m_listLobbyUserInfo.clear();
             [NetUtilCallBack getInstance]->m_listRoomUserInfo.clear();
             [NetUtilCallBack getInstance]->m_listPrincessInfo.clear();
             [NetUtilCallBack getInstance]->m_reconnectTimes = 1;
             [[NetUtilCallBack getInstance] doCurrentWork:TransactionLobbyLogin];
             //[SVProgressHUD dismiss]; 继续登录
         }
         else
         {
             NSString *msg = [results objectForKey:@"msg"];
             NSString *errorCode = [results objectForKey:@"errorcode"];
             PLog(@"doOpenLogin failed...%@(%@)", msg, errorCode);
             
             [SVProgressHUD dismiss];
             
             if (msg.length == 0)
             {
                 [SVProgressHUD showErrorWithStatus:@"账号或密码错误"];
             }
             else
             {
                 [SVProgressHUD showErrorWithStatus:msg];
             }
         }
         
     }failure:^(AFHTTPRequestOperation *operation, NSError *error)
     {
         [SVProgressHUD dismiss];
         [CommenTools showNetworkDisconnectedAlertView];
     }];
    
    [operation start];
}

#pragma mark - 重写 回调

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    UserLoginController *user = [[UserLoginController alloc] init];
    if([WXApi handleOpenURL:url delegate:user])
        return YES;
    else if([TencentOAuth HandleOpenURL:url])
        return YES;
    else if([WeiboSDK handleOpenURL:url delegate:user])
        return YES;
    else
        return YES;
}
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
    UserLoginController *user = [[UserLoginController alloc] init];
    if([WXApi handleOpenURL:url delegate:user])
        return YES;
    else if([TencentOAuth HandleOpenURL:url])
        return YES;
    else if([WeiboSDK handleOpenURL:url delegate:user])
        return YES;
    else
        return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    DisConnectLobby();
    //[BPush unbindChannel];
}

- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification NS_AVAILABLE_IOS(4_0){
    PLog(@"didReceiveLocalNotification");
    NSDictionary *infoDic = [notification userInfo];
    //PLog(@"userInfo-->%@",infoDic);
    NSString *roomSecret = [infoDic objectForKey:@"roomSecret"];
    [self loginVerify:roomSecret];
}

- (void)loginVerify:(NSString *)roomSecret
{
    NSString *encodeUrl = [PKtvAssistantAPI getLoginUrl:roomSecret];
    NSURL *url = [NSURL URLWithString:encodeUrl];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            RoomInfo *roominfo = [RoomInfo initWithDictionary:result];
            //[roominfo Log];
            [[UIApplication sharedApplication] cancelAllLocalNotifications]; // 撤销所有的Notification
            
            UILocalNotification *localNotification = [[UILocalNotification alloc] init];
            
            // 设置notification的属性
            localNotification.fireDate = [[NSDate new] dateByAddingTimeInterval:roominfo.periodsTime]; //出发时间
            localNotification.alertBody = @"您的包厢已到期，请续费或欢迎下次光临"; // 消息内容
            localNotification.soundName = UILocalNotificationDefaultSoundName; // 触发消息时播放的声音
            //设置随Notification传递的参数
            NSDictionary *infoDict = [NSDictionary dictionaryWithObjectsAndKeys:@"reminder", @"notificationId", roomSecret, @"roomSecret", nil];
            localNotification.userInfo = infoDict;
            [[UIApplication sharedApplication] scheduleLocalNotification:localNotification];
            [UserSessionManager getInstance].currentRoomInfo = roominfo;
            [UserSessionManager getInstance].isLoggedIn = YES;
            
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"loginVerify failed...errorMsg:%@(%@)", msg, errorCode);
            _loginoutAlert = [[UIAlertView alloc] initWithTitle:@"提示" message:@"您包厢的时间已经到了,请退出房间或者续费" delegate:self cancelButtonTitle:@"好" otherButtonTitles:nil, nil];
            [_loginoutAlert show];
            
            [self clearRoomInfo];
            
        }
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        PLog(@"doLogin failed...");
        
        [CommenTools showNetworkDisconnectedAlertView];
    }];
    [operation start];
}

/**
 *  重置房间信息，并且退出百度推送组
 */
-(void) clearRoomInfo{
    @try {
        NSString * tag = [[UserSessionManager getInstance].currentRoomInfo.roomPassword lowercaseString];
        //PLog(@"TAG-->>%@",tag);
        //[BPush delTag:tag];
        
        [UserSessionManager clearCurrentRoomInfo];
    }@catch (NSException *exception) {
        PLog(@"[BPush delTag:tag] Fail!!!-->exception");
    }

}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(alertView == _loginoutAlert){
        //包厢到期对话框
        PLog(@"包厢到期对话框");
        
        KTVMainViewController *centerController = [[KTVMainViewController alloc] init];
        [(UINavigationController *)self.menuController pushViewController:centerController animated:YES];
        
    }else if (alertView == _notificationAlert){
        //推送通知对话框
        PLog(@"推送通知对话框");
    }
}

// 友盟
- (void)umengTrack
{
    //[MobClick setCrashReportEnabled:NO]; // 如果不需要捕捉异常，注释掉此行
    //[MobClick setLogEnabled:YES];  // 打开友盟sdk调试，注意Release发布时需要注释掉此行,减少io消耗
    [MobClick setAppVersion:XcodeAppVersion]; //参数为NSString * 类型,自定义app版本信息，如果不设置，默认从CFBundleVersion里取
    //
    [MobClick startWithAppkey:UMENG_APPKEY reportPolicy:(ReportPolicy) REALTIME channelId:nil];
    //reportPolicy为枚举类型,可以为 REALTIME, BATCH,SENDDAILY,SENDWIFIONLY几种
    //channelId 为NSString * 类型，channelId 为nil或@""时,默认会被被当作@"App Store"渠道
    
    [MobClick checkUpdate];   //自动更新检查, 如果需要自定义更新请使用下面的方法,需要接收一个(NSDictionary *)appInfo的参数
    //[MobClick checkUpdateWithDelegate:self selector:@selector(updateMethod:)];
    
    [MobClick updateOnlineConfig];  //在线参数配置
    
    //1.6.8之前的初始化方法
    //[MobClick setDelegate:self reportPolicy:REALTIME];  //建议使用新方法
    //[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onlineConfigCallBack:) name:UMOnlineConfigDidFinishedNotification object:nil];
}


//百度推送

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    //PLog(@"test:%@",deviceToken);
    //[BPush registerDeviceToken: deviceToken];
    //[BPush bindChannel];
    //self.viewController.textView.text = [self.viewController.textView.text stringByAppendingFormat: @"Register device token: %@\n", deviceToken];
}


- (void) onMethod:(NSString*)method response:(NSDictionary*)data
{
    PLog(@"On method:%@", method);
    PLog(@"data:%@", [data description]);
    NSDictionary* res = [[NSDictionary alloc] initWithDictionary:data];
    /*if ([BPushRequestMethod_Bind isEqualToString:method])
    {
        NSString *appid = [res valueForKey:BPushRequestAppIdKey];
        NSString *userid = [res valueForKey:BPushRequestUserIdKey];
        NSString *channelid = [res valueForKey:BPushRequestChannelIdKey];
        NSString *requestid = [res valueForKey:BPushRequestRequestIdKey];
        int returnCode = [[res valueForKey:BPushRequestErrorCodeKey] intValue];
        PLog(@"appid(%@), userid(%@), channelid(%@), requestid(%@)",appid, userid, channelid, requestid);
        PLog(@"BPushRequestMethod_Bind:returnCode-->%d",returnCode);
        if (returnCode == BPushErrorCode_Success)
        {
            PLog(@"百度绑定成功");
            // 在内存中备份，以便短时间内进入可以看到这些值，而不需要重新bind
//            self.appId = appid;
//            self.channelId = channelid;
//            self.userId = userid;
        }
    }
    else if ([BPushRequestMethod_Unbind isEqualToString:method])
    {
        int returnCode = [[res valueForKey:BPushRequestErrorCodeKey] intValue];
        PLog(@"BPushRequestMethod_Unbind:returnCode-->%d",returnCode);
        if (returnCode == BPushErrorCode_Success)
        {
            PLog(@"百度解除绑定SUCCESS");
        }
    } else if ([BPushRequestMethod_DelTag isEqualToString:method]) {
        int returnCode = [[res valueForKey:BPushRequestErrorCodeKey] intValue];
        
//        NSDictionary* response_params = [res valueForKey:BPushRequestResponseParamsKey];
//        NSArray* details = [response_params valueForKey:@"details"];
//        PLog(@"details-->%@",details);
        
        PLog(@"BPushRequestMethod_DelTag:returnCode-->%d",returnCode);
        
        if (returnCode == BPushErrorCode_Success) {
            
            PLog(@"百度删除TAG SUCCESS");
        }
    } else if ([BPushRequestMethod_SetTag isEqualToString:method]) {
        int returnCode = [[res valueForKey:BPushRequestErrorCodeKey] intValue];
        
//        NSDictionary* response_params = [res valueForKey:BPushRequestResponseParamsKey];
//        NSArray* details = [response_params valueForKey:@"details"];
//        PLog(@"details-->%@",details);

        
        PLog(@"BPushRequestMethod_SetTag:returnCode-->%d",returnCode);
        
        if (returnCode == BPushErrorCode_Success) {
            PLog(@"百度设置TAG SUCCESS");
        }
    }*/
}


- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    PLog(@"Receive Notify: %@", userInfo);//[userInfo JSONString]
    NSString *alert = [[userInfo objectForKey:@"aps"] objectForKey:@"alert"];
    if (application.applicationState == UIApplicationStateActive) // 在前台,弹窗提示
    {
        
        _notificationAlert = [[UIAlertView alloc] initWithTitle:@"通知"
                                                            message:alert
                                                           delegate:self
                                                  cancelButtonTitle:@"好"
                                                  otherButtonTitles:nil];
        [_notificationAlert show];
    }
    else
    {
        MessageViewController *message = [[MessageViewController alloc] init];
        [(UINavigationController *)self.menuController pushViewController:message animated:YES];
    }
    [application setApplicationIconBadgeNumber:1];
    [application setApplicationIconBadgeNumber:0];
    
    //[BPush handleNotification:userInfo];
    
}

@end
