#ifndef _protocal_h_
#define _protocal_h_

enum DBAGENTCMD
{
	CMD_CALLSP = 0x01,
};

enum PARAM_TYPE
{
	PT_UNKNOWN = 0,
	PT_INPUT = 1,
	PT_OUTPUT = 2,
};

enum PARAM_DATATYPE
{
	PDT_EMPTY = 0,
	PDT_INT = 1,
	PDT_VARCHAR = 2,
};

enum RETCODE//lobby
{
	RET_NONEEDUPDATE = -3,//用户信息是最新的，无需更新
	RET_NOTEXIST		= -2,	//用户不存在
	RET_PASSWDERROR		= -1,	//密码错误  RET_CACHEERROR
	RET_OK				= 0,	//登录成功
	RET_NOAVATAR		= 1,	//没有创建avatar信息
	RET_DBERROR = 2, //数据库操作错误，导致失败
	RET_VERSIONERROR = 10, //客户端版本号过低
};

//add by zhang 2010.02.05  加好友状态
enum ClientADDFriendStatus//lobby
{
	STATUS_NORMAL = 0,  // 正常
	STATUS_INPROCESS = 1,  //正处理状态
	STATUS_BLACK = 2,  //服务器拒绝状态
	STATUS_FRIEND = 3,  //已是好友
	STATUS_REFUSE = 4,  //用户拒绝
	STATUS_USER_NOTEXIST = 5, //用户不在线
	STATUS_DBERROR = 6, //数据库返回失败
	STATUS_HAS_BLACKED = 7, //已经是对方的黑名单成员
	STATUS_ADDFRIBLK_MAXCOUNT = -1,	//好友列表或黑名单已满
};

enum CLUB_MSG_TYPE
{
	CLUB_MSG_APPLY = 0,
	CLUB_MSG_EXIT, 
	CLUB_MSG_DEL, 
};
//end
//所有服务器跟日志服务器之间的协议(没有包括状态服务器跟日志服务器之间的协议)
enum LOG_SERVER_CMD
{
	//房间服务器过来的协议
	CMD_MUSIC_CLICK_R2LS		=0,	//音乐点击报告
	CMD_ROOM_KEEPALIVE_R2LS		  ,	//用户上麦状态报告
	CMD_ROOM_ONMIC_R2LS			  ,	//上表演麦报告
	CMD_ROOM_OFFMIC_R2LS		  ,	//下表演麦报告
	CMD_ROOM_ONVJMIC_R2LS		  ,	//上主持麦报告
	CMD_ROOM_OFFVJMIC_R2LS		  ,	//下主持麦报告
	CMD_ROOM_ERRORINFO_R2LS		  , //房间错误信息报告
	CMD_ROOM_USERSTATUS_R2LS      , //客户端实时信息
	CMD_ROOM_ENTERROOM_R2LS		  ,	//用户进入房间
	CMD_ROOM_LEAVEROOM_R2LS		  ,	//用户离开房间

	//大厅服务器过来的协议
	CMD_LOBBY_SYSINFO_L2LS		=1000, //报告系统信息
	CMD_LOBBY_ERRORINFO_L2LS		, //大厅错误信息系统信息
	CMD_LOBBY_LOGININFO_L2LS		, //登录的信息，mac和ip
	CMD_LOBBY_PRIVATECHAT_L2LS  	//用户私聊信息
};

enum BROADTYPE
{
	BT_BROAD, //普通喇叭
	BT_EXCHANGE,//礼物兑换
	BT_RANKSONG,//歌曲上榜
};
/*
	具体协议列表
*/
//*******************************Auth 服务器协议 BEGIN**************************
enum AUTH_CMD_GameClient//Auth服务器与星光客户端使用的协议
{
    //id登录
	CMD_GAME_GETTOKEN_BYID = 1,
	//idx登录					    ReserveData_Idauth	int *ret 具体 RETCODE
	CMD_GAME_GETTOKEN_BYIDX = 2,
	CMD_GAME_MODIFYPWD_BYID,
	CMD_GAME_MODIFYPWD_BYIDX,
    //取最新token	            ReserveData_Idxauth	int *ret 具体 RETCODE
	CMD_GETNEWESTTOKEN = 8,//??
	//游客登录					ReserveData			int *ret
	CMD_GAME_XG_VISTOR = 9
};

enum AUTH_CMD_WebClient
{
	CMD_WEB_GETTOKEN_BYID = 1,
	CMD_WEB_GETTOKEN_BYIDX,
	CMD_WEB_MODIFYPWD_BYID,
	CMD_WEB_MODIFYPWD_BYIDX,
};

enum AUTH_CMD_RestrictClient
{
	CMD_RST_GETRESTRICT = 1,
	CMD_RST_SETRESTRICT,
	CMD_RST_DELRESTRICT,
};

enum AUTH_CMD_CacheClient
{
	CMD_CACHE_GETPWD = 1000, //获取密码
	CMD_CACHE_SETPWD, //写密码
	CMD_CACHE_DELPWD,
};

enum AUTH_CMD_DbAgentClient
{
};

enum AUTH_CMD_LogClient
{
	CMD_LOG_MODIFYPWD_ERROR = 1,
};

enum AUTH_CMD_MappingClient
{
	CMD_MAPPING_GETID = 1000,
	CMD_MAPPING_WRITEIDX2ID,
};

enum AUTH_CMD_InnerCmd
{
	CMD_INNER_RST_GETRESTRICT = 1,
	CMD_INNER_RST_GETRESTRICT_WITHAUTH, 
	CMD_INNER_MAPPING_GETID,
	CMD_INNER_CACHE_GETPWD,
	CMD_INNER_CACHE_MODIFYPWD,
	CMD_INNER_DB_GETAUTH_BYID,
	CMD_INNER_DB_GETAUTH_BYIDX,
	CMD_INNER_DB_MODIFYPWD_BYID,
	CMD_INNER_DB_MODIFYPWD_BYIDX,
	CMD_INNER_DB_XG_GETVISTOR,
};
//*******************************Auth 服务器协议 END**************************

//*******************************Lobby 服务器协议 BEGIN**************************
//收到的数据包对应的参数类型比对：void(*OnPackage)(const void*,short cmd,const void*);
//第一个参数是ReserveData结构的指针,第二个参数是协议命令字,第三个参数看具体协议而定
//------第一个参数		第三个参数
enum LOBBY_CMD_XGCLIENT
{
	//token认证,返回本人的用户资料															ReserveData		ResultData_LobbyLogin
	CMD_TOKENLOGIN_C2L2C = 1,
	//通知给其他用户大厅用户登陆(通知消息)													NULL			ResultData_LobbyLoginUser
	CMD_USERLOGIN_L2C,
	//通知有用户退出 NULL ResultData_lobbyLoginOutUser
	CMD_USERLOGOUT_L2C,
	//心跳包
	CMD_KEEPALIVE_C2L,

	//用户管理
	//客户端向大厅请求某个idx的用户资料	NULL(没有对应的前位信息,之后同上)		ResultData_LobbyLogin(用户各个信息字段)
	CMD_GETUSERINFO_C2L2C,
	//修改用户nick,用户资料中,只有nick可以被修改
	CMD_CHANGENICKNAME_C2L2C,

	//聊天管理
	//大厅内的用户私聊,也可以是大厅之间,包括好友和陌生人之间聊天,如果lobby的内存里没找到这个id,就发给lobbycenter
	//NULL			ResultData_ChatMessage(聊天对方的信息和聊天内容)
	CMD_USERCHAT_C2L2C,
	//俱乐部群聊		NULL			ResultData_ChatMessage(聊天对方的信息和聊天内容)
	CMD_CLUBCHAT_C2L2C,
	//大厅内的广播	NULL			ResultData_ChatMessage(聊天对方的信息和聊天内容)
	CMD_LOBBYCHAT_C2L2C,
	//大厅间的广播	NULL			ResultData_ChatMessage(聊天对方的信息和聊天内容)
	CMD_ALLCHAT_C2L2C,
	

	//好友管理
	//加好友								ReserveData_AddFriend(包含操作对象用户id)	ret(int *) >=0 成功
	CMD_ADDFRIEND_C2L2C,
	//加好友通知(通知消息)		NULL			int *idx(对方的idx)
	CMD_ADDFRIENDINFORM_L2C,
	//加黑名单							ReserveData_AddBlack(包含操作对象用户id)	ret(int *) >=0 成功
	CMD_ADDBLACK_C2L2C,
	//删好友								ReserveData_DelFriend(包含操作对象用户id)	ret(int *) >=0 成功
	CMD_DELFRIEND_C2L2C,
	//删黑名单							ReserveData_DelBlack(包含操作对象用户id)	ret(int *) >=0 成功
	CMD_DELBLACK_C2L2C,
	//取好友列表和好友状态		NULL			ResultData_GetFriendlist(用户列表)
	CMD_GETFRIENDLISTSTATUS_C2L2C,
	//取黑名单列表					NULL			ResultData_GetBlacklist(用户列表)
	CMD_GETBLACKLIST_C2L2C,

