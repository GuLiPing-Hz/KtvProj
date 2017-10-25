//
//  RedDetailView.h
//  SinaKTVAide
//
//  Created by cxp on 15/5/27.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BaseViewController.h"
#import "MessageInfoStruct.h"

@interface RedDetailView : BaseViewController<UITableViewDelegate, UITableViewDataSource>
{

}

@property (nonatomic,strong) UITableView        *tableView;
@property (nonatomic,assign) HBSendDetail       sendinfo;
@property (nonatomic,assign) NSInteger       getgold;
@end
