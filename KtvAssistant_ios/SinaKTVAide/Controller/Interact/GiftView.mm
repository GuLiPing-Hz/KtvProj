//
//  GiftView.m
//  SinaKTVAide
//
//  Created by cxp on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "GiftView.h"
#import "KTVBaseConfig.h"
#import "UserSessionManager.h"
#import "RechargeController.h"
#import "InteractController.h"
#import "NetUtilCallBack.h"
#import "com_ios_NetUtil.h"
#import "PublicChatCell.h"

@implementation GiftView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self createGiftView];
        [self createGiftScroll];
        [self setDefaultBtn:normalbtn];
        [self CreateListBox];
        [self CreatePeopleListBox];
    }
    return self;
}
-(void) GiftClick:(NSInteger)giftID :(NSInteger)price
{
    giftid = giftID;
    giftprice = price;
}
-(void)createGiftScroll
{
    view1 = [[GiftScrollView alloc] initWithFrame:CGRectMake(0, 35, _MainScreen_Width, 155) type:1];//幸运
    [giftView addSubview: view1];
    view1.delegate = self;
    view1.hidden = YES;
    
    view2 = [[GiftScrollView alloc] initWithFrame:CGRectMake(0, 35, _MainScreen_Width, 155) type:2];//普通
    [giftView addSubview: view2];
    view2.delegate = self;
    view2.hidden = NO;
    
    view3 = [[GiftScrollView alloc] initWithFrame:CGRectMake(0, 35, _MainScreen_Width, 155) type:3];//奢侈
    [giftView addSubview: view3];
    view3.delegate = self;
    view3.hidden = YES;
}

