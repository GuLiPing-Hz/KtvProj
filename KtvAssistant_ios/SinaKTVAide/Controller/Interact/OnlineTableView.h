//
//  OnlineTableView.h
//  SinaKTVAide
//
//  Created by cxp on 14-8-25.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface OnlineTableView : UIView<UITableViewDelegate, UITableViewDataSource>
{
    NSInteger currentIndex;
    NSInteger frontIndex;
    NSInteger selfIndex;
}

@property (nonatomic,strong) UITableView        *tableView;
@property (nonatomic, assign) NSInteger         presentRow;
@property (nonatomic, strong) NSMutableArray    *dataArray;
- (id)initWithFrame:(CGRect)frame;
- (void)tableReload;
@end
