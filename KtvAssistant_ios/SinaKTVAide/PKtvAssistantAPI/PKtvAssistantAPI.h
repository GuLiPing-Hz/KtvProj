//
//  PKtvAssistantAPI.h
//  SinaKTVAide
//
//  Created by apple on 13-10-28.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PKtvAssistantAPI : NSObject

/**
 * 1.1登录包厢（放弃）
 */
+(NSString *)getLoginUrl:(NSString *)password;
-(void)doLogin:(NSString *)password;

/**
 * 1.2退出包厢
 */
//-(void)doLogout:(NSString *)tRoomId;

/**
 * 1.3登录包厢（new）
 *  @param _userID  用户id
 *  @param _roomId  房间ID（传0）
 *  @param _password    房间密码
 */
+(NSString *)getRoomLoginUrl:(long)_userID roomID:(NSString *)_roomID password:(NSString *)_password;
+(NSString *)getRoomLoginUrlEx:(long)_userID pwd:(NSString*)pwd password:(NSString *)_password;
-(void)doLoginRoomWithUser:(long)_userID roomID:(NSString *)_roomID password:(NSString *)_password;

/**
 * 1.4房间退出（new）
 *  @param _userID  用户id
 *  @param _roomId  房间ID（传0）
 *  @param _password    房间密码
 */
+(NSString *)getRoomLogoutUrl:(long)_userID roomID:(NSString *)_roomID password:(NSString *)_password;
-(void)doLogoutRoomWithUser:(long)_userID roomID:(NSString *)_roomID password:(NSString *)_password;

/**
 * 2.1歌曲搜索   assistant/search
 * 参数：int roomid,
 * string Keyword,
 * int Modelid,  （若为0，则查询全部歌曲）
 * int Topicid,
 * int begin,
 * int num
 */
+(NSString *)getSongSearchUrl:(NSString *)tRoomId keyword:(NSString *)tKeyword modelId:(int)tModelId begin:(long)tBegin num:(int)tNum;
+(NSString *)getSongSearchUrl:(NSString *)tRoomId keyword:(NSString *)tKeyword modelId:(int)tModelId topicId:(int)tTopicId begin:(long)tBegin num:(int)tNum;
-(void)doSongSearch:(NSString *)tRoomId keyword:(NSString *)tKeyword modelId:(int)tModelId topicId:(int)tTopicId begin:(long)tBegin num:(int)tNum;
-(void)doSongSearch:(NSString *)tRoomId keyword:(NSString *)tKeyword modelId:(int)tModelId begin:(long)tBegin num:(int)tNum;

/**
 * 2.3根据点歌模块获取歌曲专题列表   assistant/getSongTopics
 * 得到二级目录数据
 * modelId = 3 时返回的是歌手类型列表；
 * modelId = 6 时返回的是曲种类型的列表
 *
 * 参数int roomid,
 * int Modelid,
 * int Begin,
 * int Num,
 */
+(NSString *)getGetSongTopics:(NSString *)tRoomId modelId:(int)tModelId begin:(long)tBegin num:(int)tNum;
//获取二级级列表
+(NSString *)getGetSongListByTopics:(int)tModelId topicId:(int) topicId begin:(long)tBegin num:(int)tNum;
//获取三级列表
+(NSString *)getGetSongListByTopics3:(int)tModelId topicId:(int) topicId singerId:(int) singerId begin:(long)tBegin num:(int)tNum;
//获取歌曲列表的搜索结果
+(NSString*)getGetSearchSongListByTopic:(int)tModelId topicId:(int) topicId singerId:(int) singerId key:(NSString*)key begin:(long)tBegin num:(int)tNum;
-(void)doGetSongTopics:(NSString *)tRoomId modelId:(int)tModelId begin:(long)tBegin num:(int)tNum;

/**
 * 2.3根据点歌模块获、歌手类型获取歌手列表   assistant/getSongTopics
 * 该接口现在只用于歌手模块
 * @param artistTypeId  歌手类型ID，
 * @param modelId  歌手模块ID，即为3
 * 
 * @return 返回的是歌手列表
 */
