//
//  PKtvAssistantAPI.m
//  SinaKTVAide
//
//  Created by apple on 13-10-28.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "PKtvAssistantAPI.h"
#import "KTVBaseConfig.h"
#import "PCommonUtil.h"
#import "ASIHTTPRequest.h"
#import "ASIFormDataRequest.h"
#import "RoomInfo.h"
#import "UserSessionManager.h"
#import "TopicInfo.h"
#import "SongInfo.h"
#import "User.h"
#import "TopSong.h"
#import "PointLog.h"
#import "Message.h"
#import "BannerInfo.h"

@implementation PKtvAssistantAPI

/*
 * 1.1登录包厢
 */
+(NSString *)getLoginUrl:(NSString *)password{
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPILogin];
    NSString *param = [NSString stringWithFormat:@"?roompassword=%@", password];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doLogin:(NSString *)password{
    
    NSString *encodeUrl = [PKtvAssistantAPI getLoginUrl:password];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doLogin complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            RoomInfo *roominfo = [RoomInfo initWithDictionary:result];
            [roominfo Log];
            
            [UserSessionManager getInstance].currentRoomInfo = roominfo;
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doLogin failed...%@", msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doLogin failed...");
    }];
    [resquest startAsynchronous];
    
}


/*
 * 1.3登录包厢（new）
 *  @param _userID  用户id
 *  @param _roomId  房间ID（传0）
 *  @param _password    房间密码
 */
+(NSString *)getRoomLoginUrl:(long)_userID roomID:(NSString *)_roomID password:(NSString *)_password{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIRoomLogin];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&roomid=%@&roompassword=%@", _userID, _roomID, _password];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

+(NSString *)getRoomLoginUrlEx:(long)_userID pwd:(NSString*)pwd password:(NSString *)_password
{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIRoomLoginEx];
    NSString *param = [NSString stringWithFormat:@"?idx=%ld&roompassword=%@&pwd=%@", _userID, _password,[[PCommonUtil md5Encode:pwd] lowercaseString]];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doLoginRoomWithUser:(long)_userID roomID:(NSString *)_roomID password:(NSString *)_password{
    NSString *encodeUrl = [PKtvAssistantAPI getRoomLoginUrl:_userID roomID:_roomID password:_password];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doLoginRoomWithUser complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            RoomInfo *roominfo = [RoomInfo initWithDictionary:result];
            [roominfo Log];
            
            [UserSessionManager getInstance].currentRoomInfo = roominfo;
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doLoginRoomWithUser failed...errorcode:%@(%@)", errorCode, msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doLoginRoomWithUser failed...");
    }];
    [resquest startAsynchronous];
}


/**
 * 1.4房间退出（new）
 *  @param _userID  用户id
 *  @param _roomId  房间ID（传0）
 *  @param _password    房间密码
 */
+(NSString *)getRoomLogoutUrl:(long)_userID roomID:(NSString *)_roomID password:(NSString *)_password{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIRoomLogout];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&roomid=%@&roompassword=%@", _userID, _roomID, _password];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doLogoutRoomWithUser:(long)_userID roomID:(NSString *)_roomID password:(NSString *)_password{
    NSString *encodeUrl = [PKtvAssistantAPI getRoomLogoutUrl:_userID roomID:_roomID password:_password];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doLoginRoomWithUser complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        //PLog(@"results-->%@",results);
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            PLog(@"退出包厢成功");
            [UserSessionManager clearCurrentRoomInfo];
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doLoginRoomWithUser failed...errorcode:%@(%@)", errorCode, msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doLoginRoomWithUser failed...");
    }];
    [resquest startAsynchronous];
}


/*
 * 2.1歌曲搜索   assistant/search
 * 参数：int roomid,
 * string Keyword,
 * int Modelid,  （若为0，则查询全部歌曲）
 * int? Topicid,
 * int begin,
 * int num
 */
+(NSString *)getSongSearchUrl:(NSString *)tRoomId keyword:(NSString *)tKeyword modelId:(int)tModelId begin:(long)tBegin num:(int)tNum{
    
    NSString *encodeKeyword = [PCommonUtil encodeUrlParameter:tKeyword];
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongSearch];
    NSString *param = [NSString stringWithFormat:@"?roomid=%@&Keyword=%@&Modelid=%d&Begin=%ld&Num=%d", tRoomId, encodeKeyword, tModelId, tBegin, tNum];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

+(NSString *)getSongSearchUrl:(NSString *)tRoomId keyword:(NSString *)tKeyword modelId:(int)tModelId topicId:(int)tTopicId begin:(long)tBegin num:(int)tNum{
    
    NSString *encodeKeyword = [PCommonUtil encodeUrlParameter:tKeyword];
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongSearch];
    NSString *param = [NSString stringWithFormat:@"?roomid=%@&Keyword=%@&Modelid=%d&Topicid=%d&Begin=%ld&Num=%d", tRoomId, encodeKeyword, tModelId, tTopicId, tBegin, tNum];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doSongSearch:(NSString *)tRoomId keyword:(NSString *)tKeyword modelId:(int)tModelId topicId:(int)tTopicId begin:(long)tBegin num:(int)tNum{
    
    NSString *encodeUrl = [PKtvAssistantAPI getSongSearchUrl:tRoomId keyword:tKeyword modelId:tModelId begin:tBegin num:tNum];
    if (tTopicId > 0) {
        encodeUrl = [PKtvAssistantAPI getSongSearchUrl:tRoomId keyword:tKeyword modelId:tModelId topicId:tTopicId begin:tBegin num:tNum];
    }
    
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doSongSearch complete...");
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
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doSongSearch failed...%@", msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doSongSearch failed...");
    }];
    [resquest startAsynchronous];
    
}

-(void)doSongSearch:(NSString *)tRoomId keyword:(NSString *)tKeyword modelId:(int)tModelId begin:(long)tBegin num:(int)tNum{
    [self doSongSearch:tRoomId keyword:tKeyword modelId:tModelId topicId:0 begin:tBegin num:tNum];
}

