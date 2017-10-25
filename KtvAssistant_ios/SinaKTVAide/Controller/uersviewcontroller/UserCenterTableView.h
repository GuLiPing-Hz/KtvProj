//
//  UserCenterTableView.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-21.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UserCenterTableView : UIView<UITableViewDataSource,UITableViewDelegate>
{
    NSInteger tableType; // 0奖励 1收礼 2送礼
    NSInteger index ;
    BOOL      isLoad;
}

@property (nonatomic,retain) NSMutableArray             *data;
@property (nonatomic,retain) UITableView                *tableView;
@property (nonatomic,strong) UIActivityIndicatorView    *actListMessage;


- (id)initWithFrame:(CGRect)frame type:(NSInteger)_type;
@end
