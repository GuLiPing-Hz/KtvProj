/*
 * DataMgr.cpp
 *
 *  Created on: 2014-5-13
 *      Author: Administrator
 */

#include "DataMgr.h"
#include <string.h>
#include "XmlFileDef.h"
#include "../NetApp.h"
#include "../wrap/WrapUnzip.h"

#include "../wrap/config.h"

#ifndef NETUTIL_ANDROID
#define LOGI printf
#define LOGE printf
#define LOGW printf
#endif
//用于config zip下载完成时使用的客户数据
typedef struct _ConfigZip{
    eUrlType e;
    CDataMgr* dm;
}ConfigZip;

bool CDataMgr::s_bCheckedVersion = false;

bool CDataMgr::ParseChatXml(const char* in,char* out,int size)
{
	if(!in || !out || !size)
		return false;

	out[0] = 0;
	TiXmlDocument doc;
	doc.Parse(in);
	if(doc.Error())
		return false;

	TiXmlElement* pElement = doc.FirstChildElement();//获取到msg element，firstnode为xml标准头
	//获取到msg element中的第一个名为txt的child element NextSiblingElement
	TiXmlElement* pChild = pElement->FirstChildElement();//获取到msg element中的child element
	bool bFirst = true;
	do 
	{
		if(pChild)
		{
			if (strcmp(pChild->Value(),"txt")==0)
			{
				//因为中间没有其他的xml element所以直接使用这个方法，获取到元素子结点的字符串
				const char* pStr = pChild->GetText();
				if(pStr)
				{
					if((strlen(out)+strlen(pStr)) >= size)
						break;
					strcat(out,pStr);
				}
			}
			else if(strcmp(pChild->Value(),"img")==0)
			{
				const char* pStr = pChild->Attribute("image");
				if(pStr)
				{
					if(strstr(pStr,"QQFace"))
					{
						const char* pNumber = pStr+strlen("QQFace");
						if(strlen(out)+strlen(pNumber)+4>=size)//4 = [a/]
							break;
						sprintf(out,"%s[a/%s]",out,pNumber);
					}
					else
					{
						if(strlen(out)+7>=size)// 7 = [image]
							break;
						sprintf(out,"%s[image]",out);
					}
				}
			}
		}
		else
			break;

	} while (pChild=pChild->NextSiblingElement());

	bool ret = true;

	unsigned short* unicode = g_NetApp.m_pCharsetCodecGBK->convertToUnicode(out);
	char* utf8 = g_NetApp.m_pCharsetCodecUTF8->convertFromUnicode(unicode);

	if(utf8)
	{
		strcpy(out,utf8);
		delete utf8;
	}
	else
	{
		out[0] = 0;
		ret = false;
	}
	if(unicode)
		delete unicode;
	return ret;
}

