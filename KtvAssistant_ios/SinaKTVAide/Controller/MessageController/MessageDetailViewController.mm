//
//  MessageDetailViewController.m
//  SinaKTVAide

// 消息中心详情界面

//  Created by Li Pan on 13-12-13.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "MessageDetailViewController.h"
#import "DDMenuController.h"
#import "KTVAppDelegate.h"
#import "PKtvAssistantAPI.h"
#import "Message.h"
#import "TopSong.h"

@interface MessageDetailViewController ()

@end

@implementation MessageDetailViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
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

#pragma mark - viewDidLoad

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navView.backButton.hidden = NO;
    self.navView.hidden = NO;
    self.navView.titleLabel.text = @"歌曲上榜";
    self.view.backgroundColor = UIColorFromRGB(0xe1e2e6);
    
    [self getDetailMessageData]; // 抢榜歌曲,演唱,得分,详细信息
    [self creatRushImageView]; // 创建抢榜视图框
    
    UILabel *labNotice = [[UILabel alloc] initWithFrame:CGRectMake(15, 15, _MainScreen_Width-30, 25*2)];
    labNotice.text = _content;
    labNotice.backgroundColor = [UIColor clearColor];
    labNotice.textColor = UIColorFromRGB(0x484848);
    labNotice.font =[UIFont systemFontOfSize:13.0f];
    labNotice.numberOfLines = 0;
    [self.mainContentView addSubview:labNotice];
    
    UIImageView *bgImage = [[UIImageView alloc] initWithFrame:CGRectMake(28, 80, 265, 172)];
    [bgImage setImage:[UIImage imageNamed:@"xitongxiaoxi_kuang.png"]];
    bgImage.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview:bgImage];
    
    UILabel *labSongName = [[UILabel alloc] initWithFrame:CGRectMake(55, 95, _MainScreen_Width-110, 35)];
    labSongName.text = @"歌曲:";
    labSongName.textColor = UIColorFromRGB(0x2b2b2b);
    labSongName.font =[UIFont systemFontOfSize:15.0f];
    labSongName.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview:labSongName];
    
    UILabel *labSing = [[UILabel alloc] initWithFrame:CGRectMake(55, 125, _MainScreen_Width-110, 35)];
    labSing.text = @"演唱:";
    labSing.textColor = UIColorFromRGB(0x2b2b2b);
    labSing.font =[UIFont systemFontOfSize:15.0f];
    labSing.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview:labSing];
    
    UILabel *labScore = [[UILabel alloc] initWithFrame:CGRectMake(55, 155, _MainScreen_Width-110, 35)];
    labScore.text = @"得分:";
    labScore.textColor = UIColorFromRGB(0x2b2b2b);
    labScore.font =[UIFont systemFontOfSize:15.0f];
    labScore.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview:labScore];
    
    
    /********初始化*********抢榜歌曲,演唱,得分,详细信息****************************/
    
    _SongTitle = [[UILabel alloc] initWithFrame:CGRectMake(55+40, 95, _MainScreen_Width-110, 35)];
    _SongTitle.textColor = UIColorFromRGB(0x2b2b2b);
    _SongTitle.font =[UIFont systemFontOfSize:15.0f];
    _SongTitle.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview:_SongTitle];
    
    _SongSinger = [[UILabel alloc] initWithFrame:CGRectMake(55+40, 125, _MainScreen_Width-110, 35)];
    _SongSinger.textColor = UIColorFromRGB(0x2b2b2b);
    _SongSinger.font =[UIFont systemFontOfSize:15.0f];
    _SongSinger.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview:_SongSinger];
    
    _SongScore = [[UILabel alloc] initWithFrame:CGRectMake(55+40, 155, _MainScreen_Width-110, 35)];
    _SongScore.textColor = UIColorFromRGB(0xf10f4a);
    _SongScore.font =[UIFont systemFontOfSize:15.0f];
    _SongScore.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview:_SongScore];
    
    _rushBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    _rushBtn.frame = CGRectMake(55, 155+40, _MainScreen_Width-110, 35);
    _rushBtn.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview:_rushBtn];  // 弹出抢榜视图框
}

