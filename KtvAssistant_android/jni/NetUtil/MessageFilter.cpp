#include "MessageFilter.h"
#include "../FilterDog.h"

#define NORMALSIZE 1024

inline int StrToIntFilt(char * str)
{
	int value = 0;
	int sign = 1;
	int radix;

	if(*str == '-')
	{
		sign = -1;
		str++;
	}
	if(*str == '0' && (*(str+1) == 'x' || *(str+1) == 'X'))
	{
		radix = 16;
		str += 2;
	}
	else if(*str == '0')      // 八进制首字符为0
	{
		radix = 8;
		str++;
	}
	else
		radix = 10;
	while(*str)
	{
		if(radix == 16)
		{
			if(*str >= '0' && *str <= '9')
				value = value * radix + *str - '0';
			else
				value = value * radix + (*str | 0x20) - 'a' + 10;
		}
		else
			value = value * radix + *str - '0';
		str++;
	}
	return sign*value;
}

std::ifstream& _open_file( std::ifstream& in, const std::string& file )
{ 
	in.close(); 
	in.clear(); 
	in.open( file.c_str() ); 
	return in; 
}
//--------------------------------------------------------------------------
// 构造函数
CMessageFilter::CMessageFilter()
	:m_bInit(false)
{
}
CMessageFilter::~CMessageFilter()
{

}


//--------------------------------------------------------------------------
// 获取实例
CMessageFilter* CMessageFilter::getSingle()
{
	static CMessageFilter s_instance;
	return &s_instance;
}

bool CMessageFilter::initFilterEx()
{
    static int len1 = strlen(DOG_FILTER1);
    static int len2 = strlen(DOG_FILTER2);
    return initFilterForAndroid(DOG_FILTER1,len1,DOG_FILTER2,len2);
}
bool CMessageFilter::initFilter(const char* pFilePath1,const char* pFilePath2)
{
	std::ifstream            record_file;     // 聊天敏感字文件指针
	std::ifstream            record_file2;     // 聊天敏感字文件指针
	bool                         bFile;           // 文件存在判断

	if(!g_NetApp.m_pCharsetCodecUTF8)
			return false;

	if(m_bInit)
		return true;

	// 打开输入和敏感字列表文件
	_open_file( record_file, pFilePath1 );
	if( !record_file.is_open() )
		return false;
	else
	{
		bFile = true;

		char tmpBuf[8] = {0};
		tmpBuf[0] = '0';
		tmpBuf[1] = 'x';
		char* szBuf;
		if ( (szBuf = ( char* )malloc( NORMALSIZE )) == NULL )
			return false;
		memset( szBuf, 0, NORMALSIZE );

		int cnt = 0;
		int bufSize = NORMALSIZE;

		// 将txt类型的敏感字提取出来
		while ( !record_file.eof() )
		{
			// 读取dog文件内容
			szBuf[cnt] = record_file.get();

			// 如果存储空间不足自动增加空间
			if ( cnt == bufSize-1 )
			{
				szBuf = ( char* )realloc( szBuf, NORMALSIZE + bufSize );
				memset( szBuf + bufSize, 0, NORMALSIZE );
				bufSize += NORMALSIZE;
			}

			cnt++;
		}  

		char* outBuf;
		int outBufSize = bufSize/2;
		if ( ( outBuf = ( char* )malloc( outBufSize ) ) == NULL )
			return false;
		memset( outBuf, 0, outBufSize );

		// 解密作业开始
		for ( int i = 0; i < outBufSize; i++ )
		{
			memcpy( tmpBuf + 2, szBuf + i*2, 2 );
			outBuf[ i ] =StrToIntFilt( tmpBuf );
		}

		char* pTok = strtok(outBuf,"\n");
		do 
		{
			if(pTok)
			{
				unsigned short* pWstr = g_NetApp.m_pCharsetCodecUTF8->convertToUnicode(pTok);
				m_mapSensWord[std::string(pTok)] = CCharsetCodec::caculateLength(pWstr);
				if(pWstr)
					delete pWstr;
			}
			else
				break;
		} while ((pTok=strtok(NULL,"\n")));
		free( szBuf );
		free( outBuf );

		record_file.close();
	}

	// 打开输入和敏感字列表文件
	_open_file( record_file2, pFilePath2);
	if( record_file2.is_open() )
	{
		// Add by Ln
		char tmpBuf[8] = {0};
		tmpBuf[0] = '0';
		tmpBuf[1] = 'x';
		char* szBuf;
		if ( (szBuf = ( char* )malloc( NORMALSIZE )) == NULL )
			return false;
		memset( szBuf, 0, NORMALSIZE );

		int cnt = 0;
		int bufSize = NORMALSIZE;

		// 将txt类型的敏感字提取出来
		while ( !record_file2.eof() )
		{
			// 读取dog文件内容
			szBuf[cnt] = record_file2.get();

			// 如果存储空间不足自动增加空间
			if ( cnt == bufSize-1 )
			{
				szBuf = ( char* )realloc( szBuf, NORMALSIZE + bufSize );
				memset( szBuf + bufSize, 0, NORMALSIZE );
				bufSize += NORMALSIZE;
			}

			cnt++;
		}  

		char* outBuf;
		int outBufSize = bufSize/2;
		if ( ( outBuf = ( char* )malloc( outBufSize ) ) == NULL )
			return false;
		memset( outBuf, 0, outBufSize );

		// 解密作业开始
		for ( int i = 0; i < outBufSize; i++ )
		{
			memcpy( tmpBuf + 2, szBuf + i*2, 2 );
			outBuf[ i ] =StrToIntFilt( tmpBuf );
		}

		char* pTok = strtok(outBuf,"\n");
		do 
		{
			if(pTok)
				m_setSensWord2.insert(std::string(pTok));
			else
				break;
		} while ((pTok=strtok(NULL,"\n")));

		free( szBuf );
		free( outBuf );

		record_file2.close();
	}

	m_bInit = true;
	return m_bInit;
}

