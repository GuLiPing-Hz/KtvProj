#ifndef _ReserveData_h_
#define _ReserveData_h_

#include <time.h>
#include "xingconfig.h"

class ReserveDataS
{
public:
	int serverid;
	short cmd;
	int seq;
#ifndef C__BUILD
	time_t t;
#else
	__int64 t;
#endif
	int cseq;
	int repeat;
};

class ReserveDataV : public ReserveDataS
{
public :
	virtual ~ReserveDataV(){}
	virtual void OnTimeOut();
};

#ifdef JNINETUTIL_EXPORTS
typedef ReserveDataV ReserveData;
#else
typedef ReserveDataS ReserveData;
#endif

class ReserveData_Idauth : public ReserveData
{
public:
	virtual ~ReserveData_Idauth(){}
	char id[21];
	char passwd[33];
};

class ReserveData_Idxauth : public ReserveData
{
public:
	virtual ~ReserveData_Idxauth(){}
	int idx;
	char passwd[33];
};

class ReserveData_AddFriend : public ReserveData
{
public:
	virtual ~ReserveData_AddFriend(){}
	int idx;
};

typedef ReserveData_AddFriend ReserveData_AddBlack;
typedef ReserveData_AddFriend ReserveData_DelFriend;
typedef ReserveData_AddFriend ReserveData_DelBlack;
typedef ReserveData_AddFriend ReserveData_UserSingInfo;

class ReserveData_ApplyClub : public ReserveData
{
public:
	virtual ~ReserveData_ApplyClub(){}
	int clubid;
	int cseq;
};

class ReserveData_LobbyInfo : public ReserveData
{
public:
	int lobbyid;
	//char lobbyname[200];
};

class ReserveData_SendBroad : public ReserveData
{
public:
	int from_idx;
	char content[302];
};

class ReserveData_RoomMemList:public ReserveData
{
public:
	int total;	//总数
};

class ReserveData_SendGift:public ReserveData
{
public:
	int r_idx;		//接收者idx
	int item_id;	//礼物id
	int count;		//礼物数量
};

class ReserveData_ChatKtv:public ReserveData
{
public:
    int to_idx;//聊天对象
    char content[4096];//聊天内容
};
#define IDX_MAX_LEN 200
class ReserveData_GetUserInfo : public ReserveData
{
public:
	int idx[IDX_MAX_LEN];//一次的请求数量不要超过200，
	int count;
};

#endif