+(NSString *)getGetSongTopics:(NSString *)tRoomId modelId:(int)tModelId artistTypeId:(int)tArtistTypeId begin:(long)tBegin num:(int)tNum;
-(void)doGetSongTopics:(NSString *)tRoomId modelId:(int)tModelId artistTypeId:(int)tArtistTypeId begin:(long)tBegin num:(int)tNum;

/**
 * 2.4根据模块和歌曲专题获取歌曲列表   assistant/getSongListByTopic
 *
 * Assistant/GetSongListByTopic
 * @param int roomid,
 * @param int Modelid,
 * @param int Topicid,
 * @param int Begin,
 * @param int Num
 *
 * @return 返回的是歌曲列表
 */
+(NSString *)getGetSongListByTopic:(int)tModelId topicId:(int)tTopicId begin:(long)tBegin num:(int)tNum;
-(void)doGetSongListByTopic:(NSString *)tRoomId modelId:(int)tModelId topicId:(int)tTopicId begin:(long)tBegin num:(int)tNum;

/**
 * 2.5根据原唱歌手获取歌曲列表   assistant/getSongListByArtist
 *
 * 原先用来获取歌星，对应的歌曲列表,现在和专题共用处理，被弃用.
 *
 * Assistant/GetSongListByArtist
 * 参数
 * int roomid,
 * int artistid,
 * int? Begin,
 * int? Num
 */
+(NSString *)getGetSongListByArtist:(NSString *)tRoomId artistId:(int)tArtistId begin:(long)tBegin num:(int)tNum;
-(void)doGetSongListByArtist:(NSString *)tRoomId artistId:(int)tArtistId begin:(long)tBegin num:(int)tNum;

/**
 * 2.6点播歌曲   assistant/orderSong
 *
 * 点播歌曲:Assistant/OrderSong
 * 参数int ktvid, int roomid, int Songid, string Songname, string Artistname, char Isscore
 */
+(NSString *)getOrderSongUrl:(long)_userID ktvID:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId songName:(NSString *)tSongName artistName:(NSString *)tArtistName isScore:(int)tIsScore;
-(void)doOrderSong:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId songName:(NSString *)tSongName artistName:(NSString *)tArtistName isScore:(int)tIsScore;

/**
 * 2.7获取已点歌曲列表   assistant/getOrderedSongList
 *
 */
-(NSString *)getGetOrderedSongListUrl:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword;
-(void)doGetOrderedSongList:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword;

/**
 * 2.8置顶歌曲   assistant/setSongToTop
 * 置顶歌曲：Assistant/SetSongToTop
 * 参数int ktvid, int roomid, int Songid
 */
+(NSString *)getSetSongToTopUrl:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId;
-(void)doSetSongToTop:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId;

/**
 * 2.9删除歌曲   assistant/deleteSongFromOrdered
 *
 * 删除歌曲：Assistant/DeleteSongFromOrdered
 * 参数int ktvid, int roomid, int Songid
 */
+(NSString *)getDeleteSongFromOrdered:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId;
-(void)doDeleteSongFromOrdered:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId;

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
+(NSString *)getAddAndSetSongToTopUrl:(long)_userID ktvID:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId songName:(NSString *)tSongName artistName:(NSString *)tArtistName isScore:(int)tIsScore;
-(void)doAddAndSetSongToTop:(NSString *)tKtvId roomId:(NSString *)tRoomId roomPassword:(NSString *)tRoomPassword songId:(NSString *)tSongId songName:(NSString *)tSongName artistName:(NSString *)tArtistName isScore:(int)tIsScore;

/**
 *  2.11获取当前正在播放的歌曲
 *
 *  @param tKtvId  tKtvId description
 *  @param tRoomId tRoomId description
 *
 *  @return return value description
 */
