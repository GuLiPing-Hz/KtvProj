/*
 * 
 *   by glp
 *   2012-4-9   
 *   定义c中的相应结构数据，以备在java中使用
 * 
 */

package com.jni.netutil;


class ResultData_GiftInfo
{
	public int from_idx;
	public int to_id;	//烟花--为房间id，非烟花--为用户idx
	public int number;
	public int pid;
	public int type; //0--非烟花，1--烟花礼物
	public int time;
	public int runway; //1--普通轨道，2--贵宾轨道
	public int roomid; //在哪个房间产生的
	public String roomname;//char roomname[128]; //房间名
};

class RoomApply    //单个申请结构
{
	public int idx;				//申请人idx
	public int roomid;				//申请房间号
	public int state;				//申请状态
	public String time;//char time[32];			//申请时间
};
class ResultDate_RoomApplyList
{
	public int count;          	//申请个数
	public RoomApply[] ra;			//申请内容
};

class RoomUser
{
	public int  idx;     //会员idx
	public boolean online;   //在线状态
};
class ResultDate_RoomMemList
{
	public int count;   //成员个数
	public RoomUser[] userlist;  //成员idx列表
};

class ResultDate_RoomBlackList
{
	public int count;   //黑名单人员个数
	public int[] list;    //黑名单人员idx列表
};

class ResultDate_RoomPanel
{
	public byte pwdflag; 				//是否设置房间密码 0:无 1:有
	public byte roomflag; 				//房间状态 
	public byte accessflag; 			//房间访问权限
	public byte inoutflag; 			//是否显示用户进出信息
	public byte onmicflag; 			//自由上麦
	public byte getmicflag;			//自由排麦
	public String name;//char name[50]; 				//房间名
	public String notice;//char notice[512]; 			//房间公告
	public byte chatflag; 				//允许房间公聊
	public String Salutatory;//char Salutatory[256]; 		//欢迎词
	public String logopath;//char logopath[256]; 		//logo路径
	public String pwd;//char pwd[MAXLEN_ROOM_PWD];	//pwd内容
	public byte privatemicflag;		//0:关闭私麦  1:开放私麦
};

class ResultDate_RoomList
{
	public int lobbyid;	//大厅ID
	public int count;		//房间个数
	public RoomList[] RL;	//房间信息
};

//class RoomListEx
//{
//	public int playerid;
//	public int roomid; //房间id
//	public String ip_1;//char ip_1[MAXLEN_IP];  //ip
//	public short port;  //端口
//	public String ip_2;//char ip_2[MAXLEN_IP];  //ip
//	public String m_name;//char m_name[MAXLEN_TMPROOM_NAME];   //名称
//	public byte haspasswd;     //是否有密码
//	public byte scoreflag;
//	public byte voteflag;
//	public byte nmaxplayernum;  //最大人数
//	public byte playernum;  //当前人数
//};

class ResultDate_RoomListEx
{
	public int count;			//房间个数
	public RoomListEx[]  RL;	//房间信息
};

//class ResultDate_Channel {
//	public int sectionid;							//区id
//	public String sectionname;//char sectionname[50];					//区名称
//	public int lobbyid;							//大厅id
//	public String lobbyname;//char lobbyname[50];						//大厅名称
//	public String lobbyip_1;//char lobbyip_1[MAXLEN_IP];				//大厅ip地址
//	public short lobbyport;						//大厅端口
//	public String lobbyip_2;//char lobbyip_2[MAXLEN_IP];				//大厅ip地址
//}
//
//class ResultData_GetChannellist
//{
//	public int count;
//	public ResultDate_Channel[] Ch;
//};

class ResultData_LobbyLoginUser
{
	public int idx;
	public char relation;						//USER_RELATION对方关系
	public int lobbyid;
};

//class ResultData_RoomIPPort
//{
//	public int roomid;				//房间号
//	public String ip_1;//char ip_1[MAXLEN_IP];	//房间IP
//	public short port_1;			//房间端口
//	public String ip_2;//char ip_2[MAXLEN_IP];	//房间IP
//	public int status;				//0--UNRUNING,1--RUNING,2--LOCKED
//};



