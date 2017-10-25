//
//  KTVBaseConfig.h
//  SinaKtv
//
//  Created by Zhang Chen on 13-10-22.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#ifndef SinaKtv_KTVBaseConfig_h
#define SinaKtv_KTVBaseConfig_h

#import "KTVAppThirdAccount.h"

#ifdef DEBUG
#define PLog(format, ...) NSLog(format, ## __VA_ARGS__)
#else
#define PLog(format, ...)
#endif

//设备屏幕大小
#define _MainScreenFrame   ([[UIScreen mainScreen] bounds])
//设备屏幕分辨率压缩比
#define MAIN_SCREEN_SCALE ([[UIScreen mainScreen] scale])

//绝对分辨率 相对分辨率 之间的转换
#define ABSOLUTE2RELATIVE(resolution) ((resolution)/MAIN_SCREEN_SCALE)
#define RELATIVE2ABSOLUTE(resolution) ((resolution)*MAIN_SCREEN_SCALE)

//设备屏幕宽
#define _MainScreen_Width  _MainScreenFrame.size.width
//设备屏幕高 20,表示状态栏高度.如3.5inch 的高,得到的__MainScreenFrame.size.height是480,而去掉电量那条状态栏,我们真正用到的是460;
#define _MainScreen_Height _MainScreenFrame.size.height-20

#define UIColorFromRGB(rgbValue) [UIColor colorWithRed:((float)((rgbValue & 0xFF0000) >> 16))/255.0 green:((float)((rgbValue & 0xFF00) >> 8))/255.0 blue:((float)(rgbValue & 0xFF))/255.0 alpha:1.0]
//判断设备版本
/*****************************************************/
#define IOS7_OR_LATER   ( [[[UIDevice currentDevice] systemVersion] compare:@"7.0"] != NSOrderedAscending )
/*****************************************************/
//适配设备尺寸 3.5/4.0
#define iPhone5 ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(640, 1136), [[UIScreen mainScreen] currentMode].size) : NO)
/*****************************************************/

#define APP_NAME @"KtvAssistant"
#define chatRowShowCount    100   //聊天内容显示行数

//用于接口版本区别
#define KTV_ASSISTANT_API_VERSION @"versioncode=3.0.1&client=iphone"

//ktv api aes加密密钥
#define KTV_ASSISTANT_API_SECRET_KEY @"a1b2c3d4e5f6ghijklmnopqrstuvwxyz"

//点歌界面通知
#define RECEIVE_VOD_SONG_SUCCESSFUL_NOTIFICATION      @"receive_vod_song_successful_notification"
#define RECEIVE_DELETE_SONG_SUCCESSFUL_NOTIFICATION   @"receive_delete_song_successful_notification"
#define RECEIVE_TOP_SONG_SUCCESSFUL_NOTIFICATION      @"receive_top_song_successful_notification"
#define RECEIVE_REFLESH_NOTIFICATION                  @"receive_reflesh_notification"
#define RECEIVE_DELETE_PRESONG_NOTIFICATION           @"receive_delete_presong_notification"
#define RECEIVE_ROOM_HAS_CLOSED_NOTIFICATION          @"receive_room_has_closed_notification"

#define PRE_VOD_SONGLIST_PATH                         @"presongdata.dat"

/*****************************************************/
//程序内消息
/*****************************************************/
#define KTVAssistantLoginLobbyResult                    @"KTVAssistantLoginLobbyResult" //登录消息通知
#define KTVASSISTANT_GOLD_UPDATE_NOTIFICATION           @"ktvassistant_gold_update_notification" //更新金币消息通知
#define KTVASSISTANT_USERINFO_UPDATE_NOTIFICATION       @"ktvassistant_userinfo_update_notification" //更新用户信息消息通知
#define KTVASSISTANT_CLOSEROOM_NOTIFICATION             @"ktvassistant_closeroom_notification" //包厢关闭消息通知
#define KTVASSISTANT_ISSHOW_NOTIFICATION                @"ktvassistant_isshow_notification" //是否隐藏审核问题通知
//#define KTVASSISTANT_CHAT_IMG_LABEL_CLICK_NOTIFICATION  @"ktvassistant_chat_img_label_click_notification" //公聊界面点击头像名称消息通知
#define KTVASSISTANT_GIFT_SENDTOSOMEONE_NOTIFICAITON    @"ktvassistant_gif_sendtosomeone_notification" //选择用户送礼按钮消息通知
#define KTVASSISTANT_CHAT_GIFT_RED_CLICK_NOTIFICATION  @"ktvassistant_chat_gift_red_click_notification" //点击私聊、送礼、红包按钮消息通知
#define KTVASSISTANT_ISSHOW_CHAT_GIFT_RED_CLICK_NOTIFICATION  @"ktvassistant_isshow_chat_gift_red_click_notification" //私聊、送礼、红包按钮是否显示消息

