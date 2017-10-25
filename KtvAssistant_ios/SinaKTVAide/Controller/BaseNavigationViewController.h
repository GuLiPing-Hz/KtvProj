//
//  BaseNavigationViewController.h
//  SinaKTVAide
//
//  Created by Li Pan on 13-12-2.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface BaseNavigationViewController : UINavigationController
@property(nonatomic,readonly) UISwipeGestureRecognizer *swipe;
@end
