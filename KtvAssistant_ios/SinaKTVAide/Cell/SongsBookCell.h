//
//  SongsBookCell.h
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-4.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SongsBookModel.h"
#import "SongOperationProtocol.h"

@interface SongsBookCell : UITableViewCell
{
    UILabel * _songsName;
    UILabel * _singerName;
    
    UIImageView * songOperationView;
    
    UIButton * _songBook;
    UIButton * _songDelete;
    UIButton * _collection;
    UIButton * _top;
    
    UIImageView * _alreadyView;
    
    UIImageView * _lineView;
    UIImageView * _gradedView;
    
    UIImageView * _listBtn;
    
}

@property (nonatomic, strong) SongsBookModel * model;
@property (nonatomic, assign) id<SongOperation> delegate;
@property(nonatomic,assign) int mIndex;

-(void)setCurrentSongImg:(NSInteger)index;

@end
