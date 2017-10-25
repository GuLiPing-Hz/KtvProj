/*
 * HttpDownload.cpp
 *
 *  Created on: 2014-5-21
 *      Author: glp
 */

#include "HttpDownload.h"
#include "../ResultData.h"
#include "HttpDownloadMgr.h"
#ifdef WIN32
#define R_OK  4  /* Read */
#define W_OK  2  /* Write */
#define X_OK  1  /* Execute */
#define F_OK  0  /* Existence */
#include <io.h>
#include <direct.h>
#else
#include <strings.h>
#include <unistd.h>
#include <ctype.h>
#endif
#include "Md5.h"
#include <assert.h>
#include "WrapUnzip.h"

#include "config.h"
#ifndef NETUTIL_ANDROID
#define LOGI printf
#define LOGE printf
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif//max
#ifndef min
#define min(a,b) (((a)>(b))?(b):(a))
#endif//min

#define PER_RECV_BUF_SIZE			65535/*1024 * 25*/   // 接收缓冲区大小
#define TRY_MAX_TIMES 3

namespace NetworkUtil {

CHttpDownload::CHttpDownload(Reactor *pReactor,CHttpDownloadMgr* pMgr)
:ClientSocket(pReactor)
,m_pMgr(pMgr)
,m_bInit(false)
,m_nPort(0)
,m_sRequestHeader(NULL)
,m_sResponseHeader(NULL)
,m_sPostData(NULL)
,m_nPostDataLen(0)
,m_nFrom(0)
,m_bIsRecvResponseHeader(false)
,m_isNeedSaveData(true)
,m_isSaveFile(true)
,m_nTryDownload(0)
,m_funProgress(NULL)
,m_fileTmp(NULL)
,m_bTransferEncodingChunked(false)
,m_nCurChunkSize(0)
,m_nCurChunkDownloadSize(0)
,m_nTotalChunkSize(0)
{
	assert(m_pMgr != NULL);
	m_sProtocol[0] = 0;
	m_sHost[0] = 0;
	m_sIp[0] = 0;
	m_sRequest[0] = 0;

	m_nRequestHeaderSize = 4096;
	m_nResponseHeaderSize =4096;

	m_sRequestHeader = new char[m_nRequestHeaderSize];
	m_sResponseHeader = new char[m_nResponseHeaderSize];

	memset( m_sRequestHeader, 0,  m_nRequestHeaderSize);
	memset( m_sResponseHeader, 0, m_nResponseHeaderSize );
}

CHttpDownload::~CHttpDownload() {
	uninitDownload();
	if(m_sResponseHeader)
	{
		delete m_sResponseHeader;
		m_sResponseHeader = NULL;
	}
	if(m_sRequestHeader)
	{
		delete m_sRequestHeader;
		m_sRequestHeader = NULL;
	}
}

bool isAlpha(char c)
{
	return (c>='A'&&c<='Z') || (c>='a'&&c<='z');
}

bool isDigit(char c)
{
	return c>='0'&&c<='9';
}

bool CHttpDownload::validHostChar( char c )
{
	return ( isAlpha(c) || isDigit(c) || c=='-' || c=='.' || c==':' );
}

void CHttpDownload::initDownload(const char* url,ONPROGRESS funProgress,DownloadInfo* pInfo)
{
	m_funProgress = funProgress;
	m_sUrl = url;
	parseURL(m_sUrl.c_str(),m_sProtocol,sizeof(m_sProtocol),m_sHost,sizeof(m_sHost)
			,m_sRequest,sizeof(m_sRequest),&m_nPort);
	strcpy(m_sIp,GetIpv4FromHostname(m_sHost));//从域名中解析出IP地址
	if(pInfo)
	{
		m_gDownloadInfo.download = pInfo->download;
		strcpy(m_gDownloadInfo.MD5,pInfo->MD5);
		strcpy(m_gDownloadInfo.fileName , pInfo->fileName);
		strcpy(m_gDownloadInfo.unzipDir , pInfo->unzipDir);
		m_gDownloadInfo.mandatory = pInfo->mandatory;
        static unsigned int s_count = 0;
        s_count ++;
        sprintf(m_gDownloadInfo.tmpFileName,"%s_tmp_%d",m_gDownloadInfo.fileName,s_count);
		if(pInfo->mandatory)
		{
			remove(m_gDownloadInfo.fileName);
			remove(m_gDownloadInfo.tmpFileName);
			m_gDownloadInfo.fileSize = 0;
			m_gDownloadInfo.tmpFileSize = 0;
		}
		else
		{
			m_gDownloadInfo.fileSize = getFileSize(m_gDownloadInfo.fileName);
			m_gDownloadInfo.tmpFileSize = getFileSize(m_gDownloadInfo.tmpFileName);
		}

		m_gDownloadInfo.saveBuf = pInfo->saveBuf;
		m_gDownloadInfo.saveBufLen = pInfo->saveBufLen;
		m_gDownloadInfo.saveTmpBuf = pInfo->saveTmpBuf;
	}

	//只支持Http下载
#ifdef WIN32
	if(_stricmp(m_sProtocol,"http") == 0)
#else
	if(strcasecmp(m_sProtocol,"http") == 0)
#endif
		m_bInit = true;
}

void CHttpDownload::uninitDownload()
{
	m_bIsRecvResponseHeader = false;
	m_gTimer.stop2();
	m_sUrl = "";
	m_sProtocol[0] = 0;
	m_sHost[0] = 0;
	m_sIp[0] = 0;
	m_sRequest[0] = 0;
	m_nPort = 0;
	m_nFrom = 0;
	m_funProgress = NULL;

	if(m_fileTmp)
	{
		fclose(m_fileTmp);
		m_fileTmp = NULL;
	}

	closeSocket();
	memset(&m_gDownloadState,0,sizeof(m_gDownloadState));
	m_gDownloadState.state = DownloadState::DS_IDLE;
}

bool CHttpDownload::startDownload()
{
	if(!m_bInit)
		return false;

	if(!m_isNeedSaveData)
		return false;

	if(Connect(m_sIp,m_nPort,60) == 0)
	{
		m_gDownloadState.state = DownloadState::DS_BUSY;
		//计时开始
		m_gTimer.start2();
		return true;
	}
	return false;
}

void CHttpDownload::onFDRead()
{
	char buf[PER_RECV_BUF_SIZE] = {0};/* 16*1024 */
	int len = ::recv(m_fd, buf, sizeof(buf), 0);//接收网络数据

	if (len == 0)
	{
		closeSocket();
		onSocketClose();
		return;
	}

	if (len == SOCKET_ERROR)
	{
#ifdef WIN32

		DWORD derrno = GetLastError();
		if (derrno != WSAEWOULDBLOCK)
		{
			closeSocket();
			onSocketRecvError(derrno);
		}
#else//Linux
		int errorcode = errno;
		if(errorcode!=EAGAIN)
		{
			closeSocket();
			onSocketRecvError(errorcode);
		}
#endif
		return;
	}

	if (getRB()->append(buf,len) < 0)
	{
		closeSocket();
		onNetLevelError(Net_RecvBuf_Error);
		return;
	}

	if (!m_bIsRecvResponseHeader)
	{
		int ret = recvResponseHeader( getRB() );

		if ( ret == DOWNLOAD_OK )
			m_bIsRecvResponseHeader = true;
		else if ( ret == RESPONSE_HEADER_NOT_FINISH )//服务器响应头还没有接受完整
			return ;
		else if( ret == DOWNLOAD_REDIRECTURL )//下载地址重定向
		{
			onDownloadByNewUrl();
			return ;//REDIRECT_URL;
		}
		else if( ret == DOWNLOAD_FILEEXIST )//下载文件已经存在
		{
			onDownloadFinish();
			return ;//FILE_IS_EXIST;
		}
		else if( ret == DOWNLOAD_ERROR ||  ret == RANGE_NOT_SATISFIABLE )
		{
			onDownloadError();
			return ;//DOWNLOAD_ERROR;
		}
		else if (ret == NO_RESPONSE_DATA)//不需要接受数据，关闭socket
		{
			m_bIsRecvResponseHeader = true;
			onDownloadFinish();
			return ;//NO_RESPONSE_DATA;
		}
	}
	//else//头部数据接收完毕之后还有其他的数据
	{
		int ret2 = dealWithTransferEncodingAndCommon( getRB() );

		if ( ret2 == DOWNLOAD_ERROR)
		{
			onDownloadError();
			return ;//DOWNLOAD_ERROR;
		}
		else if ( ret2 == DOWNLOAD_FINISH )
		{
			onDownloadFinish();
			return ;//DOWNLOAD_FINISH;
		}
	}

}

void CHttpDownload::onSocketConnect()
{
	if ( m_isSaveFile )
		m_nFrom = getFileSize(m_gDownloadInfo.tmpFileName);
	else
		m_nFrom = 0;
	int len = formatRequestHeader(m_sRequestHeader,m_nRequestHeaderSize,m_sRequest,sizeof(m_sRequest),
			m_sHost,m_nFrom,0,m_sPostData,m_nPostDataLen);

	//发送请求
	SendBuf(m_sRequestHeader,len);
}
void CHttpDownload::onSocketConnectTimeout()
{
}
void CHttpDownload::onSocketConnectError(int errCode)
{
}
void CHttpDownload::onSocketClose()
{
}
void CHttpDownload::onSocketRecvError(int errCode)
{
}
void CHttpDownload::onSocketSendError(int errCode)
{
}
void CHttpDownload::onNetLevelError(int errCode)
{
}

void CHttpDownload::parseURL( const char* url, char * protocol, int lprotocol, char * host, int lhost, char * request, int lrequest, short * port )
{
	if (!url || !protocol || !host || !request || !port )
		return;

	char * ptr = NULL, * ptr2 = NULL;
	char work[4096] = {0};
	*protocol = 0;
	*host = 0;
	*request = 0;
	*port = 80;
	strncpy(work,url,sizeof(work)-1);

	ptr = strchr( work, ':' ); // find protocol if any
	if(ptr != NULL)
	{
		(* ptr) = 0;
		ptr++;
		sprintf(protocol, "%s", work );
	}
	else
	{
		sprintf(protocol, "%s", "HTTP" );
		ptr = work;
	}
	if(( * ptr == '/' )&&( *( ptr+1 ) == '/' )) // skip past opening /'s
	{
		ptr += 2;
	}

	ptr2 = ptr; // find host
	while( validHostChar(* ptr2) && * ptr2 )
	{
		ptr2++;
	}
	(* ptr2) = 0;
	sprintf( host,"%s", ptr);
	char temp[1024] = {0};
	sprintf(temp, "%s", url + ( ptr2 - work ) ); // find the request

	//将请求内容转义成网络字符串
	memset(request, 0 ,lrequest);
	urlEncode(temp, sizeof(temp), request, lrequest );

	ptr = strchr(host,':'); // find the port number, if any
	if( ptr != NULL)
	{
		(* ptr) = 0;
		(* port) = (short)(atoi( ptr+1 ));
	}
}

int CHttpDownload::urlEncode( const char * str, const int strSize, char * result, const int resultSize )
{
	if ( ( !str ) || ( !result ) || ( strSize <= 0 ) || ( resultSize <= 0 ) )
		return 0;

	int j = 0;// for result index
	for ( int i = 0; ( i < strSize ) && ( j < resultSize ); i ++ )
	{
		char ch = str[ i ];
		if ( ( ch >= 'A' ) && ( ch <= 'Z' ) )
		{
			result[ j ++ ] = ch;
		}
		else if ( ( ch >= 'a' ) && ( ch <= 'z' ) )
		{
			result[ j ++ ] = ch;
		}
		else if ( ( ch >= '0' ) && ( ch <= '9' ) )
		{
			result[ j ++ ] = ch;
		}
		//else if ( ch == ' ' )
		//{
		//	result[ j ++ ] = '+';
		//}
		else if ( ch == ':' )
		{
			result[ j ++ ] = ch;
		}
		else if ( ch == '/' )
		{
			result[ j ++ ] = ch;
		}
		else if ( ch == '=' )
		{
			result[ j ++ ] = ch;
		}
		else if ( ch == '.' )
		{
			result[ j ++ ] = ch;
		}
		else if ( ch == '_' )
		{
			result[ j ++ ] = ch;
		}
		else if ( ch == '?' )
		{
			result[ j ++ ] = ch;
		}
		else if ( ch == '&' )
		{
			result[ j ++ ] = ch;
		}
		else if ( ch == '\0' )
		{
				result[ j ++ ] = ch;
				return j;
		}
		else
		{
			if ( j + 3 < resultSize )
			{
				sprintf( result + j, "%%%02X", ( unsigned char )ch );
				j += 3;
			}
			else
				return 0;
		}
	}

	result[ j ] = '\0';

	return j;
}

long CHttpDownload::getFileSize(const char* file)
{
	if(!file)
		return 0;
	FILE* fp = fopen(file,"r");
	if(!fp)
		return 0;
	fseek(fp,0,SEEK_END);
	long size = ftell(fp);
	fclose(fp);
	return size;
}

int CHttpDownload::getFieldValue(const char *szRequestHeader, const char *szSection, char *nValue, const int nMaxLen)
{
	if ( szRequestHeader == NULL || szSection == NULL || nValue == NULL)
		return -1;
	const char *nFindStr = strstr( szRequestHeader, szSection);
	if (nFindStr == NULL)
		return -2;
	int nPos = 0;
	nPos += (int)strlen(szSection);
	nPos += 2;
	int n = 0;
	while(nFindStr[nPos] != '\r' && n < nMaxLen)
	{
		nValue[n] = nFindStr[nPos];
		n++;
		nPos++;
	}
	if (nFindStr[nPos] != '\r' || nValue[nMaxLen - 1] != '\0')  //没有读取到换行符“/r/n”或字符串没有以“\0”结尾(最后一个字符也存了数据)
	{
		nValue[nMaxLen - 1] = '\0';
		return -1;
	}
	else
	{
		return n;
	}
}

int CHttpDownload::parseResponseHeader()
{
	//判断服务器的响应是否成功
	char cSucess[4] = {0};
	getFieldValue(m_sResponseHeader,"HTTP/1.", cSucess, sizeof(cSucess));
	int iSucess = atoi(cSucess);
	if (iSucess < 200 || iSucess > 206 )
	{
		if ( iSucess == 301 || iSucess == 302 )
		{
			//重定向URL
			char cNewUrl[1024] = {0};
			if( getFieldValue(m_sResponseHeader,"Location",cNewUrl,sizeof(cNewUrl)) > 0)
			{
				m_sOldUrl = m_sUrl;
				m_sUrl = cNewUrl;
				return DOWNLOAD_REDIRECTURL;
			}
			else
			{
				//接收重定向URL失败
				return DOWNLOAD_ERROR;
			}
		}
		else if ( iSucess == 416 )
		{
			//请求的文件范围超出，临时文件出错造成
			//删除临时文件，重试下载
			remove(m_gDownloadInfo.tmpFileName);
			return RANGE_NOT_SATISFIABLE;
		}
		else
		{
			//服务器响应连接不成功
			return DOWNLOAD_ERROR;
		}

	}
	//读取文件大小
	char mValue[260] = {0};
	long content_size = 0;
	int nRet = getFieldValue(m_sResponseHeader,"Content-Length", mValue, sizeof(mValue));
	if (  nRet <= 0 )
	{
		if(nRet == -2)
		{
			if ( getFieldValue(m_sResponseHeader,"Transfer-Encoding",mValue,sizeof(mValue)) > 0 ) {
				m_bTransferEncodingChunked = true;
				content_size = 0x7fffffff;
			}
		}
		else// 服务器没有返回文件的大小
			return DOWNLOAD_ERROR;
	}
	else
		content_size = atol(mValue);

	if (content_size > 0 )
	{
		//设置文件的总大小
		m_gDownloadState.filesize = m_gDownloadState.downsize +content_size;

		if ( m_isSaveFile )
		{
			if(m_gDownloadInfo.fileSize == m_gDownloadState.filesize)//文件已经存在
			{
				remove(m_gDownloadInfo.tmpFileName);
				return DOWNLOAD_FILEEXIST;
			}
		}
	}
	else if (content_size == 0)
	{
		return NO_RESPONSE_DATA;
	}
	else
	{
		// 服务器返回文件的大小出错
		return DOWNLOAD_ERROR;
	}
	return DOWNLOAD_OK;
}

int CHttpDownload::getResponseHeader(DataBlock* pDb/*char* buf*/)
{
	if (!pDb)
		return 0;

	int nIndex = 0;
	char* pData = pDb->getBuf();
	while( nIndex < pDb->getPos() )
	{
		if ( nIndex >= m_nResponseHeaderSize ) {
			m_nResponseHeaderSize = m_nResponseHeaderSize * 2;

			// 大于64K，需要查一下Response最大允许的长度。可能不一定是最大64K
			if ( m_nResponseHeaderSize > (PER_RECV_BUF_SIZE+1) ) {
				return 0;
			}
			char * pTemp = new char[m_nResponseHeaderSize];
			memset( pTemp, 0, m_nResponseHeaderSize );
			delete m_sResponseHeader;
			m_sResponseHeader = pTemp;
		}

		m_sResponseHeader[nIndex] = pData[nIndex];
		nIndex++;

		if( nIndex >= 4)
		{
			if(m_sResponseHeader[nIndex - 4] == '\r'
				&& m_sResponseHeader[nIndex - 3] == '\n'
				&& m_sResponseHeader[nIndex - 2] == '\r'
				&& m_sResponseHeader[nIndex - 1] == '\n')
			{
				return nIndex;
			}
		}
	}

	return 0;
}

int CHttpDownload::recvResponseHeader(/*char* buf, int buflen*/DataBlock* pDb)
{
	if (!pDb)
		return RESPONSE_HEADER_NOT_FINISH;

	// 读取http响应头
	int nHeadLen = getResponseHeader( /*buf*/pDb );
	if ( nHeadLen == 0 )	//服务器响应头还没有接受完整
	{
		return RESPONSE_HEADER_NOT_FINISH;
	}

	// 重置数据缓存中的数据
	int nRemainLen = pDb->getPos() - nHeadLen;
	if(nRemainLen == 0)
		pDb->initPos();
	else
	{
		char* pData = pDb->getBuf();
		pDb->copy(0,pData+nHeadLen,nRemainLen);
	}

	//文件头接收完整，开始解析
	return parseResponseHeader();
}

bool CHttpDownload::checkMD5(const std::string fileName,const  std::string md5)
{
	if(md5 == "")
		return true;

	std::string resultMd5 = MD5::FileDigest(fileName);

#ifdef WIN32
	if(_stricmp(resultMd5.c_str(),md5.c_str()) == 0)
#else
	if(strcasecmp(resultMd5.c_str(),md5.c_str()) == 0)
#endif//WIN32
		return true;
	else
		return false;
}

bool CHttpDownload::onFinishDowndFile()
{
	if(m_fileTmp)
	{
		fclose(m_fileTmp);
		m_fileTmp = NULL;
	}

	if(checkMD5(m_gDownloadInfo.tmpFileName,m_gDownloadInfo.MD5))
	{//文件正确
		FILE* fp = fopen(m_gDownloadInfo.fileName,"rb");
		if(fp)//旧文件存在
		{
			fclose(fp);
			remove(m_gDownloadInfo.fileName);
		}
		//移动文件
		rename(m_gDownloadInfo.tmpFileName,m_gDownloadInfo.fileName);
		return true;
	}
	else
	{//文件不正确，
		//删除临时文件
		remove(m_gDownloadInfo.tmpFileName);
		return false;
	}
}

CHttpDownload::DOWNLOAD_STATE CHttpDownload::dealWithTransferEncodingAndCommon(DataBlock* pDb)
{
	CHttpDownload::DOWNLOAD_STATE nRetState = DOWNLOAD_OK;
	if(!pDb)
		return nRetState;
	
	int nCurLenData = 0;
	int nCurRealLen = 0;
	do
	{
		if(m_bTransferEncodingChunked)
		{//如果是TransferEncoding传输
			if(m_nCurChunkSize == m_nCurChunkDownloadSize)//获取新的一段chunk, 首先会是 0==0的情况
			{
				m_nCurChunkDownloadSize = 0;//复原当前chunk的下载量

				const char* pData = pDb->getBuf();
				const char* pTmp = pData;
				int nLenData = pDb->getPos();

				char sChuckLen[260] = {0};
				int nLenTmp = 0;
				while(*pTmp != '\r')
				{
					nLenTmp++;
					if(nLenTmp > nLenData)
						return DOWNLOAD_OK;//数据没完整，不处理
					pTmp++;
				}
				int nLenSize = pTmp-pData;
				memcpy(sChuckLen,pData,nLenSize);
				m_nCurChunkSize = -1;
				sscanf(sChuckLen,"%lx",&m_nCurChunkSize);
				if(m_nCurChunkSize == -1)
				{
					LOGE("%s : read chunk size error\n",__FUNCTION__);
					return DOWNLOAD_ERROR;
				}

				if(m_nCurChunkSize == 0)//chunk 的结束标识
				{
					pDb->initPos();
					if(onFinishDowndFile())
						return DOWNLOAD_FINISH;
					else
						return DOWNLOAD_ERROR;
				}

				pDb->copy(0,pTmp+2,nLenData-nLenSize-2);//2 指 "\r\n"
			}
		}

		nCurLenData = pDb->getPos();
		if(m_bTransferEncodingChunked)
			nCurRealLen = min(m_nCurChunkSize-m_nCurChunkDownloadSize,nCurLenData);
		else
			nCurRealLen = nCurLenData;

		nRetState = onDownloadSaveData(pDb->getBuf(),nCurRealLen);
		if(DOWNLOAD_ERROR == nRetState || DOWNLOAD_FINISH == nRetState)//报告错误或结束，停止循环
		{
			pDb->initPos();
			break ;
		}
		
		int nCurRealLenAndEndS = nCurRealLen+2;//2 : 代指需要跳过每个chunk的\r\n
		int nRemainLen = nCurLenData-nCurRealLenAndEndS;
		if(nRemainLen>0)
			pDb->copy(0,pDb->getBuf()+nCurRealLenAndEndS,nRemainLen);
		else
			pDb->initPos();

		if(m_bTransferEncodingChunked)
			m_nCurChunkDownloadSize += nCurRealLen;

		nCurLenData = nRemainLen;//定位新长度
	}while(nCurLenData > 0);
	
	return nRetState;
}

CHttpDownload::DOWNLOAD_STATE CHttpDownload::onDownloadSaveData(const char* pData,int nLenData)
{
	if(!pData)
		return DOWNLOAD_ERROR;

	if(m_gDownloadInfo.download)//下载
	{
		if(!m_fileTmp)
		{
			//@注：文件打开必须是2进制的方式，因为接收的Tcp流也是2进制
			m_fileTmp = fopen(m_gDownloadInfo.tmpFileName,"ab");

			if(!m_fileTmp)
			{
				//循环创建目录
				CUnZip::createDir(m_gDownloadInfo.tmpFileName,false);

				//再次尝试打开
				m_fileTmp = fopen(m_gDownloadInfo.tmpFileName,"ab");
			}
		}

		if(!m_fileTmp)
			return DOWNLOAD_ERROR;

		int writtenLen = 0;
		int dataLen = nLenData;
		int remainLen = dataLen;
		do
		{
			int written = fwrite(pData+writtenLen,1,remainLen,m_fileTmp);
			writtenLen += written;
			remainLen -= written;
		}while(remainLen > 0);
		fflush(m_fileTmp);
		//pDb->initPos();
		//设置下载量
		m_gDownloadState.downsize += dataLen;
		double elapsedms = m_gTimer.getLastElapsedTimerInMilliSec();
		double speed = dataLen / elapsedms;
		if(m_funProgress)
		{
			float fProgress = m_gDownloadState.progress();
			fProgress = fProgress>1.0f?1.0f:fProgress;
			m_funProgress(fProgress,speed);
		}

		if ( m_gDownloadState.downsize >= m_gDownloadState.filesize )
		{
			if(onFinishDowndFile())
				return DOWNLOAD_FINISH;
			else
				return DOWNLOAD_ERROR;
		}
	}
	else//保存到内存
	{
		if(!m_gDownloadInfo.saveBuf)
			return DOWNLOAD_ERROR;

		int len = nLenData;
		if ( m_gDownloadInfo.saveBufLen >= (m_gDownloadState.downsize + len) )//内存可以存放
		{
			memcpy(m_gDownloadInfo.saveTmpBuf,pData,len);
			m_gDownloadInfo.saveTmpBuf += len;
			//pDb->initPos();
			//设置下载量
			m_gDownloadState.downsize += len;

			if ( m_gDownloadState.downsize >= m_gDownloadState.filesize )
			{
				return DOWNLOAD_FINISH;
			}
		}
		else
		{
			LOGE("%s : the memory to save http response is non't enough!\n",__FUNCTION__);
			return DOWNLOAD_ERROR;
		}
	}

	return DOWNLOAD_OK;
}

void CHttpDownload::onDownloadByNewUrl()
{
	std::string tryUrlAgain = m_sUrl;
	ONPROGRESS funProgress = m_funProgress;
	uninitDownload();

	initDownload(tryUrlAgain.c_str(),funProgress,NULL);
	if(!startDownload())
		onDownloadError();
}

void CHttpDownload::onDownloadError()
{
	m_gDownloadState.state = DownloadState::DS_IDLE;
	//再尝试几次
	m_nTryDownload ++;

	if(TRY_MAX_TIMES > m_nTryDownload)
	{
		std::string tryUrlAgain = m_sUrl;
		ONPROGRESS funProgress = m_funProgress;
		uninitDownload();
		//重新尝试下载
		initDownload(tryUrlAgain.c_str(),funProgress,NULL);
		if(!startDownload())
			onDownloadError();//继续尝试，直到超过最大次数
	}
	else
	{
		//uninitDownload需要管理类来做，这样可以避免在执行doNext的时候，又增加一个任务，导致当前下载项出现差错
		//uninitDownload();
		//通知下载下一个
		if(m_pMgr)
			m_pMgr->doNext(false,this);
	}

}

void CHttpDownload::onDownloadFinish()
{
	bool bSuccess = true;
	//无密码解压
	if(m_gDownloadInfo.download && strstr(m_gDownloadInfo.fileName,".zip"))
	{
		bSuccess=CUnZip::unZipFile(m_gDownloadInfo.fileName,m_gDownloadInfo.unzipDir,NULL);
		if(!bSuccess)
			LOGE("%s：%s zip file unzip failed\n",__FUNCTION__,m_gDownloadInfo.fileName);
		else//解压成功，把压缩包删除
			remove(m_gDownloadInfo.fileName);
	}

	//uninitDownload需要管理类来做，这样可以避免在执行doNext的时候，又增加一个任务，导致当前下载项出现差错
	//uninitDownload();
	//通知下载下一个
	if(m_pMgr)
		m_pMgr->doNext(bSuccess,this);
}

int CHttpDownload::formatRequestHeader( char* SendHeader, int SendHeaderSize, char* Request, int RequestSize
		, char* Host, int From, int To, char* Data, long DataSize )
{
	if (!SendHeader || !Request || !Host)
	{
		return 0;
	}

	char buffer[512] = {0};
	if( 0 == ( * Request ))
	{
		sprintf( Request, "%s", "/" );
	}
	//第1行:方法,请求的路径,版本
	if( Data )
	{
		strcpy( SendHeader, "POST " );
	}
	else
	{
		strcpy( SendHeader,"GET " );
	}
	strcat( SendHeader, Request );
	strcat( SendHeader, " HTTP/1.1\r\n" );

	if( Data && 0 < DataSize)
	{
		sprintf(SendHeader,"%sContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %ld\r\n"
				,SendHeader,DataSize);
	}

	//第2行:主机
	if ( Host != NULL )
		sprintf(SendHeader,"%sHost: %s\r\n",SendHeader,Host);

	//第3行:接收的数据类型
	strcat(SendHeader,"Accept: */*\r\n");

	//第4行:连接设置,保持
	strcat(SendHeader,"Connection: Keep-Alive\r\n");

	// 测试gzip返回
	strcat( SendHeader,"Accept-Encoding: gzip,deflate,sdch\r\n" );

	//第5行:请求的数据起始字节位置(断点续传)
	if( From > 0 )
	{
		if ( To > 0 )
			sprintf( buffer, "Range: bytes=%d-%d\r\n", From, To );
		else
			sprintf( buffer, "Range: bytes=%d-\r\n", From );
		strcat( SendHeader,buffer );
	}

	// 第6行:空行，表示请求头结束
	strcat( SendHeader, "\r\n" );

	//POST请求的实体数据
	if( Data && 0 < DataSize )
	{
		strcat( SendHeader, Data );
		strcat( SendHeader,"\r\n" );
	}
	int nRet = (int)strlen(SendHeader);
	return nRet;
}

} /* namespace NetworkUtil */
