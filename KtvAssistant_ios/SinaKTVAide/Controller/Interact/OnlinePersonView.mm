//
//  OnlinePersonView.m
//  SinaKTVAide
//
//  Created by cxp on 15/5/6.
//  Copyright (c) 2015年 Zhang Chen. All rights reserved.
//

#import "OnlinePersonView.h"
#import "KTVBaseConfig.h"
#import "OnlineCell.h"
#import "NetUtilCallBack.h"
#import "UserSessionManager.h"
#import "RedPresonEditView.h"
#import "InteractController.h"
#import "KTVAppDelegate.h"
#import "DDMenuController.h"

@implementation OnlinePersonView

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        currentIndex = -1;
        frontIndex = -1;
        isShowRoom = YES;
        isShowPrc = YES;
        isShowLobby = YES;
        rowinsection = -1;
    }
    return self;
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [NetUtilCallBack getInstance].thedelegate = self;
    [self.appDelegate.menuController setEnableGesture:NO];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(sendRed:) name:@"ktvassistant_online_sendRed" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(sendGift:) name:@"ktvassistant_online_gif_send_notification" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(sendChat:) name:@"ktvassistant_online_sendChat" object:nil];
}

-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
     [self.appDelegate.menuController setEnableGesture:YES];
    [NetUtilCallBack getInstance].thedelegate = nil;
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - NetUtilCallBackProtocol
-(void)onNameChange:(int)idx name:(const char*)name
{
    [self performSelectorOnMainThread:@selector(tableReload) withObject:nil waitUntilDone:NO];
}

-(void)sendGift:(NSNotification *)notification{
    NSDictionary * userInfo = [notification userInfo];
    NSInteger idx  = [[userInfo objectForKey:@"useridx"] intValue];
    NSString *name = [userInfo objectForKey:@"username"];
    for (UIViewController *controller in self.navigationController.viewControllers) {
        if ([controller isKindOfClass:[InteractController class]]) {
            InteractController *itac = (InteractController *)controller;
            [itac showGift:idx :name];
            [self.navigationController popToViewController:controller animated:YES];
        }
    }
    [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_GIFT_SENDTOSOMEONE_NOTIFICAITON object:nil userInfo:userInfo];
}

-(void)sendChat:(NSNotification *)notification{
    NSDictionary * userInfo = [notification userInfo];
    NSInteger idx  = [[userInfo objectForKey:@"useridx"] intValue];
    NSString *name = [userInfo objectForKey:@"username"];
    for (UIViewController *controller in self.navigationController.viewControllers) {
        if ([controller isKindOfClass:[InteractController class]]) {
            InteractController *itac = (InteractController *)controller;
            [itac showPrivateChat:idx :name];
            [self.navigationController popToViewController:controller animated:YES];
        }
    }
}


-(void)sendRed:(NSNotification *)notification{
    NSDictionary * userInfo = [notification userInfo];
    NSInteger idx  = [[userInfo objectForKey:@"useridx"] intValue];
    
    RedPresonEditView *redview = [[RedPresonEditView alloc] init];
    redview.useridx = idx;
    [self.navigationController pushViewController:redview animated:YES];
}

- (void)delegateOnlineInfo
{
    //[self findIndex];
    [self tableReload];
}

-(void)findIndex
{
    char type = [UserSessionManager getInstance].currentUser.type;
    LISTONLINEINFO::iterator it;
    int i = 0;
    if (type == '0') {
        for (it=[NetUtilCallBack getInstance]->m_listPrincessInfo.begin(); it!=[NetUtilCallBack getInstance]->m_listPrincessInfo.end(); it++)
        {
            if (it->idx == [UserSessionManager getInstance].currentUser.uid)
            {
                selfIndex = i;
                selfsection = 0;
                return;
            }
            i++;
        }
    }
    else if (type == '1')
    {
        for (it=[NetUtilCallBack getInstance]->m_listRoomUserInfo.begin(); it!=[NetUtilCallBack getInstance]->m_listRoomUserInfo.end(); it++)
        {
            if (it->idx == [UserSessionManager getInstance].currentUser.uid)
            {
                selfIndex = i;
                selfsection = 1;
                return;
            }
            i++;
        }

    }
    else
    {
        for (it=[NetUtilCallBack getInstance]->m_listLobbyUserInfo.begin(); it!=[NetUtilCallBack getInstance]->m_listLobbyUserInfo.end(); it++)
        {
            if (it->idx == [UserSessionManager getInstance].currentUser.uid)
            {
                selfIndex = i;
                selfsection = 2;
                return;
            }
            i++;
        }
    }
}

