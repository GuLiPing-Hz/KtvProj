#ifndef _ResultData_h_
#define _ResultData_h_

#define MAXLENGTH_AVATAR 1024
#define MAX_AVARTA_VALUE 10
#define PRI_VEDIO_TYPE_UP_LEVEL 1	//超过vip即可
#define PRI_VEDIO_TYPE_ME_ALLOW 2   //需要我的同意

enum ERRORCODE
{
	AUTH_NORMAL=1000,				//正常
	AUTH_NOTEXIST,					//帐号不存在
	AUTH_PWDERROR,					//登陆密码错误
	AUTH_TOURISTERROR,				//游客登录失败
	AUTH_CACHEERROR,				//登录不成功
	AUTH_TOKENERROR,				//取得token失败
	AUTH_MAX,

	LOBBY_NORMAL=2000,				//正常
    LOBBY_NONEED_UPDATE,            //用户信息无需更新，可以从本地数据库获取
	LOBBY_LOGIN_IDLOCK,				//账号被冻结
	LOBBY_LOGIN_TOOOLD,				//版本错误
	LOBBY_LOGIN_NOTEXIST,			//用户不存在
	LOBBY_LOGIN_NOAVATAR,			//还未创建avatar信息
	LOBBY_USERINFO_NOTEXIST,		//用户信息不存在
	LOBBY_USERINFO_INDULGE,			//用户防沉迷
	/***添加/删除 好友、黑名单错误代码***/
	LOBBY_ADDFRIEND_NORMAL,			//正常
	LOBBY_ADDFRIEND_WAIT,			//已转发,等待对方回复。
	LOBBY_ADDFRIEND_INPROCESS,		//正处理状态
	LOBBY_ADDFRIEND_REPEAT,			//服务器拒绝状态(重复添加)
	LOBBY_ADDFRIEND_FRIEND,			//已是好友
	LOBBY_ADDFRIEND_REFUSE,			//用户拒绝
	LOBBY_ADDFRIEND_OFFLINE,		//用户不在线
	LOBBY_ADDFRIEND_DBERROR,		//数据库返回失败
	LOBBY_ADDFRIEND_ADDSELF,		//不能添加自己为好友
	LOBBY_ADDFRIEND_ALREADY,		//已经添加对方为好友
	LOBBY_ADDFRIEND_NOTEXIST,		//自己的idx或对方有idx不存在
	LOBBY_ADDFRIEND_MAXCOUNT,		//自身好友列表已满
	LOBBY_ADDFRIEND_MAXCOUNT2,		//对方好友列表已满
	LOBBY_ADDFRIEND_INBLACK,		//对方好友列表已满
	LOBBY_DELFRIEND_NOTEXIST,		//不是好友

	LOBBY_ADDBLACK_NORMAL,			//正常
	LOBBY_ADDBLACK_ADDSELF,			//不能添加自己为黑名单
	LOBBY_ADDBLACK_ALREADY,			//已经添加对方为黑名单
	LOBBY_ADDBLACK_NOTEXIST,		//自己的idx或对方有idx不存在
	LOBBY_ADDBLACK_MAXCOUNT,		//自身黑名单列表已满
	LOBBY_DELBLACK_NOTEXIST,		//不是黑名单
	///////////////////////////////////////////////////////////////
	LOBBY_CLUB_MAXCOUNT,			//俱乐部满员
	LOBBY_CREATECLUB_EXIST,			//被创建俱乐部已存在
	LOBBY_DIMICLUB_NOTEXIST,		//被解散的俱乐部不存在
	
	LOBBY_SPEAKER_BALANCE,			//发送喇叭余额不足

	LOBBY_CREATEAVARAT_ERROR,		//创建错误
	LOBBY_CHANGEAVARAT_NOFIND,		//背包中没有所要换的衣服
	LOBBY_CHANGENICKNAME_ERR,		//背包中没有所要换的衣服

