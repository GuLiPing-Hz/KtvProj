//
//  UserCollectedSongViewController.m
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-16.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "UserCollectedSongViewController.h"
#import "PKtvAssistantAPI.h"
#import "SongsBookModel.h"
#import "DDMenuController.h"
#import "KTVAppDelegate.h"
#import "iToast.h"
#import "CMActionSheet.h"
#import "KTVInputSecretController.h"
#import "RegexKitLite.h"
//#import "BPush.h"
#import "CustomScroll.h"
#import "xingguang.h"
#import "NetUtilCallBack.h"
#import "com_ios_NetUtil.h"

@interface UserCollectedSongViewController ()

@end

@implementation UserCollectedSongViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

#pragma mark setView

- (void)setNavigationView
{
    [self.navView initOnlyBackButton:@"我的收藏"];
}


- (void)createCollectedList
{
    _collectedSongs = [[UITableView alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height-50) style:UITableViewStylePlain];

    _collectedSongs.delegate = self;
    _collectedSongs.dataSource = self;
    _collectedSongs.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    _collectedSongs.backgroundColor = UIColorFromRGB(0xf0f1f5);
    
    [self.mainContentView addSubview:_collectedSongs];
    
    UIView * freeView = [[UIView alloc]init];
    _collectedSongs.tableFooterView = freeView;
}

- (void)errorMessageShow:(NSString *)errorMessage{
    [self.errorMessageView setTitle:errorMessage];
    [self.errorMessageView show];
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
    SongsBookCell * cell = [_collectedSongs dequeueReusableCellWithIdentifier:@"cell"];
    if (cell == nil) {
        cell = [[SongsBookCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
    }
    
    SongsBookModel * model  = [_dataArray objectAtIndex:indexPath.row];
    cell.model = model;
    cell.delegate = self;
    
    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    SongsBookModel * model = [_dataArray objectAtIndex:indexPath.row];
    
    if (model.isHigher == YES) {
        return 100;
    }else {
        return 50;
    }
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    SongsBookModel * model = [_dataArray objectAtIndex:indexPath.row];
    
    CGFloat distanceToButtom = _collectedSongs.frame.size.height - (indexPath.row * 50 - _collectedSongs.contentOffset.y);
    
    if (model.isHigher == NO) {
        
        for (SongsBookModel * aModel in _dataArray) {
            if (aModel.isHigher == YES) {
                aModel.isHigher = NO;
            }
        }
        
        if (distanceToButtom < 99) {
            _collectedSongs.contentOffset = CGPointMake(_collectedSongs.contentOffset.x, _collectedSongs.contentOffset.y + 99 - distanceToButtom);
        }
        self.presentRow = indexPath.row;
        
        model.isHigher = YES;
    }else {
        model.isHigher = NO;
    }
    
    [_collectedSongs reloadData];
}

#pragma mark SongOperation
-(void)onOrder:(SongsBookModel*) info view:(id)cell point:(CGPoint)point;
{
    if ([UserSessionManager isRoomAlreadyLogin]) {
        self.cdFlashStart = [cell convertPoint:point toView:_collectedSongs];
        self.cdFlashStart = CGPointMake(self.cdFlashStart.x, self.cdFlashStart.y-_collectedSongs.contentOffset.y);
        
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
                //
                info.songInfo.isOrdered = YES;
                [_collectedSongs reloadData];
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
                PLog(@"doOrderSong failed...%@", msg);
                //[_errorRemindView show];
                NSString *errorCode = [results objectForKey:@"errorcode"];
                if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForSongExisted]||[errorCode isEqualToString:kPKtvAssistantErrorCodeForSongPlaying]) {
                    info.songInfo.isOrdered = YES;
                    [_collectedSongs reloadData];
                    [_alreadyBookedSongsArray removeAllObjects];
                    [self loadAlreadyBookedSongs];
                    //[SVProgressHUD showSuccessWithStatus:@"点歌成功"];
                    [self cdViewFlash];
                    
                    return;
                }
                [self errorMessageShow:msg];
            }
            
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            PLog(@"doOrderSong failed...");
            //[_errorRemindView show];
            
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        [operation start];

    }else{
        [self errorMessageShow:@"请先进入包厢"];
    }
}

