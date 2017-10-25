//
//  TopicListViewController.h
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-12.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseWaitViewController.h"
#import "ASIHTTPRequest.h"
#import "MONActivityIndicatorView.h"

@interface TopicListViewController : BaseWaitViewController <ASIHTTPRequestDelegate,UITableViewDataSource,UITableViewDelegate,MONActivityIndicatorViewDelegate>
{
    UITableView * _topicList;
}

@property (nonatomic, assign) int modelID;
@property (nonatomic, assign) NSInteger type;
@property (nonatomic, strong) NSMutableArray * dataArray;
@property (nonatomic, strong) NSString * topicTitle;

-(instancetype) initModelId:(int)modelId title:(NSString*)title;
@end