	//俱乐部管理
	//新建俱乐部						ReserveData_ApplyClub	int *clubid
	CMD_CREATECLUB_C2L2C,
	//解散俱乐部						ReserveData_ApplyClub	int *ret
	CMD_DISMISSCLUB_C2L2C,
	//管理员修改俱乐部信息
	CMD_CHANGECLUBINFO_C2L2C,
	//管理员修改管理员助理
	CMD_CHANGECLUBASSISTANT_C2L2C,
	//申请加入俱乐部,3种,都通过,都不通过,审核通过,管理员在时直接发给管理员,不在时如果要审核通过保存在临时表里	
	//ReserveData_ApplyClub		ret(int *) >=0 成功	具体 CLUB_ APPLY_RETCODE
	CMD_APPLYADDCLUB_C2L2C,
	//退出俱乐部																			ReserveData_ApplyClub		ret(int *) >=0 成功
	CMD_EXITCLUB_C2L2C,
	//管理员获得最近几天内的申请数量														ReserveData_ApplyClub		number(int *) >=0 成功
	CMD_GETAPPLYLISTCOUNT_C2L2C,
	//管理员获得最近几天内的申请列表														ReserveData_ApplyClub		ResultData_ClubApplyList
	CMD_GETAPPLYLIST_C2L2C,
	//加入俱乐部,可能是审批通过,也可能是俱乐部加入被设置成不需审批						NULL			int *id申请编号(组提交,分条返回)
	CMD_APPLYADDCLUBOK_C2L2C,
	//有人成功加入俱乐部通知消息(通知消息)													NULL			int *idx
	CMD_INFORMUSERADDCLUB_L2C,
	//删除申请列表																			NULL			int *id申请编号(组提交,分条返回)
	CMD_DELAPPLYLIST_C2L2C,
	//取俱乐部列表和状态																	NULL			ResultData_GetClublist(俱乐部列表)
	CMD_GETCLUBLISTSTATUS_C2L2C,
	//根据俱乐部id取俱乐部信息
	CMD_GETCLUBINFO_C2L2C,

	//房间管理	
	//创建房间
	CMD_CREATEROOM_C2L2C,
	//删除房间
	CMD_DELROOM_C2L2C,
	//进入房间
	CMD_USERENTERROOM_L2C,
	//离开房间
	CMD_USERLEAVEROOM_L2C,
	//获得的房间列表					NULL			ResultData_RoomInfo
	CMD_GETROOMLIST_C2L2C,
	//获得的房间信息
	CMD_GETROOMINFO_C2L2C,
	//获得区列表和频道列表		NULL			ResultData_GetChannellist(大厅通道列表)
	CMD_GETSECTIONLIST_C2L2C,
	
	//头像管理
	//创建头像																				NULL			ResultData_GetChannellist
	CMD_CREATEAVATAR_C2L2C,
	
	CMD_BROAD_ALL_C2L2C,    //大喇叭回应	//modify by jinguanfu 2010/5/13
	CMD_BROAD_LOBBY_C2L2C,	//小喇叭回应	//modify by jinguanfu 2010/5/13
	CMD_SYSTEMMSG_L2C,		//系统公告包体

	CMD_GETLOBBYINFO_C2L2C,	//取得大厅信息

	CMD_PRESENTBROADCAST_L2C, //礼物广播

	CMD_SYSTEMSPEAKER_L2C, //系统喇叭
	//add by zhang 2009.12.28
	CMD_USERLOGOUT_C2L, //用户登出通知大厅
	//end
	CMD_GETFRIENDLOCATION_C2L2C,//取得好友所在位置

	CMD_GETPARTINFO_C2L2C,		//取得角色信息

	CMD_GETROOMLISTBYLOBBY_C2L2C,	//获取房间列表
	//add by jinguanfu 2010/1/20
	CMD_REQUESTADDFRIEND_L2C2L,	//接收添加好友请求,返回处理结果
	//add by jinguanfu 2010/1/27
	CMD_DELETEAVARTA_C2L2C,		//删除角色信息
	//add by jinguanfu 2010/4/8
	CMD_LOGIN_AGAIN_L2C,		//重复登录通知
	CMD_GETROOMSIMPLEINFOS_C2L2C, //得到大厅所有的房间的简易信息

	//新俱乐部命令
	CMD_DENY_APPLYCLUB_L2C,		//通知客户端,俱乐部申请被拒绝
	CMD_CLUBMSG_L2C,			//通知客户端,俱乐部MSG

	CMD_BROAD_SB_ALL_L2C,			//大喇叭	//add by jinguanfu 2010/5/13
	CMD_BROAD_SB_LOBBY_L2C,		//小喇叭	//add by jinguanfu 2010/5/13
	CMD_CHANGEAVATAR_C2L2C,	//修改avarta// add by cx 10-5-20
	CMD_CHANGEPOWER_C2L2C,		//权限更新通知 //add by jinguanfu 2010/6/5

	//add by cx 10-7-6
	CMD_BUYGOODS_C2L2C,				// 购买商品
	CMD_GETPACKINFO_C2L2C,			// 取个人背包信息
	CMD_GOODS_DATED_L2C,			// 商品过期通知

	CMD_DELPACKINFO_C2L2C,			// 删除商品
	CMD_SENDGOODS_C2L2C,			//赠送商品

	CMD_SENDTOME_L2C,					// 赠送商品通知

	CMD_CREATE_PRACTICEROOM_C2L2C,	// 创建练歌房
	CMD_CREATE_PRACTICEROOM_L2C,	// 创建房间广播
	CMD_DELETE_PRACTICEROOM_L2C,		// 销毁房间广播

	CMD_GETPRACTICEROOMLIST_C2L2C,	// 取练歌房间列表信息
	CMD_GETPRACTICEROOMSimple_C2L2C,// 取练歌房间简易信息

	CMD_EXPLEVELUP_L2C,			//升级通知
	CMD_VIPLEVEL_L2C,			//vip等级变更 
	CMD_EXPCHG_L2C,				//经验变更通知 

	//使用商品
	CMD_USEGOODS_C2L2C,//
	//取得大厅礼物排行榜信息
	CMD_GIFT_TOPLIST_C2L2C,
	//取用户当天的总计在线时间
	CMD_GETONLINETIME_C2L2C = 75,
	//幸运礼物
	CMD_LUCK_GIFT_L2C = 76,
	//添加好友申请服务器转发成功通知
	CMD_APPLY_ADDFRIEND_L2C = 77,

	//取得新手的任务信息
	CMD_GET_NEWUSER_TASK_C2L2C,
	//任务完成通知
	CMD_TASK_STEP_C2L2C,
	//任务完成后,领取奖励物品
	CMD_TASK_GIFT_C2L2C,
	//获取滚屏礼物信息
	CMD_GET_GIFTINFO_C2L2C,
	//取练歌房信息
	CMD_GETPRACTICEROOMINFO_C2L2C,
	//系统喇叭包体
	CMD_SYSTEMSPEAK_L2C,
	//获取系统文字公告
	CMD_GET_SYSTME_SPEAKER_C2L2C,
	//版本错误协议
	CMD_VERSION_ERROR_L2C,
	//服务器请求背包更新
	CMD_UPDATE_PACK_L2C,
	//获取印章信息,87
	CMD_GET_SEELINFO_C2L2C,
	//删除印章信息,88
	CMD_REMOVE_SEELINFO_L2C,
	//发送用户系统数据,89
	CMD_SYSINFO_C2L,//
	//通知客户端,房间组信息已更新
	CMD_UPDATE_GROUPVERSION_L2C,
	//获取组信息
	CMD_GET_GROUPINFO_C2L2C,
	//获取组里面的房间列表
	CMD_GET_ROOMLIST_IN_GROUP_C2L2C,
	//获取歌曲列表
	CMD_GET_MUSICEXINFO_C2L2C,
	//获取av server列表
	CMD_GETAVSERVERINFO_C2L2C,//
	//服务器主动踢出用户
	CMD_KICK_USER_L2C,
	//离线消息
	CMD_GET_OFFLINEMSG_C2L2C,
	//分组置顶信息改变
	CMD_UPDATE_ROOMSORT_L2C,
	//分组置顶信息
	CMD_UPDATE_ROOMSORTINFO_C2L2C,
	//聊天管理,发送私聊信息
	CMD_USERCHATACK_L2C,
	//防沉迷时间到,服务器马上断开连接
	CMD_FATIGUEUSER_L2C,
	//获取用户小喇叭信息
	CMD_GETUSERSPEAKINFO_C2L2C,
	//歌手等级通知
	CMD_SINGERLEVEL_L2C,
	//财富等级通知
	CMD_WEALTHLEVEL_L2C,
	CMD_GET_FAMILYINFO_ALL_C2L, //获取所有家族信息,不在该协议返回信息
	CMD_FAMILYINFO_BYID_L2C, //家族信息推送给客户端，所有家族信息都有该协议推送
	CMD_UPDATE_AVCONFIG_L2C, //协议版本协议推送

	//////////////////////////////////////////////////////////////////////////
	CMD_MALLINFO_L2C = 1000,		// 从WEB过来的邮件信息
	CMD_GETALLLOBBYCURNUM_C2L2C,	// 得到所有大厅在线人数
	CMD_CREATE_ROOM_L2C,			// 创建表演房广播
	CMD_DELETE_ROOM_L2C,			// 删除表演房广播
	CMD_USER_RIGHT_L2C,				// 房间用户权限通知
	CMD_WEBCHGUSERINFO_L2C,			// 用户信息的更新
	CMD_UPDATE_ROOM_L2C,			// 更新房间通知 
	CMD_UPDATE_CITYGOODS_L2C,		// 更新商城通知 
	CMD_MOVE_ROOMSRV_L2C,			// 房间迁移
	CMD_ENTHRALLMENT_STATE_L2C,		// 用户防沉迷状态
	CMD_ROOM_LOGO_L2C,				// 房间的图片地址
	CMD_COLLECT_DATA_L2C,			// 收集用户数据
	CMD_UPDATE_AVSRV_L2C,			// 应视频服务器更改通知
	//无意义
	CMD_MAX_LOBBY_OLD,
	
