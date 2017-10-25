//
//  GIftInfoController.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "GIftInfoController.h"
#import "KTVAppDelegate.h"
#import "PKtvAssistantAPI.h"
#import "KTVGiftInfo.h"
#import "GiftInfoCell.h"
#import "GiftExchangeController.h"
#import "ExchangeHistoryController.h"
#import "DDMenuController.h"

@interface GIftInfoController ()

@end

@implementation GIftInfoController

-(void)create
{
    _arrGiftInfo = [[NSMutableArray alloc] init];
    
    _lblTip = [[UILabel alloc] initWithFrame:CGRectMake(20, 20, 100, 30)];
    _lblTip.text = ktvname;
    _lblTip.font = [UIFont systemFontOfSize:14];
    _lblTip.textColor = UIColorFromRGB(0x666666);
    _lblTip.backgroundColor = [UIColor clearColor];
    [_lblTip sizeToFit];
    [self.mainContentView addSubview:_lblTip];
    
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 44, self.mainContentView.frame.size.width, self.mainContentView.frame.size.height - 44) style:UITableViewStylePlain];
    _tableView.dataSource = self;
    _tableView.delegate = self;
    _tableView.backgroundColor = [UIColor whiteColor];
    _tableView.tableFooterView = [[UIView alloc] init];
    _tableView.separatorStyle= UITableViewCellSeparatorStyleSingleLine;
    [self.mainContentView addSubview:_tableView];
    
    UIImageView *line=[[UIImageView alloc]initWithFrame:CGRectMake(0, 43, self.mainContentView.frame.size.width, 1)];
    line.backgroundColor=UIColorFromRGB(0xc6c6c7);
    [self.mainContentView addSubview:line];
}


-(id)init:(NSString *)_ktvid ktvname:(NSString *)_ktvname
{
    self = [super init];
    if (self) {
        ktvid = _ktvid;
        ktvname = _ktvname;
    }
    return  self;
}

-(void)loadData
{
    NSString *strUrl= [PKtvAssistantAPI getGetGiftInfoUrl:ktvid page:1 size:20];
    NSURL *url =[[NSURL alloc] initWithString:strUrl];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url];
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation * operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary *html = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        NSString *status = [html objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [html objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"ktvgiftlist"];
            for (NSDictionary *dic in dataList) {
                KTVGiftInfo *info = [KTVGiftInfo initWithDictionary:dic];
                [_arrGiftInfo addObject:info];
            }
            [_tableView reloadData];
        }
        else
        {
            
        }
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
    }];
    [operation start];
}

#pragma mark table delegate
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _arrGiftInfo.count;
}

-(UITableViewCell *) tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    GiftInfoCell *cell = [tableView dequeueReusableCellWithIdentifier:@"cell"];
    if (cell == nil) {
        cell = [[GiftInfoCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
        cell.backgroundColor = [UIColor clearColor];
        cell.delegate = self;
    }
    cell.model = [_arrGiftInfo objectAtIndex:indexPath.row];
    return  cell;
}

-(CGFloat) tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 60;
}

-(void)ShowExchange:(KTVGiftInfo *)info
{
    GiftExchangeController *giftExchange= [[GiftExchangeController alloc] init:info ktvid:ktvid];
    [self.navigationController pushViewController:giftExchange animated:true];
}


-(void)ShowHistory
{
    ExchangeHistoryController *controller = [[ExchangeHistoryController alloc] init];
    [self.navigationController pushViewController:controller animated:true];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.navView.titleLabel.text = @"选择礼品";
    self.navView.btnHistory.hidden = NO;
    [self.navView.btnHistory addTarget:self action:@selector(ShowHistory) forControlEvents:UIControlEventTouchUpInside];
    self.view.backgroundColor = UIColorFromRGB(0xf8f8f9);
    [self create];//创建布局
    [self loadData];

    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:false];
}

-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:true];
}


@end
