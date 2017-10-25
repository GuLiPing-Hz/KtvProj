/*
 * HttpDownloadMgr.cpp
 *
 *  Created on: 2014-5-23
 *      Author: glp
 */

#include "HttpDownloadMgr.h"
#include <assert.h>

#include "config.h"
#ifndef NETUTIL_ANDROID
#define LOGI printf
#define LOGE printf
#endif

#define SAVE_BUF_SIZE			65535/*1024 * 25*/   // 接收缓冲区大小

namespace NetworkUtil {

CHttpDownloadMgr::CHttpDownloadMgr()
:m_pThread(NULL)
,m_pCS(NULL)
{
	// TODO Auto-generated constructor stub
	for(int i=0;i<MAX_DOWNLOAD;i++)
		m_gHttpDownload[i] = new CHttpDownload(&m_gReactor,this);

	m_pThread = ThreadWrapper::CreateThread(&CHttpDownloadMgr::HttpDownload,(void*)this);

	m_pCS = CriticalSectionWrapper::CreateCriticalSection();
}

CHttpDownloadMgr::~CHttpDownloadMgr() {
	// TODO Auto-generated destructor stub
	m_gReactor.stop();
	if(m_pThread)
	{
		if(m_pThread->Stop())
			m_pThread->Terminate(0);
		delete m_pThread;
	}

	for(int i=0;i<MAX_DOWNLOAD;i++)
	{
		if(m_gHttpDownload[i])
			delete m_gHttpDownload[i];
	}
}

bool CHttpDownloadMgr::HttpDownload(ThreadObj pParam)
{
	CHttpDownloadMgr* pMgr = (CHttpDownloadMgr*)pParam;
	if(pMgr)
		return pMgr->m_gReactor.run();

	return false;
}

int CHttpDownloadMgr::checkIdle()
{
	for(int i=0;i<MAX_DOWNLOAD;i++)
	{
		if(m_gHttpDownload[i] && m_gHttpDownload[i]->isIdle())
			return i;
	}

	return -1;
}

bool CHttpDownloadMgr::initDownload()
{
	if(!m_pThread)
		return false;
	unsigned int threadID;
	return m_pThread->Start(threadID);
}

int CHttpDownloadMgr::addTask(TaskDownload task,bool priority)
{
	CriticalSectionScoped lock(m_pCS);
	SETURL::const_iterator it = m_setUrl.find(task.url);
	if(it != m_setUrl.end())//已经在下载队列中
		return 0;

	if(!task.info.download)
	{//如果不是下载到文件
		task.info.saveBuf = new char[SAVE_BUF_SIZE];
		if(!task.info.saveBuf)
			return -1;
		task.info.saveBufLen = SAVE_BUF_SIZE;
		task.info.saveTmpBuf = task.info.saveBuf;
	}
    else
    {
        //检查MD5是否一致，一致则不下载
        if(task.info.MD5[0] != '\0')//确保不为空
        {
            //如果不要求强制下载，或者文件校验md5一致的话，则不加入下载队列，认为下载成功
            if(!task.info.mandatory && CHttpDownload::checkMD5(task.info.fileName,task.info.MD5))
            {
                //if(task.onFinish)
                //    task.onFinish(&task,true,task.userData);
                return -2;
            }
        }

		task.info.saveBuf = NULL;
		task.info.saveBufLen = 0;
		task.info.saveTmpBuf = NULL;
    }

	m_setUrl.insert(task.url);
	if(priority)
		m_listTask.push_front(task);
	else
		m_listTask.push_back(task);

	int nIndex = checkIdle();
	if(nIndex != -1 )
	{
		m_gHttpDownload[nIndex]->initDownload(task.url,task.onProgress,&task.info);

		memcpy(&m_gCurTask,&task,sizeof(task));
		if(!m_gHttpDownload[nIndex]->startDownload())
		{
			LOGE("%s : %s , start download failed\n",__FUNCTION__,task.url);
			doNext(false,NULL);
			return -1;
		}
		else
			return 0;
	}
	return 0;
}

void CHttpDownloadMgr::doNext(bool success,CHttpDownload* worker)
{
	TaskDownload info;
	{
		CriticalSectionScoped lock(m_pCS);
		memcpy(&info,&m_gCurTask,sizeof(info));
		if (worker)
			worker->uninitDownload();
	}
	//结束的回调，以及内存的回收
	if(info.onFinish)
		info.onFinish(&info,success,info.userData);
	if (info.info.saveBuf)//释放内存
		delete info.info.saveBuf;

	CriticalSectionScoped lock(m_pCS);
	m_setUrl.erase(info.url);
	if(strcmp(m_listTask.front().url , info.url) == 0)
		m_listTask.pop_front();
	else
	{
		LISTTASK::iterator it = m_listTask.begin();
		for (;it!=m_listTask.end();it++)
		{
			if(strcmp(it->url , info.url) == 0)
			{
				m_listTask.erase(it);
				//由set控制队列中的任务都是不同的URL
				break;
			}
		}
	}

	if(m_listTask.empty())
		return ;//已经都处理完毕

	//一般来说，到这一步应该是空闲的，但是如果在OnFinish中添加一个任务的话，这里就不是空闲状态了，所以还是需要判断一下
	int nIndex = checkIdle();

	if(nIndex != -1)
	{
		//取出队列的第一个
		TaskDownload next = m_listTask.front();
		m_gHttpDownload[nIndex]->initDownload(next.url,next.onProgress,&next.info);
		if(!m_gHttpDownload[nIndex]->startDownload())
		{
			LOGE("%s : %s , start download failed\n",__FUNCTION__,next.url);
			if(next.onFinish)
				next.onFinish(&next,false,next.userData);
			//继续处理下一个
			doNext(false,NULL);
		}
		else
		{
			CriticalSectionScoped lock(m_pCS);
			memcpy(&m_gCurTask,&next,sizeof(TaskDownload));
		}
	}
}

} /* namespace NetworkUtil */
