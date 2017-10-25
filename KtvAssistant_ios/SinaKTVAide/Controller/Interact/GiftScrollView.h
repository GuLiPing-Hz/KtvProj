//
//  GiftContentView.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-8-12.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CustomScroll.h"

@protocol GiftScrollDelegate <NSObject>

-(void) GiftClick:(NSInteger)giftID :(NSInteger)price;

@end

@interface GiftScrollView : UIView
{
    NSInteger giftType;//礼物类别,幸运1，普通2，奢侈3
    int count;
    UIButton *tempBtn;
}

@property (nonatomic,retain) CustomScroll               *scroll;
@property (nonatomic,retain) NSMutableArray             *views;
@property (nonatomic,retain) id<GiftScrollDelegate>     delegate;

- (id)initWithFrame:(CGRect)frame type:(NSInteger) type;

@end