	LOBBY_BUY_ERROR,				//购买商品出现错误
	LOBBY_BUY_NOMONEY,				//金币不足
	LOBBY_BUY_SEXERROR,				//性别不符
	LOBBY_BUY_ISVIP,				//已有终身卡 购买vip卡时
	LOBBY_BUY_NOSPACE,				//背包空间不足
	LOBBY_BUY_ISFULL,				//对方已拥有该永久商品
	LOBBY_GETPACK_ERROR,			//获取背包失败
	LOBBY_DELPACK_ERROR,			//删除背包失败
	LOBBY_USEPACK_ERROR,			//使用出现错误
	LOBBY_TASK_STEP_OVER,			//已完成新手导航
	LOBBY_TASK_STEP_ERRSTEP,		//非法的任务ID
	LOBBY_TASK_STEP_ERRPRIZE,		//奖励返值失败（数据库错误）
	LOBBY_KICK_BY_IDX,				//被禁idx
	LOBBY_KICK_BY_NET,				//被禁mac
	LOBBY_MAX,	

	ROOM_NORMAL			=3000,		//正常
	ROOM_TOKEN_ERROR,				//token过程异常
	ROOM_LOGIN_NONE,				//未登录
	ROOM_LOGIN_SUCCESS,				//登录成功
	ROOM_LOGIN_AGAIN,				//重复登录
	ROOM_LOGIN_NOUIN,				//非法用户
	ROOM_LOGIN_FULL,				//房间人数满
	ROOM_LOGIN_IFULL,				//房间已经达到允许人数,用户身份无法登陆
	ROOM_LOGIN_EPWD,				//房间密码错误
	ROOM_LOGIN_CLOSE,				//房间关闭
	ROOM_LOGIN_INBLACK,				//在黑名单中
	ROOM_LOGIN_INTBLACK,			//在临时黑名单中
	ROOM_LOGIN_NOTHIS,				//房间不存在或被删除
	ROOM_LOGIN_LOCK,				//房间冻结
	ROOM_LOGIN_IP_INVALIED,			//该IP在房间内被禁
	ROOM_ERROR_ROOMINFO,			//接收房间信息失败
	ROOM_ERROR_APPLYLIST,			//接收申请列表错误
	ROOM_ERROR_MEMLIST,				//接收房间会员错误
	ROOM_ERROR_UNCHAIN,				//解除房间会员失败
	ROOM_ERROR_PESIDEASSISTANT,		//设置助理主持失败
	ROOM_ERROR_PESIDE,				//设置主持人失败
	ROOM_ERROR_SUBHOST,				//设置副室主失败
	ROOM_ERROR_BLACKLIST,			//接收黑名单失败
	ROOM_ERROR_PASSWORD,			//设置密码失败
	ROOM_ERROR_INPUTFLAG,			//设置用户进出消息失败
	ROOM_ERROR_LOGO,				//设置房间信息失败
	ROOM_ERROR_ONMIC,				//设置自由上麦失败
	ROOM_ERROR_CHATFLAG,			//设置允许房间公聊失败
	ROOM_ERROR_WELCOME,				//设置房间欢迎词失败
	ROOM_ERROR_AFFICHE,				//设置房间公告失败
	ROOM_ERROR_ROOMSWITCH,			//设置房间开启/锁定失败
	ROOM_ERROR_ROOMNAME,			//设置房间名失败
	ROOM_ERROR_PARTINFO,			//接收角色信息失败
	ROOM_ERROR_ALREADYMEMBER,		//已是本房间会员或是其他房间会员
	ROOM_ERROR_MEMLISTFULL,			//房间会员已满
	ROOM_ERROR_ADDAGAIN,			//房间黑名单重复添加
	ROOM_ERROR_BLACKFULL,			//房间黑名单已满
	ROOM_ERROR_APPLYREFUSE,			//服务器拒绝会员申请(申请被管理员拒绝后,24小时内不能申请)
	ROOM_ERROR_NOTMEMBER,			//不是房间会员
	ROOM_ERROR_OWER2FULL,			//副室主人数超过2人
	ROOM_ERROR_VJFULL,				//主持人人数超过50人
	ROOM_ERROR_VJAFULL,				//助理主持人数超过50人
	ROOM_ERROR_APPLYAGAIN,			//重复申请,请等待管理员审核
	ROOM_ERROR_NOCHAT,				//房间不允许公聊
	ROOM_ERROR_FORBIDEN,			//被禁言
	ROOM_ERROR_APPLYFULL,			//房间申请列表已满
	ROOM_ERROR_GIFT_BALANCE,		//送礼时余额不足
	ROOM_ERROR_GIFT_UNKOWN,			//交易金额为空
	ROOM_ERROR_GIFT_NOTEXIST,		//该礼物不存在
	ROOM_ERROR_GIFT_OFFLINE,		//礼物接收者不在房间内
	ROOM_ERROR_MEMBER,				//由管理员设为一般会员失败
	ROOM_TMP_CREATE_ERROR,			//创建练歌房失败
	ROOM_MAX,

