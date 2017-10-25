//
//  CustomTabView.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-16.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol CustromTabDelegate <NSObject>

-(void) TabChange:(NSInteger)oldIndex newIndex:(NSInteger)newIndex;

@end

@interface CustomTabView : UIView

@property (nonatomic,retain) NSMutableArray         *titles;
@property (nonatomic,retain) NSMutableArray         *tableView;
@property (nonatomic,assign) NSInteger              currentIndex;

@property (nonatomic,retain) NSMutableArray         *tabBtn;
@property (nonatomic,retain) UIView                 *tabHead;

@property (nonatomic,retain) id<CustromTabDelegate> delegate;

- (id)initWithFrame:(CGRect)frame titles:(NSMutableArray *)titles  views:(NSMutableArray *)_views;



@end
