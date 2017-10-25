//
//  MessageDetailViewController.h
//  SinaKTVAide

// 消息中心详情界面

//  Created by Li Pan on 13-12-13.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "TopSong.h"
@interface MessageDetailViewController : BaseViewController<UIAlertViewDelegate>
@property (nonatomic,retain) UILabel *SongTitle;
@property (nonatomic,retain) UILabel *SongSinger;
@property (nonatomic,retain) UILabel *SongScore;
@property (nonatomic,retain) UIButton *rushBtn;
@property (nonatomic,retain) UIImageView *rushImage;
@property (nonatomic,retain) UIView *bgView;
@property (nonatomic,retain) UILabel *rushLabel;
@property (nonatomic,retain) UILabel *rushNoticeLabel;
@property (nonatomic,retain) TopSong *topSong;
@property (nonatomic,assign) long msgID;
@property (nonatomic,assign) long workID;
@property (nonatomic,assign) int point;
@property (nonatomic,assign) int msgType;
@property (nonatomic,copy) NSString *content;
@property (nonatomic,copy) NSString *date;
@end