	OPTION_TIMEOUT = 9999,		//操作超时
	OPTION_NOTCONN,				//未连接
	OPTION_PERMISSIONS_ERROR,
	OPTION_MAX
};

enum NOTCONN
{
	NOTCONN_AUTH = 101,
	NOTCONN_LIST = 102,
	NOTCONN_LOBBY = 103,
	NOTCONN_ROOM = 104,

	NOTCONN_MAX
};

enum CHANGEAVARAT
{
	CHANGEAVARAT_NOANY = 0,
	CHANGEAVARAT_NOCHANGE = -1
};

enum BUY_TYPE
{
	BUYTYPE_SELFG = 0,
	BUYTYPE_SENDG
};

enum ROOM_XGCLIENT_TYPE
{
	ROOMTYPE_HUNDRED,
	ROOMTYPE_SIX,
	ROOMTYPE_PRACTICEG,
	ROOMTYPE_OTHER,
	ROOMTYPE_BASEG = 99
};

//enum {
#define MAXLEN_KTV_ROOM_NAME 68
#define MAXLEN_ROOM_NAME  19
#define MAXLEN_ROOM_PWD  64
#define MAXLEN_TMPROOM_PWD  7
#define MAXLEN_TMPROOM_NAME  21
#define MAXLEN_TMPROOM_COUNT  2000
#define MAXLEN_TIMESTR  51
#define MAXLEN_GLOBALID_CNT  100
#define MAXLEN_IP  20
#define MAXLEN_PROVINCE  21
#define MAXLEN_CITY  11
#define MAXLEN_NICKNAME  64
#define MAXLEN_IDX  11
#define MAXLEN_MOVEROOM  30
#define MAXLEN_SYSTEM_MSG  1024
#define MAXLEN_SYSTEM_BROAD  256
#define MAXLEN_PCINFO  1024
#define MAXLEN_SYSTEMINFO  2048
#define MAXLEN_TOKEN  256
#define MAXLEN_ROOM_LOGO  512
#define MAXLEN_ROOM_TYPENAME  16
#define MAXLEN_ROOM_AVIPCOUNT  6
#define MAXLEN_MACADD  40
#define MAXLEN_BUY_MSG  256
#define MAXLEN_USERSPEAKER_INFO  65536 // 小喇叭最大信息长度
#define MAXLEN_CHAT  5			// 小喇叭最大数目

#define MAXLEN_WAITMICLIST  30		// 最大麦序数
#define MAXLEN_MAX  0
//};

enum Net_Level_Error
{
	Net_Colse_Normal = 0,
	Net_RecvBuf_Error = 9000000,
	Net_SendBuf_Error,
	Net_Stream_Error,
	Net_Epoll_Error,
};

struct ResultData_AvatarData
{
	char valuecount;
	short values[MAX_AVARTA_VALUE];
};

struct ResultData_UserFullInfo
{
	int ret;											//登录结果
	int m_idx;											//idx编号

	char m_id[21];										//用户id,为字符串
	char m_name[MAXLEN_NICKNAME];						//用户名字
	char m_province[21];								//省
	char m_city[11];									//城市
	char m_sex[2];										//性别
	char m_age[4];										//年龄
	int m_level;										//用户等级可转int
	char m_LevelTime[MAXLEN_TIMESTR];					//vip到期时间
	char m_isVip;										//是否vip账户
	char m_stage[6];
	char m_stageName[51];

	char m_ClubId[10];									//所在俱乐部id
	char m_ClubProfession[4];							//俱乐部权限：1管理员,0非管理员
	char m_ClubName[50];								//俱乐部名称	add by jinguanfu 2010/4/20
	char m_ExpLevel;

	char m_AvatarId;									//
	char m_AvaratCount;
	short * m_AvaratData;

	// add by wpf 2012/5/24
	short m_singerlevel; //歌手等级
	int m_singer_expvalue;
	short m_wealthlevel; //财富等级
	int m_wealth_expvalue;
	// end by wpf

