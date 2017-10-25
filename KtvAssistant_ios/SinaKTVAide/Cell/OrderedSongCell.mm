//
//  OrderedSongCell.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/18.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "OrderedSongCell.h"
#import "KTVBaseConfig.h"

@implementation OrderedSongCell

#define Y_OPERATION_VIEW 42

@synthesize mInfo;

- (void)awakeFromNib {
    // Initialization code
    [self.contentView setFrame:CGRectMake(self.contentView.frame.origin.x, self.contentView.frame.origin.y, _MainScreen_Width, self.contentView.frame.size.height)];
    [self.mUiSongOperation setFrame:CGRectMake(0,Y_OPERATION_VIEW , self.contentView.frame.size.width, self.mUiSongOperation.frame.size.height)];
    [self.mUiSongOperation setHidden:YES];
    [self.contentView addSubview:self.mUiSongOperation];
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

-(void) setInfo:(KTVOrderedSongInfo*)info withIndex:(NSUInteger)index
{
    if (info) {
        mInfo = info;
        [self.mUiName setText:mInfo.songname];
        [self.mUiSinger setText:mInfo.singername];
        
        if(index == 0)//当前播放歌曲不能操作
        {
            [self.mUiStatus setImage:[UIImage imageNamed:@"dangqian.png"]];
            [self.mUiTop setHidden:YES];//当前歌曲无法置顶
        }
        else
        {
            [self.mUiStatus setImage:[UIImage imageNamed:@"yidian.png"]];
            if(index == 1)//第一首歌曲无法置顶
                [self.mUiTop setHidden:YES];
            else
                [self.mUiTop setHidden:NO];
        }
        
        if (info.isHigher)
        {
            [self.mUiMoreTip setImage:[UIImage imageNamed:@"btn_list_top.png"]];
            [self.mUiSongOperation setHidden:NO];
        }
        else
        {
            [self.mUiMoreTip setImage:[UIImage imageNamed:@"btn_list_bottom.png"]];
            [self.mUiSongOperation setHidden:YES];
        }
    }
}

- (IBAction)onTop:(id)sender {
    if (self.mCallBack && [self.mCallBack respondsToSelector:@selector(onOrderedTop:)]) {
        [self.mCallBack onOrderedTop:self.mInfo];
    }
}

- (IBAction)onDelete:(id)sender {
    if (self.mCallBack && [self.mCallBack respondsToSelector:@selector(onOrderedDelete:)]) {
        [self.mCallBack onOrderedDelete:self.mInfo];
    }
}

- (IBAction)onCollect:(id)sender {
    if (self.mCallBack && [self.mCallBack respondsToSelector:@selector(onOrderedCollect:)]) {
        [self.mCallBack onOrderedCollect:self.mInfo];
    }
}
@end