	CMD_CHECK_VERSIONINFO_C2L2C = 10000,	// 校验版本信息
	CMD_PUSH_USERINFO_L2C,					// 服务器反推用户信息
	CMD_PUSH_ROOMINFO_L2C,					// 服务器反推房间信息
	CMD_PUSH_PACKETINFO_L2C,				// 服务器反推背包信息
	CMD_PUSH_ROOMGROUPINFO_L2C,				// 服务器反推房间组信息

	//手机助手2.0 add by caoxinping 2014/6/17 begin
	CMD_GETUSERLIST_L2C,		//= 10005//服务器发送手机用户列表
	CMD_MPUBLICCHAT_C2L2C,			//手机主页公聊
	CMD_MPRIVATECHAT_C2L2C,			//互动私聊
	CMD_MSEND_GIFT_C2L2C,				//送礼物，(大厅送礼物)
	CMD_UPDATEMONEY_L2C,      //更新钱币通知
	CMD_UPDATE_RECVCOUNT_L2C,  //更新用户接收礼物数
	CMD_SB_SEND_GIFT_L2C,   //送礼广播
	CMD_SB_GIFT_VALID_L2C,    //印章有效通知
	CMD_SB_GIFT_INVALID_L2C,    //印章无效通知
	CMD_MGET_ROOMLIST_IN_GROUP_C2L2C,  //手机获取组里面的房间列表

	//add by caoxinping 2014/9/9  begin
	CMD_GETUSERHIDE_C2L2C,  //获取好友在线状态
	CMD_CHANGEHIDE_C2L2C,    //修改在线状态及回应
	CMD_CHANGEDEFAULTHIDE_C2L2C,  //修改默认在线状态
	CMD_SB_CHANGEHIDE,      //修改在线状态广播

	CMD_GETUSERINFOFORSPEAKER_C2L2C,//获取发送喇叭用户的信息
	CMD_GETUSERINFOWITHVERSION_C2L2C,//获取用户信息夹带版本ID
	//add by caoxinping 2014/11/17 begin
	CMD_SEABETBROAD_L2C,    //深海捕鱼喇叭
	CMD_SB_UPDATEGIFT_L2C,    //礼物信息更新通知
	CMD_MONTHCHARGR_L2C,     //当月充值金币
	CMD_KTVMESSAGE_L2C,     //手机助手消息推送
	//end

	CMD_CHANGEHEADPHOTO_L2C = 30019,  //修改ktv用户头像
	CMD_CHANGENICKNAME_L2C,    //修改用户昵称
	//end
	//无意义
	CMD_MAX_LOBBY_NEW
};

enum CACHECMD
{
	CMD_WRITECACHE = 'w',
	CMD_READCACHE = 'r',        
	CMD_DELCACHE = 'd',
};

enum LOBBYCENTRECMD
{
	CMD_LOBBYLOGIN_L2LC = 2000,	//登陆,告诉大厅中心大厅id
	CMD_KEEPALIVE_L2LC,		//存活包
	CMD_USERLOGIN_L2LC2L,		//用户登陆大厅,上线通知
	CMD_USERLOGOUT_L2LC2L,		//用户离开大厅
	CMD_GETFRIENDLISTSTATUS_L2LC2L,	//取好友列表和好友状态
	CMD_GETBLACKLIST_L2LC2L,	//取黑名单列表
	CMD_GETCLUBLISTSTATUS_L2LC2L,	//取俱乐部成员和俱乐部成员状态
	CMD_ENTERROOM_L2LC2L,		//用户进入房间
	CMD_LEAVEROOM_L2LC2L,		//用户离开房间
	CMD_USERCHAT_L2LC2L,		//大厅间的用户私聊
	//CMD_FRIENDCHAT_L2LC2L,	//好友聊天
	CMD_CLUBCHAT_L2LC2L,		//俱乐部聊天
	CMD_ALLCHAT_L2LC2L,		//区内群发
	CMD_ADDFRIEND_L2LC2L,		//加好友通知更新cache
	CMD_ADDBLACK_L2LC,
	CMD_DELFRIEND_L2LC,		//删除好友通知更新cache
	CMD_DELBLACK_L2LC,
	CMD_ADDCLUB_L2LC2L,		//加入俱乐部通知更新cache
	CMD_EXITCLUB_L2LC,		//退出俱乐部通知更新cache
	CMD_DELCLUBCACHE_L2LC,		//俱乐部解散通知更新cache
	CMD_DELCLUBINFO_LC2L,    // 大厅中心通知大厅更新用户俱乐部信息
	CMD_KICKUSER_LC2L,		//用户在其他大厅

	//add by zhang 2009.12.16
	CMD_BROAD_ALL_L2LC2L,         //大喇叭
	CMD_BROAD_LOBBY_L2LC2L,   //小喇叭	
	//end
	//add by jinguanfu 2009/12/16 begin
	CMD_PRESENTBROADCAST_L2LC2L,	//礼物广播， //暂不支持跨大厅
	//CMD_GETLOBBYNAME_L2LC2L,		//取得大厅名
	//add by jinguanfu 2009/12/16 end

	// add by wangpf 2009/12/22
	CMD_CREATECLUB_L2LC, //创建俱乐部，更新cache
	// end

	//add by zhang 2009.12.31
	CMD_GETFRIENDLOCATION_L2LC2L, //取好友位置
	CMD_GETPARTINFO_L2LC2L,    //取角色信息
	CMD_GETROOMLIST_L2LC2L,   //取房间列表
	//end

	// add by wangpf 2010/1/4
	CMD_GETPARTINFO_LC2L2LC, // 把取角色信息的任务发到指定的大厅，由大厅把取得的信息返回给大厅中心
	CMD_GETROOMLIST_LC2L2LC, // 把取房间列表的任务发到指定的大厅，由大厅把取得的信息返回给大厅中心
	// end
	//add byzhang 2010.02.05
	CMD_ADDFRIEND_NEW_L2LC2L, //添加好友
	CMD_ADDFRIEND_NEW_LC2L2LC,
	//end

	//新俱乐部命令
	CMD_APPLYCLUB_L2LC, //大厅向大厅中心发送俱乐部申请批准消息(通过或不通过)
	CMD_DENY_ADDCLUB_LC2L,		//拒绝加入俱乐部通知
	CMD_CLUBMSG_LC2L,         //俱乐部消息通知
	CMD_CLUBTALK_LC2L,       //俱乐部聊天
	CMD_SELFQUITCLUB_L2LC, //大厅向大厅中心发送会员退出俱乐部
	CMD_CLUBDEL_L2LC,

	// add by wangpf 2010/04/12
	CMD_MALLINFO_LC2L, //商城消息转发
	CMD_GETALLLOBBYCURNUM_L2LC2L,
	// end
	// add by wangpf 2010/07/16
	CMD_EXPLEVEL_P2LC2L,  // python过来的经验等级通知
	CMD_EXPLEVEL_L2LC2L,  // 大厅过来转发的经验等级通知
	CMD_CHECK_OVERDUE_P2LC, // 取vip和道具的过期信息
	CMD_GET_OVERDUE_INFO_P2LC, // python 定时通知取过期信息
	CMD_OVERDUE_ITEM_LC2L,  // 通知过期的道具
	CMD_OVERDUE_CROWN_LC2L,	// 通知过期的皇冠
	CMD_WRITE_LOGSRV_LC2LS,  // 写日志到logserver
	// end
	CMD_SENDGOODS_L2LC2L,	 //赠送商品
	CMD_VIP_L2LC2L, //用户成为vip
	CMD_USER_RIGHT_LC2L, //用户权限更改通知
	CMD_LOBBYGIFT_TOPLIST_P2LC2L, //定时取大厅排行榜通知
	CMD_APPLY_ADDFRIEND_LC2L,     //添加好友申请成功通知
	CMD_GET_ROOMUSERLOGIN_STATUS_L2LC2L, //得到房间用户的登录状态
	CMD_FATIGUE_USERLIST_P2LC2L,//防沉迷用户下线

