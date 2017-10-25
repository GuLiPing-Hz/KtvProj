//
//  MessageViewController.m
//  SinaKTVAide

// 消息中心界面

//  Created by Li Pan on 13-12-13.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "MessageViewController.h"
#import "MessageDetailViewController.h"
#import "MessageCell.h"
#import "PKtvAssistantAPI.h"
#import "TopSong.h"
#import "iToast.h"
#import "CMActionSheet.h"
#import "KTVInputSecretController.h"
#import "RegexKitLite.h"
#import "KTVAppDelegate.h"
//#import "BPush.h"
#import "CommenTools.h"
#import "SystemMessageDetailViewController.h"
#import "xingguang.h"
#import "NetUtilCallBack.h"
#import "iosdatadefine.h"
#import "AFHTTPRequestOperation.h"
#import "com_ios_NetUtil.h"
#import "UserSessionManager.h"
#import "KTVBaseConfig.h"
@interface MessageViewController ()

@end

@implementation MessageViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.isRedCircle = YES;
    }
    return self;
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - viewWillAppear

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self flashUIWithRoom]; // 退出包厢刷新UI
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(disconnRoom:) name:KTVASSISTANT_CLOSEROOM_NOTIFICATION object:nil];
}

-(void)disconnRoom:(NSNotification *)notification
{
    [self performSelectorOnMainThread:@selector(closeRoom) withObject:nil waitUntilDone:YES];
}

-(void)closeRoom
{
    DisConnectRoom();
    [UserSessionManager clearCurrentRoomInfo];
    [self flashUIWithRoom]; // 退出包厢刷新room
}

#pragma mark - viewDidLoad

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self startRedCircle];
	self.navView.leftButton.hidden = NO;
    self.navView.hidden = NO;
    self.navView.titleLabel.text = @"系统消息";
    self.view.backgroundColor = UIColorFromRGB(0xe1e2e6);
    // 初始化列表数组
    if (self.arrMessage == nil) {
        self.arrMessage = [[NSMutableArray alloc] init];
    }
    
    [self creatMessageListTableView];
    
    [self creatLoadingDataFail];  // 请求失败展示失败logo
    
    [self creatNoMessageLabel];  // 无消息展示
    
    self.toolBar.hidden = NO;
    self.joinButton.hidden = NO;
    // 进入包厢
    [self.joinButton addTarget:self action:@selector(joinRoom:) forControlEvents:UIControlEventTouchUpInside];
    
    // 退出包厢
    [self.power addTarget:self action:@selector(exit:) forControlEvents:UIControlEventTouchUpInside];
    
    [self flashUIWithRoom]; // 退出包厢刷新UI
    
    [self getLoadTableViewListData];  // 进来就载入数据请求
}

#pragma mark -  UIActionSheet 进入包厢

- (void)joinRoom:(UIButton *)btn

{
    //********* 自定义表单 *********
    CMActionSheet *actionSheet = [[CMActionSheet alloc] init];
    actionSheet.title = @"请先进入包厢";
    [actionSheet addSeparator];
    
    [actionSheet addButtonWithTitle:@"扫描包厢二维码" type:CMActionSheetButtonTypeWhite block:^{
        PLog(@"扫描二维码进包厢");
        [self openAction];
        
    }];
    [actionSheet addButtonWithTitle:@"输入包厢密码" type:CMActionSheetButtonTypeBlue block:^{
        PLog(@"输入房间密码进包厢");
        KTVInputSecretController *input = [[KTVInputSecretController alloc] init];
        [self.navigationController pushViewController:input animated:YES];
    }];
    [actionSheet addButtonWithTitle:@"取消" type:CMActionSheetButtonTypeRed block:^{
        PLog(@"取消");
    }];
    [actionSheet present];
    //********* ********* *********
}


#pragma mark - 退出包厢

- (void)exit:(UIButton *)button
{
    _altExitRoom = [[UIAlertView alloc] initWithTitle:@"温馨提示" message:@"你确定要退出包厢吗?" delegate:self cancelButtonTitle:@"点错了" otherButtonTitles:@"好", nil];
    [_altExitRoom show];
}

