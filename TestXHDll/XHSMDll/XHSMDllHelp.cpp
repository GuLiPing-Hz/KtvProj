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
	//�������̿���(TH32CS_SNAPPROCESS��ʾ�������н��̵Ŀ���)  
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);  

	//PROCESSENTRY32���̿��յĽṹ��  
	PROCESSENTRY32 pe;  

	//ʵ������ʹ��Process32First��ȡ��һ�����յĽ���ǰ�����ĳ�ʼ������  
	pe.dwSize = sizeof(PROCESSENTRY32);  


	//�����IFЧ��ͬ:  
	if((hSnapShot == INVALID_HANDLE_VALUE) || (!Process32First(hSnapShot,&pe)))
	{  
		SAFE_CLOSEFILEHANDLE(hSnapShot);
		goto final;
	}  

	//���ַ���ת��ΪСд  
	for_each(strProcessName.begin(),strProcessName.end(),tolower);
	//��������Ч  ��һֱ��ȡ��һ�����ѭ����ȥ  
	while (Process32Next(hSnapShot,&pe))
	{
		//pe.szExeFile��ȡ��ǰ���̵Ŀ�ִ���ļ�����  
		std::string scTmp = pe.szExeFile;  

		//����ִ���ļ���������Ӣ����ĸ�޸�ΪСд  
		for_each(scTmp.begin(),scTmp.end(),tolower);

		//�Ƚϵ�ǰ���̵Ŀ�ִ���ļ����ƺʹ��ݽ������ļ������Ƿ���ͬ  ���ǵ�64λ���� (xxx.exe *32)
		if( 0 == scTmp.find(strProcessName))
		{  
			//�ӿ��ս����л�ȡ�ý��̵�PID(������������е�PID)  
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