#pragma mark - 抢榜歌曲,演唱,得分,详细信息

- (void)getDetailMessageData
{
    long int userID = [UserSessionManager getInstance].currentUser.uid;
    NSString *roomId = [UserSessionManager getInstance].currentRoomInfo.roomId;

    if (_msgType == 2)  // 2：歌曲上榜
    {
        NSString *encodeUrl = [PKtvAssistantAPI getGetMessageDetailUrl:userID roomId:roomId msgID:_msgID msgType:_msgType workID:_workID point:_point date:_date];
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
                 NSDictionary *msgData = [result objectForKey:@"msgdetail"];
                 //Message *msg = [Message initWithDictionary:msgData];
                 //[msg log];
                 
                 if([msgData objectForKey:@"work"])
                 {
                     NSDictionary *workData = [msgData objectForKey:@"work"];
                     _topSong = [TopSong initWithDictionary:workData];
                     [_topSong log];
                     
                     [self rushPersonInfo]; // 上榜的个人信息
                     
                     //PLog(@"%@",_topSong.user);
                     if (_topSong.user != nil)
                     {
                         [self disEnableChick:nil];  // 如果被人抢了,抢榜按钮置灰
                     }
                     else
                     {
                         NSString *msg = [results objectForKey:@"msg"];
                         NSString *errorCode = [results objectForKey:@"errorcode"];
                         if([errorCode isEqualToString:@"80003"]){
                             //80003 榜单过期
                             //该情况特殊处理，add by shoupeng
                             [self disEnableChick:msg];
                         }
                         else if([errorCode isEqualToString:@"80002"]){
                             //80002 榜单被超越
                             //该情况特殊处理，add by shoupeng
                             [self disEnableChick:msg];
                         }
                         else if([errorCode isEqualToString:@"80001"]){
                             //80002 榜单被抢
                             //该情况特殊处理，add by shoupeng
                             [self disEnableChick:msg];
                         }
                         else{
                             [self enbleChick]; // 如果还未被人抢,抢榜按钮可点
                         }
                         
                     }
                 }
             }
             else
             {
                 NSString *msg = [results objectForKey:@"msg"];
                 NSString *errorCode = [results objectForKey:@"errorcode"];
                 [CommenTools showAlertViewWithErrorMessage:msg];
                 PLog(@"doGetMessageDetail failed...errorcode:%@(%@)", errorCode, msg);
             }
        }
        failure:^(AFHTTPRequestOperation *operation, NSError *error)
        {
            PLog(@"-- failed...");
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        [operation start];
    }
}

#pragma mark - 上榜的个人信息

- (void)rushPersonInfo
{
    _SongTitle.text = _topSong.song.songName; // 歌曲名
    
    _SongSinger.text = _topSong.song.songArtist;  // 歌手
    
    float score = (float)_topSong.score/10;
    _SongScore.text = [NSString stringWithFormat:@"%.1f",score]; // 得分
}

#pragma mark - 创建抢榜视图框