- (void)createGiftView
{
    giftView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 279)];
    giftView.backgroundColor = UIColorFromRGB(0xf6f6f7);
    [self addSubview:giftView];
    UIScrollView *giftScroll = [[UIScrollView alloc]initWithFrame:CGRectMake(0, 36, _MainScreen_Width+1, 156)];
    giftScroll.layer.borderColor = [UIColorFromRGB(0xe5e5e5) CGColor];
    giftScroll.layer.borderWidth = 1;
    [giftView addSubview:giftScroll];
    
    UILabel *lblgold = [[UILabel alloc]initWithFrame:CGRectMake(9, 203, 70, 14)];
    lblgold.text = @"当前金币：";
    lblgold.font = [UIFont systemFontOfSize:14.0f];
    lblgold.textColor = UIColorFromRGB(0x666666);
    lblgold.backgroundColor = [UIColor clearColor];
    [giftView addSubview:lblgold];
    
    lblprice = [[UILabel alloc] init];
    mygold = [UserSessionManager getInstance].currentUser.gold;
    lblprice.text = [[NSString alloc] initWithFormat:@"%ld",mygold,nil];
    CGSize size = [lblprice.text sizeWithFont:[UIFont systemFontOfSize:14] constrainedToSize:CGSizeMake(MAXFLOAT, 203)];
    lblprice.frame = CGRectMake(80, 202, size.width, size.height);
    lblprice.font = [UIFont systemFontOfSize:14.0f];
    lblprice.textColor = UIColorFromRGB(0xe4397d);
    lblprice.backgroundColor = [UIColor clearColor];
    [giftView addSubview:lblprice];
    
    UILabel *lblnum = [[UILabel alloc]initWithFrame:CGRectMake(10, 237, 100, 14)];
    lblnum.text = @"数量：";
    lblnum.font = [UIFont systemFontOfSize:14.0f];
    lblnum.textColor = UIColorFromRGB(0x666666);
    [giftView addSubview:lblnum];
    
    num = [[UILabel alloc]initWithFrame:CGRectMake(46, 229, 65, 30)];
    num.layer.borderColor = [UIColorFromRGB(0xe5e5e5) CGColor];
    num.layer.borderWidth = 1;
    num.font = [UIFont systemFontOfSize:14.0f];
    num.textColor = UIColorFromRGB(0x666666);
    num.layer.cornerRadius = 3;
    num.text = @"  1";
    [giftView addSubview:num];
    
    numbtn = [[UIButton alloc]initWithFrame:CGRectMake(87, 229, 24, 29)];
    numbtn.backgroundColor = UIColorFromRGB(0xe4397d);
    numbtn.layer.cornerRadius = 3;
    numbtn.tag = 0;
    numbtn.contentMode = UIViewContentModeRedraw;
    [numbtn setImage:[UIImage imageNamed:@"droplist_down_arrow.png"] forState:UIControlStateNormal];
    [numbtn addTarget:self action:@selector(ListBoxBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [giftView addSubview:numbtn];
    giftNum = 1;
    
    UILabel *lblsend = [[UILabel alloc]initWithFrame:CGRectMake(117, 237, 20, 14)];
    lblsend.text = @"赠";
    lblsend.font = [UIFont systemFontOfSize:14.0f];
    lblsend.textColor = UIColorFromRGB(0x666666);
    [giftView addSubview:lblsend];
    
    lblwho = [[UILabel alloc]initWithFrame:CGRectMake(137, 229, _MainScreen_Width-91-137+3, 30)];
    lblwho.layer.borderColor = [UIColorFromRGB(0xe5e5e5) CGColor];
    lblwho.layer.borderWidth = 1;
    lblwho.font = [UIFont systemFontOfSize:14.0f];
    lblwho.textColor = UIColorFromRGB(0x666666);
    lblwho.layer.cornerRadius = 3;
    lblwho.text = @" 所有人";
    [giftView addSubview:lblwho];
    
    whobtn = [[UIButton alloc]initWithFrame:CGRectMake(_MainScreen_Width-91, 229, 24, 29)];
    whobtn.backgroundColor = UIColorFromRGB(0xe4397d);
    whobtn.tag = 1;
    whobtn.layer.cornerRadius = 3;
    [whobtn setImage:[UIImage imageNamed:@"droplist_down_arrow.png"] forState:UIControlStateNormal];
    [whobtn addTarget:self action:@selector(ListBoxBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    [giftView addSubview:whobtn];
    
    sendbtn = [[UIButton alloc]initWithFrame:CGRectMake(_MainScreen_Width-63, 228, 55, 31)];
    [sendbtn setTitle:@"赠 送" forState:UIControlStateNormal];
    sendbtn.backgroundColor = UIColorFromRGB(0xe4397d);
    sendbtn.titleLabel.font = [UIFont systemFontOfSize:14.0f];
    sendbtn.layer.cornerRadius = 3;
    [giftView addSubview:sendbtn];
    [sendbtn addTarget:self action:@selector(sendbtnChange:) forControlEvents:UIControlEventTouchUpInside];
    
    chargebtn = [[UIButton alloc]initWithFrame:CGRectMake(_MainScreen_Width-69, 194, 55, 30)];
    [chargebtn setTitle:@"去充值" forState:UIControlStateNormal];
    chargebtn.titleLabel.font = [UIFont systemFontOfSize:14.0f];
    [chargebtn setTitleColor:UIColorFromRGB(0xe4397d) forState:UIControlStateNormal];
    [chargebtn addTarget:self action:@selector(btnAction:) forControlEvents:UIControlEventTouchUpInside];
    [giftView addSubview:chargebtn];
    
    UIImageView *imgGo = [[UIImageView alloc]initWithFrame:CGRectMake(_MainScreen_Width-17, 202, 8, 14)];
    imgGo.image = [UIImage imageNamed:@"list_pic_more.png"];
    [giftView addSubview:imgGo];
    
    
    normalbtn = [[UIButton alloc]initWithFrame:CGRectMake(24, 0, 45, 35)];
    [normalbtn setTitle:@"普通" forState:UIControlStateNormal];
    [normalbtn setTitleColor:UIColorFromRGB(0x666666) forState:UIControlStateNormal];
    [normalbtn setTitleColor:UIColorFromRGB(0xe4397d) forState:UIControlStateHighlighted];
    normalbtn.titleLabel.font = [UIFont systemFontOfSize:14.0f];
    [giftView addSubview:normalbtn];
    
    luckylbtn = [[UIButton alloc]initWithFrame:CGRectMake(92, 0, 45, 35)];
    [luckylbtn setTitle:@"幸运" forState:UIControlStateNormal];
    [luckylbtn setTitleColor:UIColorFromRGB(0x666666) forState:UIControlStateNormal];
    [luckylbtn setTitleColor:UIColorFromRGB(0xe4397d) forState:UIControlStateHighlighted];
    luckylbtn.titleLabel.font = [UIFont systemFontOfSize:14.0f];
    [giftView addSubview:luckylbtn];
    
    explbtn = [[UIButton alloc]initWithFrame:CGRectMake(155, 0, 45, 35)];
    [explbtn setTitle:@"奢侈" forState:UIControlStateNormal];
    [explbtn setTitleColor:UIColorFromRGB(0x666666) forState:UIControlStateNormal];
    [explbtn setTitleColor:UIColorFromRGB(0xe4397d) forState:UIControlStateHighlighted];
    explbtn.titleLabel.font = [UIFont systemFontOfSize:14.0f];
    [giftView addSubview:explbtn];
    
    sepnormal = [[UIImageView alloc] initWithFrame:CGRectMake(25, 35, 44, 2)];
    sepnormal.backgroundColor = UIColorFromRGB(0xe4397d);
    [giftView addSubview:sepnormal];
    sepnormal.hidden = NO;
    
    seplucky = [[UIImageView alloc] initWithFrame:CGRectMake(93, 35, 44, 2)];
    seplucky.backgroundColor = UIColorFromRGB(0xe4397d);
    [giftView addSubview:seplucky];
    seplucky.hidden = YES;
    
    sepexp = [[UIImageView alloc] initWithFrame:CGRectMake(156, 35, 44, 2)];
    sepexp.backgroundColor = UIColorFromRGB(0xe4397d);
    [giftView addSubview:sepexp];
    sepexp.hidden = YES;
    
    [normalbtn addTarget:self action:@selector(normalbtnChange:) forControlEvents:UIControlEventTouchUpInside];
    [luckylbtn addTarget:self action:@selector(luckybtnChange:) forControlEvents:UIControlEventTouchUpInside];
    [explbtn addTarget:self action:@selector(expbtnChange:) forControlEvents:UIControlEventTouchUpInside];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(delegateUpdatemoney:) name:@"ktvassistantUpdatemoney" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(delegateSendGift:) name:KTVASSISTANT_GIFT_SENDTOSOMEONE_NOTIFICAITON object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(UpdateGold) name:KTVASSISTANT_GOLD_UPDATE_NOTIFICATION object:nil];//刷新金币值
    
}

- (void)delegateSendGift:(NSNotification *)notification
{
    NSDictionary * userInfo = [notification userInfo];
    NSString * username = [userInfo objectForKey:@"username"];
    NSString * userid = [userInfo objectForKey:@"useridx"];
    lblwho.text = username;
    idx = [userid intValue];
}

- (void)delegateUpdatemoney:(NSNotification *)notification
{
    NSDictionary * userInfo = [notification userInfo];
    NSString * newgold = [userInfo objectForKey:@"gold"];
    lblprice.text = newgold;
}


- (UIViewController*)viewController {
    for (UIView* next = [self superview];next;next = next.superview)
    {
        UIResponder* nextResponder = [next nextResponder];
        if ([nextResponder isKindOfClass:[UIViewController class]]) {
            return (UIViewController*)nextResponder;
        }
    }
    return nil;
}

-(void)sendbtnChange:(id)sender
{
    if (![NetUtilCallBack getInstance]->m_bConnectedLobby) {
        [SVProgressHUD showWithTips:@"网络出现问题！"];
        return ;
    }
    
    if (giftNum*giftprice > mygold) {
        //UIAlertView *noNetworkAlert = [[UIAlertView alloc] initWithTitle:nil message:@"您的金币不足哦，请充值后再送！" delegate:self cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
        //[noNetworkAlert show];
        [SVProgressHUD showWithTips:@"您的金币不足哦，请充值后再送！"];
        return;
    }
    if (idx == 0) {
        [SVProgressHUD showWithTips:@"请在‘身边’栏里选择对象，然后赠送礼物哦！"];
        return;
    }
    
    if (giftid == 0) {
        [SVProgressHUD showWithTips:@"请选择礼物！"];
        return;
    }
    SendGiftInLobby(idx, giftid, giftNum, [NetUtilCallBack getInstance]->m_roomId);
}
//充值
-(void)btnAction:(id)sender
{
    RechargeController *rechange = [[RechargeController alloc] init];
    [[self viewController].navigationController pushViewController:rechange animated:YES];
    
}

-(void)ListBoxBtnClick:(id) sender
{
    UIButton *btn = (UIButton *)sender;
    if (btn.tag == 0)
        [_listBox ShowOrHide];
    else
        [_peoplelistBox ShowOrHide];
}

-(void)ListBoxClick:(id)_boxView key:(NSString *)_key value:(NSString *)_value
{
    if (_boxView == _listBox) {
        NSString *str = @" ";
        str = [str stringByAppendingString:_key];
        num.text = str;
        giftNum = [_key integerValue];
    }
    else
    {
        lblwho.text = _key;
        idx = [_value intValue];
    }
}

-(void)CreatePeopleListBox
{
    userName = [[NSMutableArray alloc]init];
    useridx = [[NSMutableArray alloc]init];
    _peoplelistBox = [[CustomListBox alloc] initWithFrame:CGRectMake(136, 69, 112, 161)];
    _peoplelistBox.delegate = self;
    [giftView addSubview:_peoplelistBox];
}

-(void)CreateListBox
{
    NSMutableArray *keys = [[NSMutableArray alloc]init];
    NSMutableArray *values = [[NSMutableArray alloc]init];

    [keys addObject:[[NSString alloc] initWithFormat:@"%d",1,nil ]];
    [values addObject:[[NSString alloc] initWithFormat:@"%d",1,nil ]];
    [keys addObject:[[NSString alloc] initWithFormat:@"%d",38,nil ]];
    [values addObject:[[NSString alloc] initWithFormat:@"%d",2,nil ]];
    [keys addObject:[[NSString alloc] initWithFormat:@"%d",88,nil ]];
    [values addObject:[[NSString alloc] initWithFormat:@"%d",3,nil ]];
    [keys addObject:[[NSString alloc] initWithFormat:@"%d",99,nil ]];
    [values addObject:[[NSString alloc] initWithFormat:@"%d",4,nil ]];
    [keys addObject:[[NSString alloc] initWithFormat:@"%d",888,nil ]];
    [values addObject:[[NSString alloc] initWithFormat:@"%d",5,nil ]];
    [keys addObject:[[NSString alloc] initWithFormat:@"%d",999,nil ]];
    [values addObject:[[NSString alloc] initWithFormat:@"%d",6,nil ]];
    [keys addObject:[[NSString alloc] initWithFormat:@"%d",1314,nil ]];
    [values addObject:[[NSString alloc] initWithFormat:@"%d",7,nil ]];
    
    _listBox = [[CustomListBox alloc] initWithFrame:CGRectMake(48, 69, 60, 161)];
    _listBox.delegate = self;
    [_listBox setData:keys values:values];
    [giftView addSubview:_listBox];
    
    UITapGestureRecognizer *numbox_tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(numboxTouchDown)];
    numbox_tapGesture.cancelsTouchesInView = NO;
    [giftView addGestureRecognizer:numbox_tapGesture];
}


-(void)addSendTarge:(NSString *)name index:(NSInteger)index
{
    [userName addObject:name];
    if(![useridx containsObject:[NSString stringWithFormat:@"%d",index]])
        [useridx addObject:[NSString stringWithFormat:@"%d",index]];
    [_peoplelistBox setData:userName values:useridx];
}

-(void)numboxTouchDown
{
    [_listBox Hide];
    [_peoplelistBox Hide];
}

- (void)normalbtnChange:(id)sender
{
    [normalbtn setTitleColor:UIColorFromRGB(0xe4397d) forState:UIControlStateNormal];
    [luckylbtn setTitleColor:UIColorFromRGB(0x666666) forState:UIControlStateNormal];
    [explbtn setTitleColor:UIColorFromRGB(0x666666) forState:UIControlStateNormal];
    view1.hidden = YES;
    view2.hidden = NO;
    view3.hidden = YES;
    sepnormal.hidden = NO;
    seplucky.hidden = YES;
    sepexp.hidden = YES;
}
- (void)luckybtnChange:(id)sender
{
    [luckylbtn setTitleColor:UIColorFromRGB(0xe4397d) forState:UIControlStateNormal];
    [normalbtn setTitleColor:UIColorFromRGB(0x666666) forState:UIControlStateNormal];
    [explbtn setTitleColor:UIColorFromRGB(0x666666) forState:UIControlStateNormal];
    view1.hidden = NO;
    view2.hidden = YES;
    view3.hidden = YES;
    sepnormal.hidden = YES;
    seplucky.hidden = NO;
    sepexp.hidden = YES;
}
- (void)expbtnChange:(id)sender
{
    [explbtn setTitleColor:UIColorFromRGB(0xe4397d) forState:UIControlStateNormal];
    [luckylbtn setTitleColor:UIColorFromRGB(0x666666) forState:UIControlStateNormal];
    [normalbtn setTitleColor:UIColorFromRGB(0x666666) forState:UIControlStateNormal];
    view1.hidden = YES;
    view2.hidden = YES;
    view3.hidden = NO;
    sepnormal.hidden = YES;
    seplucky.hidden = YES;
    sepexp.hidden = NO;
}

-(void)setDefaultBtn:(UIButton *) btn
{
    [btn setTitleColor:UIColorFromRGB(0xe4397d) forState:UIControlStateNormal];
}

-(void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

-(void)showMessage
{
    AllInfo info = ([NetUtilCallBack getInstance]->m_vectAllInfo[[NetUtilCallBack getInstance]->m_vectAllInfo.size() - 1]);
    if (info.type != 3 || info.idx != [UserSessionManager getInstance].currentUser.uid)
        return;
    PublicChatCell *cell = [[PublicChatCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
        cell.backgroundView = nil;
        cell.backgroundColor = [UIColor clearColor];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    cell.Info = info;
    UIView *view = (UIView *)cell;
    NSInteger height = [PublicChatCell CountRowHeight:info];
    view.frame = CGRectMake(0, 0 - height - 1, cell.frame.size.width, height);
    [self addSubview:cell];
    [NSTimer scheduledTimerWithTimeInterval:2 target:self selector:@selector(hideMessage:) userInfo:view repeats:NO];
}

-(void)hideMessage:(NSTimer *)timer
{
    UIView *view = (UIView *)[timer userInfo];
    [UIView animateWithDuration:1 animations:^{
        view.alpha = 0;
    } completion:^(BOOL finished) {
        view.hidden = YES;
    }];
}

-(void)UpdateGold
{
    mygold = [UserSessionManager getInstance].currentUser.gold;
    lblprice.text = [[NSString alloc] initWithFormat:@"%ld",mygold,nil];
    [lblprice sizeToFit];
}

@end
