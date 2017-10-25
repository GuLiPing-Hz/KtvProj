//
//  KTVInfoController.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-23.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "ZBarReaderViewController.h"

@interface KTVInfoController : BaseViewController <UITableViewDataSource,UITableViewDelegate,ZBarReaderDelegate,UIAlertViewDelegate>
{
    BOOL isLoad;
}
//扫描二维码
@property (nonatomic, strong) ZBarReaderViewController *mViewCReader;
@property (nonatomic,retain) UIAlertView    *mAlertExchange;
@property(nonatomic,strong) NSMutableDictionary* mMapData;
@property(nonatomic,strong) UIButton* mUiScanGift;
@property (nonatomic,retain) UITableView    *tableView;
@property (nonatomic,retain) UILabel        *lblTip;
@property (nonatomic,retain) NSMutableArray *arrKtvInfo;

@end
