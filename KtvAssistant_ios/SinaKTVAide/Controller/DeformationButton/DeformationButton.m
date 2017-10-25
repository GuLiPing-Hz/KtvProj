//
//  DeformationButton.m
//  DeformationButton
//
//  Created by LuciusLu on 15/3/16.
//  Copyright (c) 2015年 MOZ. All rights reserved.
//

#import "DeformationButton.h"

@implementation DeformationButton{
    CGFloat defaultW;
    CGFloat defaultH;
    CGFloat defaultR;
    CGFloat scale;
    UIView *bgView;
    BOOL currentSuccess;
    BOOL isReset;
}

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self initSetting];
    }
    return self;
}

- (void)initBtn{
    self.forDisplayButton = [[UIButton alloc]initWithFrame:self.bounds];
    self.forDisplayButton.userInteractionEnabled = NO;
    [self addSubview:self.forDisplayButton];
}

-(CGRect)frame{
    CGRect frame = [super frame];
    self.forDisplayButton.frame = frame;
    return frame;
}

- (void)initSetting{
    currentSuccess = true;
    isReset = false;
    
    scale = 1.2;
    bgView = [[UIView alloc]initWithFrame:self.bounds];
    bgView.backgroundColor = [UIColor blueColor];
    bgView.userInteractionEnabled = NO;
    bgView.hidden = YES;
    [self addSubview:bgView];
    
    defaultW = bgView.frame.size.width;
    defaultH = bgView.frame.size.height;
    defaultR = bgView.layer.cornerRadius;
    
    MMMaterialDesignSpinner *spinnerView = [[MMMaterialDesignSpinner alloc] initWithFrame:CGRectZero];
    self.spinnerView = spinnerView;
    self.spinnerView.bounds = CGRectMake(0, 0, defaultH*0.8, defaultH*0.8);
    self.spinnerView.tintColor = [UIColor whiteColor];
    self.spinnerView.lineWidth = 2;
    self.spinnerView.center = CGPointMake(CGRectGetMidX(self.layer.bounds), CGRectGetMidY(self.layer.bounds));
    self.spinnerView.translatesAutoresizingMaskIntoConstraints = NO;
    self.spinnerView.userInteractionEnabled = NO;
    [self addSubview:self.spinnerView];
    
    [self addTarget:self action:@selector(loadingAction) forControlEvents:UIControlEventTouchUpInside];
    
    [self initBtn];
}

-(void)setContentColor:(UIColor *)contentColor{
    _contentColor = contentColor;
    bgView.backgroundColor = contentColor;
}

-(void)setProgressColor:(UIColor *)progressColor{
    _progressColor = progressColor;
    self.spinnerView.tintColor = progressColor;
}

-(void)setImage:(UIImage*)normal Success:(UIImage*)success Fail:(UIImage*)fail
{
    self.mImgNormal = normal;
    self.mImgSuccess = success;
    self.mImgFail = fail;
    [self resetButton:false onEnd:nil];
}

-(void)resetButton:(BOOL)isreset onEnd:(void(^)())onEnd
{
    if(isreset)
    {
        isReset = true;
        [self stopLoading:onEnd];
    }
    
    [self setEnabled:YES];
    [self.forDisplayButton setImage:self.mImgNormal forState:UIControlStateNormal];
    //[self.forDisplayButton setImage:nil forState:UIControlStateDisabled];
}

-(void)setStopSuccess:(BOOL)isSuccess
{
    currentSuccess = isSuccess;
    [self setIsLoading:NO];
}

-(void)setIsLoading:(BOOL)isLoading{
    _isLoading = isLoading;
    if (_isLoading) {
        [self startLoading];
    }else{
        [self stopLoading:nil];
    }
}

- (void)loadingAction{
    if (self.isLoading) {
        [self stopLoading:nil];
    }else{
        [self setEnabled:NO];
        [self startLoading];
    }
}

