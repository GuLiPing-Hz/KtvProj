package assistant.global;


public interface KtvAssistantAPIConfig {
	/** ktvapi域名 */
	String KtvAssistantAPIDomain = "http://assistant.17xg.com/";
	//String KtvAssistantAPIDomain = "http://testassistant.17xg.com/";
	
	//应用介绍页面
	String APP_INTRODUCE = "http://ktv23.17xg.com/introduce.html";
	//apk 主动查询更新版本页面
	String APK_CHECK_VERSION_URL = "http://www.17xg.com/KtvAssistant/ktvassistant_version.html"; 
	//支付宝充值异步通知页面
	String ALIPAY_NOTIFY_URL = "http://assistant.17xg.com/v20/Alipay/pay";
	//包厢预订页面
	String ROOM_ORDER = "http://ktv23.17xg.com/admin/activitys/all";
	//"http://ktv.17xg.com/admin/activitys/all"
	//渠道
	String MYSELF_CHANNEL_MAIN = "MYSELF_CHANNEL_MAIN";
	String MYSELF_CHANNEL_CHILD = "MYSELF_CHANNEL_CHILD";

	String KtvAssistantAPIAppUpdate = "Assistant/appupdate";

	/** aes密钥 */
	String KTV_ASSISTANT_APP_SECRET_KEY = "a1b2c3d4e5f6ghijklmnopqrstuvwxyz";

	/** 接口定义 */
	interface APIUrl {
		// 1. 包厢
		String RoomLogin = "V11/Assistant/Login";// 1.1登录包厢
		String RoomLogout = "V11/Assistant/Logout";// 1.2退出包厢
		String RoomLoginEx = "V11/Assistant/LoginRoom";//新版本包厢登陆

		// 2. 点歌
		String SongSearch = "Assistant/Search";// 2.1歌曲搜索
		String SongGetModelType = "";// 2.2包厢点歌模块分类
		String SongGetSongTopics = "V20/Assistant/GetSongTopics";// 2.3根据点歌模块获取歌曲专题列表
		String SongGetSongListByTopic = "Assistant/GetSongListByTopic";// 2.4根据模块和歌曲专题获取歌曲列表
		String SongGetSongListByArtist = "Assistant/GetSongListByArtist";// 2.5根据原唱歌手获取歌曲列表
		String SongOrderSong = "V20/Assistant/OrderSong";// 2.6点歌
		String SongGetOrderedSongList = "Assistant/GetOrderedSongList";// 2.7获取已点歌曲列表
		String SongSetSongToTop = "Assistant/SetSongToTop";// 2.8置顶歌曲
		String SongDeleteFromOrdered = "Assistant/DeleteSongFromOrdered";// 2.9删除已点歌曲
		String SongAddAndSetSongToTop = "V20/Assistant/AddAndSetSongToTop";// 2.10点歌并置顶歌曲
		String SongGetCurrentSong = "Assistant/GetCurrendSong";// 2.11获取当前正在播放的歌曲
		String SongSearchByArtist = "V11/Assistant/SearchBySingerIdKeyword";// 2.1搜索指定歌手下的歌曲
		// 3. webview
		String ActivityNew = "";// 3.1包厢预定活动信息
		String RecommendApp = "web/App/RecommendKtvAndroid";// 3.2应用推荐
		
		// 4. 账户
		String ThirdLogin = "User/openLogin";// 4.1 第三方平台用户登录
		String UploadPhoto = "User/uploadPhoto";// 4.2 用户头像上传
		String ThirdLogout = "User/userLogout";// 4.3 用户退出登录
		String ModifyNickName = "User/modifyNickName";// 4.4 用户昵称修改
		String AutoLogin = "User/reLogin";//用于客户端自动登陆，这里会给每天登陆的金币奖励
		String UserInfo = "User/getInfo";//获取最新的用户信息
		String Register="V20/USER/Register";//注册用户
		String Login="V20/User/Login";//登陆
		String SongRecordGet = "User/GetUserSongInfo";//获取用户演唱记录最近20条

		// 5. 收藏
		String GetCollectList = "fav/getCollectedList";// 5.1 获取收藏列表
		String CollectSong = "fav/addCollection";// 5.2 收藏歌曲
		String DeleteCollectSong = "fav/deleteCollection";// 5.3 删除收藏歌曲

		// 6. 高分榜
		String GradeTopSong="";//获取到各个排名的前几首评分歌曲 glp
		String GetTopScoreSongList = "top/getTopSongList";// 6.1 获取高分榜歌曲列
		String GetBannerList = "top/getBannerList";// 6.4 获取banner信息列表
		String PossessRankWork = "top/receiveRankSong";// 6.8 高分榜上榜歌曲抢榜

