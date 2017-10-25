//
//  MessageViewController.h
//  SinaKTVAide

// 消息中心界面

//  Created by Li Pan on 13-12-13.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "EGORefreshTableHeaderView.h"
#import "Message.h"
#import "ZBarReaderViewController.h"
@interface MessageViewController : BaseViewController<EGORefreshTableHeaderDelegate, UITableViewDelegate, UITableViewDataSource, EGORefreshTableHeaderDelegate,UIAlertViewDelegate,ZBarReaderDelegate>
{
    EGORefreshTableHeaderView *_refreshHeaderView;

    BOOL _reloading;
    
    NSString *keyword_;
}
@property (nonatomic,strong)  ZBarReaderViewController *reader;
@property (nonatomic, retain) NSString                 *keyWord;
@property (nonatomic, strong) UITableView              *messageListTable;
@property (nonatomic, strong) NSMutableArray           *arrMessage;
@property (nonatomic,strong)  UIAlertView              *altExitRoom;
@property (nonatomic,strong)  UIImageView             *egoLogo;
@property (nonatomic,strong)  UILabel                  *labNotice;
@property (nonatomic,strong)  UILabel                  *noMessageLabel;
- (void)reloadTableViewDataSource;
- (void)doneLoadingTableViewData;

@end