- (void)creatRushImageView
{
    // 抢榜阴影
    _bgView = [[UIView alloc] initWithFrame:self.view.bounds];
    [self.view addSubview:_bgView];
    _bgView.backgroundColor = [UIColor colorWithWhite:0.0 alpha:0.7];
    [_bgView setHidden:YES];
    
    // 抢榜背景图
    _rushImage = [[UIImageView alloc] init];
    _rushImage.backgroundColor = [UIColor clearColor];
    _rushImage.frame = CGRectMake(25, 150, 270, 194);
    _rushImage.userInteractionEnabled = YES;
    [self.view addSubview:_rushImage];
    [_rushImage setHidden:YES];
    
    // 关闭抢榜按钮
    UIButton *close = [UIButton buttonWithType:UIButtonTypeCustom];
    close.backgroundColor = [UIColor clearColor];
    close.showsTouchWhenHighlighted = YES;
    close.frame = CGRectMake(240, -12, 35, 35);
    [close setImage:[UIImage imageNamed:@"btn_x.png"] forState:UIControlStateNormal];
    [close setImage:[UIImage imageNamed:@"btn_x_c.png"] forState:UIControlStateHighlighted];
    [close addTarget:self action:@selector(closeRushBgView:) forControlEvents:UIControlEventTouchUpInside];
    [_rushImage addSubview:close];

    // 抢榜标题
    _rushLabel = [[UILabel alloc] initWithFrame:CGRectMake(102, 53, 100, 35)];
    _rushLabel.text = @"";
    _rushLabel.backgroundColor = [UIColor clearColor];
    _rushLabel.font =[UIFont systemFontOfSize:16.0f];
    [_rushImage addSubview:_rushLabel];

    // 抢榜提示
    _rushNoticeLabel = [[UILabel alloc] initWithFrame:CGRectMake(48, 100, 180, 60)];
    _rushNoticeLabel.text = @"";
    _rushNoticeLabel.backgroundColor = [UIColor clearColor];
    _rushNoticeLabel.numberOfLines = 0;
    _rushNoticeLabel.textColor = UIColorFromRGB(0x6c6c6c);
    _rushNoticeLabel.font =[UIFont systemFontOfSize:13.0f];
    [_rushImage addSubview:_rushNoticeLabel];
}

#pragma mark - 抢榜弹出视图事件

- (void)rushAction:(UIButton *)button
{
    if ([UserSessionManager isUserAlreadyLogin])  // 登录账号才能抢榜
    {
        long int userID = [UserSessionManager getInstance].currentUser.uid;
        NSString *roomId = [UserSessionManager getInstance].currentRoomInfo.roomId;
        NSString *encodeUrl = [PKtvAssistantAPI getReceiveRankSongUrl:userID roomId:roomId workID:self.workID score:self.point date:self.date];
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
                 BOOL res = [[result objectForKey:@"success"] boolValue];
                 if(res)
                 {
                     PLog(@"抢榜成功");
                     [self openRushBgView:YES errorMsg:nil errorcode:nil];
                 }
             }
             else
             {
                 PLog(@"抢榜失败");
                 NSString *msg = [results objectForKey:@"msg"];
                 NSString *errorCode = [results objectForKey:@"errorcode"];
                 [self openRushBgView:NO errorMsg:msg errorcode:errorCode];
                 PLog(@"doReceiveRankSong failed...errorcode:%@(%@)", errorCode, msg);
             }
        }
        failure:^(AFHTTPRequestOperation *operation, NSError *error)
         {
             PLog(@"---failed...");
             [CommenTools showNetworkDisconnectedAlertView];
         }];
        
        [operation start];
    }
}

#pragma mark - 显示抢榜成功或者失败信息

- (void)openRushBgView:(BOOL)rush errorMsg:(NSString *)msg errorcode:(NSString *)code
{
    [self openRushAnimation];  // 抢榜展示动画
    
    if (rush == YES) // 抢榜成功
    {
        _rushLabel.text = @"抢榜成功!";
        _rushLabel.textColor = UIColorFromRGB(0xff1371);
        _rushNoticeLabel.text = @"快去“我的账号”领取积分吧!";
        _rushImage.image = [UIImage imageNamed:@"Popup_success.png"];
        
    }
    else // 抢榜失败
    {
        _rushLabel.text = @"抢榜失败!";
        _rushLabel.textColor = UIColorFromRGB(0xffffff);
        _rushImage.image = [UIImage imageNamed:@"Popup_failure.png"];
        
        if(code){
            if([code isEqualToString:@"80001"]){
                //歌曲被抢榜
                _rushNoticeLabel.text = @"啊哦,被您好友抢先一步,这首歌已经被抢先上榜了!";
            }else if ([code isEqualToString:@"80002"]){
                //歌曲被超越
                _rushNoticeLabel.text = @"歌曲已被小伙伴超越，再来KTV挑战吧！";
            }else{
                _rushNoticeLabel.text = @"";
            }
        }else{
            _rushNoticeLabel.text = @"";
        }
        
    }
}

