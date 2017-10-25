//
//  TopSongCell.m
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-13.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "TopSongCell.h"
#import "UIImageView+AFNetworking.h"
#import "CommenTools.h"

@implementation TopSongCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        // Initialization code
        
        self.selectionStyle = UITableViewCellSelectionStyleNone;
        
        
        [self.contentView setBackgroundColor:UIColorFromRGB(0xf0f1f5)];
        
        _userHeadImage = [[UIImageView alloc]init];
        _userHeadImage.frame = CGRectMake(5, 5, 50, 50);
        //_userHeadImage.image = [UIImage imageNamed:@"pic_top100.png"];
        
        [self.contentView addSubview:_userHeadImage];
        
        _alreadyView = [[UIImageView alloc]init];
        _alreadyView.frame = CGRectMake(0, 0, 20, 20);
        _alreadyView.image = [UIImage imageNamed:@"yidian.png"];
        _alreadyView.hidden = YES;
        
        [self.contentView addSubview:_alreadyView];
        
        _lineView = [[UIImageView alloc]init];
        _lineView.frame = CGRectMake(0, 59, _MainScreen_Width, 1);
        _lineView.image = [UIImage imageNamed:@"list_line.png"];
        
        [self.contentView addSubview:_lineView];
        
        _songsName = [[UILabel alloc]init];
        _songsName.font = [UIFont systemFontOfSize:15];
        [_songsName setBackgroundColor:[UIColor clearColor]];
        //_songsName.frame = CGRectMake(25, 5, 200, 25);
        _songsName.textColor = UIColorFromRGB(0x212121);
        
        [self.contentView addSubview:_songsName];
        
        
        _singerName = [[UILabel alloc]init];
        _singerName.font = [UIFont systemFontOfSize:10];
        [_singerName setBackgroundColor:[UIColor clearColor]];
        _singerName.frame = CGRectMake(60, 28, 120, 15);
        _singerName.textColor = UIColorFromRGB(0x636363);
        
        [self.contentView addSubview:_singerName];
        
        _songsBookView = [[UIImageView alloc]init];
        _songsBookView.frame = CGRectMake(0, 54, _MainScreen_Width, 55);
        _songsBookView.image = [UIImage imageNamed:@"list_black.png"];
        //_songsBookView.backgroundColor = [UIColor grayColor];
        _songsBookView.hidden = YES;
        _songsBookView.userInteractionEnabled = YES;
        
        _songBook = [UIButton buttonWithType:UIButtonTypeCustom];
        _songBook.frame = CGRectMake(25, 7, 57, 48);
        //_songBook.backgroundColor = [UIColor redColor];
        [_songBook setTitle:@"点歌" forState:UIControlStateNormal];
        [_songBook setTitle:@"已点" forState:UIControlStateDisabled];
        [_songBook setTitleEdgeInsets:UIEdgeInsetsMake(25, 0, 0, 0)];
        [_songBook setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
        _songBook.titleLabel.font = [UIFont systemFontOfSize:11];
        [_songBook setBackgroundImage:[UIImage imageNamed:@"btn_diange_c"] forState:UIControlStateNormal];
        [_songBook setBackgroundImage:[UIImage imageNamed:@"btn_diange"] forState:UIControlStateHighlighted];
        [_songBook setBackgroundImage:[UIImage imageNamed:@"btn_diange_u.png"] forState:UIControlStateDisabled];
        [_songBook addTarget:self action:@selector(pressSongsBook) forControlEvents:UIControlEventTouchUpInside];
        //_songBook.hidden = YES;
        
        [_songsBookView addSubview:_songBook];
        
        _collection = [UIButton buttonWithType:UIButtonTypeCustom];
        _collection.frame = CGRectMake(132, 7, 57, 48);
        //_collection.backgroundColor = [UIColor yellowColor];
        [_collection setTitle:@"收藏" forState:UIControlStateNormal];
        [_collection setTitleEdgeInsets:UIEdgeInsetsMake(25, 0, 0, 0)];
        [_collection setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
        [_collection setBackgroundImage:[UIImage imageNamed:@"btn_shoucang"] forState:UIControlStateHighlighted];
        _collection.titleLabel.font = [UIFont systemFontOfSize:11];
        [_collection setBackgroundImage:[UIImage imageNamed:@"btn_shoucang_c"] forState:UIControlStateNormal];
        [_collection addTarget:self action:@selector(pressSongsCollect) forControlEvents:UIControlEventTouchUpInside];
        
        [_songsBookView addSubview:_collection];
        
        _top = [UIButton buttonWithType:UIButtonTypeCustom];
        _top.frame = CGRectMake(239, 7, 57, 48);
        //_top.backgroundColor = [UIColor blueColor];
        [_top setTitle:@"置顶" forState:UIControlStateNormal];
        [_top setTitleEdgeInsets:UIEdgeInsetsMake(25, 0, 0, 0)];
        [_top setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
        _top.titleLabel.font = [UIFont systemFontOfSize:11];
        [_top setBackgroundImage:[UIImage imageNamed:@"btn_zhiding_c"] forState:UIControlStateNormal];
        [_top setBackgroundImage:[UIImage imageNamed:@"btn_zhiding"] forState:UIControlStateHighlighted];
        [_top addTarget:self action:@selector(pressSongsTop) forControlEvents:UIControlEventTouchUpInside];
        
        [_songsBookView addSubview:_top];
        
        [self.contentView addSubview:_songsBookView];
        
        _gradedView = [[UIImageView alloc]init];
        _gradedView.image = [UIImage imageNamed:@"pingfen_ic"];
        _gradedView.hidden = YES;
        
        [self.contentView addSubview:_gradedView];
        
        _listBtn = [[UIImageView alloc]init];
        _listBtn.frame = CGRectMake(290, 25, 15, 15);
        
        [self.contentView addSubview:_listBtn];
        
        _score = [[UILabel alloc]init];
        _score.font = [UIFont systemFontOfSize:18];
        _score.textColor = UIColorFromRGB(0xd2365e);
        _score.backgroundColor = [UIColor clearColor];
        [self.contentView addSubview:_score];
        
        _fen = [[UILabel alloc]init];
        _fen.text = @"分";
        _fen.font = [UIFont systemFontOfSize:12];
        
        CGSize size = [_fen.text sizeWithFont:[UIFont systemFontOfSize:12] constrainedToSize:CGSizeMake(120, 50)];
        _fen.frame = CGRectMake(252, 29, size.width,size.height);
        _fen.textColor = UIColorFromRGB(0xd2365e);
        _fen.backgroundColor = [UIColor clearColor];
        [self.contentView addSubview:_fen];
        
        _userName = [[UILabel alloc]init];
        _userName.hidden = YES;
        _userName.font = [UIFont systemFontOfSize:10];
        _userName.textColor = UIColorFromRGB(0xba5009);
        _userName.backgroundColor = [UIColor clearColor];
        [self.contentView addSubview:_userName];
        
        _zai = [[UILabel alloc]init];
        _zai.hidden = YES;
        _zai.text = @"在";
        _zai.font = [UIFont systemFontOfSize:10];
        _zai.textColor = UIColorFromRGB(0x636363);
        _zai.backgroundColor = [UIColor clearColor];
        [self.contentView addSubview:_zai];
        
        _address = [[UILabel alloc]init];
        _address.hidden = YES;
        _address.font = [UIFont systemFontOfSize:10];
        _address.textColor = UIColorFromRGB(0xba5009);
        _address.backgroundColor = [UIColor clearColor];
        [self.contentView addSubview:_address];
        
        _chang = [[UILabel alloc]init];
        _chang.hidden = YES;
        _chang.text = @"演唱";
        _chang.font = [UIFont systemFontOfSize:10];
        _chang.textColor = UIColorFromRGB(0x636363);
        _chang.backgroundColor = [UIColor clearColor];
        [self.contentView addSubview:_chang];
        
        
    }
    return self;
}

- (void)pressSongsBook{
    [_delegate songsBook:_model];
}

- (void)pressSongsCollect{
    [_delegate songsCollect:_model];
}

- (void)pressSongsTop{
    [_delegate songsTop:_model];
}

- (void)setModel:(TopSongCellModel *)model{
    _model = model;
    
    CGSize size =  [_model.topSongInfo.song.songName sizeWithFont:[UIFont systemFontOfSize:15] constrainedToSize:CGSizeMake(290, 25)];
    _songsName.frame = CGRectMake(60, 8, size.width, size.height);
    
    CGFloat scoreF = (float)_model.topSongInfo.score/10;
    
    NSString * score = [NSString stringWithFormat:@"%.1f",scoreF];
    
    size = [score sizeWithFont:[UIFont systemFontOfSize:18] constrainedToSize:CGSizeMake(120, 50)];
    _score.text = score;
    _score.frame = CGRectMake(_fen.frame.origin.x - size.width, 23, size.width, size.height);
    
    if (_model.topSongInfo.address.length > 0) {
        _zai.hidden = NO;
        size = [_zai.text sizeWithFont:[UIFont systemFontOfSize:10] constrainedToSize:CGSizeMake(100, 50)];
        _zai.frame = CGRectMake(60, 43, size.width, size.height);
        
        _address.hidden = NO;
        size = [_model.topSongInfo.address sizeWithFont:[UIFont systemFontOfSize:10] constrainedToSize:CGSizeMake(300, 50)];
        _address.text = _model.topSongInfo.address;
        _address.frame = CGRectMake(_zai.frame.origin.x+_zai.frame.size.width, 43, size.width, size.height);
        
        _chang.hidden = NO;
        size = [_chang.text sizeWithFont:[UIFont systemFontOfSize:10] constrainedToSize:CGSizeMake(100, 50)];
        _chang.frame = CGRectMake(_address.frame.origin.x + _address.frame.size.width, 43, size.width, size.height);
        
        
        if (_model.topSongInfo.user) {
            _userName.hidden = NO;
            //_zai.hidden = NO;
            
            size = [_model.topSongInfo.user.name sizeWithFont:[UIFont systemFontOfSize:10] constrainedToSize:CGSizeMake(160, 50)];
            _userName.text = _model.topSongInfo.user.name;
            _userName.frame = CGRectMake(60, 43, size.width, size.height);
            
            size = [_zai.text sizeWithFont:[UIFont systemFontOfSize:10] constrainedToSize:CGSizeMake(160, 50)];
            _zai.frame = CGRectMake(_userName.frame.size.width+_userName.frame.origin.x, 43, size.width, size.height);
            
            size = [_model.topSongInfo.address sizeWithFont:[UIFont systemFontOfSize:10] constrainedToSize:CGSizeMake(300, 50)];
            _address.text = _model.topSongInfo.address;
            _address.frame = CGRectMake(_zai.frame.size.width+ _zai.frame.origin.x, 43, size.width, size.height);
            
            size = [_chang.text sizeWithFont:[UIFont systemFontOfSize:10] constrainedToSize:CGSizeMake(100, 50)];
            _chang.frame = CGRectMake(_address.frame.origin.x+_address.frame.size.width, 43, size.width, size.height);
        }else {
            _userName.hidden = YES;
            //_zai.hidden = YES;
        }
    }else{
        _userName.hidden = YES;
        _zai.hidden = YES;
        _address.hidden = YES;
        _chang.hidden = YES;
    }
    
    if (_model.topSongInfo.song.isOrdered) {
        _songBook.enabled = NO;
        _songBook.titleLabel.alpha = 0.3;
    }else {
        _songBook.enabled = YES;
        _songBook.titleLabel.alpha = 1;
    }
    
    if (_model.isHigher == YES) {
        _songsBookView.hidden = NO;
        _listBtn.image = [UIImage imageNamed:@"btn_list_top.png"];
    }else {
        _songsBookView.hidden = YES;
        _listBtn.image = [UIImage imageNamed:@"btn_list_bottom.png"];
    }
    
    if (_model.topSongInfo.song.songIsScore == 1) {
        _gradedView.hidden = NO;
        _gradedView.frame = CGRectMake(_songsName.frame.size.width+63, 12, 13, 11);
    }else {
        _gradedView.hidden = YES;
    }
    
    if (_model.topSongInfo.song.isOrdered == YES) {
        _alreadyView.hidden = NO;
    }else {
        _alreadyView.hidden = YES;
    }
    
    if (_model.topSongInfo.user == nil) {
        
        //PLog(@"%@",_model.topSongInfo.song.songArtistPhoto);
        NSArray * array = [_model.topSongInfo.song.songArtistPhoto componentsSeparatedByString:@"/"];
        NSString * urlStr = @"http:";
        
        for (int i = 0; i < array.count; i++) {
            if (i==0) {
                continue;
            }
            NSString * s = [array objectAtIndex:i];
            
            urlStr = [NSString stringWithFormat:@"%@/%@",urlStr,[PCommonUtil encodeUrlParameter:s]];
        }
        
        [_userHeadImage setImageWithURL:[NSURL URLWithString:urlStr] placeholderImage:[UIImage imageNamed:@"pic_top100_admin.png"]];
        
    }else{
        
        [_userHeadImage setImageWithURL:[NSURL URLWithString:[CommenTools GetURLWithResolutionScaleTransfered:_model.topSongInfo.user.headphoto scale:0]] placeholderImage:[UIImage imageNamed:@"pic_top100_admin.png"]];
        
        //NSLog(@"%@",_model.topSongInfo.user.headphoto);
    }
    
    //PLog(@"%@",_model.topSongInfo.user.headphoto);
    _songsName.text = _model.topSongInfo.song.songName;
    _singerName.text = _model.topSongInfo.song.songArtist;
}



- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}



@end
