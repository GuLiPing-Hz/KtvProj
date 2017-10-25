#include "dbresult.h"

bool ResultSet::ParseResult(const char* result,unsigned int resultlen)
{
	BinaryReadStream infostream(result,resultlen);
	if(!infostream.Read(rownum))
	{
		return false;
	}
	if(!infostream.Read(colnum))
	{
		return false;
	}

	static char buf[65535];
	char *ptr = buf;
	unsigned int curlen;
	for(int i = 0;i < rownum;i++)
	{
		for(int j = 0;j < colnum;j++)
		{
			if((unsigned int)(ptr - buf) > sizeof(buf))
			{
				return false;
			}
			if(!infostream.Read(ptr,sizeof(buf),curlen))
			{
				return false;
			}
			ptr[curlen] = 0;
			m_resultmap[std::make_pair(i,j)] = std::make_pair(ptr,curlen+1);
			ptr += (curlen+1);//读取下一个
		}
	}
	return true;
}

bool ResultSet::GetResult(int row,int col,char** buf,unsigned int &buflen)
{
	std::map< std::pair<int,int>, std::pair<char*,unsigned int> >::iterator it = m_resultmap.find(std::make_pair(row,col));
	if(it == m_resultmap.end())
		return false;
	*buf = it->second.first;
	buflen = it->second.second;
	return true;
}

bool ResultSet::Read(int row,int col,char* buf,unsigned int buflen)
{
	if(!buf)
		return false;

	char *pbuf;
	unsigned int pbuflen;

	std::map< std::pair<int,int>, std::pair<char*,unsigned int> >::iterator it = m_resultmap.find(std::make_pair(row,col));
	if(it == m_resultmap.end())
		return false;
	pbuf = it->second.first;
	pbuflen = it->second.second;

	if(pbuflen > buflen)	return false;
	memcpy(buf,pbuf,pbuflen);

	return true;
}




