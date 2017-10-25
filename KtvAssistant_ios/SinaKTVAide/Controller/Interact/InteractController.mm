//
//  InteractController.m
//  SinaKTVAide
//
//  Created by cxp on 14-7-17.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#define KTVAssistantMSG @"ktvassistantSendMessageResult"
#import "DDMenuController.h"
#import "InteractController.h"
#import "InteractTabView.h"
#import "MessageDetailViewController.h"
#import "PKtvAssistantAPI.h"
#import "iToast.h"
#import "CMActionSheet.h"
#import "KTVInputSecretController.h"
#import "RegexKitLite.h"
#import "KTVAppDelegate.h"
//#import "BPush.h"
#import "CommenTools.h"
#import "SystemMessageDetailViewController.h"
#import "SSImageCategory.h"
#import <UIKit/UIKit.h>
#import "Message.h"
#import "GiftView.h"
#import "com_ios_NetUtil.h"
#include "NetUtilCallBack.h"
#import "ChatTableView.h"
#import "BroadEditView.h"
#import "FaceBoard.h"
#import "KTVBaseConfig.h"
#import "AFHTTPRequestOperation.h"
#import "UserSessionManager.h"
#import "Reachability.h"
#import "SVProgressHUD.h"
#import "PCommonUtil.h"
#import "CMActionSheet.h"
//#import "BPush.h"
#import "CustomScroll.h"
#import "NSString+Regex.h"
#import "xingguang.h"
#import "BoardTipView.h"
#import "RedmoneyEditView.h"
#import "RedLobbyEditView.h"
#import "RedPresonEditView.h"
#import "OnlinePersonView.h"
#import "UIView+i7Rotate360.h"
#import "RedDetailView.h"

@interface InteractController()
{
    ChatTableView       *publicchattable;
    UITextField         *textfield;
    UIButton            *enterroombtn;
    UIView              *edtView;
    UIButton            *faceBtn;
    UIButton            *keyBtn;
    UIButton            *giftBtn;
    UIButton            *broadBtn;
    UIImageView         *imgView;
    UIView              *view_loading;
    GiftView            *sendgiftView;
    UIButton            *onlineBtn;
    UILabel             *lblonlinenum;
    UILabel             *lblrightkh;
    NSInteger           idx;
    NSString            *name;
    CustomScroll        *scroll;
    NSMutableArray      *faceMap;
    UIButton            *sendbtn;
    BOOL           iskeybordup;
    UIButton            *redBtn;
    UIImageView         *redImgView;
    UIView              *blackView;
    UIImageView         *redOptView;
    UIButton            *closeBtnVie;
    UIButton            *closeBtn;
    UIImageView         *headimg;
    UILabel             *lblName;
    UILabel             *lblRedmark;
    UILabel             *mUiLate;
    UIImageView         *openimgview;
    UIButton            *openBtn;
    UIButton            *detailBtn;
    NSString            *redid;
    UIImageView         *imgMessage;
    NSInteger           isshowopt;
    NSInteger           ret;
    
    BOOL mIsClosing;
}

@end

@implementation InteractController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        _chattype = 0;
    }
    return self;
}

#pragma mark ViewDidLoad
- (void)viewDidLoad
{
    [super viewDidLoad];
    mIsClosing = false;
    [self.navView initOnlyBackButton:@"互动聊天"];
    [self createOnlinenum];
    [self createBroadbtn];
    self.mainContentView.backgroundColor = [UIColor whiteColor];
    [self createChat];
    textfield.delegate = self;
    textfield.textAlignment = NSTextAlignmentLeft;
    [self createload];
    [self createTable];
    [self createBroadbtn];
    [self createkeyBtn];
    [self createFaceBtn];
    [self createRedBtn];
    [self createRedImgView];
    [self.mainContentView addSubview:edtView];
    [self createfacebord];
    [self createOnlineBtn];
    [self createRedOpt];
    [self createMessageBtn];
    
    //初始化列表数组
    if (self.arrMessage == nil) {
        self.arrMessage = [[NSMutableArray alloc] init];
    }
    [self creatNoMessageLabel];  // 无消息展示
    
    sendgiftView = [[GiftView alloc]initWithFrame:CGRectMake(0, self.mainContentView.frame.size.height-279, _MainScreen_Width, 279)];
    sendgiftView.hidden = YES;
    [self.mainContentView addSubview:sendgiftView];
    
    UITapGestureRecognizer *keyboard_tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(ViewTouchDown:)];
    keyboard_tapGesture.cancelsTouchesInView = NO;
    keyboard_tapGesture.delegate = self;
    [self.mainContentView addGestureRecognizer:keyboard_tapGesture];
    
    UITapGestureRecognizer *redGesture=[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(blackTouch:)];
    [blackView addGestureRecognizer:redGesture];
    [self createfacesendbtn];
    [self flashUIWithRoom];
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch { UIView *view = [touch view];
    if ([view isKindOfClass:[UIButton class]]) {
        return NO;
    }
    return YES;
}

