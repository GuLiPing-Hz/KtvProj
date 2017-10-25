//
//  RechargeController.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-29.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "RechargeController.h"
#import "KTVAppDelegate.h"
#import "UserSessionManager.h"
#import "User.h"
#import "PKtvAssistantAPI.h"
#import "DDMenuController.h"

@interface RechargeController ()


@end

@implementation RechargeController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        iap = [[InAppPurchaseManager alloc] init];
        iap.delegate = self;
        isPurchase = false;
    }
    return self;
}

-(void)createUserInfo
{
    User *user = [[UserSessionManager getInstance] currentUser];
    
    _userInfo = [[UIView alloc] initWithFrame:CGRectMake(-1, -1, self.mainContentView.frame.size.width + 2, 85 + 1)];
    _userInfo.backgroundColor = [UIColor whiteColor];
    _userInfo.layer.borderWidth  = 1;
    _userInfo.layer.borderColor = UIColorFromRGB(0xc6c6c7).CGColor;
    [self.mainContentView addSubview:_userInfo];
    
    
    NSString *headPhotoString = user.headphoto;
    NSString *standardHeadPhotoString = [CommenTools GetURLWithResolutionScaleTransfered:headPhotoString scale:0];
    _imgHead = [[UIImageView alloc] initWithFrame:CGRectMake(21, 9, 64, 64)];
    _imgHead.layer.cornerRadius = 32;
    _imgHead.layer.masksToBounds = YES;
    _imgHead.layer.borderWidth = 1;
    _imgHead.layer.borderColor = UIColorFromRGB(0xc6c6c7).CGColor;
    [_imgHead setImageWithURL:[NSURL URLWithString:standardHeadPhotoString] placeholderImage:[UIImage imageNamed:@"userFace_normal_b.png"]];
    [_userInfo addSubview:_imgHead];
    
    _imgSex = [[UIImageView alloc]initWithFrame:CGRectMake(103, 20, 14, 14)];
    if (user.gender == 0)
        _imgSex.image = [UIImage imageNamed:@"icon_women.png"];
    else
        _imgSex.image = [UIImage imageNamed:@"icon_women.png"];
    _imgSex.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview:_imgSex];
    
    UIImageView *imgMoney = [[UIImageView alloc]initWithFrame:CGRectMake(103, 50, 14, 14)];
    imgMoney.image = [UIImage imageNamed:@"icon_user_money.png"];
    imgMoney.backgroundColor = [UIColor clearColor];
    [_userInfo addSubview:imgMoney];
    
    _lblName = [[UILabel alloc] initWithFrame:CGRectMake(120, 20, 160, 16)];
    _lblName.textColor = UIColorFromRGB(0xe4397d);
    _lblName.backgroundColor = [UIColor clearColor];
    _lblName.font = [UIFont systemFontOfSize:16];
    _lblName.text = user.name;
    [_lblName sizeToFit];
    [_userInfo addSubview:_lblName];
    
    UILabel *lblMoneyTip = [[UILabel alloc] initWithFrame:CGRectMake(120, 50, 55, 14)];
    lblMoneyTip.textColor = UIColorFromRGB(0x9c9c9c);
    lblMoneyTip.font = [UIFont systemFontOfSize:14];
    lblMoneyTip.backgroundColor = [UIColor clearColor];
    lblMoneyTip.text = @"拥有金币";
    [lblMoneyTip sizeToFit];
    [_userInfo addSubview:lblMoneyTip];
    
    _lblMoney = [[UILabel alloc] initWithFrame:CGRectMake(lblMoneyTip.frame.size.width + lblMoneyTip.frame.origin.x + 10, 50, 90, 14)];
    _lblMoney.textColor = [UIColor blackColor];
    _lblMoney.backgroundColor = [UIColor clearColor];
    _lblMoney.font = [UIFont systemFontOfSize:14];
    _lblMoney.text = [[NSString alloc] initWithFormat:@"%ld",user.gold];
    [_lblMoney sizeToFit];
    [_userInfo addSubview:_lblMoney];
    
    _purchseBack = [[UIView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height)];
    _purchseBack.backgroundColor = [UIColor blackColor];
    _purchseBack.alpha = 0.4;
    _purchseBack.hidden = YES;
    [self.view addSubview:_purchseBack];
}

