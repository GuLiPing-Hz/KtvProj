//
//  BroadEditView.m
//  SinaKTVAide
//
//  Created by cxp on 14-7-28.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//


#define KTVAssistantMSG @"ktvassistantSendMessageResult"

#import "BroadEditView.h"
#import "KTVAppDelegate.h"
#import "NetUtilCallBack.h"
#import "com_ios_NetUtil.h"
#import "KTVBaseConfig.h"
#import "xingguang.h"
#import "RechargeController.h"
#import "DDMenuController.h"
#import "iToast.h"

@interface BroadEditView()
{
    UILabel     *uilabel;
    UITextView  *broadtext;
    int linenum;
}
@end

@implementation BroadEditView

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
        linenum = 0;
    }
    return self;
}
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:NO];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showSendMsgResult:) name:KTVAssistantMSG object:nil];
}

-(void)showSendMsgResult:(NSNotification *)notification
{
    NSDictionary *userinfo = [notification userInfo];
    NSString *type = [userinfo objectForKey:@"type"];
    if ( [type isEqualToString:@"2"]) {
        NSInteger result = [[userinfo objectForKey:@"result"] integerValue];
        if (result == 0) {
            [self showSuccessAlert:@"发送成功"];
        }
        else
            [self showErrorAlert:@"发送失败"];
    }
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:YES];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navView.titleLabel.text = @"发送喇叭";
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.mainContentView.backgroundColor = UIColorFromRGB(0xf8f8f9);
    
    UILabel *lblbroad = [[UILabel alloc]initWithFrame:CGRectMake(20, 20, 150, 16)];
    lblbroad.font = [UIFont systemFontOfSize:14.0f];
    lblbroad.text = @"喇叭内容：";
    lblbroad.textColor = UIColorFromRGB(0x666666);
    [self.mainContentView addSubview:lblbroad];
    [self createBroad];
    [self createBtn];
    [self createLabel];
    
    UITapGestureRecognizer *keyboard_tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(ViewTouchDown)];
    keyboard_tapGesture.cancelsTouchesInView = NO;
    [self.mainContentView addGestureRecognizer:keyboard_tapGesture];
    
}
- (void)doBack:(id)sender
{
    [broadtext resignFirstResponder];
    [self.navigationController popViewControllerAnimated:YES];
    
}
- (void)createBroad
{
    broadtext = [[UITextView alloc]initWithFrame:CGRectMake(-1, 44, _MainScreen_Width+2, 100)];
    broadtext.font = [UIFont systemFontOfSize:16.0f];
    broadtext.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    broadtext.backgroundColor = UIColorFromRGB(0xffffff);
    broadtext.layer.borderWidth = 0.5;
    broadtext.layer.borderColor = [UIColorFromRGB(0xc6c6c7) CGColor];
    broadtext.delegate = self;
    
    uilabel = [[UILabel alloc]init];
    uilabel.frame = CGRectMake(20, 11, 130, 18);
    uilabel.text = @"最多输入40字符";
    uilabel.font = [UIFont systemFontOfSize:16.0f];
    uilabel.textColor = UIColorFromRGB(0x9c9c9c);
    uilabel.enabled = NO;
    uilabel.backgroundColor = [UIColor clearColor];
    [broadtext addSubview:uilabel];
    [self.mainContentView addSubview:broadtext];
}

- (void)ViewTouchDown
{
    [broadtext resignFirstResponder];
    if ([broadtext.text isEqualToString:@""]) {
        uilabel.hidden = NO;
    }
}

- (void)createBtn
{
    UIButton *sendBtn = [[UIButton alloc]initWithFrame:CGRectMake((_MainScreen_Width-110)/2, 168, 110, 30)];
    sendBtn.backgroundColor = UIColorFromRGB(0xe4397d);
    [sendBtn setTitle:@"发送喇叭" forState:UIControlStateNormal];
    [sendBtn setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
    sendBtn.titleLabel.font = [UIFont systemFontOfSize:15.0f];
    sendBtn.layer.cornerRadius = 2;
    [sendBtn addTarget:self action:@selector(btnChange:) forControlEvents:UIControlEventTouchUpInside];
    [self.mainContentView addSubview:sendBtn];
}

#pragma mark - UITextViewDelegate
- (void)textViewDidBeginEditing:(UITextView *)textView
{
    uilabel.hidden = YES;
}

#define MAX_ENTER_LINE 5
- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    if([text isEqualToString:@"\n"])
    {
        NSString* str = [textView text];
        
        int enterNumber = -1;
        const char* enter = str.UTF8String;
        do
        {
            enterNumber++;
            
            enter = strchr(enter, '\n');
            if(enter != NULL)
                enter ++;
        }while (enter != NULL);
        
        if (enterNumber >= MAX_ENTER_LINE)
        {
            [[iToast makeText:@"换行太多啦~"] show];
            return NO;
        }
    }
    return YES;
}

- (void)btnChange:(id)sender
{
    NSInteger number = [broadtext.text length];
    if (![[Reachability reachabilityForInternetConnection] isReachable]) {
        [self showNetwork];
        return ;
    }
    if (![NetUtilCallBack getInstance]->m_bConnectedLobby)
    {
        [self showNetwork];
        return ;
    }
    if (number > 40) {
        [self showErrorAlert:@"字符个数不能超过40个哦"];
        return;
    }
    if(strlen(broadtext.text.UTF8String) == 0)
    {
        [self showErrorAlert:@"不能为空"];
        return ;
    }
        
    USER_DATA_BASE my;
        
    memset(&my,0,sizeof(my));
    GetMyInfo(&my);

    int len_msg = strlen(broadtext.text.UTF8String);
    PLog(@"%@", broadtext.text);
    SendSpeakerToLobby(XG_LOBBY_ID, broadtext.text.UTF8String,len_msg);
    PLog(@"Send Broad to Lobby");
    
    broadtext.text = @"";
    uilabel.hidden = NO;
    linenum = 0;
}

- (void)createLabel
{
    UILabel *lblwarn = [[UILabel alloc] initWithFrame:CGRectMake(28, 222, _MainScreen_Width-55, 30)];
    lblwarn.font = [UIFont systemFontOfSize:12.0f];
    lblwarn.backgroundColor = [UIColor clearColor];
    lblwarn.textColor = UIColorFromRGB(0x666666);
    lblwarn.lineBreakMode = UILineBreakModeWordWrap;
    [lblwarn setNumberOfLines:0];
    lblwarn.text = @"请勿输入任何黄色反动及人身攻击信息，一旦发现，封停账号，情节严重者移送公安机关处理";
    
    /*UILabel *lblwarn1 = [[UILabel alloc]initWithFrame:CGRectMake(28, 222, 265, 13)];
    lblwarn1.text = @"请勿输入任何黄色反动及人身攻击信息，一旦发现";
    lblwarn1.font = [UIFont systemFontOfSize:12.0f];
    lblwarn1.textColor = UIColorFromRGB(0x666666);

    UILabel *lblwarn2 = [[UILabel alloc]initWithFrame:CGRectMake(28, 222+20, 265, 13)];
    lblwarn2.text = @"封停账号，情节严重者移送公安机关处理";
    lblwarn2.font = [UIFont systemFontOfSize:12.0f];
    lblwarn2.textColor = UIColorFromRGB(0x666666);*/
    
    [self.mainContentView addSubview:lblwarn];
}

#pragma mark alertview delegate
-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 1)
    {
        RechargeController *rechange = [[RechargeController alloc] init];
        [self.navigationController pushViewController:rechange animated:YES];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
@end
