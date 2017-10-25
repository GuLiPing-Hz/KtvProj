//
//  BookedAndCollectedController.m
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-4.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BookedAndCollectedController.h"
#import "KTVBaseConfig.h"
#import "PKtvAssistantAPI.h"
#import "ASIHTTPRequest.h"
#import "SSImageCategory.h"
#import "PKtvAssistantAPI.h"
#import "ASIHTTPRequest.h"
#import "SongsBookModel.h"
#import "DDMenuController.h"
#import "KTVAppDelegate.h"
#import "CustomScroll.h"
#import "SongSetView.h"
#import "NetUtilCallBack.h"
#import "CResponse.h"
#import "NetUtilCallBack.h"


#define SCREEN_EXTRA_HEIGHT ([UIScreen mainScreen].bounds.size.height - 480.f)

@interface BookedAndCollectedController ()
@property (nonatomic ,assign) int ShowType;
@property (nonatomic ,retain) UIView *songSetView;
@end

@implementation BookedAndCollectedController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}


#pragma mark - add View

-(void)addOpenSetView
{
    _ShowType=0;
    _showSetBtn =[UIButton buttonWithType:UIButtonTypeCustom];
    _showSetBtn.frame = CGRectMake(20, self.mainContentView.frame.size.height-20-14, 25, 19);
    [_showSetBtn setImage:[UIImage imageNamed:@"btn_panelOpen@2x.png"] forState:UIControlStateNormal];
    [_showSetBtn addTarget:self action:@selector(showSongSetView) forControlEvents:UIControlEventTouchUpInside];
    [self.mainContentView addSubview:_showSetBtn];
    
    
    NSArray *view = [[NSBundle mainBundle] loadNibNamed:@"SongSetView" owner:self options:nil];
    for (int i = 0; i<view.count; i++) {
        SongSetView *songview = view[i];
        [songview setFrame:CGRectMake(_MainScreen_Width*i, 0, _MainScreen_Width, 250)];
        [songview setAllBtnBack];
    }
    _scroll = [[CustomScroll alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 270) views:[[NSMutableArray alloc] initWithArray:view]];
    _scroll.pageControl.selectColor = [UIColor whiteColor];
    _scroll.pageControl.freeColor =  [UIColor blackColor];
    _scroll.backgroundColor = [UIColor clearColor];
    UIView *view1 = [[UIView alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 270)];
    view1.backgroundColor = UIColorFromRGB(0x2a2a2a);
    view1.alpha = 0.9;
    _setPadView= [[UIView alloc]initWithFrame:CGRectMake(0, self.mainContentView.frame.size.height - 50, _MainScreen_Width, 270)];
    _setPadView.backgroundColor = [UIColor clearColor];
    _setPadView.hidden = YES;
    [_setPadView addSubview:view1];
    [_setPadView addSubview:_scroll];
    [self.mainContentView insertSubview:_setPadView belowSubview:self.toolBar];
}

-(void)showSongSetView
{
    if(_ShowType == 0)
    {
        _ShowType= -1;
        _setPadView.hidden = NO;
        [_showSetBtn setImage:nil forState:UIControlStateNormal];
        [_showSetBtn setImage:[UIImage imageNamed:@"btn_panelClose@2x.png"] forState:UIControlStateNormal];
        [UIView animateWithDuration:0.5 delay:0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
            _setPadView.frame = CGRectMake(0, self.mainContentView.frame.size.height - 320, _MainScreen_Width, 270);
        } completion:^(BOOL finished){
            _ShowType= 1;
        }];
    }
    else if(_ShowType == 1)
    {
        _ShowType= -1;
        [self.mainContentView bringSubviewToFront:self.toolBar];
        [_showSetBtn setImage:nil forState:UIControlStateNormal];
        [_showSetBtn setImage:[UIImage imageNamed:@"btn_panelOpen@2x.png"] forState:UIControlStateNormal];
        [UIView animateWithDuration:0.5 delay:0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
            _setPadView.frame = CGRectMake(0, self.mainContentView.frame.size.height - 50, _MainScreen_Width, 270);
        } completion:^(BOOL finished){
            _setPadView.hidden = YES;
            _ShowType= 0;
        }];
    }
    
}

- (void)setNavigationView{
    self.navView.hidden = NO;
    self.navView.titleLabel.text = @"已点歌曲列表";
    self.navView.backButton.hidden = NO;
    
}


