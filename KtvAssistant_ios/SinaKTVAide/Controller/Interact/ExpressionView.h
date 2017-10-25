//
//  ExpressionView.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-8-11.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CustomScroll.h"

@protocol ExpressionDelegate <NSObject>

-(void)ExpressionClick:(NSInteger) tag;

@end

@interface ExpressionView : UIView

@property (nonatomic,retain) id<ExpressionDelegate> delegate;

- (id)initWithFrame:(CGRect)frame index:(NSInteger) index;

@end
