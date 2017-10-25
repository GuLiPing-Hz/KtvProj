//
//  KTVMainViewController.m
//  ;
//
//  Created by Zhang Chen on 13-10-22.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "KTVMainViewController.h"
#import "QRCodeGenerator.h"
#import "KTVBaseConfig.h"
#import "PKtvAssistantAPI.h"
#import "KTVSettingViewController.h"
#import "PKtvAssistantAPI.h"
#import "RegexKitLite.h"
#import "iToast.h"
#import "KTVInputSecretController.h"
#import "BoxPreBookViewController.h"
#import "UIFont+PFontCategory.h"
#import "BaseNavigationViewController.h"
#import "SongsBookViewController.h"
#import "ListMenuCell.h"
#import "KTVAppDelegate.h"
#import "TopicListViewController.h"
#import "BookedAndCollectedController.h"
#import "BannerInfo.h"
#import "UIImageView+AFNetworking.h"
#import "SearchResultViewController.h"
#import "UserSessionManager.h"
#import "TopSongsListViewController.h"
#import "SongInfo.h"
// add 
#import "CMActionSheet.h"
//#import "BPush.h"
#import "InteractController.h"
#import "NetUtilCallBack.h"
#import "GiftScrollView.h"
#import "NSMutableAttributedString+Weibo.h"
#import "CoreTextView.h"
#import "xingguang.h"
#include "ResolutionDef.h"
#include "UserCollectedSongViewController.h"
#import "RankingsViewController.h"
#import "HongBGetViewController.h"
#import "NIBadgeView.h"
#import "CResponse.h"
#import "com_ios_NetUtil.h"
#import "ExchangeHistoryController.h"

#define RE_SEARCH_HEIGHT 44
#define RE_BANNER_HEIGHT 145

@interface KTVMainViewController ()

@property (atomic,assign) BOOL mIsInChatUI;
@property (atomic,assign) int mHBUnreadNumber;
@property (atomic,assign) int mChatUnreadNumber;

@end

@implementation KTVMainViewController

@synthesize boxVodActionSheet = _boxVodActionSheet,keyWord = _keyWord,arrayImage = _arrayImage,pageImage = _pageImage,scrollImage =_scrollImage,timer =_timer;
@synthesize arrayListTable = _arrayListTable,spView = _spView,reader = _reader;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        _arrayListTable = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)loadBannerData
{
    KTVAppDelegate * app = (KTVAppDelegate *)[[UIApplication sharedApplication] delegate];
    
    if (_bannerListArray.count == 0 && app.bannerListArray.count > 0) {
        [_bannerListArray addObjectsFromArray:app.bannerListArray];
        [_arrayImage addObjectsFromArray:app.bannerImageArray];
        
        [self setScrollView];
        return;
    }
    
    if (_bannerListArray.count > 0)
        return;
    
    NSString * urlStr = [PKtvAssistantAPI getBannerListUrl:[UserSessionManager getInstance].currentUser.uid roomId:[UserSessionManager getInstance].currentRoomInfo.roomId];
    NSURL *url = [NSURL URLWithString:urlStr];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject)
    {
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame)
        {
            NSDictionary *result = [results objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"bannerlists"];
            int datalistlen = [dataList count];
            for (int i=0; i<datalistlen; i++) {
                BannerInfo * banner = (BannerInfo *)[BannerInfo initWithDictionary:[dataList objectAtIndex:i]];
                //[banner log];
                if (banner.type == 1 || banner.type == 2 || banner.type == 3) {
                    UIImageView * imageView = [[UIImageView alloc]init];
                    NSString *backImg =[NSString stringWithFormat:@"mainbanner%d.jpg", i + 1];
                    
                    [imageView setImageWithURL:[NSURL URLWithString:banner.imagePhoto] placeholderImage:[UIImage imageNamed:backImg]];
                    [_bannerListArray addObject:banner];
                    [_arrayImage addObject:imageView];
                    [app.bannerListArray addObject:banner];
                    [app.bannerImageArray addObject:imageView];
                }
            }
            [self setScrollView];
        }
        else
        {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doGetBannerList failed...errorcode:%@(%@)", errorCode, msg);
        }
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        PLog(@"doGetBannerList failed...");
    }];
    [operation start];
}


- (void)pushIntoOrderedSongsList
{
    BookedAndCollectedController * orderedSongsList = [[BookedAndCollectedController alloc]init];
    [self.navigationController pushViewController:orderedSongsList animated:YES];
}

- (void)loadAlreadyOrderedSongs{
    if ([UserSessionManager isRoomAlreadyLogin]) {
        
    }else{
        return;
    }
}

