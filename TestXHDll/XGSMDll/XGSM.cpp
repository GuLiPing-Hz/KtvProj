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
		//ʵʱ��������
	case XHH::CMD_HOST_GRADE:
		{
			msg = MSG_HOST_GRADE;
			XHH::OtherGrade* pGrade = new XHH::OtherGrade;
			if(pGrade)
				memcpy(pGrade,pData,sizeof(XHH::OtherGrade));
			pTmp = pGrade;
			break;
		}
		//��Ƶ����
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
		//��ǰ�ݳ�����
	case XHH::CMD_HOST_SONG:
		{
			msg = MSG_HOST_SONG;
			XHH::DoubleString* pSong = new XHH::DoubleString;
			if(pSong)
				memcpy(pSong,pData,sizeof(XHH::DoubleString));
			pTmp = pSong;
			break;
		}
		//��Ϣ
	case XHH::CMD_HOST_MSG:
		{
			msg = MSG_HOST_MSG;
			XHH::PMessage* pMessage = new XHH::PMessage;
			if(pMessage)
				memcpy(pMessage,pData,sizeof(XHH::PMessage));
			pTmp = pMessage;
			break;
		}
		//�뿪����������
	case XHH::CMD_HOST_LEAVE:
		{
			msg = MSG_HOST_LEAVE_ROOM;
			break;
		}
		//֪ͨ����Ĭ�ϸ���
	case XHH::CMD_HOST_DEFAULT:
		{
			msg = MSG_HOST_DEFAULT;
			break;
		}
		//����ر�
	case XHH::CMD_HOST_REQUESTC:
		{
			msg = MSG_HOST_REQC;
			break;
		}
		//���뷿��
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