	// add by wangpf 2010/8/13, web protocal
	CMD_CREATEROOM_W2LC2L = 6000,	//创建房间WEB->LC->L
	CMD_DELETEROOM_W2LC2L,	//删除房间WEB->LC->L
	CMD_LOCKROOM_W2LC2L,	//冻结房间WEB->LC->L
	CMD_UNLOCKROOM_W2LC2L,	//解冻房间WEB->LC->L
	CMD_UPDATEROOM_W2LC2L, //更新房间信息WEB->LC->L
	CMD_ROOMMANAGERCHG_W2LC2L, //房间管理员更改WEB->LC->L
	CMD_USER_MONEY_W2LC2L, //币的更改通知
	CMD_USER_IDENTITY_W2LC2L, //身份的更改通知
	//	CMD_USERINFOCHG_W2LC2L, //用户基本信息更改WEB->LC->L
	CMD_SYSTEMSPEAKER_W2LC2L, //系统喇叭，全区
	CMD_SYSTEMMSG_W2LC2L, //系统信息
	CMD_LOBBYGIFT_TOPLIST_W2LC2L,//web大厅排行榜通知
	CMD_UPDATE_ROOMRIGHT_W2LC2L,//房间权限更改通知
	CMD_UPDATE_GIFT_PRIZE_CHANCE_W2LC2L, //礼物中奖的概率更改
	CMD_UPDATE_GIFT_INFO_W2LC2L, //礼物信息的更改
	CMD_UPDATE_CITY_GOODS_W2LC2L, //商城信息的更改
	CMD_MOVE_ROOM_W2LC2L, //房间的移动通知
	CMD_UPDATE_AV_W2LC2L, //中转服务器更改通知
	CMD_ROBOT_INFO_W2LC2L, //机器人信息
	CMD_UPDATE_PRATICE_ROOM_AV_W2LC2L, //更新练歌房的av信息
	CMD_UPDATE_GM_W2LC2L, //更新GM信息
	CMD_UPDATE_CLIENT_VERSIONID_W2LC2L, //更新客户端版本号
	CMD_UPDATE_PACK_W2LC2L, //领取奖品通知
	CMD_ENTHRALLMENT_STATE_W2LC2L, //沉迷状态
	CMD_ROOM_LOGO_W2LC2L, //房间图片
	CMD_ROOMGROUP_CHANGE_W2LC2L, //房间组的修改
	CMD_COLLECT_DATA_W2LC2L, //收集用户数据
	CMD_GETUSERPOSITION_W2LC2W, //获取用户的位置
	CMD_KICK_USER_W2LC2L, //后台直接踢出用户
	CMD_UPDATE_ROOMCHAT_BLACKLIST_W2LC2L, //更改房间公聊的黑名单列表
	CMD_UPDATE_ROOMGIFT_REFRESH_TIMES_W2LC2L, //更新房间礼物刷新频率
	CMD_UPDATE_ROOMSORT_W2LC2L, //更新分组排序
	CMD_UPDATE_FAMILY_SYSTEM_W2LC2L, //更新家族系统
	CMD_UPDATE_AVCONFIG_W2LC2L, //更新av配置信息
	CMD_CHARGE_W2LC2L,   //充值通知
	CMD_EXCHANGEGIFT_W2LC2L,   //礼物兑换
	CMD_UPDATEMONEY_W2LC2L,    //金币更新
	CMD_KTVMESSAGE_W2LC2L,     //手机助手消息推送
	//
	CMD_UPDATE_MUSICEX_INFO_P2LC2L=7000, //获取歌曲扩展信息
	// end
};

enum LOBBYCENTERINNERCMD
{
	CMD_SYNCUSERPOS = 1,
	CMD_DELUSERPOS,
	CMD_SYNCALLUSERPOSREQUEST,
	CMD_SYNCALLUSERPOSRESPONSE,

	// add by wangpf 2010/02/05, 处理添加朋友请求的同步
	CMD_SYNPUTFRIENDREQUEST,
	CMD_SYNDELFRIENDREQUEST,
	CMD_SYNCHDFRIENDREQUEST,
	// end
};

//add by zhang 2010.01.12
enum  CASSETTESERVERCMD
{
	CMD_GETROOMLIST_G2R2G = 1,
};
//end 2010.01.12

//add by zhang 2009.12.16
enum LOBBYERRCODE
{
	LOBBY_ERROR_CREATE_CLUB = 3001,  //创建club错误
	LOBBY_ERROR_DISMISS_CLUB ,   //解散club错误

	LOBBY_ERROR_APPLYADDCLUBOK,  //同意加入俱乐部错误
	LOBBY_ERROR_GETAPPLYLIST,   
	LOBBY_ERROR_GETAPPLYLISTCOUNT, //获取申请数失败
	LOBBY_ERROR_EXITCLUB,  //退出club失败

	LOBBY_ERROR_APPLYADDCLUB, 
	LOBBY_ERROR_GETSECTIONLIST,

	LOBBY_ERROR_TOKENLOGIN,//登录返回失败 ---chg
	LOBBY_ERROR_GETUSERINFO,   //获取某个用户信息失败
	LOBBY_ERROR_CHANGENICKNAME,  //更改用户昵称失败  ---chg
	LOBBY_ERROR_GETCLUBINFO,  //获取club信息失败 ---返回协议修改

	LOBBY_ERROR_ADDFRIEND,    //添加好友失败
	LOBBY_ERROR_ADDBLACK,     //添加黑名单失败
	LOBBY_ERROR_DELFRIEND,    //删好友失败
	LOBBY_ERROR_DELBLACK,     //删黑名单失败

	LOBBY_ERROR_BROAD_ALL_MSG, //发大喇叭消息失败
	LOBBY_ERROR_BROAD_LOBBY_MSG, //发小喇叭消息失败
	LOBBY_ERROR_GETLOBBYNAME, 
};
//end
#define LOBBYFIRSTKEY "9158xingguang"
#define ROOMFIRSTKEY "9158xingguang10"

#define RELATION_FRIEND 1
#define RELATION_CLUB   2

//#define FRIEND 1
//#define CLUB 2
#define COUNTPERPAGE 50

// add by wangpf 2010/07/30
enum state_relay
{
	CMD_GETUSERINFO_R2S2R,
	CMD_GETCLUBLIST_R2S2R,
	CMD_SPEAK_ALL_R2S2R,
	CMD_SPERK_LOBBY_R2S2R,
};
// end



//
//// 日志服务器的协议类型，主要是用来统一日志数据，根据类型来区分不同的统计信息
//enum LOGSRV_INFOTYPE
//{
//	LS_TYPE_LOGIN, // 登入写日志,也用在用户IP记录上
//	LS_TYPE_LOGOUT, // 登出写日志
//	LS_TYPE_ONLINE_REPORT, // 在线报告
//	LS_TYPE_MUSIC_CLICK_ROOM, // 房间
//	LS_TYPE_MUSIC_CLICK_SMALLROOM, // 练歌房
//	LS_TYPE_MIC_KEEPALIVE, //表演麦心跳
//	LS_TYPE_VJMIC_KEEPALIVE, //主持麦心跳
//	LS_TYPE_ONMIC, //上表演麦
//	LS_TYPE_OFFMIC, //下表演麦
//	LS_TYPE_ONVJMIC, //上主持麦
//	LS_TYPE_OFFVJMIC, //下主持麦
//	LS_TYPE_SYSINFO, //系统信息
//	LS_TYPE_LOBBYERROR, //大厅错误信息
//	LS_TYPE_ROOMERROR, //房间错误信息
//	LS_TYPE_USER_REALTIMEINFO, //用户实时信息
//};
//
////用户在麦状态
//enum USER_STATE
//{
//	USER_STATE_NONE		=0,		//未在麦上
//	USER_STATE_WAITMIC	=1,		//麦序中
//	USER_STATE_ONMIC	=2,		//表演麦上
//	USER_STATE_ONVJMIC	=3,		//主持麦上
//};

enum MONITOR
{
	CMD_GETLOBBYONLINE_NUM_P2L2P=1000,
	CMD_GETROOMONLINE_NUM_P2L2P, //获取每个房间的在线人数
};

enum LOTTE
{
	CMD_LOTTE_INFO_CHG_L2L = 3001,
};
//*******************************Lobby 服务器协议 END**************************

//*******************************Lobby与Room 服务器交互协议 BEGIN**************************
enum HALL_ROOM_CMD
{
	CMD_LEAVEROOM_R2L = 1,
	CMD_ENTERROOM_R2L,
	//CMD_ROOMCHANGE_R2L,
	CMD_ADDROOM_L2R,
	CMD_DELROOM_L2R,
	CMD_LOCKROOM_L2R,
	CMD_GETUSERINFO_R2L2R, //取得用户信息
	CMD_UPDATEROOM_L2R, //房间信息更新
	CMD_ROOMMANAGERCHG_L2R, //房间管理员更新

	CMD_ROOM_LOGIN = 0x100, //房间报告自身信息
	CMD_SYNC_ROOMINFO_L2R2L = 0x101,//大厅同步房间信息
	HALL_CMD_RETURN_SVERSERID =0x200,

	//add by zhang 2009.12.16
	CMD_FLOWER_ALL_R2L      = 300,//跑马灯
	CMD_FLOWER_LOBBY_R2L  = 301, //暂不用
	//end

	//add by jinguanfu 2010/4/8 房间名与密码标志和大厅同步
	//新俱乐部命令
	ROOM_CMD_APPLYCLUB_R2L = 304, //房间向大厅发送俱乐部申请批准消息(通过或不通过)
	//HALL_CMD_KICKUSER_L2R = 305,			//大厅转发踢出用户消息
	HALL_CMD_DELCLUBMEMBER_R2L = 306, //房间向大厅通知大厅,某个会员被管理员删除了
	ROOM_CMD_SELFQUITCLUB_R2L = 307, //房间向大厅发送俱乐部会员退出俱乐部

	CMD_LOGIN_AGAIN_L2R = 400, //重复登录，通知房间服务器 踢出重复用户
	ROOM_CMD_CHANGENAME_R2L = 401,		//通知大厅房间名更改
	ROOM_CMD_CHANGEPWD_R2L = 402,		//通知大厅房间密码更改
	ROOM_CMD_CHANGECLOSEFLAG_R2L = 403,		//通知大厅房间开启/关闭状态改变
	ROOM_CMD_CHANGEUSERONLYFLAG_R2L = 404,	//通知大厅房间私有状态改变
	ROOM_CMD_UPDATEUSERGOLD_L2R2L = 405,      //更新用户金币，银币
	ROOM_CMD_CHANGEAVATAR_L2R = 406,        //更改avarta
	ROOM_CMD_CHANGEPOWER = 407,             //更改权限
	ROOM_CMD_USERLOGOUT = 408,              //用户离开大厅，通知房间断开连接