#pragma mark - 退出包厢 UIAlertView delegate

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(alertView == self.altExitRoom)
    {
        // 退出包厢
        if (buttonIndex == 1)
        {
            if(![[Reachability reachabilityForInternetConnection] isReachable])
            {
                [self showNetwork];
                return;
            }
            //[SVProgressHUD showWithStatus:@"正在退出..." maskType:SVProgressHUDMaskTypeBlack];
            long int userID = [UserSessionManager getInstance].currentUser.uid;
            NSString *password  = [UserSessionManager getInstance].currentRoomInfo.roomPassword;
            
            NSString *roomId = [UserSessionManager getInstance].currentRoomInfo.roomId;
            NSString *encodeUrl =  [PKtvAssistantAPI getRoomLogoutUrl:userID roomID:roomId password:password];
            NSURL *url = [NSURL URLWithString:encodeUrl];
            NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
            
            [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
            AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
            [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject)
             {
                 NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
                 NSString *status = [results objectForKey:@"status"];
                 if ([status compare:@"1"] == NSOrderedSame)
                 {
                     PLog(@"退出包厢成功");
                     DisConnectRoom();
                 }
                 else
                 {
                     PLog(@"退出包厢失败");
                     NSString *msg = [results objectForKey:@"msg"];
                     NSString *errorCode = [results objectForKey:@"errorcode"];
                     PLog(@"doLoginRoomWithUser failed...errorcode:%@(%@)", errorCode, msg);
                 }
                 
             }
                                             failure:^(AFHTTPRequestOperation *operation, NSError *error)
             {
                 PLog(@"doLogout failed...");
             }];
            [operation start];
            
            //NSString * tag = [password lowercaseString];
            //PLog(@"TAG-->>%@",tag);
            //[BPush delTag:tag];
            [UserSessionManager clearCurrentRoomInfo];
            [self flashUIWithRoom]; // 退出包厢刷新room
            [self getAlreadyOrderedSongsNumber];
            
            [self setRightAndBadgeButton];
            
            self.alreadyLoad = NO;
        }
        
        [alertView dismissWithClickedButtonIndex:0 animated:YES];
    }
    else if(alertView == self.mAlertExchange)
    {
        if (buttonIndex == 1) {
            [SVProgressHUD showWithStatus:@"正在兑换"];
            int number = [[self.mMapData objectForKey:@"giftnum"] intValue];
            NSString *strUrl = [PKtvAssistantAPI getGetGiftExchangeUrl:[UserSessionManager getInstance].currentUser.uid giftID:[self.mMapData objectForKey:@"giftid"] ktvID:[self.mMapData objectForKey:@"ktvid"] num:number];
            NSURL *url = [[NSURL alloc]initWithString:strUrl];
            NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url];
            [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
            AFHTTPRequestOperation * operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
            [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
                NSDictionary *html = [PCommonUtil NSDictionaryFromJSONData2:responseObject];
                NSString *status = [html objectForKey:@"status"];
                NSString *msg = [html objectForKey:@"msg"];
                if ([status compare:@"1"]==NSOrderedSame)
                {
                    long money = [[html objectForKey:@"result"] intValue];
                    [CommenTools showAlertViewWithMessage: @"兑换成功,请用兑换码去前台兑换(兑换码在兑换记录查看)"];
                    
                    [[UserSessionManager getInstance] updateCurrentUserGold: money];
                    ExchangeHistoryController *controller =[[ExchangeHistoryController alloc] init];
                    [self.navigationController pushViewController:controller animated:true];
                    [SVProgressHUD dismiss];
                }
                else
                {
                    [CommenTools showAlertViewWithMessage:[NSString stringWithFormat:@"兑换失败:%@",msg]];
                    [SVProgressHUD dismiss];
                }
            } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
                [SVProgressHUD dismiss];
            }];
            [operation start];
        }
    }
}

#pragma mark -  UIActionSheet 进入包厢

- (void)goInRoom:(UIButton *)btn
{
    //********* 自定义表单 *********
    CMActionSheet *actionSheet = [[CMActionSheet alloc] init];
    actionSheet.title = @"请先进入包厢";
    [actionSheet addSeparator];
    
    [actionSheet addButtonWithTitle:@"扫描包厢二维码" type:CMActionSheetButtonTypeWhite block:^{
        PLog(@"扫描二维码进包厢");
        self.mScanType = SCAN_ROOMPWD;
        [self openAction];
    }];
    [actionSheet addButtonWithTitle:@"输入包厢密码" type:CMActionSheetButtonTypeBlue block:^{
        PLog(@"输入房间密码进包厢");
        KTVInputSecretController *input = [[KTVInputSecretController alloc] init];
        [self.navigationController pushViewController:input animated:YES];
    }];
    [actionSheet addButtonWithTitle:@"取消" type:CMActionSheetButtonTypeRed block:^{
        PLog(@"取消");
    }];
    [actionSheet present];
}

#pragma mark - 搜索歌曲

- (void)creatSongSearchBar
{
    _songSearchBar = [[UISearchBar alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, RE_SEARCH_HEIGHT)];
    _songSearchBar.autocorrectionType = UITextAutocorrectionTypeNo;
    if (IOS7_OR_LATER) {
//        _songSearchBar.barTintColor = UIColorFromRGB(0xebecf1);
    }
    else
    {
        _songSearchBar.tintColor = UIColorFromRGB(0xc0c1c4);
        
    }
    _songSearchBar.autocapitalizationType = UITableViewCellSeparatorStyleNone;
    _songSearchBar.keyboardType = UIKeyboardTypeDefault;
    _songSearchBar.delegate = self;
    _songSearchBar.backgroundColor = [UIColor clearColor];
    _songSearchBar.placeholder = @"请输入歌手名或歌曲名";
    [self.mainContentView addSubview:_songSearchBar];
    [self.mainContentView bringSubviewToFront:_songSearchBar];
    
    // 轻敲手势
    UITapGestureRecognizer *keyboard_tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(dismissKeyboard)];
    keyboard_tapGesture.cancelsTouchesInView = NO;
    
    // 键盘弹出透明遮盖view
    self.view_loading = [[UIView alloc] initWithFrame:CGRectMake(0, RE_SEARCH_HEIGHT, _MainScreen_Width, self.mainContentView.frame.size.height-RE_SEARCH_HEIGHT)];
    self.view_loading.backgroundColor = [UIColor colorWithWhite:0.0f alpha:0.8f];
    [self.view_loading addGestureRecognizer:keyboard_tapGesture];
    [self.mainContentView addSubview:self.view_loading];
    [self.mainContentView sendSubviewToBack:self.view_loading];
    self.view_loading.hidden = YES;
}

#pragma mark - banner 滚动条

- (void)creatScrollView
{
    _scrollImage = [[UIScrollView alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, RE_BANNER_HEIGHT)];
    _scrollImage.backgroundColor = [UIColor clearColor];
    _scrollImage.showsHorizontalScrollIndicator = NO;
    _scrollImage.showsVerticalScrollIndicator = NO;
    _scrollImage.pagingEnabled = YES;
    _scrollImage.directionalLockEnabled = YES;
    _scrollImage.bounces = NO;
    _scrollImage.delegate = self;
    self.bannerAdmin = [[UIImageView alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, RE_BANNER_HEIGHT)];
    [self.bannerAdmin setImage:[UIImage imageNamed:@"mainbanner1.jpg"]];
    [_scrollImage addSubview:self.bannerAdmin];
    
    [_spView addSubview:_scrollImage];
    
    _tapBanner = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(tapBanner:)];
    [_scrollImage addGestureRecognizer:_tapBanner];
    
    _pageImage = [[UIPageControl alloc]initWithFrame:CGRectMake(110, 130, 100, 18)];
    _pageImage.backgroundColor = [UIColor clearColor];
    _pageImage.currentPage = 0;
    [_pageImage addTarget:self action:@selector(changePage) forControlEvents:UIControlEventValueChanged];
    [_spView addSubview:_pageImage];
}

