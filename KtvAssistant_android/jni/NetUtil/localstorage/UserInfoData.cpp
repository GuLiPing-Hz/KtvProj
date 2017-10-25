#include "UserInfoData.h"
#include "XmlFileDef.h"
#include <string.h>

#include "../wrap/config.h"
#ifndef NETUTIL_ANDROID
#define LOGI printf
#define LOGE printf
#define LOGW printf
#endif

#define DEF_USERDADA "LocalUserData"

#define SETATTRIBUTE(x) \
pElement->SetAttribute(#x,userInfo.x)

#define GETATTRIBUTE(x) \
p = pElement->Attribute((#x)); \
strcpy(userInfo.x,p?p:"")

#define GETATTRIBUTEINT(x) \
p = pElement->Attribute(#x); \
userInfo.x = atoi(p?p:"0")

#define GETATTRIBUTECHAR(x) \
p = pElement->Attribute(#x); \
userInfo.x = (char)atoi(p?p:"0")

#define GETATTRIBUTESHORT(x) \
p = pElement->Attribute(#x); \
userInfo.x = (short)atoi(p?p:"0")

inline bool ReadUserInfo(const TiXmlElement* pElement,ResultData_UserFullInfo& userInfo,int& version)
{
	memset(&userInfo,0,sizeof(userInfo));
	version = -1;
	if(!pElement)
		return false;
	const char* p = NULL;

	p = pElement->Attribute("version");
	version = atoi(p?p:"-1");
	GETATTRIBUTEINT(m_idx);
 	GETATTRIBUTE(m_id);									//用户id,为字符串
	GETATTRIBUTE(m_name);					//用户名字
	GETATTRIBUTE(m_province);								//省
	GETATTRIBUTE(m_city);								//城市
	GETATTRIBUTE(m_sex);										//性别
	GETATTRIBUTE(m_age);									//年龄
	GETATTRIBUTEINT(m_level);									//用户等级可转int
	GETATTRIBUTE(m_LevelTime);					//vip到期时间

	p = pElement->Attribute("isvip");
	userInfo.m_isVip = (char)atoi(p?p:"0");//是否vip账户

	GETATTRIBUTE(m_stage);
	GETATTRIBUTE(m_stageName);

	GETATTRIBUTE(m_ClubId);									//所在俱乐部id
	GETATTRIBUTE(m_ClubProfession);						//俱乐部权限：1管理员,0非管理员
	GETATTRIBUTE(m_ClubName);							//俱乐部名称	add by jinguanfu 2010/4/20

	p = pElement->Attribute("explevel");
	userInfo.m_ExpLevel = (char)atoi(p?p:"0");

// 	char m_AvatarId;									//
// 	char m_AvaratCount;
// 	short * m_AvaratData;

	GETATTRIBUTESHORT(m_singerlevel); //歌手等级
	GETATTRIBUTEINT(m_singer_expvalue);
	GETATTRIBUTESHORT(m_wealthlevel); //财富等级
	GETATTRIBUTEINT(m_wealth_expvalue);

	GETATTRIBUTE(headurl);    //头像URL
	GETATTRIBUTEINT(recvcount);        //礼物接收数
	GETATTRIBUTEINT(songcount);        //演唱歌曲总数
	GETATTRIBUTE(topsongname);   //拿手歌曲
	GETATTRIBUTEINT(topsongscore);   //演唱最高分

    GETATTRIBUTE(songname);
    GETATTRIBUTE(singername);
    GETATTRIBUTEINT(score);
	return true;
}

inline bool WriteUserInfo(TiXmlElement* pElement,const ResultData_UserFullInfo& userInfo,int version)
{
	if(!pElement)
		return false;

	pElement->SetAttribute("version",version);
	SETATTRIBUTE(m_idx);
	SETATTRIBUTE(m_id);									//用户id,为字符串
	SETATTRIBUTE(m_name);					//用户名字
	SETATTRIBUTE(m_province);								//省
	SETATTRIBUTE(m_city);								//城市
	SETATTRIBUTE(m_sex);										//性别
	SETATTRIBUTE(m_age);									//年龄
	SETATTRIBUTE(m_level);									//用户等级可转int
	SETATTRIBUTE(m_LevelTime);					//vip到期时间
	pElement->SetAttribute("isvip",userInfo.m_isVip?"1":"0");	//是否vip账户
	SETATTRIBUTE(m_stage);
	SETATTRIBUTE(m_stageName);

	SETATTRIBUTE(m_ClubId);									//所在俱乐部id
	SETATTRIBUTE(m_ClubProfession);						//俱乐部权限：1管理员,0非管理员
	SETATTRIBUTE(m_ClubName);							//俱乐部名称	add by jinguanfu 2010/4/20
	pElement->SetAttribute("explevel",(int)userInfo.m_ExpLevel);

// 	char m_AvatarId;									//
// 	char m_AvaratCount;
// 	short * m_AvaratData;

	// add by wpf 2012/5/24
	SETATTRIBUTE(m_singerlevel); //歌手等级
	SETATTRIBUTE(m_singer_expvalue);
	SETATTRIBUTE(m_wealthlevel); //财富等级
	SETATTRIBUTE(m_wealth_expvalue);
	// end by wpf

	SETATTRIBUTE(headurl);    //头像URL
	SETATTRIBUTE(recvcount);        //礼物接收数
	SETATTRIBUTE(songcount);        //演唱歌曲总数
	SETATTRIBUTE(topsongname);   //拿手歌曲
	SETATTRIBUTE(topsongscore);   //演唱最高分

    SETATTRIBUTE(songname);
    SETATTRIBUTE(singername);
    SETATTRIBUTE(score);
	return true;
}

CUserInfoData::CUserInfoData(const char* path)
	:CLocalStorage(path,XG_USERDATA_XML,DEF_USERDADA)
	,m_pCSUser(NULL)
{
	m_pCSUser = CriticalSectionWrapper::CreateCriticalSection();
	assert(m_pCSUser != NULL);
    buildStorage();
}
CUserInfoData::~CUserInfoData()
{
	if(m_pCSUser)
		delete m_pCSUser;
}
void CUserInfoData::buildStorage()
{
	if(!m_pRoot)
		return ;
	TiXmlElement* pElement = m_pRoot->FirstChildElement();
	do 
	{
		if(!pElement)
			break;

		ResultData_UserFullInfo userInfo;
		int version;
		if(ReadUserInfo(pElement,userInfo,version))
			addUserInfo(userInfo,version,true);

	} while ((pElement=pElement->NextSiblingElement()));
}

void CUserInfoData::addUserInfo(const ResultData_UserFullInfo& userinfo,int version,bool fromlocal)
{
	CriticalSectionScoped lock(m_pCSUser);

	if(!fromlocal && version != -2)
	{
		char key[260] = {0};
		sprintf(key,"g%d",userinfo.m_idx);

		MAPIDUSERINFO::iterator it = m_mapUserInfo.find(userinfo.m_idx);
		if(it != m_mapUserInfo.end())
		{
			if(version != it->second.version)
			{
				TiXmlElement* pElement = m_pRoot->FirstChildElement(key);
				WriteUserInfo(pElement,userinfo,version);
			}
		}
		else
		{
			TiXmlElement* pElement = new TiXmlElement(key);
			if(!pElement)
				goto failed;
			m_pRoot->LinkEndChild(pElement);
			WriteUserInfo(pElement,userinfo,version);
		}
	}

failed:
	USER_DATA_BASE udb;
	memset(&udb,0,sizeof(udb));
	memcpy(&udb.user_info.base,&userinfo,sizeof(ResultData_UserFullInfo));
	udb.user_info.base.m_AvatarId = -1;
	udb.user_info.base.m_AvaratCount = 0;
	udb.user_info.base.m_AvaratData = NULL;
	udb.name_state = IF_STATE_OK;
	udb.info_state = IF_STATE_OK;
	udb.version = version;
	m_mapUserInfo[userinfo.m_idx] = udb;
}
void CUserInfoData::addUserInfo(int idx,const char* nickName)
{
	CriticalSectionScoped lock(m_pCSUser);
	MAPIDUSERINFO::iterator it = m_mapUserInfo.find(idx);
	if (it != m_mapUserInfo.end())
	{
		strcpy(it->second.user_info.base.m_name,nickName?nickName:"");
		it->second.name_state = IF_STATE_OK;
	}
	else
	{
		USER_DATA_BASE userInfo;
		memset(&userInfo, 0, sizeof(userInfo));
		userInfo.name_state = IF_STATE_OK;
		userInfo.user_info.base.m_idx = idx;
		strcpy(userInfo.user_info.base.m_name, nickName?nickName:"");
		m_mapUserInfo[userInfo.user_info.base.m_idx] = userInfo;
	}
}
void CUserInfoData::addUserInfo(const USER_DATA_BASE& userinfo)
{
	CriticalSectionScoped lock(m_pCSUser);
    
    if(userinfo.version != -2)//如果不是自己，则保存到本地文件中，因为每次登陆大厅认证都会传递自己的个人信息。
    {
        MAPIDUSERINFO::iterator it = m_mapUserInfo.find(userinfo.user_info.base.m_idx);
        char key[260] = {0};
        sprintf(key,"g%d",userinfo.user_info.base.m_idx);

        if(it != m_mapUserInfo.end())
        {
            if(it->second.version != userinfo.version)
            {
                TiXmlElement* pElement = m_pRoot->FirstChildElement(key);
                WriteUserInfo(pElement,userinfo.user_info.base,userinfo.version);
            }
        }
        else
        {
            TiXmlElement* pElement = m_pRoot->FirstChildElement(key);
            if(!pElement)
            {
                pElement = new TiXmlElement(key);
                if(!pElement)
                    goto failed;
                m_pRoot->LinkEndChild(pElement);
            }
            WriteUserInfo(pElement,userinfo.user_info.base,userinfo.version);
        }
    }
failed:
	m_mapUserInfo[userinfo.user_info.base.m_idx] = userinfo;
}

void CUserInfoData::changePartInfoState(int idx,eINFO_STATE state)
{
	m_mapUserInfo[idx].part_state = state;
}

void CUserInfoData::changeLobby(int idx,int lobbyId)
{
	CriticalSectionScoped lock(m_pCSUser);
	MAPIDUSERINFO::iterator it = m_mapUserInfo.find(idx);
	if(it==m_mapUserInfo.end())
		return ;
	m_mapUserInfo[idx].user_info.lobbyid = lobbyId;
}

void CUserInfoData::changeRoom(int idx,int roomId)
{
	CriticalSectionScoped lock(m_pCSUser);
	MAPIDUSERINFO::iterator it = m_mapUserInfo.find(idx);
	if(it==m_mapUserInfo.end())
		return ;
	m_mapUserInfo[idx].user_info.roomid = roomId;
}

void CUserInfoData::changeUserPic(int idx,const char* picture)
{
	if(idx==0 || !picture)
		return ;
	CriticalSectionScoped lock(m_pCSUser);
	MAPIDUSERINFO::iterator it = m_mapUserInfo.find(idx);
	if(it!=m_mapUserInfo.end())
		strcpy(m_mapUserInfo[idx].user_info.base.headurl,picture);
	else
	{
		LOGW("%s : non't find the user\n",__FUNCTION__);
		return ;
	}
}

void CUserInfoData::updateRecvGiftCount(int idx,int num)
{
	CriticalSectionScoped lock(m_pCSUser);
	m_mapUserInfo[idx].user_info.base.recvcount = num;
}

USER_DATA_BASE* CUserInfoData::getUserInfo(int idx)
{
	CriticalSectionScoped lock(m_pCSUser);
	MAPIDUSERINFO::iterator it = m_mapUserInfo.find(idx);
	if(it != m_mapUserInfo.end())
		return &(it->second);
	return NULL;
}

