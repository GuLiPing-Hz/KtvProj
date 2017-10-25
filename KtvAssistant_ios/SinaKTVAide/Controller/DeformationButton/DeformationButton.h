//
//  DeformationButton.h
//  DeformationButton
//
//  Created by LuciusLu on 15/3/16.
//  Copyright (c) 2015年 MOZ. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MMMaterialDesignSpinner.h"

typedef void(^OnAnimateHandler)(void);

@interface DeformationButton : UIButton

@property(nonatomic, assign)BOOL isLoading;
@property(nonatomic, retain)MMMaterialDesignSpinner *spinnerView;
@property(nonatomic, retain)UIColor *contentColor;
@property(nonatomic, retain)UIColor *progressColor;

@property(nonatomic, retain)UIButton *forDisplayButton;

@property(nonatomic,strong)UIImage* mImgNormal;
@property(nonatomic,strong)UIImage* mImgSuccess;
@property(nonatomic,strong)UIImage* mImgFail;

@property(nonatomic,copy) OnAnimateHandler animateStart;
@property(nonatomic,copy) OnAnimateHandler animateEnd;

-(void)setImage:(UIImage*)normal Success:(UIImage*)success Fail:(UIImage*)fail;
-(void)resetButton:(BOOL)isreset onEnd:(void(^)())onEnd;
-(void)setStopSuccess:(BOOL)isSuccess;
@end