//滚动图设置
- (void)setScrollView{
    _scrollImage.contentSize = CGSizeMake(self.arrayImage.count*_MainScreen_Width, RE_BANNER_HEIGHT);
    if (self.arrayImage.count > 0) {
        [self.bannerAdmin removeFromSuperview];
    }
    
    for (int i = 0; i < self.arrayImage.count; i++) {   
        UIImageView * image = [self.arrayImage objectAtIndex:i];
        image.frame = CGRectMake(i*_MainScreen_Width, 0, _MainScreen_Width, RE_BANNER_HEIGHT);
        
        [_scrollImage addSubview:image];
    }
    
    _pageImage.numberOfPages = self.arrayImage.count;
    
    _timer = [NSTimer scheduledTimerWithTimeInterval:6 target:self selector:@selector(imageScroll) userInfo:nil repeats:YES];
    _bannerScrollTimer = [NSTimer scheduledTimerWithTimeInterval:0.01 target:self selector:@selector(bannerScrolling) userInfo:nil repeats:YES];
    [_bannerScrollTimer setFireDate:[NSDate distantFuture]];
    
    //[_timer fire];
}

//banner图片滚动
- (void)imageScroll{
    [_bannerScrollTimer setFireDate:[NSDate distantPast]];
    
}

- (void)bannerScrolling{
    if (_scrollLeft) {
        _scrollImage.contentOffset = CGPointMake(_scrollImage.contentOffset.x - 10, _scrollImage.frame.origin.y);
    }else{
        _scrollImage.contentOffset = CGPointMake(_scrollImage.contentOffset.x + 10, _scrollImage.frame.origin.y);
    }
}

- (void)changePage
{
    int page = _pageImage.currentPage;  // 获取当前的page
    CGRect frame = _scrollImage.frame;
    frame.origin.y = 0;  // y 坐标不变
    frame.origin.x = frame.size.width*page; // x 坐标改变
    [_scrollImage scrollRectToVisible:frame animated:YES]; // 滚动切换图片，动画效果
}

//banner点击处理
- (void)tapBanner:(id)sender{
    
    if (_bannerListArray.count == 0) {
        return;
    }
    
    BannerInfo * info = [_bannerListArray objectAtIndex:_pageImage.currentPage];
    
    if (info.type == 1) {
        SongsBookViewController * bannerSongsList = [[SongsBookViewController alloc] initInfoNoSinger:info.songParameter.modelID topicId:info.songParameter.topicID title:info.title];
        
        [self.navigationController pushViewController:bannerSongsList animated:YES];
    }else if(info.type == 2){
        TopSongsListViewController * bannerSongsList = [[TopSongsListViewController alloc]init];
        bannerSongsList.modelID = info.songParameter.modelID;
        bannerSongsList.topicID = info.songParameter.topicID;
        bannerSongsList.listTitle = info.title;
        
        [self.navigationController pushViewController:bannerSongsList animated:YES];
    }
    
}

- (void)goInViewController:(UIButton *)button
{
//    ListMenuCell *cell = (ListMenuCell *)[button superview];
//    NSIndexPath *indexPath = [_listTable indexPathForCell:cell];
}

#pragma mark - UIScrollView delegate

//- (void)scrollViewDidScroll:(UIScrollView *)sender
//{
//    CGFloat pagewidth = self.scrollImage.frame.size.width;
//    int page = floor((self.scrollImage.contentOffset.x - pagewidth/([self.arrayImage count]+2))/pagewidth)+1;
//    page --;  // 默认从第二页开始
//    _pageImage.currentPage = page;
//}
//- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
//{
//    CGFloat pagewidth = self.scrollImage.frame.size.width;
//    CGFloat height = _scrollImage.frame.size.height;
//    int currentPage = floor((self.scrollImage.contentOffset.x - pagewidth/ ([self.arrayImage count]+2)) / pagewidth) + 1;
//    //    int currentPage_ = (int)self.scrollView.contentOffset.x/320; // 和上面两行效果一样
//    //    PLog(@"currentPage_==%d",currentPage_);
//    if (currentPage==0)
//    {
//        [self.scrollImage scrollRectToVisible:CGRectMake(320 * [self.arrayImage count],0,_MainScreen_Width,height) animated:NO]; // 序号0 最后1页
//    }
//    else if (currentPage==([self.arrayImage count]+1))
//    {
//        [self.scrollImage scrollRectToVisible:CGRectMake(_MainScreen_Width,0,_MainScreen_Width,height) animated:NO]; // 最后+1,循环第1页
//    }
//}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView{
    if (scrollView == _scrollImage) {
        if (_scrollImage.contentOffset.x == _MainScreen_Width * (_arrayImage.count-1)) {
            _scrollLeft = YES;
            
        }else if (_scrollImage.contentOffset.x == 0){
            _scrollLeft = NO;
        }
        
        if ((int)_scrollImage.contentOffset.x%320 == 0) {
            [_bannerScrollTimer setFireDate:[NSDate distantFuture]];
        }
        _pageImage.currentPage = (int)(_scrollImage.contentOffset.x+160)/320;
        //PLog(@"%f",_scrollImage.contentOffset.y);
    }
}

#pragma mark - viewWillAppear

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [_songSearchBar resignFirstResponder];
    
    [self flashUIWithRoom];
    
    if (_bannerListArray == nil) {
        _bannerListArray = [[NSMutableArray alloc]init];
        _arrayImage = [[NSMutableArray alloc]init];
    }
    
    [self loadBannerData];

    [self getAlreadyOrderedSongsNumber];
    
    if ([UserSessionManager isRoomAlreadyLogin]) {
        
    }else{
        self.alreadyLoad = NO;
    }
    
    [self loadAlreadyOrderedSongs];
    [self setRightAndBadgeButton];
    
    [self onChangeChatTip];
    [self onChangeHBTip];
    
    self.mIsInChatUI = NO;
    [NetUtilCallBack getInstance].thedelegate = self;
    //[self getAlreadyOrderedSongsNumber];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(disconnRoom:) name:KTVASSISTANT_CLOSEROOM_NOTIFICATION object:nil];
}

