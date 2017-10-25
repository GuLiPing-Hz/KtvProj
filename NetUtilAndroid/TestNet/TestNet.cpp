// TestNet.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "NetUtil/RequestBase.h"
#include "TestResponse.h"
#include "xingguang.h"
#include "netutil/localstorage/DataMgr.h"
#include "NetUtil/ChatCompatibility.h"
#include "HttpDownloadMgr.h"
#include "NetUtil/wrap/CFileMgr.h"
#include "NetUtil/xml/tinyxml.h"
#include "NetUtil/ChatCompatibility.h"
#include <string.h>
#include "NetUtil/zlib/zlib.h"
#include "NetUtil/wrap/aes.h"
#include <curl/curl.h>

CDataMgr g_DM;
//RequestBase request;
//CTestResponse response;

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

#include "NetUtil/charset/CharsetCodec.h"

NetworkUtil::CHttpDownloadMgr g_gMgr;

//Bytes/millisecond
void OnProgress(float f,double speed)
{
	printf("\r progress : ");
	for(int i=0;i<((int)f)*20;i++)
	{
		printf(">>");
	}
	speed = speed/1024*1000;//kbps;
	printf(" %.2f,speed: %0.2lf kbps",f,speed);
}

struct NormalDay
{
	int month;
	int day;
};

bool operator<(const NormalDay& a,const NormalDay& b)
{
	if(a.month != b.month)
		return a.month<b.month;
	else
		return a.day<b.day;
}

typedef std::set<NormalDay> SETDAY;
typedef std::list<std::string> LISTSTR;

void ParseDay(const char* buffer,int month,SETDAY& setDay)
{
	if(!buffer)
		return ;

	char tempBuf[300] = {0};
	strcpy(tempBuf,buffer);

	NormalDay nd;
	nd.month = month;

	LISTSTR listDay;

	char* pDay = strtok(tempBuf,",");
	do 
	{
		if(!pDay)
			break;

		listDay.push_back(pDay);

		pDay=strtok(NULL,",");
	} while (true);

	LISTSTR::iterator it = listDay.begin();
	for(;it!=listDay.end();it++)
	{
		char buf[50] = {0};
		strcpy(buf,it->c_str());

		char* p1 = strtok(buf,"~");
		char* p2 = strtok(NULL,"~");

		if(p1&&p2)
		{
			//有波浪线
			int firstDay = atoi(p1);
			int secondDay = atoi(p2);

			for(int i=firstDay;i<=secondDay;i++)
			{
				nd.day = i;
				setDay.insert(nd);
			}
		}
		else if(p1 && !p2)
		{
			//只有单天
			int day = atoi(p1);
			nd.day = day;
			setDay.insert(nd);
		}
		//异常
	}
}

void ParseHoliAndWorkDay(const char* buffer,int month,SETDAY& setHoliday,SETDAY& setWorkday)
{
	if(!buffer || month < 1 || month > 12)
		return ;

	char tempBuf[300] = {0};
	strcpy(tempBuf,buffer);

	char* Holiday = strtok(tempBuf,";");
	char* Workday = strtok(NULL,";");

	if(Holiday && strlen(Holiday))
	{
		ParseDay(Holiday,month,setHoliday);
	}

	if(Workday && strlen(Workday))
	{
		ParseDay(Workday,month,setWorkday);
	}
}

