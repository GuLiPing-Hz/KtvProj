#include "datablock.h"
#include <string.h>

namespace NetworkUtil
{
	int DataBlock::copy(unsigned int pos,const char *buf,unsigned int buflen)
	{
		if(!buf || !buflen)
			return 0;

		unsigned int tmppos = pos +  buflen;
		//未超出容量
		if(  tmppos <= m_size )
		{
			memcpy(m_buf+pos,buf,buflen);
			m_pos = tmppos;
			return 0;
		}
		//超出容量，但是小于最大长度
		if( tmppos > m_size && tmppos <= m_maxsize)
		{
			char *tmpbuf = new char[tmppos];
			memcpy(tmpbuf,m_buf,m_pos);
			delete []m_buf;
			memcpy(tmpbuf + pos,buf,buflen);

			m_buf = tmpbuf;
			m_pos = tmppos;
			m_size = tmppos;
			return 0;
		}
		//超出最大长度
		return -1;
	}
}