/*
 * 2.3根据点歌模块获取歌曲专题列表   assistant/getSongTopics
 * 得到二级目录数据（歌星点歌模块除外）
 */
+(NSString *)getGetSongTopics:(NSString *)tRoomId modelId:(int)tModelId begin:(long)tBegin num:(int)tNum{
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongGetSongTopics];
    NSString *param = [NSString stringWithFormat:@"?roomid=%@&Modelid=%d&keyword=&Begin=%ld&Num=%d", tRoomId, tModelId, tBegin, tNum];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

+(NSString *)getGetSongListByTopics:(int)tModelId topicId:(int) topicId begin:(long)tBegin num:(int)tNum
{
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongGetSongTopics];
    NSString *param = [NSString stringWithFormat:@"?Modelid=%d&topicid=%d&keyword=&Begin=%ld&Num=%d", tModelId,topicId, tBegin,tNum];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

+(NSString *)getGetSongListByTopics3:(int)tModelId topicId:(int) topicId singerId:(int) singerId begin:(long)tBegin num:(int)tNum
{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongGetSongTopics];
    NSString *param = [NSString stringWithFormat:@"?Modelid=%d&topicid=%d&singerid=%d&keyword=&Begin=%ld&Num=%d", tModelId,topicId,singerId,tBegin,tNum];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

+(NSString*)getGetSearchSongListByTopic:(int)tModelId topicId:(int) topicId singerId:(int) singerId key:(NSString*)key begin:(long)tBegin num:(int)tNum
{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongGetSongTopics];
    NSString *param = [NSString stringWithFormat:@"?Modelid=%d&topicid=%d&singerid=%d&keyword=%@&Begin=%ld&Num=%d", tModelId,topicId,singerId,[PCommonUtil encodeUrlParameter:[PCommonUtil encodeBase64:key]],tBegin,tNum];
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doGetSongTopics:(NSString *)tRoomId modelId:(int)tModelId begin:(long)tBegin num:(int)tNum{
    
    NSString *encodeUrl = [PKtvAssistantAPI getGetSongTopics:tRoomId modelId:tModelId begin:tBegin num:tNum];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doGetSongTopics complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"songtopics"];
            int datalistlen = [dataList count];
            for (int i=0; i<datalistlen; i++) {
                TopicInfo *topicinfo = [TopicInfo initWithDictionary:[dataList objectAtIndex:i]];
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doGetSongTopics failed...%@", msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doGetSongTopics failed...");
    }];
    [resquest startAsynchronous];
    
}

/*
 * 2.3根据点歌模块获取歌曲专题列表   assistant/getSongTopics
 * 歌星点歌，通过二级目录数据，得到三级目录数据
 */
+(NSString *)getGetSongTopics:(NSString *)tRoomId modelId:(int)tModelId artistTypeId:(int)tArtistTypeId begin:(long)tBegin num:(int)tNum{
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongGetSongTopics];
    NSString *param = [NSString stringWithFormat:@"?roomid=%@&Modelid=%d&SingerTypeId=%d&Begin=%ld&Num=%d", tRoomId, tModelId, tArtistTypeId, tBegin, tNum];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doGetSongTopics:(NSString *)tRoomId modelId:(int)tModelId artistTypeId:(int)tArtistTypeId begin:(long)tBegin num:(int)tNum{
    
    NSString *encodeUrl = [PKtvAssistantAPI getGetSongTopics:tRoomId modelId:tModelId artistTypeId:tArtistTypeId begin:tBegin num:tNum];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doGetSongTopics complete 3...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"songtopics"];
            int datalistlen = [dataList count];
            for (int i=0; i<datalistlen; i++) {
                TopicInfo *topicinfo = [TopicInfo initWithDictionary:[dataList objectAtIndex:i]];
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doGetSongTopics 3 failed...%@", msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doGetSongTopics 3 failed...");
    }];
    [resquest startAsynchronous];
    
}

/*
 * 2.4根据模块和歌曲专题获取歌曲列表   assistant/getSongListByTopic
 */
+(NSString *)getGetSongListByTopic:(int)tModelId topicId:(int)tTopicId begin:(long)tBegin num:(int)tNum{
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongGetSongListByTopic];
    NSString *param = [NSString stringWithFormat:@"?Modelid=%d&Topicid=%d&Begin=%ld&Num=%d", tModelId, tTopicId, tBegin, tNum];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doGetSongListByTopic:(int)tModelId topicId:(int)tTopicId begin:(long)tBegin num:(int)tNum{
    
    NSString *encodeUrl = [PKtvAssistantAPI getGetSongListByTopic:tModelId topicId:tTopicId begin:tBegin num:tNum];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doGetSongListByTopic complete...");
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
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doGetSongListByTopic failed...%@", msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doGetSongListByTopic failed...");
    }];
    [resquest startAsynchronous];
    
}

/*
 * 2.5根据原唱歌手获取歌曲列表   assistant/getSongListByArtist
 */
+(NSString *)getGetSongListByArtist:(NSString *)tRoomId artistId:(int)tArtistId begin:(long)tBegin num:(int)tNum{
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongGetSongListByArtist];
    NSString *param = [NSString stringWithFormat:@"?roomid=%@&artistid=%d&Begin=%ld&Num=%d", tRoomId, tArtistId, tBegin, tNum];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doGetSongListByArtist:(NSString *)tRoomId artistId:(int)tArtistId begin:(long)tBegin num:(int)tNum{
    
    NSString *encodeUrl = [PKtvAssistantAPI getGetSongListByArtist:tRoomId artistId:tArtistId begin:tBegin num:tNum];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doGetSongListByArtist complete...");
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
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doGetSongListByArtist failed...%@", msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doGetSongListByArtist failed...");
    }];
    [resquest startAsynchronous];
    
}

/*
 * 2.6点播歌曲   assistant/orderSong
 */