	//add by jinguanfu 2010/7/14
	HALL_CMD_CHANGELEVEL=409,			//用户经验升级
	CMD_CREATE_PRACTICEROOM_L2R2L = 410,    
	CMD_DELETEROOM_R2L = 411,

	HALL_CMD_OVERDUE_ITEM_L2R = 412,    // 通知过期的道具
	HALL_CMD_OVERDUE_VIP_L2R = 413,	// 通知过期的VIP
	HALL_CMD_EXP_CHG_R2L = 414, 		//经验改变通知
	HALL_CMD_LUCK_R2L = 415,			//幸运中奖通知
	//add by jinguanfu 2010/11/17
	HALL_CMD_RIGHT_CHANGE_L2R = 416,	//权限配置更改
	HALL_CMD_LUCKCONF_CHANGE_L2R = 417,	//幸运抽奖概率更改
	HALL_CMD_GIFTINFO_CHANGE_L2R = 418,	//礼物信息更改
	HALL_CMD_MOVE_ROOM_L2R = 419,		//房间移动 
	//add by jinguanfu 2010/11/27
	HALL_CMD_UPDATE_AVSERVER_L2R=420,	//视音频服务器更新
	HALL_CMD_VUSER_LOGIN_L2R = 421,		//后台登陆虚拟用户
	HALL_CMD_VUSER_LOGOUT_L2R=422,		//后台退出虚拟用户
	CMD_UPDATE_PRATICE_ROOM_AV_L2R = 423, //练歌房的av更新
	CMD_UPDATE_GM_L2R = 424, //houtai更新GM
	CMD_ADD_SEEL_R2L = 425, //增加印章
	CMD_REMOVE_SEEL_L2R = 426, //删除印章
	CMD_USERLOGIN_STATUS_R2L2R = 427, //判断用户的登录状态
	//add by jinguanfu 2011/3/11
	HALL_CMD_FIREWORKS_R2L = 428,	//向大厅发送送烟花通知
	//add by jinguanfu 2011/3/22
	HALL_CMD_CHANGELOGO_R2L = 429,//向大厅发送更改logo通知
	//add by jinguanfu 2011/5/31
	CMD_ROOMBLACKMEMCHG_L2R = 430, //房间黑名单操作

	//add by jinguanfu 2011/8/19

	CMD_UPDATE_ROOMCHAT_BLACKLIST_L2R = 431, //更新房间聊天的黑名单列表
	HALL_CMD_KICKUSER_R2L = 432,		//房间通知大厅踢出用户
	//add by lihongwu 2011/9/20
	CMD_UPDATE_ROOMGIFT_REFRESH_TIMES_L2R = 433,//更新礼物刷新频率

	//add by jinguanfu 2012/5/28
	HALL_CMD_UPDATE_RICH_INFO =434,		//向大厅通知财富信息
	HALL_CMD_UPDATE_SINGER_INFO =435,		//向大厅通知歌手信息

	//add by jinguanfu 2012/11/7
	HALL_CMD_UPDATE_FAMILYINFO = 436,		//大厅通知家族信息更新
	//星光2.0 add by jinguanfu 2013/9/25
	HALL_CMD_UPDATE_MUSICINFO_R2L = 437,		//更新演唱歌曲数、歌曲最高分及拿手歌曲
	HALL_CMD_UPDATE_RECVGIFTCOUNT_R2L=438,	//更新礼物接收数
};
//*******************************Lobby与Room 服务器交互协议 END**************************

//*******************************Room 服务器协议 BEGIN**************************
enum LOGIN_RET//room
{
	LOGIN_NONE,  //未登录
	LOGIN_SUCESS,  //登陆成功
	LOGIN_AGAIN,  //重复登陆
	LOGIN_NOUIN,  //非法用户	
	LOGIN_FULL,  //房间人数满
	LOGIN_IFULL,  //房间已经达到允许人数,用户身份无法登陆
	LOGIN_EPWD,  //房间密码错误
	LOGIN_CLOSE,  //房间关闭
	LOGIN_INBLACK,  //在黑名单中
	LOGIN_INTBLACK,  //在临时黑名单中　
	LOGIN_NOTEXIST, //房间不存在或被删除 LOGIN_NOROOM
	LOGIN_LOCKED, //房间冻结
	LOGIN_IP_INVALIED,	//该IP在房间内被禁
};

enum ROOM_STATE
{
	ROOM_STATE_OPEN     = 0,  //房间开发状态
	ROOM_STATE_CLOSE    = 1,  //房间关闭状态
};

enum ROOM_SERVER_CMD  //cmd中有SB为广播消息
{
	ROOM_CMD_TOKEN = 1001,                  //token验证
	ROOM_CMD_LOGIN,                         //登陆
	ROOM_CMD_LEAVEROOM,						//离开房间（切换房间）
	ROOM_CMD_KEEPALIVE,                     //心跳包
	ROOM_CMD_SB_ENTERROOM,                  //某人人进入房间
	ROOM_CMD_SB_LEAVEROOM,                  //某人离开房间
	ROOM_CMD_GET_ALLINFO,                   //得到用户列表 
	ROOM_CMD_WAITMIC,                       //用户请求排麦
	ROOM_CMD_CANCLE_WAITMIC,                //用户请求取消排麦                  
	ROOM_CMD_SB_WAITMIC,                    //某人排麦成功
	ROOM_CMD_SB_CANCLE_WAITMIC,             //某人取消排麦成功
	ROOM_CMD_WAITMIC_PK,                    //用户请求pk
	ROOM_CMD_CANCLE_WAITMIC_PK,             //用户请求取消pk
	ROOM_CMD_SB_WAITMIC_PK,                 //某人加入pk成功 
	ROOM_CMD_SB_CANCLE_WAITMIC_PK,          //某人取消pk成功
	ROOM_CMD_UP_WAITMIC,                    //提升某人的麦序
	ROOM_CMD_SB_UP_WAITMIC,                 //提升某人的麦序成功
	ROOM_CMD_DOWN_WAITMIC,                  //下降某人的麦序
	ROOM_CMD_SB_DOWN_WAITMIC,               //下降某人的麦序成功
	ROOM_CMD_SB_ONMIC_READY,                //有人上麦准备 20 // deprecated
	ROOM_CMD_ONMIC_READYOK,                 //上麦准备OK  deprecated
	ROOM_CMD_SB_ONMIC,                      //有人上麦
	ROOM_CMD_OFFMIC_READY,                  //下麦准备  deprecated
	ROOM_CMD_SB_OFFMIC_READYOK,  //下麦准备OK, 客户端可以开始打分了  deprecated
	ROOM_CMD_SCORE,                         //打分
	ROOM_CMD_SB_OFFMIC,                     //有人下麦
	ROOM_CMD_KICKOUT_SOMEONE,               //踢出某人
	ROOM_CMD_SB_KICKOUT,                    //某人被踢出房间
	ROOM_CMD_FORBIDEN_SOMEONE,              //禁言某人
	ROOM_CMD_SB_FORBIDEN,                   //某人被禁言
	ROOM_CMD_UPDATE_BLACKLIST,              //更新房间的黑名单
	ROOM_CMD_SB_BLACKLIST_UPDATE,           //房间黑名单被更新了  31
	ROOM_CMD_UPDATE_MGRLIST,                //更新房间的管理员名单
	ROOM_CMD_SB_MGRLIST_UPDATE,             //房间名单管理员名单被更新了     
	ROOM_CMD_ROOM_MGR_SUCCESS,              //告诉相应的客户端房间管理操作成功
	ROOM_CMD_PRIVATE_CHAT,                  //私聊
	ROOM_CMD_PUBLIC_CHAT,                   //公聊 36
	ROOM_CMD_ONVJ_MIC,                      //上vj麦
	ROOM_CMD_SB_ONVJ_MIC,                   //有人上vj麦了
	ROOM_CMD_OFFVJ_MIC,                     //下vj麦
	ROOM_CMD_SB_OFFVJ_MIC,                  //有人下vj麦了    40
	ROOM_CMD_SB_SCORE,                         //打分结束
	ROOM_CMD_GIVE_VJ_MIC,                   //给与vj麦  
	ROOM_CMD_GIVEOFF_VJ_MIC,                //下vj麦
	ROOM_CMD_GIVEOFF_MIC,                   //下普通麦
	ROOM_CMD_SB_GIVEOFF_MIC,                 //有人被下普通麦了
	ROOM_CMD_UPDATE_CONTENT,                //更新房间的公告
	ROOM_CMD_SB_CONTENT_UPDATE,             //房间公告被更新了
	ROOM_CMD_SEND_GIFT,                   //送礼给好友    				C=>S			NULL			int s_idx,r_idx,cate_idx,number  47
	ROOM_CMD_SB_SEND_GIFT,                //送礼通知    				S=>C			NULL			int s_idx,r_idx,cate_idx,number
	//ROOM_CMD_BROAD_ALL,                     //大喇叭
	//ROOM_CMD_BROAD_LOBBY,                   //小喇叭
	ROOM_CMD_SEND_NOTICE_TEMP,               //发送房间公告    				C=>S			NULL			ResultData_SendFlowerInfo
	ROOM_CMD_SB_SEND_NOTICE_TEMP,            //收到房间公告    				S=>C			NULL			ResultData_SendFlowerInfo   50
	ROOM_CMD_INVITE_MIC,                    //邀请用户进入表演麦排序
	ROOM_CMD_SB_INVITE_MIC,                    //邀请用户进入表演麦排序
	//add by jinguanfu 2010/1/9 房间管理命令与房间服务器统一 <begin>
	ROOM_CMD_USER_APP_JOINROOM,             //用户申请加入房间
	//ROOM_CMD_SB_USER_APP_JOINROOM,         //用户申请加入房间
	ROOM_CMD_VERIFY_USER_APP,               //审核用户会员申请                 C=>S 
	ROOM_CMD_SB_VERIFY_USER_APP,            //审核用户会员申请通知             S=>C  
	ROOM_CMD_REFUSEMEMBER,					//接收拒绝加入房间消息
	ROOM_CMD_REMOVE_USER,                   //删除会员
	ROOM_CMD_SB_REMOVE_USER, 
	ROOM_CMD_GIVE_VJ_A,                      //设置为助理主持
	ROOM_CMD_SB_GIVE_VJ_A,                   //
	ROOM_CMD_GIVE_VJ,                        //设置为主持
	ROOM_CMD_SB_GIVE_VJ,
	ROOM_CMD_GIVE_OUER_S,                    //设置为副室主
	ROOM_CMD_SB_GIVE_OUER_S,
	ROOM_CMD_SET_ROOM_PWD,                   //设置密码
	ROOM_CMD_SB_SET_ROOM_PWD,
	ROOM_CMD_SET_ROOM_LOCK,                  //房间锁定
	ROOM_CMD_SB_SET_ROOM_LOCK,
	ROOM_CMD_SET_USER_ONLY,                 //设置房间只对会员开放
	ROOM_CMD_SB_SET_USER_ONLY,              // 
	ROOM_CMD_SET_USER_INOUT,                //设置房间用户进出信息显示
	ROOM_CMD_SB_SET_USER_INOUT,
	ROOM_CMD_SET_MIC_UPDOWN,                //设置自由排序
	ROOM_CMD_SB_SET_MIC_UPDOWN,
	ROOM_CMD_SET_ROOM_NAME,                 //设置房间名称
	ROOM_CMD_SB_SET_ROOM_NAME,
	ROOM_CMD_SET_CHAT_PUBLIC,              //设置房间是否公聊
	ROOM_CMD_SB_SET_CHAT_PUBLIC,
	ROOM_CMD_SET_ROOM_WELCOME,			   //设置房间欢迎词
	ROOM_CMD_SB_SET_ROOM_WELCOME,        //  80
	ROOM_CMD_SET_ROOM_LOGO,                //设置房间LOGO
	ROOM_CMD_SB_SET_ROOM_LOGO,
	ROOM_CMD_ROOMAPPLYLIST_C2R2C,          //获取房间申请列表
	ROOM_CMD_GETBLACKLIST,                 //获取房间黑名单列表
	ROOM_CMD_GETROOMMEMBERLIST,				//取得房间会员列表
	/*add by jinguanfu 2010/2/25 成就模块*/
	ROOM_CMD_SB_SONGLEVEL,			//演唱等级通知
	ROOM_CMD_SB_RICHLEVEL,			//财富等级通知
	//add by jinguanfu 2010/4/7
	ROOM_CMD_KICKUSER,				//用户被踢掉
	ROOM_CMD_EXITMEMBER,			//用户退出房间会员

