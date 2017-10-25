/*
 * HttpUniformCallBack.cpp
 *
 *  Created on: 2014年10月9日
 *      Author: glp
 */

#include "HttpUniformCallBack.h"
#include "NetApp.h"

HttpUniformCallBack::HttpUniformCallBack(NetworkUtil::Reactor *pReactor)
:NetworkUtil::IdleEventHandler(pReactor)
{
	m_pCSMSG = CriticalSectionWrapper::CreateCriticalSection();
	m_listHttpCBM.clear();
}

HttpUniformCallBack::~HttpUniformCallBack() {
	if(m_pCSMSG)
		delete m_pCSMSG;
}
//同步到Net线程中
void HttpUniformCallBack::onRun()
{
	ResponseBase* pRb = g_NetApp.GetResponseHandler();
	if(!pRb)
		return ;

	HttpCallBackMsg gmsg;
	{
		CriticalSectionScoped lock(m_pCSMSG);
		if(m_listHttpCBM.empty())
			return ;
		gmsg.msg = m_listHttpCBM.front().msg;
		gmsg.data = m_listHttpCBM.front().data;
		m_listHttpCBM.pop_front();
	}

	SafePointer sp(gmsg.data);
	switch(gmsg.msg)
	{
		case FINISH_IMAGE:
		{
			pRb->RecvImageDownladFinished((const char*)gmsg.data);
			break;
		}
		case FINISH_DATABASE:
		{
			pRb->RecvLocalGiftDatabaseFinished(false);
			break;
		}
	}
}

void HttpUniformCallBack::postMessage(int msg,void* data)
{
	CriticalSectionScoped lock(m_pCSMSG);
	HttpCallBackMsg gmsg;
	gmsg.msg = msg;
	gmsg.data = data;
	m_listHttpCBM.push_back(gmsg);
}
