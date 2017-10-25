//
//  LeftViewController.m
//  SinaKTVAide
//
//  Created by Li Pan on 13-11-30.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "LeftViewController.h"
#import "KTVMainViewController.h"
#import "KTVSettingViewController.h"
#import "MessageViewController.h"
#import "InteractController.h"
#import "UserCollectedSongViewController.h"
#import "MessageDetailViewController.h"
#import "PKtvAssistantAPI.h"
#import "CommenTools.h"
#import "UIView+i7Rotate360.h"
#import "User.h"
#import "UserCenterViewController.h"
#import "Reachability.h"
#import "KTVBaseConfig.h"
#import "AFHTTPRequestOperation.h"
#import "PCommonUtil.h"
#import "TopicListViewController.h"
#import "GiftExchangeController.h"
#import "KTVInfoController.h"
#import "RechargeController.h"
#import "SingedSongViewController.h"
#include "NetUtilCallBack.h"
#include <stdio.h>
#include <string.h>

typedef NS_ENUM (NSInteger,sectionRows) {
    kHome = -2,//首页
    kUser,//用户中心
    kGold,//账户充值
    kExchange,//金币兑换
    //kConsumer,//消费记录 - 由于不可能存在数据，所以这个版本先不做
    kSing,//唱过的歌
    kCollect,//歌曲收藏
    kSet//系统设置
};

#define RES_TEXT_SIZE 5
#define CHECK_TEXT_SIZE 4  //审核用
//"消费记录"
const char RES_TEXT_MENUITEM [RES_TEXT_SIZE][50] = {"账户充值","礼物兑换","唱过的歌","歌曲收藏","系统设置"};
const char CHECK_TEXT_MENUITEM [RES_TEXT_SIZE][50] = {"账户充值","唱过的歌","歌曲收藏","系统设置"};  //审核用

#define CellUIColorFromRGB(rgbValue) [UIColor colorWithRed:((float)((rgbValue & 0xFF0000) >> 16))/255.0 green:((float)((rgbValue & 0xFF00) >> 8))/255.0 blue:((float)(rgbValue & 0xFF))/255.0 alpha:0.1]

@interface LeftViewController ()
{
    int currentPage;
    BOOL isChangePageing;
    BOOL isShowGift;
}

@end

@implementation LeftViewController

@synthesize menuTableView = _menuTableView,headView = _headView;


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    currentPage = kHome;
    isChangePageing = false;
    isShowGift = YES;
    return self;
}

-(void)setIndexPage:(KTVMainViewController*) page
{
    self.mIndexPage = page;
}

-(void)UpdateInfo
{
    self.titleGold.text = [[NSString alloc] initWithFormat:@"%ld", [UserSessionManager getInstance].currentUser.gold];
    [self.titleGold sizeToFit];
}

#pragma mark -viewWillAppear

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    [self isLoginWithFlashUIData];  // 刷新是否登录的数据
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(UpdateInfo) name:KTVASSISTANT_GOLD_UPDATE_NOTIFICATION object:nil];//刷新金币值
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(isShowGiftChange) name:KTVASSISTANT_ISSHOW_NOTIFICATION object:nil];
    
    isChangePageing = false;
    [self UpdateInfo];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    // 取消动画
    [_headImageView removeRotate360];
    [_headImageView stopAnimating];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - flashUI Data

- (void)isLoginWithFlashUIData
{
    // 用户昵称
    User* user = [UserSessionManager getInstance].currentUser;
    self.titleName.text = user.name;
    // 用户金币
    int gold = user.gold;
    self.titleGold.text = [NSString stringWithFormat:@"%d",gold];
    //用户头像
    NSString *headPhotoString = [UserSessionManager getInstance].currentUser.headphoto;
    _standardHeadPhotoString = [CommenTools GetURLWithResolutionScaleTransfered:headPhotoString scale:0];
    [self.headImageView setImageWithURL:[NSURL URLWithString:_standardHeadPhotoString] placeholderImage:[UIImage imageNamed:@"userFace_normal_b.png"]];

    [self performSelector:@selector(headPhotoAnimation:) withObject:_standardHeadPhotoString afterDelay:0.7];
    
    [self updateSongInfo];
}

