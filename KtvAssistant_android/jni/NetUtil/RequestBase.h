/*************************************
*请求处理类 头文件
*Create: 2009/12/21   jinguanfu 
*************************************/
#ifndef REQUESTBASE_H
#define REQUESTBASE_H

#include "ResponseBase.h"

#ifdef JNINETUTIL_EXPORTS
	#define CLIENT9158API
#else
	#ifdef NETUTIL_EXPORTS
	#define CLIENT9158API __declspec(dllexport)
	#else
	#define CLIENT9158API __declspec(dllimport)
	#endif
#endif

//DWORD WINAPI RunClient9158ThreadFunc(LPVOID lpParam);
//CLIENT9158API
class  RequestBase
{	
public:
	RequestBase(void):m_bInit(false){};
	~RequestBase(void){};
	/************************************************************************/
	/*			add by jinguanfu 2010/3/2 <begin> 回调API改造成接口			*/
	//启动客户端
	int StartClient();
	//停止客户端
	void StopClient(bool finish=true);
	//取得Token
	const char* GetToken();
	//取得Token长度
	unsigned int GetTokenlen();

	//认证服务器是否连接
	bool IsConnectAuth();
	//大厅是否连接
	bool IsConnectLobby();
	//房间是否连接
	bool IsConnectRoom();
	//大厅是否token
	bool IsTokenLoginLobby();
	//房间是否token
	bool IsTokenLoginRoom();

	int ConnectAuth(const char* host,short port,int timeout = 60);
	int DisConnectAuth();
	int ConnectLobby(const char* host,short port,int timeout = 60);
	int DisConnectLobby();
	int ConnectRoom(const char* host,short port,int type,int timeout = 60);
	int DisConnectRoom();

	int IdPassAuth(const char* id,const char* passwd,int again=0);
	int TouristAuth(int again=0);
	//Room API
	//验证token
	int ConfirmToken();
	//进入房间
	int EnterRoom(int cseq,int roomid, const char* passwd, short type);		
	//取得房间当前运行信息
	int GetRoomAllInfomation(int cseq);										
	//取麦序列表
	int GetWaitMicList();
	//排麦
	int RoomWaitMic( int musicid, short bk, char level, const char * name, char speed );				
	//取消排麦
	int RoomCancleWaitMic( int idx );											
	//提升麦序
	int RoomUpWaitMic(char tag, int idx, char flag);	
	//flag==>MIC_UP_DOWN_FLAG
	//下降麦序
	int RoomDownWaitMic(char tag, int idx, char flag);
	//flag==>MIC_UP_DOWN_FLAG
	//上麦准备ok
	//int RoomOnMicReadyOK( short music_len );
	//准备下麦
	//int RoomOffMicReady();
	//把指定用户提出房间
	int RoomKickSomeOne(int idx);
	//禁言某人（或取消禁言）
	int RoomForbidenSomeOne(int idx, char flag);
	//上主持人麦
	int RoomOnvjMic();												
	//下主持人麦
	int RoomOffvjMic();												
	//把主持人麦给某人
	int RoomGivOnevjMic(int idx);									
	// 是否同意上主持人麦 add by cx 10-6-2
	int ApplyGetVJmic( int idx, char flag );
	//让主持人下麦
	int RoomGiveOffvjMic(int idx);									
	//让普通麦上麦用户下麦
	int RoomGiveOffMic(int idx);										
	//flag==>ADD_SUB_FLAG
	//跟新房间黑名单列表(单条添加和删除)
	int RoomUpdateBlackList(int cseq,int idx, char flag);	
	//flag==>ADD_SUB_FLAG
	//跟新管理员列表(单条添加和删除)
	int RoomUpdateMgrList(int idx, char flag, char identity);