+(NSString *)getOrderSongUrl:(long)_userID ktvID:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId songName:(NSString *)tSongName artistName:(NSString *)tArtistName isScore:(int)tIsScore{
    
    NSString *encodeSongName = [PCommonUtil encodeUrlParameter:tSongName];
    NSString *encodeArtistName = [PCommonUtil encodeUrlParameter:tArtistName];
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongOrderSong];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&idx=%ld&roomid=%@&roompassword=%@&Songid=%@&Songname=%@&Artistname=%@&Isscore=%d", tKtvId, _userID,tRoomId, tRoomPassword, tSongId, encodeSongName, encodeArtistName, tIsScore];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doOrderSong:(long)_userID ktvID:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId songName:(NSString *)tSongName artistName:(NSString *)tArtistName isScore:(int)tIsScore{
    
    NSString *encodeUrl = [PKtvAssistantAPI getOrderSongUrl:_userID ktvID:tKtvId roomId:tRoomId roomPassword:tRoomPassword songId:tSongId songName:tSongName artistName:tArtistName isScore:tIsScore];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doOrderSong complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            //
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doOrderSong failed...%@", msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doOrderSong failed...");
    }];
    [resquest startAsynchronous];
    
}

/*
 * 2.7获取已点歌曲列表   assistant/getOrderedSongList
 */
-(NSString *)getGetOrderedSongListUrl:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword{
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongGetOrderedSongList];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&roomid=%@&roompassword=%@", tKtvId, tRoomId, tRoomPassword];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doGetOrderedSongList:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword{
    
    NSString *encodeUrl = [self getGetOrderedSongListUrl:tKtvId roomId:tRoomId roomPassword:tRoomPassword];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doGetOrderedSongList complete...");
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
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doGetOrderedSongList failed...%@", msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doGetOrderedSongList failed...");
    }];
    [resquest startAsynchronous];
    
}

/*
 * 2.8置顶歌曲   assistant/setSongToTop
 */
+(NSString *)getSetSongToTopUrl:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId{
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongSetSongToTop];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&roomid=%@&roompassword=%@&Songid=%@", tKtvId, tRoomId, tRoomPassword, tSongId];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doSetSongToTop:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId{
    
    NSString *encodeUrl = [PKtvAssistantAPI getSetSongToTopUrl:tKtvId roomId:tRoomId roomPassword:tRoomPassword songId:tSongId];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doSetSongToTop complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            //
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doSetSongToTop failed...%@", msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doSetSongToTop failed...");
    }];
    [resquest startAsynchronous];
    
}

/*
 * 2.9删除歌曲   assistant/deleteSongFromOrdered
 */
+(NSString *)getDeleteSongFromOrdered:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId{
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongDeleteFromOrdered];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&roomid=%@&roompassword=%@&Songid=%@", tKtvId, tRoomId, tRoomPassword, tSongId];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doDeleteSongFromOrdered:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId{
    
    NSString *encodeUrl = [PKtvAssistantAPI getDeleteSongFromOrdered:tKtvId roomId:tRoomId roomPassword:tRoomPassword songId:tSongId];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"deleteSongFromOrdered complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            //
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"deleteSongFromOrdered failed...%@", msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"deleteSongFromOrdered failed...");
    }];
    [resquest startAsynchronous];
    
}

/**
 *  2.10点歌并置顶歌曲   Assistant/AddAndSetSongToTop
 *
 *  服务器结合了点歌和歌曲置顶命令
 *
 *  @param tKtvId      tKtvId description
 *  @param tRoomId     tRoomId description
 *  @param tSongId     tSongId description
 *  @param tSongName   tSongName description
 *  @param tArtistName tArtistName description
 *  @param tIsScore    tIsScore description
 *
 *  @return return value description
 */
+(NSString *)getAddAndSetSongToTopUrl:(long)_userID ktvID:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId songName:(NSString *)tSongName artistName:(NSString *)tArtistName isScore:(int)tIsScore{
    
    NSString *encodeSongName = [PCommonUtil encodeUrlParameter:tSongName];
    NSString *encodeArtistName = [PCommonUtil encodeUrlParameter:tArtistName];
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongAddAndSetSongToTop];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&idx=%ld&roomid=%@&roompassword=%@&Songid=%@&Songname=%@&Artistname=%@&Isscore=%d", tKtvId,_userID, tRoomId, tRoomPassword, tSongId, encodeSongName, encodeArtistName, tIsScore];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doAddAndSetSongToTop:(long)_userID ktvID:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId songName:(NSString *)tSongName artistName:(NSString *)tArtistName isScore:(int)tIsScore{
    
    NSString *encodeUrl = [PKtvAssistantAPI getAddAndSetSongToTopUrl:_userID ktvID:tKtvId roomId:tRoomId roomPassword:tRoomPassword songId:tSongId songName:tSongName artistName:tArtistName isScore:tIsScore];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doAddAndSetSongToTop complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            //
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doAddAndSetSongToTop failed...%@", msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doAddAndSetSongToTop failed...");
    }];
    [resquest startAsynchronous];
    
}

/**
 *  2.11获取当前正在播放的歌曲
 *
 *  @param tKtvId  tKtvId description
 *  @param tRoomId tRoomId description
 *
 *  @return return value description
 */
+(NSString *)getGetCurrentSongUrl:(NSString *)tKtvId roomId:(NSString *)tRoomId{
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongGetCurrentSong];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&roomid=%@", tKtvId, tRoomId];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doGetCurrentSong:(NSString *)tKtvId roomId:(NSString *)tRoomId{
    
    NSString *encodeUrl = [PKtvAssistantAPI getGetCurrentSongUrl:tKtvId roomId:tRoomId];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doGetCurrentSong complete...");
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
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            PLog(@"doGetCurrentSong failed...%@", msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doGetCurrentSong failed...");
    }];
    [resquest startAsynchronous];
    
}


/**
 *  2.12 搜索指定歌手的歌曲
 *
 *  @param _keyWord 关键字
 *  @param _roomID  房间ID
 *  @param _artistid 歌手ID
 *  @param _begin   分页起始数(取第一页时传1，)
 *  @param _num     每页返回的最大条数
 *
 *  @return return value description
 */
