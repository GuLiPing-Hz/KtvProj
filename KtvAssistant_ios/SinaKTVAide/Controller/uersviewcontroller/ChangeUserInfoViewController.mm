//
//  ChangeUserInfoViewController.m
//  SinaKTVAide

//  修改昵称 界面

//  Created by Li Pan on 13-12-18.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "ChangeUserInfoViewController.h"
#import "SSImageCategory.h"
#import "PKtvAssistantAPI.h"
#import "AssociatedAccount.h"
#import "DDMenuController.h"
#import "KTVAppDelegate.h"
#import "iToast.h"
#import "CommenTools.h"
#import <AudioToolbox/AudioToolbox.h>
#import "RegexKitLite.h"
#import "AudioToolbox/AudioServices.h"
@interface ChangeUserInfoViewController ()

@end

@implementation ChangeUserInfoViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
       
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
	self.navView.titleLabel.text = @"修改昵称";
    self.view.backgroundColor = UIColorFromRGB(0xe1e2e6);
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.navView.rightButton.hidden = NO;
    UIImage *rightImageNormal = [UIImage imageWithName:@"btn_ok" type:@"png"];
    UIImage *rightImage = [UIImage imageWithName:@"btn_ok_c" type:@"png"];
    [self.navView.rightButton setBackgroundImage:rightImageNormal forState:UIControlStateNormal];
    [self.navView.rightButton setBackgroundImage:rightImage forState:UIControlStateHighlighted];
    [self.navView.rightButton addTarget:self action:@selector(rightAction:) forControlEvents:UIControlEventTouchUpInside];
    
    
    // 昵称和昵称背景图共View
    _nickView = [[UIView alloc] initWithFrame:CGRectMake(20, 20, _MainScreen_Width-40, 50)];
    _nickView.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview:_nickView];
    
    // 文本框背景
    UIImageView *bgText = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"kuang_xiugainicheng.png"]];
    bgText.frame = _nickView.bounds;
    bgText.backgroundColor = [UIColor clearColor];
    [_nickView addSubview:bgText];
    
    // 昵称文本框
    _nickName = [[UITextField alloc] initWithFrame:CGRectMake(10, 0, _nickView.frame.size.width-20, _nickView.frame.size.height)];
    _nickName.textColor = UIColorFromRGB(0x303030);
    _nickName.delegate = self;
    _nickName.borderStyle = UITextBorderStyleNone;
    _nickName.backgroundColor = [UIColor clearColor];
    _nickName.text = [UserSessionManager getInstance].currentUser.name;
    _nickName.font = [UIFont systemFontOfSize:16.0f];
    _nickName.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    _nickName.clearButtonMode = UITextFieldViewModeWhileEditing;
    [_nickName becomeFirstResponder];
    [_nickView addSubview:_nickName];
    
    // 昵称警告提示
    _nickNotice = [[UILabel alloc] initWithFrame:CGRectZero];
    _nickNotice.textColor = [UIColor redColor];
    _nickNotice.backgroundColor = [UIColor clearColor];
    _nickNotice.text = @"";
    _nickNotice.font = [UIFont systemFontOfSize:13.0f];
    [self setLabelSizeHeight];
    [_nickNotice setNumberOfLines:0];
    [self.mainContentView addSubview:_nickNotice];
}

// 动态修改UIlabel高度
- (void)setLabelSizeHeight
{
    UIFont *font = [UIFont systemFontOfSize:13.0f];
    CGSize size = CGSizeMake(_MainScreen_Width,MAXFLOAT);
    CGSize labelsize = [_nickNotice.text sizeWithFont:font constrainedToSize:size lineBreakMode:NSLineBreakByWordWrapping];
    _nickNotice.frame = CGRectMake(20, 80, _MainScreen_Width-40, labelsize.height);
}

// 保存昵称
- (void)rightAction:(UIButton *)button
{
    
    [self changeNickNameAPI];  // 修改昵称 请求
}

#pragma mark - 修改昵称 请求

