#ifndef MESSAGEFILTER__H__
#define MESSAGEFILTER__H__

/*
	封装一个比较UTF8字符串中是否含有敏感字符串的类。
*/

#include <string>
#include <fstream>
#include <vector>
#include "charset/CharsetCodec.h"
#include "NetApp.h"

typedef std::map<std::string,int> MAPWORDS;
typedef std::set<std::string> SETWORDS;
//--------------------------------------------------------------------------
class CMessageFilter
{
public:
    static CMessageFilter* getSingle();

	/*
	@param pFilePath1 : "forbiddenword.dog"含路径
	@param pFilePath1 : "forbiddenword2.dog"含路径

	务必调用此方法，否则所有的函数都将返回false
	*/
    bool initFilterEx();
	bool initFilter(const char* pFilePath1,const char* pFilePath2);
	bool initFilterForAndroid(const char* pData1,int nLen1,const char* pData2,int nLen2);
	/*
	过滤敏感字符串
	@param str_src[in]：需要对比的UTF8字符串
	@param str_des[out] : 过滤完之后的UTF8字符串

	@return : 
		false，没有初始化
		true，成功过滤完毕

	@注：str_src中的字符串必须是UTF8字符串
	*/
    bool filter_first_copy( std::string str_src, std::string &str_des );
	/*
	过滤敏感字符串
	@param str_src[in out]：需要对比的UTF8字符串
	@param size[in] : 字符串的长度

	@return : 
		false，没有初始化或者str_filter为NULL
		true，成功过滤完毕
	
	@注：str_filter中的字符串必须是UTF8字符串
	*/
    bool filter_first( char * str_filter, int size );
	/*
	查找是否含有敏感字符串

	@param str_src[in]：需要对比的UTF8字符串

	@注：str_src中的字符串必须是UTF8字符串

	@return ：true，有；false，没有
	*/
	bool find_sens_word( const std::string str_src );
	/*
	查找是否含有特殊符号

	@param str_src[in]：需要对比的字符串

	@return ：true，有；false，没有
	*/
	bool find_sens_word2( const std::string str_src );
private:
	CMessageFilter();
	~CMessageFilter();
	bool											m_bInit;
    MAPWORDS 		 						m_mapSensWord;
    SETWORDS				 					m_setSensWord2;
};

#endif//MESSAGEFILTER__H__
