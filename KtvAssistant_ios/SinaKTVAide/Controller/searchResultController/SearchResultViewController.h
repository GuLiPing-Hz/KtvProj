//
//  SearchResultViewController.h
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-18.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "SongsBookCell.h"
#import "ZBarSDK.h"
@interface SearchResultViewController : BaseViewController <UITableViewDataSource,UITableViewDelegate,ZBarReaderDelegate,SongOperation>
{
    NSString *keyword_;
    
    UITableView * _searchResultList;
    BOOL _isLoadData;
    BOOL _noData;
    int _count;
    BOOL _next;
}

@property (nonatomic, strong) ZBarReaderViewController *reader;
@property (nonatomic, retain) NSString                 *keyWord;

@property (nonatomic, strong) NSString * searchKey;
@property (nonatomic, assign) int modelID;
@property (nonatomic, assign) long topicID;

@property (nonatomic, strong) NSMutableArray * dataArray;
@property (nonatomic, strong) NSMutableArray * alreadyBookedSongsArray;

@property (nonatomic, strong) UILabel *emptyTips;

@property (nonatomic, strong) UIImageView * flashCD;
@property (nonatomic, assign) CGPoint cdFlashStart;

@property (nonatomic, assign) NSInteger presentRow;
@property (nonatomic, strong) UIView * emptyMsgView;


@end
