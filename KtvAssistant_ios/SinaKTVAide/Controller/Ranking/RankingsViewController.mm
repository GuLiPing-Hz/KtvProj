//
//  Rankings.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/20.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "RankingsViewController.h"
#import "TopSongsListViewController.h"
#import "SongsBookViewController.h"
#import "KTVAppDelegate.h"
#import "PKtvAssistantAPI.h"
#import "DDMenuController.h"

@interface RankingsViewController ()

@end

@implementation RankingsViewController

-(instancetype) initWithModelId:(int)modelId title:(NSString*) title
{
    self = [super init];
    if(self)
    {
        self.mModelId = modelId;
        self.mTitle = title;
    }
    
    return self;
}

- (void)createTableView{
    self.mUiTableView = [[UITableView alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height) style:UITableViewStylePlain];
    
    self.mUiTableView.delegate = self;
    self.mUiTableView.dataSource = self;
    self.mUiTableView.backgroundColor = UIColorFromRGB(0xf6f6f7);
    self.mUiTableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    UIView *spaceView = [[UIView alloc] init];
    self.mUiTableView.tableFooterView = spaceView;
    
    [self.mainContentView addSubview:self.mUiTableView];
    
    self.mDataLists = [[NSMutableArray alloc]init];
}

#pragma mark - MONActivityIndicatorViewDelegate Methods

- (UIColor *)activityIndicatorView:(MONActivityIndicatorView *)activityIndicatorView
      circleBackgroundColorAtIndex:(NSUInteger)index {
    return UIColorFromRGB(0xc0c0c2);
}

#pragma mark - ViewDidLoad
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    //UI
    //[[NSBundle mainBundle] loadNibNamed:@"Rankings" owner:self options:nil];
    [self.mainContentView setBackgroundColor:UIColorFromRGB(0xf6f6f7)];
    [self.navView initCommonBackButton:self.mTitle isInRoom:NO];
    
    [self createTableView];
    [self loadDataFromService];
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self.appDelegate.menuController setEnableGesture:NO];
    [self getAlreadyOrderedSongsNumber];
    [self setRightAndBadgeButton];
}

-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    [self.appDelegate.menuController setEnableGesture:YES];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark loadData
- (void)loadDataFromService{
    Reachability *reachability = [Reachability reachabilityForInternetConnection];
    if (![reachability isReachable])
    {
        [self showEmptyTip:YES msg:RES_NO_NETWORK];
        return;
    }
    
    [self.mDataLists removeAllObjects];
    
    NSString * urlStr = [PKtvAssistantAPI getGetSongTopics:[UserSessionManager getInstance].currentRoomInfo.roomId modelId:self.mModelId begin:1 num:20];
    
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
                [self.mDataLists addObject:info];
            }
            
            if(self.mDataLists.count == 0)
                [self showEmptyTip:YES msg:RES_NO_DATA];
            else
            {
                [self showEmptyTip:NO msg:nil];
                [self.mUiTableView reloadData];
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

#pragma mark - TableView DataSource
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return self.mDataLists.count/2 +self.mDataLists.count%2;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString* reuseId = @"RankingsCell";
    RankingsViewCell* cell = [tableView dequeueReusableCellWithIdentifier:reuseId];
    if (cell == nil) {
        [[NSBundle mainBundle] loadNibNamed:@"Rankings" owner:self options:nil];
        cell = self.mUiRankingsCell;
        [cell initWithDelegate:self];
    }
    TopicInfo* info1 = [self.mDataLists objectAtIndex:indexPath.row*2];
    TopicInfo* info2 = nil;
    if(self.mDataLists.count > indexPath.row*2+1)
        info2 = [self.mDataLists objectAtIndex:indexPath.row*2+1];
    [cell initWithTopicInfo:info1 info2:info2];
    
    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 121;
}

- (NSIndexPath *)tableView:(UITableView *)tableView willSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    return NULL;
}
#pragma mark RankingTouchProtocol

-(void)onTouchIn:(long) topicId title:(NSString*)title
{
    if(topicId == 0 || title == nil)
        return ;
    
    //情歌对唱榜，新歌排行榜，得分榜需要单独显示
    if(topicId == 1 || topicId == 2 || topicId == 8)
    {
        TopSongsListViewController* controller = [[TopSongsListViewController alloc] initInfo:self.mModelId topic:topicId title:title];
        [self.navigationController pushViewController:controller animated:YES];
    }
    else
    {
        SongsBookViewController* controller = [[SongsBookViewController alloc] initInfoNoSinger:self.mModelId topicId:topicId title:title];
        [self.navigationController pushViewController:controller animated:YES];
    }
    
}
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
