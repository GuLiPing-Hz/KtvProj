//
//  InteractTabView.h
//  SinaKTVAide
//
//  Created by cxp on 14-7-17.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface InteractTabView : UIView

@property (nonatomic,retain) NSMutableArray         *titles;
@property (nonatomic,retain) NSMutableArray         *itemsControllers;
@property (nonatomic,assign) NSInteger              currentIndex;
@property (nonatomic,retain) NSMutableArray         *tableView;

- (id)initWithFrame:(CGRect)frame titles:(NSMutableArray *)titles views:(NSMutableArray *)_views;
@end
