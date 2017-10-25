//
//  SearchResultViewController.m
//  SinaKTVAide
//  加包厢登录
//  Created by WangBingJian on 13-12-18.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "SearchResultViewController.h"
#import "PKtvAssistantAPI.h"
#import "SongsBookModel.h"
#import "KTVAppDelegate.h"
//add
#import "CMActionSheet.h"
#import "KTVInputSecretController.h"
#import "RegexKitLite.h"
#import "iToast.h"
//#import "BPush.h"
#import "xingguang.h"
#import "NetUtilCallBack.h"
#import "com_ios_NetUtil.h"
#import "DDMenuController.h"

@interface SearchResultViewController ()

@end

@implementation SearchResultViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

#pragma mark addView

- (void)setNavigationView{
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.navView.titleLabel.text = @"搜索";
//    self.navView.rightButton.hidden = NO;
//    self.navView.badgeButton.hidden = NO;
}

- (void)creatSearchResultList{
    _searchResultList = [[UITableView alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height) style:UITableViewStylePlain];
    _searchResultList.backgroundColor = UIColorFromRGB(0xf0f1f5);
    
    _searchResultList.delegate = self;
    _searchResultList.dataSource = self;
    
    _searchResultList.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    
    [self.mainContentView addSubview:_searchResultList];
    
    
    
}


- (void)creatFlashView{
    self.flashCD = [[UIImageView alloc]init];
    self.flashCD.frame = CGRectMake(0, 0, 24, 24);
    [self.flashCD setImage:[UIImage imageNamed:@"pic_cd.png"]];
    [self.flashCD setHidden:YES];
    
    [self.mainContentView addSubview:self.flashCD];
    [self.mainContentView bringSubviewToFront:self.flashCD];
    
}

- (void)cdViewFlash{
    self.flashCD.frame = CGRectMake(self.cdFlashStart.x,self.cdFlashStart.y, self.flashCD.frame.size.width,
                                    self.flashCD.frame.size.height);
    
    [self.flashCD setHidden:NO];
    [self.mainContentView bringSubviewToFront:self.flashCD];
    
    [UIView animateWithDuration:0.5 animations:^{
        self.flashCD.frame = CGRectMake(289, -35, self.flashCD.frame.size.width, self.flashCD.frame.size.height);
    }completion:^(BOOL finished){
        if (finished) {
            [self.flashCD setHidden:YES];
        }
    }];
    
    
}

- (void)creatEmptyMsgView{
    self.emptyMsgView = [[UIView alloc]init];
    self.emptyMsgView.frame = CGRectMake(0, 110, _MainScreen_Width, 150);
    //self.emptyMsgView.backgroundColor = [UIColor redColor];
    self.emptyMsgView.hidden = YES;
    
    UIImageView * logoView = [[UIImageView alloc]init];
    logoView.frame = CGRectMake(135, 6, 51, 48);
    logoView.image = [UIImage imageNamed:@"ktv_jiazai.png"];
    
    [self.emptyMsgView addSubview:logoView];
    
    UILabel * emptyMsg = [[UILabel alloc]init];
    emptyMsg.frame = CGRectMake(0, 70, _MainScreen_Width, 30);
    emptyMsg.text = @"没有获取到数据哟，请稍后再试吧";
    emptyMsg.backgroundColor = [UIColor clearColor];
    emptyMsg.textAlignment = NSTextAlignmentCenter;
    emptyMsg.font = [UIFont systemFontOfSize:15];
    emptyMsg.textColor = UIColorFromRGB(0x909090);
    
    [self.emptyMsgView addSubview:emptyMsg];
    
    [self.mainContentView addSubview:self.emptyMsgView];
}


