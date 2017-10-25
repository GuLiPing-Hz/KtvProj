#ifndef XINGGUANGDEF__H__
#define XINGGUANGDEF__H__

/*
	一些define
*/

#include <vector>
#include "protocol.h"
#include "ResultData.h"
using namespace std;

#define MAXBUFSIZETEA 256
#define	AUTHTUNNEL	1
#define	LOBBYTUNNEL	2
#define ROOMTUNNEL	3
#define CHANNELLISTTUNNEL	4
#define MSG_LENGTH 65535


struct VA_ConnectToSvr 
{
	char host[20];
	short port;
	int to;
};

struct VA_IdPassAuth
{
	char id[21];
	char passwd[33];
	int again;
};

typedef struct _VA_TouristAuth
{
	int again;
}VA_TouristAuth;

struct VA_ROOM
{
	int cseq;
	int id;
	char passwd[64];
	short type;
};

struct VA_CHAT
{
	int fromidx;
	int idx;
	int clubid;
	unsigned int msglen;
	char content[MSG_LENGTH];
	char nickname[40];
	char headurl[260];
};

struct VA_SCORE
{
	int score;
	unsigned int len;
	char certify[256];
};

struct VA_PCINFO
{
	unsigned int len;
	char buf[MAXLEN_PCINFO];
};

struct VA_SYSTEMINFO
{
	unsigned int len;
	char buf[MAXLEN_SYSTEMINFO];
};

struct VA_GETCLUB
{
	int clubid;
	int day;
	int pageindex;
	int pagesize;
	int cseq;
};

struct VA_APPROVALRESULT
{
	int count;
	char flag;
	int *idlist;
};

struct VA_GETROOMINFOLIST
{
	int cseq;
	int count;
	int *list;
};

struct VA_CREATEAVARAT : public ResultData_Avatar_Updata
{
	char nickname[21];	//产品文档为15字节
};

struct VA_SENDFLOWER
{	
	int r_idx;
	int cate_dx;
	int number;
};
//
struct VA_OPTION
{
	int id;		//
	int type;	//
	int cseq;
};
struct VA_SETPASSWORD
{
	char password[33];
	int  passwordlen;
	int	 cseq;
};
struct VA_ROOMNAME
{
	int cseq;
	int length;
	char roomname[51];
};
struct VA_ROOMAFFICHE
{
	int cseq;
	int length;
	char content[1024];
};
struct VA_ROOMLOGO
{
	int cseq;
	int length;
	char logopath[256];
};


struct VA_APPLYADDFRIEND
{
	int idx;
	char flag;
};

class VA_CLUBINFO : public ResultData_ClubInfo
{
public:
	int cseq;
};

enum VARIANTTYPE
{
	//auth
	VATYPE_CONNECTTOAUTH = 0x01,
	VATYPE_CLOSEAUTH,

	VATYPE_IDAUTH,
	VATYPE_TOURISTAUTH,

	//lobby
	VATYPE_CONNECTTOLOBBY,
	VATYPE_CLOSELOBBY,
	VATYPE_CONNECTCHANNELLIST,
	VATYPE_CLOSECHANNELLIST,

	VATYPE_LOGINLOBBY,
	VATYPE_GETUSERINFO,
	VATYPE_LOBBYKEEPALIVE,
	VATYPE_GETFRIENDLISTWITHUSERINFO,
	VATYPE_GETFRIENDLISTWITHOUTUSERINFO,
	VATYPE_GETBLACKLISTWITHUSERINFO,
	VATYPE_GETBLACKLISTWITHOUTUSERINFO,
	VATYPE_GETCLUBLISTWITHUSERINFO,
	VATYPE_GETCLUBLISTWITHOUTUSERINFO,

	VATYPE_ADDFRIEND,
	VATYPE_ADDBLACK,
	VATYPE_DELFRIEND,
	VATYPE_DELBLACK,
	VATYPE_GETCHANNELLIST,

	VATYPE_CREATEROOM,
	VATYPE_DELETEROOM,
	VATYPE_GETROOMLIST,
	VATYPE_GETROOMLISTEX,
	VATYPE_GETROOMINFO,
	VATYPE_GETTMPROOMINFO,

	VATYPE_CREATEAVATAR,

	VATYPE_GETLOBBYINFO,

