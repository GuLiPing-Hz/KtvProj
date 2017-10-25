//
//  KTVAppDelegate.h
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-10-23.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "WeiboSDK.h"

@class LeftViewController;
@class BaseNavigationViewController;
@class DDMenuController;

@interface KTVAppDelegate : UIResponder <UIApplicationDelegate,UIAlertViewDelegate>

@property (strong, nonatomic) UIWindow               *window;
@property (strong, nonatomic) UINavigationController *naviMain;
@property (strong, nonatomic) DDMenuController       *menuController;

//@property (strong, nonatomic) TencentOAuth           *tencentOAuth;
@property (strong, nonatomic) NSArray                *permissions;
//@property (strong, nonatomic) NSMutableArray         * alreadyOrderedSongsList;
@property (strong, nonatomic) NSMutableArray         * bannerListArray;
@property (strong, nonatomic) NSMutableArray         * bannerImageArray;
@property (strong, nonatomic) NSMutableArray         * singerList;

@property (strong, nonatomic) UIAlertView         * loginoutAlert;
@property (strong, nonatomic) UIAlertView         * notificationAlert;
@property (strong, nonatomic) UIAlertView         * loginfaledAlert;


-(void)loginMain;
@end