void CDataMgr::OnDownloadImageFinish(TaskDownload* downloadInfo,bool success,void* userData)
{
    if (!success)
    {
        LOGI("%s : %s download failed\n",__FUNCTION__,downloadInfo?downloadInfo->info.fileName:"");
        return ;
    }

    int lenName = strlen(downloadInfo->info.fileName)+1;
    void* pData = new char[lenName];
    if(downloadInfo && pData)
    	memcpy(pData,downloadInfo->info.fileName,lenName);
    g_NetApp.m_gHttpUCB.postMessage(FINISH_IMAGE,pData);
}
void CDataMgr::OnDownloadConfigFinish(TaskDownload* downloadInfo,bool success,void* userData)
{
    if (!success)
    {
        LOGI("%s : HttpRequest finished with FAILED for URL[%s]\n",__FUNCTION__,downloadInfo->url);
        return ;
    }
    CDataMgr* pDM = (CDataMgr*)userData;
    if(!pDM)
        return ;
    eUrlType e = pDM->checkURL(downloadInfo?downloadInfo->url:NULL);

    switch (e)
    {
        case URL_GIFT:
        {
            if(downloadInfo->info.saveBuf)
            {
                std::string url;
                std::string md5;
				int version;

                strcat(downloadInfo->info.saveBuf,"\r\n");
                TiXmlDocument doc;
                if(doc.Parse(downloadInfo->info.saveBuf))
                {
                    const char * p = NULL;
                    TiXmlElement * el = NULL;
                    TiXmlElement * root_node = doc.FirstChildElement("root");
                    if(root_node != NULL)
                    {
                        el = root_node->FirstChildElement();
                        if(el)
                        {
							if((p =el->Attribute("version")) != NULL)
								version = atoi(p);
							else
								version = 0;
                            if((p = el->Attribute("url")) != NULL)
                                url = p;
                            if((p =el->Attribute("md5")) != NULL)
                                md5 = p;
                        }
                    }

					if(!pDM->checkGiftVersionNeedUpdate(version))
					{
						LOGI("gift version is newest, no need to update[version = %d]\n",version);
						return ;
					}

					//下载URL中指定的zip文件
                    TaskDownload task = {0};
                    strcpy(task.url , url.c_str());
                    const char* pName = strrchr(task.url,'/');
                    task.info.download = true;
                    sprintf(task.info.fileName,"%s%s",pDM->m_dataUpdate,pName?pName:"glp.error");
                    task.info.mandatory = false;
                    strcpy(task.info.unzipDir,pDM->m_dataConfig);
                    task.onFinish = &CDataMgr::OnDownloadConfigZipFinish;
                    static ConfigZip cz;
                    cz.e = e;
                    cz.dm = pDM;
                    task.userData = &cz;
                    g_NetApp.m_gMgr.addTask(task,true);
                }
            }
            break ;
        }
        default:
            break ;
    }
}
void CDataMgr::OnDownloadConfigZipFinish(TaskDownload* downloadInfo,bool success,void* userData)
{
    if(!success)
    {
        LOGI("%s : HttpRequest finished with FAILED for URL[%s]\n",__FUNCTION__,downloadInfo->url);
        return ;
    }
    ConfigZip* pCZ = (ConfigZip*)userData;
    if(!pCZ)
        return ;

    switch (pCZ->e)
    {
        case URL_GIFT:
        {
            //重新加载礼物信息
            if(pCZ->dm)
                pCZ->dm->initGift(false);
            break ;
        }

        default:
            break ;
    }
}

CDataMgr::CDataMgr()
:m_nMe(0)
,m_pCSRoom(NULL)
,m_pCSURL(NULL)
,m_pCSGift(NULL)
,m_pServerData(NULL)
,m_pUserData(NULL)
,m_bInitUrl(false)
,m_nGiftVersion(0)
,m_pCSMyInfo(NULL)
,m_pNotifyId(NULL)
{
	memset(&m_rdMyInfo,0,sizeof(m_rdMyInfo));
    
    m_pCSRoom = CriticalSectionWrapper::CreateCriticalSection();
    m_pCSURL = CriticalSectionWrapper::CreateCriticalSection();
    m_pCSGift = CriticalSectionWrapper::CreateCriticalSection();
	m_pCSMyInfo = CriticalSectionWrapper::CreateCriticalSection();
}

CDataMgr::~CDataMgr() {
	if(m_pNotifyId)
		delete m_pNotifyId;
    if(m_pServerData)
        delete m_pServerData;
	if(m_pUserData)
		delete m_pUserData;
    if(m_pCSGift)
        delete m_pCSGift;
    if(m_pCSURL)
        delete m_pCSURL;
    if(m_pCSRoom)
        delete m_pCSRoom;
	if(m_pCSMyInfo)
		delete m_pCSMyInfo;
}