void OnFinish(TaskDownload* downloadInfo,bool success,void* userData)
{
	printf("\n HttpRequest finished: %s\n",success?"SUCCESS":"FAILED");

	return ;
	if(success)
	{
		char holidayBuffer[300] = {0};//写入文件
		char* p1 = strstr((char*)downloadInfo->info.saveBuf,"holidayversion");
		char* p2 = strstr((char*)downloadInfo->info.saveBuf,"holidayversionend");

		if(p1&&p2&&p1<p2)
		{
			strncpy(holidayBuffer,p1,p2-p1);
		}

		char* pVersion = strtok(holidayBuffer,":");
		char* pHolidays = strtok(NULL,":");

		int version = 0;
		sscanf(pVersion,"holidayversion=%d",&version);

		LISTSTR monthList;
		char* pMonth = strtok(pHolidays,"|");
		do 
		{
			if(!pMonth)
				break;

			monthList.push_back(pMonth);

			pMonth = strtok(NULL,"|");
		} while (true);

		SETDAY setholiday;
		SETDAY setworkday;
		LISTSTR::iterator it = monthList.begin();
		for(;it!=monthList.end();it++)
		{
			char tmpBuffer[100] = {0};
			strcpy(tmpBuffer,it->c_str());
			char* pMonth = strtok(tmpBuffer,"#");
			char* pDay = strtok(NULL,"#");

			if(!pMonth || !pDay)
				continue;

			int month = atoi(pMonth);
			ParseHoliAndWorkDay(pDay,month,setholiday,setworkday);
		}
	}

	return ;

	/*
	if(strcmp(downloadInfo->url , "http://xg.91555.com/res/giftxml.html") == 0)
	{
		if(downloadInfo->info.saveBuf)
		{
			int version;
			std::string url;
			std::string md5;

			strcat(downloadInfo->info.saveBuf,"\r\n");
			TiXmlDocument doc;
			if ( doc.Parse( downloadInfo->info.saveBuf ) )
			{
				const char * p = NULL;
				TiXmlElement * el = NULL;
				TiXmlElement * root_node = doc.FirstChildElement( "root" );
				if ( root_node != NULL )
				{
					el = root_node->FirstChildElement();
					if ( el )
					{
						if ( ( p = el->Attribute( "version" ) ) != NULL )
							version = atoi( p );
						if ( ( p = el->Attribute( "url" ) ) != NULL )
							url = p;
						if ( ( p = el->Attribute( "md5" ) ) != NULL )
							md5 = p;
					}
				}

				TaskDownload task = {0};
				strcpy(task.url , url.c_str());
				task.onProgress = &OnProgress;
				task.onFinish = &OnFinish;
				task.info.download = true;
				strcpy(task.info.fileName , "C:\\Users\\Administrator\\Desktop\\gift\\gift_items.zip");//IISLOG.rar
				task.info.mandatory = false;
				strcpy(task.info.unzipDir , "C:\\Users\\Administrator\\Desktop\\testunzip");
				bool b = g_gMgr.addTask(task,true);
			}
		}
	}*/
}

static char normal[] = "[a/1]sw ejfoi awj eof";
static char xml[] = "<?xml version=\"1.0\"?>" \
	"<msg version=\"2.0.2\" type=\"0\">" \
	"<font fontid=\"1\" colour=\"FFFFFFFF\"/>" \
	"<txt type=\"txt\">sdjfj sdjf sdfj</txt>" \
	"<img image=\"QQFace17\" type=\"img\" set=\"\"/>" \
	"<txt type=\"txt\">sdf sdf,sdf..</txt>" \
	"</msg>\n";


FILE* g_fp = NULL;
size_t  mycurl_write_callback(char *buffer,
							  size_t size,
							  size_t nitems,
							  void *userData)
{
	if(g_fp)
		return fwrite(buffer,size,nitems,g_fp);
	return 0;
}

size_t  mycurl_read_callback(char *buffer,
									 size_t size,
									 size_t nitems,
									 void *userData)
{
	printf("buffer = %s\n size=%d,nitems=%d\n",buffer,size,nitems);
	return size*nitems;
}

