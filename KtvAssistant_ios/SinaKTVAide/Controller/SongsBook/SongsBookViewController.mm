//
//  SongsBookViewController.m
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-4.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "SongsBookViewController.h"
#import "SongsBookModel.h"
#import "SSImageCategory.h"
#import "BookedAndCollectedController.h"
#import "PKtvAssistantAPI.h"
#import "ASIHTTPRequest.h"
#import "PCommonUtil.h"
#import "SongInfo.h"
#import "SearchResultViewController.h"
#import "DDMenuController.h"
#import "KTVAppDelegate.h"
#import "SVProgressHUD.h"
//add
#import "CMActionSheet.h"
#import "KTVInputSecretController.h"
#import "RegexKitLite.h"
#import "iToast.h"
//#import "BPush.h"
#import "xingguang.h"
#import "com_ios_NetUtil.h"
#import "NetUtilCallBack.h"

#define SCREEN_EXTRA_HEIGHT ([UIScreen mainScreen].bounds.size.height - 480.f)

@interface SongsBookViewController ()


@end

@implementation SongsBookViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

-(instancetype) initInfoNoSinger:(int)modelId topicId:(long)p1 title:(NSString*)p2
{
    self = [super init];
    if(self)
    {
        self.modelID = modelId;
        self.topicID = p1;
        self.listTitle = p2;
        self.artistID = 0;
    }
    
    return self;
}

-(instancetype) initInfo:(int)modelId topicId:(long)p1 title:(NSString*)p2 singerId:(long)p3
{
    self = [super init];
    if(self)
    {
        self.modelID = modelId;
        self.topicID = p1;
        self.listTitle = p2;
        self.artistID = p3;
    }
    
    return self;
}
#pragma mark add View

- (void)setNavigationView
{
    [self.navView initCommonBackButton:self.listTitle isInRoom:NO];
}

- (void)intoBookedAndCollected:(UIButton *)button
{
    BookedAndCollectedController *bookedAndCollected = [[BookedAndCollectedController alloc] init];
    
    [self.navigationController pushViewController:bookedAndCollected animated:YES];
}

- (void)creatSongsList{
    
    _songsList = [[UITableView alloc]initWithFrame:CGRectMake(0, 44, _MainScreen_Width, self.mainContentView.frame.size.height-44) style:UITableViewStylePlain];
    [_songsList setBackgroundColor:UIColorFromRGB(0xf0f1f5)];
    
    _songsList.separatorStyle = UITableViewCellSeparatorStyleNone;
    _songsList.delegate = self;
    _songsList.dataSource = self;
    UIView *spaceView = [[UIView alloc] init];
    _songsList.tableFooterView = spaceView;
    
    [self.mainContentView addSubview:_songsList];
    
}

