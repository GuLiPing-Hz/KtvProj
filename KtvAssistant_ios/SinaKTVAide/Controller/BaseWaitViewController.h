//
//  BaseWaitViewController.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/22.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "BaseViewController.h"
#import "MONActivityIndicatorView.h"

@interface BaseWaitViewController : BaseViewController<MONActivityIndicatorViewDelegate>

@property (strong, nonatomic) UIView *mUiEmptyView;
@property (strong, nonatomic) UILabel *mUiEmptyTip;
@property (strong,nonatomic) MONActivityIndicatorView* mUiActivityIndicator;
@property (strong,nonatomic) UIImageView* mUiLoadingView;

-(void)showEmptyTip:(BOOL)show msg:(NSString*)msg;
-(void)startWait;
-(void)stopWait;

@end
