#include "StdAfx.h"
#include "XHSMDllHelp.h"
#include "XHHead.h"
#include "progress/ShareMemoryThreadMgr.h"
#include <string>
#include <ctype.h>
#include <TlHelp32.h>
#include <algorithm>

CShareMemoryThreadMgr g_smtMgr;
DEALMESSAGE2 CXHSMDllHelp::m_pDealMessage = NULL;

void CXHSMDllHelp::InitSMHelp(DEALMESSAGE2 pDealMessage)
{
	m_pDealMessage = pDealMessage;
}


CXHSMDllHelp::CXHSMDllHelp(const char* exeDir,const char* exeName,bool bNeedStart)
{
	//KillProcessFromName(exeName);
	strcpy_s(m_exeName,exeName);
	g_smtMgr.Create();
	XHH::DoubleStringB* pdStr = new XHH::DoubleStringB;
	if (pdStr)
	{
		strcpy_s(pdStr->str1,exeName);
		strcpy_s(pdStr->str2,exeDir);
		pdStr->needstart = bNeedStart;
		g_smtMgr.PostMessage(MSG_CREATESHAREMEMORY,(void*)pdStr);
	}
}

CXHSMDllHelp::~CXHSMDllHelp(void)
{
	KillProcessFromName(m_exeName);
	g_smtMgr.Release();
}

void CXHSMDllHelp::PostClose()
{
	g_smtMgr.PostMessage(MSG_XINGGUANG_CLOSE,NULL);
}

bool CXHSMDllHelp::KillProcessFromName(const char* pProcessName)  
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

