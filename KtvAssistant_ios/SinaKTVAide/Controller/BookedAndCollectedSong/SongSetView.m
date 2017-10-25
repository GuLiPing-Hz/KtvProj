//
//  SongSetController.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-14.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "SongSetView.h"
#import "ASIHTTPRequest.h"
#import "PKtvAssistantAPI.h"

@implementation SongSetView



-(id)init
{
    self = [super init];
    if (self) {
    }
    return self;
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
    }
    return self;
}

-(void)setAllBtnBack
{
    [self setButtonBack:_voice1 tag:101];
    [self setButtonBack:_voice2 tag:102];
    [self setButtonBack:_mic1 tag:103];
    [self setButtonBack:_mic2 tag:104];
    [self setButtonBack:_playAndPause tag:105];
    [self setButtonBack:_scoreMode tag:106];
    [self setButtonBack:_replay tag:107];
    [self setButtonBack:_singMode tag:108];
    [self setButtonBack:_tone1 tag:109];
    [self setButtonBack:_tone2 tag:110];
    [self setButtonBack:_effect1 tag:111];
    [self setButtonBack:_effect2 tag:112];
    [self setButtonBack:_effect3 tag:113];
    [self setButtonBack:_effect4 tag:114];
    [self setButtonBack:_effect5 tag:115];
    [self setButtonBack:_effect6 tag:116];
    [self setButtonBack:_effect7 tag:117];
    [self setButtonBack:_effect8 tag:118];
    _effect8.tag=118;
    [self setButtonBack:_mute tag:119];
    [self setButtonBack:_tone3 tag:120];
}

-(void)setButtonBack:(UIButton *)btn tag:(NSInteger)tag
{
    if (btn == _singMode)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundCtrl_btn_track_1.png"] forState:UIControlStateHighlighted];
    }
    else if (btn == _scoreMode)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundctrl_btn_score_1.png"] forState:UIControlStateHighlighted];
    }
    else if (btn == _playAndPause)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundctrl_btn_play&pause_1.png"] forState:UIControlStateHighlighted];
    }
    else if (btn == _replay)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundCtrl_btn_replay_1.png"] forState:UIControlStateHighlighted];
    }
    else if (btn == _mute)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundctrl_btn_mute_1.png"] forState:UIControlStateHighlighted];
    }
    else if (btn == _effect1 || btn == _effect5)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundCtrl_btn_eq1_1.png"] forState:UIControlStateHighlighted];
    }
    else if (btn == _effect2 || btn == _effect6)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundCtrl_btn_eq2_1.png"] forState:UIControlStateHighlighted];
    }
    else if (btn == _effect3 || btn == _effect7)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundCtrl_btn_eq3_1.png"] forState:UIControlStateHighlighted];
    }
    else if (btn == _effect4 || btn == _effect8)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundCtrl_btn_eq4_1.png"] forState:UIControlStateHighlighted];
    }
    else if (btn == _voice1 || btn == _tone1)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundCtrl_btn_add_b_1.png"] forState:UIControlStateHighlighted];
    }
    else if (btn == _voice2 || btn == _tone2)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundCtrl_btn_subtract_b_1.png"] forState:UIControlStateHighlighted];
    }
    else if (btn == _mic1)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundCtrl_btn_add_g_1.png"] forState:UIControlStateHighlighted];
    }
    else if (btn == _mic2)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundCtrl_btn_subtract_g_1.png"] forState:UIControlStateHighlighted];
    }
    else if (btn == _tone3)
    {
        [btn setBackgroundImage:[UIImage imageNamed:@"soundCtrl_btn_tone0_1.png"] forState:UIControlStateHighlighted];
    }
    
    [btn setTitleColor:UIColorFromRGB(0xe64b89) forState:UIControlStateHighlighted];
    btn.tag = tag;
    btn.layer.cornerRadius = 25;
    btn.layer.masksToBounds = true;
    btn.alpha = 1.0f;
}

-(IBAction)btnClick:(id)sender
{
    if ([UserSessionManager isRoomAlreadyLogin])
    {
        RoomInfo *info =[UserSessionManager getInstance].currentRoomInfo;
        NSString *encodeUrl;
        UIButton *btn = (UIButton *)sender;
        switch (btn.tag) {
            case 101: //音量加
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:1 mic:0 tone:0 effect:-1];
                break;
            case 102: //音量减
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:-1 mic:0 tone:0 effect:-1];
                break;
            case 103: //麦克加
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:0 mic:1 tone:0 effect:-1];
                break;
            case 104: //麦克减
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:0 mic:-1 tone:0 effect:-1];
                break;
            case 105: //播放&暂停
                encodeUrl =[PKtvAssistantAPI getSetPauseUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword isPause:0];
                break;
            case 106: //评分模式
                encodeUrl =[PKtvAssistantAPI getSetScoreModeUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword singMode:0];
                break;
            case 107: //重唱
                encodeUrl =[PKtvAssistantAPI getSetReplayUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword];
                break;
            case 108: //原伴唱
                encodeUrl =[PKtvAssistantAPI getSetSingModeUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword singMode:0];
                break;
            case 109: //音调加
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:0 mic:0 tone:1 effect:-1];
                break;
            case 110: //音调减
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:0 mic:0 tone:-1 effect:-1];
                break;
            case 111: //魔音快歌
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:0 mic:0 tone:0 effect:0];
                break;
            case 112: //魔音顶尖
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:0 mic:0 tone:0 effect:1];
                break;
            case 113: //魔音唱将
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:0 mic:0 tone:0 effect:2];
                break;
            case 114: //魔音k歌
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:0 mic:0 tone:0 effect:3];
                break;
            case 115: //专业快歌
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:0 mic:0 tone:0 effect:4];
                break;
            case 116: //专业顶尖
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:0 mic:0 tone:0 effect:5];
                break;
            case 117: //专业唱将
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:0 mic:0 tone:0 effect:6];
                break;
            case 118: //专业k歌
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:0 mic:0 tone:0 effect:7];
                break;
            case 119: //静音
                encodeUrl = [PKtvAssistantAPI getSetMuteUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword isMute:0];
                break;
            case 120: //音调3
                encodeUrl = [PKtvAssistantAPI getSetVoicePadUrl:info.ktvId roomID:info.roomId roomPassword:info.roomPassword voice:0 mic:0 tone:2 effect:-1];
                break;
            default:
                break;
        }
        NSURL *url = [[NSURL alloc] initWithString:encodeUrl];
        NSMutableURLRequest *request=[[NSMutableURLRequest alloc]initWithURL:url];
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain",nil]];
        AFHTTPRequestOperation *operation =[[AFHTTPRequestOperation alloc]initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            NSDictionary *dic = [PCommonUtil NSDictionaryFromJSONData:responseObject];
            NSString *status = [dic objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame ) {
                
            }
            else
            {
                NSString *msg = [dic objectForKey:@"msg"];
                //NSLog(@"%@",msg);
            }
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        [operation start];
    }
    else
    {
        [CommenTools showAlertViewWithErrorMessage:@"你还没有进入包厢!"];
    }
}

@end