	char	headurl[260];    //头像URL
	int		recvcount;        //礼物接收数
	int		songcount;        //演唱歌曲总数
	char	topsongname[260];   //拿手歌曲
	int		topsongscore;   //演唱最高分

	// new info
	//最近一次演唱记录,如果没有都是0
	char songname[256];  // 歌曲名
	char singername[51];  //歌手名
	int score;       //分数
};//version id

struct ResultData_PartInfo
{
	int idx; 
	char sex; 
	short age;
	char province[MAXLEN_PROVINCE];
	int plen;
	char city[MAXLEN_CITY];
	int clen;
	char nick[MAXLEN_NICKNAME];
	int nicklen; 
	char singlevel;
	int gold; 
	int silver;
	int nexp;
};

//add by jinguanfu 2010/3/31 
struct ResultData_RoomIPPort
{
	int roomid;				//房间号
	char ip_1[MAXLEN_IP];	//房间IP
	short port_1;			//房间端口
	char ip_2[MAXLEN_IP];	//房间IP
	int status;				//0--UNRUNING,1--RUNING,2--LOCKED
};

struct ResultData_LobbyLogin : public ResultData_UserFullInfo
{
	int roomcount;						//房间数量
	ResultData_RoomIPPort * roomlist;	//房间列表指针
	int group_versionid;				//分组版本号
	int m_roomright_versionid; //房间权限版本号
};

struct ResultData_SelfData
{
	short is_adult;	// 是否经过防沉迷认证
	int gold;		// 金币
	int silver;		// 银币
	int nexp;		// 经验值
	int m_remind;	// 是否提示消费通知
	int m_consume;	// 皇冠用户当月消费金币数
};

struct UserStatus
{
	int idx;	//数字帐号
	int lobbyid;//大厅id
	int roomid; //房间id
};

struct ResultData_GetFriendlist
{
	int count;												//列表长度
	UserStatus *pUS;
};

typedef ResultData_GetFriendlist ResultData_GetBlacklist;

struct ClubUserStatus
{
	int idx;	//数字帐号
	char pro;	//俱乐部职业
	int lobbyid;//大厅id
	int roomid;	//房间id
};

struct ResultData_GetClublist
{
	int count;
	ClubUserStatus *pCUS;
};

struct Channel
{
	int sectionid;							//区id
	char sectionname[50];					//区名称
	int lobbyid;							//大厅id
	char lobbyname[50];						//大厅名称
	char lobbyip_1[MAXLEN_IP];				//大厅ip地址
	short lobbyport;						//大厅端口
	char lobbyip_2[MAXLEN_IP];				//大厅ip地址
};

struct ResultData_GetChannellist
{
	int count;
	Channel *pCh;
};

struct ResultData_LobbyLoginUser//OutUser
{
	int idx;
	char relation;						//USER_RELATION对方关系
	int lobbyid;
};

struct ResultData_LobbyRoomEnter
{
	int idx;
	char relation;						//USER_RELATION对方关系
	int lobbyid;
	int roomid;
};

struct ResultData_LobbyInfo
{
	char name[20];						//俱乐部名称
	int maxnumber;						//俱乐部用户上限
	char slogan[50];					//俱乐部口号
	char notice[50];					//俱乐部通知
	char picture[100];					//俱乐部图片链接
	int adminidx;						//俱乐部管理员idx
	char admintele[20];					//俱乐部管理员电话
	char adminQQ[20];					//俱乐部管理员QQ

};

struct ResultData_ChatMessage
{
	int idx;							//用户数字帐号
	int clubid;							//俱乐部id
	char *content;						//消息内容
};

struct ResultData_SystemMessage
{
	char *content;						//消息内容
};

struct ClubApply
{
	int id;								//申请编号
	int idx;							//申请人idx
	char time[32];						//申请时间
};

struct ResultData_ClubApplyList
{
	int count;
	ClubApply *pCA;
};

struct ResultData_Approvelresult
{
	int ret;							//审核结果
	int id;								//申请编号
	int idx;							//申请人idx
};

struct ResultData_ClubInfo
{
	int clubid;							//俱乐部编号
	char name[20];						//俱乐部名称
	char slogan[50];					//俱乐部口号
	char admintele[20];					//俱乐部管理员电话
	char adminQQ[20];					//俱乐部管理员QQ
	int assistantidx;					//俱乐部管理员idx
};

