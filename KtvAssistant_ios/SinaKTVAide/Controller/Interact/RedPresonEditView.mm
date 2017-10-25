//
//  RedPresonEditView.m
//  SinaKTVAide
//
//  Created by cxp on 15/5/6.
//  Copyright (c) 2015年 Zhang Chen. All rights reserved.
//

#import "RedPresonEditView.h"
#import "NetUtilCallBack.h"
#import "com_ios_NetUtil.h"
#import "InteractController.h"
#import "KTVAppDelegate.h"
#import "DDMenuController.h"

@interface RedPresonEditView()
{
    UITextField  *totalmoney;
    UITextField  *mark;
    UIButton     *sendBtn;
    UILabel     *lblgain;
}
@end

@implementation RedPresonEditView
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        _useridx = 0;
    }
    return self;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:NO];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showSendMsgResult:) name:@"ktvassistantSendMessageResult" object:nil];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:YES];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

-(void)showSendMsgResult:(NSNotification *)notification
{
    NSDictionary *userinfo = [notification userInfo];
    NSString *type = [userinfo objectForKey:@"type"];
    if ( [type isEqualToString:@"8"]) {
        NSInteger result = [[userinfo objectForKey:@"result"] integerValue];
        switch (result) {
            case 0:
                [self showSuccessAlert:@"发送成功"];
                break;
            case 1:
                [self showErrorAlert:@"金币不足了哦"];
                break;
            case 2:
                [self showErrorAlert:@"对方不在线了哦"];
                break;
            case 3:
                [self showErrorAlert:@"请先登录包厢哦"];
                break;
            default:
                [self showErrorAlert:@"网络不给力，发送失败"];
                break;
        }
    }
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self.navView initOnlyBackButton:@"个人红包"];
    //self.navView.titleLabel.text = @"个人红包";
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.navView.backgroundColor = UIColorFromRGB(0xd64551);
    self.mainContentView.backgroundColor = UIColorFromRGB(0xf8f8f9);
    
    [self createTotalmoney];
    [self createMark];
    [self createSendBtn];
    [self createTips];
    
    UITapGestureRecognizer *keyboard_tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(ViewTouchDown)];
    keyboard_tapGesture.cancelsTouchesInView = NO;
    [self.mainContentView addGestureRecognizer:keyboard_tapGesture];
    
}

- (void)createTotalmoney
{
    UIView *moneyView = [[UIView alloc] initWithFrame:CGRectMake(15,20,_MainScreen_Width-30,45)];
    moneyView.backgroundColor = UIColorFromRGB(0xffffff);
    totalmoney = [[UITextField alloc] initWithFrame:CGRectMake(165,13,100,20)];
    totalmoney.placeholder = @"请输入整数值";
    totalmoney.keyboardType = UIKeyboardTypeNumberPad;
    totalmoney.font = [UIFont systemFontOfSize:15.0f];
    UIColor *color = UIColorFromRGB(0xc2c2c2);
    [totalmoney setValue:[UIFont systemFontOfSize:15.0f] forKeyPath:@"_placeholderLabel.font"];
    [totalmoney setValue:color forKeyPath:@"_placeholderLabel.textColor"];
    [totalmoney addTarget:self action:@selector(textFieldDidChange:) forControlEvents:UIControlEventEditingChanged];
    UILabel *lblmoney = [[UILabel alloc]initWithFrame:CGRectMake(10, 15, 150, 16)];
    lblmoney.font = [UIFont systemFontOfSize:15.0f];
    lblmoney.text = @"金币总值";
    lblmoney.textColor = UIColorFromRGB(0x333333);
    
    [moneyView addSubview:lblmoney];
    [moneyView addSubview:totalmoney];
    [self.mainContentView addSubview:moneyView];
    
    UILabel *lblpay = [[UILabel alloc]initWithFrame:CGRectMake(15, 155, 65, 15)];
    lblpay.font = [UIFont systemFontOfSize:13.0f];
    lblpay.text = @"吞掉金币：";
    lblpay.textColor = UIColorFromRGB(0xc2c2c2);
    [self.mainContentView addSubview:lblpay];
    
    lblgain = [[UILabel alloc]initWithFrame:CGRectMake(80, 155, 100, 15)];
    lblgain.font = [UIFont systemFontOfSize:13.0f];
    lblgain.text = @"0";
    lblgain.textColor = UIColorFromRGB(0xc2c2c2);
    [self.mainContentView addSubview:lblgain];
}