#pragma mark tableViewDelegate
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (section == 0)
    {
        if (!isShowPrc)
            return 0;
        else
            return (NSInteger)[NetUtilCallBack getInstance]->m_listPrincessInfo.size();
    }
    else if (section == 1)
    {
        if (!isShowRoom)
            return 0;
        else
            return (NSInteger)[NetUtilCallBack getInstance]->m_listRoomUserInfo.size();
    }
    else
    {
        if (!isShowLobby)
            return 0;
        else
            return (NSInteger)[NetUtilCallBack getInstance]->m_listLobbyUserInfo.size();
    }
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 3;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{    
    if (section == 0) {
        return @"在线娱乐天使";
    }
    else if(section == 1){
        return @"当前包厢成员";
    }
    else
        return @"身边用户";
}

- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section;
{
    if (section == 0) {
        if ([NetUtilCallBack getInstance]->m_listPrincessInfo.size() == 0) {
            return 0;
        }
    }
    else if (section == 1)
    {
        if ([NetUtilCallBack getInstance]->m_listRoomUserInfo.size() == 0) {
            return 0;
        }
    }
    else
    {
        if ([NetUtilCallBack getInstance]->m_listLobbyUserInfo.size() == 0) {
            return 0;
        }
    }
    return 38;
}

- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    UIView *headerView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, _tableView.bounds.size.width, 38)];
    headerView.backgroundColor = UIColorFromRGB(0xf6f6f7);
    
    UIButton *titleBtn = [[UIButton alloc] initWithFrame:CGRectMake(0, 10, _tableView.bounds.size.width, 28)];
    [titleBtn setTitleColor:UIColorFromRGB(0xbbbbbb) forState:UIControlStateNormal];
    titleBtn.titleLabel.font = [UIFont systemFontOfSize:14.0f];
    titleBtn.backgroundColor = [UIColor whiteColor];
    titleBtn.layer.borderWidth = 0.5f;
    titleBtn.layer.borderColor = [UIColorFromRGB(0xebebeb) CGColor];
    titleBtn.contentEdgeInsets = UIEdgeInsetsMake(0,26, 0, 0);
    titleBtn.contentHorizontalAlignment = UIControlContentHorizontalAlignmentLeft;
    [titleBtn addTarget:self action:@selector(btnpress:) forControlEvents:UIControlEventTouchUpInside];
    titleBtn.tag = section;
    
    int num;
    if (section == 0) {
        [titleBtn setTitle: @"在线娱乐天使" forState: UIControlStateNormal];
        num = [NetUtilCallBack getInstance]->m_listPrincessInfo.size();
    }
    else if(section == 1){
        [titleBtn setTitle: @"当前包厢成员" forState: UIControlStateNormal];
        num = [NetUtilCallBack getInstance]->m_listRoomUserInfo.size();
    }
    else
    {
        [titleBtn setTitle: @"身边用户" forState: UIControlStateNormal];
        num = [NetUtilCallBack getInstance]->m_listLobbyUserInfo.size();
    }
    
    UIImageView *img = [[UIImageView alloc]initWithFrame:CGRectMake(4, 14, 20, 20)];
    img.backgroundColor = [UIColor whiteColor];
    
    if(section == 0)
    {
        if (isShowPrc)
            [img setImage:[UIImage imageNamed:@"expansion_ic_0"]];
        else
            [img setImage:[UIImage imageNamed:@"expansion_ic_1"]];
    }
    if(section == 1)
    {
        if (isShowRoom)
            [img setImage:[UIImage imageNamed:@"expansion_ic_0"]];
        else
            [img setImage:[UIImage imageNamed:@"expansion_ic_1"]];
    }
    if(section == 2)
    {
        if (isShowLobby)
            [img setImage:[UIImage imageNamed:@"expansion_ic_0"]];
        else
            [img setImage:[UIImage imageNamed:@"expansion_ic_1"]];
    }
    
    UILabel *numlbl = [[UILabel alloc] init];
    numlbl.font = [UIFont systemFontOfSize:12.0f];
    numlbl.textColor = UIColorFromRGB(0xbbbbbb);
    numlbl.backgroundColor = [UIColor clearColor];
    numlbl.text = [[NSString alloc] initWithFormat:@"%d",num];
    CGSize size = [numlbl.text sizeWithFont:[UIFont systemFontOfSize:11] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
    numlbl.frame = CGRectMake(_MainScreen_Width-size.width-20, 17, size.width+2, size.height);

    [headerView addSubview:titleBtn];
    [headerView addSubview:img];
    [headerView addSubview:numlbl];
    return headerView;
}

