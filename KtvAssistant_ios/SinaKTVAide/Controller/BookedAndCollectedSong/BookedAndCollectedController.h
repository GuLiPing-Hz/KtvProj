//
//  BookedAndCollectedController.h
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-4.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

//已点歌曲界面
#import "BaseViewController.h"
#import "CustomScroll.h"
#import "NetUtilCallBackDelegate.h"
#import "OrderedSongCell.h"
#import "SongOperationProtocol.h"
#import "NetUtilCallBackProtocol.h"
#import "KTVOrderedSongInfo.h"

#define BUTTOM_VIEW_HEIGHT 50.0f
#define REFRESH_TIME 30.0f

@interface BookedAndCollectedController : BaseViewController <ASIHTTPRequestDelegate,UITableViewDataSource,UITableViewDelegate,SongOperation>
{
    UIButton * _bookedBtn;
    UIButton * _collectedBtn;
    UITableView * _songsList;
    UIImageView * _singingView;
    
    UIButton * _changeSong;
    UIAlertView * _errorRemindView;
}
@property(nonatomic,strong) KTVOrderedSongInfo* nowSongId;
@property (nonatomic, retain) CustomScroll   * scroll;
@property (nonatomic, retain) UIView         * setPadView;

@property (nonatomic, strong) NSMutableArray * dataArray;

@property (nonatomic, strong) UILabel *emptyTips;
@property (nonatomic, assign) BOOL isNext;
@property (nonatomic, strong) NSTimer * reloadingData;
@property (nonatomic, assign) BOOL isChanging;
@property (nonatomic, strong) UIView * emptyMsgView;
@property (nonatomic, strong) UIButton * showSetBtn;

@property (strong, nonatomic) IBOutlet OrderedSongCell *mUiSongCell;
@end