- (void)createOnlinenum
{
    CGSize numsize = [@"互动聊天" sizeWithFont:[UIFont systemFontOfSize:20.0] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
    CGFloat beginx = _MainScreen_Width/2+numsize.width/2;
    UILabel *lblleftkh = [[UILabel alloc]initWithFrame:CGRectMake(beginx, self.navView.titleLabel.frame.origin.y+5, 3, 12)];
    lblleftkh.font = [UIFont systemFontOfSize:11.0f];
    lblleftkh.textColor = UIColorFromRGB(0xffffff);
    lblleftkh.text = @"(";
    lblleftkh.backgroundColor = [UIColor clearColor];
    
    lblonlinenum = [[UILabel alloc]init];
    lblonlinenum.font = [UIFont systemFontOfSize:11.0f];
    lblonlinenum.textColor = UIColorFromRGB(0xffffff);
    lblonlinenum.backgroundColor = [UIColor clearColor];
    int usernum = [NetUtilCallBack getInstance]->m_listPrincessInfo.size()
                    + [NetUtilCallBack getInstance]->m_listRoomUserInfo.size()
                    + [NetUtilCallBack getInstance]->m_listLobbyUserInfo.size();
    
    lblonlinenum.text = [[NSString alloc] initWithFormat:@"%d",usernum];
    CGSize size = [lblonlinenum.text sizeWithFont:[UIFont systemFontOfSize:11] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
    lblonlinenum.frame = CGRectMake(beginx+5, self.navView.titleLabel.frame.origin.y+5, size.width, size.height);
    
    lblrightkh = [[UILabel alloc]initWithFrame:CGRectMake(beginx+5+size.width+2, self.navView.titleLabel.frame.origin.y+5, 3, 12)];
    lblrightkh.font = [UIFont systemFontOfSize:11.0f];
    lblrightkh.textColor = UIColorFromRGB(0xffffff);
    lblrightkh.text = @")";
    lblrightkh.backgroundColor = [UIColor clearColor];
    
    [self.navView addSubview:lblleftkh];
    [self.navView addSubview:lblonlinenum];
    [self.navView addSubview:lblrightkh];
    
}
-(void)createfacesendbtn
{
    sendbtn = [[UIButton alloc]initWithFrame:CGRectMake(_MainScreen_Width-65, _MainScreen_Height-40, 65, 44)];
    sendbtn.backgroundColor = UIColorFromRGB(0x007aff);
    [sendbtn setTitle:@"发送" forState:UIControlStateNormal];
    [sendbtn setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
    sendbtn.titleLabel.font = [UIFont systemFontOfSize:16];
    [sendbtn addTarget:self action:@selector(facesendbtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [self.mainContentView addSubview:sendbtn];
    sendbtn.hidden = YES;
}
-(void)facesendbtnClick:(id)sender
{
    if (![NetUtilCallBack getInstance]->m_bConnectedLobby) {
        PLog(@"请先登录，再聊天");
        return;
    }
    
    if(strlen(textfield.text.UTF8String) == 0)
    {
        PLog(@"内容为空!");
        return;
    }
    
    char content[4096]= {0};
    strcpy(content,textfield.text.UTF8String);
    MFFilter(content, strlen(content));
    
    if (_chattype == 1) {
        if(0 == SendChatKTV(idx, content))
        {
            PLog(@"发送成功");
        }
        else
            PLog(@"发送失败");
        if (![UserSessionManager isRoomAlreadyLogin])   // 未进入包厢
        {
            enterroombtn.hidden = NO;
            textfield.hidden = YES;
        }
        
        [textfield resignFirstResponder];
        
        if (scroll.hidden == NO) {
            CGFloat duration = 0.2;
            CGRect inputFieldRect = edtView.frame;
            inputFieldRect.origin.y = scroll.frame.origin.y + 172;
            [UIView animateWithDuration:duration animations:^{
                edtView.frame = inputFieldRect;
            }];
            [self hideFaceScroll];
            sendbtn.hidden = YES;
        }
        if (imgView.hidden == NO) {
            imgView.hidden = YES;
        }
        
        if (redImgView.tag == 1) {
            [UIView animateWithDuration:.2 animations:^{
                redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
            }];
            redImgView.tag = 0;
        }
        if (_chattype == 1) {
            _chattype = 0;
            textfield.placeholder = @" 点此可给当前绑定包厢发弹幕";
        }
        faceBtn.hidden = NO;
        keyBtn.hidden = YES;
        
        if (isshowopt == 1) {
            isshowopt = 0;
            NSDictionary * userInfo;
            userInfo = [NSDictionary dictionaryWithObjectsAndKeys:nil];
            [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_ISSHOW_CHAT_GIFT_RED_CLICK_NOTIFICATION object:nil userInfo:userInfo];
            [imgMessage stopAnimating];
            imgMessage.hidden = YES;
            [self flashTable:YES];
        }
    }
    else
    {
        if(0 == SendChatKTV(0, content))
        {
            PLog(@"发送成功");
        }
        else
            PLog(@"发送失败");
    }
    
    textfield.text = @"";
    _chattype = 0;
    textfield.placeholder = @" 点此可给当前绑定包厢发弹幕";
}

- (void)flashTable:(BOOL)isReload
{
    if (isReload) {
        [publicchattable.tableView reloadData];
    }
    NSUInteger sectionCount = [publicchattable.tableView numberOfSections];
    if (sectionCount) {
        NSUInteger rowCount = [publicchattable.tableView numberOfRowsInSection:0];
        if (rowCount) {
            NSUInteger ii[2] = {0, rowCount - 1};
            NSIndexPath* indexPath = [NSIndexPath indexPathWithIndexes:ii length:2];
            [publicchattable.tableView scrollToRowAtIndexPath:indexPath atScrollPosition:UITableViewScrollPositionBottom animated:YES];
        }
    }
}

-(void)delegateHoreInfo:(NSMutableDictionary *)dic
{
    BoardTipView *boardTip = [[BoardTipView alloc] initWithInfo:dic];
    boardTip.backgroundColor = UIColorFromRGB(0xfff1c0);
    [self.view addSubview:boardTip];
    [NSTimer scheduledTimerWithTimeInterval:3 target:self selector:@selector(hideMessage:) userInfo:boardTip repeats:NO];
}

-(void)hideMessage:(NSTimer *)timer
{
    UIView *boardTip = (UIView *)[timer userInfo];
    [UIView animateWithDuration:1 animations:^{
        boardTip.alpha = 0;
    } completion:^(BOOL finished) {
        boardTip.hidden = YES;
    }];
}

#pragma mark - 红包查询的回调
- (void)delegateRedStatue:(NSMutableDictionary*)dic{
    ret = [[dic objectForKey:@"ret"] intValue];
    
    if(ret == OPTION_TIMEOUT)
    {
        //隐藏等待对话框
        [SVProgressHUD dismiss];
        
        [[iToast makeText:@"服务器响应超时"] show];
        return ;
    }
    
    NSString *hbid = [NSString stringWithFormat:@"%@",[dic objectForKey:@"redid"]];
    redid = hbid;
    
    [openBtn setBackgroundImage:[UIImage imageNamed:@"chai-13"] forState:UIControlStateNormal];
    [openBtn setTitle:@"拆红包" forState:UIControlStateNormal];
    openBtn.titleLabel.font = [UIFont systemFontOfSize:18.0f];
    //默认隐藏
    openimgview.hidden = YES;
    mUiLate.hidden = YES;
    openBtn.hidden = YES;
    detailBtn.hidden = YES;

    if (ret == 2){ //已抢过,获取红包详情
        //等待对话框不隐藏
        RequestHBDetail([hbid UTF8String]);
        return;
    }
    else
    {
        //隐藏等待对话框
        [SVProgressHUD dismiss];
        
        redOptView.image = [UIImage imageNamed:@"red_shell_0"];
        redOptView.frame = CGRectMake(22.5, 84, _MainScreen_Width-45, _MainScreen_Height-84-64);
        openimgview.frame = CGRectMake(92.5, redOptView.frame.size.height-52-(redOptView.frame.size.width-92.5-92.5), redOptView.frame.size.width-92.5-92.5, redOptView.frame.size.width-92.5-92.5);
        openBtn.frame = CGRectMake(92.5, redOptView.frame.size.height-52-(redOptView.frame.size.width-92.5-92.5), redOptView.frame.size.width-92.5-92.5, redOptView.frame.size.width-92.5-92.5+4);
        headimg.frame = CGRectMake((_MainScreen_Width-45-50)/2, (_MainScreen_Height-168)*54/800, 50, 50);
        headimg.layer.cornerRadius = 25;
        lblName.textColor = UIColorFromRGB(0xffffff);
        closeBtn.hidden = YES;
        closeBtnVie.hidden = NO;
        
        VECTALLINFO::reverse_iterator it = [NetUtilCallBack getInstance]->m_vectAllInfo.rbegin();
        for(; it!= [NetUtilCallBack getInstance]->m_vectAllInfo.rend(); it++)
        {
            if (strcmp(it->redid, [hbid UTF8String]) == 0) {
                break;
            }
        }
        NSString *username;
        if (it->idx == 0 || it->idx == [UserSessionManager getInstance].currentUser.uid) {
            username = [UserSessionManager getInstance].currentUser.name;
        }
        else if(it->idx == 1)
        {
            username = @"新浪KTV助手";
        }
        else
            username = [NSString stringWithUTF8String:it->name];
        
        if(it->idx == 1)//IDX为1 的时候是系统发送的红包
        {
            [headimg setImage:[UIImage imageNamed:@"ic_system_profile"]];
        }
        else
        {
            NSString *headurl = [NSString stringWithUTF8String:it->headphoto];
            [headimg setImageWithURL:[NSURL URLWithString:headurl] placeholderImage:[UIImage imageNamed:@"userFace_normal_b.png"]];
        }
        
        lblName.text = username;
        CGSize size = [lblName.text sizeWithFont:[UIFont systemFontOfSize:15.0f] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
        lblName.frame = CGRectMake((_MainScreen_Width-45-size.width)/2, (_MainScreen_Height-168)*172/800, size.width, size.height);
        lblRedmark.text = [NSString stringWithFormat:@"%@给大家发红包啦",username];
        size = [lblRedmark.text sizeWithFont:[UIFont systemFontOfSize:13.0f] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
        lblRedmark.frame = CGRectMake((_MainScreen_Width-45-size.width)/2, (_MainScreen_Height-168)*230/800, size.width, size.height);
        [lblRedmark setFont:[UIFont systemFontOfSize:13.0f]];
        
        if(ret == 1)//已经抢完
        {
            //显示查看详情的按钮
            detailBtn.hidden = NO;
            //显示太晚的提示
            mUiLate.hidden = NO;
        }
        else if(ret == 0)//还可以抢
        {
            //显示打开的view
            openimgview.hidden = NO;
            //显示拆红包按钮
            openBtn.hidden = NO;
        }
        
        blackView.hidden = NO;
        [self.view bringSubviewToFront:redOptView];
        [self showHBDialog:redOptView];
    }
}

#pragma mark - 红包抢的结果或者红包详情的信息返回
-(void)delegateRedInfo:(NSMutableDictionary *)dic{
    
    //隐藏等待对话框
    [SVProgressHUD dismiss];
    
    /*flag 代表的意思： 0代表抢红包的结果，不过基本抢到或者已抢的都不会到这里，1代表查询红包详情的结果*/
    NSInteger flag = [[dic objectForKey:@"flag"] intValue];
    NSInteger result = [[dic objectForKey:@"result"] intValue];
    NSInteger type = [[dic objectForKey:@"type"] intValue];
    NSInteger sendidx = [[dic objectForKey:@"idx"] intValue];
    
    if(result == OPTION_TIMEOUT)
    {
        //先隐藏可能存在的对话框
        [self closebtnClick:nil];
        //提示超时
        [[iToast makeText:@"服务器响应请求超时"] show];
    }
    else
    {
        if(flag == 0)//如果是抢红包的查询结果
        {
            if(type == 0)//个人红包
            {
                //个人红包领取的结果返回
                if(result == 1)//显示领取金币的对话框
                {
                    redOptView.image = [UIImage imageNamed:@"red_shell_1"];
                    redOptView.frame = CGRectMake(15, 106, _MainScreen_Width-30, _MainScreen_Height-106-56.5);
                    lblName.textColor = UIColorFromRGB(0x333333);
                    headimg.frame = CGRectMake((_MainScreen_Width-30-63)/2, (_MainScreen_Height-212-113)*76/810, 63, 63);
                    headimg.layer.cornerRadius = 31.5;
                    openimgview.hidden = YES;
                    openBtn.hidden = YES;
                    mUiLate.hidden = YES;
                    detailBtn.hidden = YES;
                    closeBtn.hidden = NO;
                    closeBtnVie.hidden = YES;
                    
                    lblName.text = [NSString stringWithFormat:@"来自 %@",[dic objectForKey:@"name"]];
                    CGSize size = [lblName.text sizeWithFont:[UIFont systemFontOfSize:15.0f] constrainedToSize:CGSizeMake(MAXFLOAT, 160)];
                    lblName.frame = CGRectMake((_MainScreen_Width-30-size.width)/2, (_MainScreen_Height-212-113)*76/810+63+8, size.width, size.height);
                    
                    lblRedmark.text = [NSString stringWithFormat:@"%@金币",[dic objectForKey:@"gold"]];
                    lblRedmark.textColor = UIColorFromRGB(0xffa200);
                    lblRedmark.font = [UIFont systemFontOfSize:35.0f];
                    size = [lblRedmark.text sizeWithFont:[UIFont systemFontOfSize:35.0f] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
                    lblRedmark.frame = CGRectMake((_MainScreen_Width-30-size.width)/2, (_MainScreen_Height-212-113)*76/810+63+8+41+16, size.width, size.height);
                    
                    
                    
                    blackView.hidden = NO;
                    [self.view bringSubviewToFront:redOptView];
                    [self showHBDialog:redOptView];
                    return;
                }
                //上层已经做了处理
                else if(result == 2)
                {
                }
            }
            else//争抢红包
            {
                if(result == 0)//没抢到红包
                {
                    //如果这时候对话框还在显示，则更新显示信息
                    if(![redOptView isHidden])
                    {
                        //隐藏抢红包的动画等
                        [openimgview removeRotate360];
                        openimgview.hidden = YES;
                        openBtn.hidden = YES;
                        
                        //显示查看详情的信息
                        mUiLate.hidden = NO;
                        detailBtn.hidden = NO;
                        
                        closeBtn.hidden = YES;
                        closeBtnVie.hidden = NO;
                    }
                }
                else//已经抢到了，或者抢过了
                {
                    //上层已经处理
                }
            }
        }
        //红包详情结果查询的返回
        else if(flag == 1)
        {
            //去往红包详情页面
            RedDetailView *detailview = [[RedDetailView alloc] init];
            NSInteger getgold = [[dic objectForKey:@"getgold"] intValue];
            NSInteger sendtime = [[dic objectForKey:@"sendtime"] intValue];
            HBSendDetail senddetail;
            senddetail.sendtime = sendtime;
            senddetail.sendidx = sendidx;
            strcpy(senddetail.headurl,[[dic objectForKey:@"headurl"] UTF8String]);
            strcpy(senddetail.mark,[[dic objectForKey:@"mark"] UTF8String]);
            senddetail.gold = [[dic objectForKey:@"gold"] intValue];
            strcpy(senddetail.name,[[dic objectForKey:@"name"] UTF8String]);
            senddetail.type = (char)type;
            if (type==0 && (sendidx==0 || sendidx==[UserSessionManager getInstance].currentUser.uid)) {
                senddetail.num = [[dic objectForKey:@"getnum"] intValue];
            }
            else
                senddetail.num = [[dic objectForKey:@"num"] intValue];
            detailview.sendinfo = senddetail;
            detailview.getgold = getgold;
            [textfield resignFirstResponder];
            
            if (scroll.hidden == NO) {
                CGFloat duration = 0.2;
                CGRect inputFieldRect = edtView.frame;
                inputFieldRect.origin.y = scroll.frame.origin.y + 122;
                [UIView animateWithDuration:duration animations:^{
                    edtView.frame = inputFieldRect;
                }];
                [self hideFaceScroll];
                sendbtn.hidden = YES;
            }
            redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
            redImgView.tag = 0;
            blackView.hidden = YES;
            [self closebtnClick:nil];
            //[self.view sendSubviewToBack:redOptView];
            [self.navigationController pushViewController:detailview animated:YES];
        }
    }
}

-(void)showHBDialog:(UIView*) view
{
    CAKeyframeAnimation* animation = [CAKeyframeAnimation animationWithKeyPath:@"transform"];
    animation.duration = 0.5;
    NSMutableArray *values = [NSMutableArray array];
    [values addObject:[NSValue valueWithCATransform3D:CATransform3DMakeScale(0.1, 0.1, 1.0)]];
    [values addObject:[NSValue valueWithCATransform3D:CATransform3DMakeScale(1.2, 1.2, 1.0)]];
    [values addObject:[NSValue valueWithCATransform3D:CATransform3DMakeScale(0.9, 0.9, 1.0)]];
    [values addObject:[NSValue valueWithCATransform3D:CATransform3DMakeScale(1.0, 1.0, 1.0)]];
    animation.values = values;
    [view.layer addAnimation:animation forKey:nil];
}

- (void)delegateInfo:(NSInteger)type
{
    if (type != 3) {
        if (isshowopt == 0) {
            [self flashTable:true];
            if (!sendgiftView.hidden)
                [sendgiftView showMessage];
        }
        else
        {
            imgMessage.hidden = NO;
            [imgMessage startAnimating];
        }

    }
    else
    {
        int usernum = [NetUtilCallBack getInstance]->m_listPrincessInfo.size()
        + [NetUtilCallBack getInstance]->m_listRoomUserInfo.size()
        + [NetUtilCallBack getInstance]->m_listLobbyUserInfo.size();
        
        lblonlinenum.text = @"";
        lblonlinenum.text = [[NSString alloc] initWithFormat:@"%d",usernum];
        CGSize size = [lblonlinenum.text sizeWithFont:[UIFont systemFontOfSize:11] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
        
        CGSize numsize = [@"互动聊天" sizeWithFont:[UIFont systemFontOfSize:20.0] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
        CGFloat beginx = _MainScreen_Width/2+numsize.width/2;

        lblonlinenum.frame = CGRectMake(beginx+5, self.navView.titleLabel.frame.origin.y+5, size.width, size.height);
        lblrightkh.frame = CGRectMake(beginx+5+size.width+2, self.navView.titleLabel.frame.origin.y+5, 3, 12);
    }
}

-(void)createTable
{
    publicchattable = [[ChatTableView alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height-44) type:0 delegate:self];
    publicchattable.hidden = NO;
    [self.mainContentView addSubview:publicchattable];
    [self.mainContentView bringSubviewToFront:publicchattable.privateHeadBtn];
    [self.mainContentView bringSubviewToFront:publicchattable.giftHeadBtn];
    [self.mainContentView bringSubviewToFront:publicchattable.redHeadBtn];
}
- (void)createload
{
    PLog(@"createload");
    // 轻敲手势
    UITapGestureRecognizer *gift_tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(loadTouchDown)];
    gift_tapGesture.cancelsTouchesInView = NO;
    
    // 礼物界面弹出透明遮盖view
    view_loading = [[UIView alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, self.view.frame.size.height-150)];
    view_loading.backgroundColor = [UIColor colorWithWhite:0.0f alpha:0.8f];
    [view_loading addGestureRecognizer:gift_tapGesture];
    [self.mainContentView addSubview:view_loading];
    [self.mainContentView sendSubviewToBack:view_loading];
    view_loading.hidden = YES;
}

- (void)loadTouchDown
{
    [self.mainContentView sendSubviewToBack:view_loading];
    view_loading.hidden = YES;
    sendgiftView.hidden = YES;
    broadBtn.enabled = YES;
    
    [UIView animateWithDuration:.2 animations:^{
        redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
    }];
    redImgView.tag = 0;
}

- (void)createChat
{
    //PLog(@"**********%f",_MainScreen_Width);
    edtView = [[UIView alloc] initWithFrame:CGRectMake(-1., self.mainContentView.frame.size.height-44, _MainScreen_Width+2, 44)];
    edtView.backgroundColor = UIColorFromRGB(0xf0f1f5);
    edtView.layer.borderColor = [UIColorFromRGB(0xb3b6bd) CGColor];
    edtView.layer.borderWidth = 0.5;
    edtView.tag = 0;
    textfield = [[UITextField alloc] initWithFrame:CGRectMake(45, 6, _MainScreen_Width-45-83.5, 30)];
    [textfield setAutoresizingMask: UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight];
    [textfield setAutocapitalizationType:UITextAutocapitalizationTypeNone];
    [textfield setAutocorrectionType:UITextAutocorrectionTypeNo];
    textfield.backgroundColor = UIColorFromRGB(0xffffff);
    textfield.layer.borderColor = [UIColorFromRGB(0xb3b6bd) CGColor];
    textfield.layer.borderWidth = 0.5;
    textfield.layer.cornerRadius = 5;
    textfield.font = [UIFont systemFontOfSize:14];
    textfield.returnKeyType = UIReturnKeySend;
    textfield.enablesReturnKeyAutomatically = YES;
    textfield.placeholder = @" 点此可给当前绑定包厢发弹幕";
    textfield.clearButtonMode = UITextFieldViewModeAlways;
    textfield.adjustsFontSizeToFitWidth = YES;
    textfield.tag = 0;
    textfield.delegate = self;
    [edtView addSubview:textfield];
    textfield.hidden = YES;
    
    enterroombtn = [[UIButton alloc] initWithFrame:CGRectMake(44.5, 5.5, _MainScreen_Width-45-83.5, 31)];
    enterroombtn.backgroundColor = [UIColor clearColor];
    [enterroombtn setImage:[UIImage imageNamed:@"click_on_the_box_0" ] forState:UIControlStateNormal];
    [enterroombtn setImage:[UIImage imageNamed:@"click_on_the_box_1" ] forState:UIControlStateHighlighted];
    [edtView addSubview:enterroombtn];
    enterroombtn.hidden = YES;
    // 进入包厢
    [enterroombtn addTarget:self action:@selector(goInRoom:) forControlEvents:UIControlEventTouchUpInside];
    
    if ([UserSessionManager isRoomAlreadyLogin])   // 已进入包厢
    {
        enterroombtn.hidden = YES;
        textfield.hidden = NO;
    }
    else
    {
        enterroombtn.hidden = NO;
        textfield.hidden = YES;
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

#pragma mark - 开启二维码扫描

- (void)openAction
{
    _reader = [ZBarReaderViewController new];
    //设备尺寸 3.5/4.0英寸
    _reader.view.frame = CGRectMake(0, 0, 320, 480+(iPhone5?88:0));
    //_reader.view.frame = CGRectMake(0, 0, _MainScreen_Width, _MainScreen_Height+(iPhone5?88:0));
    _reader.readerDelegate = self;
    [_reader setShowsZBarControls:NO]; // 禁止 官方的取消和帮助说明按钮
    _reader.supportedOrientationsMask = ZBarOrientationMaskAll;
    _reader.cameraFlashMode = UIImagePickerControllerCameraFlashModeOff;
    ZBarImageScanner *scanner = _reader.scanner;
    [scanner setSymbology: ZBAR_I25
                   config: ZBAR_CFG_ENABLE
                       to: 0];
    
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
    
    [self presentModalViewController: _reader animated: YES];
}

#pragma mark -  取消二维码扫描

- (void)cancelAction:(UIButton *)button
{
    [_reader dismissModalViewControllerAnimated:YES];
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
    [reader dismissModalViewControllerAnimated: YES];
    
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
             NSString * tag = [roominfo.roomPassword lowercaseString];
             //PLog(@"TAG-->>%@",tag);
             //[BPush setTag:tag];
             [SVProgressHUD dismiss];
             //[self setRightAndBadgeButton];
             
             ResultData_RoomInfo room = {0};
             GetRoomInfoFromLocal([roominfo.xgRoomID intValue], &room);
             
             int ret = 0;
             if ([NetUtilCallBack getInstance]->m_bWangtong)
                 ret = ConnectRoom(room.ip_2, room.port, XG_ROOMTYPE_BASE);
             else
                 ret = ConnectRoom(room.ip_1, room.port, XG_ROOMTYPE_BASE);
             if(ret == 0)
                 [self flashUIWithRoom]; // 进入包厢或者退出包厢刷新界面
             else
             {
                 [SVProgressHUD dismiss];
                 NSString *msg = [results objectForKey:@"msg"];
                 [CommenTools showAlertViewWithTitle:msg];
             }
         }
         else
         {
             [SVProgressHUD dismiss];
             NSString *msg = [results objectForKey:@"msg"];
             [CommenTools showAlertViewWithTitle:msg];
         }
     } failure:^(AFHTTPRequestOperation *operation, NSError *error)
     {
         [SVProgressHUD dismiss];
         PLog(@"doLogin failed...");
         [CommenTools showNetworkDisconnectedAlertView];
     }];
    [operation start];
}

#pragma mark - 进入包厢刷新界面

- (void)flashUIWithRoom
{
    if ([UserSessionManager isRoomAlreadyLogin])   // 已进入包厢
    {
        enterroombtn.hidden = YES;
        textfield.hidden = NO;
    }
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    //User *user = [UserSessionManager getInstance].currentUser;
    if(strlen(textfield.text.UTF8String) == 0)
    {
        PLog(@"内容为空!");
        return NO;
    }
    char content[4096]= {0};
    
    strcpy(content,textfield.text.UTF8String);
    MFFilter(content, strlen(content));
    
    if (_chattype == 1) {
        if(0 == SendChatKTV(idx, content))
        {
            PLog(@"发送成功");
        }
        else
            PLog(@"发送失败");
        if (![UserSessionManager isRoomAlreadyLogin])   // 未进入包厢
        {
            enterroombtn.hidden = NO;
            textfield.hidden = YES;
        }
        
        [textfield resignFirstResponder];
        
        if (scroll.hidden == NO) {
            CGFloat duration = 0.2;
            CGRect inputFieldRect = edtView.frame;
            inputFieldRect.origin.y = scroll.frame.origin.y + 172;
            [UIView animateWithDuration:duration animations:^{
                edtView.frame = inputFieldRect;
            }];
            [self hideFaceScroll];
            sendbtn.hidden = YES;
        }
        if (imgView.hidden == NO) {
            imgView.hidden = YES;
        }
        
        if (redImgView.tag == 1) {
            [UIView animateWithDuration:.2 animations:^{
                redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
            }];
            redImgView.tag = 0;
        }
        if (_chattype == 1) {
            _chattype = 0;
            textfield.placeholder = @" 点此可给当前绑定包厢发弹幕";
        }
        faceBtn.hidden = NO;
        keyBtn.hidden = YES;
        
        if (isshowopt == 1) {
            isshowopt = 0;
            NSDictionary * userInfo;
            userInfo = [NSDictionary dictionaryWithObjectsAndKeys:nil];
            [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_ISSHOW_CHAT_GIFT_RED_CLICK_NOTIFICATION object:nil userInfo:userInfo];
            [imgMessage stopAnimating];
            imgMessage.hidden = YES;
            [self flashTable:YES];
        }

    }
    else
    {
        if(0 == SendChatKTV(0, content))
        {
            PLog(@"发送成功");
        }
        else
            PLog(@"发送失败");
    }
    textfield.text = @"";
    _chattype = 0;
    textfield.placeholder = @" 点此可给当前绑定包厢发弹幕";
    return YES;
}


- (void)createBroadbtn
{
    broadBtn = [[UIButton alloc]initWithFrame:CGRectMake(7.5, 6, 30, 30)];
    broadBtn.layer.borderWidth = 0;
    [broadBtn setBackgroundColor:[UIColor clearColor]];
    UIImage *broadImageNormal = [UIImage imageNamed:@"speaker_btn_0"];
    UIImage *broadImage = [UIImage imageNamed:@"speaker_btn_1"];
    [broadBtn setBackgroundImage:(broadImageNormal) forState:UIControlStateNormal];
    [broadBtn setBackgroundImage:broadImage forState:UIControlStateHighlighted];
    [broadBtn addTarget:self action:@selector(broadBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [edtView addSubview:broadBtn];
}
- (void)broadBtnClick:(id)sender
{
    BroadEditView *broadView = [[BroadEditView alloc] init];
    
    [textfield resignFirstResponder];
    
    if (scroll.hidden == NO) {
        CGFloat duration = 0.2;
        CGRect inputFieldRect = edtView.frame;
        inputFieldRect.origin.y = scroll.frame.origin.y + 122+50;
        [UIView animateWithDuration:duration animations:^{
            edtView.frame = inputFieldRect;
        }];
        [self hideFaceScroll];
        sendbtn.hidden = YES;
    }
    [UIView animateWithDuration:.2 animations:^{
        redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
    }];
    redImgView.tag = 0;
    if (isshowopt == 1) {
        isshowopt = 0;
        NSDictionary * userInfo;
        userInfo = [NSDictionary dictionaryWithObjectsAndKeys:nil];
        [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_ISSHOW_CHAT_GIFT_RED_CLICK_NOTIFICATION object:nil userInfo:userInfo];
        if (imgMessage.hidden == NO) {
            [imgMessage stopAnimating];
            imgMessage.hidden = YES;
            [self flashTable:YES];
        }
    }
    faceBtn.hidden = NO;
    keyBtn.hidden = YES;
    [self.navigationController pushViewController:broadView animated:YES];
}
- (void)btnBroadChange:(id)sender
{
    broadBtn.backgroundColor = [UIColor clearColor];
    BroadEditView *broadView = [[BroadEditView alloc] init];
    [self.navigationController pushViewController:broadView animated:YES];
}

- (void)createkeyBtn
{
    keyBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    keyBtn.frame = CGRectMake(_MainScreen_Width-76, 7, 30, 30);
    keyBtn.layer.borderWidth = 0;
    keyBtn.tag = 0;
    [keyBtn setBackgroundColor:[UIColor clearColor]];
    UIImage *keyImage = [UIImage imageNamed:@"chatBar_btn_key0.png"];
    [keyBtn setBackgroundImage:(keyImage) forState:UIControlStateNormal];
    [keyBtn addTarget:self action:@selector(keybtnChange:) forControlEvents:UIControlEventTouchUpInside];
    [edtView addSubview:keyBtn];
    keyBtn.hidden = YES;
}
-(void)keybtnChange:(id)sender
{
    CGFloat duration = 0.2;
    CGRect inputFieldRect = edtView.frame;
    inputFieldRect.origin.y = scroll.frame.origin.y + 216 - 44;
    [UIView animateWithDuration:duration animations:^{
        edtView.frame = inputFieldRect;
    }];
    [self hideFaceScroll];
    [textfield becomeFirstResponder];
    faceBtn.hidden = NO;
    keyBtn.hidden = YES;
    sendbtn.hidden = YES;
}

-(void)textFieldDidBeginEditing:(UITextField *)textField
{
    faceBtn.hidden = NO;
    keyBtn.hidden = YES;
    scroll.hidden = YES;
    if (isshowopt == 1) {
        isshowopt = 0;
        NSDictionary * userInfo;
        userInfo = [NSDictionary dictionaryWithObjectsAndKeys:nil];
        [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_ISSHOW_CHAT_GIFT_RED_CLICK_NOTIFICATION object:nil userInfo:userInfo];
        if (imgMessage.hidden == NO) {
            [imgMessage stopAnimating];
            imgMessage.hidden = YES;
            [self flashTable:YES];
        }
    }
}
- (void)facebtnChange:(id)sender
{
    if (enterroombtn.hidden == NO) {
        return;
    }
    faceBtn.hidden = YES;
    keyBtn.hidden = NO;
    [textfield resignFirstResponder];
    sendbtn.hidden = NO;
    
    CGFloat duration = 0.2;
    CGRect inputFieldRect = edtView.frame;
    inputFieldRect.origin.y = scroll.frame.origin.y - 44;
    
    CGRect tabRect = edtView.frame;
    tabRect.size.height =  self.mainContentView.frame.size.height - scroll.frame.size.height - 44;
    [UIView animateWithDuration:duration animations:^{
        edtView.frame = inputFieldRect;
        publicchattable.tableView.frame = CGRectMake(0, 0, _MainScreen_Width, tabRect.size.height);
        [self flashTable:false];
    }];
    scroll.hidden = NO;
    if (isshowopt == 1) {
        isshowopt = 0;
        NSDictionary * userInfo;
        userInfo = [NSDictionary dictionaryWithObjectsAndKeys:nil];
        [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_ISSHOW_CHAT_GIFT_RED_CLICK_NOTIFICATION object:nil userInfo:userInfo];
        if (imgMessage.hidden == NO) {
            [imgMessage stopAnimating];
            imgMessage.hidden = YES;
            [self flashTable:YES];
        }
    }
}

-(void)hideFaceScroll
{
    if (!scroll.hidden) {
        publicchattable.tableView.frame = CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height-44);
        [self flashTable:false];
    }
    scroll.hidden = YES;
}

- (void)createFaceBtn
{
    faceBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    faceBtn.frame = CGRectMake(_MainScreen_Width-76, 7, 30, 30);
    faceBtn.layer.borderWidth = 0;
    faceBtn.tag = 0;
    [faceBtn setBackgroundColor:[UIColor clearColor]];
    UIImage *faceImageNormal = [UIImage imageNamed:@"chatBar_btn_face0.png"];
    [faceBtn setBackgroundImage:(faceImageNormal) forState:UIControlStateNormal];
    [faceBtn addTarget:self action:@selector(facebtnChange:) forControlEvents:UIControlEventTouchUpInside];
    [edtView addSubview:faceBtn];
    faceBtn.hidden = NO;
}

- (void)createRedBtn
{
    redBtn = [[UIButton alloc]initWithFrame:CGRectMake(_MainScreen_Width-40, 7, 30, 30)];
    redBtn.layer.borderWidth = 0;
    redBtn.tag = 0;
    [redBtn setBackgroundColor:[UIColor clearColor]];
    UIImage *redImageNormal = [UIImage imageNamed:@"red_btn_0"];
    UIImage *redImage = [UIImage imageNamed:@"red_btn_1"];
    [redBtn setBackgroundImage:(redImageNormal) forState:UIControlStateNormal];
    [redBtn setBackgroundImage:(redImage) forState:UIControlStateHighlighted];
    [redBtn addTarget:self action:@selector(redbtnChange:) forControlEvents:UIControlEventTouchUpInside];
    [edtView addSubview:redBtn];
}

- (void)redbtnChange:(id)sender
{
    [textfield resignFirstResponder];
    
    if (scroll.hidden == NO) {
        CGFloat duration = 0.2;
        CGRect inputFieldRect = edtView.frame;
        inputFieldRect.origin.y = scroll.frame.origin.y + 122 + 50;
        [UIView animateWithDuration:duration animations:^{
            edtView.frame = inputFieldRect;
        }];
        [self hideFaceScroll];
        sendbtn.hidden = YES;
    }
    if (faceBtn.hidden) {
        faceBtn.hidden = NO;
        keyBtn.hidden = YES;
    }
    
    if (isshowopt == 1) {
        isshowopt = 0;
        NSDictionary * userInfo;
        userInfo = [NSDictionary dictionaryWithObjectsAndKeys:nil];
        [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_ISSHOW_CHAT_GIFT_RED_CLICK_NOTIFICATION object:nil userInfo:userInfo];
        if (imgMessage.hidden == NO) {
            [imgMessage stopAnimating];
            imgMessage.hidden = YES;
            [self flashTable:YES];
        }
    }
    
    if (redImgView.tag == 0) {
        redImgView.tag = 1;
        [UIView animateWithDuration:.2 animations:^{
            redImgView.frame = CGRectMake(_MainScreen_Width-112.5, self.mainContentView.frame.size.height-44-71, 105, 71);
        }];
        
    }
    else
    {
        [UIView animateWithDuration:.2 animations:^{
            redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
        }];
        redImgView.tag = 0;
    }
}

- (void)createRedImgView{
    redImgView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"btn_red_choose"]];
    redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
    [self.mainContentView addSubview:redImgView];
    redImgView.userInteractionEnabled = YES;
    UIButton *dkredBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    dkredBtn.frame = CGRectMake(1, 1, 103, 31);
    [dkredBtn setBackgroundColor:[UIColor clearColor]];
    [dkredBtn setBackgroundImage:[UIImage imageNamed:@"click_red_choose"] forState:UIControlStateHighlighted];
    [dkredBtn setTitle:@"大款红包" forState:UIControlStateNormal];
    dkredBtn.titleLabel.font = [UIFont systemFontOfSize:15.0f];
    dkredBtn.titleLabel.textAlignment = UITextAlignmentCenter;
    [dkredBtn.titleLabel setTextColor:UIColorFromRGB(0xe5e5e5)];
    [dkredBtn addTarget:self action:@selector(dkredBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [redImgView addSubview:dkredBtn];
    
    UIButton *roomredBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    roomredBtn.frame = CGRectMake(1, 34, 103, 31);
    [roomredBtn setBackgroundColor:[UIColor clearColor]];
    [roomredBtn setBackgroundImage:[UIImage imageNamed:@"click_red_choose"] forState:UIControlStateHighlighted];
    [roomredBtn setTitle:@"包厢红包" forState:UIControlStateNormal];
    roomredBtn.titleLabel.font = [UIFont systemFontOfSize:15.0f];
    roomredBtn.titleLabel.textAlignment = UITextAlignmentCenter;
    [roomredBtn.titleLabel setTextColor:UIColorFromRGB(0xe5e5e5)];
    [roomredBtn addTarget:self action:@selector(roomredBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [redImgView addSubview:roomredBtn];
    
    redImgView.tag = 0;

}
- (void)dkredBtnClick:(id)sender{
    RedLobbyEditView *redlobbyView = [[RedLobbyEditView alloc] init];
    
    [textfield resignFirstResponder];
    
    if (scroll.hidden == NO) {
        CGFloat duration = 0.2;
        CGRect inputFieldRect = edtView.frame;
        inputFieldRect.origin.y = scroll.frame.origin.y + 122;
        [UIView animateWithDuration:duration animations:^{
            edtView.frame = inputFieldRect;
        }];
        [self hideFaceScroll];
        sendbtn.hidden = YES;
    }
    redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
    redImgView.tag = 0;
    [self.navigationController pushViewController:redlobbyView animated:YES];
}

- (void)roomredBtnClick:(id)sender{
    RedmoneyEditView *redroomView = [[RedmoneyEditView alloc] init];
    
    [textfield resignFirstResponder];
    
    if (scroll.hidden == NO) {
        CGFloat duration = 0.2;
        CGRect inputFieldRect = edtView.frame;
        inputFieldRect.origin.y = scroll.frame.origin.y + 122;
        [UIView animateWithDuration:duration animations:^{
            edtView.frame = inputFieldRect;
        }];
        [self hideFaceScroll];
        sendbtn.hidden = YES;
    }
    redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
    redImgView.tag = 0;
    if ([UserSessionManager isRoomAlreadyLogin])   // 已进入包厢
        [self.navigationController pushViewController:redroomView animated:YES];
    else
        [self goInRoom:redBtn];
}

- (void)giftbtnChange:(id)sender
{
    if (scroll.hidden == NO) {
        CGFloat duration = 0.2;
        CGRect inputFieldRect = edtView.frame;
        inputFieldRect.origin.y = scroll.frame.origin.y + 122;
        [UIView animateWithDuration:duration animations:^{
            edtView.frame = inputFieldRect;
        }];
        [self hideFaceScroll];
        sendbtn.hidden = YES;
        keyBtn.hidden = YES;
        faceBtn.hidden = NO;
    }
    
    if (iskeybordup == YES) {
        [textfield resignFirstResponder];
    }
    
    sendgiftView.hidden = NO;
    view_loading.hidden = NO;
    [self.mainContentView bringSubviewToFront:view_loading];
    [self.mainContentView bringSubviewToFront:sendgiftView];
    broadBtn.enabled = NO;
}

- (void)createfacebord
{
    NSMutableArray *arr =[[ NSMutableArray alloc]init];
    for (NSInteger i = 0; i<7; i++) {
        ExpressionView *view1= [[ExpressionView alloc]initWithFrame:CGRectMake(self.mainContentView.frame.size.width * i, 0, _MainScreen_Width, 166) index:i];
        [arr addObject: view1];
        view1.delegate = self;
    }
    scroll = [[CustomScroll alloc] initWithFrame:CGRectMake(0, _MainScreen_Height-216-44, self.mainContentView.frame.size.width, 216) views:arr];
    scroll.pageControl.selectColor = UIColorFromRGB(0x8a8e97);
    scroll.pageControl.freeColor = UIColorFromRGB(0xbbbfc4);
    scroll.backgroundColor = UIColorFromRGB(0xf0f1f5);
    [self.mainContentView addSubview:scroll];
    scroll.hidden = YES;
}

-(void)ExpressionClick:(NSInteger)tag
{
    NSMutableString *faceString = [[NSMutableString alloc] init];
    if (textfield.text) {
        [faceString appendString:textfield.text];
    }
    if (tag != -1) {
        if (textfield) {
            if (textfield.text == nil) {
                textfield.text = @"";
            }
            NSString* value = [NSString stringWithFormat:@"[a/%d]", tag];
            [faceString appendString:value];
            textfield.text = faceString;
        }
    }
    else
    {
        NSString *inputString = textfield.text;
        if ( inputString.length ) {
            NSString *string = nil;
            NSArray* arr = [inputString regexfaceContent];
            if (arr && arr.count) {
                NSString* tmp = [arr objectAtIndex:(arr.count-1)];
                if ((tmp.length == 5 || tmp.length == 6 || tmp.length == 7) && ([[tmp substringToIndex:3] isEqualToString:@"[a/"])){
                    string = [inputString substringToIndex:(inputString.length-tmp.length)];
                }
                else
                {
                    string = [inputString substringToIndex:(inputString.length-1)];
                }
            }
            else
            {
                string = [inputString substringToIndex:(inputString.length-1)];
            }
            textfield.text = string;
        }
    }
    scroll.hidden = NO;
}
- (void)searchstring:(NSString*)str1 :(NSString*)str2
{
    NSRange range = [str1 rangeOfString:str2];
    int location = range.location;
    int leight = range.length;
    NSString *astring = [[NSString alloc] initWithString:[NSString stringWithFormat:@"Location:%i,Leight:%i",location,leight]];
    PLog(@"astring:%@",astring);
}

- (void)keyboardWillShow:(NSNotification *)notification {
    NSDictionary *info = [notification userInfo];
    CGFloat duration = 0.1;//[[info objectForKey:UIKeyboardAnimationDurationUserInfoKey] floatValue];
    CGRect endKeyboardRect = [[info objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue];
    
    CGRect inputFieldRect = edtView.frame;
    inputFieldRect.origin.y = endKeyboardRect.origin.y - inputFieldRect.size.height -64;
    
    CGRect tabRect = publicchattable.frame;
    
    tabRect.size.height =  self.mainContentView.frame.size.height - endKeyboardRect.size.height - 44;
    [UIView animateWithDuration:duration animations:^{
        edtView.frame = inputFieldRect;
        publicchattable.tableView.frame = CGRectMake(0, 0, _MainScreen_Width, tabRect.size.height);
        [self flashTable:false];
        [self.mainContentView.superview bringSubviewToFront:self.navView];
    }];
    imgView.hidden = YES;
    iskeybordup = YES;
    redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
    redImgView.tag = 0;
}

-(void)keyboardWillHide:(NSNotification *)notification {
    NSDictionary *info = [notification userInfo];
    CGFloat duration = [[info objectForKey:UIKeyboardAnimationDurationUserInfoKey] floatValue];
    CGRect beginKeyboardRect = [[info objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue];
    CGRect endKeyboardRect = [[info objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue];
    CGFloat yOffset = endKeyboardRect.origin.y - beginKeyboardRect.origin.y;
    CGRect inputFieldRect = edtView.frame;
    inputFieldRect.origin.y += yOffset;
    
    CGRect tabRect = publicchattable.frame;
    //tabRect.origin.y += yOffset - 49;
    tabRect.size.height += yOffset ;
    [UIView animateWithDuration:duration animations:^{
        edtView.frame = inputFieldRect;
        
        publicchattable.tableView.frame =  CGRectMake(0, 0, _MainScreen_Width, self.mainContentView.frame.size.height-44);
        [self flashTable:false];
    }];
    iskeybordup = NO;
}

- (void)blackTouch:(UITapGestureRecognizer *)gesture
{
    CGPoint location = [gesture locationInView:redOptView];
    if([redOptView pointInside:location withEvent:nil])
        return ;
    
    [self closebtnClick:nil];
}

- (void)ViewTouchDown:(UITapGestureRecognizer *)gesture
{
    CGPoint touchPointInSuperview = [gesture locationInView:edtView];
    UIView *touchedView = [edtView hitTest:touchPointInSuperview withEvent:nil];
    if ([touchedView isKindOfClass:[UIButton class]]) {
        return;
    }
    CGPoint location = [gesture locationInView:self.mainContentView];
    if (CGRectContainsPoint(scroll.frame, location))
    {
        return;
    }
    
    [textfield resignFirstResponder];
    
    if (scroll.hidden == NO) {
        CGFloat duration = 0.2;
        CGRect inputFieldRect = edtView.frame;
        inputFieldRect.origin.y = scroll.frame.origin.y + 172;
        [UIView animateWithDuration:duration animations:^{
            edtView.frame = inputFieldRect;
        }];
        [self hideFaceScroll];
        sendbtn.hidden = YES;
    }
    if (imgView.hidden == NO) {
        imgView.hidden = YES;
    }
    
    if (redImgView.tag == 1) {
        [UIView animateWithDuration:.2 animations:^{
            redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
        }];
        redImgView.tag = 0;
    }
    if (_chattype == 1) {
        _chattype = 0;
        textfield.placeholder = @" 点此可给当前绑定包厢发弹幕";
    }
    faceBtn.hidden = NO;
    keyBtn.hidden = YES;
    
    if (isshowopt == 1) {
        isshowopt = 0;
        NSDictionary * userInfo;
        userInfo = [NSDictionary dictionaryWithObjectsAndKeys:nil];
        [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_ISSHOW_CHAT_GIFT_RED_CLICK_NOTIFICATION object:nil userInfo:userInfo];
        [imgMessage stopAnimating];
        imgMessage.hidden = YES;
        [self flashTable:YES];
    }
    if (![UserSessionManager isRoomAlreadyLogin])   // 未进入包厢
    {
        enterroombtn.hidden = NO;
        textfield.hidden = YES;
    }
}

- (void)showGift:(NSInteger)useridx:(NSString *)username;
{
    sendgiftView.hidden = NO;
    view_loading.hidden = NO;
    [self.mainContentView bringSubviewToFront:view_loading];
    [self.mainContentView bringSubviewToFront:sendgiftView];
    broadBtn.enabled = NO;
    [sendgiftView addSendTarge:username index:useridx];
}

- (void)showPrivateChat:(NSInteger)useridx:(NSString *)username;
{
    if (![UserSessionManager isRoomAlreadyLogin])   // 未进入包厢
    {
        enterroombtn.hidden = YES;
        textfield.hidden = NO;
    }
    _chattype = 1;
    idx = useridx;
    NSString *str1 = @"对";
    NSString *str = [str1 stringByAppendingString:username];
    NSString *str2 = @"说：";
    str = [str stringByAppendingString:str2];
    textfield.placeholder = str;
}
#pragma mark - ChatCellHBProtocol
-(void)onHBClick:(AllInfo)info
{
    //显示等待框
    [SVProgressHUD showWithMaskType:SVProgressHUDMaskTypeClear];
    //个人红包
    if (info.redtype == 0) {
        //如果是自己发送的红包
        if (info.idx==0 || info.idx == [UserSessionManager getInstance].currentUser.uid) {
            RequestHBDetail(info.redid);
        }
        else//别人发给我的个人红包
            RequestVieHB(info.redid);
    }
    else
    {//需要抢的红包，需要先查询红包状态
        RequestHBStatus(info.redid);
    }
}

#pragma mark - ViewWillDisappear
-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:YES];
    
    [NetUtilCallBack getInstance].thedelegate = nil;
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:NO];
    
    [NetUtilCallBack getInstance].thedelegate = self;
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillShow:) name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
    
    [self flashTable:true];
    
    int usernum = [NetUtilCallBack getInstance]->m_listPrincessInfo.size()
    + [NetUtilCallBack getInstance]->m_listRoomUserInfo.size()
    + [NetUtilCallBack getInstance]->m_listLobbyUserInfo.size();
    
    lblonlinenum.text = [[NSString alloc] initWithFormat:@"%d",usernum];
    CGSize size = [lblonlinenum.text sizeWithFont:[UIFont systemFontOfSize:11] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
    CGSize numsize = [@"互动聊天" sizeWithFont:[UIFont systemFontOfSize:20.0] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
    CGFloat beginx = _MainScreen_Width/2+numsize.width/2;
    lblonlinenum.frame = CGRectMake(beginx+5, self.navView.titleLabel.frame.origin.y+5, size.width, size.height);
    lblrightkh.frame = CGRectMake(beginx+5+size.width+2, self.navView.titleLabel.frame.origin.y+5, 3, 12);
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(imgHead_lblName_Click:) name:KTVASSISTANT_CHAT_GIFT_RED_CLICK_NOTIFICATION object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showSendMsgResult:) name:KTVAssistantMSG object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(isShowMessage:) name:@"ktvassistant_isshow_optbtn_tomain_notification" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(disconnRoom:) name:KTVASSISTANT_CLOSEROOM_NOTIFICATION object:nil];
    
    [self flashUIWithRoom];
}

-(void)disconnRoom:(NSNotification *)notification
{
    [self performSelectorOnMainThread:@selector(closeRoom) withObject:nil waitUntilDone:YES];
}

-(void)closeRoom
{
    DisConnectRoom();
    [UserSessionManager clearCurrentRoomInfo];
    enterroombtn.hidden = NO;
    textfield.hidden = YES;
}

-(void)isShowMessage:(NSNotification *)notification{
    NSDictionary * userInfo = [notification userInfo];
    isshowopt = [[userInfo objectForKey:@"show"] intValue];
    if (isshowopt == 0 && imgMessage.hidden == NO) {
        [imgMessage stopAnimating];
        imgMessage.hidden = YES;
        [publicchattable.tableView reloadData];
    }
    else
    {
        [UIView animateWithDuration:.2 animations:^{
            redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
        }];
        redImgView.tag = 0;
    }
}

-(void)showSendMsgResult:(NSNotification *)notification
{
    NSDictionary *userinfo = [notification userInfo];
    NSString *type = [userinfo objectForKey:@"type"];
    if ( [type isEqualToString:@"1"]) {
        NSInteger result = [[userinfo objectForKey:@"result"] integerValue];
        switch (result) {
            case ROOM_NORMAL:
            {
                break;
            }
            case ROOM_ERROR_GIFT_BALANCE:		//送礼时余额不足
            {
                [self showErrorAlert:@"余额不足"];
                break;
            }
            case ROOM_ERROR_GIFT_NOTEXIST:		//该礼物不存在
            {
                [self showErrorAlert:@"礼物不存在"];
                break;
            }
            case ROOM_ERROR_GIFT_OFFLINE:		//礼物接收者不在房间内
            {
                [self showErrorAlert:@"对方不在线"];
                break;
            }
        }
    }
    if ( [type isEqualToString:@"0"]) {
        NSInteger result = [[userinfo objectForKey:@"result"] integerValue];
        if (result == -1) {
            [self showErrorAlert:@"对方不在线"];
        }
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void)creatNoMessageLabel
{
    if (_noMessageLabel == nil)
    {
        _noMessageLabel = [[UILabel alloc] initWithFrame:CGRectMake(112, 230, 250, 35)];
        _noMessageLabel.backgroundColor = [UIColor clearColor];
        _noMessageLabel.text = @"您尚未有消息哟!";
        _noMessageLabel.font = [UIFont systemFontOfSize:15.0f];
        _noMessageLabel.textColor = UIColorFromRGB(0x9e9d9d);
        _noMessageLabel.shadowColor = [UIColor whiteColor];
        _noMessageLabel.shadowOffset = CGSizeMake(0, 1);
        _noMessageLabel.hidden = YES;
        [self.mainContentView addSubview:_noMessageLabel];
    }
}

#pragma mark -  UIActionSheet 点击头像送礼 私聊

- (void)imgHead_lblName_Click:(NSNotification *)notification
{
    NSDictionary * userInfo = [notification userInfo];
    NSInteger tag = [[userInfo objectForKey:@"tag"] intValue];
    name = [userInfo objectForKey:@"username"];
    idx = [[userInfo objectForKey:@"userid"] intValue];
    
    if (tag == 0) {//私聊
        if (![UserSessionManager isRoomAlreadyLogin])   // 未进入包厢
        {
            enterroombtn.hidden = YES;
            textfield.hidden = NO;
        }
        _chattype = 1;
        [textfield becomeFirstResponder];
        NSString *str1 = @"对";
        NSString *str = [str1 stringByAppendingString:name];
        NSString *str2 = @"说：";
        str = [str stringByAppendingString:str2];
        textfield.placeholder = str;
    }
    else if(tag == 1){//送礼
        [textfield resignFirstResponder];
        NSString *uid = [NSString stringWithFormat:@"%d",idx];
        NSDictionary * userInfo = [NSDictionary dictionaryWithObjectsAndKeys:name,@"username",uid,@"useridx", nil];
        [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_GIFT_SENDTOSOMEONE_NOTIFICAITON object:nil userInfo:userInfo];
        [self showGift:idx :name];
    }
    else{//红包
        RedPresonEditView *redpersonView = [[RedPresonEditView alloc] init];
        redpersonView.useridx = idx;
        [textfield resignFirstResponder];
        
        if (scroll.hidden == NO) {
            CGFloat duration = 0.2;
            CGRect inputFieldRect = edtView.frame;
            inputFieldRect.origin.y = scroll.frame.origin.y + 122;
            [UIView animateWithDuration:duration animations:^{
                edtView.frame = inputFieldRect;
            }];
            [self hideFaceScroll];
            sendbtn.hidden = YES;
        }
        redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
        redImgView.tag = 0;
        [self.navigationController pushViewController:redpersonView animated:YES];
    }
    isshowopt = 0;
}

- (void)createOnlineBtn{
    UIButton *onlinepBtn = [[UIButton alloc]initWithFrame:CGRectMake(_MainScreen_Width-36, self.navView.frame.size.height-15-20, 25, 25)];
    [onlinepBtn setBackgroundColor:[UIColor clearColor]];
    UIImage *broadImageNormal = [UIImage imageNamed:@"menu_icon_online.png"];
    UIImage *broadImage = [UIImage imageNamed:@"menu_icon_online.png"];
    [onlinepBtn setBackgroundImage:(broadImageNormal) forState:UIControlStateNormal];
    [onlinepBtn setBackgroundImage:broadImage forState:UIControlStateHighlighted];
    [onlinepBtn addTarget:self action:@selector(onlinepBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [self.navView addSubview:onlinepBtn];

}
#pragma mark - 创建红包对话框
- (void)createRedOpt{
    blackView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, _MainScreen_Height+20)];
    blackView.backgroundColor = [UIColor colorWithWhite:0.0f alpha:0.8f];;
    blackView.userInteractionEnabled = YES;
    [self.view addSubview:blackView];
    
    //红包背景
    redOptView = [[UIImageView alloc] init];
    redOptView.backgroundColor = [UIColor clearColor];
    redOptView.userInteractionEnabled = YES;
    [blackView addSubview:redOptView];
    
    //关闭按钮
    closeBtn = [[UIButton alloc] init];
    [closeBtn addTarget:self action:@selector(closebtnClick:) forControlEvents:UIControlEventTouchUpInside];
    closeBtn.frame = CGRectMake(_MainScreen_Width-50, -30, 20, 20);
    [closeBtn setImage:[UIImage imageNamed:@"red_off_btn_0"] forState:UIControlStateNormal];
    [closeBtn setImage:[UIImage imageNamed:@"red_off_btn_1"] forState:UIControlStateHighlighted];
    [redOptView addSubview:closeBtn];
    
    closeBtnVie = [[UIButton alloc] init];
    [closeBtnVie addTarget:self action:@selector(closebtnClick:) forControlEvents:UIControlEventTouchUpInside];
    closeBtnVie.frame = CGRectMake(_MainScreen_Width-75, 12.5, 22.5, 22.5);
    [closeBtnVie setImage:[UIImage imageNamed:@"shutdown_btn_0"] forState:UIControlStateNormal];
    [closeBtnVie setImage:[UIImage imageNamed:@"shutdown_btn_1"] forState:UIControlStateHighlighted];
    [redOptView addSubview:closeBtnVie];
    
    //头像
    headimg = [[UIImageView alloc] init];
    headimg.layer.masksToBounds = YES;
    [headimg setImageWithURL:[NSURL URLWithString:@""] placeholderImage:[UIImage imageNamed:@"userFace_normal_b.png"]];
    [redOptView addSubview:headimg];
    
    //名字
    lblName = [[UILabel alloc] init];
    lblName.backgroundColor = [UIColor clearColor];
    lblName.font = [UIFont systemFontOfSize:15.0f];
    [redOptView addSubview:lblName];
    
    //备注
    lblRedmark = [[UILabel alloc] init];
    [lblRedmark setTextColor:UIColorFromRGB(0xf6c5c4)];
    [redOptView addSubview:lblRedmark];
    
    //太晚了！！
    mUiLate = [[UILabel alloc] init];
    mUiLate.text = @"太晚了，红包已经被小伙伴瓜分完啦";
    mUiLate.font = [UIFont systemFontOfSize:14.0f];
    CGSize size = [mUiLate.text sizeWithFont:[UIFont systemFontOfSize:14.0f] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
    mUiLate.frame = CGRectMake((_MainScreen_Width-45-size.width)/2, (_MainScreen_Height-84-84)*5/8, size.width, size.height);
    [mUiLate setTextColor:[UIColor whiteColor]];
    [mUiLate setTextAlignment:NSTextAlignmentCenter];
    [redOptView addSubview:mUiLate];
    
    //打开View
    openimgview = [[UIImageView alloc] init];
    openimgview.backgroundColor = [UIColor clearColor];
    openimgview.layer.cornerRadius = 45.5;
    openimgview.userInteractionEnabled = YES;
    openimgview.layer.masksToBounds = YES;
    [openimgview setImage:[UIImage imageNamed:@"rotation_btn_1"]];
    openimgview.hidden = YES;
    [redOptView addSubview:openimgview];
    
    //拆红包按钮
    openBtn = [[UIButton alloc] init];
    [openBtn setBackgroundImage:[UIImage imageNamed:@"chai-13"] forState:UIControlStateNormal];
    openBtn.showsTouchWhenHighlighted = YES;
    openBtn.layer.cornerRadius = 45;
    [openBtn setTitle:@"拆红包" forState:UIControlStateNormal];
    openBtn.titleLabel.font = [UIFont systemFontOfSize:18.0f];
    openBtn.layer.masksToBounds = YES;
    openBtn.tag = 0;
    [openBtn addTarget:self action:@selector(openbtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [redOptView addSubview:openBtn];
    
    //查看详情按钮
    size = [@"查看领取详情>" sizeWithFont:[UIFont systemFontOfSize:14.0f] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
    detailBtn = [[UIButton alloc] initWithFrame:CGRectMake((_MainScreen_Width-45-size.width)/2-6, (_MainScreen_Height-84-84)*677/800, size.width+10, 21)];
    detailBtn.backgroundColor = [UIColor clearColor];
    [detailBtn setTitle:@"查看领取详情>" forState:UIControlStateNormal];
    [detailBtn setTitleColor:UIColorFromRGB(0xffe13b) forState:UIControlStateNormal];
    detailBtn.titleLabel.font = [UIFont systemFontOfSize:14.0f];
    detailBtn.tag = 1;
    [detailBtn addTarget:self action:@selector(openbtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [redOptView addSubview:detailBtn];

    blackView.hidden = YES;
    [self.view sendSubviewToBack:redOptView];
}

#pragma mark -关闭红包对话框
- (void)closebtnClick:(id)sender{
    if(mIsClosing)
        return ;
    mIsClosing = YES;
    
    redOptView.transform = CGAffineTransformMakeScale(1.0f, 1.0f);//将要显示的view按照正常比例显示出来
    [UIImageView beginAnimations:nil context:UIGraphicsGetCurrentContext()];
    [UIImageView setAnimationCurve:UIViewAnimationCurveEaseInOut];  //InOut 表示进入和出去时都启动动画
    [UIImageView setAnimationDuration:0.5f];//动画时间
    redOptView.transform=CGAffineTransformMakeScale(0.01f, 0.01f);//先让要显示的view最小直至消失
    [UIImageView commitAnimations]; //启动动画
    [self performSelector:@selector(redAnimation:) withObject:nil afterDelay:0.5];
}
    
-(void)redAnimation:(id)sender{
    [openimgview removeRotate360];
    openimgview.hidden = YES;
    blackView.hidden = YES;
    redOptView.transform = CGAffineTransformMakeScale(1.0f, 1.0f);
    [self.view sendSubviewToBack:redOptView];
    mIsClosing = false;
}

#pragma mark - 拆红包跟查看详情的action
- (void)openbtnClick:(id)sender{
    UIButton *btn = (UIButton *)sender;
    
    //拆红包
    if (btn.tag == 0) {
        openBtn.hidden = YES;
        openimgview.hidden = NO;
        
        [openimgview rotate360WithDuration:2 repeatCount:2000 timingMode:i7Rotate360TimingModeLinear];
        RequestVieHB([redid UTF8String]);
    }
    //查看红包详情
    else
    {
        [self closebtnClick:nil];
        [SVProgressHUD showWithMaskType:SVProgressHUDMaskTypeClear];
        RequestHBDetail([redid UTF8String]);
    }
}

- (void)createMessageBtn{
    imgMessage = [[UIImageView alloc] initWithFrame:CGRectMake(290, self.mainContentView.frame.size.height-87, 26, 34)];
    imgMessage.image = [UIImage imageNamed:@"instruction_ic_0"];
    imgMessage.backgroundColor = [UIColor clearColor];
    imgMessage.userInteractionEnabled = YES;
    UITapGestureRecognizer *singleTap = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onClickMessage)];
    [imgMessage addGestureRecognizer:singleTap];
    imgMessage.hidden = YES;
    NSMutableArray  *arrayM=[NSMutableArray array];
    [arrayM addObject:[UIImage imageNamed:@"instruction_ic_1"]];
    [arrayM addObject:[UIImage imageNamed:@"instruction_ic_2"]];
    [arrayM addObject:[UIImage imageNamed:@"instruction_ic_3"]];
    [self.mainContentView addSubview:imgMessage];
    //设置动画数组
    [imgMessage setAnimationImages:arrayM];
    //设置动画播放次数
    [imgMessage setAnimationRepeatCount:0];
    //设置动画播放时间
    [imgMessage setAnimationDuration:0.8];
}

-(void)onClickMessage{
    [self flashTable:true];
    [imgMessage stopAnimating];
    imgMessage.hidden = YES;
    isshowopt = 0;
    
    NSDictionary * userInfo;
    userInfo = [NSDictionary dictionaryWithObjectsAndKeys:nil];
    [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_ISSHOW_CHAT_GIFT_RED_CLICK_NOTIFICATION object:nil userInfo:userInfo];
}

- (void)onlinepBtnClick:(id)sender
{
    OnlinePersonView *onlineview = [[OnlinePersonView alloc] init];
    
    [textfield resignFirstResponder];
    
    if (scroll.hidden == NO) {
        CGFloat duration = 0.2;
        CGRect inputFieldRect = edtView.frame;
        inputFieldRect.origin.y = scroll.frame.origin.y + 122;
        [UIView animateWithDuration:duration animations:^{
            edtView.frame = inputFieldRect;
        }];
        [self hideFaceScroll];
        sendbtn.hidden = YES;
    }
    redImgView.frame = CGRectMake(_MainScreen_Width-25, _MainScreen_Height-22, 0, 0);
    redImgView.tag = 0;
    if (isshowopt == 1) {
        isshowopt = 0;
        NSDictionary * userInfo;
        userInfo = [NSDictionary dictionaryWithObjectsAndKeys:nil];
        [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_ISSHOW_CHAT_GIFT_RED_CLICK_NOTIFICATION object:nil userInfo:userInfo];
        if (imgMessage.hidden == NO) {
            [imgMessage stopAnimating];
            imgMessage.hidden = YES;
            [self flashTable:YES];
        }
    }
    [self.navigationController pushViewController:onlineview animated:YES];
}

@end