	//登录大厅
	/*
	@param loginway : 
		0：PC端，1：ANDROID，2：IOS
	*/
	int LoginLobby( const char * mac,int loginway);
	//获取大厅列表
	int GetChannellist( int version, const char * mac );	
	//登录大厅(重连)
	int LoginLobbyAuto( const char * mac, int roomid );
	//获取用户资料 参数一用户idx列表,参数二数量
	int GetUserInfo(int idxl[],int count);
	//获取好友列表
	int GetFriendListWithoutUserInfo();	
	//获取黑名单列表
	int GetBlackListWithoutUserInfo();	
	//添加好友
	int AddFriend(int idx);				
	//添加黑名单
	int AddBlack(int cseq,int idx);				
	//删除好友
	int DelFriend(int cseq,int idx);				
	//删除黑名单
	int DelBlack(int cseq,int idx);				
	//*********************************room manage api*********************************
	//获取当前大厅的房间列表
	int GetRoomList(int cseq);
	int GetRoomListEx(int cseq, int type);
	//获取房间信息,可以是一组房间
	int GetRoomInfo(int cseq,int roomcount, int *roomlist);
	int GetTmpRoomInfo( int idx );
	//*********************************************************************************
	//用户创建头像信息
	int CreateAvatar( char * nickname, ResultData_Avatar_Updata * avarat );
	//avatarname长度限制20,partlen,decoration大小限制为10
	int ChangeAvatar( ResultData_Avatar_Updata * avarat );
	//修改昵称
	int ChangeNickName( int idx, const char * name);
	//给好友送花
	int SendFlower(int s_idx,int r_idx,int cate_id,int count);
	//发送大喇叭
	int BroadAll(const char *content);
	//发送小喇叭
	int BroadLobby(int lobbyidx,const char *content);				
	//发送房间临时公告
	int SendRoomNotice(const char *content);
	//取得指定大厅信息
	int GetLobbyInfo(int cseq,int lobbyid);
	//邀请用户进入表演麦麦序
	int InviteMic(int useridx);
	/*			add by jinguanfu 2010/3/2 <end> 回调API改造成接口			*/
	/************************************************************************/
	/************************************************************************/
	/*			add by jinguanfu 2009/12/18 <begin> 聊天、喇叭功能			*/
	//发送用户私聊
	int SendChatToUser(int to_idx, const char *content);	
	//发送公聊（俱乐部内部）
	int SendChatToClub(const char *content);	
	//发送公聊（房间内）
	int SendChatToRoom(const char* content);	
	//发送大喇叭（全区）
	int SendSpeakerToAll(const char* content);
	//发送小喇叭（大厅） content 需要以0结尾
	int SendSpeakerToLobby(int lobbyid, const char* content);
	/*			add by jinguanfu 2009/12/28 <end> 聊天、喇叭功能			*/
	/************************************************************************/
	/************************************************************************/
	/*			add by jinguanfu 2009/12/28 <begin> 好友操作				*/
	//查询好友位置
	int QueryFriendLocation(int cseq,int friend_idx);
	/*			add by jinguanfu 2009/12/28 <end> 好友操作					*/
	/************************************************************************/
	/************************************************************************/
	/*			add by jinguanfu 2009/12/28 <begin> 房间管理接口			*/
	//请求取得待审核房间会员列表
	int GetRoomApplyList (int cseq);
	//用户申请房间会员
	int ApplyRoomMember(int cseq);
	//审核会员加入申请
	int AuditAddApply (int cseq,int idx,char type);
	//取房间会员列表
	int GetRoomMemberList (int cseq);
	//解除房间会员资格
	int UnchainMember (int cseq,int idx);
	//赋予用户房间主持助理权限
	int SetRoomPesideAssistant(int cseq,int idx);
	//赋予用户房间主持人权限
	int SetRoomPeside (int cseq, int idx);
	//赋予用户房间副室主权限
	int SetRoomSubHost(int cseq, int idx);
	//设置为房间会员（由管理员变为会员）	//add by jinguanfu 2010/5/13
	int SetRoomMember(int cseq, int idx);
	//取得房间黑名单
	int GetBlackList (int cseq);
	//黑名单用户添加/删除
	int AddDelBlack (int idx ,char type);
	//查看房间设置面板
	int GetRoomPanelInfo (int cseq);
	//设置房间密码
	int SetRoomPassword(int cseq,const char *password);
	//设置房间开启/关闭(上锁)
	int SetRoomSwitch(int cseq,char roomflag);
	//设置房间仅对会员开放
	int SetRoomPrivate(int cseq,char accessflag);
	//用户进出信息开启/关闭
	int SetUserInOutSwitch(int cseq,char inoutflag);
	//设置自由排麦开启/关闭
	int SetGetMicSwitch(int cseq,char getmicflag);
	//设置自由上麦开启/关闭
	int SetOnMicSwitch(int cseq,char onmicflag);
	//房间名称修改
	int ModifyRoomName(int cseq,const char* roomname);
	//设置房间公告
	int SetRoomAffiche (int cseq,const char* content);
	//设置允许房间公聊
	int AllowRoomAllChat (int cseq , char chatflag);
	//设置房间欢迎词
	int SetRoomSalutatory (int cseq,const char* content);
	//设置房间logo
	int SetRoomLogo(int cseq, const char* path);
	/*			add by jinguanfu 2009/12/28 <end> 房间管理接口				*/
	/************************************************************************/
	/************************************************************************/
	/*			add by jinguanfu 2009/12/28 <begin> 角色信息				*/
	//发送取得自己的角色信息请求，金币，银币，升级所需经验
	int GetPartInfo(int cseq,int idx);
	/*			add by jinguanfu 2009/12/28 <end> 角色信息					*/
	/************************************************************************/
	//取得指定大厅的房间列表
	int GetRoomList(int cseq,int lobbyid);
	/************************************************************************/
	/*			add by jinguanfu 2010/1/14 <begin> 							*/
	//离开房间（切换房间时使用）
	int LeaveRoom();
	/*			add by jinguanfu 2010/1/14 <end> 							*/
	/************************************************************************/
	/************************************************************************/
	/*			add by jinguanfu 2010/1/20 <begin> 							*/
	//是否同意添加好友
	//参数说明：int		idx		请求添加好友的用户
	//			char	flag	0--同意 1--不同意
	int ApplyAddFriend(int idx, char flag);
	/*add by jinguanfu 2010/1/27 删除角色信息 ********/
	/*参数说明：int		idx		用户idx	*/
	//int DeleteAvarta(int idx);
	//设置回调类对象
	void SetResponseHandle(ResponseBase* pRb);

