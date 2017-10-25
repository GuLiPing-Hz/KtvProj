//
//  PCustomNavigationBarView.m
//  itime
//
//  Created by pig on 13-4-19.
//  Copyright (c) 2013年 pig. All rights reserved.
//

#import "PCustomNavigationBarView.h"
#import "SSImageCategory.h"
#import "UIFont+PFontCategory.h"

@implementation PCustomNavigationBarView

@synthesize bgImageView = _bgImageView;
@synthesize titleLabel = _titleLabel;
@synthesize leftButton = _leftButton;
@synthesize rightButton = _rightButton;
@synthesize backButton = _backButton;

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        
    }
    return self;
}
- (id)initCustomNavigationBarView:(NSString *)tTitle
{
    // 判断版本 ios7就增加导航条的高到44+20=64 ios6导航条的高只有44
    float topdistance = 0;
    
    long losVersion = [[UIDevice currentDevice].systemVersion floatValue] * 10000;
    
    if (losVersion >= 70000)
        topdistance = 20;
    
    self = [super initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 44 + topdistance)];
    
    if (self)
    {
        // 导航条
        [self setBackgroundColor:UIColorFromRGB(0xe4397d)];
        // 导航标题
        _titleLabel = [[UILabel alloc] init];
        [_titleLabel setFrame:CGRectMake((_MainScreen_Width-200)/2, 12 + topdistance, 200, 21)];
        [_titleLabel setText:tTitle];
        [_titleLabel setTextAlignment:NSTextAlignmentCenter];
        [_titleLabel setBackgroundColor:[UIColor clearColor]];
        [_titleLabel setFont:[UIFont fontOfApp:18.0f]];
        [_titleLabel setTextColor:UIColorFromRGB(0xffffff)];
        [self addSubview:_titleLabel];
        
        // 左上角功能按钮
        _leftButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_leftButton setFrame:CGRectMake(5, 7 + topdistance, 33, 30)];
        [_leftButton setHidden:YES];
        [self addSubview:_leftButton];
        
        // 右上角功能按钮
        _rightButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_rightButton setFrame:CGRectMake(_MainScreen_Width-35, 7 + topdistance, 25, 30)];
        [_rightButton setHidden:YES];
        [self addSubview:_rightButton];
        
        // 返回按钮
        _backButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_backButton setFrame:CGRectMake(5, 7 + topdistance, 25, 30)];
        [_backButton setHidden:YES];
        [self addSubview:_backButton];
        
        //兑换记录按钮
        _btnHistory = [UIButton buttonWithType:UIButtonTypeCustom];
        _btnHistory.frame  = CGRectMake(_MainScreen_Width-60, 14 + topdistance, 50, 24);
        _btnHistory.titleLabel.font = [UIFont systemFontOfSize:12];
        _btnHistory.titleLabel.textColor = [UIColor whiteColor];
        _btnHistory.backgroundColor = [UIColor clearColor];
        [_btnHistory setTitle:@"兑换记录" forState:UIControlStateNormal];
        [_btnHistory setHidden:YES];
        [self addSubview:_btnHistory];
    }
    
    return self;
}

- (id)initWithTitle:(NSString *)tTitle
{
    self = [super initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 44)];
    if (self)
    {
        [self setBackgroundColor:UIColorFromRGB(0xe4397d)];
        _titleLabel = [[UILabel alloc] init];
        [_titleLabel setFrame:CGRectMake((_MainScreen_Width-200)/2, 12, 200, 21)];
        [_titleLabel setText:tTitle];
        [_titleLabel setTextAlignment:NSTextAlignmentCenter];
        [_titleLabel setBackgroundColor:[UIColor clearColor]];
        [_titleLabel setFont:[UIFont fontOfApp:20.0f]];
        [_titleLabel setTextColor:[UIColor whiteColor]];
        [_titleLabel setShadowOffset:CGSizeMake(0, 1)];
        [self addSubview:_titleLabel];
        
        _leftButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_leftButton setFrame:CGRectMake(5, 6, 22.5f, 17.0f)];
        [_leftButton setHidden:YES];
        [self addSubview:_leftButton];
        
        _rightButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_rightButton setFrame:CGRectMake(_MainScreen_Width-58, 6, 53, 31)];
        [_rightButton setHidden:YES];
        [self addSubview:_rightButton];
    }
    
    return self;
}

-(void)initIndexButton
{
    [self.titleLabel setText:@"新浪KTV助手"];
    [self setHidden:NO];
    [self.leftButton setHidden:NO];
}
-(void)initCommonBackButton:(NSString*)tittle isInRoom:(BOOL)is
{
    [self.titleLabel setText:tittle];
    [self setHidden:NO];
    [self.backButton setHidden:NO];
    if(is)
        [self.rightButton setHidden:NO];
}
-(void)initOnlyBackButton:(NSString*)tittle
{
    [self initCommonBackButton:tittle isInRoom:NO];
}

@end