	VATYPE_GETFRIENDLOCATION,	//取得好友位置
	VATYPE_GETPARTINFO,			//取得角色信息
	VATYPE_GETROOMLISTBYLOBBY,	//取得对应大厅上的房间列表
	VATYPE_APPLYADDFRIEND,		//同意加为好友
	VATYPE_GETROOMSIMPLEINFO,	//取得简单房间信息
	VATYPE_GETROOMSIMPLEINFOEX,	//取得简单房间信息
	VATYPE_EXITROOMMEMBER,		//用户退出房间会员
	VATYPE_GETLOBBYONLINE,		//取得大厅在线用户数
	//add by cx 10-5-20
	VATYPE_CHANGEAVATAR,		// 换装
	VATYPE_CHANGENICKNAME,		// 更换昵称
	//add by cx 10-7-6
	VATYPE_BUYPACK,
	VATYPE_SENDPACK,
	VATYPE_GETPACK,
	VATYPE_DELPACK,
	VATYPE_USEPACK,
	VATYPE_CREATE_ROOM,

	VATYPE_GET_NEWUSER_TASK_C2L2C,	// 获取新手任务
	VATYPE_TASK_STEP_C2L2C,			// 提交新手任务
	VATYPE_TASK_GIFT_C2L2C,
	VATYPE_GET_GIFTINFO_C2L2C,		// 获取礼物滚动消息
	VATYPE_GET_SYSTME_SPEAKER_C2L2C,// 获取系统文字公告
	VATYPE_GET_USER_SEEL,			// 获取系统文字公告
	VATYPE_SEND_SYSTEM_INFO,
	VATYPE_GET_GROUP_INFO,			// 获取分组信息
	VATYPE_GET_ROOMLIST_IN_GROUP,	// 获取分组中的房间列表
	VATYPE_GET_MUSICEXINFO_C2L2C,	// 获取歌曲列表
	VATYPE_GET_AVSERVERINFO_C2L2C,	// 获取音视频服务器列表
	VATYPE_GET_OFFLINEMSG_C2L2C,	// 获取离线消息
	VATYPE_GET_ROOMSORT_IN_GROUP,	// 获取分组置顶信息
	VATYPE_GET_USERSPEAKERINFO_C2L2C,	// 获取用户小喇叭信息

	//room
	VATYPE_CONNECTTOROOM_BASE,
	VATYPE_CONNECTTOROOM_PRACTICE,
	VATYPE_CLOSEROOM,

	VATYPE_CONFIRMTOKEN,
	VATYPE_ENTERROOM,
	VATYPE_GETROOMALLINFO, 
	VATYPE_ROOMKEEPALIVE,
	VATYPE_ROOMWAITMIC,
	VATYPE_ROOMCANCLEWAITMIC,
	VATYPE_ROOMWAITMICPK,
	VATYPE_ROOMCANCLEWAITMICPK,
	VATYPE_ROOMUPWAITMIC,
	VATYPE_ROOMDOWNWAITMIC,
	__VATYPE_ROOMONMICREADYOK,
	__VATYPE_ROOMOFFMICREADY,
	VATYPE_ROOMSCORE,
	VATYPE_ROOMKICKOUTSOMEONE,
	VATYPE_ROOMFORBIDENSOMEONE,
	VATYPE_ROOMPRIVATECHAT,
	VATYPE_ROOMPUBLICCHAT,
	VATYPE_ROOMONVJMIC,
	VATYPE_ROOMOFFVJ_MIC,
	VATYPE_ROOMGIVEVJMIC,
	VATYPE_ROOMGIVEOFFVJMIC,
	VATYPE_ROOMGIVEOFFMIC,
	VATYPE_ROOMUPDATEBLACKLIST,
	VATYPE_ROOMUPDATEMGRLIST,
	VATYPE_ROOMUPDATECONTENT,

	VATYPE_SENDFLOWER,

	VATYPE_INVITEMIC,

	VATYPE_ROOM_GETWAITMICLIST,		//取麦序列表

