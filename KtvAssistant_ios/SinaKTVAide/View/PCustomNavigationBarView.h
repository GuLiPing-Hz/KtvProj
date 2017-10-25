//
//  PCustomNavigationBarView.h
//  itime
//
//  Created by pig on 13-4-19.
//  Copyright (c) 2013年 pig. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PCustomNavigationBarView : UIView

@property (nonatomic, retain) UIImageView *bgImageView;
@property (nonatomic, retain) UILabel *titleLabel;
@property (nonatomic, retain) UIButton *backButton;
@property (nonatomic, retain) UIButton *leftButton;
@property (nonatomic, retain) UIButton *rightButton;
@property (nonatomic, retain) UIButton *btnHistory; //只用于礼物兑换界面的

- (id)initCustomNavigationBarView:(NSString *)tTitle;
- (id)initWithTitle:(NSString *)tTitle;

-(void)initIndexButton;
-(void)initCommonBackButton:(NSString*)tittle isInRoom:(BOOL)is;
-(void)initOnlyBackButton:(NSString*)tittle;

@end
