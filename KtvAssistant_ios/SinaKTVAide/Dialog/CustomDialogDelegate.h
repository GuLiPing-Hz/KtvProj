//
//  CustomDialogDelegate.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/25.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <Foundation/Foundation.h>

@class CommonDialogView;
@protocol CustomDialogDelegate <NSObject>

@optional
-(void)onCustomLayout:(CommonDialogView*)dialog view:(UIView*)contentView;

@end
