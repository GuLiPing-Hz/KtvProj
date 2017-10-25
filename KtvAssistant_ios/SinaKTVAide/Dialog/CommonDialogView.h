//
//  HongBDialogView.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/25.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CustomDialogDelegate.h"

extern NSString *const SIAlertViewWillShowNotification;
extern NSString *const SIAlertViewDidShowNotification;
extern NSString *const SIAlertViewWillDismissNotification;
extern NSString *const SIAlertViewDidDismissNotification;

typedef NS_ENUM(NSInteger, SIAlertViewButtonType) {
    SIAlertViewButtonTypeDefault = 0,
    SIAlertViewButtonTypeDestructive,
    SIAlertViewButtonTypeCancel
};

typedef NS_ENUM(NSInteger, SIAlertViewBackgroundStyle) {
    SIAlertViewBackgroundStyleSolid = 0,
    SIAlertViewBackgroundStyleGradient,
};

typedef NS_ENUM(NSInteger, SIAlertViewTransitionStyle) {
    SIAlertViewTransitionStyleSlideFromBottom = 0,
    SIAlertViewTransitionStyleSlideFromTop,
    SIAlertViewTransitionStyleFade,
    SIAlertViewTransitionStyleBounce,
    SIAlertViewTransitionStyleDropDown
};

@class CommonDialogView;
typedef void(^CommonDialogViewHandler)(CommonDialogView *alertView);

@interface CommonDialogView : UIView

@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *message;

@property (nonatomic, assign) SIAlertViewTransitionStyle transitionStyle; // default is SIAlertViewTransitionStyleSlideFromBottom
@property (nonatomic, assign) SIAlertViewBackgroundStyle backgroundStyle; // default is SIAlertViewBackgroundStyleSolid

@property (nonatomic, copy) CommonDialogViewHandler willShowHandler;
@property (nonatomic, copy) CommonDialogViewHandler didShowHandler;
@property (nonatomic, copy) CommonDialogViewHandler willDismissHandler;
@property (nonatomic, copy) CommonDialogViewHandler didDismissHandler;

@property (nonatomic, readonly, getter = isVisible) BOOL visible;

@property (nonatomic, strong) UIColor *viewBackgroundColor NS_AVAILABLE_IOS(5_0) UI_APPEARANCE_SELECTOR;
@property (nonatomic, strong) UIColor *titleColor NS_AVAILABLE_IOS(5_0) UI_APPEARANCE_SELECTOR;
@property (nonatomic, strong) UIColor *messageColor NS_AVAILABLE_IOS(5_0) UI_APPEARANCE_SELECTOR;
@property (nonatomic, strong) UIFont *titleFont NS_AVAILABLE_IOS(5_0) UI_APPEARANCE_SELECTOR;
@property (nonatomic, strong) UIFont *messageFont NS_AVAILABLE_IOS(5_0) UI_APPEARANCE_SELECTOR;
@property (nonatomic, strong) UIFont *buttonFont NS_AVAILABLE_IOS(5_0) UI_APPEARANCE_SELECTOR;
@property (nonatomic, assign) CGFloat cornerRadius NS_AVAILABLE_IOS(5_0) UI_APPEARANCE_SELECTOR; // default is 2.0
@property (nonatomic, assign) CGFloat shadowRadius NS_AVAILABLE_IOS(5_0) UI_APPEARANCE_SELECTOR; // default is 8.0

@property (nonatomic,weak) id<CustomDialogDelegate> mCustomLayoutDelegate;
//Custom Display Dialog
-(instancetype)initWithDelegate:(id<CustomDialogDelegate>)delegate;

//Common Display Dialog
- (id)initWithTitle:(NSString *)title andMessage:(NSString *)message;
- (void)addButtonWithTitle:(NSString *)title type:(SIAlertViewButtonType)type handler:(CommonDialogViewHandler)handler;

- (void)show;
- (void)dismissAnimated:(BOOL)animated;
@end