- (void)changeNickNameAPI
{
    if (_nickName.text.length == 0)
    {
        [self animateIncorrectMessage:_nickView];
        _nickNotice.text = @"昵称不能为空";
        [self setLabelSizeHeight];
        //AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
    }
    else
    {
        [SVProgressHUD showWithStatus:@"保存中..." maskType:SVProgressHUDMaskTypeClear];
        
        [_nickName  resignFirstResponder]; // 取消键盘
        
        /*******************************************/
        // 判断昵称
        NSRange range = [_nickName.text rangeOfString:@" "];
        if (range.location != NSNotFound)
        {
            [SVProgressHUD dismiss];
            [self animateIncorrectMessage:_nickView];
            _nickNotice.text = @"不能包含空格，请重新命名";
            [self setLabelSizeHeight];
            return;
        }
        
        if ([[[_nickName text] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] length] < 2 ||[[[_nickName text] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] length] >10)
        {
            [SVProgressHUD dismiss];
            [self animateIncorrectMessage:_nickView];
            _nickNotice.text = @"长度有误，请输入2-10位字符";
            [self setLabelSizeHeight];
            return;
        }
        else
        {
            int countWords = [self nickNameCountWord:_nickName.text];

            if (countWords > 5) // 不能超过5个汉字或者10字符
            {
                [SVProgressHUD dismiss];
                [self animateIncorrectMessage:_nickView];
                _nickNotice.text = @"长度有误，请输入2-10位字符";
                [self setLabelSizeHeight];
                return;
            }
        }
        
        if ([self nickNameRegex] == NO)
        {
            [SVProgressHUD dismiss];
            [self animateIncorrectMessage:_nickView];
            _nickNotice.text = @"昵称不可用";
            [self setLabelSizeHeight];
            return;
        }
        
        //昵称必须由数字、26个英文字母或者下划线组成的字符串，不符合的返回
        if (![_nickName.text isMatchedByRegex:@"^\\w{2,10}$"])
        {
            [SVProgressHUD dismiss];
            [self animateIncorrectMessage:_nickView];
            _nickNotice.text = @"仅支持中英文，数字和\"_\""; //   \反斜杠转义字符
            [self setLabelSizeHeight];
            return;
        }
        /*******************************************/
        // 无网络
        if(![[Reachability reachabilityForInternetConnection] isReachable])
        {
            [SVProgressHUD dismiss];
            [self showNetwork];
            return;
        }
        
        long int userID = [UserSessionManager getInstance].currentUser.uid;
        int gender = [UserSessionManager getInstance].currentUser.gender;
        AssociatedAccount *account = (AssociatedAccount *)[[UserSessionManager getInstance].currentUser.openAccounts objectAtIndex:0];
        NSString *openID = account.openID;
        NSString *encodeUrl = [PKtvAssistantAPI getUserModifyNickNameUrl:userID openID:openID nickName:_nickName.text gender:gender];
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
                     [SVProgressHUD dismissWithSuccess:@"设置成功" afterDelay:1.0];
                     [UserSessionManager getInstance].currentUser.name = _nickName.text;
                     [[UserSessionManager getInstance] writeUserInfoToFile];
                     [self.navigationController popViewControllerAnimated:YES];
                 }
                 else
                 {
                     [SVProgressHUD dismiss];
                     [self animateIncorrectMessage:_nickView];
                     NSString *msg = [results objectForKey:@"msg"];
                     NSString *errorCode = [results objectForKey:@"errorcode"];
                     if(msg && msg.length > 0)
                     {
                         _nickNotice.text = msg;
                     }
                     else
                     {
                         _nickNotice.text = @"昵称不可用";
                     }
                     PLog(@"doModifyNickName failed...errorcode:%@(%@)", errorCode, msg);
                 }
             }
             else
             {
                 [SVProgressHUD dismiss];
                 [self animateIncorrectMessage:_nickView];
                 NSString *msg = [results objectForKey:@"msg"];
                 NSString *errorCode = [results objectForKey:@"errorcode"];
                 if(msg && msg.length > 0)
                 {
                     _nickNotice.text = msg;
                 }
                 else
                 {
                     _nickNotice.text = @"昵称不可用";
                 }
                 PLog(@"doModifyNickName failed...errorcode:%@(%@)", errorCode, msg);
             }
         }
         failure:^(AFHTTPRequestOperation *operation, NSError *error)
         {
             [SVProgressHUD dismiss];
             PLog(@"--failed...");
             [CommenTools showNetworkDisconnectedAlertView];
         }];
        [operation start];
    }
}