#pragma mark - 关闭抢榜成功或者失败信息

- (void)closeRushBgView:(UIButton *)button
{
    [self disEnableChick:@"抢榜失败"];
    
    [self closeRushAnimation];
}

#pragma mark - 已被抢榜 按钮置灰

- (void)disEnableChick:(NSString *)message
{
    // 抢榜 成功/失败
    //[_rushBtn setTitle:@"已被抢榜" forState:UIControlStateNormal];
    if(message){
        [_rushBtn setTitle:message forState:UIControlStateNormal];
    }else{
        [_rushBtn setTitle:@"已被抢榜" forState:UIControlStateNormal];
    }
    [_rushBtn setTitleShadowColor:[UIColor whiteColor] forState:UIControlStateNormal];
    _rushBtn.titleLabel.shadowOffset = CGSizeMake(0, 1);
    _rushBtn.titleLabel.font = [UIFont boldSystemFontOfSize:20.0f];
    _rushBtn.titleLabel.textAlignment = UITextAlignmentCenter;
    [_rushBtn setTitleColor:UIColorFromRGB(0xd3d3d3) forState:UIControlStateNormal];
    [_rushBtn setBackgroundImage:[UIImage imageNamed:@"btn_xitongxiaoxi_unable.png"] forState:UIControlStateNormal];
    [_rushBtn setEnabled:NO];
}

#pragma mark - 抢榜 按钮可点

- (void)enbleChick
{
    [_rushBtn setTitle:@"抢先上榜" forState:UIControlStateNormal];
    _rushBtn.titleLabel.textAlignment = UITextAlignmentCenter;
    [_rushBtn setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
    [_rushBtn setBackgroundImage:[UIImage imageNamed:@"btn_xitongxiaoxi_c.png"] forState:UIControlStateNormal];
    [_rushBtn addTarget:self action:@selector(rushAction:) forControlEvents:UIControlEventTouchUpInside];
}

#pragma mark - 动画

- (void)openRushAnimation
{
    // 展示抢榜动画
//    [UIView animateWithDuration:1.0 animations:^{
//        [_bgView setHidden:NO];
//        [_rushImage setHidden:NO];
//        _bgView.alpha = 1;
//        _rushImage.alpha = 1;
//    }];
    
    [_bgView setHidden:NO];
    [_rushImage setHidden:NO];
    [self popupAnimation:self.rushImage duration:1.0];
}

- (void)closeRushAnimation
{
    // 关闭抢榜动画
    [UIView animateWithDuration:1.0 animations:^{
        _rushImage.layer.transform = CATransform3DMakeScale(1.5, 1.5, 1.0);
        _rushImage.alpha = 0;
        _bgView.alpha = 0;
    } completion:^(BOOL finished) {
        [_rushImage removeFromSuperview];
        [_bgView removeFromSuperview];
    }];
}

/* 弹出视图的动画 */
- (void)popupAnimation:(UIView *)outView duration:(CFTimeInterval)duration
{
    CAKeyframeAnimation * animation = [CAKeyframeAnimation animationWithKeyPath:@"transform"];
    
    animation.duration = duration;
    animation.removedOnCompletion = NO;
    animation.fillMode = kCAFillModeForwards;
    animation.timingFunction = [CAMediaTimingFunction functionWithName:@"easeInEaseOut"];
    
    NSMutableArray * values = [NSMutableArray array];
    
    [values addObject:[NSValue valueWithCATransform3D:CATransform3DMakeScale(0.1, 0.1, 1.0)]];
    [values addObject:[NSValue valueWithCATransform3D:CATransform3DMakeScale(1.2, 1.2, 1.0)]];
    [values addObject:[NSValue valueWithCATransform3D:CATransform3DMakeScale(0.9, 0.9, 0.9)]];
    [values addObject:[NSValue valueWithCATransform3D:CATransform3DMakeScale(1.0, 1.0, 1.0)]];
    
    animation.values = values;
    
    [outView.layer addAnimation:animation forKey:nil];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
