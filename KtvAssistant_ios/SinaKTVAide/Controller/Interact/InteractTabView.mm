//
//  InteractTabView.m
//  SinaKTVAide
//
//  Created by cxp on 14-7-17.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "InteractTabView.h"
#import "KTVBaseConfig.h"

@interface InteractTabView()
@property (nonatomic,retain) NSMutableArray    *tabBtn;

@end

@implementation InteractTabView

- (id)initWithFrame:(CGRect)frame titles:(NSMutableArray *)titles views:(NSMutableArray *)_views
{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = UIColorFromRGB(0xffffff);
        _currentIndex = 0;
        _tabBtn = [[NSMutableArray alloc]init];
        _titles = titles;
        _tableView = _views;
        [self createTab];
        [self setDefaultBtn:_tabBtn[0]];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(delegateSendGift:) name:KTVASSISTANT_GIFT_SENDTOSOMEONE_NOTIFICAITON object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(delegateSendChat:) name:@"ktvassistantsendChat" object:nil];
    }
    return self;
}

-(void)createTab
{
    int width = self.frame.size.width;
    UIView *tabHead= [[UIView alloc] initWithFrame:CGRectMake(-1., 0, width + 2, 44)];
    UIView *btnHead = [[UIView alloc] initWithFrame:CGRectMake(20, 7, 280, 28)];
    btnHead.layer.cornerRadius = 5;
    btnHead.backgroundColor = UIColorFromRGB(0xf8f8f9);
    btnHead.layer.borderWidth = 1;
    btnHead.layer.borderColor = [UIColorFromRGB(0x5d616a) CGColor];
    tabHead.backgroundColor = UIColorFromRGB(0xf8f8f9);
    tabHead.layer.borderWidth = 1;
    tabHead.layer.borderColor = [UIColorFromRGB(0xe5e5e5) CGColor];
    [self createTabButton:0];
    [self createTabButton:1];
    [self createTabButton:2];
    [self createTabButton:3];
    [btnHead addSubview:_tabBtn[0]];
    [btnHead addSubview:_tabBtn[3]];
    [btnHead addSubview:_tabBtn[1]];
    [btnHead addSubview:_tabBtn[2]];
    [tabHead addSubview:btnHead];
    [self addSubview:tabHead];
    
}

-(void)createTabButton:(int)index
{
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    if (index == 0) {
        btn.layer.cornerRadius = 5;
        btn.frame = CGRectMake(0, 0, 75, 28);
    }
    else if (index == 1) {
        btn.frame = CGRectMake(70, 0, 75, 28);
    }
    else if (index == 2)
    {
        btn.frame = CGRectMake(140, 0, 70, 28);
    }
    else
    {
        btn.layer.cornerRadius = 5;
        btn.frame = CGRectMake(205, 0, 75, 28);
    }
        
    //btn.layer.borderColor = [UIColorFromRGB(0x5d616a) CGColor];
    btn.layer.borderWidth = 0;
    btn.tag = index;
    btn.titleLabel.font = [UIFont systemFontOfSize:14];
    [btn setBackgroundColor:UIColorFromRGB(0xf8f8f9)];
    [btn setTitleColor:UIColorFromRGB(0x5d616a) forState:UIControlStateNormal];
    [btn setTitle:_titles[index] forState:UIControlStateNormal];
    [btn addTarget:self action:@selector(tabChange:) forControlEvents:UIControlEventTouchUpInside];
    [_tabBtn addObject:btn];
    [self addSubview: _tableView[index]];
}

-(void)setDefaultBtn:(UIButton *) btn
{
    [btn setBackgroundColor:UIColorFromRGB(0x5d616a)];
    [btn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [_tableView[0] setHidden:NO];
    ((UIView *)_tableView[0]).hidden = NO;
}

-(void)tabChange:(id)sender
{
    UIButton *btn = (UIButton *)sender;
    if (btn.tag == _currentIndex) {
        return;
    }
    UIButton *oldBtn = (UIButton *)_tabBtn[_currentIndex];
    [oldBtn setBackgroundColor:UIColorFromRGB(0xf8f8f9)];
    [oldBtn setTitleColor:UIColorFromRGB(0x5d616a) forState:UIControlStateNormal];
    [btn setBackgroundColor:UIColorFromRGB(0x5d616a)];
    [btn setTitleColor:UIColorFromRGB(0xf8f8f9) forState:UIControlStateNormal];
    ((UIView *)_tableView[btn.tag]).hidden = NO;
    ((UIView *)_tableView[oldBtn.tag]).hidden = YES;
    _currentIndex = btn.tag;
}

-(void)changeTabBtn
{
    UIButton *oldBtn = (UIButton *)_tabBtn[_currentIndex];
    [oldBtn setBackgroundColor:UIColorFromRGB(0xf8f8f9)];
    [oldBtn setTitleColor:UIColorFromRGB(0x5d616a) forState:UIControlStateNormal];
    [_tabBtn[0] setBackgroundColor:UIColorFromRGB(0x5d616a)];
    [_tabBtn[0] setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [_tableView[3] setHidden:YES];
    ((UIView *)_tableView[3]).hidden = YES;
    [_tableView[0] setHidden:NO];
    ((UIView *)_tableView[0]).hidden = NO;
    _currentIndex = ((UIButton *)_tabBtn[0]).tag;
}
- (void)delegateSendGift:(NSNotification *)notification
{
    [self changeTabBtn];
}
- (void)delegateSendChat:(NSNotification *)notification
{
    [self changeTabBtn];
}

@end
