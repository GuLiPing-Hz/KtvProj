// ParticleEdit.cpp : ����Ӧ�ó��������Ϊ��
//
#include "stdafx.h"
#include <vld.h>
#include "ParticleEdit.h"
#include "ParticleEditDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CParticleEditApp

BEGIN_MESSAGE_MAP(CParticleEditApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CParticleEditApp ����

CParticleEditApp::CParticleEditApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CParticleEditApp ����

CParticleEditApp theApp;

extern int glp();

DWORD CALLBACK ThreadProc(LPVOID pParam)
{
	Sleep(100);
	return glp();
}

CParticleEditDlg* g_pDlg=NULL;
// CParticleEditApp ��ʼ��

BOOL CParticleEditApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CParticleEditDlg dlg;
	g_pDlg = &dlg;

	DWORD threadID;
	HANDLE hThread = ::CreateThread(NULL,0,ThreadProc,NULL,0,&threadID);
	if (hThread)
	{
		CloseHandle(hThread);
	}
	
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	HWND hWnd;
	hWnd = hge->System_GetStateHwnd(HGE_HWND);
	::PostMessage(hWnd,WM_DESTROY,0,0);
	hge->Release();
	Sleep(100);
	return FALSE;
}