- (void)createMark{
    mark = [[UITextField alloc] initWithFrame:CGRectMake(15,95,_MainScreen_Width-30,50)];
    mark.backgroundColor = UIColorFromRGB(0xffffff);
    mark.placeholder = @"今儿个心情好，赏你个红包！";
    mark.font = [UIFont systemFontOfSize:15.0f];
    UIColor *color = UIColorFromRGB(0xc2c2c2);
    [mark setValue:[UIFont systemFontOfSize:15.0f] forKeyPath:@"_placeholderLabel.font"];
    [mark setValue:color forKeyPath:@"_placeholderLabel.textColor"];
    [self.mainContentView addSubview:mark];
}

- (void)createSendBtn{
    sendBtn = [[UIButton alloc]initWithFrame:CGRectMake(20, 198, _MainScreen_Width-40, 43)];
    [sendBtn setBackgroundImage:[UIImage imageNamed:@"btn_send_red_1"] forState:UIControlStateNormal];
    [sendBtn setBackgroundImage:[UIImage imageNamed:@"btn_send_red_1"] forState:UIControlStateHighlighted];
    [sendBtn setTitle:@"发红包" forState:UIControlStateNormal];
    sendBtn.titleLabel.font = [UIFont systemFontOfSize:18.0f];
    sendBtn.titleLabel.textAlignment = UITextAlignmentCenter;
    sendBtn.titleLabel.textColor = UIColorFromRGB(0xffffff);
    sendBtn.enabled = NO;
    [sendBtn addTarget:self action:@selector(sendBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [self.mainContentView addSubview:sendBtn];
    
}

- (void)sendBtnClick:(id)sender{
    if ([UserSessionManager getInstance].currentUser.gold < [totalmoney.text intValue]) {
        [self showErrorAlert:@"金币不足了哦"];
        return;
    }
    
    if (mark.text.length > 20) {
        [self showErrorAlert:@"红包备注不能超过20个字符哦"];
        return;
    }
    
    if (![NetUtilCallBack getInstance]->m_bConnectedLobby) {
        [self showErrorAlert:@"网络不给力哦"];
        return;
    }
    
    char content[1024]= {0};
    if (mark.text.length == 0) {
        strcpy(content, "今儿个心情好，赏你个红包！"); 
    }
    else
        strcpy(content,mark.text.UTF8String);
    MFFilter(content, strlen(content));
    
    //utf8 -> gbk
    char gbkcontent[1024] = {0};
    int len = sizeof(gbkcontent);
    if(0 == Utf8ToGbk(content, gbkcontent, len))
    {
        return ;
    }
    
    if (0 == RequestSendHB(0, 1, [totalmoney.text intValue], gbkcontent, _useridx)) {
        PLog(@"RequestSendHB OK");
    }
    else
        PLog(@"RequestSendHB failed");

}

- (void)createTips{
    UILabel *tip1 = [[UILabel alloc] initWithFrame:CGRectMake(20, 369, 40, 12)];
    tip1.text = @"提示：";
    tip1.textColor = UIColorFromRGB(0xc2c2c2);
    tip1.font = [UIFont systemFontOfSize:11.0f];
    
    UILabel *tip2 = [[UILabel alloc] initWithFrame:CGRectMake(20, 386, 240, 12)];
    tip2.text = @"1.单次发送红包超过1000金币会在全网广播！";
    tip2.textColor = UIColorFromRGB(0xc2c2c2);
    tip2.font = [UIFont systemFontOfSize:11.0f];
    
    [self.mainContentView addSubview:tip1];
    [self.mainContentView addSubview:tip2];
}

- (void)ViewTouchDown
{
    [totalmoney resignFirstResponder];
    [mark resignFirstResponder];
}

-(void)textFieldDidChange:(id)sender
{
    totalmoney.textColor = UIColorFromRGB(0x333333);
    NSInteger total = [totalmoney.text intValue];
    NSInteger gain = total * 0.1;
    lblgain.text = [NSString stringWithFormat:@"%d",gain];
    
    if ([totalmoney.text intValue] > 0 && [totalmoney.text intValue] <= 999999999) {
        sendBtn.enabled = YES;
        [sendBtn setBackgroundImage:[UIImage imageNamed:@"btn_send_red_0"] forState:UIControlStateNormal];
    }
    else
    {
        totalmoney.textColor = [UIColor redColor];
        sendBtn.enabled = NO;
        [sendBtn setBackgroundImage:[UIImage imageNamed:@"btn_send_red_1"] forState:UIControlStateNormal];
    }
}

- (void)doBack:(id)sender
{
    [totalmoney resignFirstResponder];
    [mark resignFirstResponder];
    for (UIViewController *controller in self.navigationController.viewControllers) {
        if ([controller isKindOfClass:[InteractController class]]) {
            [self.navigationController popToViewController:controller animated:YES];
        }
    }
}

@end
