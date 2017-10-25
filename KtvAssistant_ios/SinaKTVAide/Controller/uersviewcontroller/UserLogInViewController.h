//
//  UserLogInViewController.h
//  SinaKTVAide

//  用户 微博+QQ 登录界面

//  Created by Li Pan on 13-12-5.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "WeiboSDK.h"
#import <TencentOpenAPI/TencentOAuth.h>


@interface UserLogInViewController : BaseViewController <WeiboSDKDelegate,TencentSessionDelegate,WBHttpRequestDelegate>

@property (strong, nonatomic) TencentOAuth *tencentOAuth;
@property (strong, nonatomic) NSArray *permissions;
@property (copy, nonatomic) NSString *accessToken;
@property (copy, nonatomic) NSString *openId;
@property (strong, nonatomic) NSDate *expirationDate;

@property (copy, nonatomic) NSString *weiboToken;
@property (copy, nonatomic) NSString *weiboUserID;
@property (strong, nonatomic)  NSDictionary *weiboUserInfo;

@property (strong, nonatomic) UIButton *weiboButton;
@property (strong, nonatomic) UIButton *qqButton;


@property (strong, nonatomic) UINavigationController *naviMain;

@end
