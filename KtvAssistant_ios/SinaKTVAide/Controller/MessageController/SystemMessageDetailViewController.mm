//
//  SystemMessageDetailViewController.m
//  SinaKTVAide

//  系统消息详情界面

//  Created by Li Pan on 14-1-8.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "SystemMessageDetailViewController.h"
#import "KTVAppDelegate.h"
#import "DDMenuController.h"
@interface SystemMessageDetailViewController ()

@end

@implementation SystemMessageDetailViewController

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
    self.navView.backButton.hidden = NO;
    self.navView.hidden = NO;
    self.navView.titleLabel.text = @"系统消息详情";
    self.view.backgroundColor = UIColorFromRGB(0xe1e2e6);
    
    UITextView *sysTxt = [[UITextView alloc] initWithFrame:CGRectMake(10, 20, _MainScreen_Width-20, 50)];
    sysTxt.text = _SystemMessageContent;
    sysTxt.textColor = UIColorFromRGB(0x2f2f2f);
    sysTxt.backgroundColor = [UIColor clearColor];
    sysTxt.font = [UIFont systemFontOfSize:16.0];
    sysTxt.editable = NO;
    [self.mainContentView addSubview:sysTxt];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