- (void)creatSongSearchBar
{
    _songSearchBar = [[UISearchBar alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 44)];
    _songSearchBar.autocorrectionType = UITextAutocorrectionTypeNo;
    _songSearchBar.autocapitalizationType = UITableViewCellSeparatorStyleNone;
    _songSearchBar.keyboardType = UIKeyboardTypeDefault;
    _songSearchBar.delegate = self;
    _songSearchBar.backgroundColor = [UIColor clearColor];
    _songSearchBar.placeholder = @"请输入搜索内容";
    
    [self.mainContentView addSubview:_songSearchBar];
    [self.mainContentView bringSubviewToFront:_songSearchBar];
    
    // 轻敲手势
    UITapGestureRecognizer *keyboard_tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(dismissKeyboard)];
    keyboard_tapGesture.cancelsTouchesInView = NO;
    
    // 键盘弹出透明遮盖view
    self.view_loading = [[UIView alloc] initWithFrame:CGRectMake(0, 44, _MainScreen_Width, self.mainContentView.frame.size.height-44)];
    self.view_loading.backgroundColor = [UIColor colorWithWhite:0.0f alpha:0.8f];
    [self.view_loading addGestureRecognizer:keyboard_tapGesture];
    [self.mainContentView addSubview:self.view_loading];
    [self.mainContentView sendSubviewToBack:self.view_loading];
    self.view_loading.hidden = YES;
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
#pragma mark SongOperation
-(void)onOrder:(SongsBookModel*) info view:(id)cell point:(CGPoint)point
{
    if ([UserSessionManager isRoomAlreadyLogin]) {
        
        self.cdFlashStart = [cell convertPoint:point toView:self.songsList];
        self.cdFlashStart = CGPointMake(self.cdFlashStart.x, self.cdFlashStart.y-self.songsList.contentOffset.y);
        
        NSString *encodeUrl = [PKtvAssistantAPI getOrderSongUrl:[[UserSessionManager getInstance] currentUser].uid ktvID:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword songId:[NSString stringWithFormat:@"%ld",info.songInfo.songId] songName:info.songInfo.songName artistName:info.songInfo.songArtist isScore:info.songInfo.songIsScore];
        NSURL *url = [NSURL URLWithString:encodeUrl];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
        
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {            PLog(@"doOrderSong complete...");
            //NSString *response = [resquest responseString];
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData2:responseObject];
            
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                [_songsList reloadData];
                [_alreadyBookedSongsArray removeAllObjects];
                
                NSIndexPath* indexPath = [self.songsList indexPathForCell:cell];
                CGRect rectInTableView = [self.songsList rectForRowAtIndexPath:indexPath];
                CGRect rect = [self.songsList convertRect:rectInTableView toView:[self.songsList superview]];
                [self cdViewFlash];
                
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
            PLog(@"doOrderSong failed...");
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        [operation start];
        
    }else{
        
        //[self errorMessageShow:@"请先进入包厢"];
        
        // 未进包厢。底部弹出表单提供登录包厢 by liric 2014.1.2
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
            NSString *encodeUrl = [PKtvAssistantAPI getSetSongToTopUrl:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword songId:[NSString stringWithFormat:@"%ld",info.songInfo.songId]];
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
                    [_alreadyBookedSongsArray removeAllObjects];
                    
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
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
            
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                [_songsList reloadData];
                [_alreadyBookedSongsArray removeAllObjects];
            } else {
                NSString *msg = [results objectForKey:@"msg"];
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

-(void)onCollect:(SongsBookModel*) model
{
    if ([UserSessionManager isUserAlreadyLogin]) {
        NSString *encodeUrl = [PKtvAssistantAPI getAddCollectionSongUrl:[UserSessionManager getInstance].currentUser.uid songId:model.songInfo.songId];
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
                }
                [self errorMessageShow:msg];
            }
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            PLog(@"doAddCollectionSong failed...");
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        
        [operation start];
        
        
    }
}