+(NSString *)getGetCurrentSongUrl:(NSString *)tKtvId roomId:(NSString *)tRoomId;
-(void)doGetCurrentSong:(NSString *)tKtvId roomId:(NSString *)tRoomId;


/**
 *  2.12 搜索指定歌手的歌曲
 *
 *  @param _keyWord 关键字
 *  @param _roomID  房间ID
 *  @param _artistName 歌手名
 *  @param _begin   分页起始数(取第一页时传1，)
 *  @param _num     每页返回的最大条数
 *
 *  @return return value description
 */
-(void)doGetSongSearchByArtist:(NSString *)_keyWord roomID:(NSString *)_roomID artistName:(NSString *)_artistName begin:(int)_begin num:(int)_num;
+(NSString *)getSongSearchByArtistUrl:(NSString *)_keyWord roomID:(NSString *)_roomID artistName:(NSString *)_artistName begin:(int)_begin num:(int)_num;


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
-(void)doOpenLogin:(NSString *)openID roomId:(NSString *)tRoomId roomPwd:(NSString *)tPwd token:(NSString *)tToken expirationdate:(NSString *)timeStamp accountType:(int) tpye;
+(NSString *)getOpenLoginUrl:(NSString *)openID roomId:(NSString *)tRoomId roomPwd:(NSString *)tPwd token:(NSString *)tToken expirationdate:(NSString *)timeStamp accountType:(int) tpye;


/**
 *  4.3用户退出登录
 *
 *  @param userID  用户id
 *  @param tRoomId 包厢ID（可为0）
 *
 *  @return return value description
 */
-(void)doUserLogout:(long)_userID roomId:(NSString *)_roomId;
+(NSString *)getUserLogoutUrl:(long)_userID roomId:(NSString *)_roomId;


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
-(void)doUserUploadPhoto:(long)_userID roomId:(NSString *)_roomId photoName:(NSString *)_photoName imageData:(NSData *)_imageData;
+(NSString *)getUserUploadPhotoUrl:(long)_userID roomId:(NSString *)_roomId photoName:(NSString *)_photoName;


/**
 *  4.4用户昵称修改
 *
 *  @param _userID  用户id
 *  @param _roomId 包厢ID（可为0）
 *  @param _nickName 昵称
 *
 *  @return return value description
 */
-(void)doModifyNickName:(long)_userID openID:(NSString *)_openID nickName:(NSString *)_nickName gender:(int)_gender;
+(NSString *)getUserModifyNickNameUrl:(long)_userID openID:(NSString *)_openID nickName:(NSString *)_nickName gender:(int)_gender;


/**
 *  4.5已经登录的用户再次登录
 *
 *  @param _userID  用户id
 *
 *  @return return value description
 */
+(NSString *)getUserReloginUrl:(long)_userID;


/**
 *  4.6更新本地用户信息
 *
 *  @param _userID  用户id
 *
 *  @return return value description
 */
+(NSString *)getUserGetInfoUrl:(long)_userID;


/**
 *  4.7
 *
 *  @param _userID  用户id
 *
 *  @return return value description
 */
+(NSString *)getUserRegisterUrl:(NSString *)kid pwd:(NSString *)pwd sex:(NSInteger)sex;


/**
 *  4.8
 *
 *  @param _userID  用户id
 *
 *  @return return value description
 */
+(NSString *)getUserLoginUrl:(NSString *)kid pwd:(NSString *)pwd;

/**
 *  5.1获取用户收藏列表
 *
 *  @return return value description
 */
-(void)doGetUserCollectedList:(long)_userID begin:(int)_begin num:(int)_num;
+(NSString *)getUserCollectedListUrl:(long)_userID begin:(int)_begin num:(int)_num;

/**
 *  5.2添加收藏
 *
 *  @param _userID  用户id
 *  @param _songId 歌曲ID
 *
 *  @return return value description
 */
-(void)doAddCollectionSong:(long)_userID songId:(long)_songId;
+(NSString *)getAddCollectionSongUrl:(long)_userID songId:(long)_songId;