bool CMessageFilter::initFilterForAndroid(const char* pData1,int nLen1,const char* pData2,int nLen2)
{
	if(!pData1 || nLen1==0)
		return false;

	if(!g_NetApp.m_pCharsetCodecUTF8)
		return false;

	if(m_bInit)
		return true;

	m_mapSensWord.clear();
	m_setSensWord2.clear();

	char tmpBuf[8] = {0};
	tmpBuf[0] = '0';
	tmpBuf[1] = 'x';

	{
		char* outBuf = NULL;
		int outBufSize = nLen1/2;
		if ( ( outBuf = ( char* )malloc( outBufSize ) ) == NULL )
			return false;
		memset( outBuf, 0, outBufSize );

		// 解密作业开始
		for ( int i = 0; i < outBufSize; i++ )
		{
			memcpy( tmpBuf + 2, pData1 + i*2, 2 );
			outBuf[ i ] =StrToIntFilt( tmpBuf );
		}

		char* pTok = strtok(outBuf,"\n");
		do 
		{
			if(pTok)
			{
				unsigned short* pWstr = g_NetApp.m_pCharsetCodecUTF8->convertToUnicode(pTok);
				m_mapSensWord[std::string(pTok)] = CCharsetCodec::caculateLength(pWstr);
				if(pWstr)
					delete pWstr;
			}
			else
				break;
		} while ((pTok=strtok(NULL,"\n")));
		free( outBuf );
	}

	if(pData2 && nLen2 > 0)
	{
		char* outBuf;
		int outBufSize = nLen2/2;
		if ( ( outBuf = ( char* )malloc( outBufSize ) ) == NULL )
			return false;
		memset( outBuf, 0, outBufSize );

		// 解密作业开始
		for ( int i = 0; i < outBufSize; i++ )
		{
			memcpy( tmpBuf + 2, pData2 + i*2, 2 );
			outBuf[ i ] =StrToIntFilt( tmpBuf );
		}

		char* pTok = strtok(outBuf,"\n");
		do 
		{
			if(pTok)
				m_setSensWord2.insert(std::string(pTok));
			else
				break;
		} while ((pTok=strtok(NULL,"\n")));
		free( outBuf );
	}

	m_bInit = true;
	return m_bInit;
}
//--------------------------------------------------------------------------
// 字符串过滤方法一
bool CMessageFilter::filter_first_copy( std::string str_src, std::string &str_des )
{
	if ( str_src.empty() )//认为不需要过滤
	{
		str_des = str_src;
		return true;
	}

	if(!m_bInit)//没有完成初始化工作
		return false;

    str_des = str_src;

    MAPWORDS::iterator it = m_mapSensWord.begin();
    for(;it!=m_mapSensWord.end();it++)
    {
    	std::string tmpStr = it->first;
    	std::string::size_type pos = str_des.find(tmpStr);

		if(pos == std::string::npos)
			continue;

		char cReplace[260] = {0};
		memset(cReplace,'*',it->second);

		do
		{
			if(pos == std::string::npos)
				break;

			str_des.replace(pos,tmpStr.size(),cReplace);
		} while ((pos = str_des.find(tmpStr)));
    }

	return true;
}

//--------------------------------------------------------------------------
// 字符串过滤方法二
bool CMessageFilter::filter_first( char * str_filter, int size )
{
    if ( !str_filter || !m_bInit )//指针异常或者没有完成初始化工作
        return false;

	if((0 == str_filter[0]) || size==0)//不需要过滤
		return true;

	//替换敏感字符
	std::string dest_str = str_filter;
	MAPWORDS::iterator it = m_mapSensWord.begin();
	for(;it!=m_mapSensWord.end();it++)
	{
		std::string tmpStr = it->first;
		std::string::size_type pos = dest_str.find(tmpStr);

		if(pos == std::string::npos)
			continue;

		char cReplace[260] = {0};
		memset(cReplace,'*',it->second);

		do
		{
			if(pos == std::string::npos)
				break;

			dest_str.replace(pos,tmpStr.size(),cReplace);
		} while ((pos = dest_str.find(tmpStr)));
	}
	
	int final_size = dest_str.size();
	if(final_size>size)
		final_size = size-1;
	strncpy(str_filter,dest_str.c_str(),final_size);
	str_filter[final_size] = 0;
	return true;
}

//--------------------------------------------------------------------------
// 查找是否包含敏感字符
bool CMessageFilter::find_sens_word( const std::string str_src )
{
	if ( str_src.empty() || !m_bInit )
		return false;

	MAPWORDS::iterator it = m_mapSensWord.begin();
	for(;it!=m_mapSensWord.end();it++)
	{
		std::string tmpStr = it->first;
		if(str_src.find(tmpStr) != std::string::npos)
			return true;
	}

	return false;
}

//--------------------------------------------------------------------------
// 查找是否包含敏感字符
bool CMessageFilter::find_sens_word2( const std::string str_src )
{
	if ( str_src.empty() || !m_bInit ) return false;

	SETWORDS::iterator it = m_setSensWord2.begin();
	for(;it!=m_setSensWord2.end();it++)
	{
		std::string tmpStr = *it;
		if(str_src.find(tmpStr) != std::string::npos)
			return true;
	}

	return false;
}
