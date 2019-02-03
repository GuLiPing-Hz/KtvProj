#include "stdafx.h"
#include "XGSM.h"
#include "XHHead.h"
#include "progress/ShareMemoryThreadMgr.h"
#include <string>
#include <ctype.h>
#include <TlHelp32.h>
#include <algorithm>

CShareMemoryThreadMgr g_smtMgr;
DEALMESSAGE2 CXGSMHelp::m_pDealMessage = NULL;

void CXGSMHelp::InitSMHelp(DEALMESSAGE2 pDealMessage)
{
	m_pDealMessage = pDealMessage;
}


CXGSMHelp::CXGSMHelp(const char* name)
{
	g_smtMgr.Create();
	XHH::SingleString* pSS = NULL;
	if(name)
	{
		pSS = new XHH::SingleString;
		strcpy(m_exeName,name);
	}
	if (pSS)
	{
		strcpy(pSS->str,name);
		g_smtMgr.PostMessage(MSG_OPENSHAREMEMORY,(void*)pSS);
	}
}

CXGSMHelp::~CXGSMHelp(void)
{
	//KillProcessFromName(m_exeName);
	g_smtMgr.Release();
}

void CXGSMHelp::PostMessage(int cmd,void* pData)
{
	int msg = MSG_INVALID;
	void* pTmp = NULL;
	switch(cmd)
	{
		//实时评测数据
	case XHH::CMD_HOST_GRADE:
		{
			msg = MSG_HOST_GRADE;
			XHH::OtherGrade* pGrade = new XHH::OtherGrade;
			if(pGrade)
				memcpy(pGrade,pData,sizeof(XHH::OtherGrade));
			pTmp = pGrade;
			break;
		}
		//视频数据
	case XHH::CMD_HOST_VIDEO:
		{
			msg = MSG_HOST_VIDEO;
			XHH::VideoInfo* pVideo = new XHH::VideoInfo;
			if(pVideo)
			{
				memcpy(pVideo,pData,sizeof(XHH::VideoInfo));
				if(pVideo->size)
				{
					unsigned char* pRGB = new unsigned char[pVideo->size];
					if(pRGB)
						memcpy(pRGB,pVideo->buffer,pVideo->size);
					pVideo->buffer = pRGB;
				}
			}

			pTmp = pVideo;
			break;
		}
		//当前演唱歌曲
	case XHH::CMD_HOST_SONG:
		{
			msg = MSG_HOST_SONG;
			XHH::DoubleString* pSong = new XHH::DoubleString;
			if(pSong)
				memcpy(pSong,pData,sizeof(XHH::DoubleString));
			pTmp = pSong;
			break;
		}
		//消息
	case XHH::CMD_HOST_MSG:
		{
			msg = MSG_HOST_MSG;
			XHH::PMessage* pMessage = new XHH::PMessage;
			if(pMessage)
				memcpy(pMessage,pData,sizeof(XHH::PMessage));
			pTmp = pMessage;
			break;
		}
		//离开房间进入大厅
	case XHH::CMD_HOST_LEAVE:
		{
			msg = MSG_HOST_LEAVE_ROOM;
			break;
		}
		//通知播放默认歌曲
	case XHH::CMD_HOST_DEFAULT:
		{
			msg = MSG_HOST_DEFAULT;
			break;
		}
		//请求关闭
	case XHH::CMD_HOST_REQUESTC:
		{
			msg = MSG_HOST_REQC;
			break;
		}
		//进入房间
	case XHH::CMD_HOST_NOSING:
		{
			msg = MSG_HOST_NOSING;
			break;
		}
	}
	g_smtMgr.PostMessage(msg,pTmp);
}

bool CXGSMHelp::KillProcessFromName(const char* pProcessName)  
{  
	bool re = false;
	std::string strProcessName = pProcessName;
	//创建进程快照(TH32CS_SNAPPROCESS表示创建所有进程的快照)  
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);  

	//PROCESSENTRY32进程快照的结构体  
	PROCESSENTRY32 pe;  

	//实例化后使用Process32First获取第一个快照的进程前必做的初始化操作  
	pe.dwSize = sizeof(PROCESSENTRY32);  


	//下面的IF效果同:  
	if((hSnapShot == INVALID_HANDLE_VALUE) || (!Process32First(hSnapShot,&pe)))
	{  
		SAFE_CLOSEFILEHANDLE(hSnapShot);
		goto final;
	}  

	//将字符串转换为小写  
	for_each(strProcessName.begin(),strProcessName.end(),tolower);
	//如果句柄有效  则一直获取下一个句柄循环下去  
	while (Process32Next(hSnapShot,&pe))
	{
		//pe.szExeFile获取当前进程的可执行文件名称  
		std::string scTmp = pe.szExeFile;  

		//将可执行文件名称所有英文字母修改为小写  
		for_each(scTmp.begin(),scTmp.end(),tolower);

		//比较当前进程的可执行文件名称和传递进来的文件名称是否相同  考虑到64位机器 (xxx.exe *32)
		if( 0 == scTmp.find(strProcessName))
		{  
			//从快照进程中获取该进程的PID(即任务管理器中的PID)  
			DWORD dwProcessID = pe.th32ProcessID;  
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,dwProcessID);  
			::TerminateProcess(hProcess,0);  
			CloseHandle(hProcess);  
			goto final;
		}  
	}  

final:
	SAFE_CLOSEFILEHANDLE(hSnapShot);
	return re;
}  
