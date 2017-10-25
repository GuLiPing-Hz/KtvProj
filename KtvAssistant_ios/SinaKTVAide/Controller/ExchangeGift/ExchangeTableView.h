//
//  ExchangeTableView.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ExchangeLogCell.h"

@interface ExchangeTableView : UIView<UITableViewDataSource,UITableViewDelegate>
{
    NSInteger tableType; // 0未领取  1已领取  2过期
    NSInteger index;
    BOOL      isLoad;
}

@property (nonatomic,retain) NSMutableArray             *data;
@property (nonatomic,retain) UITableView                *tableView;

- (id)initWithFrame:(CGRect)frame type:(NSInteger)type;
- (void)reloadTable;
@end
