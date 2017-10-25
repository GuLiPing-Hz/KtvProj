//
//  KTVInputSecretController.m
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-10-29.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "KTVInputSecretController.h"
#import "PKtvAssistantAPI.h"
#import "BaseNavigationViewController.h"
#import "DDMenuController.h"
#import "KTVAppDelegate.h"
//#import "BPush.h"
#import "com_ios_NetUtil.h"
#import "KTVBaseConfig.h"
#import "UserSessionManager.h"
#import "AFHTTPRequestOperation.h"
#import "PCommonUtil.h"
#import "SongInfo.h"
#import "Reachability.h"
#import "xingguang.h"
#import "SVProgressHUD.h"
#import "CommenTools.h"
#import "NetUtilCallBack.h"

@interface KTVInputSecretController ()

@end

@implementation KTVInputSecretController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
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
    [self.field resignFirstResponder];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navView.titleLabel.text = @"填写包厢密码";
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    
    
    self.mainContentView.backgroundColor = UIColorFromRGB(0xe6e7ee);
    UILabel *tipsLable = [[UILabel alloc] initWithFrame:CGRectMake(30, 20, _MainScreen_Width-60, 36)];
    tipsLable.backgroundColor = [UIColor clearColor];
    tipsLable.font = [UIFont systemFontOfSize:15.0];
    tipsLable.textColor = UIColorFromRGB(0x1a1a1a);
    tipsLable.numberOfLines = 0;
    tipsLable.text = @"请填写收款单上的包厢密码,或在包厢台上也可以找到密码喔!";
    [self.mainContentView addSubview:tipsLable];
    
    UIImageView *textImage = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"bth_input.png"]];
    textImage.frame = CGRectMake(30, 75, _MainScreen_Width-60, 44);
    [self.mainContentView addSubview:textImage];
    
    
    
    self.field = [[UITextField alloc] initWithFrame:CGRectMake(44, 75, _MainScreen_Width-70, 44)];
    if (IOS7_OR_LATER)
    {
        [[UITextField appearance] setTintColor:UIColorFromRGB(0xd23675)];
        self.field.textColor = UIColorFromRGB(0xd23675);
        self.field.keyboardAppearance = UIKeyboardAppearanceLight;
    }
    else
    {
        self.field.textColor = [UIColor blueColor];
    }
    //self.field.secureTextEntry = YES;
    self.field.borderStyle = UITextBorderStyleNone;
    self.field.clearButtonMode = UITextFieldViewModeWhileEditing;
    self.field.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    self.field.placeholder = @"请输入包厢密码";
    self.field.keyboardType = UIKeyboardTypeASCIICapable;
    self.field.delegate = self;
    [self.field becomeFirstResponder];
    [self.mainContentView addSubview:self.field];
    
    UIButton *loginButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [loginButton setFrame:CGRectMake(30,148,_MainScreen_Width-60,44)];
    loginButton.titleLabel.font = [UIFont systemFontOfSize:17.0];
    [loginButton setTag:1002];
    [loginButton setTitle:@"进入包厢" forState:UIControlStateNormal];
    [loginButton setBackgroundImage:[UIImage imageNamed:@"bth_6.png"] forState:UIControlStateNormal];
    [loginButton setBackgroundImage:[UIImage imageNamed:@"bth_6_c.png"] forState:UIControlStateHighlighted];
    [loginButton addTarget:self action:@selector(loginAction:) forControlEvents:UIControlEventTouchUpInside];
    [self.mainContentView  addSubview:loginButton];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(textFiledTextChanged:) name:UITextFieldTextDidChangeNotification object:_field];
}

#pragma mark - 获取已点歌列表

