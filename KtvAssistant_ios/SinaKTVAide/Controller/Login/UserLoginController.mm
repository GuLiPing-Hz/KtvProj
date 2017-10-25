//
//  UserLoginController.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-11-3.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "UserLoginController.h"
#import "KTVBaseConfig.h"
#import "UserRegisterController.h"
#import "KTVAppDelegate.h"
#import "SSImageCategory.h"
#import "JSONKit.h"
#import "PKtvAssistantAPI.h"
#import "UserSessionManager.h"
#import "RoomInfo.h"
#import "UIUtils.h"
#import "com_ios_NetUtil.h"
#import "NetUtilCallBack.h"
#import "AFHTTPRequestOperation.h"
#import "CommenTools.h"
#import "SVProgressHUD.h"
#import "Reachability.h"
#import "PCommonUtil.h"
#import "xingguang.h"
#import "UIImage+Color.h"

@interface UserLoginController ()


@end

@implementation UserLoginController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
    }
    return self;
}

#pragma mark - ViewDidLoad
- (void)viewDidLoad {
    [super viewDidLoad];
    
    [[NSBundle mainBundle] loadNibNamed:@"Login" owner:self options:nil];
    
    [self.mUiLoginView setFrame:self.mainContentView.bounds];
    [self.mainContentView addSubview:self.mUiLoginView];
    
    [self.mainContentView setBackgroundColor:UIColorFromRGB(0xf8f8f9)];
    //设置导航
    [self setNav];
    [self isShowThirdLogin];
    //点击空白区域隐藏
    UITapGestureRecognizer *recognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(hideKeybord)];
    recognizer.delegate = self;
    [self.view addGestureRecognizer:recognizer];
}

-(void)hideKeybord
{
    [_txtName resignFirstResponder];
    [_txtPwd resignFirstResponder];
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(recvLoginLobbyMsg:) name:KTVAssistantLoginLobbyResult object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(isShowThirdLogin) name:KTVASSISTANT_ISSHOW_NOTIFICATION object:nil];
}

-(void)isShowThirdLogin
{
    NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];
    NSString *defaultKey = KTVASSISTANT_USERDEFAULTS_ISSHOW;
    NSString *value = [defaults objectForKey:defaultKey];
    if (value != nil) {
        if ([value compare:@"1"] == NSOrderedSame)
        {
            _lview.hidden = NO;
            _rview.hidden = NO;
            _qqbtn.hidden = NO;
            _wxbtn.hidden = NO;
            _wbbtn.hidden = NO;
            _qqlabel.hidden = NO;
            _wxlabel.hidden = NO;
            _wblabel.hidden = NO;
            _otherlable.hidden = NO;
            return;
        }
        else
        {
            _lview.hidden = YES;
            _rview.hidden = YES;
            _qqbtn.hidden = YES;
            _wxbtn.hidden = YES;
            _wbbtn.hidden = YES;
            _qqlabel.hidden = YES;
            _wxlabel.hidden = YES;
            _wblabel.hidden = YES;
            _otherlable.hidden = YES;

        }
    }
}

-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

-(void)recvLoginLobbyMsg:(NSNotification *)center
{
    NSDictionary *dic =[center userInfo];
    NSInteger type = [[dic objectForKey:@"result"] integerValue];
    if (type)
    {
        [SVProgressHUD dismiss];
        [self loginMainView];
    }
    else
    {
        //[self showErrorAlert:@"登录失败"];
        //KTVAppDelegate *delegate = (KTVAppDelegate *)[UIApplication sharedApplication].delegate;
    }
}

//登录主界面
- (void)loginMainView
{
    [self.appDelegate loginMain];
}

-(void)setNav
{
    self.navView.titleLabel.text = @"登录";
    self.navView.hidden = NO;
    [self.navView.btnHistory setTitle:@"注册" forState:UIControlStateNormal];
    [self.navView.btnHistory addTarget:self action:@selector(registerAction) forControlEvents:UIControlEventTouchUpInside];
    [self.navView.btnHistory setHidden:NO];
}

-(void)registerAction
{
    [self hideKeybord];
    UserRegisterController *registerController =[[UserRegisterController alloc] init];
    [self.navigationController pushViewController:registerController animated:YES];
}