struct ResultData_ClubFullInfo
{
	char name[20];						//俱乐部名称
	int maxnumber;						//俱乐部用户上限
	char slogan[50];					//俱乐部口号
	char notice[50];					//俱乐部通知
	char picture[100];					//俱乐部图片链接
	int adminidx;						//俱乐部管理员idx
	char admintele[20];					//俱乐部管理员电话
	char adminQQ[20];					//俱乐部管理员QQ
};

struct ResultData_RoomInfo
{
	int idx;						//房间idx
	int hallid;						//房间所在大厅的id
	char name[50];					//房间名
	//char pwd[20];					//进入房间密码		del by jinguanfu 2010/4/8
	char pwdflag;					//房间是否有密码 0-无 1-有 add by jinguanfu 2010/4/8
	int type;						//房间类型	0--百人房,1--六人房
	int state;						//房间状态,运行/冻结/停止
	int masterid;					//室主id
	char createdate[30];			//房间创建日期
	char sortflag[20];				//排序标志(是否允许自由排麦)
	int maxusernum;					//房间最大在线人数
	char deadline[30];				//房间到期时间
	char ispassaudit[2];			//申请会员是否需要审批
	char introduce[512];			//房间欢迎词
	char ip_1[MAXLEN_IP];			//房间IP
	short port;						//房间连接端口
	char ip_2[MAXLEN_IP];			//房间IP
	char content[1024];				//房间公告
	char updatedate[30];			//更新时间
	char logo[MAXLEN_ROOM_LOGO];
	int chatflag;					//允许房间公聊标识
	int inoutflag;					//显示房间进出信息标识
	int useronlyflag;				//房间私有标志位（锁定）//add by jinguanfu 2010/4/22
	int closeflag;					//房间关闭标志位		//add by jinguanfu 2010/4/22
};

struct ResultData_LoginInfo
{
	char ret;							//LOGIN_RET登录结果
	char identity;						//USER_IDENTITY用户属性
};

struct ResultData_UserInfo
{
	int idx;
	char identity;						//USER_IDENTITY用户属性
};

struct ResultData_MicInfo
{
	ResultData_MicInfo() : idx(0),musicid(0),bk(0),state(0), pkidx(0),pkmusicid(0),
		pkbk(0), pkstate(0), level(0){}
	int idx;							//排麦人的idx
	int musicid;						//音乐id
	short bk;							//背景
	char state;							//状态
	int pkidx;							//pk人的idx
	int pkmusicid;					//pk的音乐id
	short pkbk;							//pk的背景
	char pkstate;						//pk状态
	char level;							//难度
	//char change;						//是否对掉
	char speed;							//速度
	char name[128];						//歌曲名称
};

struct ResultData_RoomAllInfo
{
	int usercount;						//房间用户数量
	//ResultData_UserInfo *pUI;			//房间用户列表
	int *userlist;						//房间用户列表
	int miccount;						//房间排麦列表计数
	ResultData_MicInfo *pMLI;			//房间排麦列表
	int vjonmic;						//主持人idx
	ResultData_MicInfo *ponmic;			//上麦信息
	int managercount;					//管理员个数
	ResultData_UserInfo *pMI;			//房间管理员列表
	char *roomcontent;					//房间公告
	char ip_telcom[ MAXLEN_ROOM_AVIPCOUNT ][ MAXLEN_IP ];
	char ip_netcom[ MAXLEN_ROOM_AVIPCOUNT ][ MAXLEN_IP ];
	char ip_telcom_count;
	char ip_netcom_count;
	short port_telcom;;
	short port_netcom;
};

struct ResultData_UDMicInfo
{
	char tag;							//MIC_UP_DOWN_TAG	idx指示标志
	int idx;
	char flag;							//MIC_UP_DOWN_FLAG麦序操作类型
};

struct ResultData_ForbidenInfo
{
	int idx;
	char flag;							//FORBIDEN_FLAG麦序操作类型
};

struct ResultData_Score
{
	char haspk;							//是否有pk用户
	int idx;							//主麦用户idx
	int idxscore;						//主麦用户得分
	int pkidx;							//pk用户idx
	int pkidxscore;						//pk用户得分
};

