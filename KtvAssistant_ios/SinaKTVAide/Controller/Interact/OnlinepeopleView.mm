//
//  OnlinepeopleView.m
//  SinaKTVAide
//
//  Created by cxp on 14-7-28.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "OnlinepeopleView.h"
#import "KTVAppDelegate.h"
#import "KTVBaseConfig.h"
#import "NetUtilCallBack.h"
#import "OnlineCell.h"

@implementation OnlinepeopleView

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        _tableView.delegate = self;
        currentIndex = -1;
        frontIndex = -1;
    }
    return self;
}
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:NO];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:YES];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navView.titleLabel.text = @"在线人数";
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.mainContentView.backgroundColor = UIColorFromRGB(0xf8f8f9);
    
    [self createTable];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(delegateSendGift:) name:@"ktvassistantsendgift" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(delegateSendChat:) name:@"ktvassistantsendChat" object:nil];
}

- (void)delegateSendGift:(NSNotification *)notification
{
    [self.navigationController popViewControllerAnimated:true];
    NSDictionary * userInfo = [notification userInfo];
    NSString * username = [userInfo objectForKey:@"username"];
    NSString * userid = [userInfo objectForKey:@"userid"];
    
    NSInteger idx = [userid intValue];
    [_delegate sendgift:username :idx];
}
- (void)delegateSendChat:(NSNotification *)notification
{
    [self.navigationController popViewControllerAnimated:true];
    NSDictionary * userInfo = [notification userInfo];
    NSString * username = [userInfo objectForKey:@"username"];
    NSString * userid = [userInfo objectForKey:@"userid"];
    
    NSInteger idx = [userid intValue];
    [_delegate sendchat:username :idx];
}

-(void)createTable
{
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height) style:UITableViewStylePlain];
    _tableView.dataSource = self;
    _tableView.delegate = self;
    _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    _tableView.tableFooterView = [[UIView alloc] init];//多余的cell屏蔽掉
    [self.mainContentView addSubview:_tableView];
    [_tableView reloadData];
}

#pragma mark tableViewDelegate
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return (NSInteger)[NetUtilCallBack getInstance]->m_listOnlineInfo.size();
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    NSUInteger row = [indexPath row];
    if ([NetUtilCallBack getInstance]->m_listOnlineInfo.size()<=0) {
        return [[UITableViewCell alloc]init];
    }
    
    LISTONLINEINFO::iterator it = [NetUtilCallBack getInstance]->m_listOnlineInfo.begin();
    for(int i = 0; i<row; i++)
    {
        it++;
    }
    OnlineInfo& info = (*it);
    OnlineCell *cell =[tableView dequeueReusableCellWithIdentifier:@"cell" ];
    if (cell == nil) {
        cell = [[OnlineCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
        cell.backgroundView = nil;
        cell.backgroundColor = [UIColor clearColor];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    cell.Info = info;
    if (indexPath.row == currentIndex)
        [cell selectRow];
    else
        [cell deSelectRow];
        
    [_dataArray addObject:cell];
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    if (indexPath.row == currentIndex)
        currentIndex = -1;
    else
        currentIndex = indexPath.row;
    OnlineCell *cell = [_dataArray objectAtIndex:indexPath.row];
    //cell->moreView
    [_tableView reloadData];
}
-(CGFloat) tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == currentIndex)
        return  105;
    else
        return  50;
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView{
    if (scrollView == _tableView) {
        if (_tableView.contentOffset.y + _tableView.frame.size.height > (_tableView.contentSize.height + 80)) {
            [_tableView reloadData];
        }
    }
}

@end
