//
//  OnlinePersonView.h
//  SinaKTVAide
//
//  Created by cxp on 15/5/6.
//  Copyright (c) 2015年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BaseViewController.h"
#import "NetUtilCallBackDelegate.h"

@interface OnlinePersonView : BaseViewController<UITextViewDelegate,UIAlertViewDelegate,UITableViewDelegate, UITableViewDataSource,NetUtilCallBackDelegate>
{
    NSInteger selfIndex;
    NSInteger selfsection;
    NSInteger currentIndex;
    NSInteger frontIndex;
    BOOL    isShowPrc;
    BOOL    isShowRoom;
    BOOL    isShowLobby;
    NSInteger rowinsection;
}
@property (nonatomic,strong) UITableView        *tableView;

- (void)tableReload;
@end
