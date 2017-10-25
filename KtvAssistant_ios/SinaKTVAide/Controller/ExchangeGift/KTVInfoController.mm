//
//  KTVInfoController.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-23.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "KTVInfoController.h"
#import "KTVInfoCell.h"
#import "KTVInfo.h"
#import "PKtvAssistantAPI.h"
#import "ExchangeHistoryController.h"
#import "GIftInfoController.h"
#import "KTVAppDelegate.h"
#import "DDMenuController.h"
#import "iToast.h"

@interface KTVInfoController ()

@end

@implementation KTVInfoController

//UIColor 转UIImage
- (UIImage*) createImageWithColor: (UIColor*) color
{
    CGRect rect=CGRectMake(0.0f, 0.0f, 1.0f, 1.0f);
    UIGraphicsBeginImageContext(rect.size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetFillColorWithColor(context, [color CGColor]);
    CGContextFillRect(context, rect);
    UIImage *theImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return theImage;
}

-(void)create
{
    _arrKtvInfo = [[NSMutableArray alloc] init];
    
    self.mUiScanGift = [[UIButton alloc] initWithFrame:CGRectMake(0, 10, self.mainContentView.frame.size.width, 44)];
    [self.mUiScanGift setContentEdgeInsets:UIEdgeInsetsMake(0, 60, 0, 0)];
    [self.mUiScanGift setContentHorizontalAlignment:UIControlContentHorizontalAlignmentLeft];
    [self.mUiScanGift setBackgroundImage:[self createImageWithColor:[UIColor whiteColor]]forState:UIControlStateNormal];
    [self.mUiScanGift setBackgroundImage:[self createImageWithColor:UIColorFromRGB(0xe0e0e0)] forState:UIControlStateHighlighted];
    [self.mUiScanGift setTitle:@"扫描二维码兑换" forState:UIControlStateNormal];
    [self.mUiScanGift setTitleColor:UIColorFromRGB(0x333333) forState:UIControlStateNormal];
    [self.mUiScanGift addTarget:self action:@selector(onScanGift) forControlEvents:UIControlEventTouchUpInside];
    [self.mainContentView addSubview:self.mUiScanGift];
    
    UIImageView* ui_scan_imgview = [[UIImageView alloc] initWithFrame:CGRectMake(20, 18, 28, 28)];
    [ui_scan_imgview setImage:[UIImage imageNamed:@"scan_gifte" ]];
    [self.mainContentView addSubview:ui_scan_imgview];
    
    UIImageView* ui_imgview = [[UIImageView alloc] initWithFrame:CGRectMake(self.mainContentView.frame.size.width - 17 - 8, 25, 8, 14)];
    [ui_imgview setImage:[UIImage imageNamed:@"list_pic_more.png"]];
    [self.mainContentView addSubview:ui_imgview];
    
    _lblTip = [[UILabel alloc] initWithFrame:CGRectMake(20, 64, 100, 30)];
    _lblTip.text = @"请选择所在的KTV";
    _lblTip.font = [UIFont systemFontOfSize:14];
    _lblTip.textColor = UIColorFromRGB(0xa8a8a8);
    _lblTip.backgroundColor = [UIColor clearColor];
    [_lblTip sizeToFit];
    [self.mainContentView addSubview:_lblTip];
    
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 88, self.mainContentView.frame.size.width, self.mainContentView.frame.size.height - 88)];
    _tableView.dataSource = self;
    _tableView.delegate = self;
    _tableView.backgroundColor = [UIColor whiteColor];
    _tableView.tableFooterView = [[UIView alloc] init];
    _tableView.separatorStyle= UITableViewCellSeparatorStyleSingleLine;
    [self.mainContentView addSubview:_tableView];
    
    UIImageView *line1=[[UIImageView alloc]initWithFrame:CGRectMake(0, 10, self.mainContentView.frame.size.width, 1)];
    line1.backgroundColor=UIColorFromRGB(0xc6c6c7);
    [self.mainContentView addSubview:line1];
    
    UIImageView *line2=[[UIImageView alloc]initWithFrame:CGRectMake(0, 54, self.mainContentView.frame.size.width, 1)];
    line2.backgroundColor=UIColorFromRGB(0xc6c6c7);
    [self.mainContentView addSubview:line2];
    
    UIImageView *line3=[[UIImageView alloc]initWithFrame:CGRectMake(0, 87, self.mainContentView.frame.size.width, 1)];
    line3.backgroundColor=UIColorFromRGB(0xc6c6c7);
    [self.mainContentView addSubview:line3];
}

