//
//  KTVGetAlreadyDemandSongAPI.m
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-11-4.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "KTVGetAlreadyDemandSongAPI.h"
#import "SongInfo.h"

@implementation KTVGetAlreadyDemandSongAPI
@synthesize delegate = _delegate,songdataArray = _songdataArray;


-(NSString *)getGetOrderedSongListUrl:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword{
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongGetOrderedSongList];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&roomid=%@&roompassword=%@", tKtvId, tRoomId, tRoomPassword];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doGetOrderedSongList:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword{
    NSString *encodeUrl = [self getGetOrderedSongListUrl:tKtvId roomId:tRoomId roomPassword:tRoomPassword];
    NSURL *url = [NSURL URLWithString:encodeUrl];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        PLog(@"doGetOrderedSongList complete...");
        if (!_songdataArray) {
            _songdataArray = [[NSMutableArray alloc] init];
        }
        [_songdataArray removeAllObjects];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"Songlist"];
            int datalistlen = [dataList count];
            for (int i=0; i<datalistlen; i++) {
                SongInfo *songinfo = [SongInfo initWithDictionary:[dataList objectAtIndex:i]];
                [songinfo Log];
                [_songdataArray addObject:songinfo];
            }
            
            if (_delegate && [_delegate respondsToSelector:@selector(getAlreadyDemandSongWithSuccessResult:)]) {
                [_delegate getAlreadyDemandSongWithSuccessResult:_songdataArray];
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doGetOrderedSongList failed...%@", msg);
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doGetOrderedSongList failed  errorcode...%@", errorCode);
            if (_delegate && [_delegate respondsToSelector:@selector(getAlreadyDemandSongWithFaildResult:)]) {
                if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForRoomClosed] || [errorCode isEqualToString:kPKtvAssistantErrorCodeForNoRoom]) {
                    [_delegate getAlreadyDemandSongWithFaildResult:errorCode];
                }else{
                    [_delegate getAlreadyDemandSongWithFaildResult:msg];
                }
                
            }
        }
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        PLog(@"doGetOrderedSongList failed...");
        if (_delegate && [_delegate respondsToSelector:@selector(getAlreadyDemandSongWithFaildResult:)]) {
            [_delegate getAlreadyDemandSongWithFaildResult:@"无法获取信息，请检查您的网络连接"];
        }
    }];
    [operation start];
    
    
    /*
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    if (_songdataArray) {
        [_songdataArray  removeAllObjects];
    }
    [resquest setCompletionBlock:^{
        NSLog(@"doGetOrderedSongList complete...");
        if (!_songdataArray) {
            _songdataArray = [[NSMutableArray alloc] init];
        }
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"Songlist"];
            int datalistlen = [dataList count];
            for (int i=0; i<datalistlen; i++) {
                SongInfo *songinfo = [SongInfo initWithDictionary:[dataList objectAtIndex:i]];
                [songinfo Log];
                [_songdataArray addObject:songinfo];
            }
            
            if (_delegate && [_delegate respondsToSelector:@selector(getAlreadyDemandSongWithSuccessResult:)]) {
                [_delegate getAlreadyDemandSongWithSuccessResult:_songdataArray];
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doGetOrderedSongList failed...%@", msg);
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doGetOrderedSongList failed  errorcode...%@", errorCode);
            if (_delegate && [_delegate respondsToSelector:@selector(getAlreadyDemandSongWithFaildResult:)]) {
                if ([errorCode isEqualToString:kPKtvAssistantErrorCodeForRoomClosed]) {
                    [_delegate getAlreadyDemandSongWithFaildResult:errorCode];
                }else{
                    [_delegate getAlreadyDemandSongWithFaildResult:msg];
                }
                
            }
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doGetOrderedSongList failed...");
        if (_delegate && [_delegate respondsToSelector:@selector(getAlreadyDemandSongWithFaildResult:)]) {
            [_delegate getAlreadyDemandSongWithFaildResult:@"无法获取信息，请检查您的网络连接"];
        }
    }];
    [resquest startAsynchronous];
    */
}
@end
