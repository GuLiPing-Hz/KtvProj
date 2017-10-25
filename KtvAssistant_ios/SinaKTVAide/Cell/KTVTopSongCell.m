//
//  KTVTopSongCell.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/20.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "KTVTopSongCell.h"
#import "CommenTools.h"

@implementation KTVTopSongCell

#define Y_OPERATION_VIEW (76-8)

- (void)awakeFromNib {
    // Initialization code
    [self.mUIPic.layer setCornerRadius:12.5];
    [self.mUIPic.layer setBorderWidth:1];
    [self.mUIPic.layer setBorderColor:UIColorFromRGB(0xc6c6c7).CGColor];
    [self.mUIPic.layer setMasksToBounds:YES];
    
    [self.contentView setFrame:CGRectMake(self.contentView.frame.origin.x, self.contentView.frame.origin.y, _MainScreen_Width, self.contentView.frame.size.height)];
    [self.mUIOperation setFrame:CGRectMake(0,Y_OPERATION_VIEW , self.contentView.frame.size.width, self.mUIOperation.frame.size.height)];
    [self.mUIOperation setHidden:YES];
    [self.contentView addSubview:self.mUIOperation];
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

-(void)setCorrectOrderButton:(BOOL)isOrdered
{
    if(isOrdered)
    {
        [self.mUIOrder setImage:[UIImage imageNamed:@"cancel_btn_0"] forState:UIControlStateNormal];
        [self.mUIOrder setImage:[UIImage imageNamed:@"cancel_btn_1"] forState:UIControlStateHighlighted];
    }
    else
    {
        [self.mUIOrder setImage:[UIImage imageNamed:@"vod_btn_0"] forState:UIControlStateNormal];
        [self.mUIOrder setImage:[UIImage imageNamed:@"vod_btn_1"] forState:UIControlStateHighlighted];
    }
}

-(void)setTopSongInfo:(TopSongCellModel*)info
{
    self.mInfo = info;
    if (info) {
        
        if(info.isHigher)
            [self.mUIOperation setHidden:NO];
        else
            [self.mUIOperation setHidden:YES];
        
        if(info.topSongInfo.song.isOrdered)
        {
            [self.mUISongStatus setHidden:NO];
            [self setCorrectOrderButton:YES];
        }
        else
        {
            [self.mUISongStatus setHidden:YES];
            [self setCorrectOrderButton:NO];
        }
        
        [self.mUISongName setText:info.topSongInfo.song.songName];
        [self.mUISongSinger setText:info.topSongInfo.song.songArtist];
        if(info.topSongInfo.user)
        {
            [self.mUIUserName setText:info.topSongInfo.user.name];
            
            //使用用户头像
             [self.mUIPic setImageWithURL:[NSURL URLWithString:[CommenTools GetURLWithResolutionScaleTransfered:info.topSongInfo.user.headphoto scale:0]] placeholderImage:[UIImage imageNamed:@"userFace_normal.png"]];
        }
        else
        {
            [self.mUIUserName setText:@"匿名"];
            
            //使用歌手头像
            NSArray * array = [info.topSongInfo.song.songArtistPhoto componentsSeparatedByString:@"/"];
            NSString * urlStr = @"http:";
            
            for (int i = 0; i < array.count; i++) {
                if (i==0) {
                    continue;
                }
                NSString * s = [array objectAtIndex:i];
                urlStr = [NSString stringWithFormat:@"%@/%@",urlStr,[PCommonUtil encodeUrlParameter:s]];
            }
            
            [self.mUIPic setImageWithURL:[NSURL URLWithString:urlStr] placeholderImage:[UIImage imageNamed:@"userFace_normal.png"]];
        }
        //包厢
        if([info.topSongInfo.address compare:@""] == NSOrderedSame)
            [self.mUIRoom setText:@"未在包厢"];
        else
            [self.mUIRoom setText:info.topSongInfo.address];
        //分数
        CGFloat fscore = info.topSongInfo.score/10.0f;
        [self.mUIScore setText:[NSString stringWithFormat:@"%.1f",fscore]];
    }
}

//调整了下收藏跟置顶的位置，所以这里代码里弄反一下
- (IBAction)onTop:(id)sender {
    if (self.mCallBack && [self.mCallBack respondsToSelector:@selector(onTopCollect:)]) {
        [self.mCallBack onTopCollect:self.mInfo];
    }
}

- (IBAction)onCollect:(id)sender {
    if (self.mCallBack && [self.mCallBack respondsToSelector:@selector(onTopTop:)]) {
        [self.mCallBack onTopTop:self.mInfo];
    }
}

- (IBAction)onOrderOrCancel:(id)sender {
    if(self.mInfo.topSongInfo.song.isOrdered)
    {
        if (self.mCallBack && [self.mCallBack respondsToSelector:@selector(onTopDelete:)]) {
            [self.mCallBack onTopDelete:self.mInfo];
        }
    }
    else
    {
        if (self.mCallBack && [self.mCallBack respondsToSelector:@selector(onTopOrder:view:point:)]) {
            UIView* view = (UIView*)sender;
            CGPoint point = CGPointMake(view.center.x, view.center.y);
            [self.mCallBack onTopOrder:self.mInfo view:self point:point];
        }
    }
}
@end
