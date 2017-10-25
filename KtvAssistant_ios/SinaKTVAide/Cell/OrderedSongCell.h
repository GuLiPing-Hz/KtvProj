//
//  OrderedSongCell.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/18.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SongOperationProtocol.h"
#import "KTVOrderedSongInfo.h"

@interface OrderedSongCell : UITableViewCell

@property(nonatomic,weak) KTVOrderedSongInfo* mInfo;
@property (nonatomic,weak) id<SongOperation> mCallBack;

@property (weak, nonatomic) IBOutlet UIImageView *mUiStatus;
@property (weak, nonatomic) IBOutlet UILabel *mUiName;
@property (weak, nonatomic) IBOutlet UILabel *mUiSinger;
@property (weak, nonatomic) IBOutlet UIButton *mUiTop;
@property (weak, nonatomic) IBOutlet UIImageView *mUiMoreTip;
@property (weak, nonatomic) IBOutlet UIView *mUiSongOperation;
@property (weak, nonatomic) IBOutlet UIButton *mUiDelete;
@property (weak, nonatomic) IBOutlet UIButton *mUiCollect;


- (IBAction)onTop:(id)sender;
- (IBAction)onDelete:(id)sender;
- (IBAction)onCollect:(id)sender;

-(void) setInfo:(KTVOrderedSongInfo*)info withIndex:(NSUInteger)index;
@end
