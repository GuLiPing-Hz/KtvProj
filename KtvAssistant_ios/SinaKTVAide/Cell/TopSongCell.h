//
//  TopSongCell.h
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-13.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TopSongCellModel.h"

@protocol TopSongCellDelegate <NSObject>

- (void)songsBook:(TopSongCellModel *)model;
- (void)songsCollect:(TopSongCellModel *)model;
- (void)songsTop:(TopSongCellModel *)model;

@end

@interface TopSongCell : UITableViewCell

{
    UILabel * _songsName;
    UILabel * _singerName;
    
    UIImageView * _songsBookView;
    
    UIButton * _songBook;
    UIButton * _songDelete;
    UIButton * _collection;
    UIButton * _top;
    
    UIImageView * _alreadyView;
    
    UIImageView * _lineView;
    UIImageView * _gradedView;
    
    UIImageView * _listBtn;
    
    UIImageView * _userHeadImage;
    
    UILabel * _score;
    UILabel * _fen;        //“分”label
    UILabel * _userName;
    UILabel * _zai;        //“在”label
    UILabel * _address;
    UILabel * _chang;       //“唱”label
    
}

@property (nonatomic, strong) TopSongCellModel *model;
@property (nonatomic, assign) id<TopSongCellDelegate> delegate;

@end
