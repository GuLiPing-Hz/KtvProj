//
//  OnlineTableView.m
//  SinaKTVAide
//
//  Created by cxp on 14-8-25.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "OnlineTableView.h"
#import "KTVBaseConfig.h"
#import "OnlineCell.h"
#import "NetUtilCallBack.h"
#import "UserSessionManager.h"

@implementation OnlineTableView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        currentIndex = -1;
        frontIndex = -1;
        _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, _MainScreen_Height-50-44-44-44) style:UITableViewStylePlain];
        _tableView.dataSource = self;
        _tableView.delegate = self;
        _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
        _tableView.tableFooterView = [[UIView alloc] init];//多余的cell屏蔽掉
        [self addSubview:_tableView];
        [self findIndex];
        [_tableView reloadData];
    }
    return self;
}

#pragma mark tableViewDelegate
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return (NSInteger)[NetUtilCallBack getInstance]->m_vectOnlineInfo.size();
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    NSUInteger row = [indexPath row];
    if (row == 0)
        row = selfIndex;
    else if(row <= selfIndex)
        row = row - 1;
    
    if ([NetUtilCallBack getInstance]->m_vectOnlineInfo.size()<=0) {
        return [[UITableViewCell alloc]init];
    }
    OnlineInfo& info = ([NetUtilCallBack getInstance]->m_vectOnlineInfo[row]);
    OnlineCell *cell =[tableView dequeueReusableCellWithIdentifier:@"cell" ];
    if (cell == nil) {
        cell = [[OnlineCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
        cell.backgroundView = nil;
        cell.backgroundColor = [UIColor clearColor];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    cell.Info = info;
    if (indexPath.row == currentIndex){
        if (info.idx != [UserSessionManager getInstance].currentUser.uid) {
            [cell selectRow];
        }
    }
    else
        [cell deSelectRow];
    
    
    [_dataArray addObject:cell];
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    NSUInteger row = [indexPath row];
    if (row == 0)
        row = selfIndex;
    else if(row <= selfIndex)
        row = row - 1;
    
    OnlineInfo info = ([NetUtilCallBack getInstance]->m_vectOnlineInfo[row]);
    if (info.idx == [UserSessionManager getInstance].currentUser.uid) {
        return;
    }
    if (indexPath.row == currentIndex)
        currentIndex = -1;
    else
        currentIndex = indexPath.row;

    [_tableView reloadData];
}
-(CGFloat) tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == currentIndex)
        return  105;
    else
        return  50;
}

-(void)findIndex
{
    for (NSInteger i =0; i<[NetUtilCallBack getInstance]->m_vectOnlineInfo.size(); i++)
    {
        if ([NetUtilCallBack getInstance]->m_vectOnlineInfo[i].idx == [UserSessionManager getInstance].currentUser.uid)
        {
            selfIndex = i;
            return;
        }
    }
}


- (void)tableReload
{
    [self findIndex];
    [_tableView reloadData];
}


@end
