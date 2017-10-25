#ifndef DBRESULT_H_
#define DBRESULT_H_
/*
	注释添加以及修改于 2014-4-4 
	mender: glp

	解析数据类，提供解析CMD_GETCLUBINFO_C2L2C 协议
*/
#include <map>
#include "wrap/RWStream.h"

using namespace NetworkUtil;

class ResultSet
{
	public:
		bool GetResult(int row,int col,char** buf,unsigned int &buflen);

		bool ParseResult(const char* result,unsigned int resultlen);

		bool Read(int row,int col,char* buf,unsigned int buflen);
		bool Read(int row,int col,int &i){
			char str[10];
			if(!Read(row,col,str,sizeof(str)))
				return false;
			i = atoi(str);
			return true;
		}	
		bool Read(int row,int col,short &i){
			char str[10];
			if(!Read(row,col,str,sizeof(str)))
				return false;
			i = (short)atoi(str);
			return true;
		}
		bool Read(int row,int col,long &i){
			char str[32];
			if(!Read(row,col,str,sizeof(str)))
				return false;
			i = atol(str);
			return true;
		}

public:
	short rownum;
	short colnum;
private:
	std::map< std::pair<int,int>, std::pair<char*,unsigned int> > m_resultmap;
};

#endif
