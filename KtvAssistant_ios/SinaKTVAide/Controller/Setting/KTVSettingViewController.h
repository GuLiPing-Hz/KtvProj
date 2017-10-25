//
//  KTVSettingViewController.h
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-10-31.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "ZBarReaderViewController.h"
@interface KTVSettingViewController : BaseViewController<UITableViewDataSource,UITableViewDelegate,UIAlertViewDelegate,ZBarReaderDelegate>
{
    NSString *keyword_;
}
@property (nonatomic,strong)  ZBarReaderViewController *reader;
@property (nonatomic, retain) NSString                 *keyWord;
@property (nonatomic, strong) UITableView              *setTableView;
@property (nonatomic, assign) BOOL                       isNeedUpdate;
@property (nonatomic,strong)  UIAlertView              *altExitRoom;
@property (nonatomic,strong)  UIAlertView              *altOutLogin;
@property (nonatomic,strong)  UIButton                 *logOut;
@end