#pragma mark tabledelegate
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 2;
}
-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 44;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - 微信登录
/*! @brief 发送一个sendReq后，收到微信的回应
 *
 * 收到一个来自微信的处理结果。调用一次sendReq后会收到onResp。
 * 可能收到的处理结果有SendMessageToWXResp、SendAuthResp等。
 * @param resp具体的回应内容，是自动释放的
 */
-(void) onResp:(BaseResp*)resp
{
    if([resp isKindOfClass:[SendAuthResp class]])
    {
        SendAuthResp *temp = (SendAuthResp*)resp;
        
        if([temp.state compare:APP_NAME] == NSOrderedSame)
        {
            switch (temp.errCode) {
                case WXSuccess:    /**< 成功    */
                {
                    NSString* str_url = [PKtvAssistantAPI getWeiXinTokenUrl:kWeiXinAppID secret:kWeiXInAppKey code:temp.code];
                    //使用GCD分线程获取微信的Token
                    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                        NSURL *zoneUrl = [NSURL URLWithString:str_url];
                        NSString *zoneStr = [NSString stringWithContentsOfURL:zoneUrl encoding:NSUTF8StringEncoding error:nil];
                        NSData *data = [zoneStr dataUsingEncoding:NSUTF8StringEncoding];
                        
                        if (data) {
                            NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableContainers error:nil];
                            
                            NSString* wxopenid = [dict objectForKey:@"openid"];
                            if(wxopenid == nil)
                            {
                                [self showErrorAlert:@"微信授权发生错误"];
                                return ;
                            }
                            
                            //获取刷新的Token
                            NSString* refreshToken = [dict objectForKey:@"refresh_token"];
                            NSString* refresh_str_url = [PKtvAssistantAPI getWeiXinRefreshTokenUrl:kWeiXinAppID refresh:refreshToken];
                            NSURL* refreshUrl = [NSURL URLWithString:refresh_str_url];
                            NSString* refreshStr = [NSString stringWithContentsOfURL:refreshUrl encoding:NSUTF8StringEncoding error:nil];
                            NSData* refreshData = [refreshStr dataUsingEncoding:NSUTF8StringEncoding];
                            
                            dispatch_async(dispatch_get_main_queue(), ^{
                                if(refreshData)
                                {
                                    NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:refreshData options:NSJSONReadingMutableContainers error:nil];
                                    
                                    //获取到微信的OpenID和Token之后执行登录
                                    self.wxOpenId = [dict objectForKey:@"openid"];
                                    self.wxToken = [dict objectForKey:@"access_token"];
                                    [self sendRequest:4];//微信登陆
                                }
                            });
                        }
                    });
                    break;
                }
                case WXErrCodeCommon:   /**< 普通错误类型    */
                    [self showErrorAlert:@"微信授权发生错误!"];
                    break;
                case WXErrCodeUserCancel:   /**< 用户点击取消并返回    */
                    //[self showErrorAlert:@""];
                    break;
                case WXErrCodeSentFail:   /**< 发送失败    */
                    [self showErrorAlert:@"微信授权发送失败,请重试~"];
                    break;
                case WXErrCodeAuthDeny:   /**< 授权失败    */
                    [self showErrorAlert:@"微信授权失败"];
                    break;
                case WXErrCodeUnsupport:   /**< 微信不支持    */
                    [self showErrorAlert:@"您的微信版本太低,不支持授权~"];
                    break;
                    
                default:
                    break;
            }
        }
    }
}

-(void) onReq:(BaseReq*)req
{
    PLog(@"WX onReq");
}

-(void)weixinAuth
{
    SendAuthReq* req = [[SendAuthReq alloc] init];
    req.scope = @"snsapi_userinfo,snsapi_base";//@"snsapi_message,snsapi_userinfo";
    req.state = APP_NAME;
    
    [WXApi sendAuthReq:req viewController:self delegate:self];
}

#pragma mark - 微博登录

- (void)weiboJoin
{
    [self weiboOAuth];
}
// 微博授权
- (void)weiboOAuth
{
    /************************************/
    WBAuthorizeRequest *request = [WBAuthorizeRequest request];
    request.redirectURI = kWeiboRedirectURI;
    request.scope = @"all";
    request.userInfo = @{@"SSO_From": @"UserLoginController",
                         @"Other_Info_1": [NSNumber numberWithInt:123],
                         @"Other_Info_2": @[@"obj1", @"obj2"],
                         @"Other_Info_3": @{@"key1": @"obj1", @"key2": @"obj2"}};
    [WeiboSDK sendRequest:request];
    //[WeiboSDK enableDebugMode:YES];
    /************************************/
}


