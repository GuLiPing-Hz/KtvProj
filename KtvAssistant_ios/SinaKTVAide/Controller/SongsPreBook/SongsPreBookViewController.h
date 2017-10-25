//
//  SongsPreBookViewController.h
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-10-23.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BaseViewController.h"
#import "KTVGetAlreadyDemandSongAPI.h"

@interface SongsPreBookViewController : BaseViewController <UITableViewDataSource,UITableViewDelegate,KTVGetAlreadyDemandSongAPIDelegate,UIAlertViewDelegate>
{
    
    KTVGetAlreadyDemandSongAPI *getSongsAPI;
    
}
@property (nonatomic, strong) UITableView       *preSongTable;
@property (nonatomic, strong) NSMutableArray    *preSongDataArray;
@property (nonatomic, strong) UILabel *emptyTips;
@property (nonatomic, retain) UIActivityIndicatorView *indicatorView;

-(void)readLoacalPreVodSongData;
-(void)loadAlreadyDemandSongsData;
@end