#pragma mark tableViewDelegate

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return _dataArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    SongsBookCell * cell = [_searchResultList dequeueReusableCellWithIdentifier:@"cell"];
    if (cell == nil) {
        cell = [[SongsBookCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
    }
    
    SongsBookModel * model = [_dataArray objectAtIndex:indexPath.row];
    cell.model = model;
    cell.delegate = self;
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    SongsBookModel * model = [_dataArray objectAtIndex:indexPath.row];
    
    CGFloat distanceToButtom = _searchResultList.frame.size.height - (indexPath.row*50-_searchResultList.contentOffset.y);
    
    
    if (model.isHigher == NO) {
        for (SongsBookModel * aModel in _dataArray) {
            if (aModel.isHigher == YES) {
                aModel.isHigher = NO;
            }
        }
        if (distanceToButtom < 99) {
            _searchResultList.contentOffset = CGPointMake(_searchResultList.contentOffset.x, _searchResultList.contentOffset.y + 99 - distanceToButtom);
        }
        self.presentRow = indexPath.row;
        
        model.isHigher = YES;
    }else {
        model.isHigher = NO;
    }
    
    [_searchResultList reloadData];
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    SongsBookModel * model = [_dataArray objectAtIndex:indexPath.row];
    if (model.isHigher == YES) {
        return 100;
    }else {
        return 50;
    }
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView{
    if (scrollView == _searchResultList) {
        if (_searchResultList.contentOffset.y + _searchResultList.frame.size.height > _searchResultList.contentSize.height) {
            [self loadSearchData];
        }
    }
}

#pragma mark SongOperation

-(void)onOrder:(SongsBookModel*) info view:(id)cell point:(CGPoint)point
{
    if ([UserSessionManager isRoomAlreadyLogin]) {
        self.cdFlashStart = [cell convertPoint:point toView:_searchResultList];
        self.cdFlashStart = CGPointMake(self.cdFlashStart.x, self.cdFlashStart.y-_searchResultList.contentOffset.y);
        
        NSString *encodeUrl = [PKtvAssistantAPI getOrderSongUrl:[[UserSessionManager getInstance] currentUser].uid ktvID:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword songId:[NSString stringWithFormat:@"%ld",info.songInfo.songId] songName:info.songInfo.songName artistName:info.songInfo.songArtist isScore:info.songInfo.songIsScore];
        NSURL *url = [NSURL URLWithString:encodeUrl];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
        
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            PLog(@"doOrderSong complete...");
            //NSString *response = [resquest responseString];
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData2:responseObject];
            
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                info.songInfo.isOrdered = YES;
                [_searchResultList reloadData];
                [_alreadyBookedSongsArray removeAllObjects];
                [self loadAlreadyBookedSongs];
                
                //[SVProgressHUD showSuccessWithStatus:@"点歌成功"];
                
                [self cdViewFlash];
                
                //通知更新金币信息
                NSString *result  = [results objectForKey:@"result"];
                if (![result isEqualToString:@"error"] && [result integerValue]>0 && [result integerValue]!= [UserSessionManager getInstance].currentUser.gold)
                {
                    [[UserSessionManager getInstance] updateCurrentUserGold:[result integerValue]];
                }
                
            } else {
                NSString *msg = [results objectForKey:@"msg"];
                NSString *errorCode = [results objectForKey:@"errorcode"];
                PLog(@"doOrderSong failed...%@", msg);
                
                if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForSongExisted]||[errorCode isEqualToString:kPKtvAssistantErrorCodeForSongPlaying]) {
                    
                    info.songInfo.isOrdered = YES;
                    [_searchResultList reloadData];
                    [_alreadyBookedSongsArray removeAllObjects];
                    [self loadAlreadyBookedSongs];
                    
                    //[SVProgressHUD showSuccessWithStatus:@"点歌成功"];
                    [self cdViewFlash];
                    
                    return;
                }
                [CommenTools showAlertViewWithErrorMessage:msg];
            }
            
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            PLog(@"doOrderSong failed...");
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        [operation start];
        
        
    }else{
        [self goInRoom:nil];
    }
}
-(void)onTop:(SongsBookModel*) info
{
    if ([UserSessionManager isRoomAlreadyLogin]) {
        if (info.songInfo.isOrdered == NO) {
            NSString *encodeUrl = [PKtvAssistantAPI getAddAndSetSongToTopUrl:[[UserSessionManager getInstance] currentUser].uid ktvID:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword songId:[NSString stringWithFormat:@"%ld",info.songInfo.songId] songName:info.songInfo.songName artistName:info.songInfo.songArtist isScore:info.songInfo.songIsScore];
            NSURL *url = [NSURL URLWithString:encodeUrl];
            NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
            
            [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
            AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
            [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
                //NSLog(@"doAddAndSetSongToTop complete...");
                //NSString *response = [resquest responseString];
                NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData2:responseObject];
                
                NSString *status = [results objectForKey:@"status"];
                if ([status compare:@"1"] == NSOrderedSame) {
                    //
                    info.songInfo.isOrdered = YES;
                    [_searchResultList reloadData];
                    
                    [_alreadyBookedSongsArray removeAllObjects];
                    [self loadAlreadyBookedSongs];
                    
                    [SVProgressHUD showSuccessWithStatus:@"置顶成功"];
                    
                    
                    //通知更新金币信息
                    NSString *result  = [results objectForKey:@"result"];
                    if (![result isEqualToString:@"error"] && [result integerValue]>0 && [result integerValue]!= [UserSessionManager getInstance].currentUser.gold)
                    {
                        [[UserSessionManager getInstance] updateCurrentUserGold:[result integerValue]];
                    }
                } else {
                    NSString *msg = [results objectForKey:@"msg"];
                    PLog(@"doAddAndSetSongToTop failed...%@", msg);
                    
                    
                    
                    [CommenTools showAlertViewWithErrorMessage:msg];
                }
                
            } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
                PLog(@"doAddAndSetSongToTop failed...");
                [CommenTools showNetworkDisconnectedAlertView];
            }];
            [operation start];
            
        }else{
            NSString *encodeUrl = [PKtvAssistantAPI getSetSongToTopUrl:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword songId:[NSString stringWithFormat:@"%ld",info.songInfo.songId]];
            
            NSURL *url = [NSURL URLWithString:encodeUrl];
            NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
            
            [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
            AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
            [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
                //NSLog(@"doSetSongToTop complete...");
                //NSString *response = [resquest responseString];
                NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
                
                NSString *status = [results objectForKey:@"status"];
                if ([status compare:@"1"] == NSOrderedSame) {
                    //
                    [_alreadyBookedSongsArray removeAllObjects];
                    [self loadAlreadyBookedSongs];
                    
                    [SVProgressHUD showSuccessWithStatus:@"置顶成功"];
                    
                } else {
                    NSString *msg = [results objectForKey:@"msg"];
                    PLog(@"doSetSongToTop failed...%@", msg);
                    
                    NSString *errorCode = [results objectForKey:@"errorcode"];
                    if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForSongError]) {
                        info.songInfo.isOrdered = NO;
                        
                        [self onTop:info];
                        return;
                    }
                    
                    [CommenTools showAlertViewWithErrorMessage:msg];
                }
                
            } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
                PLog(@"doSetSongToTop failed...");
                [CommenTools showNetworkDisconnectedAlertView];
            }];
            [operation start];
            
        }
        
    }else{
        [self goInRoom:nil];
    }
}
-(void)onDelete:(SongsBookModel*) info
{
    if ([UserSessionManager isRoomAlreadyLogin]) {
        RoomInfo *currentRoomInfo = [UserSessionManager getInstance].currentRoomInfo;
        NSString* strUrl = [PKtvAssistantAPI getDeleteSongFromOrdered:currentRoomInfo.ktvId roomId:currentRoomInfo.roomId roomPassword:currentRoomInfo.roomPassword songId:[NSString stringWithFormat:@"%ld",info.songInfo.songId]];
        NSURL *url = [NSURL URLWithString:strUrl];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
        
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            PLog(@"doOrderSong complete...");
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
            
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                info.songInfo.isOrdered = NO;
                [_searchResultList reloadData];
                [_alreadyBookedSongsArray removeAllObjects];
                [self loadAlreadyBookedSongs];
            } else {
                NSString *msg = [results objectForKey:@"msg"];
                NSString *errorCode = [results objectForKey:@"errorcode"];
                PLog(@"doDeleteSong failed...%@", msg);
                
                if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForSongExisted]||[errorCode isEqualToString:kPKtvAssistantErrorCodeForSongPlaying]) {
                    
                    info.songInfo.isOrdered = YES;
                    [_searchResultList reloadData];
                    [_alreadyBookedSongsArray removeAllObjects];
                    [self loadAlreadyBookedSongs];
                    return;
                }
                [CommenTools showAlertViewWithErrorMessage:msg];
            }
            
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            PLog(@"doOrderSong failed...");
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        [operation start];
    }else{
        [self goInRoom:nil];
    }
}
-(void)onCollect:(SongsBookModel*) info
{
    if ([UserSessionManager isUserAlreadyLogin]) {
        NSString *encodeUrl = [PKtvAssistantAPI getAddCollectionSongUrl:[UserSessionManager getInstance].currentUser.uid songId:info.songInfo.songId];
        NSURL *url = [NSURL URLWithString:encodeUrl];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
        
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            PLog(@"doAddCollectionSong complete...");
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
            
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                NSDictionary *result = [results objectForKey:@"result"];
                BOOL res = [[result objectForKey:@"success"] boolValue];
                if(res){
                    PLog(@"添加收藏成功");
                    [SVProgressHUD showSuccessWithStatus:@"添加收藏成功"];
                }else{
                    PLog(@"添加收藏失败");
                    [SVProgressHUD showSuccessWithStatus:@"添加收藏失败"];
                }
                
            } else {
                NSString *msg = [results objectForKey:@"msg"];
                NSString *errorCode = [results objectForKey:@"errorcode"];
                PLog(@"doAddCollectionSong failed...errorcode:%@(%@)", errorCode, msg);
                if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForSongAlreadyCollected]) {
                    [SVProgressHUD showSuccessWithStatus:@"添加收藏成功"];
                    return;
                }
                [CommenTools showAlertViewWithErrorMessage:msg];
            }
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            
            PLog(@"doAddCollectionSong failed...");
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        
        [operation start];
        
    }
}