#pragma mark tableViewDelegate

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (_type == Search_edit) {
        return _searchDataArray.count;
    }else{
        return _dataArray.count;
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *identifier = @"CellID";
    SongsBookCell * cell;
    if ([_songsList isEqual:tableView])
    {
        cell = (SongsBookCell *) [tableView dequeueReusableCellWithIdentifier:identifier];
        if (cell == nil)
        {
            cell = [[SongsBookCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identifier];
        }
        if (_type == Search_edit) {
            SongsBookModel * model = [_searchDataArray objectAtIndex:indexPath.row];
            cell.model = model;
        }else{
            SongsBookModel * model = [_dataArray objectAtIndex:indexPath.row];
            cell.model = model;
        }
        cell.delegate = self;
    }
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    SongsBookModel * model = nil;
    
    CGFloat distanceToButtom = _songsList.frame.size.height - (indexPath.row * 50 - _songsList.contentOffset.y);
    
    if (_type == Search_normal) {
        model = [_dataArray objectAtIndex:indexPath.row];
    }else{
        model = [_searchDataArray objectAtIndex:indexPath.row];
    }
    if (model.isHigher == NO) {
        
        if (_type == Search_normal) {
            for (SongsBookModel * aModel in _dataArray) {
                if (aModel.isHigher == YES) {
                    aModel.isHigher = NO;
                }
            }
        }else {
            for (SongsBookModel * aModel in _searchDataArray) {
                if (aModel.isHigher == YES) {
                    aModel.isHigher = NO;
                }
            }
        }
        
        if (distanceToButtom < 99) {
            _songsList.contentOffset = CGPointMake(_songsList.contentOffset.x, _songsList.contentOffset.y + 99 - distanceToButtom);
        }
        self.presentRow = indexPath.row;
        
        model.isHigher = YES;
    }else {
        model.isHigher = NO;
    }
    
    [_songsList reloadData];
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    SongsBookModel * model = nil;
    if (_type == Search_normal) {
        model = [_dataArray objectAtIndex:indexPath.row];
    }else{
        model = [_searchDataArray objectAtIndex:indexPath.row];
    }
    
    if (model.isHigher == YES) {
        return 100;
    }else {
        return 50;
    }
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView{
    if (scrollView == _songsList) {
        
        if (_type == Search_edit) {
            return;
        }
        
        if (_songsList.contentOffset.y + _songsList.frame.size.height > _songsList.contentSize.height) {
            [self loadDataFromServicer];
        }
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
    
    [self presentViewController:_reader animated:YES completion:nil];
    //[self presentModalViewController: _reader animated: YES];
}

#pragma mark -  取消二维码扫描

- (void)cancelAction:(UIButton *)button
{
    [_reader dismissViewControllerAnimated: YES completion:nil];
    //[_reader dismissModalViewControllerAnimated:YES];
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
    
    [reader dismissViewControllerAnimated: YES completion:nil];
    //[reader dismissModalViewControllerAnimated: YES];
    
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
//            [self.emptyMsgView setHidden:NO];
//            [self.mainContentView bringSubviewToFront:self.emptyMsgView];
        }else{
            //[self showNetwork];
            [self showEmptyTip:YES msg:RES_NO_NETWORK];
        }
        return;
    }
    
    _isLoadData = YES;
    
    //getGetSongListByTopic
    //NSString * urlStr = [PKtvAssistantAPI getGetSongListByTopics: self.modelID topicId:self.topicID begin:_count*20+1 num:20];
    NSString* urlStr = [PKtvAssistantAPI getGetSongListByTopics3:self.modelID topicId:self.topicID singerId:self.artistID begin:_count*20+1 num:20];
    
    NSURL *url = [NSURL URLWithString:urlStr];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        PLog(@"doGetSongListByTopic complete...");
        NSDictionary *results = nil;
        if(self.modelID == 3)
            results = [PCommonUtil NSDictionaryFromJSONData3:responseObject];
        else
            results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            _next = YES;
            
            NSDictionary * result = [results objectForKey:@"result"];
            
            //这里其实是获取到SongList，不过由于web比较麻烦，所以这里客户端更改
            NSArray * songList = [result objectForKey:@"Songlist"];
            
            if (songList.count < 20) {
                _noData = YES;
            }
            for (NSDictionary * dic in songList) {
                SongsBookModel * model = [[SongsBookModel alloc]init];
                model.cellType = TYPECELL_CommonOrderSong;
                
                SongInfo * info = [SongInfo initWithDictionary:dic];
                
                model.songInfo = info;
                
                //检查数据是否正常
                if(model.songInfo.songId == 0 || [model.songInfo.songName compare:@""] == NSOrderedSame || [model.songInfo.songArtist compare:@""] == NSOrderedSame)
                    continue;
                
                [_dataArray addObject:model];
            }
            
            if(_dataArray.count == 0)
                [self showEmptyTip:YES msg:RES_NO_DATA];
            else
            {
                [self showEmptyTip:NO msg:nil];
                [self markAlreadyOrderedSongs];
                [self getAlreadyOrderedSongsNumber];
            }
            _count ++;
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doGetSongListByTopic failed...%@", msg);
            
            if (_next == NO) {
                [self showEmptyTip:YES msg:msg];
            }
        }
        _isLoadData = NO;
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        _isLoadData = NO;
        PLog(@"doGetSongListByTopic failed...");
        
        if (_next == NO) {
            
            [self showEmptyTip:YES msg:RES_NO_NETWORK];
//            [self.emptyMsgView setHidden:NO];
//            [self.mainContentView bringSubviewToFront:self.emptyMsgView];
        }
    }];
    [self startWait];
    [operation start];
}

- (void)loadSearchDataFromService{
    
    [_searchDataArray removeAllObjects];
    
    Reachability *reachability = [Reachability reachabilityForInternetConnection];
    if (![reachability isReachable])
    {
        _type = Search_edit;
        [_songsList reloadData];
        
        [self showEmptyTip:YES msg:RES_NO_NETWORK];
        return;
    }
    
    //NSString * encodeUrl = [PKtvAssistantAPI getSongSearchUrl:[UserSessionManager getInstance].currentRoomInfo.roomId keyword:_keyword modelId:self.modelID topicId:self.topicID begin:1 num:200];
    NSString * encodeUrl = [PKtvAssistantAPI getGetSearchSongListByTopic:self.modelID topicId:self.topicID singerId:self.artistID key:_keyWord begin:1 num:200];
    
    NSURL *url = [NSURL URLWithString:encodeUrl];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        PLog(@"doSongSearch complete...");
        NSDictionary *results = NULL;
        if(self.modelID == kPKtvAssistantAPIModelTypeArtist)//如果是歌星页面，则不解密
            results = [PCommonUtil NSDictionaryFromJSONData3:responseObject];
        else
            results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            
            
            NSDictionary *result = [results objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"Songlist"];
            for (NSDictionary * dic in dataList) {
                SongsBookModel * model = [[SongsBookModel alloc]init];
                model.cellType = TYPECELL_CommonOrderSong;
                
                SongInfo * info = [SongInfo initWithDictionary:dic];
                model.songInfo = info;
                [_searchDataArray addObject:model];
            }
            
            if(_searchDataArray.count == 0)
            {
                [self showEmptyTip:YES msg:RES_NO_DATA];
            }
            else
            {
                [self showEmptyTip:NO msg:nil];
                [self markSearchedAlreadyOrderedSongs];
                [self doSearch];
            }
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doSongSearch failed...%@", msg);
            _type = Search_edit;
            [_songsList reloadData];
            
            
            [self showEmptyTip:YES msg:msg];
//            [self.emptyMsgView setHidden:NO];
//            [self.mainContentView bringSubviewToFront:self.emptyMsgView];
        }
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        PLog(@"doSongSearch failed...");
        _type = Search_edit;
        [_songsList reloadData];
        
        [self showEmptyTip:YES msg:RES_NO_NETWORK];
