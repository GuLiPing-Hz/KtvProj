//
//  HongBaoGetViewController.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/21.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "HongBGetViewController.h"
#import "KTVHongBaoSectionInfo.h"
#import "PKtvAssistantAPI.h"
#import "HBRecordViewController.h"
#import "KTVAppDelegate.h"
#import "DDMenuController.h"

@interface HongBGetViewController ()

@property (atomic,strong) KTVHongBaoInfo* currentHB;

@end

@implementation HongBGetViewController

#pragma mark - onHongBaoHistory
-(void)onHongBaoHistory
{
    PLog(@"go to 红包记录");
    HBRecordViewController* controller = [[HBRecordViewController alloc] init];
    [self.navigationController pushViewController:controller animated:YES];
}

#define ABSOLUTE_TABLEVIEW_LR 20
#pragma mark - ViewDidLoad
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    //这里加载有问题
    //[[NSBundle mainBundle] loadNibNamed:@"HongBaoGetViewController" owner:self options:nil];
    [self.mainContentView setBackgroundColor:UIColorFromRGB(0xf6f6f7)];
    
    //导航条
    [self.navView initOnlyBackButton:@"领取红包"];
    [self.navView.btnHistory setTitle:@"红包记录" forState:UIControlStateNormal];
    [self.navView.btnHistory addTarget:self action:@selector(onHongBaoHistory) forControlEvents:UIControlEventTouchUpInside];
    [self.navView.btnHistory setHidden:NO];
    
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
    
    NSString * urlStr = [PKtvAssistantAPI getHongBaoListUrl:[UserSessionManager getInstance].currentUser.uid type:0];
    
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
    GetHongBaoViewCell* cell = [tableView dequeueReusableCellWithIdentifier:reuseId];
    if(cell == nil)
    {
        [[NSBundle mainBundle] loadNibNamed:@"HongBaoGetViewController" owner:self options:nil];
        cell = self.mUiTableCell;
        cell.mCallBack = self;
    }
    
    //根据指定的section
    KTVHongBaoSectionInfo* sectioninfo = [self.mDataList objectAtIndex:indexPath.section];//首先获取到对应的section
    KTVHongBaoInfo* info = [sectioninfo.array objectAtIndex:indexPath.row];//其次获取到对应的红包信息
    [cell initWithHBInfo:info];
    return cell;
}

#pragma mark - TableViewDelegate
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 90;
}
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 40;
}
- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    [[NSBundle mainBundle] loadNibNamed:@"HongBaoGetViewController" owner:self options:nil];
    UIView* view = self.mUiSectionView;
    //设置边框等
    [view.layer setBorderWidth:1];
    [view.layer setBorderColor:UIColorFromRGB(0xebebeb).CGColor];
    [self.mUiSectionBtn setTag:section];
    
    KTVHongBaoSectionInfo* info = [self.mDataList objectAtIndex:section];
    //设置Tip的显示
    if(info.nclass == 1)
        [self.mUiSectionTip setHidden:NO];
    else
        [self.mUiSectionTip setHidden:YES];
    //设置名称
    [self.mUIClassName setText:info.name];
    
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