+(NSString *)getSongSearchByArtistUrl:(NSString *)_keyWord roomID:(NSString *)_roomID artistName:(NSString *)_artistName begin:(int)_begin num:(int)_num{
    NSString *encodeKeyword = [PCommonUtil encodeUrlParameter:_keyWord];
    NSString *encodeArtistName = [PCommonUtil encodeUrlParameter:_artistName];
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongSearchByArtist];
    NSString *param = [NSString stringWithFormat:@"?roomid=%@&Keyword=%@&artistname=%@&begin=%d&num=%d", _roomID, encodeKeyword, encodeArtistName, _begin, _num];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doGetSongSearchByArtist:(NSString *)_keyWord roomID:(NSString *)_roomID artistName:(NSString *)_artistName begin:(int)_begin num:(int)_num{
    NSString *encodeUrl = [PKtvAssistantAPI getSongSearchByArtistUrl:_keyWord roomID:_roomID artistName:_artistName begin:_begin num:_num];
    NSURL *url = [NSURL URLWithString:encodeUrl];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        PLog(@"doGetSongSearchByArtist complete...");
        
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"Songlist"];
            int datalistlen = [dataList count];
            for (int i=0; i<datalistlen; i++) {
                SongInfo *songinfo = [SongInfo initWithDictionary:[dataList objectAtIndex:i]];
                [songinfo Log];
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            
            PLog(@"doGetSongSearchByArtist failed...%@(%@)", msg, errorCode);
            
        }
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        PLog(@"doGetSongSearchByArtist Netfailed..."); //@"无法获取信息，请检查您的网络连接"
    }];
    [operation start];
}




/**
 *  4.1 第三方平台用户登录
 *
 *  @param openID  第三方平台的用户ID（例如：腾讯的传openID， 新浪微博的传uid）
 *  @param tRoomId 包厢ID
 *  @param tToken token
 *  @param timeStamp 过期时间（expirationdate）
 *  @param tRoomId tpye （登录账户类型）
 *
 *  @return return value description
 */
+(NSString *)getOpenLoginUrl:(NSString *)openID roomId:(NSString *)tRoomId roomPwd:(NSString *)tPwd token:(NSString *)tToken expirationdate:(NSString *)timeStamp accountType:(int) tpye{
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIUserOpenLogin];
    NSString *param = [NSString stringWithFormat:@"?openid=%@&roomid=%@&roompwd=%@&accesstoken=%@&expirationdate=%@&aType=%d",
                       openID, tRoomId, tPwd, tToken, [PCommonUtil encodeUrlParameter:timeStamp], tpye];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doOpenLogin:(NSString *)openID roomId:(NSString *)tRoomId roomPwd:(NSString *)tPwd token:(NSString *)tToken expirationdate:(NSString *)timeStamp accountType:(int) tpye{
    
    NSString *encodeUrl = [PKtvAssistantAPI getOpenLoginUrl:openID roomId:tRoomId roomPwd:tPwd token:tToken expirationdate:timeStamp accountType:tpye];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doOpenLogin complete...");
        NSString *response = [resquest responseString];
        
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            User *user = [User initWithLoginDictionary:result];
            [UserSessionManager getInstance].currentUser = user;
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doOpenLogin failed...errorcode:%@(%@)", errorCode, msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doOpenLogin failed...");
    }];
    [resquest startAsynchronous];
    
}


/**
 *  4.3用户退出登录
 *
 *  @param userID  用户id
 *  @param tRoomId 包厢ID（可为0）
 *
 *  @return return value description
 */
+(NSString *)getUserLogoutUrl:(long)_userID roomId:(NSString *)_roomId {
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIUserLogout];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&roomid=%@", _userID, _roomId];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doUserLogout:(long)_userID roomId:(NSString *)_roomId {
    
    NSString *encodeUrl = [PKtvAssistantAPI getUserLogoutUrl:_userID roomId:_roomId];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doOpenLogin complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            BOOL res = [[result objectForKey:@"success"] boolValue];
            if(res){
                PLog(@"成功退出");
            }else{
                PLog(@"退出失败");
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doOpenLogin failed...errorcode:%@(%@)", errorCode, msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doUserLogout failed...");
    }];
    [resquest startAsynchronous];
    
}


/**
 *  4.2用户头像上传
 *
 *  @param _userID  用户id
 *  @param _roomId 包厢ID（可为0）
 *  @param _photoName 图片名
 *  @param _imageData 图片数据
 *
 *  @return return value description
 */