bool CDataMgr::parseCommonGift(CLocalStorage& ls)
{
	TiXmlElement * el = NULL;
	TiXmlElement * elc = NULL;
	//TiXmlElement * root_node = doc.FirstChild( "shopgift" );
    el = ls.findFirstElement("version");
	if(el)
	{
		const char* pVersion = el->Attribute("property");
		m_nGiftVersion = atoi(pVersion?pVersion:"0");
	}
	else
		m_nGiftVersion = 0;

    el = ls.findFirstElement("item");
    if(!el)
        return false;
	//el = root_node->FirstChildElement( "item" );
    
	while ( el )
	{
		GiftInfo commonGift = {0};
		elc = el->FirstChildElement( "index" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            commonGift.id = atol(pText?pText:"");
        }
		elc = el->FirstChildElement("order");
		if(elc)
		{
			const char* pText = elc->GetText();
			commonGift.order = atol(pText?pText:"");
		}
		elc = el->FirstChildElement( "picname" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            strcpy(commonGift.image_name,pText?pText:"");
        }
		elc = el->FirstChildElement( "picUrl" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            strcpy(commonGift.image_url,pText?pText:"");
        }
		elc = el->FirstChildElement( "itemname" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            strcpy(commonGift.name,pText?pText:"");
        }
		{//金币
			elc = el->FirstChildElement( "itemvalue" );
			if ( elc )
            {
                const char* pText = elc->GetText();
                commonGift.cash = atol(pText?pText:"");
            }
			commonGift.coin = 0;
		}

		elc = el->FirstChildElement( "unitname" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            strcpy(commonGift.unit_name,pText?pText:"");
        }
        
        elc = el->FirstChildElement("effects");
        if(elc)
        {
            const char* pText = elc->GetText();
            commonGift.effectId = atoi(pText?pText:"");
        }
        
		//普通
		commonGift.gift_type = 2;
		m_mapGift[commonGift.id] = commonGift;
        
		elc = el->FirstChildElement( "picMD5" );
		if ( elc )
		{
            //下载指定文件
            const char* md5 = elc->GetText();
            if(downloadGift(commonGift.image_url, md5) == -1)
                LOGE("%s : %s add http download failed for url[%s]"
                     ,__FUNCTION__,commonGift.image_name,commonGift.image_url);
		}
        
		el = el->NextSiblingElement( "item" );
	}
    
    return true;
}
bool CDataMgr::parseLuckGift(CLocalStorage& ls)
{
	TiXmlElement * el = NULL;
	TiXmlElement * elc = NULL;
    
	el = ls.findFirstElement("version");
	if(el)
	{
		const char* pVersion = el->Attribute("property");
		m_nGiftVersion = atoi(pVersion?pVersion:"0");
	}
	else
		m_nGiftVersion = 0;

    el = ls.findFirstElement("item");
    if(!el)
        return false;
	//el = root_node->FirstChildElement( "item" );
	while ( el )
	{
		GiftInfo luckGift = {0};
		elc = el->FirstChildElement( "index" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            luckGift.id = atol(pText?pText:"");
        }	
		elc = el->FirstChildElement("order");
		if(elc)
		{
			const char* pText = elc->GetText();
			luckGift.order = atol(pText?pText:"");
		}
		elc = el->FirstChildElement( "title" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            strcpy(luckGift.name,pText?pText:"");
        }
		elc = el->FirstChildElement( "picname" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            strcpy(luckGift.image_name,pText?pText:"");
        }
		elc = el->FirstChildElement( "picUrl" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            strcpy(luckGift.image_url,pText?pText:"");
        }
        
		elc = el->FirstChildElement( "cash" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            luckGift.cash = atol(pText?pText:"");
        }
		elc = el->FirstChildElement( "coin" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            luckGift.coin = atol(pText?pText:"");
        }
        
		elc = el->FirstChildElement( "unitname" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            strcpy(luckGift.unit_name,pText?pText:"");
        }
        elc = el->FirstChildElement("effects");
        if(elc)
        {
            const char* pText = elc->GetText();
            luckGift.effectId = atoi(pText?pText:"");
        }

		//幸运礼物
        luckGift.gift_type = 1;
		m_mapGift[luckGift.id] = luckGift;
        
		elc = el->FirstChildElement( "picMD5" );
		if ( elc )
		{
            //下载图片
            const char* md5 = elc->GetText();
            if(downloadGift(luckGift.image_url, md5) == -1)
                LOGE("%s : %s add http download failed for url[%s]"
                     ,__FUNCTION__,luckGift.image_name,luckGift.image_url);
		}
        
		el = el->NextSiblingElement( "item" );
	}
    return true;
}
bool CDataMgr::parseLuxuryGift(CLocalStorage& ls)
{
	TiXmlElement * el = NULL;
	TiXmlElement * elc = NULL;

	el = ls.findFirstElement("version");
	if(el)
	{
		const char* pVersion = el->Attribute("property");
		m_nGiftVersion = atoi(pVersion?pVersion:"0");
	}
	else
		m_nGiftVersion = 0;

    el = ls.findFirstElement("item");
    if(!el)
        return false;
	//el = root_node->FirstChildElement( "item" );
	while ( el )
	{
		GiftInfo luxuryGift = {0};
		elc = el->FirstChildElement( "index" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            luxuryGift.id = atol( pText?pText:"" );
        }
		elc = el->FirstChildElement("order");
		if(elc)
		{
			const char* pText = elc->GetText();
			luxuryGift.order = atol(pText?pText:"");
		}
		elc = el->FirstChildElement( "picname" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            strcpy( luxuryGift.image_name,pText?pText:"");
        }
		elc = el->FirstChildElement( "picUrl" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            strcpy( luxuryGift.image_url,pText?pText:"");
        }
		elc = el->FirstChildElement( "itemname" );
        if ( elc )
        {
            const char* pText = elc->GetText();
            strcpy( luxuryGift.name,pText?pText:"");
        }

		{//金币
			elc = el->FirstChildElement( "itemvalue" );
            if ( elc )
            {
                const char* pText = elc->GetText();
                luxuryGift.cash = atol(pText?pText:"");
            }
			luxuryGift.coin = 0;
		}
        
// 		elc = el->FirstChildElement( "luxurious" );
//         if ( elc )
//         {
//             const char* pText = elc->GetText();
//             luxuryGift.luxurious = atol(pText?pText:"");
//         }

		elc = el->FirstChildElement( "unitname" );
		if ( elc )
        {
            const char* pText = elc->GetText();
            strcpy( luxuryGift.unit_name,pText?pText:"");
        }
// 		elc = el->FirstChildElement( "sort" );
//         if ( elc )
//         {
//             const char* pText = elc->GetText();
//             luxuryGift.sort = atol( pText?pText:"" );
//         }
        
// 		elc = el->FirstChildElement( "type" );
// 		char type[260] = "0";
//         if ( elc )
//         {
//             const char* pText = elc->GetText();
//             strcpy(type,pText?pText:"");
//         }
        
        elc = el->FirstChildElement("effects");
        if(elc)
        {
            const char* pText = elc->GetText();
            luxuryGift.effectId = atoi(pText?pText:"");
        }
        
		//奢侈
		luxuryGift.gift_type = 3;
		m_mapGift[luxuryGift.id] = luxuryGift;
		
        
		elc = el->FirstChildElement( "picMD5" );
		if ( elc )
		{
            //下载指定图片
            const char* md5 = elc->GetText();
            if(downloadGift(luxuryGift.image_url, md5) == -1)
                LOGE("%s : %s add http download failed for url[%s]"
                     ,__FUNCTION__,luxuryGift.image_name,luxuryGift.image_url);
		}
        
		el = el->NextSiblingElement( "item" );
	}
    return true;
}

