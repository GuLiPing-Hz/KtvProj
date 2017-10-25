//
//  BaseNavigationViewController.m
//  SinaKTVAide
//
//  Created by Li Pan on 13-12-2.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseNavigationViewController.h"

@interface BaseNavigationViewController ()

@end

@implementation BaseNavigationViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // 隐藏导航条
    [self setNavigationBarHidden:YES];
    
    // 滑动返回手势
    if (!_swipe) {
        UISwipeGestureRecognizer *swipe = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipe:)];
        swipe.delegate = (id<UIGestureRecognizerDelegate>)self;
        swipe.direction = UISwipeGestureRecognizerDirectionRight;
        [self.view addGestureRecognizer:swipe];
        _swipe = swipe;
    }
}

- (void)swipe:(UISwipeGestureRecognizer *)gesture
{
    if (self.viewControllers.count > 1) {
        if (gesture.direction == UISwipeGestureRecognizerDirectionRight) {
            [self popViewControllerAnimated:YES];
        }
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