#pragma mark - 退出包厢/注销账号 UIAlertView delegate

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 1)
    {
        /**
         退出包厢
         */
        if (alertView == _altExitRoom)
        {
            if(![[Reachability reachabilityForInternetConnection] isReachable])
            {
                [self showNetwork];
                return;
            }
            //[SVProgressHUD showWithStatus:@"正在退出..." maskType:SVProgressHUDMaskTypeBlack];
            long int userID = [UserSessionManager getInstance].currentUser.uid;
            NSString *password  = [UserSessionManager getInstance].currentRoomInfo.roomPassword;
            
            NSString *roomId = [UserSessionManager getInstance].currentRoomInfo.roomId;
            NSString *encodeUrl = [PKtvAssistantAPI getRoomLogoutUrl:userID roomID:roomId password:password];
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
                     PLog(@"退出包厢成功");
                     DisConnectRoom();
//                     NSString * tag = [[UserSessionManager getInstance].currentRoomInfo.roomPassword lowercaseString];
//                     //PLog(@"TAG-->>%@",tag);
//                     [BPush delTag:tag];
//                     [SVProgressHUD dismissWithSuccess:@"成功退出包厢" afterDelay:1.0];
//                     [UserSessionManager clearCurrentRoomInfo];
//                     
//                     [self flashUIWithRoom]; // 退出包厢刷新room
                 }
                 else
                 {
                     PLog(@"退出包厢失败");
                     //[SVProgressHUD dismissWithError:@"退出包厢失败"];
                     NSString *msg = [results objectForKey:@"msg"];
                     NSString *errorCode = [results objectForKey:@"errorcode"];
                     PLog(@"doLoginRoomWithUser failed...errorcode:%@(%@)", errorCode, msg);
                 }
                 
                 
             }
                                             failure:^(AFHTTPRequestOperation *operation, NSError *error)
             {
                 PLog(@"doLogin failed...");
                 //[SVProgressHUD dismiss];
                 //[CommenTools showNetworkDisconnectedAlertView];
             }];
            [operation start];
            
            NSString * tag = [password lowercaseString];
            //PLog(@"TAG-->>%@",tag);
            //[BPush delTag:tag withCompleteHandler:nil];
            [UserSessionManager clearCurrentRoomInfo];
            [self flashUIWithRoom]; // 退出包厢刷新room

        }
    }
    
    [alertView dismissWithClickedButtonIndex:0 animated:YES];
}

#pragma mark - 开启二维码扫描