-(void)btnpress:(id)sender
{
    UIButton *btn = (UIButton *)sender;
    rowinsection = btn.tag;
    if (rowinsection == 0) {
        isShowPrc = !isShowPrc;
    }
    else if (rowinsection == 1)
        isShowRoom = !isShowRoom;
    else
        isShowLobby = !isShowLobby;
    [self tableReload];
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    NSUInteger section = indexPath.section;
    NSUInteger row = [indexPath row];
    
    if (indexPath.section == rowinsection) {
        if (row == 0)
            row = selfIndex;
        else if(row <= selfIndex)
            row = row - 1;
    }
    
    OnlineInfo userinfo;
    if (section == 0) {
        if ([NetUtilCallBack getInstance]->m_listPrincessInfo.size()<=0) {
            return [[UITableViewCell alloc]init];
        }
        
        LISTONLINEINFO::iterator prcit = [NetUtilCallBack getInstance]->m_listPrincessInfo.begin();
        for (int i=0; i<row; i++) {
            prcit++;
        }
        userinfo = (*prcit);

    }
    else if (section == 1)
    {
        if ([NetUtilCallBack getInstance]->m_listRoomUserInfo.size()<=0) {
            return [[UITableViewCell alloc]init];
        }
        
        LISTONLINEINFO::iterator roomit = [NetUtilCallBack getInstance]->m_listRoomUserInfo.begin();
        for (int i=0; i<row; i++) {
            roomit++;
        }
        userinfo = (*roomit);
    }
    else
    {
        if ([NetUtilCallBack getInstance]->m_listLobbyUserInfo.size()<=0) {
            return [[UITableViewCell alloc]init];
        }
        
        LISTONLINEINFO::iterator lobbyit = [NetUtilCallBack getInstance]->m_listLobbyUserInfo.begin();
        for (int i=0; i<row; i++) {
            lobbyit++;
        }
        userinfo = (*lobbyit);
    }
    OnlineInfo& info = userinfo;
    OnlineCell *cell =[tableView dequeueReusableCellWithIdentifier:@"cell" ];
    if (cell == nil) {
        cell = [[OnlineCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
        cell.backgroundView = nil;
        cell.backgroundColor = [UIColor clearColor];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    cell.Info = info;
    if (indexPath.row == currentIndex && indexPath.section == frontIndex){
        if (info.idx != [UserSessionManager getInstance].currentUser.uid) {
            [cell selectRow];
        }
    }
    else
        [cell deSelectRow];
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath{
    NSInteger section = indexPath.section;
    NSUInteger row = [indexPath row];
    if (indexPath.section == rowinsection) {
        if (row == 0)
            row = selfIndex;
        else if(row <= selfIndex)
            row = row - 1;
    }
    
    OnlineInfo info;
    LISTONLINEINFO::iterator it;
    if (section == 0) {
        it = [NetUtilCallBack getInstance]->m_listPrincessInfo.begin();
        for (int i = 0; i < row; i++) {
            it++;
        }
        info = (*it);
    }
    else if (section == 1)
    {
        it = [NetUtilCallBack getInstance]->m_listRoomUserInfo.begin();
        for (int i = 0; i < row; i++) {
            it++;
        }
        info = (*it);
    }
    else
    {
        it = [NetUtilCallBack getInstance]->m_listLobbyUserInfo.begin();
        for (int i = 0; i < row; i++) {
            it++;
        }
        info = (*it);
    }
    
    if (info.idx == [UserSessionManager getInstance].currentUser.uid) {
        return;
    }
    if (indexPath.row == currentIndex)
    {
        currentIndex = -1;
        frontIndex = -1;
    }
    else
    {
        currentIndex = indexPath.row;
        frontIndex = indexPath.section;
    }
    [self tableReload];
}
-(CGFloat) tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == currentIndex && indexPath.section == frontIndex)
        return  105;
    else
        return  50;
}

- (void)tableReload
{
    //[self findIndex];
    [_tableView reloadData];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self.navView initOnlyBackButton:@"在线人数"];
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.mainContentView.backgroundColor = UIColorFromRGB(0xf6f6f7);    
    
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, _MainScreen_Height-44) style:UITableViewStylePlain];
    _tableView.dataSource = self;
    _tableView.delegate = self;
    [NetUtilCallBack getInstance].thedelegate = self;
    _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    _tableView.tableFooterView = [[UIView alloc] init];//多余的cell屏蔽掉
    [self.mainContentView addSubview:_tableView];
    [self tableReload];
}
@end