//单个房间信息
//class RoomList
//{
//	public int roomid;				//申请编号
//	public String roomname;//char roomname[50];	    //房间名  
//};

//class ResultData_LobbyLogin extends ResultData_UserFullInfo
//{
//	public int roomcount;						//房间数量
//	public ResultData_RoomIPPort[] roomlist;	//房间列表指针
//	public int group_versionid;				//分组版本号
//	public int m_roomright_versionid; //房间权限版本号
//};

class UserStatus
{
	public int idx;	//数字帐号
	public int lobbyid;//大厅id
	public int roomid; //房间id
};

class ResultData_GetFriendlist
{
	public int count;												//列表长度
	public UserStatus[] US;
};

class ClubUserStatus
{
	public int idx;	//数字帐号
	public byte pro;	//俱乐部职业
	public int lobbyid;//大厅id
	public int roomid;	//房间id
};

class ResultData_GetClublist
{
	public int count;
	public ClubUserStatus[] CUS;
};

class ClubApply
{
	public int id;								//申请编号
	public int idx;							//申请人idx
	public String time;//char time[32];						//申请时间
};

class ResultData_ClubApplyList
{
	public int count;
	public ClubApply[] CA;
};

class ResultData_Approvelresult
{
	public int ret;							//审核结果
	public int id;								//申请编号
	public int idx;							//申请人idx
};

class ResultData_ClubFullInfo
{
	public String name;//char name[20];						//俱乐部名称
	public int maxnumber;						//俱乐部用户上限
	public String slogan;//char slogan[50];					//俱乐部口号
	public String notice;//char notice[50];					//俱乐部通知
	public String picture;//char picture[100];					//俱乐部图片链接
	public int adminidx;						//俱乐部管理员idx
	public String admintele;//char admintele[20];					//俱乐部管理员电话
	public String adminQQ;//char adminQQ[20];					//俱乐部管理员QQ
};

class ResultData_LobbyRoomEnter
{
	public int idx;
	public byte relation;						//USER_RELATION对方关系
	public int lobbyid;
	public int roomid;
};

class ResultData_LobbyName
{
	public int lobbyid;
	public String lobbyname;//char lobbyname[50];
};

class ResultData_LoginInfo
{
	public byte ret;							//LOGIN_RET登录结果
	public byte identity;						//USER_IDENTITY用户属性
};

class ResultData_UserInfo
{
	public int idx;
	public byte identity;						//USER_IDENTITY用户属性
};

class ResultData_MicInfo
{
	public int idx;							//排麦人的idx
	public int musicid;						//音乐id
	public short bk;							//背景
	public byte state;							//状态
	public int pkidx;							//pk人的idx
	public int pkmusicid;					//pk的音乐id
	public short pkbk;							//pk的背景
	public byte pkstate;						//pk状态
	public byte level;							//难度
	//char change;						//是否对掉
	public byte speed;							//速度
	public String name;//char name[128];						//歌曲名称
};
class ResultData_RoomAllInfo
{
	public int usercount;						//房间用户数量
	//ResultData_UserInfo *pUI;			//房间用户列表
	public int[] userlist;						//房间用户列表
	public int miccount;						//房间排麦列表计数
	public ResultData_MicInfo[] MLI;			//房间排麦列表
	public int vjonmic;						//主持人idx
	public ResultData_MicInfo[] onmic;			//麦上信息
	public int managercount;					//管理员个数
	public ResultData_UserInfo[] MI;			//房间管理员列表
	public String roomcontent;//char *roomcontent;					//房间公告
	public String[] ip_telcom;//char ip_telcom[ MAXLEN_ROOM_AVIPCOUNT ][ MAXLEN_IP ];
	public String[] ip_netcom;//char ip_netcom[ MAXLEN_ROOM_AVIPCOUNT ][ MAXLEN_IP ];
	public byte ip_telcom_count;
	public byte ip_netcom_count;
	public short port_telcom;;
	public short port_netcom;
};

class ResultData_UDMicInfo
{
	public byte tag;							//MIC_UP_DOWN_TAG	idx指示标志
	public int idx;
	public byte flag;							//MIC_UP_DOWN_FLAG麦序操作类型
};

