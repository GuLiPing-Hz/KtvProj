//
//  UserRegisterController.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-11-3.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "UserRegisterController.h"
#import "UIImage+Color.h"
#import "PKtvAssistantAPI.h"
#import "AFHTTPRequestOperation.h"
#import "com_ios_NetUtil.h"
#import "NetUtilCallBack.h"
#import "KTVAppDelegate.h"

@interface UserRegisterController ()

@end

@implementation UserRegisterController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = UIColorFromRGB(0xf8f8f9);
    self.navView.titleLabel.text = @"注册";
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    _sex = 1;//男
    [self isShowSex];
    [self createTable];
    [self createBtn];
    UITapGestureRecognizer *recognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(hideKeybord)];
    recognizer.delegate = self;
    [self.view addGestureRecognizer:recognizer];
}

-(void)isShowSex
{
    isShow = FALSE;
    space = 44;
    NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];
    NSString *defaultKey = KTVASSISTANT_USERDEFAULTS_ISSHOW;
    NSString *value = [defaults objectForKey:defaultKey];
    if (value != nil) {
        isShow = [value compare:@"1"] == NSOrderedSame;
        if (isShow)
            space = 0;
    }
}

-(BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
{
    if ([NSStringFromClass([touch.view class]) isEqualToString:@"UITableViewCellContentView"]) {//如果当前是tableView
        //做自己想做的事
        return NO;
    }
    return YES;
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(recvLoginLobbyMsg:) name:KTVAssistantLoginLobbyResult object:nil];
}

-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

-(void)hideKeybord
{
    [_txtName resignFirstResponder];
    [_txtPwd resignFirstResponder];
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
        [self showErrorAlert:@"登录失败"];
        KTVAppDelegate *delegate = (KTVAppDelegate *)[UIApplication sharedApplication].delegate;
    }
}

//登录主界面
- (void)loginMainView
{
    [self.appDelegate loginMain];
}

-(void)createTable
{
    _tableRegister = [[UITableView alloc]initWithFrame:CGRectMake(0, 25, _MainScreen_Width, 132 - space) style:UITableViewStylePlain];
    _tableRegister.tag = 0;
    _tableRegister.bounces = NO;
    _tableRegister.separatorStyle = UITableViewCellSeparatorStyleNone;
    _tableRegister.tableFooterView = [[UIView alloc] init];
    _tableRegister.dataSource = self;
    _tableRegister.delegate = self;
    [self.mainContentView addSubview:_tableRegister];
}