	//add by jinguanfu 2010/4/8 取得简单房间信息
	int GetRoomSimpleInfo();
	int GetRoomSimpleInfoEx( int type );
	//add by jinguanfu 2010/4/11 用户退出房间会员
	int ExitRoomMember();	
	//add by jinguanfu 2010/4/15 取得大厅在线人数
	int GetLobbyOnlineNum();
	//add by cx 10-7-6 购买
	int BuyPack( ResultData_Buy * pPack, int count, int type, int idx );
	int GetPack();
	int DelPack( int cseq, int * idx, int count );
	int UsePack( int idx );
	int CreateRoom( ResultData_CreateRoom * room );
	//烟花
	int SendFireworks( int cate_idx, int count );

	//////////////////////////////////////////////////////////////////////////

	//取得大厅礼物排行榜信息
	int GetGiftTopList();
	//取得在线时间
	int GetOnlineTime( int idx );
	//取得新手任务
	int GetNewUserTask();
	//提交新手任务
	int TaskStep( short step );
	int TaskGift( short step );
	//获取礼物滚动广播
	int GetGiftInfo();
	//获取系统文字公告
	int GetSystemBroad();
	//获取印章信息
	int GetUserSeel( const int * idx, int count );
	//发送用户pc配置
	int SendSysteInfo( char * data );
	//获取分组信息
	int GetGroupInfo();
	//获取组里面的房间列表
	int GetRoomListInGroup( int group_idx );
    int GetRoomListInGroupBatch(short* groupId,int count);
	//获取歌曲列表
	int GetSongInfoList();
	//获取音视频服务器列表
	int GetAvServerList( short net_type=2 );
	//获取离线消息
	int GetOffLineMsg();
	//获取分组置顶信息
	int GetRoomSortInGroup( short group_idx );
	//获取小喇叭信息
	int GetUserSpeakerInfo();
	// 获取家族信息
	int GetFamilyInfo();

	//////////////////////////////////////////////////////////////////////////

