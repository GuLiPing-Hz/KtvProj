//
//  ExchangeHistoryController.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "CustomTabView.h"

@interface ExchangeHistoryController : BaseViewController<CustromTabDelegate>

@property (nonatomic,retain) NSMutableArray         *titles;
@property (nonatomic,retain) NSMutableArray         *tbView;


@end