- (void)onCollect:(SongsBookModel *)info{
    if ([UserSessionManager isUserAlreadyLogin]) {
        NSString *encodeUrl = [PKtvAssistantAPI getDeleteCollectionSongUrl:[UserSessionManager getInstance].currentUser.uid songId:info.songInfo.songId];
        NSURL *url = [NSURL URLWithString:encodeUrl];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
        
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            PLog(@"doDeleteCollectionSong complete...");
            //NSString *response = [resquest responseString];
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
            
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                NSDictionary *result = [results objectForKey:@"result"];
                BOOL res = [[result objectForKey:@"success"] boolValue];
                if(res){
                    PLog(@"删除收藏成功");
                    [SVProgressHUD showSuccessWithStatus:@"删除收藏成功"];
                }else{
                    PLog(@"删除收藏失败");
                    [SVProgressHUD showSuccessWithStatus:@"删除收藏失败"];
                }
                [self loadDataFromService];
                
            } else {
                NSString *msg = [results objectForKey:@"msg"];
                NSString *errorCode = [results objectForKey:@"errorcode"];
                PLog(@"doAddCollectionSong failed...errorcode:%@(%@)", errorCode, msg);
                if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForSongCollectedError]) {
                    [self loadDataFromService];
                    
                    return;
                }
                [self errorMessageShow:msg];
            }
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            PLog(@"doDeleteCollectionSong failed...");
            
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        
        [operation start];

    }else{
        [self errorMessageShow:@"请先用户登录"];
    }
}

- (void)onTop:(SongsBookModel *)info{
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
                    info.songInfo.isOrdered = YES;
                    [_collectedSongs reloadData];
                    
                    [_alreadyBookedSongsArray removeAllObjects];
                    [self loadAlreadyBookedSongs];
                    
                    [SVProgressHUD showSuccessWithStatus:@"置顶成功"];
                    //通知更新金币信息
                    NSString *result  = [results objectForKey:@"result"];
                    if (![result isEqualToString:@"error"] && [result integerValue]>0)
                    {
                        [[UserSessionManager getInstance] updateCurrentUserGold:[result integerValue]];
                    }
                    
                } else {
                    NSString *msg = [results objectForKey:@"msg"];
                    PLog(@"doAddAndSetSongToTop failed...%@", msg);
                    //[_errorRemindView show];
                    
                    [self errorMessageShow:msg];
                }
                
            } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
                PLog(@"doAddAndSetSongToTop failed...");
                //[_errorRemindView show];
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
                    
                    [self errorMessageShow:msg];
                }
                
            } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
                PLog(@"doSetSongToTop failed...");
                [self errorMessageShow:@"系统异常"];
            }];
            [operation start];
        }
        
    }else{
        [self errorMessageShow:@"请先进入包厢"];
    }
}

#pragma mark loadData

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
    
    [_collectedSongs reloadData];
}

- (void)loadDataFromService{
    Reachability *reachability = [Reachability reachabilityForInternetConnection];
    if (![reachability isReachable])
    {
        if (_dataArray.count == 0) {
            [self.emptyMsgView setHidden:NO];
            [self.mainContentView bringSubviewToFront:self.emptyMsgView];
        }
        self.emptyTips.hidden =YES;
        return;
    }
    
    NSString * urlStr = [PKtvAssistantAPI getUserCollectedListUrl:[UserSessionManager getInstance].currentUser.uid begin:1 num:200];
    
    //PLog(@"%@",urlStr);
    NSURL *url = [NSURL URLWithString:urlStr];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        PLog(@"doGetUserCollectedList complete...");
        
        
        //NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            [_dataArray removeAllObjects];
            
            NSDictionary * result = [results objectForKey:@"result"];
            NSArray * dataList = [result objectForKey:@"songlists"];
            if (dataList.count == 0) {
                if(self.emptyTips==nil){
                    self.emptyTips = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 14)];
                    self.emptyTips.center = CGPointMake(160.0f, 208.0f);
                    self.emptyTips.backgroundColor = [UIColor clearColor];
                    self.emptyTips.text = @"您尚未收藏歌曲";
                    self.emptyTips.textAlignment = NSTextAlignmentCenter;
                    self.emptyTips.textColor = [UIColor grayColor];
                    self.emptyTips.font = [UIFont systemFontOfSize:14];
                    [self.mainContentView addSubview:self.emptyTips];
                }
                self.emptyTips.hidden =NO;
                [self.mainContentView bringSubviewToFront:self.emptyTips];
                
                [_collectedSongs reloadData];
                return;
            }
            
            self.emptyTips.hidden =YES;
            [self.mainContentView sendSubviewToBack:self.emptyTips];
            
            for (NSDictionary * dic in dataList) {
                SongsBookModel * model = [[SongsBookModel alloc]init];
                
                SongInfo * info = [SongInfo initWithDictionary:dic];
                model.songInfo = info;
                model.cellType = TYPECELL_CollectSong;
                [_dataArray addObject:model];
            }
            
            [_collectedSongs reloadData];
            [self markAlreadyOrderedSongs];
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doGetUserCollectedList failed...errorcode:%@(%@)", errorCode, msg);
            [self.emptyMsgView setHidden:NO];
            [self.mainContentView bringSubviewToFront:self.emptyMsgView];
        }
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        PLog(@"doGetUserCollectedList failed...");
        [self.emptyMsgView setHidden:NO];
        [self.mainContentView bringSubviewToFront:self.emptyMsgView];
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


