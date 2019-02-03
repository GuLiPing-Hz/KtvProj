#ifndef XHSMDLLHELP__H__
#define XHSMDLLHELP__H__

#include "XHHead.h"

#ifdef XGSMDLL_EXPORTS
#define DLL_CLASS __declspec(dllexport)
#else
#define DLL_CLASS __declspec(dllimport)
#endif

//#ifndef SMHELP__H__
/*
	�ص��ӿڶ���
	@param cmd [in]: �����ʶ
	@param pData [in]: ����Э�����ݣ�����Ҫ�ͷţ��ڲ�����
*/
typedef void (*DEALMESSAGE2) (int cmd,void* pData);
//#endif//SMHELP__H__


class DLL_CLASS CXGSMHelp
{
public:
public:
	//ע��ص�����
	static void InitSMHelp(DEALMESSAGE2 pDealMessage=NULL);

	//���캯�������ȥ�򿪹����ڴ棬PostMessage��MSG_OPENSHAREMEMORY��name��
	CXGSMHelp(const char* name);
	virtual ~CXGSMHelp(void);

	//������Ϣ,������Ϣ�μ�XHHead.h  e_CMD_P2C
	/*
	@param cmd[in] : ������Ϣ����μ�XHHead.h  e_CMD_P2C
	@param pData[in] : �������ݣ��ṹ�μ� XHHead.h  e_CMD_P2C��ÿ�������˵����
		�ڴ���Ҫ�Լ�ά���������ﲻ�����ͷš�
	*/
	void PostMessage(int cmd,void* pData);
private:
	bool KillProcessFromName(const char* pProcessName);
public:
	static DEALMESSAGE2 m_pDealMessage;
private:
	char m_exeName[260];
};

#endif//XHSMDLLHELP__H__
