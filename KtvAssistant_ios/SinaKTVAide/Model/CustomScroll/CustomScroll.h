//
//  CustomScroll.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-8-9.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PageControl.h"

@interface CustomScroll : UIView <UIScrollViewDelegate>
{
    NSMutableArray *contentView;
}


@property (nonatomic,retain) UIScrollView    *scrollView;
@property (nonatomic,retain) PageControl   *pageControl;


- (id)initWithFrame:(CGRect)frame views:(NSMutableArray *)views;


@end