- (void)openAction
{
    _reader = [ZBarReaderViewController new];
    //设备尺寸 3.5/4.0英寸
    _reader.view.frame = CGRectMake(0, 0, _MainScreen_Width, _MainScreen_Height+(iPhone5?88:0));
    _reader.readerDelegate = self;
    [_reader setShowsZBarControls:NO]; // 禁止 官方的取消和帮助说明按钮
    _reader.supportedOrientationsMask = ZBarOrientationMaskAll;
    _reader.cameraFlashMode = UIImagePickerControllerCameraFlashModeOff;
    ZBarImageScanner *scanner = _reader.scanner;
    [scanner setSymbology: ZBAR_I25
                   config: ZBAR_CFG_ENABLE
                       to: 0];
    
    // 取消二维码扫描
    UIButton *cancel = [UIButton buttonWithType:UIButtonTypeCustom];
    cancel.frame = CGRectMake(20, _reader.view.frame.size.height-60, 60, 60);
    cancel.showsTouchWhenHighlighted = YES;
    cancel.backgroundColor = [UIColor clearColor];
    [cancel setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
    [cancel setTitle:@"取消" forState:UIControlStateNormal];
    [cancel addTarget:self action:@selector(cancelAction:) forControlEvents:UIControlEventTouchUpInside];
    [_reader.view addSubview:cancel];
    
    // 是否开启闪关灯照明
    UIButton *flash = [UIButton buttonWithType:UIButtonTypeCustom];
    flash.frame = CGRectMake(_MainScreen_Width-60-20, _reader.view.frame.size.height-60, 60, 60);
    flash.showsTouchWhenHighlighted = YES;
    flash.backgroundColor = [UIColor clearColor];
    flash.tag = 0;
    [flash setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
    [flash setTitle:@"照明" forState:UIControlStateNormal];
    [flash addTarget:self action:@selector(flashAction:) forControlEvents:UIControlEventTouchUpInside];
    [_reader.view addSubview:flash];
    
    [self presentModalViewController: _reader animated: YES];
}

#pragma mark -  取消二维码扫描

- (void)cancelAction:(UIButton *)button
{
    [_reader dismissModalViewControllerAnimated:YES];
}

#pragma mark -  是否开启闪关灯照明

- (void)flashAction:(UIButton *)button
{
    if (button.tag == 0)
    {
        [button setTag:1];
        _reader.cameraFlashMode = UIImagePickerControllerCameraFlashModeOn; // 后置灯光模式开
    }
    else
    {
        [button setTag:0];
        _reader.cameraFlashMode = UIImagePickerControllerCameraFlashModeOff; // 后置灯光模式关
    }
}

#pragma mark - UIImagePickerController delegate

- (void) imagePickerController: (UIImagePickerController*) reader
 didFinishPickingMediaWithInfo: (NSDictionary*) info
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
    [reader dismissModalViewControllerAnimated: YES];
    
    if (![resultText isMatchedByRegex:@"^[a-zA-Z0-9]+$"] || [resultText length] != 6)
    {
        PLog(@"字符串不符合标准！！！");
        [[iToast makeText:@"该密码密码不符合密码标准，请重试"] show];
        return;
    }
    else
    {
        self.keyWord = resultText;
        [self login];
    }
}

#pragma mark - 扫描二维码登录进入包厢

-(void)login
{
    if(![[Reachability reachabilityForInternetConnection] isReachable])
    {
        [self showNetwork];
        return;
    }
    [SVProgressHUD showWithStatus:@"进入房间中,请稍后" maskType:SVProgressHUDMaskTypeClear];
    
    User * user = [UserSessionManager getInstance].currentUser;
    long int userID = user.uid;
    NSString *encodeUrl =  [PKtvAssistantAPI getRoomLoginUrlEx:userID pwd:user.pwd password:self.keyWord];
    
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
             NSInteger gold = [[result objectForKey:@"gold"] integerValue];
             if ([UserSessionManager getInstance].currentUser.gold != gold && gold > 0) {
                 [[UserSessionManager getInstance] updateCurrentUserGold:gold];
             }
             RoomInfo *roominfo = [RoomInfo initWithDictionary:result];
             roominfo.roomPassword = _keyWord;
             //[roominfo Log];
             [[UIApplication sharedApplication] cancelAllLocalNotifications]; // 撤销所有的Notification
             UILocalNotification *localNotification = [[UILocalNotification alloc] init];
             // 设置notification的属性
             localNotification.fireDate = [[NSDate new] dateByAddingTimeInterval:roominfo.periodsTime]; //出发时间
             localNotification.alertBody = @"您的包厢已到期，请续费或欢迎下次光临"; // 消息内容
             localNotification.soundName = UILocalNotificationDefaultSoundName; // 触发消息时播放的声音
             //设置随Notification传递的参数
             NSDictionary *infoDict = [NSDictionary dictionaryWithObjectsAndKeys:@"reminder", @"notificationId", self.keyWord, @"roomSecret", nil];
             localNotification.userInfo = infoDict;
             [[UIApplication sharedApplication] scheduleLocalNotification:localNotification];
             [UserSessionManager getInstance].currentRoomInfo = roominfo;
             [UserSessionManager getInstance].isLoggedIn = YES;
             NSString * tag = [roominfo.roomPassword lowercaseString];
             //PLog(@"TAG-->>%@",tag);
             //[BPush setTag:tag withCompleteHandler:nil];
             [SVProgressHUD dismiss];
             
             NetUtilCallBack* pNetUtil = [NetUtilCallBack getInstance];
             [pNetUtil setRoomId:[roominfo.xgRoomID intValue]];
             [pNetUtil doCurrentWork:TransactionRoomLogin listener:^(BOOL success) {
                 if(success)
                 {
                     [self flashUIWithRoom]; // 进入包厢或者退出包厢刷新界面
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
         [SVProgressHUD dismiss];
         PLog(@"doLogin failed...");
         [CommenTools showNetworkDisconnectedAlertView];
     }];
    [operation start];
}

#pragma mark - 进入包厢或者退出包厢刷新界面

- (void)flashUIWithRoom

{
    if ([UserSessionManager isRoomAlreadyLogin])   // 已进入包厢
    {
        NSString *roomName = [UserSessionManager getInstance].currentRoomInfo.roomName;
        self.inRoomNumber.text = [NSString stringWithFormat:@"所在包厢: %@",roomName];
        self.joinButton.hidden = YES;
        self.power.hidden = NO;
        self.inRoomNumber.hidden = NO;
    }
    else  // 未进入包厢
    {
        self.joinButton.hidden = NO;
        self.power.hidden = YES;
        self.inRoomNumber.hidden = YES;
    }
}


#pragma mark - 进来就载入数据请求
/**
	 载入数据请求
  */
- (void)getLoadTableViewListData
{
    long int userID = [UserSessionManager getInstance].currentUser.uid;
    NSString *roomId = [UserSessionManager getInstance].currentRoomInfo.roomId;
    NSString *encodeUrl = [PKtvAssistantAPI getGetMessageListUrl:userID roomId:roomId since:(0) before:0 num:20];
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
            NSArray *dataList = [result objectForKey:@"msglist"];
            if (self.arrMessage.count > 0)
            {
                [self.arrMessage removeAllObjects]; // 删除之前的列表信息，重新加载数据
            }
            
            for (int i=0; i < [dataList count]; i++)
            {
                Message *msg = [Message initWithDictionary:[dataList objectAtIndex:i]];
                //[msg log];
                [self.arrMessage addObject:msg];
                // 消息已删
//                NSString *documentPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
//                NSString *msgDeletePath = [documentPath stringByAppendingPathComponent:@"msgAlreadyDelete.plist"];
//                NSMutableArray *arrmsgDelete = [NSMutableArray arrayWithContentsOfFile:msgDeletePath];
//                if (arrmsgDelete) // 本地存在删除的消息msgID
//                {
//                    PLog(@"arrmsgDelete-----= %@",arrmsgDelete);
//                    
//                    PLog(@"self.arrMessage = %@",self.arrMessage);
//                    
//                    // 显示无消息标签
//                    if (self.arrMessage.count == 0)
//                    {
//                        _noMessageLabel.hidden = NO;
//                    }
//                }
//                [self.arrMessage removeObject:[arrmsgDelete objectAtIndex:msg.msgID]];
            }
            
             [self stopRedCircle];
             
             //停止加载，弹回下拉
             [self doneLoadingTableViewData];
             
             [self.messageListTable reloadData]; // 刷新列表
             
             // 显示无网络logo
             if ( (_egoLogo.hidden == NO) && (_labNotice.hidden == NO))
             {
                 _egoLogo.hidden = YES;
                 _labNotice.hidden = YES;
             }
             // 显示无消息
             if (_noMessageLabel.hidden == NO)
             {
                 _noMessageLabel.hidden = YES;
             }
         }
         else
         {
            [self stopRedCircle];
             
            //停止加载，弹回下拉
            [self doneLoadingTableViewData];
             
             if (_arrMessage.count == 0)
             {
                 _egoLogo.hidden = NO;  // 请求失败 展示 失败 logo 不隐藏
                 _labNotice.hidden = NO;
                 
                 _noMessageLabel.hidden = NO;
             }
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            [CommenTools showAlertViewWithErrorMessage:msg];
            PLog(@"doGetMessageList failed...errorcode:%@(%@)", errorCode, msg);
         }
     }
     failure:^(AFHTTPRequestOperation *operation, NSError *error)
     {
         [self stopRedCircle];
         
         //停止加载，弹回下拉
         [self doneLoadingTableViewData];
         if (_arrMessage.count == 0)
         {
             _egoLogo.hidden = NO;  // 请求失败 展示 失败 logo 不隐藏
             _labNotice.hidden = NO;
         }
         PLog(@"--failed...");
     }];
         
    [operation start];
    
}