	//客户端房间打分
	//const char *scoreinfo -- 打分信息结构体指针
	//int length--打分信息结构体长度
	int RoomScore(const char *scoreinfo);
	//显示房间可领取收益
	int ShowRoomIncome();
	//显示收益已领取记录
	int ShowRoomGetIncome();
	//室主领取房间内收益
	int GetRoomIncome();
	//发送用户pc概况
	int SendPcData( char * data );
	//邀请用户加入房间会员
	int InviteJoinRoom( int idx, int wseq );
	//邀请用户加入房间会员
	int ReInviteJoinRoom( char type );
	//取得当日礼物赠送记录
	int GetGiftSend();
	//取得当日礼物接收记录
	int GetGiftRecv();
	//封mac
	int KickUserByMac( int idx );
	//封ip
	int KickUserByIp( int idx );
	//设置在麦剩余时间
	int SetOnMicTemainTime( int idx, int time );
	//设置在歌曲剩余时间
	int SendSongRemnantsTime( short time );

	/************************************************************************/
	/* 私麦接口                                                             */
	/************************************************************************/
	//上私麦, mictype为私麦的类型(vip以上可观看或需要对方确认可观看)
	int OpenPrivateMic(short mictype);
	//下私麦
	int ClosePrivateMic();
	//被抱下私麦
	int ClosedPrivateMic(int bidx); //bidx=私麦持有者
	//请求观看idx的私麦
	int RequestWatchPrivateMic(int idx); //idx=私麦持有者
	//回复观看私麦,只有同意才调用这个接口
	int ResponseWatchPrivateMic(int allow, int idx);//idx=观看者,0->同意，-1->拒绝
	//邀请其他人看私麦
	int InviteWatchPrivateMic(int num, int idxs[]);
	//回复邀请看私麦
	int ResponseInviteWatchPrivateMic(int allow, int idx);
	//房间私麦管理
	int OpenRoomPrivateMicFlag(char flag);//0：不开放 1：开放

	// 皇冠踢人
	int CrownKickoutSomeOne(int idx);
	// 室主对用户的IP进行禁用
	int ForbidUserIp(int idx);

	// 发送句评分
	int SingerScores( int idx, short scores[], int count );
	// 要求上传歌曲
	int UpdateSong( int idx );

	//请求获得用户其他信息
	//int SendUserSingInfo(int idx);
    
    //手机KTV用户聊天
    /*
     @param toIdx:聊天对象，如果是0，则为公聊
     @param contet:聊天信息 不要超过4096,GBK字符串
     
     @return:0,成功；-1,失败
     */
    int SendChatKTV(int toIdx,const char* content);
    
    //手机KTV用户在大厅送礼物
    /*
     @param toIdx:送礼物的对象IDX
     @param giftId:礼物ID
     @param number:礼物数量
     @param roomId:房间ID，0代表大厅
     
     @return:0,成功；-1,失败
     */
    int SendGiftInLobby(int toIdx,int giftId,int number,int roomId);
	/*
	请求发喇叭的用户信息
	@param idx:请求的idx数组
	@param count:请求的idx的数量，不能超过200

	@return:0,成功；-1,失败
	*/
	int GetUserInfoForSpeaker(int idx[],int count);
    /*
	请求用户信息
	@param data:请求的idx数组
	@param count:请求的idx的数量 ,不能超过200

	@return:0,成功；-1,失败
	*/
	int GetUserInfoWithVersion(RequestData_IdxWithVersion data[],int count);


	//add by glp 2015-5-7
	/*
	请求发红包
	@param hbtype : 红包类型 0-个人  1房间手气 2-房间均分  3-大厅手气 4-大厅均分
	@param number : 红包数量
	@param gold : 红包总金额
	@param mark : 红包备注
	@param extradata : 如果是个人红包，则代表接受者的IDX；如果是房间红包或是大厅，则是0

	@return : -1失败；0成功
	*/
	int RequestSendHB(char hbtype,int number,int gold,const char* mark,int extradata);

	/*
	请求查询红包状态，是否可抢
	@param hbid : 红包ID

	@return : -1失败；0成功
	*/
	int RequestHBStatus(const char* hbid);
	/*
	请求抢红包
	@param hbid : 红包ID

	@return : -1失败；0成功
	*/
	int RequestVieHB(const char* hbid);

	/*
	查看红包领取详细情况
	@param hbid : 红包ID

	@return : -1失败；0成功
	*/
	int RequestHBDetail(const char* hbid);

	/*
	请求获取包厢红包信息

	@param roomid : 当前所在的包厢

	@return : -1失败；0成功
	*/
	int RequestCurrentRoomHB(int roomid);
private:
    bool m_bInit;
};

#endif

