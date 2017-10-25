//
//  SongsBookViewController.h
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-4.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseWaitViewController.h"
#import "SongsBookCell.h"
#import "ZBarSDK.h"
typedef enum {
    Search_normal,
    Search_edit,
}SearchDataType;

//歌曲列表详细页面，基本上所有的主题下的歌曲列表都可以通过这个页面显示
@interface SongsBookViewController : BaseWaitViewController <UITableViewDataSource,UITableViewDelegate,ZBarReaderDelegate,SongOperation,ASIHTTPRequestDelegate,UISearchBarDelegate>
{
    //NSString *keyword_;
    //NSString * _keyword;
    UIAlertView * _errorRemindView;
    
    BOOL _isLoadData;
    BOOL _noData;
    int _count;
    BOOL _next;
}

@property (nonatomic, strong) ZBarReaderViewController *reader;
@property (nonatomic, retain) NSString                 *keyWord;

@property (nonatomic, strong) UITableView * songsList;
@property (nonatomic, strong) NSMutableArray * dataArray;
@property (nonatomic, strong) NSMutableArray * alreadyBookedSongsArray;

@property (nonatomic, assign) int modelID;
@property (nonatomic, assign) long topicID;
@property (nonatomic, strong) NSString * listTitle;
@property (nonatomic, assign) long artistID;

@property (nonatomic, strong) UISearchBar *songSearchBar;
@property (nonatomic, strong) UIView      *view_loading;
@property (assign) SearchDataType type;
@property (nonatomic, strong) NSMutableArray *searchDataArray;
@property (nonatomic, strong) UILabel *emptyTips;

@property (nonatomic, strong) UIImageView * flashCD;
@property (nonatomic, assign) CGPoint cdFlashStart;

@property (nonatomic, assign) NSInteger presentRow;

-(instancetype) initInfoNoSinger:(int)modelId topicId:(long)p1 title:(NSString*)p2;
-(instancetype) initInfo:(int)modelId topicId:(long)p1 title:(NSString*)p2 singerId:(long)p3;
@end
