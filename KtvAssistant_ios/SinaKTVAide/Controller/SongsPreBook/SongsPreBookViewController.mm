//
//  SongsPreBookViewController.m
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-10-23.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "SongsPreBookViewController.h"
#import "KTVSongPreBookCell.h"
#import "PDatabaseManager.h"

@interface SongsPreBookViewController ()

@end

@implementation SongsPreBookViewController

@synthesize preSongTable = _preSongTable,preSongDataArray = _preSongDataArray,emptyTips = _emptyTips;
@synthesize indicatorView = _indicatorView;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

-(void)doBack:(id)sender{
    
    if (getSongsAPI) {
        getSongsAPI.delegate = nil;
    }
    
    [super doBack:sender];
    
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    _preSongTable = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, self.mainContentView.frame.size.width, self.mainContentView.frame.size.height) style:UITableViewStylePlain];
    _preSongTable.separatorStyle = UITableViewCellSeparatorStyleNone;
    _preSongTable.backgroundColor = UIColorFromRGB(0xe6e7ee);
    _preSongTable.delegate = self;
    _preSongTable.dataSource = self;
    _preSongDataArray = [[NSMutableArray alloc] init];
    [self.mainContentView addSubview:_preSongTable];
    
    _indicatorView = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray];
    [_indicatorView setHidesWhenStopped:YES];
    [_indicatorView setCenter:CGPointMake(160, 200)];
    [self.mainContentView addSubview:_indicatorView];
    
    BOOL islogin = [[UserSessionManager getInstance] isLoggedIn];
    if (islogin) {
        self.navView.titleLabel.text = @"导入预点歌曲(共0首)";
        [self loadAlreadyDemandSongsData];
    }else{
        self.navView.titleLabel.text = @"预点歌列表(共0首)";
        [self readLoacalPreVodSongData];
    }
    
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:YES];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(readLoacalPreVodSongData) name:RECEIVE_DELETE_PRESONG_NOTIFICATION object:Nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(pquitBoxAction) name:RECEIVE_ROOM_HAS_CLOSED_NOTIFICATION object:nil];
    
}

-(void)viewWillDisappear:(BOOL)animated{
    [super viewWillDisappear:YES];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:RECEIVE_DELETE_PRESONG_NOTIFICATION object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:RECEIVE_ROOM_HAS_CLOSED_NOTIFICATION object:nil];
    
}

-(void)loadAlreadyDemandSongsData
{
    Reachability *reachability = [Reachability reachabilityForInternetConnection];
    if (![reachability isReachable])
    {
        UIAlertView *noNetworkAlert = [[UIAlertView alloc] initWithTitle:nil message:@"你的网络好像有问题，请重试！" delegate:self cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
        [noNetworkAlert show];
        return;
    }
    
    [_indicatorView startAnimating];
    
    RoomInfo *info = [[UserSessionManager getInstance] currentRoomInfo];
    getSongsAPI = [[KTVGetAlreadyDemandSongAPI alloc] init];
    getSongsAPI.delegate = self;
    [getSongsAPI doGetOrderedSongList:info.ktvId roomId:info.roomId roomPassword:info.roomPassword];
    
}

#pragma mark
#pragma KTVGetAlreadyDemandSongAPIDelegate methods

-(void)getAlreadyDemandSongWithSuccessResult:(id)result
{
    PLog(@"getAlreadyDemandSongWithSuccessResult");
    NSMutableArray *resultArray = (NSMutableArray *)result;
    PDatabaseManager *datamanager = [PDatabaseManager GetInstance];
    NSMutableArray *preSongDataArray = [datamanager  getSongInfoList:1000];
    for (int i = 0; i < [preSongDataArray count]; i++) {
        SongInfo *preInfo = [preSongDataArray objectAtIndex:i];
        for (SongInfo *newInfo in resultArray) {
            if (newInfo.songId == preInfo.songId) preInfo.isOrdered = YES;
        }
        [_preSongDataArray addObject:preInfo];
    }
    
    [_indicatorView stopAnimating];
    
    if ([_preSongDataArray count] == 0) {
        if(self.emptyTips==nil){
            self.emptyTips = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 14)];
            self.emptyTips.center = CGPointMake(160.0f, 208.0f);
            self.emptyTips.backgroundColor = [UIColor clearColor];
            self.emptyTips.text = @"您还没有预点歌曲";
            self.emptyTips.textAlignment = UITextAlignmentCenter;
            self.emptyTips.textColor = [UIColor grayColor];
            self.emptyTips.font = [UIFont systemFontOfSize:14];
            [self.mainContentView addSubview:self.emptyTips];
        }
        self.emptyTips.hidden =NO;
        [self.mainContentView bringSubviewToFront:self.emptyTips];
    }else{
        BOOL islogin = [[UserSessionManager getInstance] isLoggedIn];
        if (islogin) {
            self.navView.titleLabel.text = [NSString stringWithFormat:@"导入预点歌曲(共%d首)",[_preSongDataArray count]];
        }else{
            self.navView.titleLabel.text = [NSString stringWithFormat:@"预点歌列表(共%d首)",[_preSongDataArray count]];;
        }
        
        self.emptyTips.hidden = YES;
        [self.mainContentView sendSubviewToBack:self.emptyTips];
        [_preSongTable reloadData];
    }
    
    
}
-(void)getAlreadyDemandSongWithFaildResult:(id)result
{
    PLog(@"getAlreadyDemandSongWithFaildResult");
    
    [_indicatorView stopAnimating];
    NSString *msg = (NSString *)result;
    [CommenTools showAlertViewWithTitle:msg];
}