-(void)createTips
{
    CGSize size = [@"首次充值98元即可免费获得10000金币" sizeWithFont:[UIFont systemFontOfSize:15.0] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
    UIImageView *tipLine = [[UIImageView alloc] initWithFrame:CGRectMake(20, 110, _MainScreen_Width - 40, 1)];
    tipLine.backgroundColor = UIColorFromRGB(0xc6c6c7);
    [_recharge addSubview:tipLine];
    
    _lblTip = [[UILabel alloc] initWithFrame:CGRectMake((_MainScreen_Width-size.width)/2, 120, _MainScreen_Width - 40, 15)];
    _lblTip.textColor = UIColorFromRGB(0x9c9c9c);
    _lblTip.font = [UIFont systemFontOfSize:15];
    _lblTip.numberOfLines = 0;
    _lblTip.text = @"首次充值98元即可免费获得10000金币";
    [_recharge addSubview:_lblTip];
}

-(void)createChargeBtn:(NSInteger)index
{
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    UILabel *lbl = [[UILabel alloc] init];
    btn.frame = CGRectMake((_MainScreen_Width-55*4)/5 * (index + 1) + 55 * index, 15, 55, 55);
    [btn setBackgroundImage:[UIImage imageNamed:@"user_center_btn_buyA0.png"] forState:UIControlStateNormal];
    [btn setBackgroundImage:[UIImage imageNamed:@"user_center_btn_buyA1.png"] forState:UIControlStateHighlighted];
    if (index == 0)
    {
        [btn setTitle:@"8400  金币" forState:UIControlStateNormal];
        lbl.text = @"12元";
    }
    else if (index == 1)
    {
        [btn setTitle:@"21000金币" forState:UIControlStateNormal];
        lbl.text = @"30元";
    }
    else if (index == 2)
    {
        [btn setTitle:@"35000金币" forState:UIControlStateNormal];
        lbl.text = @"50元";
    }
    else if (index == 3)
    {
        [btn setTitle:@"68600金币" forState:UIControlStateNormal];
        lbl.text = @"98元";
    }
    [btn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    btn.titleLabel.lineBreakMode = NSLineBreakByWordWrapping;
    btn.titleLabel.numberOfLines = 0;
    btn.titleLabel.textAlignment = NSTextAlignmentCenter;
    btn.titleLabel.font = [UIFont systemFontOfSize:15];
    btn.backgroundColor = [UIColor clearColor];
    btn.tag = index;
    [btn addTarget:self action:@selector(BtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [_recharge addSubview: btn];
    [_btnRecharge addObject:btn];
    
    lbl.font = [UIFont systemFontOfSize:15];
    CGSize size = [lbl.text sizeWithFont:[UIFont systemFontOfSize:15] constrainedToSize:CGSizeMake(60, 15)];
    lbl.frame = CGRectMake(btn.frame.origin.x + 28 - size.width / 2, 80, size.width, size.height);
    lbl.textColor = UIColorFromRGB(0x666666);
    [_recharge addSubview:lbl];
    [_lblRecharge addObject:lbl];
}

-(void)BtnClick:(id)sender
{
    if(![[Reachability reachabilityForInternetConnection] isReachable])
    {
        [self showNetwork];
        return;
    }
    UIButton *btn = (UIButton *)sender;
    if ([iap canMakePurchases]) {
        
        [iap loadStore:productName[btn.tag]];
        [SVProgressHUD show];
        _purchseBack.hidden = NO;
        isPurchase = true;
    }
    
}


-(void)createCharge
{
    CGSize size = [@"购买金币" sizeWithFont:[UIFont systemFontOfSize:14.0] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
    UILabel *lblChargeTitle = [[UILabel alloc] initWithFrame:CGRectMake((_MainScreen_Width-size.width)/2, 100, 120, 14)];
    lblChargeTitle.text = @"购买金币";
    lblChargeTitle.textColor = UIColorFromRGB(0x666666);
    lblChargeTitle.font = [UIFont systemFontOfSize:14];
    lblChargeTitle.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview:lblChargeTitle];
    
    _recharge = [[UIView alloc] initWithFrame:CGRectMake(0, 129, self.mainContentView.frame.size.width, 151)];
    _recharge.backgroundColor = [UIColor whiteColor];
    _recharge.layer.borderWidth = 1;
    _recharge.layer.borderColor = UIColorFromRGB(0xc6c6c7).CGColor;
    [self.mainContentView addSubview:_recharge];
    
    [self createChargeBtn:0];
    [self createChargeBtn:1];
    [self createChargeBtn:2];
    [self createChargeBtn:3];
    
}

-(void)initProductName
{
    productName = [[NSMutableArray alloc] init];
    [productName addObject:@"com.tiange.ktvassistant.first"];
    [productName addObject:@"com.tiange.ktvassistant.two"];
    [productName addObject:@"com.tiange.ktvassistant.three"];
    [productName addObject:@"com.tiange.ktvassistant.four"];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.view.backgroundColor = UIColorFromRGB(0xf8f8f9);
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.navView.titleLabel.text = @"金币充值";
    // Do any additional setup after loading the view.
    [self initProductName];
    [self createUserInfo];
    [self createCharge];
    [self createTips];
    
    // 滑动返回手势
    if (!_swipe) {
        UISwipeGestureRecognizer *swipe = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipe:)];
        swipe.delegate = (id<UIGestureRecognizerDelegate>)self;
        swipe.direction = UISwipeGestureRecognizerDirectionRight;
        [self.view addGestureRecognizer:swipe];
        _swipe = swipe;
    }
}

- (void)swipe:(UISwipeGestureRecognizer *)gesture
{
    if (gesture.direction == UISwipeGestureRecognizerDirectionRight && !isPurchase) {
        [self.navigationController popViewControllerAnimated:true];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:false];
    [self UpdateInfo];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(UpdateInfo) name:KTVASSISTANT_GOLD_UPDATE_NOTIFICATION object:nil];//刷新金币值
}

-(void)UpdateInfo
{
    _lblMoney.text = [[NSString alloc] initWithFormat:@"%ld",[UserSessionManager getInstance].currentUser.gold];
    [_lblMoney sizeToFit];
}

-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:true];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark InAppPurchaseDelegate
-(void)PurchaseDown
{
    _lblMoney.text = [[NSString alloc] initWithFormat:@"%ld",[UserSessionManager getInstance].currentUser.gold];
    [_lblMoney sizeToFit];
    [SVProgressHUD dismiss];
    _purchseBack.hidden = YES;
    isPurchase = false;
}

-(void)PurchaseFail
{
    _lblMoney.text = [[NSString alloc] initWithFormat:@"%ld",[UserSessionManager getInstance].currentUser.gold];
    [_lblMoney sizeToFit];
    [SVProgressHUD showErrorWithStatus:@"充值失败,有问题请联系ktv工作人员"];
    _purchseBack.hidden = YES;
    isPurchase = false;
}




@end