- (void)loadAlreadyOrderedSongs{
    if ([UserSessionManager isRoomAlreadyLogin]) {
        //NSLog(@"————————————————————>成功");
        NSString * urlStr = [[[PKtvAssistantAPI alloc]init] getGetOrderedSongListUrl:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword];
        
        NSURL *url = [NSURL URLWithString:urlStr];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
        
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            
            
            PLog(@"doGetOrderedSongList complete...");
            //NSData *response = [resquest responseData];
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
            
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                
                
                NSDictionary * result = [results objectForKey:@"result"];
                if (result == nil) {
                    //[self getAlreadyOrderedSongsNumber];
                    
                    return;
                }
                
                NSArray *dataList = [result objectForKey:@"Songlist"];
                for (NSDictionary * dic in dataList) {
                    
                    SongInfo * info = [SongInfo initWithDictionary:dic];
                }
                
                //[self getAlreadyOrderedSongsNumber];
            } else {
                NSString *msg = [results objectForKey:@"msg"];
                PLog(@"doGetOrderedSongList failed...%@", msg);
            }
            
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            PLog(@"doGetOrderedSongList failed...");
        }];
        [operation start];
    }else{
        //NSLog(@"——————————————————>失败");
        return;
    }
    
    
}


#pragma mark - 进入包厢

-(void)loginAction:(id)sender
{
    if (loginKeyWord.length == 0)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"温馨提示" message:@"请您输入包厢密码" delegate:self cancelButtonTitle:@"好" otherButtonTitles:nil, nil];
        [alert show];
    }
    else
    {
        [_field resignFirstResponder];
        
        if(![[Reachability reachabilityForInternetConnection] isReachable])
        {
            [self showNetwork];
            return;
        }
        
        [SVProgressHUD showWithStatus:@"进入房间中,请稍后" maskType:SVProgressHUDMaskTypeClear];
        //NSString *encodeUrl = [PKtvAssistantAPI getRoomLoginUrl:userID roomID:roomId password:_field.text];
        
        User * user = [UserSessionManager getInstance].currentUser;
        long int userID = user.uid;
        NSString *encodeUrl =  [PKtvAssistantAPI getRoomLoginUrlEx:userID pwd:user.pwd password:_field.text];
        
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
                 PLog(@"正在连接房间...");
                 NSDictionary *result = [results objectForKey:@"result"];
                 NSInteger gold = [[result objectForKey:@"gold"] integerValue];
                 if ([UserSessionManager getInstance].currentUser.gold != gold && gold > 0) {
                     [[UserSessionManager getInstance] updateCurrentUserGold:gold];
                 }
                 RoomInfo *roominfo = [RoomInfo initWithDictionary:result];
                 roominfo.roomPassword = loginKeyWord;
                 [roominfo Log];
                 
                 [UserSessionManager getInstance].currentRoomInfo = roominfo;
                 [UserSessionManager getInstance].isLoggedIn = YES;
                 NSString * tag = [roominfo.roomPassword lowercaseString];
                 //PLog(@"TAG-->>%@",tag);
                 //[BPush setTag:tag withCompleteHandler:nil];
                 [SVProgressHUD dismiss];
                 //caoxinping 进入房间
                 
                 NetUtilCallBack* pNetUtil = [NetUtilCallBack getInstance];
                 [pNetUtil setRoomId:[roominfo.xgRoomID intValue]];
                 [pNetUtil doCurrentWork:TransactionRoomLogin listener:^(BOOL success) {
                     if(success)
                     {
                         [self.navigationController popViewControllerAnimated:YES]; // 输入完密码退回 首页
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
             PLog(@"doLogin failed...");
             [SVProgressHUD dismiss];
             [CommenTools showNetworkDisconnectedAlertView];
         }];
        
        [operation start];
    }
}

-(void)textFiledTextChanged:(NSNotification *)notification
{
    UITextField *field = (UITextField *)[notification object];
    loginKeyWord = field.text;
}

#pragma UITextFieldDelegate methods

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    loginKeyWord = textField.text;
    if (loginKeyWord.length > 0)
    {
        [self loginAction:Nil];
    }
    else
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"温馨提示" message:@"请您输入包厢密码" delegate:self cancelButtonTitle:@"好" otherButtonTitles:nil, nil];
        [alert show];
    }
    return YES;
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    if ([textField isEqual:_field])
    {
        if (range.location >= 6)
        {
            return  NO;
        }
        else
        {
            return  YES;
        }
    }
    return YES;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
