//
//  KTVSongPreBookCell.m
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-11-1.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "KTVSongPreBookCell.h"
#import "PKtvAssistantAPI.h"
#import "PDatabaseManager.h"

@implementation KTVSongPreBookCell
@synthesize songHasVodImage = _songHasVodImage,songNameLabel = _songNameLabel,aritstNameLabel = _aritstNameLabel,commentImage = _commentImage,leftButton = _leftButton,toTopButton = _toTopButton,song = _song,deleteSongButton = _deleteSongButton;
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        // Initialization code
    }
    return self;
}

- (void)awakeFromNib
{
    [super awakeFromNib];
    self.songHasVodImage.hidden = YES;
    self.commentImage.hidden = YES;
    self.songNameLabel.font = [UIFont systemFontOfSize:15.0f];
    self.songNameLabel.textColor = UIColorFromRGB(0x212121);
    self.aritstNameLabel.font = [UIFont systemFontOfSize:10.0f];
    self.aritstNameLabel.textColor = UIColorFromRGB(0x636363);
    self.leftButton.titleLabel.font = [UIFont systemFontOfSize:12.0];
    self.toTopButton.titleLabel.font = [UIFont systemFontOfSize:12.0];
    
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];

    self.songNameLabel.text = self.song.songName;
    self.aritstNameLabel.text = self.song.songArtist;
    if (self.song.songIsScore  == 1) {
        self.commentImage.hidden = NO;
        CGSize size = [self.aritstNameLabel.text sizeWithFont:self.aritstNameLabel.font];
        [self.commentImage setFrame:CGRectMake(self.aritstNameLabel.frame.origin.x+size.width+5, self.aritstNameLabel.frame.origin.y, 13, 11)];
    }
    if (self.song.isOrdered) {
        self.songHasVodImage.hidden = NO;
        [self.leftButton setBackgroundImage:[UIImage imageNamed:@"bth_5.png"] forState:UIControlStateNormal];
        [self.leftButton setBackgroundImage:[UIImage imageNamed:@"bth_5_c.png"] forState:UIControlStateHighlighted];
        [self.leftButton setTitle:@"已添加" forState:UIControlStateNormal];
        [self.leftButton setEnabled:NO];
        
    }
}