+(NSString *)getUserUploadPhotoUrl:(long)_userID roomId:(NSString *)_roomId photoName:(NSString *)_photoName{
    
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIUserUploadPhoto];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&roomid=%@&imageName=%@", _userID, _roomId, _photoName];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doUserUploadPhoto:(long)_userID roomId:(NSString *)_roomId photoName:(NSString *)_photoName imageData:(NSData *)_imageData{
    
    NSString *encodeUrl = [PKtvAssistantAPI getUserUploadPhotoUrl:_userID roomId:_roomId photoName:[PCommonUtil encodeUrlParameter:_photoName]];
    
    NSString *recordPath = [PCommonUtil storeDataToFile:_photoName subDir:@"photo" data:_imageData];
    
    __unsafe_unretained ASIFormDataRequest *request = [ASIFormDataRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [request setRequestMethod:@"POST"];
    [request setTimeOutSeconds:20];
    [request setPostFormat:ASIMultipartFormDataPostFormat];
    [request setFile:recordPath forKey:@"imagedata"];
    [request setCompletionBlock:^{
        PLog(@"doUserUploadPhoto complete...");
        NSString *response = [request responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            
//            NSString *newHeadPhoto = [result objectForKey:@"headphoto"];
//            if(newHeadPhoto){
//                [[UserSessionManager getInstance] updateCurrentUserPhoto:newHeadPhoto];
//                PLog(@"头像上传成功-->%@",newHeadPhoto);
//                
//            }else{
//                PLog(@"头像上传失败");
//            }
            
            User *user = [User initWithLoginDictionary:result];
            
            if(user){
                [[UserSessionManager getInstance] updateCurrentUserInfo:user];
                PLog(@"头像上传成功-->%@",[[UserSessionManager getInstance].currentUser description]);
                
            }else{
                PLog(@"头像上传失败");
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doUserUploadPhoto failed...errorcode:%@(%@)", errorCode, msg);
        }
        
    }];
    [request setFailedBlock:^{
        PLog(@"doUserUploadPhoto failed...");
    }];
    [request startAsynchronous];
    
}


/**
 *  4.4用户昵称修改
 *
 *  @param _userID  用户id
 *  @param _roomId 包厢ID（可为0）
 *  @param _nickName 昵称
 *
 *  @return return value description
 */
+(NSString *)getUserModifyNickNameUrl:(long)_userID openID:(NSString *)_openID nickName:(NSString *)_nickName gender:(int)_gender{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIUserModifyNickName];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&openID=%@&nickname=%@&sex=%d",
                       _userID, _openID, [PCommonUtil encodeUrlParameter:_nickName], _gender];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doModifyNickName:(long)_userID openID:(NSString *)_openID nickName:(NSString *)_nickName gender:(int)_gender{
    NSString *encodeUrl = [PKtvAssistantAPI getUserModifyNickNameUrl:_userID openID:_openID nickName:_nickName gender:_gender];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doModifyNickName complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
//            NSDictionary *result = [results objectForKey:@"result"];
//            User *user = [User initWithUserDictionary:result];
//            [[UserSessionManager getInstance] updateCurrentUserInfo:user];
//            [[UserSessionManager getInstance].currentUser log];
            
            NSDictionary *result = [results objectForKey:@"result"];
            BOOL res = [[result objectForKey:@"success"] boolValue];
            if(res){
                PLog(@"修改昵称成功");
            }else{
                PLog(@"修改昵称失败");
            }
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doModifyNickName failed...errorcode:%@(%@)", errorCode, msg);
        }

        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doModifyNickName failed...");
    }];
    [resquest startAsynchronous];

}


/**
 *  4.5已经登录的用户再次登录
 *
 *  @param _userID  用户id
 *
 *  @return return value description
 */
+(NSString *)getUserReloginUrl:(long)_userID{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIUserRelogin];
    NSString *param = [NSString stringWithFormat:@"?id=%ld",_userID];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}



/**
 *  4.6更新本地用户信息
 *
 *  @param _userID  用户id
 *
 *  @return return value description
 */
+(NSString *)getUserGetInfoUrl:(long)_userID{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIUserGetInfo];
    NSString *param = [NSString stringWithFormat:@"?id=%ld",_userID];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}


/**
 *  4.7更新本地用户信息
 *
 *  @param _userID  用户id
 *
 *  @return return value description
 */
+(NSString *)getUserRegisterUrl:(NSString *)kid pwd:(NSString *)pwd sex:(NSInteger)sex
{
    
    PLog(@"%@",[PCommonUtil md5Encode:pwd]);
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIUserRegister];
    NSString *param = [NSString stringWithFormat:@"?id=%@&pwd=%@&sex=%d",kid, [[PCommonUtil md5Encode:pwd] lowercaseString],sex];
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}


/**
 *  4.8更新本地用户信息
 *
 *  @param _userID  用户id
 *
 *  @return return value description
 */
+(NSString *)getUserLoginUrl:(NSString *)kid pwd:(NSString *)pwd
{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIUserLgoin];
    NSString *param = [NSString stringWithFormat:@"?id=%@&pwd=%@",kid, [[PCommonUtil md5Encode:pwd] lowercaseString]];
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}



/**
 *  5.1获取用户收藏列表
 *
 *  @param _userID  用户id
 *
 *  @return return value description
 */
+(NSString *)getUserCollectedListUrl:(long)_userID begin:(int)_begin num:(int)_num;{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongGetCollectedList];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&begin=%d&num=%d", _userID, _begin, _num];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doGetUserCollectedList:(long)_userID begin:(int)_begin num:(int)_num {
    NSString *encodeUrl = [PKtvAssistantAPI getUserCollectedListUrl:_userID begin:_begin num:_num];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doGetUserCollectedList complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"songlists"];
            int datalistlen = [dataList count];
            for (int i=0; i<datalistlen; i++) {
                SongInfo *songinfo = [SongInfo initWithDictionary:[dataList objectAtIndex:i]];
                [songinfo Log];
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doGetUserCollectedList failed...errorcode:%@(%@)", errorCode, msg);
        }
    }];
    
    [resquest setFailedBlock:^{
        PLog(@"doGetUserCollectedList failed...");
    }];
    
    [resquest startAsynchronous];
}


/**
 *  5.2添加收藏
 *
 *  @param _userID  用户id
 *  @param _songId 歌曲ID
 *
 *  @return return value description
 */
+(NSString *)getAddCollectionSongUrl:(long)_userID songId:(long)_songId{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongAddCollection];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&songid=%ld", _userID, _songId];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doAddCollectionSong:(long)_userID songId:(long)_songId{
    NSString *encodeUrl = [PKtvAssistantAPI getAddCollectionSongUrl:_userID songId:_songId];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doAddCollectionSong complete...");
        NSString *response = [resquest responseString];
        PLog(@"response->%@",response);
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            BOOL res = [[result objectForKey:@"success"] boolValue];
            if(res){
                PLog(@"添加收藏成功");
            }else{
                PLog(@"添加收藏失败");
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doAddCollectionSong failed...errorcode:%@(%@)", errorCode, msg);
        }
    }];
    
    [resquest setFailedBlock:^{
        PLog(@"doAddCollectionSong failed...");
    }];
    
    [resquest startAsynchronous];
}


/**
 *  5.3取消收藏
 *
 *  @param _userID  用户id
 *  @param _songId 歌曲ID
 *
 *  @return return value description
 */
