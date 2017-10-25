//
//  ExchangeHistoryController.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "ExchangeHistoryController.h"
#import "KTVAppDelegate.h"
#import "CustomTabView.h"
#import "ExchangeTableView.h"
#import "DDMenuController.h"

@interface ExchangeHistoryController ()

@end

@implementation ExchangeHistoryController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.navView.titleLabel.text = @"兑换记录";
    // Do any additional setup after loading the view.
    _titles = [[NSMutableArray alloc] initWithObjects:@"未领取",@"已领取",@"已过期", nil];
    _tbView= [[NSMutableArray alloc]init];
    [self createTable:0];
    [self createTable:1];
    [self createTable:2];

    CustomTabView *view = [[CustomTabView alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height) titles:_titles views:_tbView];
    view.delegate = self;
    [self.mainContentView addSubview:view];
}


-(void)createTable:(NSInteger)index
{
    ExchangeTableView *table = [[ExchangeTableView alloc]initWithFrame:CGRectMake(0, 44, _MainScreen_Width, self.mainContentView.frame.size.height-44) type:index];
    table.hidden = YES;
    [_tbView addObject: table];
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

#pragma tab delegate
-(void)TabChange:(NSInteger)oldIndex newIndex:(NSInteger)newIndex
{
    if (newIndex > _tbView.count)
        return;
    ExchangeTableView *table = (ExchangeTableView *)_tbView[newIndex];
    [table reloadTable];

}

@end