#pragma mark - viewDidLoad

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self isShowGiftChange];
    [self creatHeadPhotoButton]; // 头像按钮
	[self creatMenuTableView]; // 菜单列表
    
}

- (void)isShowGiftChange{
    NSUserDefaults * defaults = [NSUserDefaults standardUserDefaults];
    NSString *defaultKey = KTVASSISTANT_USERDEFAULTS_ISSHOW;
    NSString *value = [defaults objectForKey:defaultKey];
    if (value != nil) {
        if ([value compare:@"1"] == NSOrderedSame)
        {
            isShowGift = YES;
            return;
        }
        else
        {
            isShowGift = NO;
        }
    }
}

- (void)creatHeadPhotoButton
{
#define X_START_IDX 105
#define X_START_HEADPHOTO 24
    
    // 背景
    _bgImage = [[UIImageView alloc] init];
    _bgImage.image = [UIImage imageNamed:@"left_default_bg.png"];
    _bgImage.frame = CGRectMake(0, 0, _MainScreen_Width, _MainScreen_Height+20); 
    _bgImage.userInteractionEnabled = YES;
    [self.view addSubview:_bgImage];

    // 头像，登录，积分整个大按钮
    _headView = [UIButton buttonWithType:UIButtonTypeCustom];
    _headView.backgroundColor = [UIColor clearColor];
    
    User *user = [UserSessionManager getInstance].currentUser;
    
    // 头像
    self.headImageView = [[UIImageView alloc] init];
    self.headImageView.layer.cornerRadius = 30.0;
    self.headImageView.layer.borderWidth = 1.0;
    self.headImageView.layer.borderColor = [UIColor whiteColor].CGColor;
    self.headImageView.layer.masksToBounds = YES;
    NSString* str_photo = [CommenTools GetURLWithResolutionScaleTransfered:user.headphoto scale:0];
    [self.headImageView setImageWithURL:[NSURL URLWithString:str_photo] placeholderImage:[UIImage imageNamed:@"userFace_default"]];
    [self.headView addSubview:self.headImageView];
    
    // 性别
    if (user && user.gender == 0)
        _genderImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"icon_women.png"]];
    else
        _genderImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"icon_men.png"]];

    _genderImageView.backgroundColor = [UIColor clearColor];
    [self.headView addSubview:_genderImageView];
    
    // 用户昵称
    self.titleName = [[UILabel alloc] init];
    if(user)
        [self.titleName setText:user.name];
    else
        self.titleName.text = @"立即登录";
    self.titleName.backgroundColor =[UIColor clearColor];
    self.titleName.font = [UIFont systemFontOfSize:16.0f];
    self.titleName.textColor = UIColorFromRGB(0xffffff);
    [self.headView addSubview:self.titleName];
    
    // 用户IDX
    self.titleIdx = [[UILabel alloc] init];
    self.titleIdx.backgroundColor =[UIColor clearColor];
    self.titleIdx.font = [UIFont systemFontOfSize:13.0f];
    self.titleIdx.textColor = UIColorFromRGB(0x88d8d8d);
    [self.titleIdx setText:[NSString stringWithFormat:@"ID: %ld",(user!=nil?user.uid:0)]];
    [self.headView addSubview:self.titleIdx];
    
    // 金币icon
    UIImageView *goldIcon = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"left_icon_money.png"]];
    goldIcon.backgroundColor = [UIColor clearColor];
    [self.headView addSubview:goldIcon];
    
    // 金币
    self.titleGold = [[UILabel alloc] init];
    self.titleGold.text = [NSString stringWithFormat:@"0"];
    self.titleGold.backgroundColor =[UIColor clearColor];
    self.titleGold.font = [UIFont systemFontOfSize:13.0f];
    self.titleGold.textColor = UIColorFromRGB(0xcfac2c);
    [self.headView addSubview: self.titleGold];
    
    self.mUiLabelSongLast = [[UILabel alloc] init];
    [self.mUiLabelSongLast setFont:[UIFont systemFontOfSize:16]];
    [self.mUiLabelSongLast setTextColor:UIColorFromRGB(0x797388)];
    [self.headView addSubview:self.mUiLabelSongLast];
    
    UIView* ui_line = [[UIView alloc] initWithFrame:CGRectMake(X_START_HEADPHOTO, 155+(IOS7_OR_LATER?20:0), _MainScreen_Width, 1)];
    [ui_line setBackgroundColor:[UIColor colorWithRed:1.0f green:1.0f blue:1.0f alpha:0.12f]];
    [self.headView addSubview:ui_line];
    
    if (IOS7_OR_LATER)
    {
        _headView.frame = CGRectMake(0, 25, 246, 167);
        _headImageView.frame = CGRectMake(X_START_HEADPHOTO, 25+20, 64, 64);
        _genderImageView.frame=CGRectMake(102, 26+20, 14, 14);
        self.titleName.frame = CGRectMake(120, 26+20, 90, 18);
        self.titleIdx.frame = CGRectMake(X_START_IDX, 56+20, 120, 12);
        goldIcon.frame = CGRectMake(X_START_IDX,78+20, 12, 12);
         self.titleGold.frame = CGRectMake(120, 76+20, 100, 12);
        [self.mUiLabelSongLast setFrame:CGRectMake(X_START_HEADPHOTO, 140, 200, 18)];
    }
    else
    {
        _headView.frame = CGRectMake(0, 25, 246, 167-20);
        _headImageView.frame = CGRectMake( X_START_HEADPHOTO, 25, 64, 64);
        _genderImageView.frame=CGRectMake(102, 26, 14, 14);
        self.titleName.frame = CGRectMake(120, 26, 90, 15);
        self.titleIdx.frame = CGRectMake(X_START_IDX, 56, 120, 12);
        goldIcon.frame = CGRectMake(X_START_IDX,78 , 12, 12);
         self.titleGold.frame = CGRectMake(120, 76, 100, 12);
        [self.mUiLabelSongLast setFrame:CGRectMake(X_START_HEADPHOTO, 140, 200, 18)];
    }
    
    [self.headView addTarget:self action:@selector(actionHeadPhoto:) forControlEvents:UIControlEventTouchUpInside];

    [self.view addSubview:self.headView];
}

