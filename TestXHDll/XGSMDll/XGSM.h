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
	回调接口定义
	@param cmd [in]: 命令标识
	@param pData [in]: 具体协议数据，不需要释放，内部处理
*/
typedef void (*DEALMESSAGE2) (int cmd,void* pData);
//#endif//SMHELP__H__


class DLL_CLASS CXGSMHelp
{
public:
public:
	//注册回调函数
	static void InitSMHelp(DEALMESSAGE2 pDealMessage=NULL);

	//构造函数里面会去打开共享内存，PostMessage（MSG_OPENSHAREMEMORY，name）
	CXGSMHelp(const char* name);
	virtual ~CXGSMHelp(void);

	//发送消息,具体消息参见XHHead.h  e_CMD_P2C
	/*
	@param cmd[in] : 具体消息命令参见XHHead.h  e_CMD_P2C
	@param pData[in] : 具体数据，结构参见 XHHead.h  e_CMD_P2C中每个命令的说明，
		内存需要自己维护，函数里不会做释放。
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