/*****************************************************/
//UserDefaults key
/*****************************************************/
#define KTVASSISTANT_USERDEFAULTS_ISSHOW @"KTVASSISTANT_UserDefaults_ISSHOW"

/*
 * 注意，切换正式和测试服务器时，下面域名需要同时修改
 *
 * 定义不同的值说明:
 * [#define IS_DEBUG_SERVER 0] 为正式服务器
 * [#define IS_DEBUG_SERVER 1] 为临时正式服务器，测试环境
 * [#define IS_DEBUG_SERVER 2] 为测试服务器，测试环境
 */

#define IS_DEBUG_SERVER 0
#if (0 == IS_DEBUG_SERVER)

//api domain
#define kPKtvAssistantAPIDomain                         @"http://assistant.17xg.com/"
 //兑换奖品
#define kPKtvAssistantAPIExchangeAwardUrl               @"http://debug.sinaktvserver.9158.com/web/gift/index"
//设置
//应用推荐
#define kPKtvAssistantAPIAppRecommendedUrl              @"http://ktvapi.9158.com/web/App/RecommendKTV"
 //包厢预订
#define kPKtvAssistantAPIAppBoxVodUrl                   @"http://ktv23.17xg.com/admin/activitys/all"
//app store上的更新地址
#define kPKtvAssistantAppUpdateUrl                      @"https://itunes.apple.com/cn/app/id765420058"


#elif (1 == IS_DEBUG_SERVER)

#define kPKtvAssistantAPIDomain                         @"http://192.168.83.166:8080/"
#define kPKtvAssistantAPIExchangeAwardUrl               @"http://debug.sinaktvserver.9158.com/web/gift/index"
#define kPKtvAssistantAPIAppRecommendedUrl              @"http://debug.ktvapi.9158.com/web/App/RecommendKTV"
#define kPKtvAssistantAPIAppBoxVodUrl                   @"http://ktv.17xg.com/admin/activitys/all"
#define kPKtvAssistantAppUpdateUrl                      @"https://itunes.apple.com/cn/app/id765420058"

#elif (2 == IS_DEBUG_SERVER)

#define kPKtvAssistantAPIDomain                         @"http://testassistant.17xg.com/"
#define kPKtvAssistantAPIExchangeAwardUrl               @"http://debug.sinaktvserver.9158.com/web/gift/index"
#define kPKtvAssistantAPIAppRecommendedUrl              @"http://ktvapi.9158.com/web/App/RecommendKTV"
#define kPKtvAssistantAPIAppBoxVodUrl                   @"http://ktv.17xg.com/admin/activitys/all"
#define kPKtvAssistantAppUpdateUrl                      @"https://itunes.apple.com/cn/app/id765420058"

#endif


/*
 * 1 USER API
 */
//1.1登录包厢
#define kPKtvAssistantAPILogin                          @"Assistant/Login"
//1.2退出包厢
#define kPKtvAssistantAPILogout                         @"User/logout"
//1.3登录包厢（new）
#define kPKtvAssistantAPIRoomLogin                      @"v11/Assistant/Login"
#define kPKtvAssistantAPIRoomLoginEx                 @"v11/Assistant/LoginRoom"
//1.4包厢退出（new）
#define kPKtvAssistantAPIRoomLogout                     @"v11/Assistant/Logout"
//第三方平台用户登录
#define kPKtvAssistantAPIUserOpenLogin                  @"User/openLogin"
//第三方平台用户退出登录
#define kPKtvAssistantAPIUserLogout                     @"User/userLogout"
//用户上传头像
#define kPKtvAssistantAPIUserUploadPhoto                @"User/uploadPhoto"
//用户修改昵称
#define kPKtvAssistantAPIUserModifyNickName             @"User/modifyNickName"
//已经登录的用户再次登录
#define kPKtvAssistantAPIUserRelogin                    @"User/reLogin"
//更新本地用户信息
#define kPKtvAssistantAPIUserGetInfo                    @"User/getInfo"
//手机用户注册
#define kPKtvAssistantAPIUserRegister                   @"v20/User/Register"
//手机用户登录
#define kPKtvAssistantAPIUserLgoin                      @"v20/User/Login"