- (void)headPhotoAnimation:(NSString *)HeadPhotoString
{
    [_headImageView rotate360WithDuration:2.0 repeatCount:1 timingMode:i7Rotate360TimingModeLinear];
    _headImageView.animationDuration = 2.0;
    _headImageView.animationImages = [NSArray arrayWithObjects:self.headImageView.image,self.headImageView.image,self.headImageView.image,self.headImageView.image,nil];
    _headImageView.animationRepeatCount = 1;
    [_headImageView startAnimating];
}

-(void)updateSongInfo
{
    KtvUser* ktvuser = &([NetUtilCallBack getInstance]->m_myInfo);
    //用户演唱记录
    if(ktvuser && strcmp(ktvuser->fullInfo.songname,"0") != 0)
    {
        char txt_songinfo[200] = {0};
        sprintf(txt_songinfo, "%s  %s  %0.1f分",ktvuser->fullInfo.singername,ktvuser->fullInfo.songname,ktvuser->fullInfo.score/10.0f);
        [self.mUiLabelSongLast setText:[NSString stringWithUTF8String:txt_songinfo]];
    }
    else
        [self.mUiLabelSongLast setText:@"暂无演唱记录~"];
}

#pragma mark - NetUtilCallBackProtocol
-(void)onRecvSingSongChange:(const char*)songname singer:(const char*)p1 score:(int)p2
{
    [self performSelectorOnMainThread:@selector(updateSongInfo) withObject:nil waitUntilDone:NO];
}
#pragma mark - 点击头像进入个人中心

