//
//  HongBRecordViewController.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/22.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "HBRecordViewController.h"
#import "PKtvAssistantAPI.h"
#import "KTVHongBaoSectionInfo.h"
#import "KTVAppDelegate.h"
#import "DDMenuController.h"

@interface HBRecordViewController ()

@end

@implementation HBRecordViewController

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:false];
}
-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:true];
}

#pragma mark - ViewDidLoad
#define ABSOLUTE_TABLEVIEW_LR 20
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    [self.mainContentView setBackgroundColor:UIColorFromRGB(0xf6f6f7)];
    
    //导航条
    [self.navView initOnlyBackButton:@"红包记录"];
    
    //表格
    CGFloat offset = ABSOLUTE2RELATIVE(ABSOLUTE_TABLEVIEW_LR);
    CGFloat width = _MainScreen_Width-offset*2;
    self.mUiTableView = [[UITableView alloc] initWithFrame:CGRectMake(offset, offset, width, self.mainContentView.frame.size.height) style:UITableViewStyleGrouped];
    [self.mUiTableView setBackgroundColor:[UIColor clearColor]];
    [self.mUiTableView setDataSource:self];
    [self.mUiTableView setDelegate:self];
    [self.mUiTableView setSeparatorStyle:UITableViewCellSeparatorStyleNone];
    [self.mainContentView addSubview:self.mUiTableView];
    
    self.mDataList = [[NSMutableArray alloc] init];
    [self loadData];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Load Net Data
-(void)loadData
{
    Reachability *reachability = [Reachability reachabilityForInternetConnection];
    if (![reachability isReachable])
    {
        [self showEmptyTip:YES msg:RES_NO_NETWORK];
        return;
    }
    
    [self.mDataList removeAllObjects];
    
    NSString * urlStr = [PKtvAssistantAPI getHongBaoListUrl:[UserSessionManager getInstance].currentUser.uid type:1];
    
    NSURL *url = [NSURL URLWithString:urlStr];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary * result = [results objectForKey:@"result"];
            NSArray * hongbaolist = [result objectForKey:@"RewardInfolist"];
            
            for (NSDictionary * dic in hongbaolist) {
                KTVHongBaoInfo * info = [KTVHongBaoInfo initWithDic:dic];
                
                BOOL isFind = false;
                for(KTVHongBaoSectionInfo* secinfo in self.mDataList)
                {
                    if(info.hbnclass == secinfo.nclass)
                    {
                        [secinfo.array addObject:info];
                        isFind = true;
                        break;
                    }
                }
                
                if(isFind)
                    continue;
                
                //默认都展开
                KTVHongBaoSectionInfo* secinfo = [[KTVHongBaoSectionInfo alloc] init];
                secinfo.nclass = info.hbnclass;
                secinfo.name = info.hbclass;
                [secinfo.array addObject:info];
                [self.mDataList addObject:secinfo];
            }
            
            if(self.mDataList.count == 0)
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

#pragma mark - TableViewDataSource
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return self.mDataList.count;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    KTVHongBaoSectionInfo* sectioninfo = [self.mDataList objectAtIndex:section];
    if(sectioninfo.expand)//如果展开
        return sectioninfo.array.count;
    else
        return 0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString* reuseId = @"GetHongBaoViewCell";
    HongBRecordViewCell* cell = [tableView dequeueReusableCellWithIdentifier:reuseId];
    if(cell == nil)
    {
        [[NSBundle mainBundle] loadNibNamed:@"HongBRecordViewController" owner:self options:nil];
        cell = self.mUiTableViewCell;
    }
    
    //根据指定的section
    KTVHongBaoSectionInfo* sectioninfo = [self.mDataList objectAtIndex:indexPath.section];//首先获取到对应的section
    KTVHongBaoInfo* info = [sectioninfo.array objectAtIndex:indexPath.row];//其次获取到对应的红包信息
    [cell initWithHBInfo:info];
    return cell;
}

#pragma mark - TableViewDelegate
#pragma mark - TableViewDelegate
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 80;
}
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 40;
}
- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    [[NSBundle mainBundle] loadNibNamed:@"HongBRecordViewController" owner:self options:nil];
    UIView* view = self.mUiSectionView;
    //设置边框等
    [view.layer setBorderWidth:1];
    [view.layer setBorderColor:UIColorFromRGB(0xebebeb).CGColor];
    [self.mUiSectionBtn setTag:section];
    
    KTVHongBaoSectionInfo* info = [self.mDataList objectAtIndex:section];
    //设置Tip的显示
    [self.mUiNumber setText:[NSString stringWithFormat:@"(%d个)",info.array.count]];
    //设置名称
    [self.mUiSectionName setText:info.name];
    //是否展开
    if(info.expand)
        [self.mUiSectionStatus setImage:[UIImage imageNamed:@"btn_list_top.png"]];
    else
        [self.mUiSectionStatus setImage:[UIImage imageNamed:@"btn_list_bottom.png"]];
    
    return view;
}

- (BOOL)tableView:(UITableView *)tableView shouldHighlightRowAtIndexPath:(NSIndexPath *)indexPath NS_AVAILABLE_IOS(6_0)
{
    return NO;
}

/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */

- (IBAction)onTouchSection:(id)sender {
    UIButton* btn = (UIButton*)sender;
    KTVHongBaoSectionInfo* sectioninfo = [self.mDataList objectAtIndex:btn.tag];
    sectioninfo.expand = !sectioninfo.expand;
    
    [self.mUiTableView reloadData];
}
@end
