//
//  PageControl.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-8-9.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PageControl : UIPageControl

@property (nonatomic,retain) UIColor *selectColor;
@property (nonatomic,retain) UIColor *freeColor;

- (void)updateDots6;

@end