#pragma mark - 微博请求传回的响应

- (void)didReceiveWeiboRequest:(WBBaseRequest *)request
{
    PLog(@"----request-----%@",request);
}
- (void)didReceiveWeiboResponse:(WBBaseResponse *)response
{
    if ([response isKindOfClass:WBSendMessageToWeiboResponse.class])
    {
        NSString *message = [NSString stringWithFormat:@"响应状态: %d\n响应UserInfo数据: %@\n原请求UserInfo数据: %@",
                             response.statusCode, response.userInfo, response.requestUserInfo];
        PLog(@"send=%@",message);
        self.weiboUserInfo = response.userInfo;
    }
    else if ([response isKindOfClass:WBAuthorizeResponse.class])
    {
        int statusCode = response.statusCode;
        PLog(@"响应状态: %d",response.statusCode);
        if(statusCode < 0)  // 用户取消授权登陆
        {
            [self WeiboAuthorizeFailorCancel];
        }
        else
        {
            self.weiboToken = [(WBAuthorizeResponse *)response accessToken];
            self.weiboUserID = [(WBAuthorizeResponse *)response userID];
            self.expirationDate = [(WBAuthorizeResponse *)response expirationDate];  // 微博 qq 共用一个 expirationDate
            
            [self sendRequest:1];  // 微博登录
        }
    }
}

- (void)WeiboAuthorizeFailorCancel {
    PLog(@"用户取消微博登陆");
}


#pragma mark - 微博 http 请求
/**
 收到一个来自微博Http请求的响应
 
 @param response 具体的响应对象
 */
- (void)request:(WBHttpRequest *)request didReceiveResponse:(NSURLResponse *)response
{
    
}

/**
 收到一个来自微博Http请求失败的响应
 
 @param error 错误信息
 */
- (void)request:(WBHttpRequest *)request didFailWithError:(NSError *)error
{
    
}

/**
 收到一个来自微博Http请求的网络返回
 
 @param result 请求返回结果
 */
- (void)request:(WBHttpRequest *)request didFinishLoadingWithResult:(NSString *)result
{
    
}


#pragma mark - QQ 登录

- (void)qqJoin
{
    [self qqOAuth];
}

// QQ授权
- (void)qqOAuth
{
    //注册QQ
    if(self.tencentOAuth == nil)
        self.tencentOAuth = [[TencentOAuth alloc] initWithAppId:kQQAppID andDelegate:self];
    /************************************/
    _permissions = [NSArray arrayWithObjects:
                    kOPEN_PERMISSION_GET_USER_INFO,
                    kOPEN_PERMISSION_GET_SIMPLE_USER_INFO, nil];
    [self.tencentOAuth authorize:_permissions inSafari:NO];
    /************************************/
}


#pragma mark - QQ 登录 回调
/**
 * 登录成功后的回调
 */
- (void)tencentDidLogin
{
    if (_tencentOAuth.accessToken && 0 != [_tencentOAuth.accessToken length])
    {
        PLog(@"登录成功 获取accesstoken");
        // 直接获取, 建议应用在用户登录后，即调用getUserInfo接口获得该用户的头像、昵称并显示在界面上，使用户体验统一。
        self.accessToken = [_tencentOAuth accessToken];
        self.openId = [_tencentOAuth openId];
        self.expirationDate = [_tencentOAuth expirationDate];  // 微博 qq 共用一个 expirationDate
        [self sendRequest:2];  // QQ登录
    }else{
        PLog(@"QQ授权失败 获取accesstoken Fail");
    }
}
/**
 * 登录失败后的回调
 * \param cancelled 代表用户是否主动退出登录
 */
- (void)tencentDidNotLogin:(BOOL)cancelled
{
    if (cancelled)
    {
        PLog(@"用户取消登录");
    }
    else
    {
        PLog(@"登录失败");
    }
}
/**
 * 登录时网络有问题的回调
 */
- (void)tencentDidNotNetWork
{
    [self showNetwork];
}