+(NSString *)getDeleteCollectionSongUrl:(long)_userID songId:(long)_songId{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongDeleteCollection];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&songid=%ld", _userID, _songId];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doDeleteCollectionSong:(long)_userID songId:(long)_songId{
    NSString *encodeUrl = [PKtvAssistantAPI getDeleteCollectionSongUrl:_userID songId:_songId];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doDeleteCollectionSong complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            BOOL res = [[result objectForKey:@"success"] boolValue];
            if(res){
                PLog(@"删除收藏成功");
            }else{
                PLog(@"删除收藏失败");
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doAddCollectionSong failed...errorcode:%@(%@)", errorCode, msg);
        }
    }];
    
    [resquest setFailedBlock:^{
        PLog(@"doDeleteCollectionSong failed...");
    }];
    
    [resquest startAsynchronous];
}



/**
 *  6.1获取高分榜歌曲列表
 *
 *  @param _userID  用户id
 *  @param _roomId  房间ID
 *
 *  @return return value description
 */
+(NSString *)getTopSongListUrl:(long)_userID roomId:(NSString *)_roomId modelID:(int)_modelID topicID:(int)_topicID begin:(int)_begin num:(int)_num {
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongGetTopSongList];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&roomid=%@&modelid=%d&topicid=%d&begin=%d&num=%d",
                       _userID, _roomId, _modelID, _topicID, _begin ,_num];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doGetTopSongList:(long)_userID roomId:(NSString *)_roomId modelID:(int)_modelID topicID:(int)_topicID begin:(int)_begin num:(int)_num {
    NSString *encodeUrl = [PKtvAssistantAPI getTopSongListUrl:_userID roomId:_roomId modelID:_modelID topicID:_topicID  begin:_begin num:_num ];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doGetTopSongList complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"worklists"];
            int datalistlen = [dataList count];
            for (int i=0; i< datalistlen; i++) {
                TopSong *topSong = [TopSong initWithDictionary:[dataList objectAtIndex:i]];
                [topSong log];
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doGetTopSongList failed...errorcode:%@(%@)", errorCode, msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doGetTopSongList failed...");
    }];
    [resquest startAsynchronous];
}


/**
 *  6.4获取banner信息列表
 *
 *  @param _userID  用户id
 *  @param _roomId  房间ID
 *
 *  @return return value description
 */
+(NSString *)getBannerListUrl:(long)_userID roomId:(NSString *)_roomId{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIGetBannerList];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&roomid=%@", _userID, _roomId];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doGetBannerList:(long)_userID roomId:(NSString *)_roomId{
    NSString *encodeUrl = [PKtvAssistantAPI getBannerListUrl:_userID roomId:_roomId];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doGetBannerList complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"bannerlists"];
            int datalistlen = [dataList count];
            for (int i=0; i<datalistlen; i++) {
                BannerInfo *banner = [BannerInfo initWithDictionary:[dataList objectAtIndex:i]];
                [banner log];
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doGetBannerList failed...errorcode:%@(%@)", errorCode, msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doGetBannerList failed...");
    }];
    [resquest startAsynchronous];
}


/**
 *  6.8高分榜上榜歌曲抢榜
 *
 *  @param _userID  用户id
 *  @param _roomId  房间ID
 *  @param _tsID    演唱记录ID
 *
 *  @return return value description
 */
+(NSString *)getReceiveRankSongUrl:(long)_userID roomId:(NSString *)_roomId workID:(long)_workID score:(int)_score date:(NSString *)_date{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISongReceiveRankSong];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&roomid=%@&workid=%ld&score=%d&date=%@",
                       _userID, _roomId, _workID, _score, [PCommonUtil encodeUrlParameter:_date]];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doReceiveRankSong:(long)_userID roomId:(NSString *)_roomId workID:(long)_workID score:(int)_score date:(NSString *)_date{
    NSString *encodeUrl = [PKtvAssistantAPI getReceiveRankSongUrl:_userID roomId:_roomId workID:_workID score:_score date:_date];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doReceiveRankSong complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame)
        {
            NSDictionary *result = [results objectForKey:@"result"];
            BOOL res = [[result objectForKey:@"success"] boolValue];
            if(res)
            {
                PLog(@"抢榜成功");
            }
            else
            {
                PLog(@"抢榜失败");
            }
            
        }
        else
        {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doReceiveRankSong failed...errorcode:%@(%@)", errorCode, msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doReceiveRankSong failed...");
    }];
    [resquest startAsynchronous];
}


/**
 *  7.1是否有未领积分(2.0版本新接口)
 *
 *  @param _userID  用户id
 *
 *  @return return value description
 */

+(NSString *)getCheckRewardUrl:(long)_userID{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIRewardCheck];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld", _userID];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

/**
 *  7.2获取用户的积分信息(2.0版本新接口)
 *
 *  @param _userID  用户id
 *  @param _bid  条数（起始传1）
 *  @param _num  每页条数
 *
 *  @return return value description
 */
+(NSString *)getUserRewardUrl:(long)_userID begin:(int)_bid num:(int)_num{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIRewardGet];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&begin=%d&num=%d",
                       _userID, _bid, _num];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}


/**
 *  7.3用户聊天送礼金币变化
 *
 *  @param  _userID     用户id
 *  @param  _page        积分记录ID
 *  @param  _size        积分记录ID
 *  @tparam _type         记录类型：1 收礼 2送礼
 *
 *  @return return value description
 */
+(NSString *)getChatGiftLogUrl:(long)_userID page:(int)_page size:(int)_size type:(int)_type{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIChatGiftLog];
    NSString *param = [NSString stringWithFormat:@"?idx=%ld&page=%d&size=%d&type=%d",_userID,
                       _page, _size, _type];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}


/**
 *  7.4用户金币奖励
 *
 *  @param _userID  用户id
 *  @param _sourceID 积分记录ID
 *  @param _ktvid 积分记录ID
 *
 *  @return return value description
 */
+(NSString *)getReceiveRewardUrl:(long)_userID sourceID:(long)_sID ktvid:(NSString *)_ktvid{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIRewardReceive];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&sourceid=%ld&ktvid=%@",
                       _userID, _sID, _ktvid];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}


/**
 *  8.1获取消息列表
 *
 *  @param _userID  用户id
 *  @param _roomId  房间ID
 *  @param _sid
 *  @param _bid
 *  @param _num
 *
 *  @return return value description
 */