//        [self.emptyMsgView setHidden:NO];
//        [self.mainContentView bringSubviewToFront:self.emptyMsgView];
    }];
    [operation start];

}

- (void)markAlreadyOrderedSongs{
    
    VECTREQSONGLIST vectOrderList = [NetUtilCallBack getInstance]->m_vectreqsonglist;
    
    for (SongsBookModel * model in _dataArray) {
        model.songInfo.isOrdered = NO;
        
        VECTREQSONGLIST::iterator it = vectOrderList.begin();
        for(;it!=vectOrderList.end();it++)
        {
            if(it->songid == model.songInfo.songId)
            {
                model.songInfo.isOrdered = YES;
            }
        }
    }
    
    [_songsList reloadData];
}

#pragma mark NetUtilCallBackProtocol
-(void)closeRoom
{
    [self performSelectorOnMainThread:@selector(dealWithSonglist) withObject:nil waitUntilDone:NO];
}
-(void)OnRoomTunnelClose
{
    [self performSelectorOnMainThread:@selector(dealWithSonglist) withObject:nil waitUntilDone:NO];
}
-(void)pushReqSongInfo:(const ResultData_SongList*)pSonglist :(int)count
{
    [super pushReqSongInfo:pSonglist :count];
    [self performSelectorOnMainThread:@selector(dealWithSonglist) withObject:nil waitUntilDone:NO];
}

-(void)dealWithSonglist
{
    [self markSearchedAlreadyOrderedSongs];
    [self markAlreadyOrderedSongs];
}

- (void)markSearchedAlreadyOrderedSongs{
    VECTREQSONGLIST vectOrderList = [NetUtilCallBack getInstance]->m_vectreqsonglist;
    
    for (SongsBookModel * model in _searchDataArray) {
        model.songInfo.isOrdered = NO;
        
        VECTREQSONGLIST::iterator it = vectOrderList.begin();
        for(;it!=vectOrderList.end();it++)
        {
            if(it->songid == model.songInfo.songId)
            {
                model.songInfo.isOrdered = YES;
            }
        }
    }
    
    [_songsList reloadData];
}

#pragma mark - UISearchBarDelegate methods

-(void) searchBarSearchButtonClicked:(UISearchBar *)theSearchBar
{
    self.keyWord = [theSearchBar text];
    if (self.keyWord.length !=0) {
//        if (self.modelID == kPKtvAssistantAPIModelTypeArtist) {
//            [self doSearch];
//        }else{
//            [self loadSearchDataFromService];
//        }
        [self loadSearchDataFromService];
    }
    [theSearchBar resignFirstResponder];
}

- (void)searchBarTextDidBeginEditing:(UISearchBar *)theSearchBar
{
    
    [theSearchBar setShowsCancelButton:YES animated:YES];
    _songSearchBar.placeholder = @"请输入搜索内容";
    self.view_loading.hidden = NO;
    [self.mainContentView bringSubviewToFront:self.view_loading];
    
}

- (void)searchBarTextDidEndEditing:(UISearchBar *)theSearchBar
{
    [theSearchBar setShowsCancelButton:NO animated:YES];
    [self.mainContentView sendSubviewToBack:self.view_loading];
}

