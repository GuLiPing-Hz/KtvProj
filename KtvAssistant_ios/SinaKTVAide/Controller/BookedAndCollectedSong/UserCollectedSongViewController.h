//
//  UserCollectedSongViewController.h
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-16.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "SongsBookCell.h"
#import "ZBarReaderViewController.h"
@interface UserCollectedSongViewController : BaseViewController <ASIHTTPRequestDelegate,UITableViewDataSource,UITableViewDelegate,SongOperation,UIAlertViewDelegate,ZBarReaderDelegate>
{
    UITableView * _collectedSongs;
     NSString *keyword_;
}

@property (nonatomic,strong)  ZBarReaderViewController *reader;
@property (nonatomic, retain) NSString                 *keyWord;
@property (nonatomic, strong) NSMutableArray           * dataArray;
@property (nonatomic,strong)  UIAlertView              *altExitRoom;
@property (nonatomic, strong) NSMutableArray * alreadyBookedSongsArray;
@property (nonatomic, strong) UIAlertView * errorMessageView;
@property (nonatomic, strong) NSString * errorMessage;

@property (nonatomic, strong) UILabel *emptyTips;

@property (nonatomic, strong) UIImageView * flashCD;
@property (nonatomic, assign) CGPoint cdFlashStart;

@property (nonatomic, assign) NSInteger presentRow;

@property (nonatomic, strong) UIView * emptyMsgView;

@end
