//
//  CustomScroll.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-8-9.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "CustomScroll.h"

@implementation CustomScroll

- (id)initWithFrame:(CGRect)frame views:(NSMutableArray *)views
{
    self = [super initWithFrame:frame];
    if (self) {
        if (views.count<=0)
            return self;
        NSInteger height = ((UIView *)views[0]).frame.size.height;
        _scrollView = [[UIScrollView alloc] initWithFrame:CGRectMake(0, 0,frame.size.width, height)];
        _scrollView.contentSize = CGSizeMake(frame.size.width * views.count, height);
        _scrollView.showsHorizontalScrollIndicator = NO;
        _scrollView.showsVerticalScrollIndicator = NO;
        _scrollView.delegate = self;
        _scrollView.bounces = NO;
        _scrollView.backgroundColor = [UIColor clearColor];
        _scrollView.pagingEnabled = YES;
        _pageControl = [[PageControl alloc] initWithFrame:CGRectMake(0, height + 1,frame.size.width, frame.size.height - height)];
        _pageControl.userInteractionEnabled = NO;
        _pageControl.numberOfPages = views.count;
        _pageControl.currentPage = 0;
        contentView = [[NSMutableArray alloc] init];
        [contentView addObjectsFromArray:views];
        [self addSubview:_scrollView];
        [self addSubview:_pageControl];
        for (UIView *view in views) {
            [_scrollView addSubview:view];
        }
    }
    return self;
}

#pragma mark - UIScrollViewDelegate
-(void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    float targetX = scrollView.contentOffset.x;
    NSInteger page = targetX / self.frame.size.width;
    _pageControl.currentPage = page;
}
-(void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate
{
    
}


- (void)scrollToIndex:(id)sender
{
    int index = ((UIButton *)sender).tag;
    if (index == 1) {
        index = -1;
    }
    [self moveToTargetPosition:_scrollView.frame.size.width*(index+1)];
}


- (void)moveToTargetPosition:(CGFloat)targetX
{
    BOOL animated = YES;
    //    NSLog(@"moveToTargetPosition : %f" , targetX);
    [_scrollView setContentOffset:CGPointMake(targetX, 0) animated:animated];
}


@end