- (void)actionHeadPhoto:(UIButton *)button
{
    
    if(currentPage == kUser)//如果已经是当前页，则直接show
    {
        [self.appDelegate.menuController showRootController:YES];
        return ;
    }
    
    // 用户个人中心
    UserCenterViewController *profile = [[UserCenterViewController alloc] init];
    [self.mIndexPage.navigationController pushViewController:profile animated:YES];
    [self.appDelegate.menuController showRootController:YES];
}
// 菜单列表  头像headView作为列表头部tableHeaderView
- (void)creatMenuTableView
{
    _menuTableView = [[UITableView alloc] initWithFrame:CGRectMake(0, self.headView.frame.origin.y+self.headView.frame.size.height+10, _MainScreen_Width, _MainScreen_Height+64) style:UITableViewStylePlain];
    _menuTableView.backgroundColor = [UIColor clearColor];
    _menuTableView.backgroundView = nil;
    _menuTableView.dataSource = self;
    _menuTableView.delegate = self;
    _menuTableView.rowHeight = 50;
    _menuTableView.scrollEnabled = YES;
    _menuTableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    //_menuTableView.tableHeaderView = _headView;
    _menuTableView.tableFooterView = [[UIView alloc] init];//多余的cell屏蔽掉
    [self.view addSubview:_menuTableView];
}

#pragma mark -UITableView delegate

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (isShowGift)
        return RES_TEXT_SIZE;
    else
        return CHECK_TEXT_SIZE;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString * cellIdentifier = @"Cell";
    UITableViewCell *cell;
    if ([_menuTableView isEqual:tableView])
    {
        cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
        if (cell == nil)
        {
            cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellIdentifier];
            // 定制头像
            UIImageView *headView = [[UIImageView alloc] initWithFrame:CGRectMake(24, 8, 30, 30)];
            headView.backgroundColor = [UIColor clearColor];
            headView.tag = 1001;
            [cell.contentView addSubview:headView];
            // 定制标题
            UILabel *title = [[UILabel alloc] initWithFrame:CGRectMake(70, 8, 100, 30)];
            title.tag = 1002;
            title.backgroundColor = [UIColor clearColor];
            title.font = [UIFont systemFontOfSize:16.0f];
            title.textColor = UIColorFromRGB(0x929292);
            [cell.contentView addSubview:title];
        }
        cell.backgroundColor = [UIColor clearColor];
        cell.backgroundView = nil;
        cell.selectedBackgroundView = [[UIView alloc] initWithFrame:cell.bounds];
        UIImageView *headView = (UIImageView *)[cell.contentView viewWithTag:1001];
        UILabel *title = (UILabel *)[cell.contentView viewWithTag:1002];
        if (isShowGift)
            [title setText:[NSString stringWithUTF8String:RES_TEXT_MENUITEM[indexPath.row]]];
        else
            [title setText:[NSString stringWithUTF8String:CHECK_TEXT_MENUITEM[indexPath.row]]];
        //title.text = [self.arrayMenuList objectAtIndex:indexPath.row];
        cell.selectedBackgroundView.backgroundColor = CellUIColorFromRGB(0x000000);
        
        if (isShowGift) {
            switch (indexPath.row)
            {
                    //            case kHome://首页
                    //                break;
                    //            case kUser://用户中心
                    //                break;
                case kGold://账户充值
                    [headView setImage:[UIImage imageNamed:@"ic_topup"]];
                    break;
                case kExchange://金币兑换
                    [headView setImage:[UIImage imageNamed:@"ic_exchange"]];
                    break;
                    //            case kConsumer://消费记录
                    //                [headView setImage:[UIImage imageNamed:@"ic_consumption"]];
                    //                break;
                case kSing://唱过的歌
                    [headView setImage:[UIImage imageNamed:@"ic_singing"]];
                    break;
                case kCollect:
                    [headView setImage:[UIImage imageNamed:@"ic_collect"]];
                    break;
                case kSet://系统设置
                    [headView setImage:[UIImage imageNamed:@"ic_setup"]];
                    break;
                    
                default:
                    break;
            }
        }
        else
        {
            switch (indexPath.row)
            {
                case kGold://账户充值
                    [headView setImage:[UIImage imageNamed:@"ic_topup"]];
                    break;
                case kExchange://唱过的歌
                    [headView setImage:[UIImage imageNamed:@"ic_singing"]];
                    break;
                case kSing:
                    [headView setImage:[UIImage imageNamed:@"ic_collect"]];
                    break;
                case kCollect://系统设置
                    [headView setImage:[UIImage imageNamed:@"ic_setup"]];
                    break;
                    
                default:
                    break;
            }
        }
    }
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 44.0;
}

