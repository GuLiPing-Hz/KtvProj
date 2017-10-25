//
//  UserCenterViewController.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-16.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "UserCenterViewController.h"
#import "PKtvAssistantAPI.h"
#import "KTVInfoController.h"
#import "CustomTabView.h"
#import "RegexKitLite.h"
#import "iToast.h"
#import "UserCenterTableView.h"
#import "RechargeController.h"
#import "KTVBaseConfig.h"
#import "UserSessionManager.h"
#import "CommenTools.h"
#import "AFHTTPRequestOperation.h"
#import "SVProgressHUD.h"
#import "Reachability.h"
#import "PCommonUtil.h"
#import "AFHTTPClient.h"
#import "ExchangeHistoryController.h"
#import "KTVAppDelegate.h"
#import "DDMenuController.h"

@interface UserCenterViewController ()

@end

@implementation UserCenterViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

-(void)createSeperate:(NSInteger)width
{
    UIImageView *separate = [[UIImageView alloc]initWithFrame:CGRectMake(width, 22, 1, 58)];
    separate.backgroundColor = UIColorFromRGB(0xc6c6c7);
    [self.mainContentView addSubview: separate];
}



-(void)createHeadImg
{
    //头像按钮
    _btnHead = [[UIButton alloc]initWithFrame:CGRectMake((_MainScreen_Width/3-70)/2, 16, 70, 70)];
    _btnHead.backgroundColor = [UIColor clearColor];
    [_btnHead addTarget:self action:@selector(changePhoto) forControlEvents:UIControlEventTouchUpInside];
    [self.mainContentView addSubview: _btnHead];
    
    //大头像
    _imgHead = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 64, 64)];
    _imgHead.backgroundColor = [UIColor clearColor];
    _imgHead.layer.cornerRadius = 32;
    _imgHead.layer.borderWidth = 1 ;
    _imgHead.layer.borderColor = UIColorFromRGB(0xc6c6c7).CGColor;
    _imgHead.layer.masksToBounds = YES;
    [_btnHead addSubview:_imgHead];
    NSString *headPhotoString = [UserSessionManager getInstance].currentUser.headphoto;
    NSString *standardHeadPhotoString = [CommenTools GetURLWithResolutionScaleTransfered:headPhotoString scale:0];
    [self.imgHead setImageWithURL:[NSURL URLWithString:standardHeadPhotoString] placeholderImage:[UIImage imageNamed:@"userFace_normal_b.png"]];
    
    //拍照图标
    UIImageView *takePhotoView = [[UIImageView alloc]initWithFrame:CGRectMake(44, 44, 22, 22)];
    takePhotoView.backgroundColor = [UIColor clearColor];
    takePhotoView.image = [UIImage imageNamed:@"user_center_face_camera.png"];
    takePhotoView.layer.cornerRadius = 11;
    takePhotoView.layer.borderWidth = 1 ;
    takePhotoView.layer.borderColor = [UIColor whiteColor].CGColor;
    takePhotoView.layer.masksToBounds = YES;
    [_btnHead addSubview:takePhotoView];

}


-(void)createButton
{
    _btnGift = [UIButton buttonWithType:UIButtonTypeCustom];
    _btnGift.frame = CGRectMake(_MainScreen_Width/3+(_MainScreen_Width/3-46)/2, 20, 46, 42);
    _btnGift.backgroundColor = [UIColor clearColor];
    _btnGift.tag = 1 ;
    [_btnGift setImage:[UIImage imageNamed:@"user_center_btn_gift0.png"] forState:UIControlStateNormal];
    [_btnGift setImage:[UIImage imageNamed:@"user_center_btn_gift1.png"] forState:UIControlStateHighlighted];
    [_btnGift addTarget:self action:@selector(btnAction:) forControlEvents:UIControlEventTouchUpInside];
    [self.mainContentView addSubview: _btnGift];

    _lblGift= [[UILabel alloc]initWithFrame:CGRectMake(_MainScreen_Width/3+(_MainScreen_Width/3-11-35)/2, 67, 11, 11)];
    _lblGift.text = @"礼物兑换";
    _lblGift.font = [UIFont systemFontOfSize:11];
    _lblGift.textColor = UIColorFromRGB(0x666666);
    [_lblGift sizeToFit];
    [self.mainContentView addSubview:_lblGift];
    
    _btnGold = [UIButton buttonWithType:UIButtonTypeCustom];
    _btnGold.frame = CGRectMake(_MainScreen_Width/3*2+(_MainScreen_Width/3-46)/2, 20, 46, 42);
    _btnGold.backgroundColor = [UIColor clearColor];
    _btnGold.tag = 2 ;
    [_btnGold setImage:[UIImage imageNamed:@"user_center_btn_charge0.png"] forState:UIControlStateNormal];
    [_btnGold setImage:[UIImage imageNamed:@"user_center_btn_charge1.png"] forState:UIControlStateHighlighted];
    [_btnGold addTarget:self action:@selector(btnAction:) forControlEvents:UIControlEventTouchUpInside];
    [self.mainContentView addSubview: _btnGold];
    
    UILabel *lblGold= [[UILabel alloc]initWithFrame:CGRectMake(_MainScreen_Width/3*2+(_MainScreen_Width/3-11-35)/2, 67, 11, 11)];
    lblGold.text = @"购买金币";
    lblGold.font = [UIFont systemFontOfSize:11];
    lblGold.textColor = UIColorFromRGB(0x666666);
    [lblGold sizeToFit];
    [self.mainContentView addSubview:lblGold];
}