#pragma mark - viewDidLoad

- (void)viewDidLoad
{
    [super viewDidLoad];
	
    self.view.backgroundColor = UIColorFromRGB(0xf0f1f5);
    [self setNavigationView];
    
    _errorMessageView = [[UIAlertView alloc]initWithTitle:nil message:nil delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil];
    
    _dataArray = [[NSMutableArray alloc]init];
    _alreadyBookedSongsArray = [[NSMutableArray alloc]init];
    
    [self createCollectedList];
    
    [self creatEmptyMsgView];
    
    [self creatFlashView];
    
//    self.toolBar.hidden = NO;
//    self.joinButton.hidden = NO;
//    // 进入包厢
//    [self.joinButton addTarget:self action:@selector(inRoom:) forControlEvents:UIControlEventTouchUpInside];
//    
//    // 退出包厢
//    [self.power addTarget:self action:@selector(exit_Room:) forControlEvents:UIControlEventTouchUpInside];
    
    //[self flashUIWithRoom]; // 退出包厢刷新UI
}



#pragma mark -  UIActionSheet 进入包厢

- (void)inRoom:(UIButton *)btn

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

- (void)exit_Room:(UIButton *)button
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
                     [SVProgressHUD dismissWithError:@"退出包厢失败"];
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
            
            [self getAlreadyOrderedSongsNumber];
            
            [self setRightAndBadgeButton];
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
             
             NetUtilCallBack* pNetUtil = [NetUtilCallBack getInstance];
             [pNetUtil setRoomId:[roominfo.xgRoomID intValue]];
             [pNetUtil doCurrentWork:TransactionRoomLogin listener:^(BOOL success) {
                 if(success)
                 {
                     [self setRightAndBadgeButton];
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
//    if ([UserSessionManager isRoomAlreadyLogin])   // 已进入包厢
//    {
//        NSString *roomName = [UserSessionManager getInstance].currentRoomInfo.roomName;
//        self.inRoomNumber.text = [NSString stringWithFormat:@"所在包厢: %@",roomName];
//        self.joinButton.hidden = YES;
//        self.power.hidden = NO;
//        self.inRoomNumber.hidden = NO;
//    }
//    else  // 未进入包厢
//    {
//        self.joinButton.hidden = NO;
//        self.power.hidden = YES;
//        self.inRoomNumber.hidden = YES;
//    }
}

#pragma mark - viewWillAppear

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:NO];
    [self flashUIWithRoom]; // 退出包厢刷新UI
    
    self.emptyMsgView.hidden = YES;
    
    [self loadDataFromService];
    
    [self loadAlreadyBookedSongs];
    
    [self getAlreadyOrderedSongsNumber];
    
    [self markAlreadyOrderedSongs];
    
    [self setRightAndBadgeButton];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(disconnRoom:) name:KTVASSISTANT_CLOSEROOM_NOTIFICATION object:nil];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:YES];
    [[NSNotificationCenter defaultCenter] removeObserver:self];
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

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
