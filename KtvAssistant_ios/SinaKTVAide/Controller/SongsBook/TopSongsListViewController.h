//
//  TopSongsListViewController.h
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-13.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseWaitViewController.h"
#import "KTVTopSongCell.h"
#import "PKtvAssistantAPI.h"
#import "TopSongCell.h"
#import "ZBarSDK.h"
@interface TopSongsListViewController : BaseWaitViewController <UITableViewDataSource,UITableViewDelegate,ZBarReaderDelegate,ASIHTTPRequestDelegate,SongOperation>
{
    NSString *keyword_;
    
    UITableView * _topSongsList;
    UIAlertView * _errorRemindView;
    
    BOOL _isLoadData;
    BOOL _noData;
    int _count;
    
    BOOL _next;
}

@property (nonatomic, strong) ZBarReaderViewController *reader;
@property (nonatomic, retain) NSString                 *keyWord;


@property (nonatomic,strong) NSString * listTitle;
@property (nonatomic, assign) int modelID;
@property (nonatomic, assign) long topicID;

@property (nonatomic, strong) NSMutableArray * dataArray;
@property (nonatomic, strong) NSMutableArray * alreadyBookedSongs;

@property (nonatomic, strong) UIImageView * flashCD;
@property (nonatomic, assign) CGPoint cdFlashStart;

@property (nonatomic, assign) NSInteger presentRow;

@property (strong, nonatomic) IBOutlet KTVTopSongCell *mUITopSongCell;

-(instancetype) initInfo:(int) model topic:(long)topic title:(NSString*)title;
@end