#pragma mark - Switch Menu
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if(isChangePageing)
        return ;
    
    isChangePageing = true;
    if ([_menuTableView isEqual:tableView])
    {
        if (isShowGift) {
            switch (indexPath.row)
            {
                    //            case kHome:
                    //            case kUser:
                    //                break;
                case kGold://账户充值
                {
                    RechargeController *rechange = [[RechargeController alloc] init];
                    [self.mIndexPage.navigationController pushViewController:rechange animated:YES];
                    break;
                }
                case kExchange://礼物兑换
                {
                    KTVInfoController *ktvInfo = [[KTVInfoController alloc] init];
                    [self.mIndexPage.navigationController pushViewController:ktvInfo animated:YES];
                    break;
                }
                    //            case kConsumer://消费记录 - new
                    //            {
                    //                break;
                    //            }
                case kSing://唱过的歌 - new
                {
                    SingedSongViewController* singed = [[SingedSongViewController alloc] init];
                    [self.mIndexPage.navigationController pushViewController:singed animated:YES];
                    break;
                }
                case kCollect://歌曲收藏
                {
                    UserCollectedSongViewController * userCollect = [[UserCollectedSongViewController alloc]init];
                    [self.mIndexPage.navigationController pushViewController:userCollect animated:YES];
                    break;
                }
                case kSet://系统设置
                {
                    KTVSettingViewController *set = [[KTVSettingViewController alloc] init];
                    [self.mIndexPage.navigationController pushViewController:set animated:YES];
                    //BaseNavigationViewController *navSet= [[BaseNavigationViewController alloc] initWithRootViewController:set];
                    //[self.appDelegate.menuController setRootController:navSet animated:YES];
                    break;
                }
                    
                default:
                    break;
            }
        }
        else{
            switch (indexPath.row)
            {
                case kGold://账户充值
                {
                    RechargeController *rechange = [[RechargeController alloc] init];
                    [self.mIndexPage.navigationController pushViewController:rechange animated:YES];
                    break;
                }
                case kExchange://唱过的歌 - new
                {
                    SingedSongViewController* singed = [[SingedSongViewController alloc] init];
                    [self.mIndexPage.navigationController pushViewController:singed animated:YES];
                    break;
                }
                case kSing://歌曲收藏
                {
                    UserCollectedSongViewController * userCollect = [[UserCollectedSongViewController alloc]init];
                    [self.mIndexPage.navigationController pushViewController:userCollect animated:YES];
                    break;
                }
                case kCollect://系统设置
                {
                    KTVSettingViewController *set = [[KTVSettingViewController alloc] init];
                    [self.mIndexPage.navigationController pushViewController:set animated:YES];
                    //BaseNavigationViewController *navSet= [[BaseNavigationViewController alloc] initWithRootViewController:set];
                    //[self.appDelegate.menuController setRootController:navSet animated:YES];
                    break;
                }
                    
                default:
                    break;
            }
        }
        
        [self.appDelegate.menuController showRootController:YES];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