#pragma mark - CustomDialogDelegate
-(void)onCustomLayout:(CommonDialogView*)dialog view:(UIView*)contentView;
{
    if(self.mUiHongBao == nil)
    {
        self.mUiHongBao = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 290, 442)];
        [self.mUiHongBao setBackgroundColor:[UIColor clearColor]];
        
        UIImageView* ui_img = [[UIImageView alloc] initWithFrame:CGRectMake(0, 58, 290, 384)];
        [ui_img setImage:[UIImage imageNamed:@"red_shell_1"]];
        [self.mUiHongBao addSubview:ui_img];
        
        UIButton* ui_btn_clost = [[UIButton alloc] initWithFrame:CGRectMake(250, 8, 40, 40)];
        [ui_btn_clost setImage:[UIImage imageNamed:@"red_off_btn_1"] forState:UIControlStateNormal];
        [ui_btn_clost addTarget:self action:@selector(onCloseHb) forControlEvents:UIControlEventTouchUpInside];
        [self.mUiHongBao addSubview:ui_btn_clost];
        
        self.mUiHbName = [[UILabel alloc] initWithFrame:CGRectMake(26, 69, 238, 32)];
        [self.mUiHbName setFont:[UIFont systemFontOfSize:26]];
        [self.mUiHbName setTextColor:UIColorFromRGB(0xe64a45)];
        [self.mUiHbName setTextAlignment:NSTextAlignmentCenter];
        [self.mUiHongBao addSubview:self.mUiHbName];
        
        self.mUiHbMission= [[UILabel alloc] initWithFrame:CGRectMake(26, 104, 238, 32)];
        [self.mUiHbMission setFont:[UIFont systemFontOfSize:20]];
        [self.mUiHbMission setTextColor:UIColorFromRGB(0x666666)];
        [self.mUiHbMission setTextAlignment:NSTextAlignmentCenter];
        [self.mUiHongBao addSubview:self.mUiHbMission];
        
        self.mUiHbGold= [[UILabel alloc] initWithFrame:CGRectMake(26, 167, 238, 42)];
        [self.mUiHbGold setFont:[UIFont systemFontOfSize:35]];
        [self.mUiHbGold setTextColor:UIColorFromRGB(0xFFA200)];
        [self.mUiHbGold setTextAlignment:NSTextAlignmentCenter];
        [self.mUiHongBao addSubview:self.mUiHbGold];
        
        self.mUiGetHBBtn = [[DeformationButton alloc] initWithFrame:CGRectMake(98, 233, 95, 33)];
        self.mUiGetHBBtn.contentColor = UIColorFromRGB(0xff3d3d);//52c332
        self.mUiGetHBBtn.progressColor = [UIColor whiteColor];
        
        [self.mUiGetHBBtn setImage:[UIImage imageNamed:@"income_btn_0"] Success:[UIImage imageNamed:@"income_btn_success"] Fail:[UIImage imageNamed:@"income_btn_fail"]];
        [self.mUiGetHBBtn addTarget:self action:@selector(onTouchGetHb) forControlEvents:UIControlEventTouchUpInside];
        [self.mUiHongBao addSubview:self.mUiGetHBBtn];
    }
    
    [self.mUiHbName setText:self.currentHB.hbclass];
    [self.mUiHbMission setText:self.currentHB.hbmission];
    [self.mUiHbGold setText:[NSString stringWithFormat:@"%d金币",self.currentHB.gold]];
    //[self.mUiGetHBBtn resetButton:YES];
    
    [contentView setFrame:CGRectMake(0, 0, self.mUiHongBao.frame.size.width, self.mUiHongBao.frame.size.height)];
    [contentView addSubview:self.mUiHongBao];
    
    [self.mUiGetHBBtn sendActionsForControlEvents:UIControlEventTouchUpInside];
}

#pragma mark HongBaoCellProtocol
-(void)onTouchGetHB:(KTVHongBaoInfo*)info
{
    //弹出对话框
    PLog(@"onTouchGetHB");
    self.currentHB = info;
    
    if(self.currentHB != nil)
    {
        if(self.mDialogView == nil)
        {
            self.mDialogView = [[CommonDialogView alloc] initWithDelegate:self];
            self.mDialogView.transitionStyle = SIAlertViewTransitionStyleBounce;
        }
        [self.mDialogView show];
    }
}

#pragma mark - other
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */
#pragma mark Section Delegate
- (IBAction)onTouchSection:(id)sender {
    UIButton* btn = (UIButton*)sender;
    KTVHongBaoSectionInfo* sectioninfo = [self.mDataList objectAtIndex:btn.tag];
    sectioninfo.expand = !sectioninfo.expand;
    
    [self.mUiTableView reloadData];
}

- (void)onTouchGetHb{
    PLog(@"onTouchGetHb");
    
    if(self.currentHB == nil)
        return ;
    NSString* str_url = [PKtvAssistantAPI getHongBaoGetUrl:[UserSessionManager getInstance].currentUser.uid hbid:self.currentHB.hbid];
    NSURLRequest* request = [[NSURLRequest alloc] initWithURL:[[NSURL alloc] initWithString:str_url]];
    AFHTTPRequestOperation* operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary* html = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        NSString* status = [html objectForKey:@"status"];
        
        if([status compare:@"1"] == NSOrderedSame)
        {
            [self onFinishHongBBtn:YES];
        }
        else
        {
            [self onFinishHongBBtn:NO];
        }
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        //[self.mUiGetHBBtn setStopSuccess:NO];
        [self onFinishHongBBtn:NO];
    }];
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [NSThread sleepForTimeInterval:2];
        [operation start];
    });
   
}

-(void)onFinishHongBBtn:(BOOL)success
{
    PLog(success?@"success":@"fail");
    [self.mUiGetHBBtn setStopSuccess:success];
    [self loadData];
}

- (void)onCloseHb {
    PLog(@"onCloseHb");
    if(self.mDialogView)
    {
        self.currentHB = nil;
        [self.mUiGetHBBtn resetButton:YES onEnd:^{
            //[self.mDialogView dismissAnimated:YES];
        }];
        [self.mDialogView dismissAnimated:YES];
        self.mDialogView = nil;
    }
}
@end
