//
//  ArtistListViewController.m
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-12.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "ArtistListViewController.h"
#import "PKtvAssistantAPI.h"
#import "KTVBaseConfig.h"
#import "TopicInfo.h"
#import "SongsBookViewController.h"
#import "TopicInfoCell.h"
#import "DDMenuController.h"
#import "KTVAppDelegate.h"
#import "UserSessionManager.h"
@interface ArtistListViewController ()

@end

@implementation ArtistListViewController

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
    self.navView.titleLabel.text = self.artistListTitle;
//    self.navView.rightButton.hidden = NO;
//    self.navView.badgeButton.hidden = NO;
}


- (void)doBack:(id)sender{
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)creatArtistList{
    _artistList = [[UITableView alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height) style:UITableViewStylePlain];
    
    _artistList.delegate = self;
    _artistList.dataSource = self;
    _artistList.backgroundColor = UIColorFromRGB(0xf0f1f5);
    _artistList.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    [self.mainContentView addSubview:_artistList];
    
    
    //[self.mainContentView addSubview:_pullUpView];
    
    UIView * freeView = [[UIView alloc]init];
    _artistList.tableFooterView = freeView;
}

#pragma mark tableViewDelegate

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section{
    NSMutableArray * sectionArray = [_dataArray objectAtIndex:section];
    
    return sectionArray.count;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{
    return _dataArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    TopicInfoCell * cell = [_artistList dequeueReusableCellWithIdentifier:@"cell"];
    
    if (cell == nil) {
        cell = [[TopicInfoCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
    }
    NSMutableArray * section = [_dataArray objectAtIndex:indexPath.section];
    TopicInfo * info = [section objectAtIndex:indexPath.row];
    
    cell.topicInfo = info;
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    NSMutableArray * sectionArray = [_dataArray objectAtIndex:indexPath.section];
    
    TopicInfo * info = [sectionArray objectAtIndex:indexPath.row];
    SongsBookViewController * songsList = [[SongsBookViewController alloc] initInfo:self.modelID topicId:self.topicID title:info.topicTitle singerId:info.topicId];
//    songsList.listTitle = info.topicTitle;
//    songsList.modelID = self.modelID;
//    songsList.topicID = self.topicID;
//    songsList.artistID = info.topicId;
    
    [self.navigationController pushViewController:songsList animated:YES];
    [_artistList deselectRowAtIndexPath:indexPath animated:YES];
    
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section{
    if ([[_dataArray objectAtIndex:section] count] == 0) {
        return nil;
    }else{
        if (section == 0) {
            return @"#";
        }else{
            return [NSString stringWithFormat:@"%c",section+64];
        }
    }
}

- (NSArray *)sectionIndexTitlesForTableView:(UITableView *)tableView{
    return _indexArray;
}

#pragma mark loadData

- (void)loadData{
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];
        
        NSString * defaultKey = [NSString stringWithFormat:@"Artist_List_Type%ld",self.topicID];
        NSArray * array = [defaults objectForKey:defaultKey];
        
        KTVAppDelegate *appDelegate = (KTVAppDelegate *)[UIApplication sharedApplication].delegate;
        dispatch_async(dispatch_get_main_queue(), ^{
            if (array == nil)
            {
                [self loadDataFromService];
            }
            else if([appDelegate.singerList containsObject:[NSString stringWithFormat:@"Artist_List_Type%ld",self.topicID]])
            {
                [self loadLocalData];
                _localData = YES;
                //[self loadDataFromService];
            }
            else
            {
                [self loadLocalData];
                _localData = YES;
                [self loadDataFromService];
            }
        });

    });
    
}

- (void)loadLocalData{
    NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];
    
    NSString * defaultKey = [NSString stringWithFormat:@"Artist_List_Type%ld",self.topicID];
    NSArray * array = [defaults objectForKey:defaultKey];
    
    [self setDataArrayWithArray:array];
}