	/**********************************************/
	/*add by jinguanfu 2009/12/28 <begin> 房间管理*/
	VATYPE_GETROOMAPPLYLIST,	//取得房间会员申请列表
	VATYPE_APPLYROOMMEMBER,		//申请房间会员
	VATYPE_AUDITADDAPPLY,		//审核房间会员申请
	VATYPE_GETROOMMEMBERLIST,	//取得房间会员申请
	VATYPE_UNCHAINROOMMEMBER,	//解除房间会员资格
	VATYPE_PESIDEASSISTANT,		//房间主持助理权限
	VATYPE_PESIDE,				//房间主持人权限
	VATYPE_SUBHOST,				//房间副室主权限
	VATYPE_GETROOMBLACKLIST,	//取得房间黑名单
	VATYPE_ADDDELROOMBLACK,		//添加删除房间黑名单
	VATYPE_GETROOMPANELINFO,	//取得房间设置面板信息
	VATYPE_ROOMPASSWORD,		//设置房间密码
	VATYPE_ROOMSWITCH,			//房间开关
	VATYPE_ROOMPRIVATE,			//房间访问权限
	VATYPE_USERINOUTSWITCH,		//用户进出信息开启/关闭
	VATYPE_FREEGETMICSWITCH,		//自由排麦开启/关闭
	VATYPE_FREEONMICSWITCH,		//自由上麦开启/关闭
	VATYPE_MODIFYROOMNAME,		//修改房间公告
	//VATYPE_ROOMAFFICHE,			//房间公告
	VATYPE_ALLOWROOMALLCHAT,	//允许房间公聊
	VATYPE_ROOMSALUTATORY,		//房间欢迎词
	VATYPE_ROOMLOGO,			//房间LOGO
	/*add by jinguanfu 2009/12/28 <end> 房间管理*/
	/********************************************/
	VATYPE_LEAVEROOM,			//离开房间（切换房间）
	//add by jinguanfu 2010/5/13
	VATYPE_MEMBER,				//设置为房间会员（由管理员降为一般会员）
	//add by cx 10-6-2
	VATYPE_APPLYGETVJMIC,		// 是否同意上主持人麦
	VATYPE_SEND_FIREWORKS,
	VATYPE_GET_GIFT_TOPLIST,
	VATYPE_GET_ONLINE_TIME,

	VATYPE_ROOM_VIEW_INCOME,		//显示房间可领取收益
	VATYPE_ROOM_VIEW_GET_INCOME,	//显示收益已领取记录
	VATYPE_ROOM_GET_INCOME,			//室主领取房间内收益

	VATYPE_ROOM_SEND_PC_INFO,		//发送pc简易信息

	VATYPE_ROOM_INVITE_MEMBER,		//邀请会员C->S
	VATYPE_ROOM_INVITE_REPLY,		//邀请回复C->S
	VATYPE_ROOM_GET_GIFTSEND,		//取得当日礼物赠送记录
	VATYPE_ROOM_GET_GIFTRECV,		//取得当日礼物接收记录
	VATYPE_ROOM_KICK_BY_MAC,		//封mac
	VATYPE_ROOM_KICK_BY_IP,			//封ip
	VATYPE_ROOM_SET_ONMIC_TIME,		//设置在麦时间
	VATYPE_ROOM_SET_SONG_TIME,		//设置在歌曲剩余时间

	//////////////////////////////////////////////////////////////////////////

	VATYPE_CHATTOUSER,
	VATYPE_CHATTOCLUB,
	VATYPE_CHATTOLOBBY,
	VATYPE_CHATTOALL,
	VATYPE_BROADALL,
	VATYPE_BROADLOBBY,
	VATYPE_SENDROOMNOTIECETEMP,

	VATYPE_CLUBAPPLYADD,
	VATYPE_CLUBAPPLYEXIT,
	VATYPE_CLUBGETAPPLYLISTCOUNT,
	VATYPE_CLUBGETAPPLYLIST,
	VATYPE_CLUBAPPROVALRESULT,
	VATYPE_CLUBCREATE,
	VATYPE_CLUBDISMISS,
	VATYPE_CLUBCHANGEINFO,
	VATYPE_CLUBCHANGEASSIS,
	VATYPE_CLUBGETINFO,

//	VATYPE_CHANGEAVATAR,// add by cx 10-5-20
	VATYPE_CREATEROOMOK,//???
	VATYPE_DELETEROOMOK,//???
	VATYPE_LOCKROOM,//???

	VATYPE_DELETEAVARTA,		//删除角色信息

	//////////////////////////////////////////////////////////////////////////

	/************************************************************************/
	/* 私麦                                                                 */
	/************************************************************************/
	VATYPE_OPEN_PRIVATEMIC,
	VATYPE_CLOSE_PRIVATEMIC,
	VATYPE_CLOSED_PRIVATEMIC,
	VATYPE_REQUEST_WATCH_PRIVATEMIC,
	VATYPE_RESPONSE_WATCH_PRIVATEMIC,
	VATYPE_INVITEPRIVATEMIC,
	VATYPE_RESPONSE_INVITEPRIVATEMIC,
	VATYPE_OPEN_ROOMPRIVATEMIC_FLAG,

	//皇冠踢人
	VATYPE_CROWN_KICKOUT_SOMEONE,
	//室主禁用户IP
	VATYPE_FORBID_USERIP,
	/************************************************************************/
	/* 家族                                                                 */
	/************************************************************************/
	VATYPE_GETFAMILYINFO,

	//发送句评分
	VATYPE_SINGER_SCORES,
	//上传歌曲
	VATYPE_UPDATESONG,
    
    // 获取分组中的房间列表
    KTV_GETROOMLIST_FROMGROUP,
    
