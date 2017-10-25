//
//  SongsBookCell.m
//  SinaKTVAide
//
//  Created by WangBingJian on 13-12-4.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "SongsBookCell.h"

@implementation SongsBookCell

-(void)setCorrectOrderButton:(BOOL)isOrdered btn:(UIButton*) btn
{
    if(isOrdered)
    {
        [_songBook setBackgroundImage:[UIImage imageNamed:@"cancel_btn_0"] forState:UIControlStateNormal];
        [_songBook setBackgroundImage:[UIImage imageNamed:@"cancel_btn_1"] forState:UIControlStateHighlighted];
    }
    else
    {
        [_songBook setBackgroundImage:[UIImage imageNamed:@"vod_btn_0"] forState:UIControlStateNormal];
        [_songBook setBackgroundImage:[UIImage imageNamed:@"vod_btn_1"] forState:UIControlStateHighlighted];
    }
}

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        // Initialization code
        self.selectionStyle = UITableViewCellSelectionStyleNone;
        //self.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        [self.contentView setBackgroundColor:UIColorFromRGB(0xf0f1f5)];
        
        _alreadyView = [[UIImageView alloc]init];
        _alreadyView.frame = CGRectMake(0, 0, 20, 20);
        _alreadyView.image = [UIImage imageNamed:@"yidian.png"];
        _alreadyView.hidden = YES;
        
        [self.contentView addSubview:_alreadyView];
        
        _lineView = [[UIImageView alloc]init];
        _lineView.frame = CGRectMake(0, 49,_MainScreen_Width, 1);
        _lineView.image = [UIImage imageNamed:@"list_line.png"];
        [self.contentView addSubview:_lineView];
        
        _songsName = [[UILabel alloc]init];
        _songsName.font = [UIFont systemFontOfSize:15];
        [_songsName setBackgroundColor:[UIColor clearColor]];
        _songsName.textColor = UIColorFromRGB(0x212121);
        [self.contentView addSubview:_songsName];
        
        _singerName = [[UILabel alloc]init];
        _singerName.font = [UIFont systemFontOfSize:10];
        [_singerName setBackgroundColor:[UIColor clearColor]];
        _singerName.frame = CGRectMake(23, 28, 120, 15);
        _singerName.textColor = UIColorFromRGB(0x636363);
        [self.contentView addSubview:_singerName];
        
        songOperationView = [[UIImageView alloc]init];
        songOperationView.frame = CGRectMake(0, 44, _MainScreen_Width, 55);
        songOperationView.image = [UIImage imageNamed:@"list_black.png"];
        songOperationView.hidden = YES;
        songOperationView.userInteractionEnabled = YES;
        
        _top = [UIButton buttonWithType:UIButtonTypeCustom];
        _top.frame = CGRectMake(_MainScreen_Width/2, 7, _MainScreen_Width/2, 48);
        //_top.backgroundColor = [UIColor blueColor];
        [_top setTitle:@"置顶" forState:UIControlStateNormal];
        [_top setTitleEdgeInsets:UIEdgeInsetsMake(25, -80, 0, 0)];
        [_top setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
        _top.titleLabel.font = [UIFont systemFontOfSize:11];
        [_top setImage:[UIImage imageNamed:@"btn_zhiding_c"] forState:UIControlStateNormal];
        [_top setImage:[UIImage imageNamed:@"btn_zhiding"] forState:UIControlStateHighlighted];
        [_top addTarget:self action:@selector(onTop:) forControlEvents:UIControlEventTouchUpInside];
        [songOperationView addSubview:_top];
        [self.contentView addSubview:songOperationView];
        
        _collection = [UIButton buttonWithType:UIButtonTypeCustom];
        _collection.frame = CGRectMake(0, 7, _MainScreen_Width/2, 48);
        //_collection.backgroundColor = [UIColor yellowColor];
        [_collection setTitle:@"收藏" forState:UIControlStateNormal];
        [_collection setTitleEdgeInsets:UIEdgeInsetsMake(25, -80, 0, 0)];
        [_collection setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
        [_collection setImage:[UIImage imageNamed:@"btn_shoucang"] forState:UIControlStateHighlighted];
        _collection.titleLabel.font = [UIFont systemFontOfSize:11];
        [_collection setImage:[UIImage imageNamed:@"btn_shoucang_c"] forState:UIControlStateNormal];
        [_collection addTarget:self action:@selector(onCollect:) forControlEvents:UIControlEventTouchUpInside];
        [songOperationView addSubview:_collection];
        
        _gradedView = [[UIImageView alloc]init];
        _gradedView.image = [UIImage imageNamed:@"score_ic"];
        _gradedView.hidden = YES;
        [self.contentView addSubview:_gradedView];
        
        _listBtn = [[UIImageView alloc]init];
        _listBtn.frame = CGRectMake(_MainScreen_Width-30, 15, 15, 15);
        [self.contentView addSubview:_listBtn];
        
        _songBook = [UIButton buttonWithType:UIButtonTypeCustom];
        _songBook.frame = CGRectMake(_listBtn.frame.origin.x-60, 10, 55, 25);
        [self setCorrectOrderButton:NO btn:_songBook];
        [_songBook addTarget:self action:@selector(onOrderOrDelete:) forControlEvents:UIControlEventTouchUpInside];
        [self.contentView addSubview:_songBook];
    }
    return self;
}