void CDataMgr::checkGiftVersion()
{
	//检查跟服务器的版本信息
	downloadGiftConfig();
}

int CDataMgr::downloadGift(const char* url,const char* md5)
{
    if(!url || !md5)
        return false;
    
    TaskDownload task = {0};
    strcpy(task.url, url);
    task.info.download = true;
    const char* pName = strrchr(url, '/');
    sprintf(task.info.fileName, "%s%s",m_dataImages,pName?pName:"glp.error");
    strcpy(task.info.MD5, md5);
    task.info.mandatory = false;//非强制下载，禮物圖片應該不會怎麼變
    task.onFinish = &CDataMgr::OnDownloadImageFinish;
    
    return g_NetApp.m_gMgr.addTask(task,false);
}
//check URL
eUrlType CDataMgr::checkURL(const char* url)
{
    if(!url)
        return URL_NONE;
    
    CriticalSectionScoped lock(m_pCSURL);
    if(strcmp(m_url[URL_GIFT], url) == 0)
        return URL_GIFT;
    else
        return URL_NONE;
}
bool CDataMgr::checkGiftVersionNeedUpdate(int version)
{
	CriticalSectionScoped lock(m_pCSGift);

	//LOGI("Local Version : %d,Server Version : %d\n",m_nGiftVersion,version);
	bool isNeddUpdate = version > m_nGiftVersion;
	if(isNeddUpdate)
	{
		char file[260] = {0};
		sprintf(file,"%s/%s",m_dataConfig,XG_GIFT_COMMON_XML);
		remove(file);
		sprintf(file,"%s/%s",m_dataConfig,XG_GIFT_PROP_XML);
		remove(file);
		sprintf(file,"%s/%s",m_dataConfig,XG_GIFT_LUXURY_XML);
		remove(file);
		s_bCheckedVersion = false;
	}
	return isNeddUpdate;
}
void CDataMgr::initDownloadUrl(const char* url,unsigned int forUse)
{
    if(! url)
        return ;
    if(forUse >= URL_MAX)
        return ;
    {
        CriticalSectionScoped lock(m_pCSURL);
        strcpy(m_url[forUse], url);
    }
    
    m_bInitUrl = true;
    for (int i=0; i<URL_MAX; i++)
    {
        if(m_url[i][0] == '\0')
        {
            m_bInitUrl = false;
            return ;
        }
    }
}
bool CDataMgr::initRootDir(const char* path)
{
    if(!m_bInitUrl)
        return false;
    
    if(!path)
    {
        m_dataConfig[0] = 0;
        m_dataImages[0] = 0;
        m_dataRoot[0] = 0;
        m_dataUpdate[0] = 0;
    }
    else
    {
        strcpy(m_dataRoot,path);
        sprintf(m_dataConfig,"%s/%s",path,XG_DIR_CONFIG);
        sprintf(m_dataImages,"%s/%s",path,XG_DIR_IMAGE);
        sprintf(m_dataUpdate,"%s/%s",path,XG_DIR_UPDATE);
        
        CUnZip::createDir(m_dataConfig);
        CUnZip::createDir(m_dataImages);
        CUnZip::createDir(m_dataUpdate);
    }
    
    if(m_pServerData)
        delete m_pServerData;
    m_pServerData = new CServerData(m_dataConfig);
    if(!m_pServerData)
        return false;
    
	if(m_pUserData)
		delete m_pUserData;
	m_pUserData = new CUserInfoData(m_dataConfig);
	if(!m_pUserData)
		return false;

	if(!m_pNotifyId)
		m_pNotifyId = new CNotifyMsg(m_dataConfig);
    initGift();
    
    return true;
}
void CDataMgr::initGift(bool direct)
{
    if(!loadGiftInfo(direct))
    {
        //重新下载zip包
        downloadGiftConfig();
    }
	else
	{
		if(!s_bCheckedVersion)
		{
			//检查web上的版本
			checkGiftVersion();
			s_bCheckedVersion = true;
		}
	}
}
void CDataMgr::reinitGift()
{
	downloadGiftConfig();
}
bool CDataMgr::loadGiftInfo(bool direct)
{
    {
        CriticalSectionScoped lock(m_pCSGift);
		m_mapGift.clear();
        CLocalStorage common_gift(m_dataConfig,XG_GIFT_COMMON_XML);
        if(!common_gift.checkLoad())
            return false;
        if(!parseCommonGift(common_gift))
            return false;
        
        CLocalStorage prop_gift(m_dataConfig,XG_GIFT_PROP_XML);
        if(!prop_gift.checkLoad())
            return false;
        if(!parseLuckGift(prop_gift))
            return false;
        
        CLocalStorage luxury_gift(m_dataConfig,XG_GIFT_LUXURY_XML);
        if(!luxury_gift.checkLoad())
            return false;
        if(!parseLuxuryGift(luxury_gift))
            return false;
    }
    
    if(direct)
    {
		ResponseBase* pRb = g_NetApp.GetResponseHandler();
		if(pRb)
			pRb->RecvLocalGiftDatabaseFinished(true);
    }
    else
    	g_NetApp.m_gHttpUCB.postMessage(FINISH_DATABASE,NULL);
    
    return true;
}
void CDataMgr::downloadGiftConfig()
{
    //首先先解析URL,网页中的URL
    TaskDownload task = {0};
    strcpy(task.url, m_url[URL_GIFT]);
    task.info.download = false;
    task.onFinish = &CDataMgr::OnDownloadConfigFinish;
    task.userData = this;
    g_NetApp.m_gMgr.addTask(task);
}