struct ResultData_UpBlackList
{
	int idx;
	char flag;							//ADD_SUB_FLAG增加或删除标志
};

struct ResultData_UpMgrList
{
	int idx;
	char flag;							//ADD_SUB_FLAG增加或删除标志
	char identity;						//USER_IDENTITY用户属性
};

struct ResultData_SendFlowerInfo
{
	int s_idx;						//送礼者IDX
    int s_roomid;              //发送者房间
    char s_room[260];      //发送者房间名字
	int r_idx;						//收礼者IDX
    int r_roomid;               //接受者房间
    char r_room[260];       //收礼者房间名字
	int cate_idx;				//礼物类别
	int number;			        //礼物总数
	int leavings;		        //礼物剩余数
	int time;
	int once_num;				//每次送礼的数量
};

struct RoomApply    //单个申请结构
{
	int idx;				//申请人idx
	int roomid;				//申请房间号
	char state;				//申请状态
	char time[32];			//申请时间
};
struct ResultDate_RoomApplyList
{
	int count;          	//申请个数
	RoomApply *pRA;			//申请内容
};

struct RoomUser
{
	int  idx;     //会员idx
	bool online;   //在线状态
};
struct ResultDate_RoomMemList
{
	int count;   //成员个数
	RoomUser *userlist;  //成员idx列表
};

struct ResultDate_RoomBlackList
{
	int count;   //黑名单人员个数
	int *list;    //黑名单人员idx列表
};
struct ResultDate_RoomPanel
{
	char pwdflag; 				//是否设置房间密码 0:无 1:有
	char roomflag; 				//房间状态 
	char accessflag; 			//房间访问权限
	char inoutflag; 			//是否显示用户进出信息
	char onmicflag; 			//自由上麦
	char getmicflag;			//自由排麦
	char name[50]; 				//房间名
	char notice[512]; 			//房间公告
	char chatflag; 				//允许房间公聊
	char Salutatory[256]; 		//欢迎词
	char logopath[256]; 		//logo路径
	char pwd[MAXLEN_ROOM_PWD];	//pwd内容
	char privatemicflag;		//0:关闭私麦  1:开放私麦
};

//单个房间信息
struct RoomList
{
	int roomid;				//申请编号
	char roomname[50];	    //房间名  
};

struct RoomListEx
{
	int playerid;
	int roomid; //房间id
	char ip_1[MAXLEN_IP];  //ip
	short port;  //端口
	char ip_2[MAXLEN_IP];  //ip
	char m_name[MAXLEN_TMPROOM_NAME];   //名称
	char haspasswd;     //是否有密码
	char scoreflag;
	char voteflag;
	char nmaxplayernum;  //最大人数
	char playernum;  //当前人数
};

struct ResultDate_RoomList
{
	int lobbyid;	//大厅ID
	int count;		//房间个数
	RoomList *pRL;	//房间信息
};

struct ResultDate_RoomListEx
{
	int count;			//房间个数
	RoomListEx * pRL;	//房间信息
};

struct ResultData_LobbyName
{
	int lobbyid;
	char lobbyname[50];
};

struct ResultData_RoomSimpleInfo
{
	int roomid;			//房间ID
	char roomname[50];	//房间名
	char pwdflag;		//房间有无密码 0-无 1-有
	short clientnum;	//在线人数
	short boy;
	short girl;
	int isvip;
	int	isclose;
	int	state;
	int	maxplayer;
	short re_player_num;
};

struct ResultData_RoomSimpleInfoList
{
	short roomcount;	//房间个数
	ResultData_RoomSimpleInfo* RoomSimpleInfoList;
};

struct ResultData_LobbyOnline
{
	int lobbyid;		//大厅ID
	int onlinenum;		//大厅在线人数
};

struct ResultData_LobbyOnlineList
{
	short lobbycount;	//大厅个数
	ResultData_LobbyOnline* Lobbyonline;
};

//enum{
#define PART_LEN  6
#define LINK_LEN  7
#define CNTL_LEN  1
//};

enum CNTL_
{
	CNTL_KEY,
	CNTL_MAX
};

enum COUNT_
{
	COUNT_PART	= 0,
	COUNT_LIST	= 1,

	COUNT_KEY	= 0,
	COUNT_LEN	= 1,

	COUNT_MAX	= 2
};