-(void)disconnRoom:(NSNotification *)notification
{
    [self performSelectorOnMainThread:@selector(closeRoom) withObject:nil waitUntilDone:YES];
}

-(void)closeRoom
{
    DisConnectRoom();
    [UserSessionManager clearCurrentRoomInfo];
    [self flashUIWithRoom]; // 退出包厢刷新room
}

-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [NetUtilCallBack getInstance].thedelegate = nil;
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark  Index Button
-(void)createButtonView
{
    CGFloat y_scrollBtn = RE_SEARCH_HEIGHT+RE_BANNER_HEIGHT;
    UIScrollView* scrollView = [[UIScrollView alloc] initWithFrame:CGRectMake(0, y_scrollBtn, _MainScreen_Width, self.mainContentView.frame.size.height-y_scrollBtn-HEIGHT_BOTTOM)];
    
    self.btnView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, ABSOLUTE2RELATIVE(ABSOLUTE_HEIGHT_BTNVIEW))];
    
    CGFloat start_x = (_MainScreen_Width - RELATIVE_WH_IMAGE*3-ABSOLUTE2RELATIVE(ABSOLUTE_WIDTH_IMAGE_SPACE)*4-2)/2.0f;
    
    CGFloat start_y = ABSOLUTE2RELATIVE(ABSOLUTE_HEIGHT_BTNVIEW_TB);
    CGFloat single_height = RELATIVE_H_TXT+RELATIVE_WH_IMAGE+ABSOLUTE2RELATIVE(ABSOLUTE_HEIGHT_TXT_BT)*2;
    for(int i=0;i<NUMBER_BTN;i++)//NUMBER_BTN
    {
        CGFloat offset_x = start_x+i%3*(1+RELATIVE_WH_IMAGE+ABSOLUTE2RELATIVE(ABSOLUTE_WIDTH_IMAGE_SPACE*2));
        CGFloat offset_y = start_y;
        if(i>=3)
            offset_y += ABSOLUTE2RELATIVE(ABSOLUTE_HEIGHT_BTNVIEW_TB) + single_height;
        if(i>=6)
            offset_y += ABSOLUTE2RELATIVE(ABSOLUTE_HEIGHT_BTNVIEW_TB)+single_height;
        [self createSingleBtn:i x:offset_x y:offset_y img:RES_BTN_IMAGE[i] text:RES_BTN_TEXT[i]];
    }
    
    //分割线添加
    CGFloat h_vertical_line = ABSOLUTE2RELATIVE(ABSOLUTE_HEIGHT_BTNVIEW)*0.9f;
    CGFloat w_horizontal_line = _MainScreen_Width*0.9f;
    CGFloat x_line = _MainScreen_Width*0.1/2.0f;
    CGFloat y_line = ABSOLUTE2RELATIVE(ABSOLUTE_HEIGHT_BTNVIEW)*0.1f/2.0f;
    //竖线分割线
    UIView* ui_v_spliter1 = [[UIView alloc] initWithFrame:CGRectMake(start_x+RELATIVE_WH_IMAGE+ABSOLUTE2RELATIVE(ABSOLUTE_WIDTH_IMAGE_SPACE), y_line, 1, h_vertical_line)];
    [ui_v_spliter1 setBackgroundColor:UIColorFromRGB(COLOR_SPLITER)];
    [self.btnView addSubview:ui_v_spliter1];
    UIView* ui_v_spliter2 = [[UIView alloc] initWithFrame:CGRectMake(start_x+RELATIVE_WH_IMAGE*2+ABSOLUTE2RELATIVE(ABSOLUTE_WIDTH_IMAGE_SPACE*3)+1, y_line, 1, h_vertical_line)];
    [ui_v_spliter2 setBackgroundColor:UIColorFromRGB(COLOR_SPLITER)];
    [self.btnView addSubview:ui_v_spliter2];
    //横线分割线
    UIView* ui_h_spliter1 = [[UIView alloc] initWithFrame:CGRectMake(x_line, start_y+RELATIVE_WH_IMAGE+ABSOLUTE2RELATIVE(ABSOLUTE_HEIGHT_TXT_BT*2)+RELATIVE_H_TXT, w_horizontal_line, 1)];
    [ui_h_spliter1 setBackgroundColor:UIColorFromRGB(COLOR_SPLITER)];
    [self.btnView addSubview:ui_h_spliter1];
    UIView* ui_h_spliter2 = [[UIView alloc] initWithFrame:CGRectMake(x_line, start_y+RELATIVE_WH_IMAGE*2+ABSOLUTE2RELATIVE(ABSOLUTE_HEIGHT_IMAGE_TOPMARGIN+ABSOLUTE_HEIGHT_TXT_BT*4)+RELATIVE_H_TXT*2, w_horizontal_line, 1)];
    [ui_h_spliter2 setBackgroundColor:UIColorFromRGB(COLOR_SPLITER)];
    [self.btnView addSubview:ui_h_spliter2];
    [scrollView setContentSize:self.btnView.bounds.size];
    
    [scrollView addSubview:self.btnView];
    [self.mainContentView addSubview:scrollView];
}

#define INIT_REDTIP(view_,x_,y_) \
view_ = [[NIBadgeView alloc] initWithFrame:CGRectMake(x_+25, y_-10, 0, 0)]; \
view_.text = @"0"; \
view_.tintColor = [UIColor redColor]; \
view_.textColor = [UIColor whiteColor]; \
[view_ sizeToFit]; \
view_.backgroundColor = [UIColor clearColor];

