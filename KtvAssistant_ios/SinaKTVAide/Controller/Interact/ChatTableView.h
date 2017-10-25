//
//  PublicChatTableView.h
//  SinaKTVAide
//
//  Created by cxp on 14-7-23.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PublicChatCell.h"

@interface ChatTableView : UIView<UITableViewDelegate, UITableViewDataSource,ChatCellHBProtocol>
{
    BOOL      isLoad;
    NSInteger cType;//0:公聊 1:私聊  2:喇叭
    //UIButton   *privateHeadBtn;
    //UIButton   *giftHeadBtn;
    //UIButton   *redHeadBtn;
    NSString   *uid;
    NSString   *name;
    CGFloat    pointY;
    BOOL       isShow;
    NSInteger  cellIndex;
    NSInteger  lastrow;
    NSInteger  rowheight;
}

@property (nonatomic,retain) UITableView                *tableView;
@property (nonatomic,strong) UIActivityIndicatorView    *actListMessage;
@property (nonatomic,retain) UIButton                *privateHeadBtn;
@property (nonatomic,retain) UIButton                *giftHeadBtn;
@property (nonatomic,retain) UIButton                *redHeadBtn;
@property (nonatomic,retain) id<ChatCellHBProtocol> mDelegate;

- (id)initWithFrame:(CGRect)frame  type:(NSInteger)type delegate:(id<ChatCellHBProtocol>)delegate;
@end