- (void)creatLoadingDataFail
{
    [self.egoLogo setImage:[UIImage imageNamed:@"ktv_jiazai.png"]];
    _egoLogo.frame = CGRectMake(135, 150, 51, 48);
    _egoLogo.hidden = YES;
    [self.mainContentView addSubview:_egoLogo];
    
    _labNotice = [[UILabel alloc] initWithFrame:CGRectMake(62, 210, 250, 35)];
    _labNotice.backgroundColor = [UIColor clearColor];
    _labNotice.text = @"没有获取到数据哟,请稍后再试!";
    _labNotice.font = [UIFont systemFontOfSize:15.0f];
    _labNotice.textColor = UIColorFromRGB(0x9e9d9d);
    _labNotice.shadowColor = [UIColor whiteColor];
    _labNotice.shadowOffset = CGSizeMake(0, 1);
    _labNotice.hidden = YES;
    [self.mainContentView addSubview:_labNotice];
}


- (void)creatMessageListTableView
{
    if (_messageListTable == nil)
    {
        _messageListTable = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, _MainScreen_Height+20-64-50) style:UITableViewStylePlain];
        _messageListTable.backgroundColor = UIColorFromRGB(0xe1e2e6);
        _messageListTable.dataSource = self;
        _messageListTable.delegate = self;
        _messageListTable.rowHeight = 50;
        if ([_messageListTable respondsToSelector:@selector(setSeparatorInset:)]) {
            [_messageListTable setSeparatorInset:UIEdgeInsetsZero]; // 分隔线延伸从最左边开始
        }
        _messageListTable.hidden = NO;
        _messageListTable.userInteractionEnabled = YES;
        _messageListTable.tableFooterView = [[UIView alloc] init]; // 多余的cell屏蔽掉
        [self.mainContentView insertSubview:_messageListTable belowSubview:self.redCircle];
    }
    
    if (_refreshHeaderView == nil)
    {
		EGORefreshTableHeaderView *headView = [[EGORefreshTableHeaderView alloc] initWithFrame:CGRectMake(0.0f, 0.0f - self.messageListTable.bounds.size.height, self.view.frame.size.width, self.messageListTable.bounds.size.height)];
		headView.delegate = self;
		[self.messageListTable addSubview:headView];
		_refreshHeaderView = headView;
	}
	
	//  最后一次加载的时间
	[_refreshHeaderView refreshLastUpdatedDate];
}

