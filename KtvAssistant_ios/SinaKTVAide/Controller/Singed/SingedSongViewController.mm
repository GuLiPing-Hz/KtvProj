//
//  SingedSongViewController.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/12.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "SingedSongViewController.h"
#import "KTVBaseConfig.h"
#import "PKtvAssistantAPI.h"
#import "UserSessionManager.h"
#import "AFHTTPRequestOperation.h"
#import "User.h"
#import "SongRecord.h"
#import "CommenTools.h"
#import "KTVAppDelegate.h"
#import "DDMenuController.h"

@interface SingedSongViewController ()

@end

@implementation SingedSongViewController

#define AB_USERINFO_MARGINTOP 20
#define AB_USERINFO_MARGINLR 20
#define AB_TABLE_MARGINLEFT 50
#define AB_TABLE_MARGINTOP 26
#define AB_LINE_X 60
#define AB_SPACING_TABELVIEW 20

-(void)createUserInfoView
{
    
    //self.mUiUserInfo = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 320, 66)];
    
    //self.mUiImgViewHead = [[UIImageView alloc] initWithFrame:CGRectMake(0, 1, 64, 64)];
    //[self.mUiUserInfo addSubview:self.mUiImgViewHead];
    
    [self.mUiImgViewHead.layer setCornerRadius:32];
    [self.mUiImgViewHead.layer setBorderWidth:1];
    [self.mUiImgViewHead.layer setBorderColor:UIColorFromRGB(0xc6c6c7).CGColor];
    [self.mUiImgViewHead.layer setMasksToBounds:YES];
    
    User* user = [UserSessionManager getInstance].currentUser;
    
    if(user)
    {
        [self.mUiLabelName setText:user.name];
        [self.mUiLabelIdx setText:[NSString stringWithFormat:@"ID: %ld",user.uid]];
        
        NSString* str_photo = [CommenTools GetURLWithResolutionScaleTransfered:user.headphoto scale:0];
        [self.mUiImgViewHead setImageWithURL:[NSURL URLWithString:str_photo] placeholderImage:[UIImage imageNamed:@"userFace_default"]];
    }
    
    CGFloat height = self.mUiUserInfo.frame.size.height;
    [self.mUiUserInfo setFrame:CGRectMake(ABSOLUTE2RELATIVE(AB_USERINFO_MARGINLR), ABSOLUTE2RELATIVE(AB_USERINFO_MARGINTOP), _MainScreen_Width-ABSOLUTE2RELATIVE(AB_USERINFO_MARGINLR*2), height)];
    
    //添加到Main Content View中
    [self.mainContentView addSubview:self.mUiUserInfo];
}

#pragma mark viewDidLoad
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    //data
    self.mSongRecord = [[NSMutableArray alloc] init];
    
    //UI
    [[NSBundle mainBundle] loadNibNamed:@"HeadInfo" owner:self options:nil];
    [self.mainContentView setBackgroundColor:[UIColor whiteColor]];
    [self.navView initOnlyBackButton:@"唱过的歌"];
    
    //用户信息
    [self createUserInfoView];
    
    int y_line = self.mUiUserInfo.frame.origin.y+self.mUiUserInfo.frame.size.height;
    int y_tableview =  y_line + ABSOLUTE2RELATIVE(AB_TABLE_MARGINTOP);
    
    self.mUiLine = [[UIView alloc] initWithFrame:CGRectMake(ABSOLUTE2RELATIVE(AB_LINE_X)+13.5f, y_line, 1, _MainScreen_Height-44-y_tableview+ABSOLUTE2RELATIVE(AB_TABLE_MARGINTOP))];
    [self.mUiLine setBackgroundColor:UIColorFromRGB(0xebebeb)];
    [self.mUiLine setHidden:YES];
    [self.mainContentView addSubview:self.mUiLine];
    
    self.mUiTableView = [[UITableView alloc] initWithFrame:CGRectMake(ABSOLUTE2RELATIVE(AB_TABLE_MARGINLEFT), y_tableview, _MainScreen_Width-ABSOLUTE2RELATIVE(AB_TABLE_MARGINLEFT)-ABSOLUTE2RELATIVE(AB_USERINFO_MARGINLR), _MainScreen_Height-44-y_tableview) style:UITableViewStylePlain];
    
    [self.mUiTableView setSeparatorColor:[UIColor clearColor]];
    [self.mUiTableView setDataSource:self];
    [self.mUiTableView setDelegate:self];
    [self.mUiTableView setBackgroundColor:[UIColor clearColor]];
//    UIView *spaceView = [[UIView alloc] init];
//    self.mUiTableView.tableFooterView = spaceView;
    [self.mainContentView addSubview:self.mUiTableView];
    
    [self loadUserInfo];
}