		// 7. 积分
		String CheckPoint = "point/checkPoint";// 7.1 是否有未领积分
		String GetUserPoints = "point/getUserPoints";// 7.2 获取用户的积分信息
		String ReceivePoints = "point/receivePoints";// 7.3 用户领取积分

		// 8.消息
		String GetMessageList = "msg/getMessageList";// 8.1 获取系统消息列表
		String GetMessageDetail = "msg/getMessageDetail";// 8.2
															// 根据消息ID和消息类型获取消息的详细信息

		// 9.歌曲控制
		String ChangeSingingSong = "control/cutSong";// 9.1 切歌
		String VoicePad = "control/SetVoicePad";//音量面板
		String PauseOO = "control/SetPauseOnOff";//暂停继续
		String MuteOO = "control/SetMute";//静音切换
		String SingMode = "control/SetSingMode";//切换原伴唱
		String ScoreState = "control/SetScoreStatus";//切换演唱评分模式 单人->Pk->无 ； 对唱->无
		String Atmosphere = "control/SetAtmosphere";//气氛
		String Resing = "control/SetReplay";//重唱
		
		//10.门店信息
		String KtvShopInfo = "V20/Gift/GetKtvInfoList";//获取门店列表信息
		String GiftInfoList = "V20/Gift/GetGiftInfoList";//获取礼物信息
		String GiftExchangeInfo = "V20/Gift/GetGiftLogList";//获取礼物兑换记录
		String GiftExchange = "V20/Gift/GiftExchange";//兑换礼物
		String GiftSendRecvInfo = "V20/Gift/GetChatGiftLog";//获取礼物赠送接受记录
		String GoldGetInfo = "V20/Reward/getRewardList";//获取用户金币增加的记录
		
		String GetRoomGold = "V20/Reward/GetRoomGold";//获取包厢金币
		
		String GetHongBaoList = "User/GetUserRewardList";//请求红包列表
		String GetHongBao = "User/GetGoldFromReward";//请求领取某个红包
	}

	/*********** 以下是与API服务端交互的错误 *************/
	// error msg
	String ErrorMsgUnknow = "网络异常,请求失败";
	String ErrorMsgSystem = "系统忙,请稍后再试试";

	/** 返回的错误代码 */
	interface APIErrorCode {
		/** 成功代码，没有错误，ErrorSuccessCode */
		int Nothing = 0;
		/** 失败代码，Error */
		int Error = 1;

		/** 参数传递错误 */
		int ParamPostError = 10001;
		/** Secret错误 */
		int SecretError = 10002;
		/** 任务过多,系统繁忙 */
		int ServerBusy = 10003;
		/** 任务超时 */
		int TimeOut = 10004;
		/** 不合法的微博用户 */
		int InvalidUser = 10005;
		/** 数据库错误,请联系系统管理员 */
		int DataBaseError = 10006;
		/** 数据未找到 */
		int DataNotFound = 10007;
		/** 没有操作权限 */
		int NoPermission = 10008;
		
		//房间没有金币
		int NoRoomGold = 11002;
		/** 房间不存在 */
		int NoRoom = 50001;
		/** 点播歌曲超过99首 */
		int OrderOverage = 50002;
		/** Socket错误 */
		int SocketError = 50003;
		/** 歌曲不存在 */
		int SongNoExist = 50004;
		/** 数据接收错误 */
		int DataError = 50005;
		/** 歌曲已存在 */
		int SongExisted = 50006;
		/** 房间关闭 */
		int RoomClosed = 50007;
		/** 房间维护 */
		int RoomRepair = 50008;
		/** 歌曲正在播放 */
		int NowPlaying = 50009;

		/** 积分已领取 */
		int PointHaveReceived = 60001;
		/** 积分已过期 */
		int PointOutOfTime = 60002;
		//金币不足
		int GoldNotEnough = 60003;
		
		/** 9158登录失败 */
		int LoginError = 70002;
		
		/** 榜单被抢 */
		int TopRankBeRobbed = 80001;
		/** 榜单排名被超越 */
		int TopRankBeSurpassed = 80002;
		/** 榜单过期 */
		int TopRankOutDate = 80003;
		
		/** 已收藏 */
		int HaveCollected = 90001;
		/** 删除收藏失败 */
		int DeleteCollectionFail = 90002;
		
		/** 网络超时 */
		int NetConnOutOfTime = 101000;
		
	}
}