//获取制定ID的礼物信息
bool CDataMgr::getGiftInfoForId(int giftId,GiftInfo* pGI)
{
    if(!pGI)
        return false;
    CriticalSectionScoped lock(m_pCSGift);
    //查找礼物
    MAPGIFTINFO::iterator it = m_mapGift.find(giftId);
    if(it != m_mapGift.end())
    {
        memcpy(pGI, &(it->second), sizeof(GiftInfo));
        return true;
    }
    
    memset(pGI, 0, sizeof(GiftInfo));
    return false;
}

bool CDataMgr::getAllGiftId(int pId[],int& nNum)
{
    CriticalSectionScoped lock(m_pCSGift);
    int total = m_mapGift.size();
    if(!pId || nNum<total)
    {
        nNum = total;
        return false;
    }
    else
    {
        int i = 0;
		MAPGIFTINFO::const_iterator it = m_mapGift.begin();
		for (;it!=m_mapGift.end();it++)
		{
			pId[i++] = it->first;
		}
        nNum = i;
        return true;
    }
}
void CDataMgr::saveSelfData(ResultData_SelfData* rd_sd)
{
	CriticalSectionScoped lock(m_pCSMyInfo);
	if(rd_sd)
		memcpy(&m_rdMyInfo,rd_sd,sizeof(m_rdMyInfo));
}