-(void)readLoacalPreVodSongData{
    PDatabaseManager *datamanager = [PDatabaseManager GetInstance];
    NSMutableArray *preSongDataArray = [datamanager  getSongInfoList:1000];
    PLog(@"resultArray count: %d",[preSongDataArray count]);
    if (_preSongDataArray) {
        [_preSongDataArray removeAllObjects];
    }
    [_preSongDataArray addObjectsFromArray:preSongDataArray];
    
    if ([_preSongDataArray count] == 0) {
        if(self.emptyTips==nil){
            self.emptyTips = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 14)];
            self.emptyTips.center = CGPointMake(160.0f, 208.0f);
            self.emptyTips.backgroundColor = [UIColor clearColor];
            self.emptyTips.text = @"您还没有预点歌曲";
            self.emptyTips.textAlignment = UITextAlignmentCenter;
            self.emptyTips.textColor = [UIColor grayColor];
            self.emptyTips.font = [UIFont systemFontOfSize:14];
            [self.mainContentView addSubview:self.emptyTips];
        }
        self.emptyTips.hidden =NO;
        [self.mainContentView bringSubviewToFront:self.emptyTips];
    }else{
        self.emptyTips.hidden = YES;
        [self.mainContentView sendSubviewToBack:self.emptyTips];
        
    }
    BOOL islogin = [[UserSessionManager getInstance] isLoggedIn];
    if (islogin) {
        self.navView.titleLabel.text = [NSString stringWithFormat:@"导入预点歌曲(共%d首)",[_preSongDataArray count]];
    }else{
        self.navView.titleLabel.text = [NSString stringWithFormat:@"预点歌列表(共%d首)",[_preSongDataArray count]];
    }
    
    [_preSongTable reloadData];
    
}

-(void)pquitBoxAction{
    UIAlertView *alert =[[UIAlertView alloc] initWithTitle:@""
                                                   message:@"房间已关闭,请退出房间"
                                                  delegate:self
                                         cancelButtonTitle:@"确定"
                                         otherButtonTitles:nil];
    [alert show];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    [UserSessionManager getInstance].isLoggedIn = NO;
    [self.navigationController popToRootViewControllerAnimated:YES];
    
}

#pragma UItableView delegate

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{
    
    
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    
    
    return [_preSongDataArray count];
}



- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    static NSString *dataCellIdentifier = @"SongListCellIdentifier";
    KTVSongPreBookCell *cell = (KTVSongPreBookCell *)[tableView  dequeueReusableCellWithIdentifier:dataCellIdentifier];
    if (cell ==nil) {
        NSArray *nib = [[NSBundle mainBundle] loadNibNamed:@"KTVSongPreBookCell" owner:self options:nil];
        BOOL islogin = [[UserSessionManager getInstance] isLoggedIn];
        if (islogin) {
            cell = [nib objectAtIndex:0];
        }else{
            cell = [nib objectAtIndex:1];
        }
        UIImageView *bottom_image = [[UIImageView alloc] initWithFrame:CGRectMake(0, 49, _MainScreen_Width, 1)];
        bottom_image.image = [UIImage imageNamed:@"list_line.png"];
        [cell.contentView addSubview:bottom_image];
    }
    SongInfo * info= [_preSongDataArray objectAtIndex:indexPath.row];
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    cell.song = info;
    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    
    return 50.0f;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



@end
