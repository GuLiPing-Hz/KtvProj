//
//  KTVAboutController.m
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-11-1.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "KTVAboutController.h"
#import "SSImageCategory.h"
#import "UIFont+PFontCategory.h"
#import "DDMenuController.h"
#import "KTVAppDelegate.h"
@interface KTVAboutController ()
{
    UIImageView *cdImageView;
    CGPoint locationPoint;
}
@end

@implementation KTVAboutController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:NO];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:YES];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    
    self.navView.titleLabel.text = @"关于";
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.mainContentView.backgroundColor = UIColorFromRGB(0xecedf2);

    
    //215, 217 icon
    UIImageView *aboutIconImageView = [[UIImageView alloc] init];
    aboutIconImageView.frame = CGRectMake((_MainScreen_Width - 108)/2, 105, 108, 108);
    aboutIconImageView.image = [UIImage imageWithName:@"icon_guanyu" type:@"png"];
    [self.mainContentView addSubview:aboutIconImageView];
    
    //desc
    UILabel *lblAppDesc = [[UILabel alloc] init];
    lblAppDesc.frame = CGRectMake(0, 105 + 108 + 20, _MainScreen_Width, 30);
    lblAppDesc.backgroundColor = [UIColor clearColor];
    lblAppDesc.textAlignment = UITextAlignmentCenter;
    lblAppDesc.textColor = UIColorFromRGB(0x7a7a7a);
    lblAppDesc.font = [UIFont fontOfSystem:14.0f];
    lblAppDesc.text = [NSString stringWithFormat:@"新浪KTV助手V%@", [[NSBundle mainBundle] objectForInfoDictionaryKey: @"CFBundleShortVersionString"]];
    [self.mainContentView addSubview:lblAppDesc];
}

// 飞盘效果
//- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
//{
//    UITouch *touch = [touches anyObject];
//    locationPoint = [touch locationInView:cdImageView];
//    PLog(@"locationPoint=%@",NSStringFromCGPoint(locationPoint));
//    [self actionAA];
//}
//
//- (void)actionAA
//{
//    CGPoint newLocationPoint;
//    newLocationPoint.x = _MainScreen_Width+locationPoint.x-24;
//    newLocationPoint.y = locationPoint.y+40;
//    PLog(@"newLocationPoint=%@",NSStringFromCGPoint(newLocationPoint));
//    cdImageView.frame = CGRectMake(newLocationPoint.x, newLocationPoint.y, 24, 24);
//    
//    [UIView animateWithDuration:1.0 animations:^{
//        [cdImageView setHidden:NO];
//        CGRect newFrame = cdImageView.frame;
//        newFrame.origin = CGPointMake(290, 44);
//        cdImageView.frame = newFrame;
//    } completion:^(BOOL finished) {
//        if (finished) {
//            [cdImageView setHidden:YES];
//        }
//    }];
//}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
