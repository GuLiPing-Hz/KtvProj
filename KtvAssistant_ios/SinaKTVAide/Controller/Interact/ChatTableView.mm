//
//  InteractTableView.m
//  SinaKTVAide
//
//  Created by cxp on 14-7-23.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "ChatTableView.h"
#import "NetUtilCallBack.h"
#include "PublicChatCell.h"
#import "KTVBaseConfig.h"
#import "NSMutableAttributedString+Weibo.h"
#import "CoreTextView.h"

@implementation ChatTableView

- (id)initWithFrame:(CGRect)frame  type:(NSInteger)type delegate:(id<ChatCellHBProtocol>)delegate
{
    self = [super initWithFrame:frame];
    if (self) {
        self.mDelegate = delegate;
        
        _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, _MainScreen_Height-44-44) style:UITableViewStylePlain];
        _tableView.dataSource = self;
        _tableView.delegate = self;
        _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
        _tableView.tableFooterView = [[UIView alloc] init];//多余的cell屏蔽掉
        _tableView.backgroundColor = UIColorFromRGB(0xe5e5e5);
        [self addSubview:_tableView];
        isLoad = false;
        cType = type;
        lastrow = -1;
        isShow = NO;
        rowheight = 0;
        [self createHeadOpt];
        //[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(imgHead_lblName_Click:) name:KTVASSISTANT_CHAT_IMG_LABEL_CLICK_NOTIFICATION object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showBtn:) name:KTVASSISTANT_ISSHOW_CHAT_GIFT_RED_CLICK_NOTIFICATION object:nil];
        
        [_tableView reloadData];
    }
    return self;
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)createHeadOpt{
    _privateHeadBtn =[[UIButton alloc] initWithFrame:CGRectZero];
    [_privateHeadBtn setBackgroundImage:[UIImage imageNamed:@"letter_btn_0@3x.png"] forState:UIControlStateNormal];
    [_privateHeadBtn setBackgroundImage:[UIImage imageNamed:@"letter_brn_1@3x.png"] forState:UIControlStateHighlighted];
    [_privateHeadBtn setTitle: @"私信" forState:UIControlStateNormal];
    _privateHeadBtn.titleLabel.font = [UIFont systemFontOfSize: 12.0f];
    [_privateHeadBtn setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
    _privateHeadBtn.contentEdgeInsets = UIEdgeInsetsMake(26,0, 0, 0);
    _privateHeadBtn.tag = 0;
    [_privateHeadBtn addTarget:self action:@selector(InteractClick:) forControlEvents:UIControlEventTouchUpInside];
    [_tableView addSubview:_privateHeadBtn];
    _privateHeadBtn.layer.masksToBounds = YES;
    _privateHeadBtn.layer.cornerRadius = 25;
    
    _giftHeadBtn =[[UIButton alloc] initWithFrame:CGRectZero];
    [_giftHeadBtn setBackgroundImage:[UIImage imageNamed:@"gifts_btn_0@3x.png"] forState:UIControlStateNormal];
    [_giftHeadBtn setBackgroundImage:[UIImage imageNamed:@"gifts_btn_1@3x.png"] forState:UIControlStateHighlighted];
    [_giftHeadBtn setTitle: @"送礼" forState:UIControlStateNormal];
    _giftHeadBtn.titleLabel.font = [UIFont systemFontOfSize: 12.0f];
    [_giftHeadBtn setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
    _giftHeadBtn.contentEdgeInsets = UIEdgeInsetsMake(26,0, 0, 0);
    _giftHeadBtn.tag = 1;
    [_giftHeadBtn addTarget:self action:@selector(InteractClick:) forControlEvents:UIControlEventTouchUpInside];
    [_tableView addSubview:_giftHeadBtn];
    _giftHeadBtn.layer.masksToBounds = YES;
    _giftHeadBtn.layer.cornerRadius = 25;
    
    _redHeadBtn =[[UIButton alloc] initWithFrame:CGRectZero];
    [_redHeadBtn setBackgroundImage:[UIImage imageNamed:@"red_btn_2@3x.png"] forState:UIControlStateNormal];
    [_redHeadBtn setBackgroundImage:[UIImage imageNamed:@"red_btn_3@3x.png"] forState:UIControlStateHighlighted];
    [_redHeadBtn setTitle: @"红包" forState:UIControlStateNormal];
    _redHeadBtn.titleLabel.font = [UIFont systemFontOfSize: 12.0f];
    [_redHeadBtn setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
    _redHeadBtn.contentEdgeInsets = UIEdgeInsetsMake(26,0, 0, 0);
    _redHeadBtn.tag = 2;
    [_redHeadBtn addTarget:self action:@selector(InteractClick:) forControlEvents:UIControlEventTouchUpInside];
    [_tableView addSubview:_redHeadBtn];
    _redHeadBtn.layer.masksToBounds = YES;
    _redHeadBtn.layer.cornerRadius = 25;
}

- (void)InteractClick:(id)sender{
    if (cellIndex == 0) {
        isShow = NO;
        pointY -= 58;
        [_tableView reloadData];
    }
    _privateHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
    _giftHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
    _redHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
    UIButton *btn = (UIButton *)sender;
    NSString *strtag = [NSString stringWithFormat:@"%d", btn.tag];
    NSDictionary * userInfo = [NSDictionary dictionaryWithObjectsAndKeys:strtag,@"tag",name,@"username",uid,@"userid", nil];
    [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_CHAT_GIFT_RED_CLICK_NOTIFICATION object:nil userInfo:userInfo];
}

- (void)showBtn:(NSNotification *)notification{
    isShow = NO;
    [UIView animateWithDuration:.2 animations:^{
        _privateHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
        _giftHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
        _redHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
    }];
}
/*
#pragma mark -  UIActionSheet 点击头像送礼 私聊
- (void)imgHead_lblName_Click:(NSNotification *)notification
{
    isShow = !isShow;
    NSDictionary * userInfo = [notification userInfo];
    y = [[userInfo objectForKey:@"y"] intValue];
    NSIndexPath *indexPath = [NSIndexPath indexPathForRow:y inSection:0];
    NSLog(@"row = %d, section = %d", indexPath.row, indexPath.section);
    
    NSArray *array = [_tableView indexPathsForVisibleRows];
    if (_tableView == nil) {
        NSLog(@"_tableView == nil");
    }
    
    NSArray *array1 = [_tableView visibleCells];
    
    UITableViewCell *cell = [_tableView cellForRowAtIndexPath:indexPath];
    if (cell == nil) {
        NSLog(@"cell == nil");
    }

    
    CGRect rectInTableView = [_tableView rectForRowAtIndexPath:indexPath];
    pointY = [_tableView convertRect:rectInTableView toView:[_tableView superview]].origin.y
        + [_tableView contentOffset].y;
  
    uid = [userInfo objectForKey:@"userid"];
    name = [userInfo objectForKey:@"username"];
    
    //第一行和最后一行
    if (y == 0) {
        isShow?rowheight=57:rowheight=0;
        [_tableView reloadData];
    }
    else
    {
        if (rowheight > 0) {
            [_tableView reloadData];
        }
        rowheight = 0;
    }
    
    //如果正在显示，则变小
    if (_privateHeadBtn.frame.size.width > 0) {
        [UIView animateWithDuration:.2 animations:^{
            _privateHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
            _giftHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
            _redHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
        }];
    }
    else//否则变大
    {
        _privateHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
        _giftHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
        _redHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
        
        if (y==0) {
            [UIView animateWithDuration:.2 animations:^{
                _privateHeadBtn.frame = CGRectMake(40, pointY-1, 50, 50);
                _giftHeadBtn.frame = CGRectMake(80, pointY+60, 50, 50);
                _redHeadBtn.frame = CGRectMake(40, pointY+123, 50, 50);
            }];
        }
        else
        {
            [UIView animateWithDuration:.2 animations:^{
                _privateHeadBtn.frame = CGRectMake(40, pointY-58, 50, 50);
                _giftHeadBtn.frame = CGRectMake(80, pointY+3, 50, 50);
                _redHeadBtn.frame = CGRectMake(40, pointY+66, 50, 50);
            }];
        }
    }
    
    //通知是否停止消息滚动
    NSString *strshow = _privateHeadBtn.frame.size.width > 0?@"1":@"0";
    NSDictionary * userInfo2 = [NSDictionary dictionaryWithObjectsAndKeys:strshow,@"show",nil];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"ktvassistant_isshow_optbtn_tomain_notification" object:nil userInfo:userInfo2];
}
*/

#pragma mark - ChatCellHBProtocol
-(void)onHBClick:(AllInfo)info
{
    if(self.mDelegate)
    {
        [self.mDelegate onHBClick:info];
    }
}
-(void)onUserHeadClick:(CGPoint)point view:(id)view name:(NSString*)name idx:(int)idx
{
    isShow = !isShow;
    point = [self convertPoint:point fromView:view];
    PLog(@"Y : %f",point.y);
    uid = [NSString stringWithFormat:@"%d",idx];
    self->name = name;

    pointY = point.y;
    if(pointY <= 0)
        cellIndex = 0;
    else
        cellIndex = 1;
    
    pointY = pointY + self.tableView.contentOffset.y;
    //第一行和最后一行
    if (cellIndex == 0) {
        isShow?rowheight=57:rowheight=0;
        [_tableView reloadData];
    }
    else
    {
        if (rowheight > 0) {
            [_tableView reloadData];
        }
        rowheight = 0;
    }
    
    //如果正在显示，则变小
    if (_privateHeadBtn.frame.size.width > 0) {
        [UIView animateWithDuration:.2 animations:^{
            _privateHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
            _giftHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
            _redHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
        }];
    }
    else//否则变大
    {
        _privateHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
        _giftHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
        _redHeadBtn.frame = CGRectMake(33, pointY+22, 0, 0);
        
        if (cellIndex==0) {
            [UIView animateWithDuration:.2 animations:^{
                _privateHeadBtn.frame = CGRectMake(40, pointY-1, 50, 50);
                _giftHeadBtn.frame = CGRectMake(80, pointY+60, 50, 50);
                _redHeadBtn.frame = CGRectMake(40, pointY+123, 50, 50);
            }];
        }
        else
        {
            [UIView animateWithDuration:.2 animations:^{
                _privateHeadBtn.frame = CGRectMake(40, pointY-58, 50, 50);
                _giftHeadBtn.frame = CGRectMake(80, pointY+3, 50, 50);
                _redHeadBtn.frame = CGRectMake(40, pointY+66, 50, 50);
            }];
        }
    }
    
    //通知是否停止消息滚动
    NSString *strshow = _privateHeadBtn.frame.size.width > 0?@"1":@"0";
    NSDictionary * userInfo2 = [NSDictionary dictionaryWithObjectsAndKeys:strshow,@"show",nil];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"ktvassistant_isshow_optbtn_tomain_notification" object:nil userInfo:userInfo2];
}

#pragma mark -
#pragma mark Table Data Source Methods

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    int size = (NSInteger)[NetUtilCallBack getInstance]->m_vectAllInfo.size();
    if (size > chatRowShowCount)
        return chatRowShowCount;
    else
        return size;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CMainCell = @"CMainCell";
    NSUInteger row = [indexPath row];
    int size = [NetUtilCallBack getInstance]->m_vectAllInfo.size();
    if (size > chatRowShowCount)
        row = size - chatRowShowCount + row;
    if ([NetUtilCallBack getInstance]->m_vectAllInfo.size()<=0) {
        return [[UITableViewCell alloc]init];
    }
    AllInfo info = ([NetUtilCallBack getInstance]->m_vectAllInfo[row]);
   
    PublicChatCell *cell =[tableView dequeueReusableCellWithIdentifier:CMainCell ];
    if (cell == nil) {
        cell = [[PublicChatCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CMainCell];
        cell.backgroundView = nil;
        cell.backgroundColor = [UIColor clearColor];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
        cell.mHBDelegate = self;//设置委托
    }
    cell.y = row;
    if (row == 0 && isShow && cellIndex == 0) {
        cell.isShow = YES;
    }
    else
        cell.isShow = NO;
    cell.Info = info;
    PLog(@"%f", cell.frame.size.width);
    return cell;
}

-(CGFloat) tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSUInteger row = [indexPath row];
    AllInfo info = ([NetUtilCallBack getInstance]->m_vectAllInfo[row]);
    //最后一行
    if (row == [NetUtilCallBack getInstance]->m_vectAllInfo.size()-1 && info.idx != 0 && info.idx != [UserSessionManager getInstance].currentUser.uid)
    {
        return [PublicChatCell CountRowHeight:info] + 25;
    }
    else
    {
        if((row == 0 && cellIndex == 0 && isShow))
        {
            return [PublicChatCell CountRowHeight:info] + rowheight;
        }
        else
            return [PublicChatCell CountRowHeight:info];
    }
}

@end
