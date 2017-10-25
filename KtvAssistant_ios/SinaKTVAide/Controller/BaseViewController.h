//
//  BaseViewController.h
//
//  Created by pig on 13-8-15.
//  Copyright (c) 2013年 pig. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "UIImageView+AFNetworking.h"
#import "UIButton+AFNetworking.h"
#import "PCustomNavigationBarView.h"
#import "TJSpinner.h"
#import "ResString.h"
#import "NetUtilCallBackProtocol.h"

#define HEIGHT_BOTTOM 50

@class KTVAppDelegate;
@interface BaseViewController : UIViewController<NetUtilCallBackProtocol>

//导航
@property (nonatomic, strong) PCustomNavigationBarView *navView;
//背景图片
@property (nonatomic, strong) UIImageView             *bgImageView;
//内容显示的rootView
@property (nonatomic, strong) UIView                   *mainContentView;
// 包厢toolBar
@property (nonatomic,strong)  UIView                   *toolBar;
@property (nonatomic,strong)  UILabel                  *inRoomNumber;
@property (nonatomic,strong)  UIButton                 *power;
@property (nonatomic,strong) UIButton                 *joinButton;

@property (nonatomic, strong) TJSpinner                *redCircle;
@property (nonatomic,assign)  BOOL                       isRedCircle;

@property (nonatomic, strong) UIImageView             *cdImageView;
@property(nonatomic,assign) BOOL mIsRemoveOnDestroy;

-(void)doBack:(id)sender;

-(void)leftAction:(id)sender;

- (KTVAppDelegate *)appDelegate;

- (void)showNetwork;
- (void)showErrorAlert:(NSString *)content;
- (void)showNormalAlert:(NSString *)content;
- (void)showSuccessAlert:(NSString *)content;

- (void)startRedCircle;
- (void)stopRedCircle;
- (void)setRightAndBadgeButton;
- (void)getAlreadyOrderedSongsNumber;
- (Boolean)isShowDefault;

@end
