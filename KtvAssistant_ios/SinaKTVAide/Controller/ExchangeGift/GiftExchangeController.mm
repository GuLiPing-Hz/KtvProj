//
//  GiftExchangeController.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "GiftExchangeController.h"
#import "KTVAppDelegate.h"
#import "ExchangeHistoryController.h"
#import "CustomListBox.h"
#import "PKtvAssistantAPI.h"
#import "UserCenterViewController.h"
#import "DDMenuController.h"

@interface GiftExchangeController ()

@end

@implementation GiftExchangeController

-(id)init:(KTVGiftInfo *)_info  ktvid:(NSString *)ktvid
{
    self = [super init];
    if (self) {
        _gift = _info;
        giftKtvID = ktvid;
    }
    return  self;
}

-(void)ShowHistory
{
    ExchangeHistoryController *controller = [[ExchangeHistoryController alloc] init];
    [self.navigationController pushViewController:controller animated:true];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.view.backgroundColor = UIColorFromRGB(0xf8f8f9);
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.navView.titleLabel.text = @"确认兑换";
    self.navView.btnHistory.hidden = NO;
    [self.navView.btnHistory addTarget:self action:@selector(ShowHistory) forControlEvents:UIControlEventTouchUpInside];
    giftNum = 1;
    self.view.backgroundColor = UIColorFromRGB(0xf8f8f9);
    
    _recognzier = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(ListBoxHide)];
    [self.mainContentView addGestureRecognizer:_recognzier];
    _recognzier.delegate = self;
    
    [self CreateContent];
   
}
#pragma mark tap Delegate
-(BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch
{
    CGPoint location = [touch locationInView:self.mainContentView];
    if (CGRectContainsPoint(_listBox.frame, location)) {
        return  false;
    }
    return  true;
    
}

#pragma mark listbox delegate

-(void)ListBoxClick:(id)_boxView key:(NSString *)_key value:(NSString *)_value
{
    _lblGiftNum.text = _key;
    _lblGiftCost.text = [[NSString alloc] initWithFormat:@"%d", [_value integerValue] * _gift.giftGold,nil];
    giftNum = [_value integerValue];
}

-(void)CreateListBox
{
    NSMutableArray *keys = [[NSMutableArray alloc]init];
    NSMutableArray *values = [[NSMutableArray alloc]init];
    for (NSInteger i = 1; i<= 10; i++) {
        [keys addObject:[[NSString alloc] initWithFormat:@"%d",i,nil ]];
        [values addObject:[[NSString alloc] initWithFormat:@"%d",i,nil ]];
    }
    
    _listBox = [[CustomListBox alloc] initWithFrame:CGRectMake(85, 94, 60, 210)];
    _listBox.delegate = self;
    [_listBox setData:keys values:values];
    [self.mainContentView addSubview:_listBox];
}

-(void)ListBoxBtnClick
{
    [_listBox ShowOrHide];
}

-(void)ListBoxHide
{
    [_listBox Hide];
}

-(void)CreateContent
{
    _imgGift = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, self.mainContentView.frame.size.width, 240)];
    [_imgGift  setImageWithURL:[NSURL URLWithString:_gift.giftBigImg]  placeholderImage:[UIImage imageWithData:[NSData dataWithContentsOfURL:[NSURL URLWithString:_gift.giftSmallImg]]]];
    _imgGift.backgroundColor = [UIColor clearColor];
    [self.mainContentView addSubview: _imgGift];
    
    _contentView = [[UIView alloc] initWithFrame:CGRectMake(-1, 240, self.mainContentView.frame.size.width + 2, 176)];
    _contentView.backgroundColor = [UIColor whiteColor];
    _contentView.layer.borderWidth = 1;
    _contentView.layer.borderColor = UIColorFromRGB(0xc6c6c7).CGColor;
    [self.mainContentView addSubview:_contentView];
    
    _lblGiftName = [[UILabel alloc] initWithFrame:CGRectMake(19, 11, 65, 16)];
    _lblGiftName.font = [UIFont systemFontOfSize:16];
    _lblGiftName.backgroundColor = [UIColor clearColor];
    _lblGiftName.textColor = [UIColor blackColor];
    _lblGiftName.text = _gift.giftName;
    [_lblGiftName sizeToFit];
    [_contentView addSubview: _lblGiftName];
    
    UIImageView *line = [[UIImageView alloc] initWithFrame:CGRectMake(15, 41, _contentView.frame.size.width -30, 1)];
    line.backgroundColor = UIColorFromRGB(0xc6c6c7);
    [_contentView addSubview:line];
    
    UILabel *lblNumTip = [[UILabel alloc] initWithFrame:CGRectMake(19, 70, 65, 15)];
    lblNumTip.text = @"兑换数量:";
    lblNumTip.font = [UIFont systemFontOfSize:15];
    lblNumTip.textColor = UIColorFromRGB(0x666666);
    lblNumTip.backgroundColor = [UIColor clearColor];
    [_contentView addSubview:lblNumTip];
    
    UIButton *viewBtn = [[UIButton alloc] initWithFrame:CGRectMake(85, 64, 60, 30)];
    viewBtn.layer.borderWidth = 1;
    viewBtn.layer.cornerRadius = 2;
    viewBtn.layer.borderColor = UIColorFromRGB(0xc6c6c7).CGColor;
    viewBtn.backgroundColor = [UIColor clearColor];
    [viewBtn addTarget:self action:@selector(ListBoxBtnClick) forControlEvents:UIControlEventTouchUpInside];
    [_contentView addSubview:viewBtn];
    
    _lblGiftNum = [[UILabel alloc] initWithFrame:CGRectMake(1, 1, 32, 28)];
    _lblGiftNum.text = @"1";
    _lblGiftNum.textAlignment = NSTextAlignmentCenter;
    _lblGiftNum.backgroundColor = [UIColor clearColor];
    _lblGiftNum.textColor = UIColorFromRGB(0xe4397d);
    [viewBtn addSubview:_lblGiftNum];
    
    _imgGiftNum = [[UIImageView alloc] initWithFrame:CGRectMake(35, 1, 24, 28)];
    _imgGiftNum.backgroundColor =  UIColorFromRGB(0xe4397d);
    _imgGiftNum.layer.cornerRadius = 2;
    _imgGiftNum.contentMode = UIViewContentModeCenter;
    [_imgGiftNum setImage:[UIImage imageNamed:@"droplist_down_arrow.png"]];
    [viewBtn addSubview:_imgGiftNum];
    
    UILabel *lblGoldTip = [[UILabel alloc] initWithFrame:CGRectMake(179, 70, 65, 15)];
    lblGoldTip.text = @"所需金币:";
    lblGoldTip.font = [UIFont systemFontOfSize:15];
    lblGoldTip.textColor = UIColorFromRGB(0x666666);
    lblGoldTip.backgroundColor = [UIColor clearColor];
    [_contentView addSubview:lblGoldTip];
    
    _lblGiftCost =  [[UILabel alloc] initWithFrame:CGRectMake(245, 64, 60, 30)];
    _lblGiftCost.text = [[NSString alloc] initWithFormat:@"%d",giftNum * _gift.giftGold,nil];
    _lblGiftCost.backgroundColor = [UIColor clearColor];
    _lblGiftCost.textColor = UIColorFromRGB(0xe4397d);
    _lblGiftCost.layer.borderWidth = 1;
    _lblGiftCost.layer.cornerRadius = 2;
    _lblGiftCost.layer.borderColor = UIColorFromRGB(0xc6c6c7).CGColor;
    _lblGiftCost.textAlignment = NSTextAlignmentCenter;
    [_contentView addSubview:_lblGiftCost];
    
    _btnExchange = [UIButton buttonWithType:UIButtonTypeCustom];
    [_btnExchange setTitle:@"确认兑换" forState:UIControlStateNormal];
    [_btnExchange setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];    _btnExchange.frame = CGRectMake(18, 120, _contentView.frame.size.width-36, 35);
    _btnExchange.backgroundColor = UIColorFromRGB(0xe4397d);
    _btnExchange.layer.cornerRadius = 5;
    _btnExchange.layer.masksToBounds = YES;
    _btnExchange.titleLabel.font = [UIFont systemFontOfSize:15];
    //[_btnExchange addTarget:self action:@selector(ChangeBtnBg) forControlEvents:UIControlEventTouchDown];
    [_btnExchange addTarget:self action:@selector(ExchangeGift) forControlEvents:UIControlEventTouchUpInside];
    [_contentView addSubview:_btnExchange];
    
    [self CreateListBox];
}

