//
//  Rankings.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/20.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "BaseWaitViewController.h"
#import "RankingsViewCell.h"
#import "MONActivityIndicatorView.h"

@interface RankingsViewController : BaseWaitViewController<UITableViewDataSource,UITableViewDelegate,RankingTouchProtocol,MONActivityIndicatorViewDelegate>

@property(nonatomic,strong) UITableView* mUiTableView;
@property(nonatomic,assign) int mModelId;
@property(nonatomic,strong) NSString* mTitle;

@property(nonatomic,strong) NSMutableArray* mDataLists;

@property (strong, nonatomic) IBOutlet RankingsViewCell *mUiRankingsCell;

-(instancetype) initWithModelId:(int)modelId title:(NSString*) title;
@end