- (void)onTop:(id)sender {
    if (self.delegate && [self.delegate respondsToSelector:@selector(onTop:)]) {
        [self.delegate onTop:self.model];
    }
}

- (void)onOrderOrDelete:(id)sender {
    if(self.model.songInfo.isOrdered)
    {
        if (self.delegate && [self.delegate respondsToSelector:@selector(onDelete:)]) {
            [self.delegate onDelete:self.model];
        }
    }
    else
    {
        //_songBook
        if (self.delegate && [self.delegate respondsToSelector:@selector(onOrder:view:point:)]) {
            UIView* view = (UIView*)sender;
            CGPoint point = CGPointMake(view.frame.size.width/2.0f, view.frame.size.height/2.0f);
            point = [view convertPoint:point toView:self];
            [self.delegate onOrder:self.model view:self point:point];
            
        }
    }

}

- (void)onCollect:(id)sender {
    if (self.delegate && [self.delegate respondsToSelector:@selector(onCollect:)]) {
        [self.delegate onCollect:self.model];
    }
}

//- (void)pressFirstBtn{
//    [_delegate firstAction:_model];
//}
//
//- (void)pressMiddleBtn{
//    [_delegate middleAction:_model];
//}
//
//- (void)pressLastBtn{
//    [_delegate lastAction:_model];
//}

#define X_FONT 20
#define MAXSIZE_FONT 190
- (void)setModel:(SongsBookModel *)model{
    
    _model = model;
    
    CGSize size =  [_model.songInfo.songName sizeWithFont:[UIFont systemFontOfSize:15] constrainedToSize:CGSizeMake(MAXSIZE_FONT, 25)];
    //CGFloat width_max = size.width > MAXSIZE_FONT?MAXSIZE_FONT:size.width;
    _songsName.frame = CGRectMake(X_FONT, 8, size.width, size.height);
    
    if (_model.cellType == TYPECELL_CommonOrderSong) {
//        if (_model.songInfo.isOrdered == YES) {
//            _songBook.enabled = NO;
//            _songBook.titleLabel.alpha = 0.3;
//        }else{
//            _songBook.enabled = YES;
//            _songBook.titleLabel.alpha = 1;
//        }
    }else if(_model.cellType == TYPECELL_OrderedSong){
        [_songBook setTitle:@"删除" forState:UIControlStateNormal];
        [_songBook setImage:[UIImage imageNamed:@"btn_delete_c.png"] forState:UIControlStateNormal];
        [_songBook setImage:[UIImage imageNamed:@"btn_delete.png"] forState:UIControlStateHighlighted];
    }else if (_model.cellType == TYPECELL_CollectSong){
        [_collection setTitle:@"删除" forState:UIControlStateNormal];
        [_collection setImage:[UIImage imageNamed:@"btn_delete_c"] forState:UIControlStateNormal];
        [_collection setImage:[UIImage imageNamed:@"btn_delete"] forState:UIControlStateHighlighted];
        
        if (_model.songInfo.isOrdered == YES) {
            _songBook.enabled = NO;
            _songBook.titleLabel.alpha = 0.3;
        }else{
            _songBook.enabled = YES;
            _songBook.titleLabel.alpha = 1;
        }
    }
    
    if (_model.isHigher == YES) {
        songOperationView.hidden = NO;
        _listBtn.image = [UIImage imageNamed:@"btn_list_top.png"];
    }else {
        songOperationView.hidden = YES;
        _listBtn.image = [UIImage imageNamed:@"btn_list_bottom.png"];
    }
    
    if (_model.songInfo.songIsScore == 1) {
        _gradedView.hidden = NO;
        _gradedView.frame = CGRectMake(_songsName.frame.size.width+22, 12.5f, 19, 11);
    }else {
        _gradedView.hidden = YES;
    }
    
    if (_model.songInfo.isOrdered == YES) {
        _alreadyView.hidden = NO;
        [self setCorrectOrderButton:YES btn:_songBook];
    }else {
        _alreadyView.hidden = YES;
        [self setCorrectOrderButton:NO btn:_songBook];
    }
    
    _songsName.text = _model.songInfo.songName;
    _singerName.text = _model.songInfo.songArtist;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

-(void)setCurrentSongImg:(NSInteger)index
{
    self.mIndex = index;
    if (index == 0)
        _alreadyView.image = [UIImage imageNamed:@"dangqian.png"];
    else
        _alreadyView.image = [UIImage imageNamed:@"yidian.png"];
}

@end