//金币 礼物
-(void)btnAction:(id)sender
{
    UIButton *btn =(UIButton *)sender;
    if(btn.tag == 1) //礼物
    {
        KTVInfoController *ktvInfo = [[KTVInfoController alloc] init];
        [self.navigationController pushViewController:ktvInfo animated:YES];
    }
    else  //购买金币
    {
        RechargeController *rechange = [[RechargeController alloc] init];
        [self.navigationController pushViewController:rechange animated:YES];
    }
}

-(void)createInfo
{
    User *user = [UserSessionManager getInstance].currentUser;
    UIImageView *imgSex = [[UIImageView alloc]initWithFrame:CGRectMake(14, 94, 14, 14)];
    if (user.gender == 0)
        imgSex.image = [UIImage imageNamed:@"icon_women.png"];
    else
        imgSex.image = [UIImage imageNamed:@"icon_men.png"];
    imgSex.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview:imgSex];
    
    
    UIImageView *imgMoney = [[UIImageView alloc]initWithFrame:CGRectMake(_MainScreen_Width-123, 94, 14, 14)];
    imgMoney.image = [UIImage imageNamed:@"icon_user_money.png"];
    imgMoney.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview:imgMoney];
    
    _lblName = [[UILabel alloc] initWithFrame:CGRectMake(32, 91, 20, 14)];
    _lblName.textColor = UIColorFromRGB(0xe4397d);
    _lblName.font = [UIFont systemFontOfSize:16];
    _lblName.text = user.name;
    [_lblName sizeToFit];
    [self.mainContentView addSubview:_lblName];
    
    
    _lblMoney = [[UILabel alloc] initWithFrame:CGRectMake(_MainScreen_Width-107, 91, 20, 14)];
    _lblMoney.textColor = UIColorFromRGB(0x666666);
    _lblMoney.font = [UIFont systemFontOfSize:16];
    _lblMoney.text = [[NSString alloc] initWithFormat:@"%ld",user.gold];
    [_lblMoney sizeToFit];
    [self.mainContentView addSubview:_lblMoney];
}

-(void)createUserInfoView
{
    [self createSeperate:_MainScreen_Width/3];//第一条分割线
    [self createSeperate:_MainScreen_Width/3*2];//第二条分割线
    [self createHeadImg];//头像
    [self createButton];//礼物、金币按钮
    [self createInfo];//用户名字 金币信息
}

#pragma mark - 换头像按钮

-(void)changePhoto
{    _changeHeadPhotoSheet = [[UIActionSheet alloc] initWithTitle:nil delegate:self cancelButtonTitle:@"取消" destructiveButtonTitle:nil otherButtonTitles:@"拍照",@"从相册选择", nil];
    [_changeHeadPhotoSheet showInView:self.view];
}


#pragma mark -UIActionSheet delegate

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex

{
    if (buttonIndex == 0) // 调用系统拍照
    {
        if (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera])
        {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil message:@"该设备不支持拍照功能" delegate:nil cancelButtonTitle:nil otherButtonTitles:@"好", nil];
            [alert show];
        }
        else
        {
            UIImagePickerController * imagePickerController = [[UIImagePickerController alloc]init];
            imagePickerController.sourceType = UIImagePickerControllerSourceTypeCamera;
            imagePickerController.delegate = self;
            imagePickerController.allowsEditing = YES;
            [self presentModalViewController:imagePickerController animated:YES];
        }
    }
    else if (buttonIndex == 1) // 调用系统相册库
    {
        UIImagePickerController * imagePickerController = [[UIImagePickerController alloc]init];
        imagePickerController.navigationBar.tintColor = [UIColor whiteColor];
        if (IOS7_OR_LATER) {
            imagePickerController.navigationBar.barTintColor = [UIColor grayColor];
            imagePickerController.navigationBar.tintColor = [UIColor whiteColor];
        }
        else
        {
            imagePickerController.navigationBar.tintColor = [UIColor grayColor];
        }
        imagePickerController.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
        imagePickerController.delegate = self;
        imagePickerController.allowsEditing = YES;
        [self presentModalViewController:imagePickerController animated:YES];
    }
}

#pragma mark - 上传头像 UIImagePickerController delegate

// 选完照片的委托
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    if(![[Reachability reachabilityForInternetConnection] isReachable])
    {
        [self dismissModalViewControllerAnimated:YES];
        [self showNetwork];
        return;
    }
    [SVProgressHUD showWithStatus:@"上传中..." maskType:SVProgressHUDMaskTypeBlack];
    [picker dismissModalViewControllerAnimated:YES];
    //UIImage *oldImage = [info objectForKey:UIImagePickerControllerOriginalImage];
    //UIImage *newImage = [self imageWithImage:oldImage scaledToSize:CGSizeMake(640, 640)];  // 640*640
    UIImage *oldImage = [info objectForKey:UIImagePickerControllerEditedImage];
    UIImage *newImage = oldImage;
    NSData* imageData;
    imageData = UIImageJPEGRepresentation(newImage, 0.7);
    _changeHeadImage = [UIImage imageWithData:imageData];
    
    // 保存头像到本地
    NSString *photoName = @"UserHeadPhoto.jpg";
    NSString *path = [[NSHomeDirectory() stringByAppendingPathComponent:@"Documents"] stringByAppendingPathComponent:@"HeadPhotoFile"];
    NSString *headPhotoPath = [path stringByAppendingPathComponent:photoName];
    [imageData writeToFile:headPhotoPath atomically:NO];
    
    // 将头像上传服务器
    long userID = [UserSessionManager getInstance].currentUser.uid;
    NSString *roomId = [UserSessionManager getInstance].currentRoomInfo.roomId;
    NSString *encodeUrl = [PKtvAssistantAPI getUserUploadPhotoUrl:userID roomId:roomId photoName:photoName];
    
    NSString *baseURLstr = kPKtvAssistantAPIDomain;
    NSString *pathURLstr = [encodeUrl stringByReplacingOccurrencesOfString:baseURLstr withString:@""];
    //PLog(@"pathURLstr-->%@",pathURLstr);
    
    NSURL *baseURL = [NSURL URLWithString:baseURLstr];
    AFHTTPClient *httpClient = [[AFHTTPClient alloc] initWithBaseURL:baseURL];
    
    NSMutableURLRequest *request = [httpClient multipartFormRequestWithMethod:@"POST" path:pathURLstr parameters:nil constructingBodyWithBlock: ^(id <AFMultipartFormData>formData) {
        [formData appendPartWithFileData:imageData name:@"imagedata" fileName:photoName mimeType:@"image/jpeg"];
    }];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setUploadProgressBlock:^(NSUInteger bytesWritten, long long totalBytesWritten, long long totalBytesExpectedToWrite) {
        PLog(@"上传 %lld of %lld bytes", totalBytesWritten, totalBytesExpectedToWrite);
    }];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject){
        
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame)
        {
            [SVProgressHUD dismissWithSuccess:@"头像上传成功"];
            PLog(@"头像上传成功...");
            // 更新头像
            [_imgHead setImage:_changeHeadImage];
            NSDictionary *result = [results objectForKey:@"result"];
            User *user = [User initWithLoginDictionary:result];
            [[UserSessionManager getInstance] updateCurrentUserInfo:user];
            _lblName.text = user.name;
            _lblMoney.text = [[NSString alloc] initWithFormat:@"%ld",user.gold,nil];
            [_lblMoney sizeToFit];
        }
        else
        {
            [SVProgressHUD dismissWithError:@"头像上传失败"];
            PLog(@"头像上传失败...");
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doUserUploadPhoto failed...errorcode:%@(%@)", errorCode, msg);
        }
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        [SVProgressHUD dismiss];
        PLog(@"doLogin failed...");
        [CommenTools showNetworkDisconnectedAlertView];
    }];
    [operation start];
    
}