    //手机用户聊天
    KTV_MOBILE_CHAT_C2L2C,
    //手机用户大厅送礼物
    KTV_MOBILE_SENDGIFT_C2L2C,
	VATYPE_GETUSERINFO_FORHORN_C2L2C,
	VATYPE_GETUSERINFO_WITHVERSION_C2L2C,
	//获取用户其他信息
	//VATYPE_GETUSEREXTINFO,
	VATYPE_MAX
};

typedef ResultData_UserInfo VA_USERINFO;

typedef std::vector<VA_USERINFO> UserInfoList;

typedef ResultData_LoginInfo VA_LOGININFO;

class VA_MIC_INFO : public ResultData_MicInfo
{
public:
		bool operator == (VA_MIC_INFO const & rhs)
		{
			return (idx == rhs.idx) ? true : false;
		}

		bool operator == (int nidx)
		{
			return (pkidx == nidx) ? true : false;
		}
};

typedef ResultData_UDMicInfo VA_UD_MICINFO;
typedef ResultData_ForbidenInfo VA_FORBIDENINFO;
class VA_UPBLACKLIST : public ResultData_UpBlackList
{
public:
	int cseq;
};
typedef ResultData_UpMgrList VA_UPMGRLIST;

struct VA_GETUSERINFO
{
	void *idxlist;
};

struct VA_ADDFRIEND
{
	int cseq;
	int idx;
};

typedef VA_ADDFRIEND VA_ADDBLACK;		//添加黑名单
typedef VA_ADDFRIEND VA_DELFRIEND;		//删除好友
typedef VA_ADDFRIEND VA_DELBLACK;		//删除黑名单
typedef VA_ADDFRIEND VA_FRIENDLOCATION;	//取好友位置
typedef VA_ADDFRIEND VA_UNCHAINMEMBER;	//取消房间会员
typedef VA_ADDFRIEND VA_PESIDEASSISTANT;//助理主持人权限
typedef VA_ADDFRIEND VA_PESIDE;			//主持人权限
typedef VA_ADDFRIEND VA_SUBHOST;		//副室主
typedef VA_ADDFRIEND VA_PARTINFO;		//取得角色信息
typedef VA_ADDFRIEND VA_MEMBER;			//房间会员

struct VA_ADDCLUB
{
	int cseq;
	int clubid;
};
typedef VA_ADDCLUB VA_EXITCLUB;	//退出俱乐部
typedef VA_ADDCLUB VA_DISMCLUB;	//解散俱乐部
typedef VA_ADDCLUB VA_GETCLUBINFO;

//取大厅名
struct VA_LOBBYINFO
{
	int cseq;
	int lobbyid;
};

typedef VA_LOBBYINFO VA_ROOMLIST;

struct VA_SETFLAG
{
	int cseq;
	char flag;
};

#define MAXBUYCOUNT 10
struct VA_BUYCOUNT
{
	int idx;
	char count;
	ResultData_Buy pack[MAXBUYCOUNT];
};

struct VA_DELPACP
{
	int idx[MAXLEN_GLOBALID_CNT];
	int cseq;
	char count;
};

typedef ResultData_CreateRoom VA_CREATE_ROOM;

struct VA_GET_ROOM_LIST
{
	int cseq;
	int type;
};

struct VA_SEND_FIREWORKS
{
	int idx;
	int count;
};

struct WEB_DATA
{
	char url[ 128 ];
	char data[ 1024 ];
	int len;
};

struct VA_GET_LOBBY_LIST
{
	int version;
	char mac[ MAXLEN_MACADD ];
	int len;
};

struct VA_LOGIN_LOBBY
{
	char mac[ MAXLEN_MACADD ];
	int len;
	short isautologin;
	int roomid;
    int loginway;
};

struct VA_SET_MICTIME
{
	int idx;
	int time;
};

enum FLAG
{
	FLAG_ADD = 0,    //添加
	FLAG_DELETE,	 //删除
	FLAG_UPDATE,     //更新
};

struct VA_SEND_SCORES
{
	int idx;
	void * scores;
};

typedef struct _KTV_CHAT_INFO
{
    int toIdx;
    char buf[4096];
}KTVCHATINFO;

typedef struct _KTV_SEND_GIFT
{
    int toIdx;
    int giftId;
    int count;
    int roomId;
}KTV_SEND_GIFT;

typedef struct _GroupBatch
{
    short count;
    short* pId;
}GroupBatch;

typedef struct _IdxArray
{
	int* idx;
	int count;
}IdxArray;

typedef struct _IdxVersionArray
{
	RequestData_IdxWithVersion* idx;
	int count;
}IdxVersionArray;

#endif//XINGGUANGDEF__H__