- (void)loadDataFromService{

    
    Reachability *reachability = [Reachability reachabilityForInternetConnection];
    if (![reachability isReachable])
    {
        
        if (_localData == NO) {
            [self showEmptyTip:YES msg:RES_NO_NETWORK];
            [self.indexArray removeAllObjects];
            
            [_artistList reloadData];
        }
        
        return;
    }
    //使用新的方式获取歌手列表 歌星数量一次性全部获取，目前歌库上有1w6千多个歌星
    NSString * urlStr = [PKtvAssistantAPI getGetSongListByTopics:self.modelID topicId:self.topicID begin:1 num:20000];
    
    NSURL *url = [NSURL URLWithString:urlStr];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData3:responseObject];
            
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                NSDictionary *result = [results objectForKey:@"result"];
                NSArray *dataList = [result objectForKey:@"songtopics"];
                
                dispatch_async(dispatch_get_main_queue(), ^{
                    if (_localData == NO) {
                        [self showEmptyTip:NO msg:nil];
                        [self setDataArrayWithArray:dataList];
                    }
                });
                
                KTVAppDelegate *appDelegate = (KTVAppDelegate *)[UIApplication sharedApplication].delegate;
                [appDelegate.singerList addObject:[NSString stringWithFormat:@"Artist_List_Type%ld",self.topicID]];
                
                NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];
                NSString * defaultKey = [NSString stringWithFormat:@"Artist_List_Type%ld",self.topicID];
                
                [defaults setObject:dataList forKey:defaultKey];
                [defaults synchronize];
                
            } else {
                NSString *msg = [results objectForKey:@"msg"];
                PLog(@"doGetSongTopics 3 failed...%@", msg);
                
                    if (_localData == NO) {
                        dispatch_async(dispatch_get_main_queue(), ^{
                            [self showEmptyTip:YES msg:msg];
                            [self.indexArray removeAllObjects];
                        });
                    }
            }
        });

        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        PLog(@"doGetSongTopics 3 failed...");
        
        if (_localData == NO) {
            [self showEmptyTip:YES msg:RES_NO_NETWORK];
            [self.indexArray removeAllObjects];
        }
        
    }];
    
    if (_localData == NO)
        [self startWait];
    [operation start];
}

- (void)setDataArrayWithArray:(NSArray *) dataList{
    for (NSDictionary * dic in dataList) {
        TopicInfo * info = [TopicInfo initWithDictionary:dic];
        unichar first = [info.pinyin characterAtIndex:0];
        
        //NSLog(@"%d",first);
        if (first >= 97 && first <= 122) {
            NSMutableArray * sectionArray = [_dataArray objectAtIndex:first-96];
            
            [sectionArray addObject:info];
        }else if (first >= 65 && first <= 90){
            NSMutableArray * sectionArray = [_dataArray objectAtIndex:first-64];
            
            [sectionArray addObject:info];
        }else{
            NSMutableArray * sectionArray = [_dataArray objectAtIndex:0];
            
            [sectionArray addObject:info];
        }
    }
    [_artistList reloadData];

}

- (void)showArray{
    NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];
    
    NSString * defaultKey = [NSString stringWithFormat:@"Artist_List_Type%ld",self.topicID];
    NSArray * array = [defaults objectForKey:defaultKey];
    NSLog(@"——————————————————————>%d",array.count);
}

#pragma mark viewDidLoad

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    
    [self setNavigationView];
    
    _dataArray = [[NSMutableArray alloc]init];
    
    _indexArray = [[NSMutableArray alloc]init];
    
    for (int i = 0; i < 27; i++) {

        
        if (i == 0) {
            [_indexArray addObject:@"#"];
        }else{
            [_indexArray addObject:[NSString stringWithFormat:@"%c",i+64]];
        }
    }
    
    for (int i = 0; i < 27; i++) {
        NSMutableArray * sectionArray = [[NSMutableArray alloc]init];
        
        [_dataArray addObject:sectionArray];
    }
    
    
    
    [self creatArtistList];
    [self loadData];
    
    //[self loadDataFromService];
    
    //[self showArray];
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
