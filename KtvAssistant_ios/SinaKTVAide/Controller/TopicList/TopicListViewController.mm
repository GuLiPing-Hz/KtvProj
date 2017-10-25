//
//  TopicListViewController.m
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-12.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "TopicListViewController.h"
#import "PKtvAssistantAPI.h"
#import "TopicInfo.h"
#import "KTVBaseConfig.h"
#import "ArtistListViewController.h"
#import "SongsBookViewController.h"
#import "TopSongsListViewController.h"
#import "TopicInfoCell.h"
#import "DDMenuController.h"
#import "KTVAppDelegate.h"
@interface TopicListViewController ()

@end

@implementation TopicListViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

-(instancetype) initModelId:(int)modelId title:(NSString*)title
{
    self = [super init];
    if(self)
    {
        self.modelID = modelId;
        self.topicTitle = title;
    }
    
    return self;
}
#pragma mark addView

- (void)setNavigationView{
    [self.navView initCommonBackButton:self.topicTitle isInRoom:NO];
}

- (void)creatTopicList{
    _topicList = [[UITableView alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height) style:UITableViewStylePlain];
    
    _topicList.delegate = self;
    _topicList.dataSource = self;
    _topicList.backgroundColor = UIColorFromRGB(0xf0f1f5);
    _topicList.separatorStyle = UITableViewCellSeparatorStyleNone;
    UIView *spaceView = [[UIView alloc] init];
    _topicList.tableFooterView = spaceView;
    
    [self.mainContentView addSubview:_topicList];
    
    _dataArray = [[NSMutableArray alloc]init];
}

#pragma mark tableViewDelegate

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    return _dataArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    static NSString* reuseId = @"TopicCell";
    TopicInfoCell * cell = [_topicList dequeueReusableCellWithIdentifier:reuseId];
    if (cell == nil) {
        cell = [[TopicInfoCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:reuseId];
    }
    TopicInfo * info = [_dataArray objectAtIndex:indexPath.row];
    
    cell.topicInfo = info;
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    TopicInfo * info = [_dataArray objectAtIndex:indexPath.row];
    if (self.modelID == kPKtvAssistantAPIModelTypeArtist) {
        ArtistListViewController * artistList = [[ArtistListViewController alloc]init];
        artistList.artistListTitle = info.topicTitle;
        artistList.topicID = info.topicId;
        artistList.modelID = self.modelID;
        [self.navigationController pushViewController:artistList animated:YES];
    }else if (self.modelID == kPKtvAssistantAPIModelTypeRank){
        TopSongsListViewController * topSongsList = [[TopSongsListViewController alloc]init];
        topSongsList.listTitle = info.topicTitle;
        topSongsList.modelID = self.modelID;
        topSongsList.topicID = info.topicId;
        [self.navigationController pushViewController:topSongsList animated:YES];
    }else {
        SongsBookViewController * songsList = [[SongsBookViewController alloc] initInfoNoSinger:self.modelID topicId:info.topicId title:info.topicTitle];
//        songsList.listTitle = info.topicTitle;
//        songsList.modelID = self.modelID;
//        songsList.topicID = info.topicId;
        [self.navigationController pushViewController:songsList animated:YES];
    }
    [_topicList deselectRowAtIndexPath:indexPath animated:YES];
}

#pragma mark loadData
- (void)loadDataFromService{
    Reachability *reachability = [Reachability reachabilityForInternetConnection];
    if (![reachability isReachable])
    {
        [self showEmptyTip:YES msg:RES_NO_NETWORK];
        return;
    }
    
    NSString * urlStr = [PKtvAssistantAPI getGetSongTopics:[UserSessionManager getInstance].currentRoomInfo.roomId modelId:self.modelID begin:1 num:20];
    NSURL *url = [NSURL URLWithString:urlStr];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary * result = [results objectForKey:@"result"];
            NSArray * songList = [result objectForKey:@"songtopics"];
            for (NSDictionary * dic in songList) {
                TopicInfo * info = [TopicInfo initWithDictionary:dic];
                [_dataArray addObject:info];
            }
            
            if(_dataArray.count == 0)
                [self showEmptyTip:YES msg:RES_NO_DATA];
            else
            {
                [self showEmptyTip:NO msg:nil];
                [_topicList reloadData];
            }
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doGetSongTopics failed...%@", msg);
            
            [self showEmptyTip:YES msg:msg];
        }
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        
        PLog(@"doGetSongTopics failed...");
        [self showEmptyTip:YES msg:RES_NO_NETWORK];
    }];
    
    [self startWait];
    [operation start];
}

#pragma mark viewDidLoad

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    [self setNavigationView];
    [self creatTopicList];
    [self loadDataFromService];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:NO];
    
    [self getAlreadyOrderedSongsNumber];
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