int _tmain(int argc, _TCHAR* argv[])
{
	const char* pJieri = "1-1~3;1-4|2-18~24;2-15,2-28|4-5;4-6|5-1|6-20;6-22|9-27|10-1~7;10-10";

	/*AES 加密*/
	BYTE aes_key[] = {0x01,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
	int key_size = sizeof(aes_key)*8;
	AESWORD key_schedule[60] = {0};
	aes_key_setup(aes_key,key_schedule,key_size);

	BYTE plain_text[32] = "abcdefghijhijklmabcdefghijhijkl";
	int timess = sizeof(plain_text)/16;
	BYTE cipher_text[16*2] = {0};
	BYTE end_text[16*2+1] = {0};

	printf("加密前:%s\n",plain_text);
	for(int i=0;i<timess;i++)
	{
		aes_encrypt(&plain_text[16*i],&cipher_text[16*i],key_schedule,key_size);
	}

	printf("加密后:");
	for(int i=0;i<timess;i++)
	{
		printf("%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X," \
			   "%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,",cipher_text[16*i],cipher_text[i*16+1]
		,cipher_text[16*i+2],cipher_text[i*16+3],cipher_text[16*i+4],cipher_text[i*16+5],cipher_text[16*i+6],cipher_text[i*16+7]
		,cipher_text[16*i+8],cipher_text[i*16+9],cipher_text[16*i+10],cipher_text[i*16+11],cipher_text[16*i+12],cipher_text[i*16+13]
		,cipher_text[16*i+14],cipher_text[i*16+15]);
	}
	printf("\n");

	for(int i=0;i<timess;i++)
	{
		aes_decrypt(&cipher_text[16*i],&end_text[16*i],key_schedule,key_size);
	}

	printf("解密后:%s\n",end_text);
	//getchar();

	/*CURL 获取web页面*/
	/*CURL*         pCurl; 
	pCurl = curl_easy_init();
	if(pCurl) {
		CURLcode res;
		curl_easy_setopt(pCurl, CURLOPT_URL, "http://blog.csdn.net/glp3329/article/details/49020683");

		g_fp = fopen("11.txt","w");
		curl_easy_setopt(pCurl,CURLOPT_WRITEFUNCTION,mycurl_write_callback);
		//curl_easy_setopt(pCurl,CURLOPT_READFUNCTION,mycurl_read_callback);

		res = curl_easy_perform(pCurl);
		curl_easy_cleanup(pCurl);

		if(g_fp)
			fclose(g_fp);
	}*/
	//getchar();
#ifdef WIN32
	AESWORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2,2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if(err != 0) 
		return -1;
#endif
	//CUnZip::unZipFile("C:\\Users\\Administrator\\Desktop\\gift\\gift_items.zip","C:\\Users\\Administrator\\Desktop",NULL);

	bool b = g_gMgr.initDownload();
	/*
	http://xg.91555.com/res/giftxml.html
	http://res.xg.91555.com/res/file/giftPic/gift_100001.png
	http://res.xg.91555.com/res/file/gift/167/gift_items.zip
	http://baike.baidu.com/view/2117188.htm?fr=aladdin
	*/
	/*
	TaskDownload task = {0};
	strcpy(task.url , "http://baike.baidu.com/view/2117188.htm?fr=aladdin");//"";//gift_items.zip
	task.onProgress = &OnProgress;
	task.onFinish = &OnFinish;
	task.info.download = true;
	strcpy(task.info.fileName , "C:\\Users\\Administrator\\Desktop\\gift\\gift_items.html");//IISLOG.rar
	task.info.mandatory = false;
	strcpy(task.info.unzipDir , "C:\\Users\\Administrator\\Desktop\\testunzip");
	//strcpy(task.info.MD5,"1A11DDA90D08CCF9E885C41FB8700AB0");
	int nRet = g_gMgr.addTask(task);
// 	task.url = "http://res.xg.91555.com/res/file/role/rightEx.zip";
// 	task.info.fileName = "C:\\Users\\Administrator\\Desktop\\rightEx.zip";
// 	b = g_gMgr.addTask(task);
	if(nRet == -1)
		printf("下载失败。");*/

	/*Content-Encoding: deflate    编码*/
	/*gzip例子 http://blog.csdn.net/glp3329/article/details/49020683 CSDN中国假日 */
	/*deflate例子 http://baike.baidu.com/view/2117188.htm?fr=aladdin*/
	TaskDownload task = {0};
	strcpy(task.url , "http://blog.csdn.net/glp3329/article/details/49020683");
	task.onProgress = &OnProgress;
	task.onFinish = &OnFinish;
	task.info.download = true;
	strcpy(task.info.fileName , "11.txt");
// 	int bufferLen = 1024*1024;
// 	task.info.saveBuf = (unsigned char*) malloc(bufferLen);
// 	task.info.saveBufLen = bufferLen;
	int ret = g_gMgr.addTask(task);
	getchar();
#ifdef WIN32
	WSACleanup();
#endif
	return 0;
}