/**
 *  5.3取消收藏
 *
 *  @param _userID  用户id
 *  @param _roomId 歌曲ID
 *
 *  @return return value description
 */
-(void)doDeleteCollectionSong:(long)_userID songId:(long)_songId;
+(NSString *)getDeleteCollectionSongUrl:(long)_userID songId:(long)_songId;

/**
 *  6.1获取高分榜歌曲列表
 *
 *  @param _userID      用户id
 *  @param _roomId      房间ID
 *  @param _modelId     模块ID
 *  @param _topicID     专题ID
 *
 *  @return return value description
 */
-(void)doGetTopSongList:(long)_userID roomId:(NSString *)_roomId modelID:(int)_modelID topicID:(int)_topicID begin:(int)_begin num:(int)_num;
+(NSString *)getTopSongListUrl:(long)_userID roomId:(NSString *)_roomId modelID:(int)_modelID topicID:(int)_topicID begin:(int)_begin num:(int)_num;

/**
 *  6.4获取banner信息列表
 *
 *  @param _userID  用户id
 *  @param _roomId  房间ID
 *
 *  @return return value description
 */
-(void)doGetBannerList:(long)_userID roomId:(NSString *)_roomId;
+(NSString *)getBannerListUrl:(long)_userID roomId:(NSString *)_roomId;



/**
 *  6.8高分榜上榜歌曲抢榜
 *
 *  @param _userID  用户id
 *  @param _roomId  房间ID
 *  @param _tsID    演唱记录ID
 *
 *  @return return value description
 */
-(void)doReceiveRankSong:(long)_userID roomId:(NSString *)_roomId workID:(long)_workID score:(int)_score date:(NSString *)_date;
+(NSString *)getReceiveRankSongUrl:(long)_userID roomId:(NSString *)_roomId workID:(long)_workID score:(int)_score date:(NSString *)_date;


/**
 *  7.1是否有未领积分(2.0版本新接口)
 *
 *  @param _userID  用户id
 *
 *  @return return value description
 */

+(NSString *)getCheckRewardUrl:(long)_userID;

/**
 *  7.2获取用户的积分信息(2.0版本新接口)
 *
 *  @param _userID  用户id
 *  @param _bid  条数（起始传1）
 *  @param _num  每页条数
 *
 *  @return return value description
 */
+(NSString *)getUserRewardUrl:(long)_userID begin:(int)_bid num:(int)_num;


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
+(NSString *)getChatGiftLogUrl:(long)_userID page:(int)_page size:(int)_size type:(int)_type;

/**
 *  7.4用户金币奖励
 *
 *  @param _userID  用户id
 *  @param _sourceID 积分记录ID
 *  @param _ktvid 积分记录ID
 *
 *  @return return value description
 */
+(NSString *)getReceiveRewardUrl:(long)_userID sourceID:(long)_sID ktvid:(NSString *)_ktvid;


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
-(void)doGetMessageList:(long)_userID roomId:(NSString *)_roomId since:(long)_sid before:(long)_bid num:(int)_num;
+(NSString *)getGetMessageListUrl:(long)_userID roomId:(NSString *)_roomId since:(long)_sid before:(long)_bid num:(int)_num;


/**
 *  8.2获取消息详细信息
 *
 *  @param _userID  用户id
 *  @param _roomId  房间ID
 *  @param _msgID   消息ID
 *  @param _msgType 消息类型
 *  @param _workID  上榜歌演唱记录ID（只有当消息类型为“歌曲上榜”时，该值才会大于0）
 *  @param _point   演唱的得分（只有当消息类型为“歌曲上榜”时，该值才会大于0）
 *
 *  @return return value description
 */
-(void)doGetMessageDetail:(long)_userID roomId:(NSString *)_roomId msgID:(long)_msgID msgType:(int)_msgType workID:(long)_workID point:(int)_point date:(NSString *)_date;
+(NSString *)getGetMessageDetailUrl:(long)_userID roomId:(NSString *)_roomId msgID:(long)_msgID msgType:(int)_msgType workID:(long)_workID point:(int)_point date:(NSString *)_date;