class ResultData_ForbidenInfo
{
	public int idx;
	public byte flag;							//FORBIDEN_FLAG麦序操作类型
};

class ResultData_UpBlackList
{
	public int idx;
	public byte flag;							//ADD_SUB_FLAG增加或删除标志
};

class ResultData_UpMgrList
{
	public int idx;
	public byte flag;							//ADD_SUB_FLAG增加或删除标志
	public byte identity;						//USER_IDENTITY用户属性
};

class ResultData_RoomSimpleInfo
{
	public int roomid;			//房间ID
	public String roomname;//char roomname[50];	//房间名
	public byte pwdflag;		//房间有无密码 0-无 1-有
	public short clientnum;	//在线人数
	public short boy;
	public short girl;
	public int isvip;
	public int	isclose;
	public int	state;
	public int	maxplayer;
	public short re_player_num;
};
class ResultData_RoomSimpleInfoList
{
	public short roomcount;	//房间个数
	public ResultData_RoomSimpleInfo[] RoomSimpleInfoList;
};

class ResultData_Pack
{
	public int global_id;
	public short goods_id;
	public byte status;
	public int day_num;
	public String buy_day;//char buy_day[ MAXLEN_TIMESTR ];
	public byte is_use;
	public int count;
};

class ResultData_Gift
{
	public int gift_type; //礼物类型
	public int recv_idx;
	public String recv_id;//char recv_id[ MAXLEN_NICKNAME ];
	public int recv_num; //接受的礼物数
};
class ResultData_GiftList
{
	public short gift_count;
	public String curtime;//char curtime[MAXLEN_TIMESTR];
	public ResultData_Gift[] list ;//list[50];
};

class ResultData_SongInfo
{
	public int m_musicid;
	public short m_topscores; //最高分
	public int m_topidx; //最高分idx
	public String m_topname;//char m_topname[ MAXLEN_NICKNAME ];
	public short m_dayclicks; //日点击率
	public short m_weekclicks; //周点击率
	public short m_monthclicks; //月点击率
	public int m_totalclicks; //总的点击率
};

class ResultData_AvServer 
{
	public String ip;//char ip[ MAXLEN_IP ];
	public short port;
};

class ResultData_OffLineMsg
{
	public int idx;
	public String buy_day;//char buy_day[ MAXLEN_TIMESTR ];
	public String buy_msg;//char buy_msg[ MAXLEN_BUY_MSG ];
};

class ResultData_RoomSort
{
	public int idx;
	public int sort;
};

class ResultData_IncomeDate
{
	public int income;
	public String date;//char date[MAXLEN_TIMESTR];
};

class ResultData_AvChange
{
	public short port_telcom;
	public short port_netcom;

	public byte telcom_count;
	public byte netcom_count;

	public String[] telcom;//char telcom[ MAXLEN_ROOM_AVIPCOUNT ][ MAXLEN_IP ];
	public String[] netcom;//char netcom[ MAXLEN_ROOM_AVIPCOUNT ][ MAXLEN_IP ];
};

class ResultData_GiftCount
{
	public int   idx;
	public int   giftid;
	public int   giftcount;
};

/*
 家族信息                                                             

#define FAMILY_NAME_MAXLEN 128
#define FAMILY_URL_MAXLEN 128
#define FAMILY_INTRUDUCE 256
#define FAMILY_MAX_ROOMS 128 //每个家族的最大房间数
*/
class ResultData_Family
{

	public int familyId; //家族id
	public int familyManageIdx; //族长idx
	public short familyState; //家族状态（0正常，1冻结，9删除）
	public short familySort; //家族顺序
	public String familyName;//char familyName[FAMILY_NAME_MAXLEN]; //家族名
	public String familyIntroduce;//char familyIntroduce[FAMILY_INTRUDUCE]; //家族介绍
	public String familyImage;//char familyImage[FAMILY_URL_MAXLEN]; // 家族海报
	public String familyLogo;//char familyLogo[FAMILY_URL_MAXLEN];//家族logo

	public int roomsize; //家族里的房间数量
	public int[] roomids;// roomids[128];//家族里的房间列表
};

class ResultData_LobbyOnline
{
	public int lobbyid;		//大厅ID
	public int onlinenum;		//大厅在线人数
};