#pragma mark -  UIActionSheet 进入包厢

- (void)goInRoom:(UIButton *)btn
{
    /*
     _boxVodActionSheet = [[UIActionSheet alloc] initWithTitle:@"请选择点歌方式" delegate:self cancelButtonTitle:@"取消" destructiveButtonTitle:nil otherButtonTitles:@"扫描二维码进包厢",@"输入房间密码进包厢", nil];
     [_boxVodActionSheet showInView:self.view];
     _boxVodActionSheet.actionSheetStyle = UIBarStyleDefault;
     */
    
    //********* 自定义表单 *********
    CMActionSheet *actionSheet = [[CMActionSheet alloc] init];
    actionSheet.title = @"请先进入包厢";
    [actionSheet addSeparator];
    
    [actionSheet addButtonWithTitle:@"扫描包厢二维码" type:CMActionSheetButtonTypeWhite block:^{
        PLog(@"扫描包厢二维码");
        [self openAction];
        
    }];
    [actionSheet addButtonWithTitle:@"输入包厢密码" type:CMActionSheetButtonTypeBlue block:^{
        PLog(@"输入包厢密码");
        KTVInputSecretController *input = [[KTVInputSecretController alloc] init];
        [self.navigationController pushViewController:input animated:YES];
    }];
    [actionSheet addButtonWithTitle:@"取消" type:CMActionSheetButtonTypeRed block:^{
        PLog(@"取消");
    }];
    [actionSheet present];
}

