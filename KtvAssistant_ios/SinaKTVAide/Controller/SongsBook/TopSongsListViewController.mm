//
//  TopSongsListViewController.m
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-13.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "TopSongsListViewController.h"
#import "BookedAndCollectedController.h"
#import "TopSong.h"
#import "SongInfo.h"
#import "User.h"
#import "DDMenuController.h"
#import "KTVAppDelegate.h"
#import "SVProgressHUD.h"
#import "CMActionSheet.h"
#import "KTVInputSecretController.h"
#import "RegexKitLite.h"
#import "iToast.h"
//#import "BPush.h"
#import "NetUtilCallBack.h"

@interface TopSongsListViewController ()

@end

@implementation TopSongsListViewController

-(instancetype) initInfo:(int) model topic:(long)topic title:(NSString*)title
{
    self = [super init];
    if(self)
    {
        self.modelID = model;
        self.topicID = topic;
        self.listTitle = title;
    }
    return self;
}

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
    self.navView.titleLabel.text = self.listTitle;
    
    self.navView.hidden = NO;
    //    UIImage *backImageNormal = [UIImage imageWithName:@"bth_back" type:@"png"];
    //    UIImage *backImage = [UIImage imageWithName:@"bth_back_c" type:@"png"];
    [self.navView.backButton setHidden:NO];
    
    [self.navView.rightButton setHidden:NO];
//    [self.navView.rightButton addTarget:self action:@selector(intoBookedAndCollected:) forControlEvents:UIControlEventTouchUpInside];
    
    //self.navView.badgeButton.enabled = NO;
    
}

- (void)intoBookedAndCollected:(UIButton *)button
{
    BookedAndCollectedController *bookedAndCollected = [[BookedAndCollectedController alloc] init];
    
    [self.navigationController pushViewController:bookedAndCollected animated:YES];
}

- (void)creatTopSongsList{
    _topSongsList = [[UITableView alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height) style:UITableViewStylePlain];
    
    _topSongsList.delegate = self;
    _topSongsList.dataSource = self;
    _topSongsList.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    UIView *spaceView = [[UIView alloc] init];
    _topSongsList.tableFooterView = spaceView;
    
    [_topSongsList setBackgroundColor:UIColorFromRGB(0xf0f1f5)];
    
    [self.mainContentView addSubview:_topSongsList];
    
    _dataArray = [[NSMutableArray alloc]init];
    _alreadyBookedSongs = [[NSMutableArray alloc]init];
}

- (void)errorMessageShow:(NSString *)errorMessage{
    [_errorRemindView setTitle:errorMessage];
    [_errorRemindView show];
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
    self.flashCD.frame = CGRectMake(self.cdFlashStart.x,self.cdFlashStart.y, self.flashCD.frame.size.width, self.flashCD.frame.size.height);
    
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

#pragma mark tableViewDelegate
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return _dataArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    static NSString* reuseId = @"TopSongCellG";
    KTVTopSongCell* cell = [_topSongsList dequeueReusableCellWithIdentifier:reuseId];
    if(cell == nil)
    {
        [[NSBundle mainBundle] loadNibNamed:@"KTVTopSongCell" owner:self options:nil];
        cell = self.mUITopSongCell;
        cell.mCallBack = self;
    }
    [cell setTopSongInfo:[_dataArray objectAtIndex:indexPath.row]];
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    TopSongCellModel * model = [_dataArray objectAtIndex:indexPath.row];
    
    CGFloat distanceToButtom = _topSongsList.frame.size.height - (indexPath.row*60-_topSongsList.contentOffset.y);
    
    if (model.isHigher == NO) {
        for (TopSongCellModel * aModel in _dataArray) {
            if (aModel.isHigher == YES) {
                
                aModel.isHigher = NO;
            }
        }
        if (distanceToButtom < 109) {
            _topSongsList.contentOffset = CGPointMake(_topSongsList.contentOffset.x, _topSongsList.contentOffset.y + 109 - distanceToButtom);
        }
        self.presentRow = indexPath.row;
        
        model.isHigher = YES;

        
        
    }else {
        model.isHigher = NO;
        
    }
    
    [_topSongsList reloadData];
    
    
}