#pragma mark - 解决push照片库时状态栏颜色改变

- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated
{
    if (IOS7_OR_LATER) {
        [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
    }
}

- (void)navigationController:(UINavigationController *)navigationController didShowViewController:(UIViewController *)viewController animated:(BOOL)animated
{
    if (IOS7_OR_LATER) {
        [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleLightContent];
    }
}

/**
 * 对图片尺寸进行压缩
 */
-(UIImage*)imageWithImage:(UIImage*)image scaledToSize:(CGSize)newSize
{
    // Create a graphics image context
    UIGraphicsBeginImageContext(newSize);
    
    // Tell the old image to draw in this new context, with the desired
    // new size
    [image drawInRect:CGRectMake(0,0,newSize.width,newSize.height)];
    
    // Get the new image from the context
    UIImage* newImage = UIGraphicsGetImageFromCurrentImageContext();
    
    // End the context
    UIGraphicsEndImageContext();
    
    // Return the new image.
    return newImage;
}

/**
 *	取消按钮的委托
 */
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker

{
    [self dismissModalViewControllerAnimated:YES];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.navView.hidden = NO;
    self.navView.titleLabel.text = @"个人中心";
    self.navView.backButton.hidden = NO;
    self.navView.btnHistory.hidden = NO;
    [self.navView.btnHistory addTarget:self action:@selector(ShowHistory) forControlEvents:UIControlEventTouchUpInside];
    
    //个人信息界面
    [self createUserInfoView];
    
    self.mainContentView.backgroundColor = [UIColor whiteColor];
    NSMutableArray *titles = [[NSMutableArray alloc] initWithObjects:@"系统奖励",@"收到的礼物",@"送出的礼物",nil];
    
    _tbView= [[NSMutableArray alloc]init];
    [self createTable:0];
    [self createTable:1];
    [self createTable:2];
    CustomTabView *view = [[CustomTabView alloc]initWithFrame:CGRectMake(0, 126, _MainScreen_Width, self.mainContentView.frame.size.height-126) titles:titles views:_tbView];
    [self.mainContentView addSubview:view];
    [self isShowGiftChange];
}

- (void)isShowGiftChange{
    NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];
    NSString *defaultKey = KTVASSISTANT_USERDEFAULTS_ISSHOW;
    NSString *value = [defaults objectForKey:defaultKey];
    if (value != nil) {
        if ([value compare:@"1"] == NSOrderedSame)
        {
            _lblGift.hidden = NO;
            _btnGift.hidden = NO;
            self.navView.btnHistory.hidden = NO;
            return;
        }
        else
        {
            _lblGift.hidden = YES;
            _btnGift.hidden = YES;
            self.navView.btnHistory.hidden = YES;
        }
    }
}

-(void)ShowHistory
{
    ExchangeHistoryController *controller = [[ExchangeHistoryController alloc] init];
    [self.navigationController pushViewController:controller animated:true];
}


-(void)createTable:(NSInteger)index
{
    UserCenterTableView *table = [[UserCenterTableView alloc]initWithFrame:CGRectMake(0, 44, _MainScreen_Width, self.mainContentView.frame.size.height-126 - 44) type:index];
    table.hidden = YES;
    [_tbView addObject: table];
}

-(void)UpdateInfo
{
    _lblMoney.text = [[NSString alloc] initWithFormat:@"%ld", [UserSessionManager getInstance].currentUser.gold];
    [_lblMoney sizeToFit];
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:NO];
    [self UpdateInfo];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(UpdateInfo) name:KTVASSISTANT_GOLD_UPDATE_NOTIFICATION object:nil];//刷新金币值
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(isShowGiftChange) name:KTVASSISTANT_ISSHOW_NOTIFICATION object:nil];
}

-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:YES];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


-(void)updateUserGold
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
            _lblMoney.text = [NSString stringWithFormat:@"%ld",user.gold];
            [_lblMoney sizeToFit];
        }
        else
        {
            
        }
        
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        
    }];
    [operation start];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