#pragma mark - 开启二维码扫描

- (void)openAction
{
    _reader = [ZBarReaderViewController new];
    //设备尺寸 3.5/4.0英寸
    _reader.view.frame = CGRectMake(0, 0, _MainScreen_Width, 480+(iPhone5?88:0));
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
             [roominfo Log];
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
             
             [self getAlreadyOrderedSongsNumber];
             [self setRightAndBadgeButton];
             [self loadAlreadyBookedSongs];
             
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
             PLog(@"doLogin failed...%@", msg);
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




#pragma mark loadData

- (void)loadSearchData{
    if (_isLoadData == YES || _noData == YES) {
        return;
    }
    
    Reachability *reachability = [Reachability reachabilityForInternetConnection];
    if (![reachability isReachable])
    {
        if (_next == NO) {
            [self.emptyMsgView setHidden:NO];
            [self.mainContentView bringSubviewToFront:self.emptyMsgView];
        }else{
            [self showNetwork];
        }
        return;
    }
    
    _isLoadData = YES;
    
    NSString * urlStr = [PKtvAssistantAPI getSongSearchUrl:[UserSessionManager getInstance].currentRoomInfo.roomId keyword:self.searchKey modelId:self.modelID topicId:self.topicID begin:_count*20+1 num:20];
    
    //NSLog(@"%@",urlStr);
    NSURL *url = [NSURL URLWithString:urlStr];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            PLog(@"doSongSearch complete...");
            //NSString *response = [resquest responseString];
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
            
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                NSDictionary *result = [results objectForKey:@"result"];
                NSArray *dataList = [result objectForKey:@"Songlist"];
                _next = YES;
                
                if (dataList.count < 20) {
                    _noData = YES;
                    
                    if (dataList.count == 0 && _count == 0) {
                        if(self.emptyTips==nil){
                            self.emptyTips = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 14)];
                            self.emptyTips.center = CGPointMake(160.0f, 208.0f);
                            self.emptyTips.backgroundColor = [UIColor clearColor];
                            self.emptyTips.text = @"对不起，没有找到对应的内容";
                            self.emptyTips.textAlignment = UITextAlignmentCenter;
                            self.emptyTips.textColor = [UIColor grayColor];
                            self.emptyTips.font = [UIFont systemFontOfSize:14];
                            [self.mainContentView addSubview:self.emptyTips];
                        }
                        self.emptyTips.hidden =NO;
                        [self.mainContentView bringSubviewToFront:self.emptyTips];
                    }
                }
                
                int datalistlen = [dataList count];
                for (int i=0; i<datalistlen; i++) {
                    SongsBookModel * model = [[SongsBookModel alloc]init];
                    
                    SongInfo *songinfo = [SongInfo initWithDictionary:[dataList objectAtIndex:i]];
                    //[songinfo Log];
                    model.songInfo = songinfo;
                    model.cellType = TYPECELL_CommonOrderSong;
                    
                    [_dataArray addObject:model];
                }
                
                [_searchResultList reloadData];
                
                [self markAlreadyOrderedSongs];
                
                _count++;
                
            } else {
                NSString *msg = [results objectForKey:@"msg"];
                PLog(@"doSongSearch failed...%@", msg);
                
                if (_next == NO) {
                    [self.emptyMsgView setHidden:NO];
                    [self.mainContentView bringSubviewToFront:self.emptyMsgView];
                }

            }
            
            _isLoadData = NO;
            
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {

            PLog(@"doSongSearch failed...");
            
            _isLoadData = NO;
            
        if (_next == NO) {
            [self.emptyMsgView setHidden:NO];
            [self.mainContentView bringSubviewToFront:self.emptyMsgView];
        }

    }];
    [operation start];
    
    
}