+(NSString *)getGetMessageListUrl:(long)_userID roomId:(NSString *)_roomId since:(long)_sid before:(long)_bid num:(int)_num{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIMsgGetMessageList];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&roomid=%@&since=%ld&before=%ld&num=%d", _userID, _roomId, _sid, _bid, _num];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doGetMessageList:(long)_userID roomId:(NSString *)_roomId since:(long)_sid before:(long)_bid num:(int)_num{
    NSString *encodeUrl = [PKtvAssistantAPI getGetMessageListUrl:_userID roomId:_roomId since:_sid before:_bid num:_num];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doGetMessageList complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            
            NSArray *dataList = [result objectForKey:@"msglist"];
            int datalistlen = [dataList count];
            for (int i=0; i < datalistlen; i++) {
                Message *msg = [Message initWithDictionary:[dataList objectAtIndex:i]];
                [msg log];
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doGetMessageList failed...errorcode:%@(%@)", errorCode, msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doGetMessageList failed...");
    }];
    [resquest startAsynchronous];
}


/**
 *  8.2获取消息详细信息
 *
 *  @param _userID  用户id
 *  @param _roomId  房间ID
 *  @param _msgID   消息ID
 *  @param _workID  上榜歌演唱记录ID（只有当消息类型为“歌曲上榜”时，该值才会大于0）
 *
 *  @return return value description
 */
+(NSString *)getGetMessageDetailUrl:(long)_userID roomId:(NSString *)_roomId msgID:(long)_msgID msgType:(int)_msgType workID:(long)_workID point:(int)_point date:(NSString *)_date{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIMsgGetMessageDetail];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&roomid=%@&msgid=%ld&msgtype=%d&workid=%ld&point=%d&date=%@",
                       _userID, _roomId, _msgID, _msgType, _workID, _point, [PCommonUtil encodeUrlParameter:_date]];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doGetMessageDetail:(long)_userID roomId:(NSString *)_roomId msgID:(long)_msgID msgType:(int)_msgType workID:(long)_workID point:(int)_point date:(NSString *)_date{
    NSString *encodeUrl = [PKtvAssistantAPI getGetMessageDetailUrl:_userID roomId:_roomId msgID:_msgID msgType:_msgType workID:_workID point:_point date:_date];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doGetMessageDetail complete...");
        NSString *response = [resquest responseString];
        NSDictionary *results = [PCommonUtil NSDictionaryFromJSONString:response];
        
        NSString *status = [results objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [results objectForKey:@"result"];
            
            NSDictionary *msgData = [result objectForKey:@"msgdetail"];
            Message *msg = [Message initWithDictionary:msgData];
            [msg log];
            
            if([msgData objectForKey:@"work"]){
                NSDictionary *workData = [msgData objectForKey:@"work"];
                
                TopSong *topSong = [TopSong initWithDictionary:workData];
                [topSong log];
            }
            
            
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doGetMessageDetail failed...errorcode:%@(%@)", errorCode, msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doGetMessageDetail failed...");
    }];
    [resquest startAsynchronous];
}


/**
 *  9.1切歌
 *
 *  @param _userID  用户id
 *  @param _roomId  房间ID
 *  @param _roomPwd 房间密码
 *
 *  @return return value description
 */
+(NSString *)getCutSongUrl:(long)_userID ktvID:(NSString *)_ktvId roomId:(NSString *)_roomId roomPassword:(NSString *)_roomPwd songID:(long)_songID{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPICutSong];
    NSString *param = [NSString stringWithFormat:@"?userid=%ld&ktvid=%@&roomid=%@&roompassword=%@&musicid=%ld", _userID, _ktvId, _roomId, _roomPwd, _songID];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

-(void)doCutSong:(long)_userID ktvID:(NSString *)_ktvId roomId:(NSString *)_roomId roomPassword:(NSString *)_roomPwd songID:(long)_songID{
    NSString *encodeUrl = [PKtvAssistantAPI getCutSongUrl:_userID ktvID:_ktvId roomId:_roomId roomPassword:_roomPwd songID:_songID];
    __unsafe_unretained ASIHTTPRequest *resquest = [ASIHTTPRequest requestWithURL:[NSURL URLWithString:encodeUrl]];
    [resquest setCompletionBlock:^{
        PLog(@"doCutSong complete...");
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
            }
            
        } else {
            NSString *msg = [results objectForKey:@"msg"];
            NSString *errorCode = [results objectForKey:@"errorcode"];
            PLog(@"doCutSong failed...errorcode:%@(%@)", errorCode, msg);
        }
        
    }];
    [resquest setFailedBlock:^{
        PLog(@"doCutSong failed...");
    }];
    [resquest startAsynchronous];
}

/**
 *  9.2设置调音版
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *  @param _voice   音量 （-1：减少音量，0：音量不变，1增加音量）
 *  @param _mic     麦克风 （-1：减少麦克风音量，0：麦克风音量不变，1增加麦克风音量）
 *  @param _tone    音调 （-1：降低音调，0：音调不变，1增加音调）
 *  @param _effect  音效 （-1: 不变，0：魔音快歌，1：魔音顶尖，2：魔音唱将，3：魔音K歌，4：专业快歌，5：专业顶尖，6专业唱将，7：专业K歌）
 *
 *  @return return value description
 */
+(NSString *)getSetVoicePadUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd voice:(int)_voice mic:(int)_mic tone:(int)_tone effect:(int)_effect{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPISetVoicePad];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&roomid=%@&roompassword=%@&voice=%d&mic=%d&tone=%d&effect=%d", _ktvId, _roomID, _roomPwd, _voice, _mic,_tone,_effect];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
    
}

/**
 *  9.3设置暂停
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *  @param _isPause 是否暂停 （0:服务器自动改变模式）
 *
 *  @return return value description
 */
+(NSString *)getSetPauseUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd isPause:(int)_isPause{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKTVAssistantAPISetPause];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&roomid=%@&roompassword=%@&isPause=%d", _ktvId, _roomID, _roomPwd, _isPause];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