#define CELL_HEIGHT 76.0f
#define OPERATION_HEIGHT 50.0f

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    TopSongCellModel * model = [_dataArray objectAtIndex:indexPath.row];
    
    if (model.isHigher == YES) {
        return CELL_HEIGHT+OPERATION_HEIGHT;
    }else {
        return CELL_HEIGHT;
    }
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView{
    if (scrollView == _topSongsList) {
        if (_topSongsList.contentOffset.y + _topSongsList.frame.size.height > _topSongsList.contentSize.height) {
            [self loadDataFromServicer];
        }
    }
}
#pragma mark SongOperation
-(void)onTopOrder:(TopSongCellModel*) info view:(id)cell point:(CGPoint)point
{
    if ([UserSessionManager isRoomAlreadyLogin]) {
        
        self.cdFlashStart = [cell convertPoint:point toView:_topSongsList];
        self.cdFlashStart = CGPointMake(self.cdFlashStart.x, self.cdFlashStart.y-_topSongsList.contentOffset.y);
        
        NSString *encodeUrl = [PKtvAssistantAPI getOrderSongUrl:[[UserSessionManager getInstance] currentUser].uid ktvID:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword songId:[NSString stringWithFormat:@"%ld",info.topSongInfo.song.songId] songName:info.topSongInfo.song.songName artistName:info.topSongInfo.song.songArtist isScore:info.topSongInfo.song.songIsScore];
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
                
                [self cdViewFlash];
                
                //通知更新金币信息
                NSString *result  = [results objectForKey:@"result"];
                if (![result isEqualToString:@"error"] && [result integerValue]>0)
                {
                    [[UserSessionManager getInstance] updateCurrentUserGold:[result integerValue]];
                }

            } else {
                NSString *msg = [results objectForKey:@"msg"];
                [self errorMessageShow:msg];
            }
            
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            PLog(@"doOrderSong failed...");
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        [operation start];
        
    }else{
        
        //[CommenTools showAlertViewWithErrorMessage:@"请先进入包厢"];
        
        // 未进包厢。底部弹出表单提供登录包厢 by liric 2014.1.2
        [self goInRoom:nil];
    }
}

-(void)onTopDelete:(TopSongCellModel*) info
{
    NSString *encodeUrl = [PKtvAssistantAPI getDeleteSongFromOrdered:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword songId:[NSString stringWithFormat:@"%ld",info.topSongInfo.song.songId]];
    NSURL *url = [NSURL URLWithString:encodeUrl];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        PLog(@"deleteSongFromOrdered complete...");
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            [_topSongsList reloadData];
            [_alreadyBookedSongs removeAllObjects];
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            [CommenTools showAlertViewWithErrorMessage:msg];
        }
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        PLog(@"deleteSongFromOrdered failed...");
        [CommenTools showNetworkDisconnectedAlertView];
    }];
    [operation start];
}

- (void)onTopCollect:(TopSongCellModel *)info{
    if ([UserSessionManager isUserAlreadyLogin]) {
        NSString *encodeUrl = [PKtvAssistantAPI getAddCollectionSongUrl:[UserSessionManager getInstance].currentUser.uid songId:info.topSongInfo.song.songId];
        NSURL *url = [NSURL URLWithString:encodeUrl];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
        
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            PLog(@"doAddCollectionSong complete...");
            //NSString *response = [resquest responseString];
            //PLog(@"response->%@",response);
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
                }else{
                    [self errorMessageShow:msg];
                }
            }
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            PLog(@"doAddCollectionSong failed...");
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        
        [operation start];
    }
}

