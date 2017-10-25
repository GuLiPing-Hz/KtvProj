//
//  ArtistListViewController.h
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-12.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseWaitViewController.h"


@interface ArtistListViewController : BaseWaitViewController <ASIHTTPRequestDelegate,UITableViewDataSource,UITableViewDelegate>
{
    UITableView * _artistList;
    BOOL _localData;
}

@property (nonatomic, strong) NSString * artistListTitle;
@property (nonatomic, strong) NSMutableArray * dataArray;
@property (nonatomic, assign) int modelID;
@property (nonatomic, assign) long topicID;
@property (nonatomic, strong) NSMutableArray * indexArray;

@end