- (void)creatNoMessageLabel
{
    if (_noMessageLabel == nil)
    {
        _noMessageLabel = [[UILabel alloc] initWithFrame:CGRectMake(112, 230, 250, 35)];
        _noMessageLabel.backgroundColor = [UIColor clearColor];
        _noMessageLabel.text = @"您尚未消息哟!";
        _noMessageLabel.font = [UIFont systemFontOfSize:15.0f];
        _noMessageLabel.textColor = UIColorFromRGB(0x9e9d9d);
        _noMessageLabel.shadowColor = [UIColor whiteColor];
        _noMessageLabel.shadowOffset = CGSizeMake(0, 1);
        _noMessageLabel.hidden = YES;
        [self.mainContentView addSubview:_noMessageLabel];
    }
}

#pragma mark UITableViewDataSource

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [self.arrMessage count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"MessageCell";
    
    MessageCell *cell = (MessageCell *)[tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil)
    {
        cell = [[MessageCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    cell.selectedBackgroundView = [[UIView alloc] initWithFrame:cell.bounds];
    cell.selectedBackgroundView.backgroundColor = UIColorFromRGB(0xd4d4d4);
    Message *msg = [_arrMessage objectAtIndex:indexPath.row];
    switch (msg.msgType)
    {
        case 1:
            cell.scoreLabel.text = @"系统消息";
            break;
        case 2:
            cell.scoreLabel.text = @"歌曲上榜";
            break;
        case 3:
            cell.scoreLabel.text = @"其他消息";
            break;
        default:
            break;
    }
    cell.labMessage.text = msg.content;
    cell.labDate.text = [cell getDate:msg.date];
    
    // 消息已读
    NSString *documentPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *msgIDPath = [documentPath stringByAppendingPathComponent:@"msgAlreadyRead.plist"];
    NSMutableArray *arrmsgRead = [NSMutableArray arrayWithContentsOfFile:msgIDPath];
    if (arrmsgRead)
    {
        if ([arrmsgRead containsObject:[NSNumber numberWithLong:msg.msgID]]) // 匹配有msgID的不显示红点,字体变灰
        {
            cell.badgeImage.hidden = YES;
            cell.labMessage.textColor = UIColorFromRGB(0xacacac);
        }
        else
        {
            cell.badgeImage.hidden = NO;
            cell.labMessage.textColor = UIColorFromRGB(0x2f2f2f);
        }
    }
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    Message *msg = [_arrMessage objectAtIndex:indexPath.row];
    
    if (msg.msgType == 1)  // 系统消息
    {
        SystemMessageDetailViewController *sys = [[SystemMessageDetailViewController alloc] init];
        sys.SystemMessageContent = msg.content;
        [self.navigationController pushViewController:sys animated:YES];
    }
    else if (msg.msgType == 2)  // 上榜消息
    {
        MessageDetailViewController *detail = [[MessageDetailViewController alloc] init];
        detail.msgID = msg.msgID;
        detail.workID = msg.workID;
        detail.point = msg.point;
        detail.msgType = msg.msgType;
        detail.content = msg.content;
        detail.date = msg.date;
        [self.navigationController pushViewController:detail animated:YES];
    }
    
    MessageCell *selectCell;
    // 点击cell后 已读
    
    // 保存到本地，如果没有,就写入本地，本地有就先取出来在加进去新的 再写入本地
    NSString *documentPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *msgIDPath = [documentPath stringByAppendingPathComponent:@"msgAlreadyRead.plist"];
    NSMutableArray *arrmsg = [NSMutableArray arrayWithContentsOfFile:msgIDPath];
    if (arrmsg)
    {
        [arrmsg addObject:[NSNumber numberWithLong:msg.msgID]];
        [arrmsg writeToFile:msgIDPath atomically:NO];
    }
    else
    {
        NSMutableArray *arrmsgID;
        if (arrmsgID == nil)
        {
            arrmsgID = [[NSMutableArray alloc] init];
        }
        [arrmsgID addObject:[NSNumber numberWithLong:msg.msgID]];
        [arrmsgID writeToFile:msgIDPath atomically:NO];
    }
    
    selectCell = (MessageCell *)[tableView cellForRowAtIndexPath:indexPath];
    selectCell.badgeImage.hidden = YES;
    [selectCell.badgeImage removeFromSuperview];
    selectCell.labMessage.textColor = UIColorFromRGB(0xacacac);
    
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    //Message *msg = [_arrMessage objectAtIndex:indexPath.row];
    if (editingStyle == UITableViewCellEditingStyleDelete)
    {
        // 删除的消息保存到本地
//        NSString *documentPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
//        NSString *msgDeletePath = [documentPath stringByAppendingPathComponent:@"msgAlreadyDelete.plist"];
//        NSMutableArray *arrmsgDelete = [NSMutableArray arrayWithContentsOfFile:msgDeletePath];
//        if (arrmsgDelete)
//        {
//            [arrmsgDelete removeAllObjects];
//            [arrmsgDelete addObject:[NSNumber numberWithLong:msg.msgID]];
//            [arrmsgDelete writeToFile:msgDeletePath atomically:NO];
//        }
//        else
//        {
//            NSMutableArray *arrmsgIDDelete;
//            if (arrmsgIDDelete == nil)
//            {
//                arrmsgIDDelete = [[NSMutableArray alloc] init];
//            }
//            [arrmsgIDDelete addObject:[NSNumber numberWithLong:msg.msgID]];
//            [arrmsgIDDelete writeToFile:msgDeletePath atomically:NO];
//        }
        // 删除数据源加动画
        [self.arrMessage removeObjectAtIndex:indexPath.row];
        [self.messageListTable deleteRowsAtIndexPaths:[NSMutableArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationAutomatic];
        [self.messageListTable reloadData];
        // 显示无消息标签
        if (self.arrMessage.count == 0)
        {
            _noMessageLabel.hidden = NO;
        }
    }
}


//- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath
//{
//    return UITableViewCellEditingStyleDelete;
//}
//
//- (NSString *)tableView:(UITableView *)tableView titleForDeleteConfirmationButtonForRowAtIndexPath:(NSIndexPath *)indexPath
//{
//    return @"删除";
//}
//
//- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
//{
//    return YES;
//}
//
//- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer
//{
//    return YES;
//}
//
//- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
//{
//    return YES;
//}

#pragma mark -
#pragma mark Data Source Loading / Reloading Methods

- (void)reloadTableViewDataSource
{
	_reloading = YES;
}

- (void)doneLoadingTableViewData
{
	_reloading = NO;
	[_refreshHeaderView egoRefreshScrollViewDataSourceDidFinishedLoading:self.messageListTable];
}


#pragma mark -
#pragma mark UIScrollViewDelegate Methods

- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
	[_refreshHeaderView egoRefreshScrollViewDidScroll:scrollView];
}

- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate
{
	[_refreshHeaderView egoRefreshScrollViewDidEndDragging:scrollView];
}


#pragma mark -
#pragma mark EGORefreshTableHeaderDelegate Methods
//下拉到一定距离，手指放开时调用
- (void)egoRefreshTableHeaderDidTriggerRefresh:(EGORefreshTableHeaderView*)view
{
    [self reloadTableViewDataSource];

    [self getLoadTableViewListData]; // 加载数据

}

- (BOOL)egoRefreshTableHeaderDataSourceIsLoading:(EGORefreshTableHeaderView*)view
{
	return _reloading; // should return if data source model is reloading
}

//取得下拉刷新的时间
- (NSDate*)egoRefreshTableHeaderDataSourceLastUpdated:(EGORefreshTableHeaderView*)view
{
	return [NSDate date]; // should return date data source was last changed
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

@end
