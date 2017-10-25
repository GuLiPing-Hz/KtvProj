/*
 * DataMgr.h
 *
 *  Created on: 2014-5-13
 *  author: glp
 */

#ifndef DATAMGR__H__
#define DATAMGR__H__
#include <map>
#include "ServerData.h"
#include "../xingconfig.h"
#include "../wrap/HttpDownloadType.h"
#include "UserInfoData.h"
#include "NotifyMsg.h"

typedef std::map<int,ROOM_DATA> MAPIDROOMINFO;
typedef std::map<int,GiftInfo> MAPGIFTINFO;

typedef enum _eUrlType
{
    URL_NONE = -1,
    URL_GIFT,
    URL_MAX
}eUrlType;

class CDataMgr {
public:
	CDataMgr();
	virtual ~CDataMgr();

    static void OnDownloadImageFinish(TaskDownload* downloadInfo,bool success,void* userData);
    static void OnDownloadConfigFinish(TaskDownload* downloadInfo,bool success,void* userData);
    static void OnDownloadConfigZipFinish(TaskDownload* downloadInfo,bool success,void* userData);
    
    //check URL
    eUrlType checkURL(const char* url);
	bool checkGiftVersionNeedUpdate(int version);
    //指定资源路径根目录,务必先制定下载路径，然后再去制定目录路径
    void initDownloadUrl(const char* url,unsigned int forUse);
    bool initRootDir(const char* path);
    //初始化礼物信息
    void initGift(bool direct = true);
    void reinitGift();
    //加载指定路径的礼物xml信息配置文件
    bool loadGiftInfo(bool direct);
    //下载xml配置文件
    void downloadGiftConfig();
    //获取指定ID的礼物信息
    bool getGiftInfoForId(int giftId,GiftInfo* pGI);
    //获取所有礼物信息的ID
    /*
     @return ：true , 数组够存放，成功获取ID；false , 获取失败
     
     @注：一定要严格判断返回值
    */
    bool getAllGiftId(int pId[],int& nNum);

	void saveMyIdx(int idx){CriticalSectionScoped lock(m_pCSMyInfo); m_nMe = idx;}
	int getMyIdx(){CriticalSectionScoped lock(m_pCSMyInfo); return m_nMe;}
	//保存自己的信息
	void saveSelfData(ResultData_SelfData* rd_sd);
	//保存金币信息，升级所需经验信息
    void saveMoney(int gold,int silver);
	void saveMyPartInfo(ResultData_PartInfo* rd_pi);
	void savePartInfo(ResultData_PartInfo* rd_pi);
	//保存用户信息
	void changeMyLobby(int lobbyId){changeLobby(m_nMe,lobbyId);}
	void changeMyRoom(int roomId){changeRoom(m_nMe,roomId);}
	void changeLobby(int idx,int lobbyId);
	void changeRoom(int idx,int roomId);
	void saveUserInfo(ResultData_UserFullInfo userinfo,int version);
    void saveUserInfo(int idx,const char* nickName);
	void saveUserInfo(const USER_DATA_BASE& userinfo);
	void changeUserPic(int idx,const char* picture);

	void saveRoomInfo(const ROOM_DATA& roominfo);
    //更新我的礼物接受数量
    void updateRecvGiftCount(int idx,int num);
    void updateMyRecvGiftCount(int num){updateRecvGiftCount(getMyIdx(),num);}

	USER_DATA_BASE* getUserInfo(int idx);
	USER_DATA_BASE* getMyInfo(){CriticalSectionScoped lock(m_pCSMyInfo); return getUserInfo(m_nMe);}
	ResultData_SelfData* getSelfData(){CriticalSectionScoped lock(m_pCSMyInfo); return &m_rdMyInfo;}

	ROOM_DATA* getRoomInfo(int roomid);

	//通知相关
	int getCurMsgId();
	void setCurMsgId(int id);
private:
    bool parseCommonGift(CLocalStorage& ls);
    bool parseLuckGift(CLocalStorage& ls);
    bool parseLuxuryGift(CLocalStorage& ls);
	void checkGiftVersion();
    
    int downloadGift(const char* url,const char* md5);
private:
    static bool             s_bCheckedVersion;
    
    CriticalSectionWrapper*	m_pCSURL;
    CriticalSectionWrapper*	m_pCSRoom;
	CriticalSectionWrapper*	m_pCSMyInfo;
	//房间信息表
	MAPIDROOMINFO			m_mapRoomInfo;
	//我自己的IDX
	int									m_nMe;
	//我自己的个人数据
	ResultData_SelfData 			m_rdMyInfo;
    CriticalSectionWrapper*	m_pCSGift;
	int										m_nGiftVersion;
	//礼物信息
	MAPGIFTINFO				m_mapGift;
        
    bool								m_bInitUrl;
    char									m_url[URL_MAX][260];
public:
    CServerData*					m_pServerData;
    CUserInfoData*				m_pUserData;
    CNotifyMsg*					m_pNotifyId;
    char                    m_dataRoot[260];
    char                    m_dataConfig[260];
    char                    m_dataImages[260];
    char                    m_dataUpdate[260];
};

#endif /* DATAMGR__H__ */