class ResultData_LobbyOnlineList
{
	public short lobbycount;	//大厅个数
	public ResultData_LobbyOnline[] Lobbyonline;
};

class ResultData_Avatar
{
	public byte avaratid;
	public byte len;
	public short[] goods_id;//short goods_id[PART_LEN+LINK_LEN*10];
};
class ResultData_Avatar_Updata extends ResultData_Avatar
{
	public String[] count;//char count[PART_LEN+LINK_LEN][COUNT_MAX];
	public int[] global_id;//int global_id[PART_LEN+LINK_LEN*10];
};

class ResultData_Buy
{
	public short goods_id;
	public byte count;
	public String msg;//char msg[ MAXLEN_BUY_MSG ];
};

class ResultData_CreateRoom
{
	public String name;//char name[MAXLEN_TMPROOM_NAME];
	public byte is_pwd;
	public String pwd;//char pwd[MAXLEN_TMPROOM_PWD];
	public byte is_score;
	public byte is_ballot;
	public byte count;
};

//class ResultData_GroupInfo
//{
//	public short group_id;
//	public String group_name;//char group_name[ MAXLEN_ROOM_TYPENAME ];
//	public short disable; //0：启用 1：禁用
//};

class ResultData_PartInfo
{
	public int idx; 
	public byte sex; 
	public short age;
	public String province;//char province[MAXLEN_PROVINCE];
	public int plen;
	public String city;//char city[MAXLEN_CITY];
	public int clen;
	public String nick;//char nick[MAXLEN_NICKNAME];
	public int nicklen; 
	public byte singlevel;
	public int gold; 
	public int silver;
	public int nexp;
};

//class ResultData_RoomInfo
//{
//	public int idx;						//房间idx
//	public int hallid;						//房间所在大厅的id
//	public String name;//char name[50];					//房间名
//	//char pwd[20];					//进入房间密码		del by jinguanfu 2010/4/8
//	public byte pwdflag;					//房间是否有密码 0-无 1-有 add by jinguanfu 2010/4/8
//	public int type;						//房间类型	0--百人房,1--六人房
//	public int state;						//房间状态,运行/冻结/停止
//	public int masterid;					//室主id
//	public String createdate;//char createdate[30];			//房间创建日期
//	public String sortflag;//char sortflag[20];				//排序标志(是否允许自由排麦)
//	public int maxusernum;					//房间最大在线人数
//	public String deadline;//char deadline[30];				//房间到期时间
//	public String ispassaudit;//char ispassaudit[2];			//申请会员是否需要审批
//	public String introduce;//char introduce[512];			//房间欢迎词
//	public String ip_1;//char ip_1[MAXLEN_IP];			//房间IP
//	public short port;						//房间连接端口
//	public String ip_2;//char ip_2[MAXLEN_IP];			//房间IP
//	public String content;//char content[1024];				//房间公告
//	public String updatedate;//char updatedate[30];			//更新时间
//	public String logo;//char logo[MAXLEN_ROOM_LOGO];
//	public int chatflag;					//允许房间公聊标识
//	public int inoutflag;					//显示房间进出信息标识
//	public int useronlyflag;				//房间私有标志位（锁定）//add by jinguanfu 2010/4/22
//	public int closeflag;					//房间关闭标志位		//add by jinguanfu 2010/4/22
//};



//class ResultData_SendFlowerInfo
//{
//	public int s_idx;						//送礼者IDX
//	public int r_idx;						//收礼者IDX
//	public int cate_idx;				    //礼物类别
//	public int number;			            //礼物总数
//	public int leavings;					//礼物剩余数
//	public int time;
//	public int once_num;					//每次送礼的数量
//};

class ResultData_UserSpeakerInfo
{
	int time;			// 发送时间
	int fromidx;		//发送者idx;
	String chat;	// 小喇叭内容
    int roomId;//房间ID
    String roomName;//房间名字 ktv店名+包厢名 举例：小和山西和店A31小包
};

class ResultData_GroupRoomInfo{
    short grpup_id;//分组ID
    short roomnum;//房间数量
    int[] roomlist;//房间ID列表
};