- (void)onTopTop:(TopSongCellModel *)info{
    if ([UserSessionManager isRoomAlreadyLogin]) {
        
        if (info.topSongInfo.song.isOrdered == NO) {
            NSString *encodeUrl = [PKtvAssistantAPI getAddAndSetSongToTopUrl:[[UserSessionManager getInstance] currentUser].uid ktvID:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword songId:[NSString stringWithFormat:@"%ld",info.topSongInfo.song.songId] songName:info.topSongInfo.song.songName artistName:info.topSongInfo.song.songArtist isScore:info.topSongInfo.song.songIsScore];
            NSURL *url = [NSURL URLWithString:encodeUrl];
            NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
            
            [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
            AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
            [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
                PLog(@"doAddAndSetSongToTop complete...");
                //NSString *response = [resquest responseString];
                NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData2:responseObject];
                
                NSString *status = [results objectForKey:@"status"];
                if ([status compare:@"1"] == NSOrderedSame) {
                    [SVProgressHUD showSuccessWithStatus:@"置顶成功"];
                    //通知更新金币信息
                    NSString *result  = [results objectForKey:@"result"];
                    if (![result isEqualToString:@"error"] && [result integerValue]>0 && [result integerValue]!= [UserSessionManager getInstance].currentUser.gold)
                    {
                        [[UserSessionManager getInstance] updateCurrentUserGold:[result integerValue]];
                    }
                } else {
                    NSString *msg = [results objectForKey:@"msg"];
                    [self errorMessageShow:msg];
                }
                
            } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
                PLog(@"doAddAndSetSongToTop failed...");
                [CommenTools showNetworkDisconnectedAlertView];
            }];
            [operation start];
        }else{
            NSString *encodeUrl = [PKtvAssistantAPI getSetSongToTopUrl:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword songId:[NSString stringWithFormat:@"%ld",info.topSongInfo.song.songId]];
            NSURL *url = [NSURL URLWithString:encodeUrl];
            NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
            
            [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
            AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
            [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
                PLog(@"doSetSongToTop complete...");
                //NSString *response = [resquest responseString];
                NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
                
                NSString *status = [results objectForKey:@"status"];
                if ([status compare:@"1"] == NSOrderedSame) {
                    //
                    [_alreadyBookedSongs removeAllObjects];
                    
                    [SVProgressHUD showSuccessWithStatus:@"置顶成功"];
                    
                } else {
                    NSString *msg = [results objectForKey:@"msg"];
                    PLog(@"doSetSongToTop failed...%@", msg);
                    NSString *errorCode = [results objectForKey:@"errorcode"];
                    if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForSongError]) {
                        info.topSongInfo.song.isOrdered = NO;
                        [self onTopTop:info];
                        
                        return;
                    }
                    [self errorMessageShow:msg];
                }
                
           } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
                PLog(@"doSetSongToTop failed...");
                [CommenTools showNetworkDisconnectedAlertView];
            }];
            [operation start];

        }
    }else{
       
        //[self errorMessageShow:@"请先进入包厢"];
        
        // 未进包厢。底部弹出表单提供登录包厢 by liric 2014.1.2
        [self goInRoom:nil];
    }
}

#pragma mark -  UIActionSheet 进入包厢

