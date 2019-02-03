#ifndef XHSMDLLHELP__H__
#define XHSMDLLHELP__H__


#ifdef XHSMDLL_EXPORTS
#define DLL_CLASS __declspec(dllexport)
#else
#define DLL_CLASS __declspec(dllimport)
#endif

//#ifndef SMHELP__H__
typedef void (*DEALMESSAGE2) (int cmd,void* pData);//pData ���ݲ����ͷţ��ڲ�����
//#endif//SMHELP__H__


class DLL_CLASS CXHSMDllHelp
{
public:
public:
	//ע��ص�����
	static void InitSMHelp(DEALMESSAGE2 pDealMessage=NULL);

	//(xingguang.exe ���ڵ�Ŀ¼,"xingguang.exe")
	CXHSMDllHelp(const char* exeDir,const char* exeName,bool bNeedStart=true);
	virtual ~CXHSMDllHelp(void);
	
	//�ر��ǹ�
	void PostClose();
private:
	bool KillProcessFromName(const char* pProcessName);
public:
	static DEALMESSAGE2 m_pDealMessage;
private:
	char m_exeName[260];
};

#endif//XHSMDLLHELP__H__