#pragma mark alert delegate
-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 1) {
        [SVProgressHUD showWithStatus:@"正在兑换"];
        int number = [[self.mMapData objectForKey:@"giftnum"] intValue];
        NSString *strUrl = [PKtvAssistantAPI getGetGiftExchangeUrl:[UserSessionManager getInstance].currentUser.uid giftID:[self.mMapData objectForKey:@"giftid"] ktvID:[self.mMapData objectForKey:@"ktvid"] num:number];
        NSURL *url = [[NSURL alloc]initWithString:strUrl];
        NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url];
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
        AFHTTPRequestOperation * operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            NSDictionary *html = [PCommonUtil NSDictionaryFromJSONData2:responseObject];
            NSString *status = [html objectForKey:@"status"];
            NSString *msg = [html objectForKey:@"msg"];
            if ([status compare:@"1"]==NSOrderedSame)
            {
                long money = [[html objectForKey:@"result"] intValue];
                [CommenTools showAlertViewWithMessage: @"兑换成功,请用兑换码去前台兑换(兑换码在兑换记录查看)"];
                
                [[UserSessionManager getInstance] updateCurrentUserGold: money];
                ExchangeHistoryController *controller =[[ExchangeHistoryController alloc] init];
                [self.navigationController pushViewController:controller animated:true];
                [SVProgressHUD dismiss];
            }
            else
            {
                [CommenTools showAlertViewWithMessage:[NSString stringWithFormat:@"兑换失败:%@",msg]];
                [SVProgressHUD dismiss];
            }
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            [SVProgressHUD dismiss];
        }];
        [operation start];
    }
}

#pragma mark - ZBarReaderDelegate
- (void) imagePickerController: (UIImagePickerController*) reader didFinishPickingMediaWithInfo: (NSDictionary*) info
{
    id<NSFastEnumeration> results = [info objectForKey: ZBarReaderControllerResults];
    ZBarSymbol *symbol = nil;
    for(symbol in results)
        break;
    NSString *resultText = symbol.data;
    //解决扫描的二维码是汉字生成时会乱码的问题
    if ([resultText canBeConvertedToEncoding:NSShiftJISStringEncoding])
    {
        resultText = [NSString stringWithCString:[resultText cStringUsingEncoding:NSShiftJISStringEncoding] encoding:NSUTF8StringEncoding];
    }
    PLog(@"resultText: %@",resultText);
    [reader dismissViewControllerAnimated:YES completion:nil];
    
    self.mMapData = [[NSMutableDictionary alloc] init];
    NSArray* strKeyValues = [resultText componentsSeparatedByString:@";"];
    for(NSString* str in strKeyValues)
    {
        NSArray* strKeyValue = [str componentsSeparatedByString:@":"];
        if(strKeyValue == nil || strKeyValue.count != 2)
        {
            [[iToast makeText:@"扫描的二维码不正确"] show];
            return ;
        }
        [self.mMapData setObject:[strKeyValue objectAtIndex:1] forKey:[strKeyValue objectAtIndex:0]];
    }
    
    if(self.mMapData.count == 0)
    {
        [[iToast makeText:@"扫描的二维码不正确"] show];
        return ;
    }
    
    NSString* payType = [self.mMapData objectForKey:@"scanpay-type"];
    if([payType compare:@"1"] != NSOrderedSame)
    {
        [[iToast makeText:@"请选择兑换礼物的二维码"] show];
        return ;
    }
    
    self.mAlertExchange  = [[UIAlertView alloc] initWithTitle:nil message:[[NSString alloc] initWithFormat:@"确认要兑换%@%@%@(%@金币)?",[self.mMapData objectForKey:@"giftnum"],[self.mMapData objectForKey:@"giftunit"],[self.mMapData objectForKey:@"giftname"],[self.mMapData objectForKey:@"gold"]] delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
    [self.mAlertExchange show];
}

-(void)onScanGift
{
    //扫描兑换礼物
    self.mViewCReader = [ZBarReaderViewController new];
    //设备尺寸 3.5/4.0英寸
    self.mViewCReader.view.frame = CGRectMake(0, 0, 320, 480+(iPhone5?88:0));
    self.mViewCReader.readerDelegate = self;
    [self.mViewCReader setShowsZBarControls:NO]; // 禁止 官方的取消和帮助说明按钮
    self.mViewCReader.supportedOrientationsMask = ZBarOrientationMaskAll;
    self.mViewCReader.cameraFlashMode = UIImagePickerControllerCameraFlashModeOff;
    ZBarImageScanner *scanner = self.mViewCReader.scanner;
    [scanner setSymbology: ZBAR_I25 config: ZBAR_CFG_ENABLE to: 0];
    
    // 取消二维码扫描
    UIButton *cancel = [UIButton buttonWithType:UIButtonTypeCustom];
    cancel.frame = CGRectMake(20, self.mViewCReader.view.frame.size.height-60, 60, 60);
    cancel.showsTouchWhenHighlighted = YES;
    cancel.backgroundColor = [UIColor clearColor];
    [cancel setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
    [cancel setTitle:@"取消" forState:UIControlStateNormal];
    [cancel addTarget:self action:@selector(cancelAction:) forControlEvents:UIControlEventTouchUpInside];
    [self.mViewCReader.view addSubview:cancel];
    
    // 是否开启闪关灯照明
    UIButton *flash = [UIButton buttonWithType:UIButtonTypeCustom];
    flash.frame = CGRectMake(_MainScreen_Width-60-20, self.mViewCReader.view.frame.size.height-60, 60, 60);
    flash.showsTouchWhenHighlighted = YES;
    flash.backgroundColor = [UIColor clearColor];
    flash.tag = 0;
    [flash setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
    [flash setTitle:@"照明" forState:UIControlStateNormal];
    [flash addTarget:self action:@selector(flashAction:) forControlEvents:UIControlEventTouchUpInside];
    [self.mViewCReader.view addSubview:flash];
    
    [self presentViewController:self.mViewCReader animated:YES completion:nil];
}

#pragma mark -  取消二维码扫描

- (void)cancelAction:(UIButton *)button
{
    [self.mViewCReader dismissViewControllerAnimated:YES completion:nil];
}

#pragma mark -  是否开启闪关灯照明

- (void)flashAction:(UIButton *)button
{
    if (button.tag == 0)
    {
        [button setTag:1];
        self.mViewCReader.cameraFlashMode = UIImagePickerControllerCameraFlashModeOn; // 后置灯光模式开
    }
    else
    {
        [button setTag:0];
        self.mViewCReader.cameraFlashMode = UIImagePickerControllerCameraFlashModeOff; // 后置灯光模式关
    }
}

-(void)showHistory
{
    ExchangeHistoryController *controller = [[ExchangeHistoryController alloc] init];
    [self.navigationController pushViewController:controller animated:true];
}

-(void)loadData
{
    isLoad = true;
    NSString *strUrl= [PKtvAssistantAPI getGetKtvInfoUrl:1 size:20];
    NSURL *url =[[NSURL alloc] initWithString:strUrl];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url];
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation * operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary *html = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        NSString *status = [html objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [html objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"ktvlist"];
            for (NSDictionary *dic in dataList) {
                KTVInfo *info = [KTVInfo initWithDictionary:dic];
                [_arrKtvInfo addObject:info];
            }
            [_tableView reloadData];
        }
        else
        {
            
        }
        isLoad = NO;
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        isLoad = NO;
    }];
    [operation start];
}