-(void)ChangeBtnBg
{
    _btnExchange.backgroundColor = UIColorFromRGB(0xd4296d);
}


-(void)ExchangeGift
{
    _btnExchange.backgroundColor = UIColorFromRGB(0xe4397d);
    _confirmExchange  = [[UIAlertView alloc] initWithTitle:nil message:[[NSString alloc] initWithFormat:@"确认用%d金币兑换%d个%@",giftNum * _gift.giftGold,giftNum,_gift.giftName,nil] delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"兑换", nil];
    [_confirmExchange show];
}

#pragma mark alert delegate
-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 1) {
        [SVProgressHUD showWithStatus:@"正在兑换"];
        NSString *strUrl = [PKtvAssistantAPI getGetGiftExchangeUrl:[UserSessionManager getInstance].currentUser.uid giftID:_gift.giftID ktvID:
                            giftKtvID num:giftNum];
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
                //NSString* s_money = [html objectForKey:@"result"];
                long money = [[html objectForKey:@"result"] intValue];
                [CommenTools showAlertViewWithMessage: @"兑换成功,请用兑换码去前台兑换(兑换码在兑换记录查看)"];
                
                [[UserSessionManager getInstance] updateCurrentUserGold: money];
                ExchangeHistoryController *controller =[[ExchangeHistoryController alloc] init];
                [self.navigationController pushViewController:controller animated:true];
                [SVProgressHUD dismiss];
            }
            else
            {
                [CommenTools showAlertViewWithMessage: [NSString stringWithFormat:@"兑换失败:%@",msg]];
                [SVProgressHUD dismiss];
            }
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            [SVProgressHUD dismiss];
        }];
        [operation start];
    }
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


@end
