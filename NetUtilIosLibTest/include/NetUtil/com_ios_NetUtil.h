//
//  com_ios_NetUtil.h
//  NetUtil
//
//  Created by guliping on 14-4-18.
//  Copyright (c) 2014年 天格科技. All rights reserved.
//

#ifndef NetUtil_com_ios_NetUtil_h
#define NetUtil_com_ios_NetUtil_h

#include "ResultData.h"
#include "ResponseBase.h"
#include "DataType.h"
#include "HttpDownloadType.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IOSEXPORT
    /*
     @注： 
        所有字符串从接口获得的都是GBK字符串，如果在ios中使用的时候，需要转换为UTF8，已提供相应接口，
        所有通过参数传递给接口的字符串，必须保证是GBK编码的，如果ios调用的话，需要进行转换，接口也已经提供。
     */
    
    //启动客户端
    IOSEXPORT int StartClient();
    //停止客户端
    IOSEXPORT void StopClient();
    //取得Token
    IOSEXPORT const char* GetToken();
    //取得Token长度
    IOSEXPORT unsigned int GetTokenlen();

    //认证服务器是否连接
    IOSEXPORT bool IsConnectAuth();
    //大厅是否连接
    IOSEXPORT bool IsConnectLobby();
    //房间是否连接
    IOSEXPORT bool IsConnectRoom();
    //大厅是否token
    IOSEXPORT bool IsTokenLoginLobby();
    //房间是否token
    IOSEXPORT bool IsTokenLoginRoom();

    IOSEXPORT int ConnectAuth(const char* host,short port,int timeout = 60);
    IOSEXPORT int DisConnectAuth();
    IOSEXPORT int ConnectLobby(const char* host,short port,int timeout = 60);
    IOSEXPORT int DisConnectLobby();
    IOSEXPORT int ConnectRoom(const char* host,short port,int type,int timeout = 60);
    IOSEXPORT int DisConnectRoom();

    IOSEXPORT int IdPassAuth(const char* id,const char* passwd);
    IOSEXPORT int TouristAuth();
    //Room API
    //验证token
    IOSEXPORT int ConfirmToken();
    //进入房间
    IOSEXPORT int EnterRoom(int cseq,int roomid, const char* passwd, short type);
    //取得房间当前运行信息
    IOSEXPORT int GetRoomAllInfomation(int cseq);
    //取麦序列表
    IOSEXPORT int GetWaitMicList();
    //排麦
    IOSEXPORT int RoomWaitMic( int musicid, short bk, char level, const char * name, char speed );
    //取消排麦
    IOSEXPORT int RoomCancleWaitMic( int idx );
    //提升麦序
    IOSEXPORT int RoomUpWaitMic(char tag, int idx, char flag);
    //flag==>MIC_UP_DOWN_FLAG
    //下降麦序
    IOSEXPORT int RoomDownWaitMic(char tag, int idx, char flag);
    //flag==>MIC_UP_DOWN_FLAG
    //上麦准备ok
    //int RoomOnMicReadyOK( short music_len );
    //准备下麦
    //int RoomOffMicReady();
    //把指定用户提出房间
    IOSEXPORT int RoomKickSomeOne(int idx);
    //禁言某人（或取消禁言）
    IOSEXPORT int RoomForbidenSomeOne(int idx, char flag);
    //上主持人麦
    IOSEXPORT int RoomOnvjMic();
    //下主持人麦
    IOSEXPORT int RoomOffvjMic();
    //把主持人麦给某人
    IOSEXPORT int RoomGivOnevjMic(int idx);
    // 是否同意上主持人麦 add by cx 10-6-2
    IOSEXPORT int ApplyGetVJmic( int idx, char flag );
    //让主持人下麦
    IOSEXPORT int RoomGiveOffvjMic(int idx);
    //让普通麦上麦用户下麦
    IOSEXPORT int RoomGiveOffMic(int idx);
    //flag==>ADD_SUB_FLAG
    //跟新房间黑名单列表(单条添加和删除)
    IOSEXPORT int RoomUpdateBlackList(int cseq,int idx, char flag);
    //flag==>ADD_SUB_FLAG
    //跟新管理员列表(单条添加和删除)
    IOSEXPORT int RoomUpdateMgrList(int idx, char flag, char identity);

    //登录大厅
    /*
     @param mac:一般为设备的MAC地址，但是由于IOS，苹果禁止获取MAC地址，所以在IOS设备上使用的是UIDevice Unicode的字符串
     @param loginway:登录的方式，0：PC端，1：ANDROID，2：IOS
     
     @return:1，成功；0，失败。
     */
    IOSEXPORT int LoginLobby( const char * mac,int loginway);
    //登录大厅(重连)
    IOSEXPORT int LoginLobbyAuto( const char * mac, int roomid );
    //获取用户资料 参数一用户idx列表,参数二数量
    IOSEXPORT int GetUserInfo_AII(int idxl[],int count);
    //获取好友列表
    IOSEXPORT int GetFriendListWithoutUserInfo();
    //获取黑名单列表
    IOSEXPORT int GetBlackListWithoutUserInfo();
    //添加好友
    IOSEXPORT int AddFriend(int idx);
    //添加黑名单
    IOSEXPORT int AddBlack(int cseq,int idx);
    //删除好友
    IOSEXPORT int DelFriend(int cseq,int idx);
    //删除黑名单
    IOSEXPORT int DelBlack(int cseq,int idx);
    //获取通道列表
    IOSEXPORT int GetChannellist( int version, const char * mac );
    //*********************************room manage api*********************************
    //获取当前大厅的房间列表
    IOSEXPORT int GetRoomList(int cseq);
    IOSEXPORT int GetRoomListEx(int cseq, int type);
    //获取房间信息,可以是一组房间
    IOSEXPORT int GetRoomInfo(int cseq,int roomcount, int *roomlist);
    IOSEXPORT int GetTmpRoomInfo( int idx );
    //*********************************************************************************
    //用户创建头像信息
    IOSEXPORT int CreateAvatar( char * nickname, ResultData_Avatar_Updata * avarat );
    //avatarname长度限制20,partlen,decoration大小限制为10
    IOSEXPORT int ChangeAvatar( ResultData_Avatar_Updata * avarat );
    //修改昵称
    IOSEXPORT int ChangeNickName( int idx, char * name, int len );
    //给好友送花
    IOSEXPORT int SendFlower(int s_idx,int r_idx,int cate_id,int number);
    //发送大喇叭
    IOSEXPORT int BroadAll(const char *content);
    //发送小喇叭
    IOSEXPORT int BroadLobby(int lobbyidx,const char *content);
    //发送房间临时公告
    IOSEXPORT int SendRoomNotice(const char *content);
    //取得指定大厅信息
    IOSEXPORT int GetLobbyInfo(int cseq,int lobbyid);
    //邀请用户进入表演麦麦序
    IOSEXPORT int InviteMic(int useridx);
    /*			add by jinguanfu 2010/3/2 <end> 回调API改造成接口			*/
    /*			add by jinguanfu 2009/12/18 <begin> 聊天、喇叭功能			*/
    //发送用户私聊
    IOSEXPORT int SendChatToUser(int to_idx, const char *content, int contentlen);
    //发送公聊（俱乐部内部）
    IOSEXPORT int SendChatToClub(const char *content,int contentlen);
    //发送公聊（房间内）
    IOSEXPORT int SendChatToRoom(const char* content,int contentlen);
    //发送大喇叭（全区）
    IOSEXPORT int SendSpeakerToAll(const char* msg,int msglen);
    //发送小喇叭（大厅）
    IOSEXPORT int SendSpeakerToLobby(int lobbyid, const char* msg,int msglen);
    //发送房间临时公告
    IOSEXPORT int SendRoomNotice_SI(const char *content,int contentlen);
    /*			add by jinguanfu 2009/12/28 <begin> 好友操作				*/
    //查询好友位置
    IOSEXPORT int QueryFriendLocation(int cseq,int friend_idx);
    /*			add by jinguanfu 2009/12/28 <begin> 房间管理接口			*/
    //请求取得待审核房间会员列表
    IOSEXPORT int GetRoomApplyList (int cseq);
    //用户申请房间会员
    IOSEXPORT int ApplyRoomMember(int cseq);
    //审核会员加入申请
    IOSEXPORT int AuditAddApply (int cseq,int idx,char type);
    //取房间会员列表
    IOSEXPORT int GetRoomMemberList (int cseq);
    //解除房间会员资格
    IOSEXPORT int UnchainMember (int cseq,int idx);
    //赋予用户房间主持助理权限
    IOSEXPORT int SetRoomPesideAssistant(int cseq,int idx);
    //赋予用户房间主持人权限
    IOSEXPORT int SetRoomPeside (int cseq, int idx);
    //赋予用户房间副室主权限
    IOSEXPORT int SetRoomSubHost(int cseq, int idx);
    //设置为房间会员（由管理员变为会员）	//add by jinguanfu 2010/5/13
    IOSEXPORT int SetRoomMember(int cseq, int idx);
    //取得房间黑名单
    IOSEXPORT int GetBlackList (int cseq);
    //黑名单用户添加/删除
    IOSEXPORT int AddDelBlack (int idx ,char type);
    //查看房间设置面板
    IOSEXPORT int GetRoomPanelInfo (int cseq);
    //设置房间密码
    IOSEXPORT int SetRoomPassword(int cseq,const char *password , int passwordlen);
    //设置房间开启/关闭(上锁)
    IOSEXPORT int SetRoomSwitch(int cseq,char roomflag);
    //设置房间仅对会员开放
    IOSEXPORT int SetRoomPrivate(int cseq,char accessflag);
    //用户进出信息开启/关闭
    IOSEXPORT int SetUserInOutSwitch(int cseq,char inoutflag);
    //设置自由排麦开启/关闭
    IOSEXPORT int SetGetMicSwitch(int cseq,char getmicflag);
    //设置自由上麦开启/关闭
    IOSEXPORT int SetOnMicSwitch(int cseq,char onmicflag);
    //房间名称修改
    IOSEXPORT int ModifyRoomName(int cseq,const char* roomname, int namelen);
    //设置房间公告
    IOSEXPORT int SetRoomAffiche (int cseq,const char* content, int contentlen);
    //设置允许房间公聊
    IOSEXPORT int AllowRoomAllChat (int cseq , char chatflag);
    //设置房间欢迎词
    IOSEXPORT int SetRoomSalutatory (int cseq,const char* content , int contentlen);
    //设置房间logo
    IOSEXPORT int SetRoomLogo(int cseq, const char* path, int pathlen);
    /*			add by jinguanfu 2009/12/28 <begin> 角色信息				*/
    //发送取得角色信息请求
    IOSEXPORT int GetPartInfo(int cseq,int idx);
    //取得指定大厅的房间列表
    IOSEXPORT int GetRoomList_II(int cseq,int lobbyid);
    /************************************************************************/
    /*			add by jinguanfu 2010/1/14 <begin> 							*/
    //离开房间（切换房间时使用）
    IOSEXPORT int LeaveRoom();
    /*			add by jinguanfu 2010/1/20 <begin> 							*/
    //是否同意添加好友
    //参数说明：int		idx		请求添加好友的用户
    //			char	flag	0--同意 1--不同意
    IOSEXPORT int ApplyAddFriend(int idx, char flag);
    /*add by jinguanfu 2010/1/27 删除角色信息 ********/
    /*参数说明：int		idx		用户idx	*/
    //int DeleteAvarta(int idx);
    //设置回调类对象
    IOSEXPORT void SetResponseHandle(ResponseBase* pRb);

    //add by jinguanfu 2010/4/8 取得简单房间信息
    IOSEXPORT int GetRoomSimpleInfo();
    IOSEXPORT int GetRoomSimpleInfoEx( int type );
    //add by jinguanfu 2010/4/11 用户退出房间会员
    IOSEXPORT int ExitRoomMember();
    //add by jinguanfu 2010/4/15 取得大厅在线人数
    IOSEXPORT int GetLobbyOnlineNum();
    //add by cx 10-7-6 购买
    IOSEXPORT int BuyPack( ResultData_Buy * pPack, int count, int type, int idx );
    IOSEXPORT int GetPack();
    IOSEXPORT int DelPack( int cseq, int * idx, int count );
    IOSEXPORT int UsePack( int idx );
    IOSEXPORT int CreateRoom( ResultData_CreateRoom * room );
    //烟花
    IOSEXPORT int SendFireworks( int cate_idx, int count );

    //////////////////////////////////////////////////////////////////////////

    //取得大厅礼物排行榜信息
    IOSEXPORT int GetGiftTopList();
    //取得在线时间
    IOSEXPORT int GetOnlineTime( int idx );
    //取得新手任务
    IOSEXPORT int GetNewUserTask();
    //提交新手任务
    IOSEXPORT int TaskStep( short step );
    IOSEXPORT int TaskGift( short step );
    //获取礼物滚动广播
    IOSEXPORT int GetGiftInfo();
    //获取系统文字公告
    IOSEXPORT int GetSystemBroad();
    //获取印章信息
    IOSEXPORT int GetUserSeel( int * idx, int count );
    //发送用户pc配置
    IOSEXPORT int SendSysteInfo( char * data, int len );
    //获取分组信息
    IOSEXPORT int GetGroupInfo();
    //获取组里面的房间列表
    IOSEXPORT int GetRoomListInGroup( int group_idx );
    IOSEXPORT int GetRoomListInGroupBatch(short* groupId,short count);
    //获取歌曲列表
    IOSEXPORT int GetSongInfoList();
    //获取音视频服务器列表
    IOSEXPORT int GetAvServerList( short net_type=2 );
    //获取离线消息
    IOSEXPORT int GetOffLineMsg();
    //获取分组置顶信息
    IOSEXPORT int GetRoomSortInGroup( short group_idx );
    //获取小喇叭信息
    IOSEXPORT int GetUserSpeakerInfo();
    // 获取家族信息
    IOSEXPORT int GetFamilyInfo();

    //////////////////////////////////////////////////////////////////////////

    //客户端房间打分
    //const char *scoreinfo -- 打分信息结构体指针
    //int length--打分信息结构体长度
    IOSEXPORT int RoomScore(const char *scoreinfo,int length);
    //显示房间可领取收益
    IOSEXPORT int ShowRoomIncome();
    //显示收益已领取记录
    IOSEXPORT int ShowRoomGetIncome();
    //室主领取房间内收益
    IOSEXPORT int GetRoomIncome();
    //发送用户pc概况
    IOSEXPORT int SendPcData( char * data, int len );
    //邀请用户加入房间会员
    IOSEXPORT int InviteJoinRoom( int idx, int wseq );
    //邀请用户加入房间会员
    IOSEXPORT int ReInviteJoinRoom( char type );
    //取得当日礼物赠送记录
    IOSEXPORT int GetGiftSend();
    //取得当日礼物接收记录
    IOSEXPORT int GetGiftRecv();
    //封mac
    IOSEXPORT int KickUserByMac( int idx );
    //封ip
    IOSEXPORT int KickUserByIp( int idx );
    //设置在麦剩余时间
    IOSEXPORT int SetOnMicTemainTime( int idx, int time );
    //设置在歌曲剩余时间
    IOSEXPORT int SendSongRemnantsTime( short time );

    /* 私麦接口                                                             */
    //上私麦, mictype为私麦的类型(vip以上可观看或需要对方确认可观看)
    IOSEXPORT int OpenPrivateMic(short mictype);
    //下私麦
    IOSEXPORT int ClosePrivateMic();
    //被抱下私麦
    IOSEXPORT int ClosedPrivateMic(int bidx); //bidx=私麦持有者
    //请求观看idx的私麦
    IOSEXPORT int RequestWatchPrivateMic(int idx); //idx=私麦持有者
    //回复观看私麦,只有同意才调用这个接口
    IOSEXPORT int ResponseWatchPrivateMic(int allow, int idx);//idx=观看者,0->同意，-1->拒绝
    //邀请其他人看私麦
    IOSEXPORT int InviteWatchPrivateMic(int num, int idxs[]);
    //回复邀请看私麦
    IOSEXPORT int ResponseInviteWatchPrivateMic(int allow, int idx);
    //房间私麦管理
    IOSEXPORT int OpenRoomPrivateMicFlag(char flag);//0：不开放 1：开放

    // 皇冠踢人
    IOSEXPORT int CrownKickoutSomeOne(int idx);
    // 室主对用户的IP进行禁用
    IOSEXPORT int ForbidUserIp(int idx);

    // 发送句评分
    IOSEXPORT int SingerScores( int idx, short scores[], int count );
    // 要求上传歌曲
    IOSEXPORT int UpdateSong( int idx );
    
    //2014-09-26 by glp
    /*
     获取发喇叭的用户信息
     */
    IOSEXPORT int GetUserInfoForSpeaker(int idx[],int count);
    /*
     获取用户信息夹带版本ID
     */
    IOSEXPORT int GetUserInfoWithVersion(RequestData_IdxWithVersion data[],int count);
    //上面接口都是0成功，-1失败
        
    //本地数据库 by glp 2014-6-18
    /*
     初始化下载URL地址(包含一些资源地址等等)具体参照原PC星光客户端
     @param url[in]: URL路径
     @param forUse[in]: 使用的用途,下面一一说明,
        0 : 礼物更新地址
     
     @return：1，成功；0，失败
     
     @例:InitDownLoadUrl("http://res2.17xg.com/res/giftxml.html",0);
     
     @注：必须先调用InitDownLoadUrl再调用InitLoaclDatabaseDir.
     */
    IOSEXPORT int InitDownLoadUrl(const char* url,unsigned int forUse);
    //初始化本地数据目录,包含图片资源路径
    /*
    @param path[in]：指定保存数据文件的目录，注意是目录
     
    @return：1，成功；0，失败
    */
    IOSEXPORT int InitLoaclDatabaseDir(const char* path);
    //保存人物部分信息，
    /*
     @param partInfo[in]：具体的信息结构
     
     @return：1，成功；0，失败
     */
    IOSEXPORT int SavePartInfo(ResultData_PartInfo *partInfo);
    //保存自己的部分信息
    /*
     @param partInfo[in]：具体的信息结构
     
     @return：1，成功；0，失败
     */
    IOSEXPORT int SaveMyPartInfo(ResultData_PartInfo *partInfo);
    //获取自己的信息
    IOSEXPORT int GetMyInfo(USER_DATA_BASE *pMy);
    //获取到自己的数据
    IOSEXPORT int GetSelfData(ResultData_SelfData *pSD);
    //保存自己的数据
    IOSEXPORT int SaveSelfData(ResultData_SelfData *pSD);
    //获取本人的ID
    /*
     @return：本人ID
     */
    IOSEXPORT int GetMyIdx();
    //设置本人的ID
    IOSEXPORT int SaveMyIdx(int idx);
    //保存用户信息，
    /*
    @param userInfo[in]：用户基本信息
         
    @return：1，成功；0，失败
    */
    IOSEXPORT int SaveUserInfo(USER_DATA_BASE *userInfo);
    //提供给手机只是保存当前的在线用户的信息
    /*
     @param idx:IDX
     @param nickName:用户昵称
     
     @return：1，成功；0，失败
     */
    IOSEXPORT int SaveUserInfoSample(int idx,const char* nickName);
    //保存房间信息
    IOSEXPORT int SaveRoomInfo(ROOM_DATA* roomInfo);
    //获取房间分组版本号
    /*
     @param lobbyId[in]：大厅ID
     
     @return：分组版本号
     */
    IOSEXPORT int GetServerGroupVersion(int lobbyId);
    //保存房间分组版本号
    IOSEXPORT int SaveServerGroupVersion(int lobbyID,int groupVersionId);
    //清除本地所有房间缓存信息
    IOSEXPORT int ClearRoom();
    //清除本地所有分组缓存信息
    IOSEXPORT int ClearGroup();
    //增加一个分组信息
    /*
     @param groupId[in]：分组ID
     @param groupName[in]：分组名称
     @param disable[in]：是否禁用
     @param lobbyId[in]：大厅ID
     
     @return：1，成功；0，失败
     注：之后必须调用AddGroupDataEnd函数
     */
    IOSEXPORT int AddGroupData(int groupId,const char* groupName,bool disable,int lobbyId);
    //分组信息添加结束。
    IOSEXPORT int AddGroupDataEnd();
    //增加一个房间信息
    /*
     @param roomId[in]：房间ID
     @param groupId[in]：分组ID
     @param lobbyId[in]：大厅ID
     
     @return：1，成功；0，失败
     注：之后必须调用AddRoomDataEnd函数
     */
    IOSEXPORT int AddRoomData(int roomId,int groupId,int lobbyId);
    //房间信息添加结束。
    IOSEXPORT int AddRoomDataEnd();
    
    //字符集的转换 by glp 2014-9-19
    /*
     UTF8转GBK
     @param utf8[in]：UTF8字符串
     @parma gbk[out]：转换之后的GBK字符串存放位置
     @parma size[in out]：gbk buffer的大小相当于gbk[size]，最后返回gbk中实际字符串长度
     
     @return：1，成功；0，失败。
     */
    IOSEXPORT int Utf8ToGbk(const char* utf8,char* gbk,int& size);
    /*
     GBK转UTF8
     @param gbk[in]：GBK字符串
     @parma utf8[out]：转换之后的UTF8字符串存放位置
     @parma size[in out]：utf8 buffer的大小相当于utf8[size]，最后返回utf8中实际字符串长度
     
     @return：1，成功；0，失败。
     */
    IOSEXPORT int GbkToUtf8(const char* gbk,char* utf8,int& size);
    /*
     UNICODE转GBK
     @param unicode[in]：UNICODE字符串
     @parma gbk[out]：转换之后的GBK字符串存放位置
     @parma size[in out]：gbk buffer的大小相当于gbk[size]，最后返回gbk中实际字符串长度
     
     @return：1，成功；0，失败。
     */
    IOSEXPORT int UnicodeToGbk(const unsigned short* unicode,char* gbk,int& size);
    /*
     GBK转UNICODE
     @param gbk[in]：UTF8字符串
     @parma unicode[out]：转换之后的UNICODE字符串存放位置
     @parma size[in out]：unicode buffer的大小相当于unicode[size]，最后返回gbk中实际字符串长度。
     @注：UNICODE一个字符两个字节
     
     @return：1，成功；0，失败。
     */
    IOSEXPORT int GbkToUnicode(const char* gbk,unsigned short* unicode,int& size);
    /*
     判断是否是网通用户
     @param host[in]：要连接的域名
     @param strWangtong[in]：指定的网通IP地址，可以把几个IP地址连起来，放进去，通过";"分割。
     
     @return：0，不是网通用户；1，是网通用户。
     */
    IOSEXPORT int IsWangtong(const char* host,const char* strWangtong);
    /*
     从本地数据库中获取到指定房间ID的房间信息
     @param roomId[in]：指定的房间ID，或者包厢ID
     @parma roomInfo[out]：指定房间信息的存放位置
     
     @return：1，成功；0，失败。
     */
    IOSEXPORT int GetRoomInfoFromLocal(int roomId,ResultData_RoomInfo* roomInfo);
    /*
     从本地数据库中获取指定IDX的用户信息
     @param idx[in]:指定IDX
     @param userInfo[out]:获取到的个人信息。
     
     @return：1，成功；0，失败。
     */
    IOSEXPORT int GetUserInfoFromLocal(int idx,USER_DATA_BASE* userInfo);
    /*
     从本地数据库获取制定礼物信息
     @param giftId:指定礼物ID
     @param pGI:礼物信息存放位置
     
     @return：1，成功；0，失败。
     */
    IOSEXPORT int GetGiftInfoFromLocal(int giftId,GiftInfo* pGI);
    //获取所有礼物信息的ID
    /*
     @param pId[out]：礼物ID存放的位置
     @param nNum[in out]：数组大小，返回存放了多少个ID
     @return 数量够存放，true；数量不够存放，false
     
     @注：一定要严格判断返回值
     */
    IOSEXPORT bool GetAllGiftId(int pId[],int& nNum);
    /*
     专门用于星光物件的更新接口，包含礼物等
     
     @param taskInfo[in]：下载任务信息，具体参见结构说明
     @param priority[in]：是否优先下载
     
     @注1：对于TaskDownload结构中的DownloadInfo结构中的saveBuf，这里已经对其做了管理，用户无需自己申请管理内存，只要在回调的函数里使用即可
     @注2：对于结构中url和download必须根据实际相匹配
     
     @例:
     TaskDownload task;
     strcpy(task.url , "http://xg.91555.com/res/giftxml.html");
     task.onProgress = &OnProgress;//可选，指定进度函数，可以显示下载，访问进度条。
     task.onFinish = &OnFinish;//指定完成函数，
     task.info.download = false;
     strcpy(task.info.fileName , "C:\\Users\\Administrator\\Desktop\\gift\\gift_items.zip");//指定文件名和位置
     task.info.mandatory = false;
     strcpy(task.info.unzipDir , "C:\\Users\\Administrator\\Desktop\\testunzip");//指定解压路径
     b = g_gMgr.addTask(task);
     //上面这个例子中，你还需要在OnFinish中解析你Http请求到的内容，然后进一步添加任务，下载真正的zip文件，详情请咨询@guliping。
     
     @return：0，添加任务成功；-1，添加任务失败；-2，文件已经存在无需下载
     */
    IOSEXPORT int AddHttpDownLoadTask(TaskDownload* taskInfo,bool priority=false);
    
    //下面几个解析Xml的函数请一起使用，重新调用ParaseXml之后每次的结果都会不一样。
    /*
     解析给定的Xml buffer
     
     @param buffer[in]：指定的需要解析的buffer
     @param utf8[in]：指定的字符编码，默认为utf8.
     
     @return：1，解析成功；0，解析失败。
     */
    IOSEXPORT int ParaseXml(const char* buffer,bool utf8=true);
    /*
     对上面传入的xml查找第一个子节点
     @param name[in]：可以为NULL，如果有名字为查找对应的名字，查找的结果会保存在函数里面
     
     @return：1，找到对应的element；0，没有找到。
     */
    IOSEXPORT int FindFirstChildElementFromDoc(const char* name);
    /*
     对当前查找到的element继续查找子element
     @param name[in]：可以为NULL，如果有名字为查找对应的名字，查找的结果会保存在函数里面
     
     @return：1，找到对应的element；0，没有找到。
     */
    IOSEXPORT int FindFirstChildElementFromCur(const char* name);
    /*
     获取到当前element的名字为name的值。对于有些值可以转int类型
     @param name[in]：指定名称
     @param buffer[out]：值保存的地方，
     @param size[in out]：buffer的大小，返回值为实际存入的大小。
     
     @return：1，成功；0，失败。
     */
    IOSEXPORT int GetElementAttribute(const char* name,char* buffer,int& size);
    /*
     手机用户聊天接口
     @param toIdx[in]:发送给谁，如果是0就是公聊。
     @param content[in]:聊天的内容.
     
     @return：0，成功；-1，失败。
     */
    IOSEXPORT int SendChatKTV(int toIdx,const char* content);
    /*
     手机用户送礼接口
     @param toIdx[in]:发送给谁
     @param giftId[in]:礼物ID
     @param number[in]:礼物数量
     @param roomId[in]:房间ID，如果在大厅则传0
     
     @return：0，成功；-1，失败。
     */
    IOSEXPORT int SendGiftInLobby(int toIdx,int giftId,int number,int roomId);
    
    IOSEXPORT bool MFInitFilter();
    /*
     @param pFilePath1 : "forbiddenword.dog"含路径
     @param pFilePath1 : "forbiddenword2.dog"含路径
     
     务必调用此方法，否则所有MF前缀的函数都将返回false
     */
    //@Deprecated
	//IOSEXPORT bool MFInitFilter(const char* pFilePath1,const char* pFilePath2);
	/*
     过滤敏感字符串
     @param utf8[in out]：需要对比的UTF8字符串
     @param size[in] : 字符串的长度
     
     @return :
     false，没有初始化或者utf8为NULL
     true，成功过滤完毕
     
     @注：utf8中的字符串必须是UTF8字符串
     */
    IOSEXPORT bool MFFilter( char * utf8, int size );
	/*
     查找是否含有敏感字符串
     
     @param utf8[in]：需要对比的UTF8字符串
     
     @注：utf8中的字符串必须是UTF8字符串
     
     @return ：true，有；false，没有
     */
	IOSEXPORT bool MFFindSensWord( const char* utf8 );
	/*
     查找是否含有特殊符号
     
     @param utf8[in]：需要对比的字符串
     
     @return ：true，有；false，没有
     */
	IOSEXPORT bool MFFindSensWord2( const char* utf8 );
#ifdef __cplusplus
}
#endif
    
#endif