- (void)goInRoom:(UIButton *)btn
{
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
    _reader.view.frame = CGRectMake(0, 0, _MainScreen_Width, _MainScreen_Height);
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
             //[BPush setTag:tag];
             [SVProgressHUD dismiss];
             
             [self getAlreadyOrderedSongsNumber];
             [self setRightAndBadgeButton];
             [self flashUIWithRoom]; // 进入包厢或者退出包厢刷新界面
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
- (void)loadDataFromServicer{
    if (_isLoadData == YES || _noData == YES) {
        return;
    }
    
    Reachability *reachability = [Reachability reachabilityForInternetConnection];
    if (![reachability isReachable])
    {
        if (_next == NO) {
            [self showEmptyTip:YES msg:RES_NO_NETWORK];
        }else{
            [self showNetwork];
        }
        return;
    }

    _isLoadData = YES;
    
    
    //NSString * urlStr = [PKtvAssistantAPI getTopSongListUrl:[UserSessionManager getInstance].currentUser.uid roomId:[UserSessionManager getInstance].currentRoomInfo.roomId modelID:_modelID topicID:_topicID  begin:_count*20+1 num:20 ];
    NSString* urlStr = [PKtvAssistantAPI getGetSongListByTopics:self.modelID topicId:self.topicID begin:_count*20+1 num:20];
    NSURL *url = [NSURL URLWithString:urlStr];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];

    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        PLog(@"doGetTopSongList complete...");
        //NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            _next = YES;
            
            NSDictionary *result = [results objectForKey:@"result"];
            NSArray *dataList = NULL;
            if(self.topicID == 1 || self.topicID == 2 || self.topicID == 8)
                dataList = [result objectForKey:@"worklists"];
            else
                dataList = [[NSArray alloc] init];
            
            if (dataList.count < 20) {
                _noData = YES;
            }
            for (NSDictionary * dic in dataList) {
                TopSong * top = [TopSong initWithDictionary:dic];
                TopSongCellModel * model = [[TopSongCellModel alloc]init];
                model.topSongInfo = top;
                
                //检查数据是否正常
                if(model.topSongInfo.song == nil || [model.topSongInfo.song.songName compare:@""] == NSOrderedSame)
                    continue;
                [_dataArray addObject:model];
            }
            
            if(dataList.count == 0 && _count == 0)
                [self showEmptyTip:YES msg:RES_NO_DATA];
            else
            {
                [self showEmptyTip:NO msg:nil];
                [self markAlreadyOrderedSongs];
            }
            
            [self getAlreadyOrderedSongsNumber];
            _count ++;
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doGetTopSongList failed...errorcode:%@(%@)", errorCode, msg);
            
            if (_next == NO) {
                [self showEmptyTip:YES msg:msg];
            }
        }
        _isLoadData = NO;
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        _isLoadData = NO;
        PLog(@"doGetTopSongList failed...");
        
        if (_next == NO) {
            [self showEmptyTip:YES msg:RES_NO_NETWORK];
        }
    }];
    [self startWait];
    [operation start];
}

#pragma mark NetUtilCallBackProtocol
-(void)closeRoom
{
    [self performSelectorOnMainThread:@selector(markAlreadyOrderedSongs) withObject:nil waitUntilDone:NO];
}
-(void)OnRoomTunnelClose
{
    [self performSelectorOnMainThread:@selector(markAlreadyOrderedSongs) withObject:nil waitUntilDone:NO];
}
-(void)pushReqSongInfo:(const ResultData_SongList*)pSonglist :(int)count
{
    [super pushReqSongInfo:pSonglist :count];
    [self performSelectorOnMainThread:@selector(markAlreadyOrderedSongs) withObject:nil waitUntilDone:NO];
}

- (void)markAlreadyOrderedSongs{
    VECTREQSONGLIST vectOrderList = [NetUtilCallBack getInstance]->m_vectreqsonglist;
    
    for (TopSongCellModel * top in _dataArray) {
        top.topSongInfo.song.isOrdered = NO;
        
        VECTREQSONGLIST::iterator it = vectOrderList.begin();
        for(;it!=vectOrderList.end();it++)
        {
            if(it->songid == top.topSongInfo.song.songId)
                top.topSongInfo.song.isOrdered = YES;
        }
    }
    
    [_topSongsList reloadData];
}

#pragma mark viewDidLoad



- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    
    [self setNavigationView];
    
    [self creatTopSongsList];
    
    [self creatFlashView];
    
    _errorRemindView = [[UIAlertView alloc]initWithTitle:nil message:nil delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil];
    
    [self loadDataFromServicer];
    }

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:NO];
    
    [self getAlreadyOrderedSongsNumber];
    
    [self markAlreadyOrderedSongs];
    
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