/*
 * 2 SONG API
 */
#define rankBtnTag       1001
#define newSongBtnTag    1002
#define songStarBtnTag   1003
#define songKindBtnTag   1004
#define languageBtnTag   1005

//2.0模块id定义
#define kPKtvAssistantAPIModelType0                     0
#define kPKtvAssistantAPIModelTypeNewSong               1//新曲，二级菜单
#define kPKtvAssistantAPIModelTypePinYin                2//拼音，手机端没有
#define kPKtvAssistantAPIModelTypeArtist                3//歌星，三级菜单
#define kPKtvAssistantAPIModelTypeRank                  4//排行榜，二级菜单（1.1后称为高分榜）
#define kPKtvAssistantAPIModelTypeLanguageType       5//语种，二级菜单
#define kPKtvAssistantAPIModelTypeMusicType              6//曲种，二级菜单
#define kPKtvAssistantAPIModelTypeSelectedTopics      7//精选专题，二级菜单
#define kPKtvAssistantAPIModelTypeTopRanking            8//热门排行，二级菜单
#define kPKtvAssistantAPIModelTypeTheme                     9//主题 ，获取二级菜单

//2.1歌曲搜索
#define kPKtvAssistantAPISongSearch                     @"Assistant/Search"
//2.2包厢点歌模块分类
#define kPKtvAssistantAPISongGetModelType               @""
//2.3根据点歌模块获取歌曲专题列表
#define kPKtvAssistantAPISongGetSongTopics              @"V20/Assistant/GetSongTopics"
//2.4根据模块和歌曲专题获取歌曲列表
#define kPKtvAssistantAPISongGetSongListByTopic         @"Assistant/GetSongListByTopic"
//2.5根据原唱歌手获取歌曲列表
#define kPKtvAssistantAPISongGetSongListByArtist        @"Assistant/GetSongListByArtist"
//2.6点歌
#define kPKtvAssistantAPISongOrderSong                  @"V20/Assistant/OrderSong"
//2.7获取已点歌曲列表
#define kPKtvAssistantAPISongGetOrderedSongList         @"Assistant/GetOrderedSongList"
//2.8置顶歌曲
#define kPKtvAssistantAPISongSetSongToTop               @"Assistant/SetSongToTop"
//2.9删除已点歌曲
#define kPKtvAssistantAPISongDeleteFromOrdered          @"Assistant/DeleteSongFromOrdered"
//2.10点歌并置顶歌曲
#define kPKtvAssistantAPISongAddAndSetSongToTop         @"V20/Assistant/AddAndSetSongToTop"
//2.11获取当前正在播放的歌曲
#define kPKtvAssistantAPISongGetCurrentSong             @"Assistant/GetCurrendSong"
//2.12歌手的歌曲搜索
#define kPKtvAssistantAPISongSearchByArtist             @"V11/Assistant/SearchBySingerIdKeyword"
//6.1获取高分榜歌曲列表
#define kPKtvAssistantAPISongGetTopSongList             @"Top/getTopSongList"
//6.4获取banner信息列表
#define kPKtvAssistantAPIGetBannerList                  @"Top/getBannerList"
//6.8高分榜上榜歌曲抢榜
#define kPKtvAssistantAPISongReceiveRankSong            @"Top/receiveRankSong"

/*
 红包
 */
//请求红包列表
#define kPKtvAssistantAPIHongBaoList                        @"User/GetUserRewardList"
//请求领取某个红包
#define kPKtvAssistantAPIHongBaoGet                         @"User/GetGoldFromReward"

/*
 * 收藏 API
 */
//获取收藏列表
#define kPKtvAssistantAPISongGetCollectedList           @"fav/getCollectedList"
//添加收藏
#define kPKtvAssistantAPISongAddCollection              @"fav/addCollection"
//删除收藏
#define kPKtvAssistantAPISongDeleteCollection           @"fav/deleteCollection"

/*
 * 积分 API－（2.0之前的）
 */
//是否有未领积分
#define kPKtvAssistantAPIPointCheckPoint                @"Point/checkPoint"
//获取用户的积分信息
#define kPKtvAssistantAPIPointGetPoints                 @"Point/getUserPoints"
//领取积分
#define kPKtvAssistantAPIPointReceivePoints             @"Point/receivePoints"


/*
 * 金币 API－（新版本）
 */