/**
 *  9.4设置静音
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *  @param _isMute 是否暂停 （0:服务器自动改变模式）
 *
 *  @return return value description
 */
+(NSString *)getSetMuteUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd isMute:(int)_isMute{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKTVAssistantAPISetMute];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&roomid=%@&roompassword=%@&ismute=%d", _ktvId, _roomID, _roomPwd, _isMute];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

/**
 *  9.5设置伴唱
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *  @param _singMode 是否暂停 （0:服务器自动改变模式）
 *
 *  @return return value description
 */
+(NSString *)getSetSingModeUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd singMode:(int)_singMode{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKTVAssistantAPISetSingMode];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&roomid=%@&roompassword=%@&singMode=%d", _ktvId, _roomID, _roomPwd, _singMode];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

/**
 *  9.6设置评分模式
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *  @param _scoreMode 评分模式 （0:服务器自动改变模式）
 *
 *  @return return value description
 */
+(NSString *)getSetScoreModeUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd singMode:(int)_singMode{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKTVAssistantAPISetScoreStatus];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&roomid=%@&roompassword=%@&scoreMode=%d", _ktvId, _roomID, _roomPwd, _singMode];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}


/**
 *  9.7设置气氛
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *  @param _atmosphere 气氛（1：喝彩，2鼓掌)
 *
 *  @return return value description
 */
+(NSString *)getSetAtmosphereUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd _atmosphere:(int)_atmosphere{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKTVAssistantAPISetAtmosphere];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&roomid=%@&roompassword=%@&atmosphere=%d", _ktvId, _roomID, _roomPwd, _atmosphere];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}



/**
 *  9.8设置重唱
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *
 *  @return return value description
 */
+(NSString *)getSetReplayUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKTVAssistantAPISetReplay];
    NSString *param = [NSString stringWithFormat:@"?ktvid=%@&roomid=%@&roompassword=%@", _ktvId, _roomID, _roomPwd];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}


/**
 *  10.1获取ktv信息
 *
 *
 *  @return return value description
 */
+(NSString *)getGetKtvInfoUrl:(int)_page size:(int)_size{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIGetKtvInfo];
    NSString *param = [NSString stringWithFormat:@"?page=%d&size=%d", _page, _size];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}


/**
 *  10.2获取ktv下的礼物信息
 *
 *
 *  @return return value description
 */
+(NSString *)getGetGiftInfoUrl:(NSString *)_ktvid page:(int)_page size:(int)_size{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIGetGiftInfo];
    NSString *param = [NSString stringWithFormat:@"?ktvID=%@&page=%d&size=%d",_ktvid, _page, _size];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}


/**
 *  10.3获取礼物兑换信息
 *
 
 *
 *  @return return value description
 */
+(NSString *)getGetGiftLogUrl:(long) _idx type:(int)_type page:(int)_page size:(int)_size{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIGetGiftLog];
    NSString *param = [NSString stringWithFormat:@"?idx=%ld&type=%d&page=%d&size=%d",_idx,_type, _page, _size];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}


/**
 *  10.4礼物兑换
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *
 *  @return return value description
 */
+(NSString *)getGetGiftExchangeUrl:(long) _idx giftID:(NSString *)_giftID ktvID:(NSString *)_ktvID num:(int)_num{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIGiftExchange];
    NSString *param = [NSString stringWithFormat:@"?idx=%ld&giftID=%@&ktvID=%@&giftnum=%d", _idx, _giftID, _ktvID, _num];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

/*
  获取用户最近20条唱过的评分歌曲的记录
 */
+(NSString*)getUserSongRecord:(long)idx
{
    NSString* baseUrl = [NSString stringWithFormat:@"%@%@",kPKtvAssistantAPIDomain,kPKtvAssistantAPISongRecord];
    NSString* param = [NSString stringWithFormat:@"?id=%ld&TopNum=10",idx];
    
    NSString* encodeUrl = [PCommonUtil generateAPIStringWithSecret:baseUrl param:param];
    return encodeUrl;
}

/**
 *  appstore充值接口
 *
 *
 *  @return return value description
 */
+(NSString *)getIAPPayUrl:(long)_idx ktvID:(NSString *)_ktvID roomID:(NSString *)_roomID identifier:(NSString *)_identifier{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIAppStorePay];
    NSString *param = [NSString stringWithFormat:@"?idx=%ld&kid=%@&rid=%@&name=%@",_idx,_ktvID,_roomID,_identifier];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}
/**
 *  是否显示
 */
+(NSString *)getShow
{
    NSString *tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain, kPKtvAssistantAPIShowThird];
    NSString *param = [NSString stringWithFormat:@"?show=0"];
    
    NSString *encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

/*
 请求获取 可领取的红包以及未完成的红包任务
 */
+(NSString*)getHongBaoListUrl:(long)idx type:(int)type
{
    NSString* tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain,kPKtvAssistantAPIHongBaoList];
    NSString* param = [NSString stringWithFormat:@"?idx=%ld&type=%d",idx,type];
    
    NSString* encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

//请求领取某个红包
+(NSString*)getHongBaoGetUrl:(long)idx hbid:(NSString*)hbid
{
    NSString* tempUrl = [NSString stringWithFormat:@"%@%@", kPKtvAssistantAPIDomain,kPKtvAssistantAPIHongBaoGet];
    NSString* param = [NSString stringWithFormat:@"?Idx=%ld&ID=%@",idx,hbid];
    
    NSString* encodeUrl = [PCommonUtil generateAPIStringWithSecret:tempUrl param:param];
    return encodeUrl;
}

+(NSString*)getWeiXinTokenUrl:(NSString*)appId secret:(NSString*)secret code:(NSString*)code
{
    return [NSString stringWithFormat:@"https://api.weixin.qq.com/sns/oauth2/access_token?appid=%@&secret=%@&code=%@&grant_type=authorization_code",appId,secret,code];
}
+(NSString*)getWeiXinRefreshTokenUrl:(NSString*)appId refresh:(NSString*)refresh
{
    return [NSString stringWithFormat:@"https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=%@&grant_type=refresh_token&refresh_token=%@",appId,refresh];
}
@end