- (void)startLoading{
    _isLoading = YES;
    bgView.hidden = NO;
    self.userInteractionEnabled = NO;
    
    self.forDisplayButton.hidden = YES;
    
    CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"cornerRadius"];
    animation.timingFunction = [CAMediaTimingFunction     functionWithName:kCAMediaTimingFunctionLinear];
    animation.fromValue = [NSNumber numberWithFloat:defaultR];
    animation.toValue = [NSNumber numberWithFloat:defaultH*scale*0.5];
    animation.duration = 0.2;
    [bgView.layer setCornerRadius:defaultH*scale*0.5];
    [bgView.layer addAnimation:animation forKey:@"cornerRadius"];

    if(self.animateStart)
        self.animateStart();
    
    [UIView animateWithDuration:0.3 delay:0 usingSpringWithDamping:0.6 initialSpringVelocity:0 options:UIViewAnimationOptionCurveLinear animations:^{
        bgView.layer.bounds = CGRectMake(0, 0, defaultW*scale, defaultH*scale);
    } completion:^(BOOL finished) {
        [UIView animateWithDuration:0.3 delay:0 usingSpringWithDamping:0.6 initialSpringVelocity:0 options:UIViewAnimationOptionCurveLinear animations:^{
            bgView.layer.bounds = CGRectMake(0, 0, defaultH*scale, defaultH*scale);
            self.forDisplayButton.transform = CGAffineTransformMakeScale(0, 0);
            self.forDisplayButton.alpha = 0;
            [self setOpaque:NO];
        } completion:^(BOOL finished) {
            self.forDisplayButton.hidden = YES;
            [self.spinnerView startAnimating];
        }];
    }];
}

- (void)stopLoading:(void(^)()) onEnd{
    [self.spinnerView stopAnimating];

    [UIView animateWithDuration:0.3 delay:0 usingSpringWithDamping:0.8 initialSpringVelocity:0 options:
     UIViewAnimationOptionCurveLinear animations:^{
        self.forDisplayButton.transform = CGAffineTransformMakeScale(1, 1);
        self.forDisplayButton.alpha = 1;
    } completion:^(BOOL finished) {
        
    }];
    
    [UIView animateWithDuration:0.3 delay:0 usingSpringWithDamping:0.6 initialSpringVelocity:0 options:UIViewAnimationOptionCurveLinear animations:^{
        bgView.layer.bounds = CGRectMake(0, 0, defaultW*scale, defaultH*scale);
    } completion:^(BOOL finished) {
        CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"cornerRadius"];
        animation.timingFunction = [CAMediaTimingFunction     functionWithName:kCAMediaTimingFunctionLinear];
        animation.fromValue = [NSNumber numberWithFloat:bgView.layer.cornerRadius];
        animation.toValue = [NSNumber numberWithFloat:defaultR];
        animation.duration = 0.2;
        [bgView.layer setCornerRadius:defaultR];
        [bgView.layer addAnimation:animation forKey:@"cornerRadius"];
        [UIView animateWithDuration:0.3 delay:0 usingSpringWithDamping:0.6 initialSpringVelocity:0 options:UIViewAnimationOptionCurveLinear animations:^{
            bgView.layer.bounds = CGRectMake(0, 0, defaultW, defaultH);
        } completion:^(BOOL finished) {
            
            if(self.animateEnd)
                self.animateEnd();
            
            if(isReset)
            {
                [self setEnabled:YES];
                isReset = false;
            }
            else
            {
                bgView.hidden = YES;
                self.forDisplayButton.hidden = NO;
                if(currentSuccess)
                {
                    [self.forDisplayButton setImage:self.mImgSuccess forState:UIControlStateDisabled];
                    [self setEnabled:NO];
                }
                else
                {
                    [self.forDisplayButton setImage:self.mImgFail forState:UIControlStateNormal];
                    [self setEnabled:YES];
                }
            }
            self.userInteractionEnabled = YES;
            
            if(onEnd)
            {
                onEnd();
            }
            _isLoading = NO;
        }];
    }];
}

-(void)setSelected:(BOOL)selected{
    if(self.isLoading)
        return ;
    
    [super setSelected:selected];
    [self.forDisplayButton setSelected:selected];
}

- (void)setHighlighted:(BOOL)highlighted{
//    if(self.isLoading)
//        return ;
    
    [self resetButton:NO onEnd:nil];
    [super setHighlighted:highlighted];
    [self.forDisplayButton setHighlighted:highlighted];
}

-(void)setEnabled:(BOOL)enabled
{
    [super setEnabled:enabled];
    [self.forDisplayButton setEnabled:enabled];
}
/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

@end