#pragma mark - Net Load Data
-(void)loadUserInfo
{
    NSString* str_request = [PKtvAssistantAPI getUserGetInfoUrl:[UserSessionManager getInstance].currentUser.uid];
    NSURL* url_request = [[NSURL alloc] initWithString:str_request];
    NSURLRequest* request = [[NSURLRequest alloc] initWithURL:url_request];
    AFHTTPRequestOperation* operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary* html = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        NSString* status = [html objectForKey:@"status"];
        if([status compare:@"1"] == NSOrderedSame)
        {
            NSDictionary* result = [html objectForKey:@"result"];
            NSDictionary* dic_user = [result objectForKey:@"User"];
            User* user = [User initWithUserDictionary:dic_user];
            [self.mUiLabelTotalScore setText:[NSString stringWithFormat:@"%0.1f",user.songtotalscore/10.0f]];
        }
        else
            PLog(@"request user info result failed");
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        PLog(@"request user info failed");
    }];
    
    //启动运行
    [operation start];
}
-(void)loadSongList
{
    Reachability *reachability = [Reachability reachabilityForInternetConnection];
    if (![reachability isReachable])
    {
        [self showEmptyTip:YES msg:RES_NO_NETWORK];
        return;
    }
    
    [self.mSongRecord removeAllObjects];
    
    NSString* str_request = [PKtvAssistantAPI getUserSongRecord:[UserSessionManager getInstance].currentUser.uid];
    NSURL* url_request = [[NSURL alloc] initWithString:str_request];
    NSURLRequest* request = [[NSURLRequest alloc] initWithURL:url_request];
    AFHTTPRequestOperation* operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary* html = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        NSString* status = [html objectForKey:@"status"];
        if([status compare:@"1"] == NSOrderedSame)
        {
            NSDictionary* result = [html objectForKey:@"result"];
            NSArray* dataList = [result objectForKey:@"UserSongLogList"];
            
            for(NSDictionary *dict in dataList)
            {
                SongRecord* sr = [SongRecord initWithDic:dict];
                [self.mSongRecord addObject:sr];
            }
            
            if(self.mSongRecord.count > 0)
            {
                [self.mUiLine setHidden:NO];
                [self showEmptyTip:NO msg:nil];
                [self.mUiTableView reloadData];//重新加载数据
            }
            else
                [self showEmptyTip:YES msg:RES_NO_DATA];
        }
        else
        {
            NSString* msg = [NSString stringWithFormat:@"%@",[html objectForKey:@"msg"]];
            
            [self showEmptyTip:YES msg:msg];
        }
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        [self showEmptyTip:YES msg:RES_NO_NETWORK];
    }];
    
    [self startWait];
    [operation start];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    [self.appDelegate.menuController setEnableGesture:true];
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:false];
    
    [self loadSongList];
}

-(void)onUserSongTotalScoreChange:(int) score
{
    char buf[50] = {0};
    sprintf(buf, "%0.1f",score/10.0f);
    [self.mUiLabelTotalScore setText:[NSString stringWithUTF8String:buf]];
}

#pragma mark - MONActivityIndicatorViewDelegate Methods

- (UIColor *)activityIndicatorView:(MONActivityIndicatorView *)activityIndicatorView
      circleBackgroundColorAtIndex:(NSUInteger)index {
    return UIColorFromRGB(0xc0c0c2);
}

#pragma mark UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return self.mSongRecord.count;
}

// Row display. Implementers should *always* try to reuse cells by setting each cell's reuseIdentifier and querying for available reusable cells with dequeueReusableCellWithIdentifier:
// Cell gets various attributes set automatically based on table (separators) and data source (accessory views, editing controls)

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString* CellTableIdentifier = @"SingedSongTableViewCell";
    UITableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:CellTableIdentifier];
    
    if(cell == nil)
    {
        [[NSBundle mainBundle] loadNibNamed:@"SingedSong" owner:self options:nil];
        cell = self.mUiTableCell;
    }
    
    NSUInteger row = [indexPath row];
    
    SongRecord* sr = [self.mSongRecord objectAtIndex:row];
    NSString* song_info = [NSString stringWithFormat:@"%@  %@  %@",sr.songname,sr.singername,[NSString stringWithFormat:@"%0.1f",sr.score/10.0f]];
    
    UILabel* ui_songInfo = (UILabel*)[cell.contentView viewWithTag:1];
    [ui_songInfo setText:song_info];
    UILabel* ui_room = (UILabel*)[cell.contentView viewWithTag:2];
    if(sr.room.length == 0)
        [ui_room setText:RES_NO_INROOM];
    else
        [ui_room setText:sr.room];
    UILabel* ui_time = (UILabel*)[cell.contentView viewWithTag:3];
    [ui_time setText:sr.time];
    
    return cell;
}
#pragma mark UITableViewDelegate

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 55;
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
