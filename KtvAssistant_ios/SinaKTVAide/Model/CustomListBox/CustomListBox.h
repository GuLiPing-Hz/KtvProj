//
//  CustomListBox.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-25.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol CustomListBoxDelegate <NSObject>

-(void)ListBoxClick:(id)_boxView key:(NSString *)_key value:(NSString *)_value;

@end

@interface CustomListBox : UIView <UITableViewDataSource,UITableViewDelegate>
{
    NSArray     *key;
    NSArray     *value;
    NSInteger   showType;  //-1：正在渐变  0已经隐藏  1已经show
}

@property (nonatomic,retain) UITableView                    *tableView;

@property (nonatomic,retain) id<CustomListBoxDelegate>      delegate;


-(void)setData:(NSArray *)_keys values:(NSArray *)_values;
-(void)ShowOrHide;
-(void)Hide;

@end
