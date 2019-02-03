#ifndef XHSMDLLHELP__H__
#define XHSMDLLHELP__H__


#ifdef XHSMDLL_EXPORTS
#define DLL_CLASS __declspec(dllexport)
#else
#define DLL_CLASS __declspec(dllimport)
#endif

//#ifndef SMHELP__H__
typedef void (*DEALMESSAGE2) (int cmd,void* pData);//pData 数据不用释放，内部处理
//#endif//SMHELP__H__


class DLL_CLASS CXHSMDllHelp
{
public:
public:
	//注册回调函数
	static void InitSMHelp(DEALMESSAGE2 pDealMessage=NULL);

	//(xingguang.exe 所在的目录,"xingguang.exe")
	CXHSMDllHelp(const char* exeDir,const char* exeName,bool bNeedStart=true);
	virtual ~CXHSMDllHelp(void);
	
	//关闭星光
	void PostClose();
private:
	bool KillProcessFromName(const char* pProcessName);
public:
	static DEALMESSAGE2 m_pDealMessage;
private:
	char m_exeName[260];
};

#endif//XHSMDLLHELP__H__
