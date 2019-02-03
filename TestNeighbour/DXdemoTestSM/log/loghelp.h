#ifndef LOGHELP__H__GLP
#define LOGHELP__H__GLP
#include <sstream>

#ifdef DLL_EXPORT
#define DLL_CLASS __declspec(dllexport)
#else
#define DLL_CLASS __declspec(dllimport)
#endif

enum eLogLevel {
	LL_INFO, LL_WARNING, LL_ERROR
};

class DLL_CLASS LogHelp
{
public:
	//����ʵ��ǰ����Ҫ�����Init���е��ã�
	//logfile ���������־�ļ�
	static void InitLogHelp(/*in*/const char* logfile);

	//����
	//file������ļ���,
	//line�������
	LogHelp(const char* file,int line,eLogLevel ell);
	//����
	//д����־
	~LogHelp();

	//��ȡostream
	std::ostream& stream();
private:
	std::ostringstream slog_;
};


//���º���Ҫ�ȵ���InitLogHelp֮�����������
#define LOGH(file,line,ell) LogHelp(file,line,ell).stream()
#define LOGH_INFO	LOGH(__FILE__,__LINE__,LL_INFO) << __FUNCTION__ << "):"
#define LOGH_WARN	LOGH(__FILE__,__LINE__,LL_WARNING) << __FUNCTION__ << "):"
#define LOGH_ERRO	LOGH(__FILE__,__LINE__,LL_ERROR) << __FUNCTION__ << "):"
//////////////////////////////////////////////////////////////////////////info
#define LOG_IF0(v0) LOGH_INFO << v0
#define LOG_IFN0(v0) LOG_IF0(v0) << ".\n"

#define LOG_IF1(v1) LOGH_INFO << #v1 <<"=" <<v1
#define LOG_IFN1(v1) LOG_IF1(v1) << ".\n"
#define LOG_IF2(v1,v2) LOG_IF1(v1) << "," << #v2 <<"=" <<v2 
#define LOG_IFN2(v1,v2) LOG_IF2(v1,v2) << ".\n"
#define LOG_IFN3(v1,v2,v3) LOG_IF2(v1,v2) << ","  << #v3 <<"=" <<v3 << ".\n"

#define LOG_IFS2(v1,v2) LOGH_INFO << v1 <<"=" <<v2 << ".\n"
//////////////////////////////////////////////////////////////////////////warning
#define LOG_WR0(v0) LOGH_WARN << v0
#define LOG_WRN0(v0) LOG_WR0(v0) << ".\n"

#define LOG_WR1(v1) LOGH_WARN << #v1 <<"=" <<v1
#define LOG_WRN1(v1) LOG_WR1(v1) << ".\n"
#define LOG_WR2(v1,v2) LOG_WR1(v1) << "," << #v2 <<"=" <<v2
#define LOG_WRN2(v1,v2) LOG_WR2(v1,v2) << ".\n"
#define LOG_WRN3(v1,v2,v3) LOG_WR2(v1,v2) << "," << #v3 <<"=" <<v3 << ".\n"

#define LOG_WRS2(v1,v2) LOGH_WARN << v1 << "=" << v2 << ".\n"
//////////////////////////////////////////////////////////////////////////error
#define LOG_ER0(v0) LOGH_ERRO << v0
#define LOG_ERN0(v0) LOG_ER0(v0) << ".\n"

#define LOG_ER1(v1) LOGH_ERRO << #v1 <<"=" <<v1
#define LOG_ERN1(v1) LOG_ER1(v1) << ".\n"
#define LOG_ER2(v1,v2) LOG_ER1(v1) << "," << #v2 <<"=" <<v2 
#define LOG_ERN2(v1,v2) LOG_ER2(v1,v2) << ".\n"
#define LOG_ERN3(v1,v2,v3) LOG_ER2(v1,v2) << "," << #v3 <<"=" <<v3 << ".\n"

#define LOG_ERS2(v1,v2) LOGH_ERRO << v1 <<"=" <<v2 << ".\n"

#endif//LOGHELP__H__GLP