	ROOM_CMD_GETPANELINFO,			//取得房间设置面板信息	
	//add by jinguanfu 2010/5/13
	ROOM_CMD_GIVE_MEMBER,			//由管理员降为一般会员
	ROOM_CMD_SB_GIVE_MEMBER,		//设为一般会员的通知
	
	//add by cx 10-5-20
	ROOM_CMD_SB_CHANGEAVATAR,		//avatar改变时房间内的广播
	//add by cx 10-6-2
	ROOM_CMD_REQUEST_GIVE_VJ_MIC,	//收到vj麦的邀请
	//升级通知
	ROOM_CMD_SB_LEVEL,//
	//VIP、皇冠改变通知
	ROOM_CMD_SB_VIP,
	//更新钱币通知
	ROOM_CMD_UPDATEMONEY,
	ROOM_CMD_SB_DELETEROOM,				//房间删除通知
	ROOM_CMD_SB_LOCKROOM,				//房间冻结通知
	ROOM_CMD_SB_ROOMINFO_UPDATE,		//房间信息更改通知
	ROOM_CMD_SB_GIVE_OWNER,				//房间室主更改通知

	ROOM_CMD_SEND_FIREWORKS,			//赠送烟花
	ROOM_CMD_SB_SEND_FIREWORKS,			//赠送烟花通知
	ROOM_CMD_RECV_FIREWORKS,			//接收烟花
	ROOM_CMD_SB_LUCKY,					//幸运礼物中奖通知
	ROOM_CMD_SB_GIFT_INVALID,			//礼物失效（印章）的协议
	ROOM_CMD_SB_GIFT_VALID,				//印章有效通知

	ROOM_CMD_VIEW_INCOME,				//显示房间可领取收益
	ROOM_CMD_VIEW_INCOME_LOG,			//显示收益已领取记录 
	ROOM_CMD_GET_INCOME,				//室主领取房间内收益

	ROOM_CMD_SB_GIFTINFO_CHANGE=1113,	//礼物信息更改通知
	ROOM_CMD_SB_RIGHTCONF_CHANGE,		//权限配置更新通知
	ROOM_CMD_SB_AVSERVER_CHANGE,//
	ROOM_CMD_SB_UPDATE_GM,
	ROOM_CMD_SB_EXITMEMBER,				//用户主动退出会员后通知
	ROOM_CMD_UP_NETSTATUS,				//用户上传网络状态信息
	ROOM_CMD_SB_NETSTATUS,				//向房间内用户广播网络状态信息

	ROOM_CMD_INVITE_MEMBER,				//邀请会员C->S
	ROOM_CMD_INVITE_NOTIFY,				//邀请通知S->C
	ROOM_CMD_INVITE_REPLY,				//邀请回复C->S

	ROOM_CMD_SEND_STAR,

	ROOM_CMD_GET_GIFTSEND,			//取得当日礼物赠送记录
	ROOM_CMD_GET_GIFTRECV,				//取得当日礼物接收记录

	ROOM_CMD_SET_AUTOONMIC,				//设置房间暂时上麦
	ROOM_CMD_SB_SET_AUTOONMIC,			//暂停上麦状态通知

	ROOM_CMD_DISABLE_IPADDR,			//禁用对方的IP
	ROOM_CMD_DISABLE_MACADDR,			//禁用对方的MAC,对IOS就是唯一标示码

	ROOM_CMD_ADD_TM,				//增加在麦时间
	ROOM_CMD_SB_ADDTM,	//增加在麦时间广播

	//only room server - begin
	ROOM_CMD_LASTERROR,				//DEBUG用返回权限错误
	//add by jinguanfu 2010/11/9
	ROOM_CMD_UPDATE_MUSICINFO,		//向数据库更新歌曲的点唱率
	ROOM_CMD_DB_LUCKCONF_CHANGE,		//中奖概率更改
	//only room server - end

	ROOM_CMD_UPDATE_MUSICTIME,		//设置在歌曲剩余时间
	ROOM_CMD_GET_WAITMICLIST,	//获取（排）麦序列表

	ROOM_CMD_ON_PRIVATE_MIC_C2S2C,        //上私麦
	ROOM_CMD_SB_ON_PRIVATE_MIC,           //有用户上私麦了，群发
	ROOM_CMD_OFF_PRIVATE_MIC_C2S2C,       //下私麦
    ROOM_CMD_SB_OFF_PRIVATE_MIC,          //有用户下私麦了，群发
	ROOM_CMD_GIVEOFF_PRIVATE_MIC_C2S2C,   //抱下私麦
	ROOM_CMD_SB_GIVEOFF_PRIVATE_MIC,      //有用户被抱下私麦了，群发
	ROOM_CMD_REQUEST_PRIVATE_MIC_C2S2C,   //客户端请求看某人的私麦
	ROOM_CMD_SEND_REQUEST_PRIVATE_MIC_S2C,//服务器发给客户端请求，有人想看他的私麦
	ROOM_CMD_REQUEST_ACK_PRIVATE_MIC_C2S2C, //客户端回应允许某人看自己的私麦
	ROOM_CMD_INVITE_PRIVATE_MIC_C2S,      //客户端邀请某人看自己的私麦
	ROOM_CMD_SEND_INVITE_PRIVATE_MIC_S2C,  //服务器发给客户端请求，有人邀请他看私麦
	ROOM_CMD_INVITE_ACK_PRIVATE_MIC_C2S,  //客户端回应某人邀请自己看他的私麦
	ROOM_CMD_SEND_INFO_PRIVATE_MIC_S2C,   //服务器把本房间的私麦信息发送给用户

	ROOM_CMD_UPDATE_PRIVATEMIC,				//设置是否开放私麦
	ROOM_CMD_SB_UPDATE_PRIVATEMIC,			//设置开放私麦后广播