struct ResultData_Avatar
{
	char avaratid;
	char len;
	short goods_id[PART_LEN+LINK_LEN*10];
};

struct ResultData_Avatar_Updata : public ResultData_Avatar
{
	char count[PART_LEN+LINK_LEN][COUNT_MAX];
	int global_id[PART_LEN+LINK_LEN*10];
};

struct ResultData_Buy
{
	short goods_id;
	char count;
	char msg[ MAXLEN_BUY_MSG ];
};

struct ResultData_Pack
{
	int global_id;
	short goods_id;
	char status;
	int day_num;
	char buy_day[ MAXLEN_TIMESTR ];
	char is_use;
	int count;
};

struct ResultData_CreateRoom
{
	char name[MAXLEN_TMPROOM_NAME];
	char is_pwd;
	char pwd[MAXLEN_TMPROOM_PWD];
	char is_score;
	char is_ballot;
	char count;
};

enum ROOM_STATA
{
	RUNABLE_CNNABLE = 1,	// 状态RUNABLE和CNNABLE的结合,房间为运行状态
	LOCKED_CNNABLE,			// 状态LOCKED和CNNABLE的结合,房间已冻结,但该房间的连接正常
	RUNABLE_CNNDISABLE,		// 状态RUNABLE和CNNDISABLE的结合,房间可以运行,但当前没有该房间的连接
	LOCKED_CNNDISABLE		// 状态LOCKED和CNNDISABLE的结合,房间已冻结,并且房间未连接
};

struct ResultData_Gift
{
	int gift_type; //礼物类型
	int recv_idx;
	char recv_id[ MAXLEN_NICKNAME ];
	int recv_num; //接受的礼物数
};

struct ResultData_GiftList
{
	short gift_count;
	char curtime[MAXLEN_TIMESTR];
	ResultData_Gift list[50];
};

struct ResultData_ChangeNickName
{
	int player_idx;
	char nick_name[MAXLEN_NICKNAME];
};

struct ResultData_IncomeDate
{
	int income;
	char date[MAXLEN_TIMESTR];
};

struct ResultData_RoomMoveData
{
	short add_count;
	short del_count;
	int * add_room;
	int * del_room;
};

struct ResultData_GroupInfo
{
	short group_id;
	char group_name[ MAXLEN_ROOM_TYPENAME ];
	short disable; //0：启用 1：禁用
};

struct ResultData_GiftCount
{
	int   idx;
	int   giftid;
	int   giftcount;
};

struct ResultData_AvChange
{
	short port_telcom;
	short port_netcom;

	char telcom_count;
	char netcom_count;

	char telcom[ MAXLEN_ROOM_AVIPCOUNT ][ MAXLEN_IP ];
	char netcom[ MAXLEN_ROOM_AVIPCOUNT ][ MAXLEN_IP ];
};

struct ResultData_SongInfo
{
	int m_musicid;
	short m_topscores; //最高分
	int m_topidx; //最高分idx
	char m_topname[ MAXLEN_NICKNAME ];
	short m_dayclicks; //日点击率
	short m_weekclicks; //周点击率
	short m_monthclicks; //月点击率
	int m_totalclicks; //总的点击率
};

struct ResultData_AvServer 
{
	char ip[ MAXLEN_IP ];
	short port;
};

struct ResultData_OffLineMsg
{
	int idx;
	char buy_day[ MAXLEN_TIMESTR ];
	char buy_msg[ MAXLEN_BUY_MSG ];
};

struct ResultData_RoomSort
{
	int idx;
	int sort;
};

typedef struct _ResultData_UserSampleInfo
{
	int idx;
	char nickname[ MAXLEN_NICKNAME ];		// 昵称
	char headurl[256];//头像
}ResultData_UserSampleInfo;

typedef struct _ResultData_UserSpeakerInfo
{
	int time;			// 发送时间
	int fromidx;		//发送者idx;
	char chat[ MAXLEN_USERSPEAKER_INFO ];	// 小喇叭内容
    int roomId;//房间ID
    char roomName[MAXLEN_KTV_ROOM_NAME];//房间名字 ktv店名+包厢名 举例：小和山西和店A31小包
}ResultData_UserSpeakerInfo;