- (void)creatSongsList{
    _songsList = [[UITableView alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height-BUTTOM_VIEW_HEIGHT) style:UITableViewStylePlain];
    _songsList.separatorStyle = UITableViewCellSeparatorStyleNone;
    [_songsList setBackgroundColor:UIColorFromRGB(0xf0f1f5)];
    
    _dataArray = [[NSMutableArray alloc]init];
    //_cellConfigArray = [[NSMutableArray alloc]init];
    
    _songsList.delegate = self;
    _songsList.dataSource = self;
    
    UIView *spaceView = [[UIView alloc] init];
    _songsList.tableFooterView = spaceView;
    
    //[self.mainContentView addSubview:_songsList];
    [self.mainContentView insertSubview:_songsList belowSubview:self.toolBar];
    
}

- (void)creatSingingView{
    _singingView = [[UIImageView alloc]init];
    _singingView.frame = CGRectMake(0, _songsList.frame.origin.y+_songsList.frame.size.height, _MainScreen_Width, BUTTOM_VIEW_HEIGHT);
    _singingView.image = [UIImage imageNamed:@"bottom_bar.png"];
    _singingView.userInteractionEnabled = YES;
    
    [self.mainContentView addSubview:_singingView];
    
    _changeSong = [UIButton buttonWithType:UIButtonTypeCustom];
    _changeSong.frame = CGRectMake(_MainScreen_Width-110, 9, 82, BUTTOM_VIEW_HEIGHT-9);
    [_changeSong setBackgroundImage:[UIImage imageNamed:@"btn_qiege.png"] forState:UIControlStateNormal];
    [_changeSong setBackgroundImage:[UIImage imageNamed:@"btn_qiege_c.png"] forState:UIControlStateHighlighted];
    [_changeSong addTarget:self action:@selector(changeSong) forControlEvents:UIControlEventTouchUpInside];
    
    [_singingView addSubview:_changeSong];
    
    
}

#pragma mark - 切歌
- (void)changeSong{
    if ([UserSessionManager isRoomAlreadyLogin]) {
        if (self.nowSongId == nil || self.isChanging == YES) {
            return;
        }
        
        self.isChanging = YES;
        
        NSString *encodeUrl = [PKtvAssistantAPI getCutSongUrl:[UserSessionManager getInstance].currentUser.uid ktvID:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword songID:(long)self.nowSongId.songId];
        NSURL *url = [NSURL URLWithString:encodeUrl];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
        
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                
                [SVProgressHUD showSuccessWithStatus:@"切歌成功"];
                
                self.isChanging = NO;
            } else {
                
                NSString *msg = [results objectForKey:@"msg"];
                NSString *errorCode = [results objectForKey:@"errorcode"];
                if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForSongError]) {
                    self.isChanging = NO;
                    return;
                }
                
                [self errorMessageShow:msg];
                self.isChanging = NO;
            }
            
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            
            PLog(@"doCutSong failed...");
            
            [CommenTools showNetworkDisconnectedAlertView];
            self.isChanging = NO;
        }];
        [operation start];

    }else{
        
        [CommenTools showAlertViewWithErrorMessage:@"您的包厢已经到期，请退出包厢"];
        
    }
}


- (void)errorMessageShow:(NSString *)errorMessage{
    [_errorRemindView setTitle:errorMessage];
    [_errorRemindView show];
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

#pragma mark - tableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return self.dataArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    static NSString* reuseid = @"OrderedSong";
    OrderedSongCell* cell = [_songsList dequeueReusableCellWithIdentifier:reuseid];
    if (cell == nil) {
        [[NSBundle mainBundle] loadNibNamed:@"OrderedSongCell" owner:self options:nil];
        cell = self.mUiSongCell;
        cell.mCallBack = self;
    }
    
    PLog(@"%f,%f",cell.frame.origin.x,cell.frame.origin.y);
    [cell setInfo:[_dataArray objectAtIndex:indexPath.row] withIndex:indexPath.row];
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
    KTVOrderedSongInfo * info = [_dataArray objectAtIndex:indexPath.row];
    
    CGFloat distanceToButtom = _songsList.frame.size.height - (indexPath.row * 50 - _songsList.contentOffset.y);
    
    if (info.isHigher == NO) {
        
        for (KTVOrderedSongInfo * aModel in _dataArray) {
            if (aModel.isHigher == YES) {
                aModel.isHigher = NO;
            }
        }
        if (distanceToButtom < 99) {
            _songsList.contentOffset = CGPointMake(_songsList.contentOffset.x, _songsList.contentOffset.y + 99 - distanceToButtom);
        }
        
        info.isHigher = YES;
    }else {
        info.isHigher = NO;
    }
    
    [_songsList reloadData];
}

#pragma mark - viewDidLoad

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    
    //[self creatTopButton];
    
    _dataArray = [[NSMutableArray alloc]init];
    
    
    [self setNavigationView];
    
    _errorRemindView = [[UIAlertView alloc]initWithTitle:nil message:nil delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil];
    
    [self creatSongsList];
    
    [self creatSingingView];
    
    [self creatEmptyMsgView];
    
    [self addOpenSetView];
    
    [self loadOrderedSong];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:NO];
    [_reloadingData setFireDate:[NSDate distantPast]];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:YES];
    
    [_reloadingData setFireDate:[NSDate distantFuture]];
}