	ROOM_CMD_CROWN_KICKOUT_SOMEONE,			//皇冠踢人
	ROOM_CMD_DISABLEIP_BYROOM,				//室主临时禁止用户ip

	ROOM_CMD_UPLOAD_SECTIONSCORE,//发送句评分
	ROOM_CMD_UPLOAD_RECORDDATE,	  //GM上传用户录音
	//ROOM_CMD_GET_ONMICUSER_PANEL_C2S2C,//获取用户的其他信息（头像，演唱次数，演唱最高分及该歌曲名等）

	ROOM_CMD_SB_UPDATE_RECVCOUNT,			//更新用户接收礼物数
	ROOM_CMD_SB_UPDATE_TOPSONG,				//更新用户最高分歌曲及K歌数量
    
    //by 2014-6-24
    //来自KTV服务器的通知，通知已经绑定包厢的手机用户当前的播放列表
    //@int listcount 歌曲数量
    //@while
    //  @string songname 歌曲名
    //  @string singername 歌手名
    ROOM_CMD_SB_KTV_CUR_SONGLIST,           
    //来自KTV服务器的通知，通知当前包厢的状态
    //@char state:0包厢开启，1包厢关闭，
    ROOM_CMD_SB_KTV_ROOM_STATE,
	//来自KTV服务器的通知，通知当前包厢用户演唱的歌曲已经上榜
	//@null
	ROOM_CMD_SB_KTV_SONGRANKING,
	ROOM_CMD_SB_KTV_SONGNORMALOVER,
	//无意义
	ROOM_CMD_MAX
};

enum MICTM_ADD_STATE
{
	ADDTM_SUCESS = 0,     //麦序时间延长成功
	ADDTM_FAILURE = 1,     //麦序时间延长失败
};

enum _TIMEOUT_TYPE
{
	TIMEOUT_TB = 100,              //临时黑名单
	TIMEOUT_SCORE,             //打分
	TIMEOUT_ONMIC,			//上麦
	TIMEOUT_FORBIDEN,		//禁言
	TIMEOUT_OFFMIC,		//下麦
	//add by jinguanfu 2011/8/10
	TIMEOUT_READY,			//上麦准备超时
};

enum USER_IDENTITY//room
{
	USER_ID_NONE      = 0,  //普通用户
	USER_ID_OWNER     = 1,  //室主
	USER_ID_A_MANAGER = 2,  //A管理员
	USER_ID_B_MANAGER = 3,  //B管理员
	USER_ID_VJ        = 4,  //主持人
	USER_ID_VJ_A      = 5, 	//助理主持人
	USER_ID_VIP       = 6,  	//房间会员
	USER_ID_GM        = 7,  	//游戏管理员
	USER_ID_OWNER_S   = 8,  //副室主
	USER_ID_GVIP=9,		//游戏VIP用户
	USER_ID_RED=10,		//红冠用户
	USER_ID_PURPLE =11,	//紫冠用户
	USER_ID_SUPER =12,		//超冠用户
	USER_ID_IMPERIAL =13,	//至尊用户

	USER_ID_GUEST = 14,	//游客

	USER_ID_FAMILYOWNER=15,  //家族长

	USER_ID_GREEN = 16,	//绿冠用户
};

enum MIC_UP_DOWN_FLAG//room
{
	MIC_UP_DOWN_ONE      = 0,  //升降一位
	MIC_UP_DOWN_ALL      = 1,  //升降到顶
};

enum WAITMIC_UP_DOWN_SEARCH_
{
	WAITMIC_UP_DOWN_IDX      = 0,  //idx搜索
	WAITMIC_UP_DOWN_PKIDX    = 1,  //pkidx搜索
};

enum USER_APP_REF
{
	SUCCESS      = 0, //成功
	ALREADY    	=-1,//已是本房间会员或其他房间会员
	LISTFULL 	=-2,	//会员列表或黑名单已满
	REFUSE		=-3,//服务器拒绝，申请被拒绝后24小时内不能再申请
	IAGAIN		=-4,//重复重申，等待管理员审核
	APPLYFULL	=-5,//房间申请列表已满
};

enum ROOM_TYPE
{
	ROOM_TYPE_ROOM=0,	//演唱房
	ROOM_TYPE_SMALL=1,	//练歌房
};

enum VIP_LEVEL				//vip等级
{
	VIP_LEVEL_NONE		=0,		//普通用户
	VIP_LEVEL_VIP		=1,		//VIP用户
	VIP_LEVEL_LIFEVIP	=2,		//终生VIP用户
	VIP_LEVEL_GUEST   =3,		//游客用户
	VIP_LEVEL_GREEN   =4,		//绿冠用户
	VIP_LEVEL_RED		=100,	//红冠用户
	VIP_LEVEL_PURPLE	=200,	//紫冠用户
	VIP_LEVEL_SUPER	=300,	//超冠用户
	VIP_LEVEL_IMPERIAL	=400,	//至尊用户
};

enum MONITOR_CMD
{
	MONITOR_CMD_REPORT=1,
	MONITOR_CMD_REPLY=4,		//房间服务器代号
};

enum DBRESULT_GIFT	//礼物、道具赠送
{
	DBRESULT_GIFT_NOMONEY	=-1,		//余额不足
	DBRESULT_GIFT_SUCCESS		=0,		//成功
	DBRESULT_GIFT_NULL		=1,		//交易金额为0
	DBRESULT_GIFT_NOGIFT		=2,		//无此交易类型
	DBRESULT_GIFT_OFFLINE		=3,		//礼物接收者不房间内
};

enum DBRESULT_MEMBER	//会员操作
{
	DBRESULT_MEMBER_FULL		=-1,		//人数已满
	DBRESULT_MEMBER_SUCCESS	=0,		//成功
};

enum DBRESULT_VERIFY		//审核申请
{

	DBRESULT_VERIFY_SUCCESS	=0,		//成功
	DBRESULT_VERIFY_ALREADY	=1,		//已是本房间或其他房间会员
	DBRESULT_VERIFY_FULL		=2,		//会员数已满
};

enum DBRESULT_APPLY		//申请会员
{
	DBRESULT_APPLY_FULL		=-1,		//人数已满
	DBRESULT_APPLY_SUCCESS	=0,		//成功
	DBRESULT_APPLY_NONE		=1,		//
	DBRESULT_APPLY_AGAGN		=2,		//重复申请
	DBRESULT_APPLY_REFUSE	=3,		//服务器拒绝(管理员拒绝24小时内不能再申请)
	DBRESULT_APPLY_ALREADY	=4,		//已是本房间或其他房间会员
};

enum DBRESULT_SCORE		//打分后加经验操作
{
	DBRESULT_SCORE_SUCCESS	=0,		//成功
	DBRESULT_SCORE_FALIED		=1,		//失败
};

enum GIFT_TYPE
{
	GIFT_TYPE_NONE		=1,		//一般礼物
	GIFT_TYPE_LUCK		=2,		//幸运礼物
	GIFT_TYPE_BFIRE	=3,		//大烟花
	GIFT_TYPE_SFIRE	=4,		//小烟花
	GIFT_TYPE_SEAL		=5,		//印章
};

enum DBRESULT_INCOME		//领取房间收益
{
	DBRESULT_INCOME_SUCCESS	=0,		//成功
	DBRESULT_INCOME_FALIED	=1,		//失败
};

enum DBRESULT_BLACKLIST	//操作房间黑名单
{
	DBRESULT_BLACKLIST_SUCCESS	=0,
	DBRESULT_BLACKLIST_FALIED	=1,	//已在黑名单中或已从黑名单里删除
};

//房间服务器内部用命令
enum INNER_CMD
{
	INNER_CMD_GETROOMINFO=2000,	//取得房间基本信息
	INNER_CMD_GETMEMBERLIST,			//取得房间会员列表
	INNER_CMD_GETBLACKLIST,		//取得房间黑名单
	INNER_CMD_GETAPPLYLIST,		//取得房间会员申请列表
	//add by jinguanfu 2011/5/31
	INNER_CMD_UPDATEROOMMANGER,		//更新房间管理员列表
	INNER_CMD_UPDATEROOMBLACKLIST,	//更新房间黑名单
};

//add by jinguanfu 2011/1/25
//用户在麦状态
enum USER_STATE
{
	USER_STATE_NONE		=0,		//未在麦上
	USER_STATE_WAITMIC	=1,		//麦序中
	USER_STATE_ONMIC		=2,		//表演麦上
	USER_STATE_ONVJMIC	=3,		//主持麦上
};

enum RIGHT_OPTION
{
	RIGHT_OPTION_CHKPWD			=1,		//验证进入房间密码
	RIGHT_OPTION_CHKCLOSE			=2,		//验证进入关闭房间
	RIGHT_OPTION_CHKPRIVATE		=3,		//验证进入会员房间
	RIGHT_OPTION_CHKFULL			=4,		//验证进入满员房间
	RIGHT_OPTION_ONVJMIC			=5,		//上主持麦
	RIGHT_OPTION_CHGWAITMIC		=6,		//调整麦序
	RIGHT_OPTION_FREEWAITMIC		=7,		//非自由排麦下排麦
	RIGHT_OPTION_FORBIDEN			=8,		//禁言
	RIGHT_OPTION_BLACK			=9,		//操作黑名单
	RIGHT_OPTION_KICKOUT			=10,		//踢用户出房间
	RIGHT_OPTION_GIVEVJMIC		=11,		//邀请主持麦
	RIGHT_OPTION_OFFVJMIC			=12,		//下别人主持麦
	RIGHT_OPTION_OFFMIC			=13,		//下别人表演麦
	RIGHT_OPTION_DELWAITMIC		=14,		//删除麦序
	RIGHT_OPTION_INVITEMIC		=15,		//邀请别人排麦
	RIGHT_OPTION_SENDCHAT			=16,		//发送房间公聊

