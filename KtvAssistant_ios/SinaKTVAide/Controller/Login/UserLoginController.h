//
//  UserLoginController.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-11-3.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "WeiboSDK.h"
#import "WXApi.h"
#import <TencentOpenAPI/TencentOAuth.h>

@interface UserLoginController : BaseViewController <WeiboSDKDelegate,TencentSessionDelegate,WBHttpRequestDelegate,UITextFieldDelegate,UIGestureRecognizerDelegate,WXApiDelegate>

//微博用户信息
@property (copy,nonatomic) NSString             *weiboToken;
@property (copy,nonatomic) NSString             *weiboUserID;
@property (strong,nonatomic) NSDictionary       *weiboUserInfo;

//QQ用户信息
@property (strong,nonatomic) TencentOAuth       *tencentOAuth;
@property (strong,nonatomic) NSArray            *permissions;
@property (copy, nonatomic) NSString            *accessToken;
@property (copy, nonatomic) NSString            *openId;

//微信用户信息
@property(copy,nonatomic) NSString* wxOpenId;
@property(copy,nonatomic) NSString* wxToken;

//所有用户都使用同一个超时时间
@property (strong, nonatomic) NSDate            *expirationDate;

@property (strong, nonatomic) IBOutlet UIView *mUiLoginView;
@property(nonatomic,weak) IBOutlet UITextField         *txtName;
@property(nonatomic,weak) IBOutlet UITextField         *txtPwd;

//第三方登录
@property (strong, nonatomic) IBOutlet UIView *lview;
@property (strong, nonatomic) IBOutlet UIView *rview;
@property (strong, nonatomic) IBOutlet UIButton *qqbtn;
@property (strong, nonatomic) IBOutlet UIButton *wxbtn;
@property (strong, nonatomic) IBOutlet UIButton *wbbtn;
@property (strong, nonatomic) IBOutlet UILabel *qqlabel;
@property (strong, nonatomic) IBOutlet UILabel *wxlabel;
@property (strong, nonatomic) IBOutlet UILabel *wblabel;
@property (strong, nonatomic) IBOutlet UILabel *otherlable;


- (IBAction)onQQLogin:(id)sender;
- (IBAction)onWXLogin:(id)sender;
- (IBAction)onWBLogin:(id)sender;
- (IBAction)onLogin:(id)sender;

@end