// 返回按钮
-(void)doBack:(id)sender
{
    [_nickName  resignFirstResponder];
    [self.navigationController popViewControllerAnimated:YES];
}

/**
 * 文本框抖动动画
 */
- (void)animateIncorrectMessage:(UIView *)view
{
    CGAffineTransform moveRight = CGAffineTransformTranslate(CGAffineTransformIdentity, 8, 0);
    CGAffineTransform moveLeft = CGAffineTransformTranslate(CGAffineTransformIdentity, -8, 0);
    
    CGAffineTransform resetTransform = CGAffineTransformTranslate(CGAffineTransformIdentity, 0, 0);
    
    [UIView animateWithDuration:0.1 animations:^{
        
        view.transform = moveLeft;
        
    } completion:^(BOOL finished) {
        
        [UIView animateWithDuration:0.1 animations:^{
            
            view.transform = moveRight;
            
        } completion:^(BOOL finished) {
            
            [UIView animateWithDuration:0.1 animations:^{
                
                view.transform = moveLeft;
                
            } completion:^(BOOL finished) {
                
                [UIView animateWithDuration:0.1 animations:^{
                    
                    view.transform = moveRight;
                    
                } completion:^(BOOL finished) {
                    
                    [UIView animateWithDuration:0.1 animations:^{
                        
                        view.transform = resetTransform;
                        
                    }];
                    
                }];
                
            }];
            
        }];
        
    }];
}

/**
 * 匹配不合法的关键字
 */
- (BOOL)nickNameRegex
{
    NSMutableArray *arrayNick = [NSMutableArray arrayWithObjects:@"我操",@"我日你妈",@"mm",@"jj",@"bb",@"脱",@"拖",@"托",@"操你妈",@"你好难看呀",@"我想操你",@"我想干你",@"你中奖了",@"jb",@"sb",@"几吧",@"煞笔",@"www.",@"com.",@"@",@"http:/",@"baobei",@"admin",@"&",@"%",@"9158",@"就约我吧",@"我草",@"叼",@"卧槽",@"色情",@"情色",@"a片",@"毛片",@"女优",@"av",@"妓女",@"鸭子", nil];
    
    for (NSString *strNick in arrayNick)
    {
        if ([strNick isEqualToString:_nickName.text])
        {
            return NO;
        }
    }
    
    NSString *regex = @"系.*统|客.*服|公.*告|中.*奖|消.*息|官.*方|客.*服|大.*江|运.*营|技.*术|巡.*管|代.*理|[9９].*[1１].*[5５].*[8８]";
    
    NSArray *array = [_nickName.text componentsMatchedByRegex:regex];
    for (NSString *string in array)
    {
        return NO;
    }
    
    return YES;
}

/**
 * 统计昵称字数
 */
- (int)nickNameCountWord:(NSString*)s
{
    int i,n=[s length],l=0,a=0,b=0;
    unichar c;
    for(i=0;i<n;i++){
        c=[s characterAtIndex:i];
        if(isblank(c)){
            b++;
        }else if(isascii(c)){
            a++;
        }else{
            l++;
        }
    }
    if(a==0 && l==0) return 0;
    return l+(int)ceilf((float)(a+b)/2.0);
}

// 同上一个方法一样
//-(NSUInteger) unicodeLengthOfString: (NSString *) text {
//    NSUInteger asciiLength = 0;
//    
//    for (NSUInteger i = 0; i < text.length; i++) {
//        
//        
//        unichar uc = [text characterAtIndex: i];
//        
//        asciiLength += isascii(uc) ? 1 : 2;
//    }
//    
//    NSUInteger unicodeLength = asciiLength / 2;
//    
//    if(asciiLength % 2) {
//        unicodeLength++;
//    }
//    
//    return unicodeLength;
//}

#pragma mark - UITextField delegate

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
    //_nickNotice.text = @"";
    
    return YES;
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    //_nickNotice.text = [NSString stringWithFormat:@"你还可以输入%d个字符",10-[self nickNameCountWord:_nickName.text]];
    return YES;
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc
{
    //AudioServicesDisposeSystemSoundID(kSystemSoundID_Vibrate);
}

@end