- (void)loadAlreadyBookedSongs{
    if ([UserSessionManager isRoomAlreadyLogin]) {
        NSString * urlStr = [[[PKtvAssistantAPI alloc]init]getGetOrderedSongListUrl:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword];
        
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
                    
                    return;
                }
                
                NSArray *dataList = [result objectForKey:@"Songlist"];
                for (NSDictionary * dic in dataList) {
                    
                    SongInfo * info = [SongInfo initWithDictionary:dic];
                    
                    [_alreadyBookedSongsArray addObject:info];
                    
                }
                
                [self getAlreadyOrderedSongsNumber];
                
                [self markAlreadyOrderedSongs];
            } else {
                NSString *msg = [results objectForKey:@"msg"];
                PLog(@"doGetOrderedSongList failed...%@", msg);
            }
            
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {

            PLog(@"doGetOrderedSongList failed...");
        }];
        [operation start];
        
    }else{
        return;
    }
    
}


- (void)markAlreadyOrderedSongs{
    
    VECTREQSONGLIST vectOrderList = [NetUtilCallBack getInstance]->m_vectreqsonglist;
    
    for (SongsBookModel * model in _dataArray) {
        model.songInfo.isOrdered = NO;
        
        VECTREQSONGLIST::iterator it = vectOrderList.begin();
        for (; it!=vectOrderList.end(); it++) {
            if(it->songid == model.songInfo.songId)
                model.songInfo.isOrdered = YES;
        }
    }
    
    [_searchResultList reloadData];
}

#pragma mark viewDidLoad

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    
    [self setNavigationView];
    
    _dataArray = [[NSMutableArray alloc]init];
    _alreadyBookedSongsArray = [[NSMutableArray alloc]init];
    
    
    
    [self creatSearchResultList];
    
    [self creatFlashView];
    
    [self creatEmptyMsgView];
    
    [self loadSearchData];
    
    
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:NO];
    
    [self getAlreadyOrderedSongsNumber];
    
    [_alreadyBookedSongsArray removeAllObjects];
    
    [self loadAlreadyBookedSongs];
    
    [self setRightAndBadgeButton];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:YES];
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