/**
 *  9.1切歌
 *
 *  @param _userID  用户id
 *  @param _ktvId   KTVID
 *  @param _roomId  房间ID
 *  @param _roomPwd 房间密码
 *
 *  @return return value description
 */
-(void)doCutSong:(long)_userID ktvID:(NSString *)_ktvId roomId:(NSString *)_roomId roomPassword:(NSString *)_roomPwd songID:(long)_songID;
+(NSString *)getCutSongUrl:(long)_userID ktvID:(NSString *)_ktvId roomId:(NSString *)_roomId roomPassword:(NSString *)_roomPwd songID:(long)_songID;

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
+(NSString *)getSetVoicePadUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd voice:(int)_voice mic:(int)_mic tone:(int)_tone effect:(int)_effect;

/**
 *  9.3设置暂停
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *  @param _isPause 是否暂停 （1：暂停，2：播放）
 *
 *  @return return value description
 */
+(NSString *)getSetPauseUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd isPause:(int)_isPause;

/**
 *  9.4设置静音
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *  @param _isMute 是否暂停 （0：不静音，1静音）
 *
 *  @return return value description
 */
+(NSString *)getSetMuteUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd isMute:(int)_isMute;

/**
 *  9.5设置伴唱
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *  @param _singMode 是否暂停 （0：原唱，1：伴唱）
 *
 *  @return return value description
 */
+(NSString *)getSetSingModeUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd singMode:(int)_singMode;

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
+(NSString *)getSetScoreModeUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd singMode:(int)_singMode;


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
+(NSString *)getSetAtmosphereUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd _atmosphere:(int)_atmosphere;


/**
 *  9.8设置重唱
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *
 *  @return return value description
 */
+(NSString *)getSetReplayUrl:(NSString *)_ktvId roomID:(NSString *)_roomID roomPassword:(NSString *)_roomPwd;

/**
 *  10.1获取ktv信息
 *
 *
 *  @return return value description
 */
+(NSString *)getGetKtvInfoUrl:(int)_page size:(int)_size;


/**
 *  10.2获取ktv下的礼物信息
 *
 *
 *  @return return value description
 */
+(NSString *)getGetGiftInfoUrl:(NSString *)_ktvid page:(int)_page size:(int)_size;


/**
 *  10.3获取礼物兑换信息
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *
 *  @return return value description
 */

+(NSString *)getGetGiftLogUrl:(long) _idx type:(int)_type page:(int)_page size:(int)_size;


/**
 *  10.4礼物兑换
 *
 *  @param _ktvID   ktvid
 *  @param _roomID  房间ID
 *  @param _roomPwd 房间密码
 *
 *  @return return value description
 */
+(NSString *)getGetGiftExchangeUrl:(long) _idx giftID:(NSString *)_giftID ktvID:(NSString *)_ktvID num:(int)_num;
//用户演唱记录
+(NSString*)getUserSongRecord:(long)idx;

//获取新曲

//获取主题歌曲二级菜单
/**
 *  appstore充值接口
 */
+(NSString *)getIAPPayUrl:(long)_idx ktvID:(NSString *)_ktvID roomID:(NSString *)_roomID identifier:(NSString *)_identifier;

/**
 *  是否显示第三份登录
 */
+(NSString *)getShow;

/*
 请求获取 可领取的红包以及未完成的红包任务
 */
+(NSString*)getHongBaoListUrl:(long)idx type:(int)type; 

//请求领取某个红包
+(NSString*)getHongBaoGetUrl:(long)idx hbid:(NSString*)hbid;


//微信相关
//获取Token
+(NSString*)getWeiXinTokenUrl:(NSString*)appId secret:(NSString*)secret code:(NSString*)code;
//刷新Token
+(NSString*)getWeiXinRefreshTokenUrl:(NSString*)appId refresh:(NSString*)refresh;

@end