//是否有未领积分
#define kPKtvAssistantAPIRewardCheck                    @"V20/Reward/checkReceive"
//获取用户的积分信息
#define kPKtvAssistantAPIRewardGet                      @"V20/Reward/getRewardList"
//领取积分
#define kPKtvAssistantAPIRewardReceive                  @"V20/Reward/receiveReward"
//用户聊天送礼金币变化标
#define kPKtvAssistantAPIChatGiftLog                    @"V20/Gift/GetChatGiftLog"


/*
 * 金币 API－（新版本）
 */
//获取ktv信息
#define kPKtvAssistantAPIGetKtvInfo                     @"V20/Gift/getKtvInfoList"
//获取ktv下的礼物信息
#define kPKtvAssistantAPIGetGiftInfo                    @"V20/Gift/getGiftInfoList"
//获取礼物兑换信息
#define kPKtvAssistantAPIGetGiftLog                     @"V20/Gift/getGiftLogList"
//礼物兑换
#define kPKtvAssistantAPIGiftExchange                   @"V20/Gift/GiftExchange"
//获取用户歌曲最近的演唱记录 (或者最高)
#define kPKtvAssistantAPISongRecord             @"User/GetUserSongInfo"

/*
 * 支付 API
 */
//获取消息列表
#define kPKtvAssistantAPIAppStorePay                    @"V20/Pay/IAPPay"


/*
 * 消息 API
 */
//获取消息列表
#define kPKtvAssistantAPIMsgGetMessageList              @"Msg/getMessageList"
//获取消息的详细信息
#define kPKtvAssistantAPIMsgGetMessageDetail            @"Msg/getMessageDetail"

/*
 * 包厢操作 API
 */
//切歌
#define kPKtvAssistantAPICutSong                        @"control/cutSong"
//设置调音版
#define kPKtvAssistantAPISetVoicePad                    @"control/setVoicePad"
//设置暂停或者开始
#define kPKTVAssistantAPISetPause                       @"control/setPauseOnOff"
//设置静音
#define kPKTVAssistantAPISetMute                        @"control/setMute"
//设置伴唱
#define kPKTVAssistantAPISetSingMode                    @"control/setSingMode"
//设置评分模式
#define kPKTVAssistantAPISetScoreStatus                 @"control/setScoreStatus"
//设置气氛
#define kPKTVAssistantAPISetAtmosphere                  @"control/setAtmosphere"
//设置重唱
#define kPKTVAssistantAPISetReplay                      @"control/setReplay"

/*
 * 3 ACTIVITY API
 */
//3.1包厢预定活动信息
#define kPKtvAssistantAPIActivityNew                    @""

//3.1包厢预定活动信息
#define kPKtvAssistantAPIShowThird                      @"Set/ShowThird"

//error msg
#define kPKtvAssistantErrorMsgUnknow                    @"未知错误"
#define kPKtvAssistantErrorMsgSystem                    @"系统忙,请稍后再试试"
#define kPKtvAssistantErrorMsgJsonError                 @"Json 解析失败"

//error code
//成功代码，没有错误，ErrorSuccessCode
#define kPKtvAssistantErrorCodeForNothing               @"0"
//失败代码，ErrorErrorCode
#define kPKtvAssistantErrorCodeForError                 @"1"

//口令错误或者房间已关闭
#define kPKtvAssistantErrorCodeForLogin                 @"90000"//客户端定义

//房间不存在
#define kPKtvAssistantErrorCodeForNoRoom                @"50001"
//点播歌曲超过99首
#define kPKtvAssistantErrorCodeForOrderOverage          @"50002"
//Socket错误
#define kPKtvAssistantErrorCodeForSocketError           @"50003"
//歌曲不存在或者已存在
#define kPKtvAssistantErrorCodeForSongError             @"50004"
//数据接收错误
#define kPKtvAssistantErrorCodeForDataError             @"50005"
//歌曲已存在
#define kPKtvAssistantErrorCodeForSongExisted           @"50006"
//房间关闭
#define kPKtvAssistantErrorCodeForRoomClosed            @"50007"
//房间维护
#define kPKtvAssistantErrorCodeForRoomSafeguard         @"50008"
//歌曲正在播放
#define kPKtvAssistantErrorCodeForSongPlaying           @"50009"
//歌曲已收藏
#define kPKtvAssistantErrorCodeForSongAlreadyCollected  @"90001"
//收藏已删除
#define kPKtvAssistantErrorCodeForSongCollectedError    @"90002"


//api error code
//参数传递错误
#define kPKtvAssistantErrorCodeForParamPostError        @"10001"
//Secret错误
#define kPKtvAssistantErrorCodeForSecretError           @"10002"

#endif
