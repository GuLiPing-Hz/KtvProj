//
//  Onlinepeople.h
//  SinaKTVAide
//
//  Created by cxp on 14-7-28.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BaseViewController.h"

@protocol OnlineDelegate <NSObject>

-(void) sendgift:(NSString*)name :(NSInteger)uidx;
-(void) sendchat:(NSString*)username :(NSInteger)uidx;

@end

@interface OnlinepeopleView : BaseViewController<UITableViewDelegate, UITableViewDataSource>
{
    NSInteger currentIndex;
    NSInteger frontIndex;
}

@property (nonatomic,strong) UITableView        *tableView;
@property (nonatomic, assign) NSInteger         presentRow;
@property (nonatomic, strong) NSMutableArray    *dataArray;
@property (nonatomic,retain) id<OnlineDelegate>    delegate;

- (id)initWithFrame:(CGRect)frame;


@end