- (BOOL)searchBar:(UISearchBar *)theSearchBar shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text{
    if (range.location != NSNotFound) {
        self.keyWord = [theSearchBar.text stringByReplacingCharactersInRange:range withString:text];
        self.keyWord = [self.keyWord stringByReplacingOccurrencesOfString:@" " withString:@""];
    }
    if (self.keyWord.length != 0 && ![self isChinese:self.keyWord]) {
        //_type = Search_edit;
    }else{
        [self.mainContentView bringSubviewToFront:self.view_loading];
    }
    return YES;
}
//处理 点击右边小叉
//- (void)searchBar:(UISearchBar *)theSearchBar textDidChange:(NSString *)searchText{
//    _songSearchBar.placeholder = @"";
//    if([searchText isEqualToString:@""]){
//        _type = Search_normal;
//    }else {
//        //_type = Search_edit;
//    }
//    
//    _songSearchBar.placeholder = @"请输入歌曲名或歌手名";
//    
//    
//}
- (void)searchBarCancelButtonClicked:(UISearchBar *) theSearchBar{
    _songSearchBar.text = @"";
    [self dismissKeyboard];
}
-(void)dismissKeyboard{
    if ([_songSearchBar.text length] == 0) {
        _type = Search_normal;
        //[self.songsList bringSubviewToFront:_songSearchBar];
        [_songsList reloadData];
        [self.mainContentView sendSubviewToBack:self.emptyTips];
//        [self.emptyMsgView setHidden:YES];
//        [self.mainContentView sendSubviewToBack:self.emptyMsgView];
    }
    [_songSearchBar resignFirstResponder];
    
    //[_songsList reloadData];
    //PLog(@"——————————————>%d",_type);
}

//数字0--9的ASCII码 48--57
//中文ASCII码 0x4e00--0x9fff
//ASCⅡ码在97和122之间（包括97和122,为小写英文字母a——z)；
//ASCⅡ码在65和90之间（包括65和90，为大写英文字母A——Z）。
-(BOOL)isChinese:(NSString *) searchKey{
    int len = searchKey.length;
    if (searchKey.length > 0) {
        for(int i=0; i< len;i++){
            int a = [searchKey characterAtIndex:i];
            if(!(a >= 65 && a <= 90) && !(a >= 97 && a <= 122)){        //非英文字母
                return YES;
            }
        }
    }
    return NO;
}

-(void)doSearch
{
    if (self.modelID == kPKtvAssistantAPIModelTypeArtist) {
        [self.searchDataArray removeAllObjects];
        
        for (int i = 0; i < [_dataArray count]; i++) {
            SongsBookModel * model = [_dataArray objectAtIndex:i];
            NSString *songArtist = model.songInfo.songArtist;
            NSString *songName = model.songInfo.songName;
            if ([songName hasPrefix:self.keyWord] || [songArtist hasPrefix:self.keyWord]) {
                [_searchDataArray addObject:model];
            }
        }
    }
    
    if (_searchDataArray.count >0) {
        self.emptyTips.hidden = YES;
        [self.mainContentView sendSubviewToBack:self.emptyTips];
        
    }else{
        if(self.emptyTips==nil){
            self.emptyTips = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 14)];
            self.emptyTips.center = CGPointMake(160.0f, 208.0f);
            self.emptyTips.backgroundColor = [UIColor clearColor];
            self.emptyTips.text = @"对不起，没有找到对应的内容";
            self.emptyTips.textAlignment = NSTextAlignmentCenter;
            self.emptyTips.textColor = [UIColor grayColor];
            self.emptyTips.font = [UIFont systemFontOfSize:14];
            [self.mainContentView addSubview:self.emptyTips];
        }
        self.emptyTips.hidden =NO;
        [self.mainContentView bringSubviewToFront:self.emptyTips];
        
    }
    _type = Search_edit;
    [_songsList reloadData];
    
}


#pragma mark viewDidLoad

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    
    [self setNavigationView];
    [self creatFlashView];
    
    _dataArray = [[NSMutableArray alloc]init];
    _alreadyBookedSongsArray = [[NSMutableArray alloc]init];
    _searchDataArray = [[NSMutableArray alloc]init];
    _errorRemindView = [[UIAlertView alloc]initWithTitle:nil message:nil delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil];
    
    [self creatSongsList];
    [self creatSongSearchBar];
    [self loadDataFromServicer];
    //[self loadAlreadyBookedSongs];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    [self.appDelegate.menuController setEnableGesture:NO];
    [self getAlreadyOrderedSongsNumber];
    
    [_alreadyBookedSongsArray removeAllObjects];
    
    [self setRightAndBadgeButton];
    //[self loadDataFromServicer];
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
