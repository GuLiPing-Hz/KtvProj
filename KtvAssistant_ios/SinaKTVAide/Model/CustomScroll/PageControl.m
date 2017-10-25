//
//  PageControl.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-8-9.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "PageControl.h"

@implementation PageControl

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        
    }
    return self;
}

- (void)updateDots
{
    self.currentPageIndicatorTintColor = _selectColor;
    self.pageIndicatorTintColor = _freeColor;
}

- (void)updateDots6
{
    for (int i = 0; i< [self.subviews count]; i++) {
        UIImageView *dot = [self.subviews objectAtIndex:i];
        dot.image = nil;
        dot.layer.cornerRadius = 3;
        dot.layer.masksToBounds = YES;
        PLog(@"%f",dot.frame.size.width);
        if (i == self.currentPage)
            dot.backgroundColor = _selectColor;
        else
            dot.backgroundColor = _freeColor;
    }
}

- (void)setCurrentPage:(NSInteger)currentPage
{
    [super setCurrentPage:currentPage];
    if(IOS7_OR_LATER)
        [self updateDots];
    else
        [self updateDots6];
}


@end