	RIGHT_OPTION_GETAPPLYLIST	=17,		//取得会员审请列表
	RIGHT_OPTION_AUDITAPPLY		=18,		//审核会员申请
	RIGHT_OPTION_MEMBERLIST		=19,		//查看会员列表
	RIGHT_OPTION_DELMEMBER		=20,		//删除房间会员
	RIGHT_OPTION_GIVEMEMBER		=21,		//管理员降为会员
	RIGHT_OPTION_GIVEVJA			=22,		//设置为助理主持
	RIGHT_OPTION_GIVEVJ			=23,		//设置为主持
	RIGHT_OPTION_GIVESUBOWNER	=24,		//设置为副室主
	RIGHT_OPTION_GETBLACK			=25,		//取得黑名单
	RIGHT_OPTION_SETROOMPWD		=26,		//设置房间密码
	RIGHT_OPTION_SETROOMCLOSE	=27,		//关闭、开放房间
	RIGHT_OPTION_SETROOMPRIVATE	=28,		//设置会员房间
	RIGHT_OPTION_SETROOMINOUT	=29,		//设置房间进出信息
	RIGHT_OPTION_SETFREEMIC		=30,		//设置房间内自由上麦
	RIGHT_OPTION_SETROOMNAME		=31,		//设置房间名
	RIGHT_OPTION_SETROOMCONTENT	=32,		//设置房间公告
	RIGHT_OPTION_SENDCONTENT		=33,		//发送房间临时公告
	RIGHT_OPTION_SETROOMCHAT		=34,		//设置房间公聊
	RIGHT_OPTION_SETWELCOMEWORD	=35,		//设置房间欢迎词
	RIGHT_OPTION_SETROOMLOGO		=36,		//设置房间LOGO
	RIGHT_OPTION_SETAUTOONMIC	=37,		//设置房间自动 上麦
	//add by jinguanfu 2011/8/19
	RIGHT_OPTTON_DISABLEMACIP   =38,		//禁用IP或MAC
	//add by lihongwu 2011/9/30
	RIGHT_OPTION_ADDTM    =39,              //麦序时间延长
	//add by jinguanfu 2012/5/28
	RIGHT_OPTION_ONPRIVATEMIC = 40,		//上私麦
	RIGHT_OPTION_GIVEOFFPRIVATEMIC = 41,	//踢下私麦
	RIGHT_OPTION_REQUESTPRIVATEMIC= 42,	//请求看私麦
	RIGHT_OPTION_INVITEPRIVATEMIC= 43,	//邀请某人看私麦
	RIGHT_OPTION_SETROOMPRIMIC = 44,		//设置是否开放私麦
	//45，46，47三个权限为客户端专用权限
	//add by jinguanfu	2012/10/11
	RIGHT_OPTION_DISABLEIP = 48,			//室主禁用IP
	//add by jinguanfu 2013/4/10
	RIGHT_OPTION_UPLOADRECORD = 49,		//上传录音
};

//add by jinguanfu 2011/3/9
//星光工厂服务器
enum FACTORY_CMD
{
	FACTORY_CMD_LOGIN_R2F			=0,		//用户进入房间
	FACTORY_CMD_LOGOUT_R2F,				//用户退出房间
	FACTORY_CMD_KEEPALIVE_R2F,			//用户心跳
	FACTORY_CMD_STAR_NUM_F2R,		//生成星光道具通知
};

//add by jinguanfu 2011/3/22 
//邀请主持麦后回应
enum RESULT_GIVEVJMIC
{
	RESULT_GIVEVJMIC_ACCEPT	=0,	//被邀请者同意上麦
	RESULT_GIVEVJMIC_REFUSED,	//被邀请者拒绝上麦
	RESULT_GIVEVJMIC_ONVJMIC,	//有人在主持麦上
	RESULT_GIVEVJMIC_ONMIC,		//被邀请者在表演麦上
};

//add by jinguanfu 2011/8/11
//用户防沉迷状态
enum USER_STATUS
{
	USER_STATUS_NONE 	=0,		//未认证
	USER_STATUS_JUST 	=1,		//暂时非防沉迷
	USER_STATUS_BAD 	=2,		//防沉迷
	USER_STATUS_GOOD 	=3,		//非防沉迷

};

//add by jinguanfu 2011/9/27
//用户TOKEN认证返回值
enum TOKENLOGIN_RET
{
	TOKENLOGIN_SUCCESS =0,	//成功
	TOKENLOGIN_TIMEOUT =-1,	//失败，token超时
	TOKENLOGIN_INVALID =-2,	//失败，该用户未登陆大厅
};

//add by lihongwu 2011/10/25
//幸运礼物中奖广播方式
enum LUCKINFO_FLAG
{
	LUCK_BUGLE   =0,  //50倍以上喇叭广播
	LUCK_CONTENT =1,  //500倍喇叭和公告
};

//add by jinguanfu 2012/7/26
//抽奖服务器间协议
enum LOTTE_CMD
{
	LOTTE_CMD_RUNLOTTE=3000,
};
enum MIC_INFO_STATE//room
{
	MIC_INFO_WAIT = 0,              //排麦s
	MIC_INFO_ONMIC_READY,           //上麦开始准备
	MIC_INFO_ONMIC_READYOK,         //上麦准备完成
	MIC_INFO_ONMIC,                 //上麦中
	MIC_INFO_OFFMIC_READY,          //下麦开始准备
	MIC_INFO_OFFMIC_READYOK,        //下麦准备完成
	MIC_INFO_SCORE,                 //上传分数完成
	MIC_INFO_OFFMIC,			//下麦
};
//*******************************Room 服务器协议 END**************************

//*******************************XGClient DEFINE BEGIN**************************
enum ADD_SUB_FLAG//client
{
	SUB_FLAG			 = 0,  //删除
	ADD_FLAG			 = 1,  //增加
};

enum SPEAKER_TYPE
{
	SPEAKER_SYSTEM		= 0,	//系统喇叭
	SPEAKER_USER		= 1,	//用户喇叭
};

enum ERRORCODE_CLUB
{
	DISMISSCLUB_IDXNOTEXIST	=-2,		//删除者不存在
	CREATECLUB_IDXNOTEXIST	=-3,		//创建者不存在
	CREATECLUB_ADDERROR		=-2,		//俱乐部添加成员失败
	CREATECLUB_EXIST		=-1,		//创建的俱乐部已存在
	DISMISSCLUB_NOTEXIST	=0,			//俱乐部不存在
	EXITCLUB_NOTEXIST		=0,			//成员不在俱乐部内
};
enum ERRORCODE_LOGIN
{
	//OPTION_TIMEOUT	=-99, //操作超时
	XGC_LOGIN_NOTEXIST	=-2,  //用户名不存在 g
	XGC_LOGIN_ERRORPWD  =-1,  //登陆密码错误 g
};
//add by jinguanfu 2010/3/25 <begin>
enum ERRORCODE_CLUBAPPLY
{
	CLUBAPPLY_ALREADY				= -2,		//已经是
	CLUBAPPLY_FULL					= -1,		//申请列表已满
	CLUBAPPLY_DUP					= 0,		//重复申请
	CLUBAPPLY_OK					= 1,		//申请成功
};

enum ERRORCODE_DELCLUBAPPLY
{
	DELCLUBAPPLY_NOTEXIST			=-1,		//申请记录不存在
	DELCLUBAPPLY_NORIGHT			=-2,		//无操作权限
};

enum ERRORCODE_CLUBAPPLYOK
{
	CLUBAPPLYOK_NORIGHT				=-2,		//无操作权限
	CLUBAPPLYOK_FULL				=-1,		//俱乐部成员已满
};

enum ERRORCODE_UPDATEROOMMRG
{
	UPDATEROOMMRG_OK			=0,				//添加、更新、删除管理员成功
	UPDATEROOMMRG_EXIST			=1,				//添加管理员时已存在
};

enum ERRORCODE_ADDFRIENDBLACK
{
	ADDFRIBLK_ADDSELF			=-99,			//添加自己为好友
	ADDFRIBLK_ALREADY			=-98,			//已经添加为好友
	ADDFRIBLK_NOTEXIST			=-97,			//idx或friend_idx不存在
	ADDFRIBLK_MAXCOUNT			=-1,			//好友列表或黑名单已满
	ADDFRIBLK_MAXCOUNT2			=-2,			//对方好友列表已满
	XGC_STATUS_HAS_BLACKED			=7		//在对方黑名单中
};
enum ERRORCODE_LOBBYLOGIN
{
	LOBBYLOGIN_NORMAL			=0,				//正常
	LOBBYLOGIN_NOAVATAR			=1,				//没有创建AVATAR
};
//*******************************XGClient DEFINE END**************************
enum KTVLOBBY
{
	CMD_EXCHANGEGIFT_K2L = 30017,  //手机礼物兑换
	CMD_GETKTVUSERLIST_K2L,    //获取ktv用户列表
	CMD_CHANGEHEADPHOTO_K2L,  //修改ktv用户头像
};


#endif