-(void)createBtn
{
    _btnRegister = [UIButton buttonWithType:UIButtonTypeCustom];
    _btnRegister.frame = CGRectMake((_MainScreen_Width - 294) / 2, 181 - space, 294, 40);
    _btnRegister.layer.cornerRadius = 2;
    _btnRegister.titleLabel.font = [UIFont systemFontOfSize:18];
    [_btnRegister setTitle:@"注册" forState:UIControlStateNormal];
    [_btnRegister setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [_btnRegister setBackgroundImage:[UIImage imageWithColor:UIColorFromRGB(0x1b88ff) Rect:CGRectMake(0, 0, 294, 26)] forState:UIControlStateNormal];
    [_btnRegister setBackgroundImage:[UIImage imageWithColor:UIColorFromRGB(0x007aff) Rect:CGRectMake(0, 0, 294, 26)] forState:UIControlStateNormal];
    _btnRegister.layer.masksToBounds = YES;
    [_btnRegister addTarget:self action:@selector(btnClick) forControlEvents:UIControlEventTouchUpInside];
    [self.mainContentView addSubview:_btnRegister];
}

-(void)btnClick
{
    NSString *name = _txtName.text;
    NSString *pwd = _txtPwd.text;
    if (name.length < 6 || name.length>16)
    {
        [self showErrorAlert:@"账号长度为6-16"];
        return;
    }
    if (pwd.length < 5 || pwd.length>12)
    {
        [self showErrorAlert:@"密码长度为5-12"];
        return;
    }
    [self Register:name pwd:pwd];
}

#pragma mark tabledelegate
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 3;
}
-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 44;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"cell"];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
        cell.backgroundColor = [UIColor clearColor];
    }
    cell.backgroundColor = [UIColor whiteColor];
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    if (indexPath.row == 0)
        [cell addSubview:[self createSeparator:NO]];
    [cell addSubview:[self createSeparator:YES]];
    UILabel *lblRegister = [[UILabel alloc] initWithFrame:CGRectMake(20, 12, 40, 20)];
    lblRegister.backgroundColor = [UIColor clearColor];
    lblRegister.font = [UIFont systemFontOfSize:17];
    lblRegister.textColor = UIColorFromRGB(0x9c9c9c);
    UITextField *text;
    if (indexPath.row == 0)
    {
        lblRegister.text = @"账号:";
        _txtName = [[UITextField alloc] initWithFrame:CGRectMake(70, 2, _MainScreen_Width - 80, cell.frame.size.height - 2)];
        _txtName.tag = 0;
        _txtName.placeholder = @"账号最大长度6-16";
        text = _txtName;
        [cell addSubview:text];
    }
    else if(indexPath.row == 1)
    {
        lblRegister.text = @"密码:";
        _txtPwd = [[UITextField alloc] initWithFrame:CGRectMake(70, 2, _MainScreen_Width - 80, cell.frame.size.height - 2)];
        _txtPwd.tag = 1;
        _txtPwd.placeholder = @"密码最大长度5-12";
        text = _txtPwd;
        _txtPwd.keyboardType = UIKeyboardTypeAlphabet;
        [cell addSubview:text];
    }
    else//性别选择按钮
    {
        cell.hidden = !isShow;
        _imgMen = [[UIImageView alloc] initWithFrame:CGRectMake(70, 14, 16, 16)];
        _imgMen.backgroundColor = [UIColor clearColor];
        _imgMen.image = [UIImage imageNamed:@"pic_selected"];
        
        _imgWomen = [[UIImageView alloc] initWithFrame:CGRectMake(170, 14, 16, 16)];
        _imgWomen.backgroundColor = [UIColor clearColor];
        _imgWomen.image = [UIImage imageNamed:@"pic_selected"];
        _imgWomen.hidden = YES;
        
        UIButton *btnMen = [UIButton buttonWithType:UIButtonTypeCustom];
        btnMen.backgroundColor = [UIColor clearColor];
        btnMen.frame = CGRectMake(95, 12, 40, 20);
        btnMen.tag = 1;
        btnMen.userInteractionEnabled = YES;
        UIImageView *btnImgMen = [[UIImageView alloc] initWithFrame:CGRectMake(22, 2, 14, 14)];
        btnImgMen.image = [UIImage imageNamed:@"icon_men"];
        btnImgMen.backgroundColor = [UIColor clearColor];
        UILabel *lblMen = [[UILabel alloc]initWithFrame:CGRectMake(0, 1, 26, 17)];
        lblMen.backgroundColor = [UIColor clearColor];
        lblMen.text = @"男";
        lblMen.font = [UIFont systemFontOfSize:17];
        lblMen.textColor = [UIColor blackColor];
        [btnMen addSubview:lblMen];
        [btnMen addSubview:btnImgMen];
        [btnMen addTarget:self action:@selector(sexClick:) forControlEvents:UIControlEventTouchUpInside];
        
        UIButton *btnWomen = [UIButton buttonWithType:UIButtonTypeCustom];
        btnWomen.backgroundColor = [UIColor clearColor];
        btnWomen.frame = CGRectMake(190, 12, 40, 20);
        btnWomen.tag = 0;
        btnWomen.userInteractionEnabled = YES;
        UIImageView *btnImgWomen = [[UIImageView alloc] initWithFrame:CGRectMake(22, 2, 14, 14)];
        btnImgWomen.image = [UIImage imageNamed:@"icon_women"];
        btnImgWomen.backgroundColor = [UIColor clearColor];
        UILabel *lblWomen = [[UILabel alloc]initWithFrame:CGRectMake(0, 1, 26, 17)];
        lblWomen.backgroundColor = [UIColor clearColor];
        lblWomen.text = @"女";
        lblWomen.font = [UIFont systemFontOfSize:17];
        lblWomen.textColor = [UIColor blackColor];
        [btnWomen addSubview:lblWomen];
        [btnWomen addSubview:btnImgWomen];
        [btnWomen addTarget:self action:@selector(sexClick:) forControlEvents:UIControlEventTouchUpInside];
        
        
        [cell addSubview: _imgMen];
        [cell addSubview: _imgWomen];
        [cell addSubview: btnMen];
        [cell addSubview: btnWomen];
        lblRegister.text = @"性别:";
    }
    text.layer.borderWidth = 0;
    text.autocapitalizationType = UITextAutocapitalizationTypeNone;
    text.autocorrectionType = UITextAutocorrectionTypeNo;
    text.font = [UIFont systemFontOfSize:17];
    text.textColor = [UIColor blackColor];
    text.delegate = self;
    text.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
    text.returnKeyType = UIReturnKeyDone;
    text.tag = indexPath.row;
    text.clearButtonMode = UITextFieldViewModeWhileEditing;
    [text addTarget:self action:@selector(textFieldDidEndEditing:) forControlEvents:UIControlEventEditingDidEndOnExit];

    [cell addSubview:lblRegister];
    return cell;
}