-(void)createSingleBtn:(int)index x:(CGFloat)x y:(CGFloat)y img:(const char*)img text:(const char*)text
{
    UIButton* ui_btn = [UIButton buttonWithType:UIButtonTypeCustom];
    UILabel* ui_lab = [[UILabel alloc] init];
    
    //设置按钮属性
    [ui_btn setTag:index];
    CGFloat width = RELATIVE_WH_IMAGE;
    //后面的高度是增加点击区域扩展到字的位置
    CGFloat height = RELATIVE_WH_IMAGE+ABSOLUTE2RELATIVE(ABSOLUTE_HEIGHT_TXT_BT)+RELATIVE_H_TXT;
    [ui_btn setFrame:CGRectMake(x, y, width, height)];
    UIImage* ui_img = [UIImage imageNamed:[NSString stringWithUTF8String:img]];
    [ui_btn setImage:ui_img forState:UIControlStateNormal];//设置正常状态的图片
    [ui_btn setContentVerticalAlignment:UIControlContentVerticalAlignmentTop];//置顶
    [ui_btn addTarget:self action:@selector(buttonClick:) forControlEvents:UIControlEventTouchUpInside];
    
    //设置文字属性
    CGFloat label_y = y+RELATIVE_WH_IMAGE+ABSOLUTE2RELATIVE(ABSOLUTE_HEIGHT_TXT_BT);
    width = RELATIVE_WH_IMAGE;
    height = RELATIVE_H_TXT;
    [ui_lab setFrame:CGRectMake(x, label_y, width, height)];
    [ui_lab setFont:[UIFont systemFontOfSize:11]];
    [ui_lab setTextColor:UIColorFromRGB(COLOR_TEXT)];
    [ui_lab setText:[NSString stringWithUTF8String:text]];
    [ui_lab setTextAlignment:NSTextAlignmentCenter];
    
    [self.btnView addSubview:ui_lab];
    [self.btnView addSubview:ui_btn];
    
    if(index == 0)//弹幕提醒
    {
        INIT_REDTIP(self.mBarrageTip,x,y);
        self.mChatUnreadNumber = 0;
        [self.btnView addSubview:self.mBarrageTip];
    }
    else if(index == 1)//红包提醒
    {
        INIT_REDTIP(self.mHongBaoTip,x,y);
        self.mHBUnreadNumber = 0;
        [self.btnView addSubview:self.mHongBaoTip];
    }
}


#pragma mark -
#pragma mark - SwitchMenu
-(void)buttonClick:(id)sender
{
    UIButton *btn = (UIButton *)sender;
    
    switch(btn.tag)
    {
        case 0://弹幕互动
        {
            InteractController *interact = [[InteractController alloc] init];
            [self.navigationController pushViewController:interact animated:YES];
            self.mChatUnreadNumber = 0;
            self.mIsInChatUI = YES;
            break;
        }
        case 1://领取红包
        {
            HongBGetViewController* controller = [[HongBGetViewController alloc] init];
            [self.navigationController pushViewController:controller animated:YES];
            self.mHBUnreadNumber = 0;
            break;
        }
        case 2://包厢预定
        {
            BoxPreBookViewController *box = [[BoxPreBookViewController alloc] init];
            [self.navigationController pushViewController:box animated:YES];
            break;
        }
        case 3://歌手
        {
            TopicListViewController * topicList = [[TopicListViewController alloc] initModelId:kPKtvAssistantAPIModelTypeArtist title:@"歌手分类"];
            [self.navigationController pushViewController:topicList animated:YES];
            break;
        }
        case 4://主题
        {
            TopicListViewController * topicList = [[TopicListViewController alloc] initModelId:kPKtvAssistantAPIModelTypeTheme title:@"主题"];
            [self.navigationController pushViewController:topicList animated:YES];
            break;
        }
        case 5://新歌
        {
            SongsBookViewController * viewController = [[SongsBookViewController alloc] initInfoNoSinger:kPKtvAssistantAPIModelTypeNewSong topicId:0 title:@"新歌"];
            [self.navigationController pushViewController:viewController animated:YES];
            break;
        }
        case 6://排行榜
        {
            RankingsViewController* viewController = [[RankingsViewController alloc] initWithModelId:kPKtvAssistantAPIModelTypeRank title:@"排行榜"];
            [self.navigationController pushViewController:viewController animated:YES];
            break;
        }
        case 7://语种
        {
            TopicListViewController * viewController = [[TopicListViewController alloc] initModelId:kPKtvAssistantAPIModelTypeLanguageType title:@"语种"];
            [self.navigationController pushViewController:viewController animated:YES];
            break;
        }
        case 8://扫码
        {
            self.mScanType = SCAN_COMMON;
            [self openAction];
            break;
        }
    }
}

#pragma mark On Main Thread
-(void)onChangeChatTip
{
    if(self.mChatUnreadNumber == 0)
        [self.mBarrageTip setHidden:YES];
    else
    {
        NSString* tip;
        if(self.mChatUnreadNumber > 99)
            tip = @"99+";
        else
            tip = [NSString stringWithFormat:@"%d",self.mChatUnreadNumber];
        [self.mBarrageTip setText:tip];
        [self.mBarrageTip setHidden:NO];
    }
}

-(void)onChangeHBTip
{
    if(self.mHBUnreadNumber == 0)
        [self.mHongBaoTip setHidden:YES];
    else
    {
        NSString* tip;
        if(self.mHBUnreadNumber > 99)
            tip = @"99+";
        else
            tip = [NSString stringWithFormat:@"%d",self.mHBUnreadNumber];
        [self.mHongBaoTip setText:tip];
        [self.mHongBaoTip setHidden:NO];
    }
}

#pragma mark - NetUtilCallBackProtocol
-(void)onRecvHongBNumber:(int)number
{
    self.mHBUnreadNumber = number;
    [self performSelectorOnMainThread:@selector(onChangeHBTip) withObject:nil waitUntilDone:NO];
}
-(void)onHongBUnReadAdd
{
    self.mHBUnreadNumber += 1;
    [self performSelectorOnMainThread:@selector(onChangeHBTip) withObject:nil waitUntilDone:NO];
}
-(void)onChatUnReadAdd
{
    if(self.mIsInChatUI)
        return ;
    self.mChatUnreadNumber += 1;
    [self performSelectorOnMainThread:@selector(onChangeChatTip) withObject:nil waitUntilDone:NO];
}
- (void)delegateRedNum
{
    if(self.mIsInChatUI)
        return ;
    self.mChatUnreadNumber += 1;
    [self performSelectorOnMainThread:@selector(onChangeChatTip) withObject:nil waitUntilDone:NO];
}

