/*
 * HttpDownload.h
 *
 *  Created on: 2014-5-21
 *      Author: glp
 */

#ifndef HTTPDOWNLOAD_H_
#define HTTPDOWNLOAD_H_
#include "ClientSocket.h"
#include "Timer.h"
#include "HttpDownloadType.h"

namespace NetworkUtil {

typedef struct _DownloadState
{
	// 下载状态
	typedef enum _eDownloadState
	{
		DS_IDLE,//空闲
		DS_BUSY,//下载中
		DS_STOP,// 暂停下载
	}eDownloadState;
	volatile eDownloadState 		state;//下载状态
	long											downsize;//已经下载大小
	long											filesize;//文件总大小

	_DownloadState():state(DS_IDLE),downsize(0L),filesize(0L){}
	float progress()const{ if(filesize==0L)return 0.0f;return downsize*1.0f/filesize;}
}DownloadState;

class Reactor;
class CHttpDownloadMgr;
class CHttpDownload : public ClientSocket
{
public:
	enum DOWNLOAD_STATE
	{
		DOWNLOAD_ERROR = -1,
		DOWNLOAD_OK = 0,
		DOWNLOAD_FINISH = 1,
		DOWNLOAD_REDIRECTURL = 2,
		DOWNLOAD_FILEEXIST =  3,
		RANGE_NOT_SATISFIABLE =4,
		NO_RESPONSE_DATA = 5,
		RESPONSE_HEADER_NOT_FINISH = 7,
	};
public:
	CHttpDownload(Reactor *pReactor,CHttpDownloadMgr* pMgr);
	virtual ~CHttpDownload();
    /*
     校验文件MD5
     @param fileName[in]:文件名
     @param md5[in]:用于比较的md5值
     
     @return : 如果md5为空，或者文件校验MD5值一致的话返回true;其他返回false
     */
    static bool checkMD5(const std::string fileName,const  std::string md5);
private:
	bool validHostChar( char c );
	void parseURL( const char* url, char * protocol, int lprotocol, char * host, int lhost, char * request, int lrequest, short * port );
	int urlEncode( const char * str, const int strSize, char * result, const int resultSize );
	int formatRequestHeader( char* SendHeader, int SendHeaderSize, char* Request, int RequestSize, char* Host
			, int From, int To, char* Data, long DataSize );

	long getFileSize(const char* file);
	int getFieldValue(const char *szRequestHeader, const char *szSection, char *nValue, const int nMaxLen);
	int parseResponseHeader();
	int getResponseHeader(DataBlock* pDb);
	int recvResponseHeader(DataBlock* pDb);

	bool onFinishDowndFile();
	DOWNLOAD_STATE dealWithTransferEncodingAndCommon(DataBlock* pDb);
	virtual DOWNLOAD_STATE onDownloadSaveData(const char* pData,int nLenData);
	virtual void onDownloadByNewUrl();
	virtual void onDownloadError();
	virtual void onDownloadFinish();
protected:
	virtual void onFDRead();

	virtual void onSocketConnect();
	virtual void onSocketConnectTimeout();
	virtual void onSocketConnectError(int errCode);
	virtual void onSocketClose();
	virtual void onSocketRecvError(int errCode);
	virtual void onSocketSendError(int errCode);
	virtual void onNetLevelError(int errCode);
public:
	//当下载出错的时候pInfo可为NULL，如果是新加入的下载，必须指定pInfo
	void initDownload(const char* url,ONPROGRESS funProgress,DownloadInfo* pInfo);
	void uninitDownload();
	bool startDownload();
	bool isIdle(){return (m_gDownloadState.state != DownloadState::DS_BUSY);}
private:
	CHttpDownloadMgr*               m_pMgr;
	bool 						m_bInit;
	bool							m_isSaveFile;
	bool							m_isNeedSaveData;
	bool							m_bTransferEncodingChunked;
	int 							m_nTryDownload;

	std::string				m_sOldUrl;

	std::string 				m_sUrl;
	char							m_sProtocol[20];
	char 						m_sHost[256];
	char							m_sIp[256];
	char							m_sRequest[1024];	// 协议、服务器主机、请求内容
	short						m_nPort;
	//断点续传
	int							m_nFrom;

	char*						m_sPostData;
	int							m_nPostDataLen;

	char*						m_sRequestHeader;
	int 							m_nRequestHeaderSize;
	char*						m_sResponseHeader;
	int 							m_nResponseHeaderSize;

	bool							m_bIsRecvResponseHeader;

	//保存下载信息
	DownloadInfo         m_gDownloadInfo;
	//chunk 定义,如果http回应里面没有content lenght的处理
	long							m_nCurChunkSize;// 当前块的大小。
	long							m_nCurChunkDownloadSize;// 当前块已经下载字节数。
	long							m_nTotalChunkSize;// 所有块总共的累计字节数。
	//保存下载状态
	DownloadState                  m_gDownloadState;

	ONPROGRESS                    m_funProgress;

	FILE*							m_fileTmp;
	COS_Timer					m_gTimer;
};

} /* namespace NetworkUtil */
#endif /* HTTPDOWNLOAD_H_ */
