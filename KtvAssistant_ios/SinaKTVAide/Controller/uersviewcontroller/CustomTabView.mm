//
//  CustomTabView.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-16.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "CustomTabView.h"
#import "UserCenterTableView.h"


@implementation CustomTabView

- (id)initWithFrame:(CGRect)frame titles:(NSMutableArray *)titles views:(NSMutableArray *)_views
{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor whiteColor];
        _currentIndex = 0;
        _tabBtn = [[NSMutableArray alloc]init];
        _tableView = _views;
        _titles = titles;
        [self createTab];
        [self setDefault:_tabBtn[0]];
    }
    return self;
}

-(void)createTab
{
    int width = self.frame.size.width;
    _tabHead= [[UIView alloc] initWithFrame:CGRectMake(-1., 0, width + 2, 44)];
    _tabHead.backgroundColor = UIColorFromRGB(0xf8f8f9);
    _tabHead.layer.borderWidth = 1;
    _tabHead.layer.borderColor = [UIColorFromRGB(0xc6c6c7) CGColor];
    [self createTab:0];
    [self createTab:1];
    [self createTab:2];
    [_tabHead bringSubviewToFront:_tabBtn[1]];
    [self addSubview:_tabHead];

}


-(void)createTab:(int)index
{
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    if (index == 1) {
        btn.frame = CGRectMake(index * (_MainScreen_Width-20)/3+5+3, 8, (_MainScreen_Width-20)/3+5, 28);
    }
    else
    {
        btn.layer.cornerRadius = 5;
        btn.frame = CGRectMake(13 + index * (_MainScreen_Width-20)/3-5, 8, (_MainScreen_Width-20)/3+5, 28);
    }
    btn.layer.borderColor = [UIColorFromRGB(0x5d6e6a) CGColor];
    btn.layer.borderWidth = 1;
    btn.tag = index;
    btn.titleLabel.font = [UIFont systemFontOfSize:13];
    [btn setBackgroundColor:UIColorFromRGB(0xf8f8f9)];
    [btn setTitleColor:UIColorFromRGB(0x5d6e6a) forState:UIControlStateNormal];
    [btn setTitle:_titles[index] forState:UIControlStateNormal];
    [btn addTarget:self action:@selector(tabChange:) forControlEvents:UIControlEventTouchUpInside];
    [_tabBtn addObject:btn];
    [_tabHead addSubview:btn];
    [self addSubview: _tableView[index]];
}

-(void)setDefault:(UIButton *) btn
{
    [btn setBackgroundColor:UIColorFromRGB(0x5d6e6a)];
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
    if ([(NSObject *)_delegate respondsToSelector:@selector(TabChange:newIndex:)] == YES)
    {
        [_delegate TabChange:_currentIndex newIndex:btn.tag];
    }
    UIButton *oldBtn = (UIButton *)_tabBtn[_currentIndex];
    [oldBtn setBackgroundColor:UIColorFromRGB(0xf8f8f9)];
    [oldBtn setTitleColor:UIColorFromRGB(0x5d6e6a) forState:UIControlStateNormal];
    [btn setBackgroundColor:UIColorFromRGB(0x5d6e6a)];
    [btn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    ((UIView *)_tableView[btn.tag]).hidden = NO;
    ((UIView *)_tableView[oldBtn.tag]).hidden = YES;
    _currentIndex = btn.tag;
}

/*
 // Only override drawRect: if you perform custom drawing.
 // An empty implementation adversely affects performance during animation.
 - (void)drawRect:(CGRect)rect
 {
 // Drawing code
 }
 */


@end