#pragma mark - viewDidLoad
- (void)viewDidLoad
{
    [super viewDidLoad];
    self.mScanType = SCAN_COMMON;
    self.mIsRemoveOnDestroy = YES;
    
    [NetUtilCallBack getInstance].thedelegate = self;
    self.view.backgroundColor = UIColorFromRGB(0xebecf1);
    
    self.mainContentView.backgroundColor = [UIColor whiteColor];
    
    _spView = [[UIView alloc] initWithFrame:CGRectMake(0, RE_SEARCH_HEIGHT, _MainScreen_Width, RE_BANNER_HEIGHT)];
    _spView.backgroundColor = [UIColor whiteColor];
    [self.mainContentView addSubview:_spView];
    
    self.navView.titleLabel.text = @"KTV手机助手";
    self.navView.hidden = NO;
    self.navView.leftButton.hidden = NO;

    [self creatSongSearchBar]; // 搜索歌曲
    [self creatScrollView]; // 滚动大图
    
    self.toolBar.hidden = NO;
    self.joinButton.hidden = NO;
    // 进入包厢
    [self.joinButton addTarget:self action:@selector(goInRoom:) forControlEvents:UIControlEventTouchUpInside];
    
    // 退出包厢
    [self.power addTarget:self action:@selector(_exitRoom:) forControlEvents:UIControlEventTouchUpInside];
    
    [self createButtonView];
    [self flashUIWithRoom];
}

-(void)ExpressionClick:(NSInteger)tag
{
    PLog(@"%d",tag);
}

#pragma mark - 退出包厢

- (void)_exitRoom:(UIButton *)button
{
    _altExitRoom = [[UIAlertView alloc] initWithTitle:@"温馨提示" message:@"你确定要退出包厢吗?" delegate:self cancelButtonTitle:@"点错了" otherButtonTitles:@"好", nil];
    [_altExitRoom show];
}

- (void)delegateLeaveRoom
{
    [UserSessionManager clearCurrentRoomInfo];
    [self flashUIWithRoom]; // 退出包厢刷新room
    [self getAlreadyOrderedSongsNumber];
    
    [self setRightAndBadgeButton];
    
    self.alreadyLoad = NO;
    
    UIAlertView *alert =[[UIAlertView alloc] initWithTitle:@""
                                                   message:@"包厢关闭或包厢登录被门店用户注销"
                                                  delegate:self
                                         cancelButtonTitle:@"确定"
                                         otherButtonTitles:nil];
    [alert show];

}

#pragma mark - UISearchBar Delegate

-(void) searchBarSearchButtonClicked:(UISearchBar *)theSearchBar
{
    keyword_ = [theSearchBar text];
    if (keyword_.length !=0)
    {
        [self doSearch];
    }
    [theSearchBar resignFirstResponder];
}

- (void)searchBarTextDidBeginEditing:(UISearchBar *)theSearchBar
{
    [theSearchBar setShowsCancelButton:YES animated:YES];
    self.songSearchBar.placeholder = @"请输入歌手名或歌曲名";
    self.view_loading.hidden = NO;
    [self.mainContentView bringSubviewToFront:self.view_loading];
    
    self.songSearchBar.showsCancelButton = YES;
}

- (void)searchBarTextDidEndEditing:(UISearchBar *)theSearchBar
{
    [theSearchBar setShowsCancelButton:NO animated:YES];
    [self.mainContentView sendSubviewToBack:self.view_loading];
    self.view_loading.hidden = YES;
}

- (BOOL)searchBar:(UISearchBar *)theSearchBar shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text{
    if (range.location != NSNotFound)
    {
        keyword_ = [theSearchBar.text stringByReplacingCharactersInRange:range withString:text];
        keyword_ = [keyword_ stringByReplacingOccurrencesOfString:@" " withString:@""];
    }
    if (keyword_.length != 0 && ![self isChinese:keyword_])
    {
    }
    else
    {
        self.view_loading.hidden = NO;
        [self.mainContentView bringSubviewToFront:self.view_loading];
    }
    return YES;
}
//处理 点击右边小叉
- (void)searchBar:(UISearchBar *)theSearchBar textDidChange:(NSString *)searchText{
    
    if([searchText isEqualToString:@""])
    {
        self.songSearchBar.placeholder = @"请输入歌手名或歌曲名";
    }
    else
    {
    }
}
- (void)searchBarCancelButtonClicked:(UISearchBar *) theSearchBar
{
    _songSearchBar.text = @"";
    [self dismissKeyboard];
}

//数字0--9的ASCII码 48--57
//中文ASCII码 0x4e00--0x9fff
//ASCⅡ码在97和122之间（包括97和122,为小写英文字母a——z)；
//ASCⅡ码在65和90之间（包括65和90，为大写英文字母A——Z）。
-(BOOL)isChinese:(NSString *) searchKey
{
    int len = searchKey.length;
    if (searchKey.length > 0)
    {
        for(int i=0; i< len;i++)
        {
            int a = [searchKey characterAtIndex:i];
            if(!(a >= 65 && a <= 90) && !(a >= 97 && a <= 122))
            {        //非英文字母
                return YES;
            }
        }
    }
    return NO;
}

-(void)dismissKeyboard
{
    [self.songSearchBar resignFirstResponder];
}

-(void)doSearch
{
    //进入搜索页面，通过当前关键字获取搜索数据
    SearchResultViewController * searchResult = [[SearchResultViewController alloc]init];
    searchResult.searchKey = keyword_;
    searchResult.modelID = kPKtvAssistantAPIModelType0;
    
    [self dismissKeyboard];
    [self.navigationController pushViewController:searchResult animated:YES];
}

#pragma mark - UIActionSheetDelegate

//- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
//{
//    if (actionSheet == _boxVodActionSheet)
//    {
//        if (buttonIndex == 0) // 二维码扫描进包厢
//        {
//            [self openAction]; // 开启二维码扫描
//        }
//        else if(buttonIndex ==1) // 输入KTV包厢密码界面
//        {
//            KTVInputSecretController *input = [[KTVInputSecretController alloc] init];
//            [self.navigationController pushViewController:input animated:YES];
//        }
//    }
//    [actionSheet dismissWithClickedButtonIndex:0 animated:YES];
//}

#pragma mark - 开启二维码扫描

- (void)openAction
{
    _reader = [ZBarReaderViewController new];
    //设备尺寸 3.5/4.0英寸
    _reader.view.frame = CGRectMake(0, 0, 320, 480+(iPhone5?88:0));
    _reader.readerDelegate = self;
    [_reader setShowsZBarControls:NO]; // 禁止 官方的取消和帮助说明按钮
    _reader.supportedOrientationsMask = ZBarOrientationMaskAll;
    _reader.cameraFlashMode = UIImagePickerControllerCameraFlashModeOff;
    ZBarImageScanner *scanner = _reader.scanner;
    [scanner setSymbology: ZBAR_I25 config: ZBAR_CFG_ENABLE to: 0];
    
    // 取消二维码扫描
    UIButton *cancel = [UIButton buttonWithType:UIButtonTypeCustom];
    cancel.frame = CGRectMake(20, _reader.view.frame.size.height-60, 60, 60);
    cancel.showsTouchWhenHighlighted = YES;
    cancel.backgroundColor = [UIColor clearColor];
    [cancel setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
    [cancel setTitle:@"取消" forState:UIControlStateNormal];
    [cancel addTarget:self action:@selector(cancelAction:) forControlEvents:UIControlEventTouchUpInside];
    [_reader.view addSubview:cancel];
    
    // 是否开启闪关灯照明
    UIButton *flash = [UIButton buttonWithType:UIButtonTypeCustom];
    flash.frame = CGRectMake(_MainScreen_Width-60-20, _reader.view.frame.size.height-60, 60, 60);
    flash.showsTouchWhenHighlighted = YES;
    flash.backgroundColor = [UIColor clearColor];
    flash.tag = 0;
    [flash setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
    [flash setTitle:@"照明" forState:UIControlStateNormal];
    [flash addTarget:self action:@selector(flashAction:) forControlEvents:UIControlEventTouchUpInside];
    [_reader.view addSubview:flash];
    
    [self presentViewController:_reader animated:YES completion:nil];
}

#pragma mark -  取消二维码扫描

- (void)cancelAction:(UIButton *)button
{
    //取消扫码重置为COMMON
    self.mScanType = SCAN_COMMON;
    [_reader dismissViewControllerAnimated:YES completion:nil];
}

#pragma mark -  是否开启闪关灯照明

- (void)flashAction:(UIButton *)button
{
    if (button.tag == 0)
    {
        [button setTag:1];
        _reader.cameraFlashMode = UIImagePickerControllerCameraFlashModeOn; // 后置灯光模式开
    }
    else
    {
        [button setTag:0];
        _reader.cameraFlashMode = UIImagePickerControllerCameraFlashModeOff; // 后置灯光模式关
    }
}

#pragma mark - UIImagePickerController delegate
- (void) imagePickerController: (UIImagePickerController*) reader
 didFinishPickingMediaWithInfo: (NSDictionary*) info
{
    id<NSFastEnumeration> results = [info objectForKey: ZBarReaderControllerResults];
    ZBarSymbol *symbol = nil;
    for(symbol in results)
        break;
    NSString *resultText = symbol.data;
    //解决扫描的二维码是汉字生成时会乱码的问题
    if ([resultText canBeConvertedToEncoding:NSShiftJISStringEncoding])
    {
        resultText = [NSString stringWithCString:[resultText cStringUsingEncoding:NSShiftJISStringEncoding] encoding:NSUTF8StringEncoding];
    }
    PLog(@"resultText: %@",resultText);
    //[reader dismissModalViewControllerAnimated: YES];
    [reader dismissViewControllerAnimated:YES completion:nil];
    
    switch (self.mScanType) {
        case SCAN_ROOMPWD:
        {
            if (![resultText isMatchedByRegex:@"^[a-zA-Z0-9]+$"] || [resultText length] != 6)
            {
                PLog(@"字符串不符合标准！！！");
                [[iToast makeText:@"该密码密码不符合密码标准，请重试"] show];
                return;
            }
            else
            {
                self.keyWord = resultText;
                [self login];
            }
            break;
        }
        case SCAN_GIFT:
        {
            self.mMapData = [[NSMutableDictionary alloc] init];
            NSArray* strKeyValues = [resultText componentsSeparatedByString:@";"];
            for(NSString* str in strKeyValues)
            {
                NSArray* strKeyValue = [str componentsSeparatedByString:@":"];
                if(strKeyValue == nil || strKeyValue.count != 2)
                {
                    [[iToast makeText:@"扫描的二维码不正确"] show];
                    return ;
                }
                [self.mMapData setObject:[strKeyValue objectAtIndex:1] forKey:[strKeyValue objectAtIndex:0]];
            }
            
            if(self.mMapData.count == 0)
            {
                [[iToast makeText:@"扫描的二维码不正确"] show];
                return ;
            }
            
            NSString* payType = [self.mMapData objectForKey:@"scanpay-type"];
            if([payType compare:@"1"] != NSOrderedSame)
            {
                [[iToast makeText:@"请选择兑换礼物的二维码"] show];
                return ;
            }
            
            self.mAlertExchange  = [[UIAlertView alloc] initWithTitle:nil message:[[NSString alloc] initWithFormat:@"确认要兑换%@%@%@(%@金币)?",[self.mMapData objectForKey:@"giftnum"],[self.mMapData objectForKey:@"giftunit"],[self.mMapData objectForKey:@"giftname"],[self.mMapData objectForKey:@"gold"]] delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
            [self.mAlertExchange show];
            
            break;
        }
        case SCAN_COMMON:
        default:
        {
            //尝试判断一下是否是我们的扫码兑换礼物二维码。
            do
            {
                self.mMapData = [[NSMutableDictionary alloc] init];
                NSArray* strKeyValues = [resultText componentsSeparatedByString:@";"];
                for(NSString* str in strKeyValues)
                {
                    NSArray* strKeyValue = [str componentsSeparatedByString:@":"];
                    if(strKeyValue == nil || strKeyValue.count != 2)
                    {
                        break ;
                    }
                    [self.mMapData setObject:[strKeyValue objectAtIndex:1] forKey:[strKeyValue objectAtIndex:0]];
                }
                
                if(self.mMapData.count == 0)
                {
                    break ;
                }
                
                NSString* payType = [self.mMapData objectForKey:@"scanpay-type"];
                if(payType == nil || [payType compare:@"1"] != NSOrderedSame)
                {
                    break ;
                }
                
                self.mAlertExchange  = [[UIAlertView alloc] initWithTitle:nil message:[[NSString alloc] initWithFormat:@"确认要兑换%@%@%@(%@金币)?",[self.mMapData objectForKey:@"giftnum"],[self.mMapData objectForKey:@"giftunit"],[self.mMapData objectForKey:@"giftname"],[self.mMapData objectForKey:@"gold"]] delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
                [self.mAlertExchange show];
                return ;
                
            }while (false);
            
            //如果到了这里，说明不是我们的兑换礼物二维码，那么我们只是提示一下。
            self.mAlertExchange  = [[UIAlertView alloc] initWithTitle:nil message:[[NSString alloc] initWithFormat:@"扫码结果:%@",resultText] delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil];
            [self.mAlertExchange show];
            break;
        }
    }
    
    self.mScanType = SCAN_COMMON;
}

#pragma mark - 扫描二维码登录进入包厢

-(void)login
{
    if(![[Reachability reachabilityForInternetConnection] isReachable])
    {
        [self showNetwork];
        return;
    }
    [SVProgressHUD showWithStatus:@"进入房间中,请稍后" maskType:SVProgressHUDMaskTypeClear];
    
    User * user = [UserSessionManager getInstance].currentUser;
    long int userID = user.uid;
    NSString *encodeUrl =  [PKtvAssistantAPI getRoomLoginUrlEx:userID pwd:user.pwd password:self.keyWord];
    NSURL *url = [NSURL URLWithString:encodeUrl];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject)
    {
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame)
        {
            NSDictionary *result = [results objectForKey:@"result"];
            NSInteger gold = [[result objectForKey:@"gold"] integerValue];
            if ([UserSessionManager getInstance].currentUser.gold != gold && gold > 0) {
                [[UserSessionManager getInstance] updateCurrentUserGold:gold];
            }
            RoomInfo *roominfo = [RoomInfo initWithDictionary:result];
            roominfo.roomPassword = _keyWord;
            [roominfo Log];
            [[UIApplication sharedApplication] cancelAllLocalNotifications]; // 撤销所有的Notification
            UILocalNotification *localNotification = [[UILocalNotification alloc] init];
            // 设置notification的属性
            localNotification.fireDate = [[NSDate new] dateByAddingTimeInterval:roominfo.periodsTime]; //出发时间
            localNotification.alertBody = @"您的包厢已到期，请续费或欢迎下次光临"; // 消息内容
            localNotification.soundName = UILocalNotificationDefaultSoundName; // 触发消息时播放的声音
            //设置随Notification传递的参数
            NSDictionary *infoDict = [NSDictionary dictionaryWithObjectsAndKeys:@"reminder", @"notificationId", self.keyWord, @"roomSecret", nil];
            localNotification.userInfo = infoDict;
            [[UIApplication sharedApplication] scheduleLocalNotification:localNotification];
            [UserSessionManager getInstance].currentRoomInfo = roominfo;
            [UserSessionManager getInstance].isLoggedIn = YES;
            //NSString * tag = [roominfo.roomPassword lowercaseString];
            //PLog(@"TAG-->>%@",tag);
            //[BPush setTag:tag];
            [SVProgressHUD dismiss];
            
            [self loadAlreadyOrderedSongs];
            
            [self setRightAndBadgeButton];
            
            NetUtilCallBack* pNetUtil = [NetUtilCallBack getInstance];
            [pNetUtil setRoomId:[roominfo.xgRoomID intValue]];
            [pNetUtil doCurrentWork:TransactionRoomLogin listener:^(BOOL success) {
                if(success)
                {
                    [self flashUIWithRoom]; // 进入包厢或者退出包厢刷新界面
                }
                else
                {
                    [SVProgressHUD dismiss];
                    NSString *msg = [results objectForKey:@"msg"];
                    [CommenTools showAlertViewWithTitle:msg];
                }
            }];
        }
        else
        {
            [SVProgressHUD dismiss];
            NSString *msg = [results objectForKey:@"msg"];
            [CommenTools showAlertViewWithTitle:msg];
            //[CommenTools showAlertViewWithTitle:kPKtvAssistantErrorCodeForLogin];
        }
    } failure:^(AFHTTPRequestOperation *operation, NSError *error)
     {
         [SVProgressHUD dismiss];
         PLog(@"doLogin failed...");
         [CommenTools showNetworkDisconnectedAlertView];
     }];
    [operation start];
}

#pragma mark - 进入包厢或者退出包厢刷新界面

- (void)flashUIWithRoom
{
    if ([UserSessionManager isRoomAlreadyLogin])   // 已进入包厢
    {
        NSString *roomName = [UserSessionManager getInstance].currentRoomInfo.roomName;
        self.inRoomNumber.text = [NSString stringWithFormat:@"所在包厢: %@",roomName];
        self.joinButton.hidden = YES;
        self.power.hidden = NO;
        self.inRoomNumber.hidden = NO;
    }
    else  // 未进入包厢
    {
        self.joinButton.hidden = NO;
        self.power.hidden = YES;
        self.inRoomNumber.hidden = YES;
        _songSearchBar.text = @"";
        _songSearchBar.placeholder = @"请输入歌手名或歌曲名";
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark NetUtilCallBackDelegate

@end