-(void)sexClick:(id)sender
{
    UIButton *btn = (UIButton *)sender;
    if (_sex == 0 && btn.tag == 1)
    {
        _sex = 1;
        _imgMen.hidden = NO;
        _imgWomen.hidden = YES;
    }
    else if(_sex == 1 && btn.tag == 0)
    {
        _sex = 0;
        _imgMen.hidden = YES;
        _imgWomen.hidden = NO;
    }
}

-(UIImageView *)createSeparator:(BOOL)isBottom
{
    UIImageView *line = [[UIImageView alloc] init];
    if (isBottom)
        line.frame = CGRectMake(0, 43, _MainScreen_Width, 1);
    else
        line.frame = CGRectMake(0, 0, _MainScreen_Width, 1);
    line.backgroundColor = UIColorFromRGB(0xc6c6c7);
    return  line;
}

#pragma mark textfield delegate
-(void)textFieldDidEndEditing:(UITextField *)textField
{
    [textField resignFirstResponder];
}

-(BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    NSInteger max = 16;
    if (textField.tag == 1)
        max = 12;
    if (range.location >= max)
        return NO;
    return YES;
}

-(void)Register:(NSString *)name pwd:(NSString *)pwd
{
    if (![[Reachability reachabilityForInternetConnection] isReachable]) {
        [self showNetwork];
        return;
    }
    [SVProgressHUD showWithStatus:@"正在注册" maskType:SVProgressHUDMaskTypeClear];
    NSString *strUrl = [PKtvAssistantAPI getUserRegisterUrl:name pwd:pwd sex:_sex];
    NSURL *url = [NSURL URLWithString:strUrl];
    NSMutableURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:url];
    [AFHTTPRequestOperation  addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain",nil]];
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
            PLog(@"%@",user.pwd);
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
            if ([errorCode compare:@"-10"] == NSOrderedSame || [errorCode compare:@"-9"] == NSOrderedSame  || [errorCode compare:@"-3"] == NSOrderedSame  || [errorCode compare:@"-4"] == NSOrderedSame  || [errorCode compare:@"-5"] == NSOrderedSame  || [errorCode compare:@"-6"] == NSOrderedSame )
            {
                [SVProgressHUD dismissWithError:@"所在网络注册受限,请用qq或微博登录" afterDelay:1.0];
            }
            else
            {
                [SVProgressHUD dismiss];
                [self showErrorAlert:msg];
            }
        }

    }
    failure:^(AFHTTPRequestOperation *operation, NSError *error)
     {
         [SVProgressHUD dismiss];
         [CommenTools showNetworkDisconnectedAlertView];
    }];
    [operation start];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