-(IBAction)vodSongAction:(id)sender
{
    RoomInfo *currentRoomInfo = [[UserSessionManager getInstance] currentRoomInfo];
    BOOL isScore = (self.song.songIsScore  == 1);
    NSString *currentSongId = [NSString stringWithFormat:@"%ld",self.song.songId];
    NSString *encodeUrl = [PKtvAssistantAPI getOrderSongUrl:[[UserSessionManager getInstance] currentUser].uid ktvID:currentRoomInfo.ktvId roomId:currentRoomInfo.roomId roomPassword:currentRoomInfo.roomPassword songId:currentSongId songName:self.song.songName artistName:self.song.songArtist isScore:isScore];
    
    NSURL *url = [NSURL URLWithString:encodeUrl];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData2:responseObject];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            //
            self.song.isOrdered = YES;
            self.songHasVodImage.hidden = NO;
            [self.leftButton setBackgroundImage:[UIImage imageNamed:@"bth_5.png"] forState:UIControlStateNormal];
            [self.leftButton setBackgroundImage:[UIImage imageNamed:@"bth_5_c.png"] forState:UIControlStateHighlighted];
            [self.leftButton setTitle:@"已添加" forState:UIControlStateNormal];
            self.leftButton.enabled = NO;
            
            //通知更新金币信息
            NSString *result  = [results objectForKey:@"result"];
            if (![result isEqualToString:@"error"] && [result integerValue]>0 && [result integerValue]!= [UserSessionManager getInstance].currentUser.gold)
            {
                [[UserSessionManager getInstance] updateCurrentUserGold:[result integerValue]];
            }
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doOrderSong failed...%@", msg);
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doOrderSong errorcode....%@",errorCode);
            if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForRoomClosed] || [errorCode isEqualToString:kPKtvAssistantErrorCodeForNoRoom]) {
                [[NSNotificationCenter defaultCenter] postNotificationName:RECEIVE_ROOM_HAS_CLOSED_NOTIFICATION object:nil];
            }else{
                [CommenTools showAlertViewWithTitle:errorCode];
            }
        }
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        PLog(@"doOrderSong failed...");
        [CommenTools  showNetworkDisconnectedAlertView];
    }];
    [operation start];
    
    /*
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        NSLog(@"doOrderSong complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            //
            self.song.isOrdered = YES;
            self.songHasVodImage.hidden = NO;
            [self.leftButton setBackgroundImage:[UIImage imageNamed:@"bth_5.png"] forState:UIControlStateNormal];
            [self.leftButton setBackgroundImage:[UIImage imageNamed:@"bth_5_c.png"] forState:UIControlStateHighlighted];
            [self.leftButton setTitle:@"已添加" forState:UIControlStateNormal];
            self.leftButton.enabled = NO;
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doOrderSong failed...%@", msg);
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doOrderSong errorcode....%@",errorCode);
            if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForRoomClosed]) {
                [[NSNotificationCenter defaultCenter] postNotificationName:RECEIVE_ROOM_HAS_CLOSED_NOTIFICATION object:nil];
            }else{
                [CommenTools showAlertViewWithTitle:errorCode];
            }
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doOrderSong failed...");
        [CommenTools  showNetworkDisconnectedAlertView];
    }];
    [resquest startAsynchronous];
    */
    
}
-(IBAction)toTopAction:(id)sender
{
    RoomInfo *currentRoomInfo = [[UserSessionManager getInstance] currentRoomInfo];
    NSString *currentSongId = [NSString stringWithFormat:@"%ld",self.song.songId];
    BOOL isScore = (self.song.songIsScore  == 1);
    if (!self.song.isOrdered) {
        NSString *encodeUrl = [PKtvAssistantAPI getAddAndSetSongToTopUrl:[[UserSessionManager getInstance] currentUser].uid ktvID:currentRoomInfo.ktvId roomId:currentRoomInfo.roomId roomPassword:currentRoomInfo.roomPassword songId:currentSongId songName:self.song.songName artistName:self.song.songArtist isScore:isScore];
        
        
        NSURL *url = [NSURL URLWithString:encodeUrl];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
        
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData2:responseObject];
            
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                self.song.isOrdered = YES;
                self.songHasVodImage.hidden = NO;
                [self.leftButton setBackgroundImage:[UIImage imageNamed:@"bth_5.png"] forState:UIControlStateNormal];
                [self.leftButton setBackgroundImage:[UIImage imageNamed:@"bth_5_c.png"] forState:UIControlStateHighlighted];
                [self.leftButton setTitle:@"已添加" forState:UIControlStateNormal];
                self.leftButton.enabled = NO;
                [SVProgressHUD showSuccessWithStatus: @"置顶成功" duration:1.5];
                
                //通知更新金币信息
                NSString *result  = [results objectForKey:@"result"];
                if (![result isEqualToString:@"error"] && [result integerValue]>0 && [result integerValue]!= [UserSessionManager getInstance].currentUser.gold)
                {
                    [[UserSessionManager getInstance] updateCurrentUserGold:[result integerValue]];
                }
                
            } else {
                NSString *msg = [results objectForKey:@"msg"];
                PLog(@"doAddAndSetSongToTop failed...%@", msg);
                NSString *errorCode = [results objectForKey:@"errorcode"];
                PLog(@"doOrderSong errorcode....%@",errorCode);
                if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForRoomClosed] || [errorCode isEqualToString:kPKtvAssistantErrorCodeForNoRoom]) {
                    [[NSNotificationCenter defaultCenter] postNotificationName:RECEIVE_ROOM_HAS_CLOSED_NOTIFICATION object:nil];
                }else{
                    [CommenTools showAlertViewWithTitle:errorCode];
                }
            }
            
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            PLog(@"doAddAndSetSongToTop failed...");
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        [operation start];
        
        /*
        __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
        [resquest setCompletionBlock:^{
            NSLog(@"doAddAndSetSongToTop complete...");
            NSString *response = [resquest responseString];
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
            
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                self.song.isOrdered = YES;
                self.songHasVodImage.hidden = NO;
                [self.leftButton setBackgroundImage:[UIImage imageNamed:@"bth_5.png"] forState:UIControlStateNormal];
                [self.leftButton setBackgroundImage:[UIImage imageNamed:@"bth_5_c.png"] forState:UIControlStateHighlighted];
                [self.leftButton setTitle:@"已添加" forState:UIControlStateNormal];
                self.leftButton.enabled = NO;
                [SVProgressHUD showSuccessWithStatus: @"置顶成功" duration:1.5];
                
            } else {
                NSString *msg = [results objectForKey:@"msg"];
                PLog(@"doAddAndSetSongToTop failed...%@", msg);
                NSString *errorCode = [results objectForKey:@"errorcode"];
                PLog(@"doOrderSong errorcode....%@",errorCode);
                if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForRoomClosed]) {
                    [[NSNotificationCenter defaultCenter] postNotificationName:RECEIVE_ROOM_HAS_CLOSED_NOTIFICATION object:nil];
                }else{
                    [CommenTools showAlertViewWithTitle:errorCode];
                }
            }
            
        }];
        [resquest setFailedBlock:^{
            PLog(@"doAddAndSetSongToTop failed...");
            [CommenTools showNetworkDisconnectedAlertView];
        }];
        [resquest startAsynchronous];
        */
        
    }else{
        
        NSString *encodeUrl = [PKtvAssistantAPI getSetSongToTopUrl:currentRoomInfo.ktvId roomId:currentRoomInfo.roomId roomPassword:currentRoomInfo.roomPassword songId:currentSongId];
        
        NSURL *url = [NSURL URLWithString:encodeUrl];
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
        
        [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
        AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
        [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
            
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                //
                [SVProgressHUD showSuccessWithStatus: @"置顶成功" duration:1.5];
            } else {
                NSString *msg = [results objectForKey:@"msg"];
                PLog(@"doSetSongToTop failed...%@", msg);
                NSString *errorCode = [results objectForKey:@"errorcode"];
                PLog(@"doOrderSong errorcode....%@",errorCode);
                if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForRoomClosed] || [errorCode isEqualToString:kPKtvAssistantErrorCodeForNoRoom]) {
                    [[NSNotificationCenter defaultCenter] postNotificationName:RECEIVE_ROOM_HAS_CLOSED_NOTIFICATION object:nil];
                }else{
                    [CommenTools showAlertViewWithTitle:errorCode];
                }
            }
            
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            PLog(@"doSetSongToTop failed...");
            [CommenTools  showNetworkDisconnectedAlertView];
        }];
        [operation start];
        
        /*
        __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
        [resquest setCompletionBlock:^{
            NSLog(@"doSetSongToTop complete...");
            NSString *response = [resquest responseString];
            NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
            
            NSString *status = [results objectForKey:@"status"];
            if ([status compare:@"1"] == NSOrderedSame) {
                //
                [SVProgressHUD showSuccessWithStatus: @"置顶成功" duration:1.5];
            } else {
                NSString *msg = [results objectForKey:@"msg"];
                PLog(@"doSetSongToTop failed...%@", msg);
                NSString *errorCode = [results objectForKey:@"errorcode"];
                PLog(@"doOrderSong errorcode....%@",errorCode);
                if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForRoomClosed]) {
                    [[NSNotificationCenter defaultCenter] postNotificationName:RECEIVE_ROOM_HAS_CLOSED_NOTIFICATION object:nil];
                }else{
                    [CommenTools showAlertViewWithTitle:errorCode];
                }
            }
            
        }];
        [resquest setFailedBlock:^{
            PLog(@"doSetSongToTop failed...");
            [CommenTools  showNetworkDisconnectedAlertView];
        }];
        [resquest startAsynchronous];
        */
        
    }
}
-(IBAction)deleteSongAction:(id)sender
{
    PDatabaseManager *dataManager = [PDatabaseManager GetInstance];
    [dataManager deleteSongInfo:self.song.songId];
    [[NSNotificationCenter defaultCenter] postNotificationName:RECEIVE_DELETE_PRESONG_NOTIFICATION object:nil];
}

@end