void CDataMgr::savePartInfo(ResultData_PartInfo* rd_pi)
{
	if(rd_pi)
		m_pUserData->changePartInfoState(rd_pi->idx,IF_STATE_OK);
}
void CDataMgr::saveMoney(int gold,int silver)
{
	CriticalSectionScoped lock(m_pCSMyInfo);
    m_rdMyInfo.gold = gold;
    m_rdMyInfo.silver = silver;
}
void CDataMgr::saveMyPartInfo(ResultData_PartInfo* rd_pi)
{
	if(rd_pi)
	{
		{
			CriticalSectionScoped lock(m_pCSMyInfo);
			m_rdMyInfo.gold = rd_pi->gold;
			m_rdMyInfo.silver = rd_pi->silver;
			m_rdMyInfo.nexp = rd_pi->nexp;
		}

		m_pUserData->changePartInfoState(rd_pi->idx,IF_STATE_OK);
	}
}
void CDataMgr::changeLobby(int idx,int lobbyId)
{
	m_pUserData->changeLobby(idx,lobbyId);
}
void CDataMgr::changeRoom(int idx,int roomId)
{
	m_pUserData->changeRoom(idx,roomId);
}
void CDataMgr::saveUserInfo(ResultData_UserFullInfo userinfo,int version)
{
	m_pUserData->addUserInfo(userinfo,version);
    m_pUserData->addEnd();
}
void CDataMgr::saveUserInfo(int idx,const char* nickName)
{
	m_pUserData->addUserInfo(idx,nickName);
}
void CDataMgr::saveUserInfo(const USER_DATA_BASE& userinfo)
{
	m_pUserData->addUserInfo(userinfo);
    m_pUserData->addEnd();
}
void CDataMgr::changeUserPic(int idx,const char* picture)
{
	m_pUserData->changeUserPic(idx,picture);
}
void CDataMgr::saveRoomInfo(const ROOM_DATA& roominfo)
{
    CriticalSectionScoped lock(m_pCSRoom);
	m_mapRoomInfo[roominfo.base.idx] = roominfo;
}
void CDataMgr::updateRecvGiftCount(int idx,int num)
{
	m_pUserData->updateRecvGiftCount(idx,num);
}
USER_DATA_BASE* CDataMgr::getUserInfo(int idx)
{
    return m_pUserData->getUserInfo(idx);
}

ROOM_DATA* CDataMgr::getRoomInfo(int roomid)
{
    CriticalSectionScoped lock(m_pCSRoom);
	MAPIDROOMINFO::iterator it = m_mapRoomInfo.find(roomid);
	if(it != m_mapRoomInfo.end())
		return &(it->second);
	return NULL;
}

int CDataMgr::getCurMsgId()
{
	return m_pNotifyId?m_pNotifyId->getCurMsgId():0;
}
void CDataMgr::setCurMsgId(int id)
{
	if(m_pNotifyId)
		m_pNotifyId->setCurMsgId(id);
}