#pragma mark - Self Login Model
- (void)sendRequest:(NSInteger)type
{
    if(![[Reachability reachabilityForInternetConnection] isReachable])
    {
        [self showNetwork];
        return;
    }
    
    [SVProgressHUD showWithStatus:@"获取个人信息" maskType:SVProgressHUDMaskTypeClear];
    NSString *roomId = [UserSessionManager getInstance].currentRoomInfo.roomId;
    NSString *roomPassword = [UserSessionManager getInstance].currentRoomInfo.roomPassword;
    
    NSString *_expirationDateStr = nil;
    if(type != 3)
        _expirationDateStr= [UIUtils stringFromFomate:self.expirationDate formate:@"yyyy-MM-dd HH:mm:ss"];
    
    NSMutableURLRequest *request;
    NSString *encodeUrl;
    switch (type)
    {
            // 1> 微博 、2> QQ，3>自己注册，4>微信
        case 1://微博
            encodeUrl = [PKtvAssistantAPI getOpenLoginUrl:_weiboUserID roomId:roomId roomPwd:roomPassword token:_weiboToken expirationdate:_expirationDateStr accountType:type];
            break;
            
        case 2://QQ
            encodeUrl = [PKtvAssistantAPI getOpenLoginUrl:_openId roomId:roomId roomPwd:roomPassword token:_accessToken expirationdate:_expirationDateStr accountType:type];
            break;
        case 3://自己注册的用户
            break;//Android里面已经定义
        case 4://微信
            encodeUrl = [PKtvAssistantAPI getOpenLoginUrl:self.wxOpenId roomId:roomId roomPwd:roomPassword token:self.wxToken expirationdate:@"" accountType:type];
            break;
        default:
            break;
    }
    NSURL *url = [NSURL URLWithString:encodeUrl];
    request = [NSMutableURLRequest requestWithURL:url];
    
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
             [UserSessionManager getInstance].currentUser = user;
             [[UserSessionManager getInstance] writeUserInfoToFile];
             //[user log];
             [NetUtilCallBack getInstance]->m_listLobbyUserInfo.clear();
             [NetUtilCallBack getInstance]->m_listRoomUserInfo.clear();
             [NetUtilCallBack getInstance]->m_listPrincessInfo.clear();
             [NetUtilCallBack getInstance]->m_reconnectTimes = 1;
             [[NetUtilCallBack getInstance] doCurrentWork:TransactionLobbyLogin];
         }
         else
         {
             
             NSString *msg = [results objectForKey:@"msg"];
             NSString *errorCode = [results objectForKey:@"errorcode"];
             PLog(@"doOpenLogin failed...%@(%@)", msg, errorCode);
             if (msg.length == 0)
             {
                 [SVProgressHUD dismissWithError:@"登陆失败,请重试" afterDelay:1.0];
             }
             else
             {
                 [SVProgressHUD dismiss];
                 [CommenTools showAlertViewWithErrorMessage:msg];
             }
         }
     }
     failure:^(AFHTTPRequestOperation *operation, NSError *error)
     {
         PLog(@"doLogin failed...");
         [SVProgressHUD dismiss];
         [CommenTools showNetworkDisconnectedAlertView];
     }];
    
    [operation start];
}


-(void)Login:(NSString *)kid pwd:(NSString *)pwd
{
    if (![[Reachability reachabilityForInternetConnection] isReachable])
    {
        [self showNetwork];
        return;
    }
    [SVProgressHUD showWithStatus:@"正在登录" maskType:SVProgressHUDMaskTypeClear];
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
                [self showErrorAlert:@"账号或密码错误"];
            else
                [self showErrorAlert:msg];
        }

    }
    failure:^(AFHTTPRequestOperation *operation, NSError *error)
    {
        [SVProgressHUD dismiss];
        [CommenTools showNetworkDisconnectedAlertView];
    }];
    
    
    [operation start];
}

- (IBAction)onQQLogin:(id)sender {
    [self qqJoin];
}

- (IBAction)onWXLogin:(id)sender {
    PLog(@"wx login");
    [self weixinAuth];
}

- (IBAction)onWBLogin:(id)sender {
    [self weiboJoin];
}

- (IBAction)onLogin:(id)sender {
    
    [self hideKeybord];
    NSString *name = _txtName.text;
    NSString *pwd = _txtPwd.text;
    if (name.length <= 0 || pwd.length <= 0)
    {
        [self showErrorAlert:@"请输入用户名"];
        return;
    }
    if(pwd.length == 0)
    {
        [self showErrorAlert:@"请输入密码"];
        return ;
    }
    [self Login:name pwd:pwd];
}
@end