struct ResultData_GiftInfo
{
	int from_idx;
	int to_id;	//烟花--为房间id，非烟花--为用户idx
	int number;
	int pid;
	char type; //0--非烟花，1--烟花礼物
	int time;
	char runway; //1--普通轨道，2--贵宾轨道
	int roomid; //在哪个房间产生的
	char roomname[128]; //房间名
};

typedef struct _UserSingInfo//add in 9/24 2013
{
	char	headurl[260];    //头像URL
	int	recvcount;        //礼物接收数
	int	songcount;        //演唱歌曲总数
	char	topsongname[260];   //拿手歌曲
	int	topsongscore;   //演唱最高分
}ResultData_UserSingInfo;

/************************************************************************/
/* 私麦                                                                 */
/************************************************************************/
enum RET_PRIVATE_MIC{ //返回值
	RET_PRIVATE_VEDIO_ERROR = -1,  //错误
	RET_PRIVATE_VEDIO_SUCCESS = 0, //成功
};
enum ERROR_PRIVATE_MIC{ //错误码
	PRI_VEDIO_ERROR_POWER_NO_ENOUGH = 100,   //权限不够
	PRI_VEDIO_ERROR_FULL,                    //私麦已经满了
	PRI_VEDIO_ERROR_NOTFIND_THIS_GUY_ONMIC,  //私麦没有找到这个人
	PRI_VEDIO_ERROR_GUY_NO_INTHISROOM,       //这个人么有在这个房间里面
	PRI_VEDIO_ERROR_INVITE_NUM,              //邀请数量错误
	PRI_VEDIO_ERROR_GUY_NOIN_INVITE_MAP,     //没有在邀请列表中
};

/************************************************************************/
/* 家族信息                                                             */
/************************************************************************/
#define FAMILY_NAME_MAXLEN 128
#define FAMILY_URL_MAXLEN 128
#define FAMILY_INTRUDUCE 256
#define FAMILY_MAX_ROOMS 128 //每个家族的最大房间数
struct ResultData_Family
{

	int familyId; //家族id
	int familyManageIdx; //族长idx
	short familyState; //家族状态（0正常，1冻结，9删除）
	short familySort; //家族顺序
	char familyName[FAMILY_NAME_MAXLEN]; //家族名
	char familyIntroduce[FAMILY_INTRUDUCE]; //家族介绍
	char familyImage[FAMILY_URL_MAXLEN]; // 家族海报
	char familyLogo[FAMILY_URL_MAXLEN];//家族logo

	int roomsize; //家族里的房间数量
	int roomids[FAMILY_MAX_ROOMS];//家族里的房间列表
};

//个人信息 by glp 2014-6-19
typedef struct _ResultData_GUser
{
    ResultData_UserFullInfo fullInfo;
    //0: 表示没有认证
	// 1: 表示暂定未防沉迷 （认证后，成年用户，未向公安部验证）
	// 2: 防沉迷 (认证后，未成年用户)
	// 3: 非防沉迷（向公安部验证后的成年用户）
    short   is_adult;//防沉迷认证标示
    bool    is_remind;//是否提示消费通知
    int		consume;// 皇冠用户当月消费金币数
    int		family_idx;//所属家族
	bool	peri;//是否美女
}ResultData_GUser;

typedef struct _USERINFO
{
    int idx;                // 用户idx
    char nickname[51];      //用户昵称
}USERINFO;

typedef struct _ResultData_SongList
{
	int songid;//歌曲ID
    char songname[260];//歌曲名
    char singername[260];//歌手名
}ResultData_SongList;

typedef struct _GroupRoomInfo{
    short grpup_id;//分组ID
    short roomnum;//房间数量
    int* roomlist;//房间ID列表
}ResultData_GroupRoomInfo;

typedef struct _RequestData_IdxWithVersion
{
	int id;
	int version;
}RequestData_IdxWithVersion;

typedef struct _ResultData_UIdxWithType
{
	int   idx; //用户idx
	char  type;  //0-主播，1-本包厢，2-其他包厢，3-大厅
} ResultData_UIdxWithType;

typedef struct _ResultData_HBDetail
{
	int idx;//idx
	int gold;//金币
	int time;  //领取时间
}ResultData_HBDetail;

typedef struct _ResultData_RoomHB
{
	int idx;//红包发送IDX
	char hbid[51];//红包ID
	char mark[260];//红包备注
	char hbtype;//红包类型
}ResultData_RoomHB;
#endif
