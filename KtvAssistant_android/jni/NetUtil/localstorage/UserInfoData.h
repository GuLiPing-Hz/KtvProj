#ifndef USERINFODADA__H__
#define USERINFODADA__H__

#include <stdlib.h>
#include "../wrap/NUtility.h"
#include "LocalStorage.h"
#include "../ResultData.h"
#include "DataType.h"
#include <map>

typedef std::map<int,USER_DATA_BASE> MAPIDUSERINFO;

class CUserInfoData : public CLocalStorage
{
	CUserInfoData(CUserInfoData&);
	CUserInfoData& operator=(const CUserInfoData&);
public:
	CUserInfoData(const char* path);
	virtual ~CUserInfoData();
	//CLocalStorage
	virtual void buildStorage();

	void addUserInfo(const ResultData_UserFullInfo& userinfo,int version,bool fromlocal=false);
	void addUserInfo(int idx,const char* nickName);
	void addUserInfo(const USER_DATA_BASE& userinfo);

	void changePartInfoState(int idx,eINFO_STATE state);
	void changeLobby(int idx,int lobbyId);
	void changeRoom(int idx,int roomId);
	void changeUserPic(int idx,const char* picture);
	void updateRecvGiftCount(int idx,int num);

	void addEnd(){saveFile();}

	USER_DATA_BASE* getUserInfo(int idx);
private:
	CriticalSectionWrapper*	m_pCSUser;

	//用户信息表
	MAPIDUSERINFO				m_mapUserInfo;
};

#endif//USERINFODADA__H__