-(void)loadOrderedSong
{
    [self.dataArray removeAllObjects];
    VECTREQSONGLIST vectOrderedList = [NetUtilCallBack getInstance]->m_vectreqsonglist;
    
    VECTREQSONGLIST::iterator it = vectOrderedList.begin();
    BOOL isFirst = true;
    for(;it!=vectOrderedList.end();it++)
    {
        KTVOrderedSongInfo* info = [[KTVOrderedSongInfo alloc] init];
        info.songId = it->songid;
        info.songname = [NSString stringWithUTF8String:it->songname];
        info.singername = [NSString stringWithUTF8String:it->singername];
        info.isHigher = NO;
        [self.dataArray addObject:info];
        if(isFirst)
        {
            self.nowSongId = info;
            isFirst = false;
        }
    }
}

#pragma mark NetUtilCallBackProtocol
-(void)closeRoom
{
    [self performSelectorOnMainThread:@selector(onChageList) withObject:nil waitUntilDone:NO];
}
-(void)OnRoomTunnelClose
{
    [self performSelectorOnMainThread:@selector(onChageList) withObject:nil waitUntilDone:NO];
}
-(void)pushReqSongInfo:(const ResultData_SongList*)pSonglist :(int)count
{
    [super pushReqSongInfo:pSonglist :count];
    [self performSelectorOnMainThread:@selector(onChageList) withObject:nil waitUntilDone:NO];
}

-(void)onChageList
{
    [self loadOrderedSong];
    if(self.dataArray.count == 0)
    {
        self.nowSongId = nil;
        
        if(self.emptyTips==nil){
            self.emptyTips = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 14)];
            self.emptyTips.center = CGPointMake(160.0f, 208.0f);
            self.emptyTips.backgroundColor = [UIColor clearColor];
            self.emptyTips.text = @"您尚未点歌";
            self.emptyTips.textAlignment = NSTextAlignmentCenter;
            self.emptyTips.textColor = [UIColor grayColor];
            self.emptyTips.font = [UIFont systemFontOfSize:14];
            [self.mainContentView insertSubview:self.emptyTips belowSubview:_songSetView];
        }
        self.emptyTips.hidden =NO;
        [_songsList reloadData];
    }
    else
    {
        self.nowSongId = [self.dataArray objectAtIndex:0];
        [_songsList reloadData];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma SongOpreation

-(void)onOrderedTop:(KTVOrderedSongInfo*) info
{
    NSString *encodeUrl = [PKtvAssistantAPI getSetSongToTopUrl:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword songId:[NSString stringWithFormat:@"%d",info.songId]];
    
    NSURL *url = [NSURL URLWithString:encodeUrl];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            
            [SVProgressHUD showSuccessWithStatus:@"置顶成功"];
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForSongError]) {
                return;
            }
            PLog(@"doSetSongToTop failed...%@", msg);
            [self errorMessageShow:msg];
        }
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        PLog(@"doSetSongToTop failed...");
        [CommenTools showNetworkDisconnectedAlertView];
    }];
    [operation start];
}
-(void)onOrderedDelete:(KTVOrderedSongInfo*) info
{
    NSString *encodeUrl = [PKtvAssistantAPI getDeleteSongFromOrdered:[UserSessionManager getInstance].currentRoomInfo.ktvId roomId:[UserSessionManager getInstance].currentRoomInfo.roomId roomPassword:[UserSessionManager getInstance].currentRoomInfo.roomPassword songId:[NSString stringWithFormat:@"%d",info.songId]];
    NSURL *url = [NSURL URLWithString:encodeUrl];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            
            [SVProgressHUD showSuccessWithStatus:@"删除成功"];
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"deleteSongFromOrdered failed...%@", msg);
            NSString *errorCode = [results objectForKey:@"errorcode"];
            if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForSongError]) {
                
                [SVProgressHUD showSuccessWithStatus:@"删除成功"];
                
                return;
            }
            [self errorMessageShow:msg];
        }
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        PLog(@"deleteSongFromOrdered failed...");
        [CommenTools showNetworkDisconnectedAlertView];
    }];
    [operation start];
}

-(void)onOrderedCollect:(KTVOrderedSongInfo*) info
{
    if ([UserSessionManager isUserAlreadyLogin]) {
        NSString *encodeUrl = [PKtvAssistantAPI getAddCollectionSongUrl:[UserSessionManager getInstance].currentUser.uid songId:info.songId];
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
                [self errorMessageShow:msg];
            }
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            PLog(@"doAddCollectionSong failed...");
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        
        [operation start];
    }
}
@end