- (void)viewDidLoad
{
    [super viewDidLoad];
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.navView.titleLabel.text = @"礼物兑换";
    self.navView.btnHistory.hidden = NO;
    [self.navView.btnHistory addTarget:self action:@selector(showHistory) forControlEvents:UIControlEventTouchUpInside];
    self.view.backgroundColor = UIColorFromRGB(0xf8f8f9);
    [self create];//创建布局
    [self loadData];
}

#pragma mark table delegate
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _arrKtvInfo.count;
}

-(UITableViewCell *) tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    KTVInfoCell *cell = [tableView dequeueReusableCellWithIdentifier:@"cell"];
    if (cell == nil) {
        cell = [[KTVInfoCell alloc]  initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
        cell.backgroundColor = [UIColor clearColor];
    }
    cell.model = [_arrKtvInfo objectAtIndex:indexPath.row];
    return  cell;
}

-(CGFloat) tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    //return 60;
    KTVInfo *info = [_arrKtvInfo objectAtIndex:indexPath.row];
    CGSize size = [info.ktvAddress sizeWithFont:[UIFont boldSystemFontOfSize:14.0f]
                                  constrainedToSize:CGSizeMake(_MainScreen_Width-45, MAXFLOAT)
                                      lineBreakMode:UILineBreakModeWordWrap];
        
    return size.height + 41;
}

-(void) tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    KTVInfo *info = [_arrKtvInfo objectAtIndex:indexPath.row];
    GIftInfoController *controller = [[GIftInfoController alloc] init:info.ktvID ktvname:info.ktvName];
    [self.navigationController pushViewController:controller animated:true];
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
}

-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:true];
}


@end
