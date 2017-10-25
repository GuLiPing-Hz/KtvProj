//
//  InteractController.h
//  SinaKTVAide
//
//  Created by cxp on 14-7-17.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "EGORefreshTableHeaderView.h"
#import "ZBarReaderViewController.h"
#import "NetUtilCallBackDelegate.h"
#import "ExpressionView.h"
#import "PublicChatCell.h"

@interface InteractController : BaseViewController<UIAlertViewDelegate,ZBarReaderDelegate,UITextFieldDelegate, NetUtilCallBackDelegate, ExpressionDelegate,ChatCellHBProtocol,UIGestureRecognizerDelegate>
{
    //EGORefreshTableHeaderView *_refreshHeaderView;
    BOOL _reloading;
    NSString *keyword_;
    
}
@property (nonatomic,strong)  ZBarReaderViewController *reader;
@property (nonatomic,strong) NSMutableArray           *arrMessage;
@property (nonatomic,strong) UIAlertView              *altExitRoom;
@property (nonatomic, retain) NSString                *keyWord;
@property (nonatomic,strong)  UILabel                 *labNotice;
@property (nonatomic,strong)  UILabel                 *noMessageLabel;
@property (nonatomic,assign) NSInteger                chattype;

- (void)showGift:(NSInteger)useridx:(NSString *)username;
- (void)showPrivateChat:(NSInteger)useridx:(NSString *)username;
@end
