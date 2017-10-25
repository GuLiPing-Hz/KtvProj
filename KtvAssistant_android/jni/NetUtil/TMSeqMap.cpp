#include "tmseqmap.h"
//#include "NetApp.h"

void TMSeqMap::onTimeOut()
{
	std::map<int,ReserveDataV*> RDMap = *GetMap();
	std::map<int,ReserveDataV*>::iterator it;

	ReserveDataV *pRD;

	for(it = RDMap.begin();it != RDMap.end(); it++)
	{
		pRD = it->second;
		if( (time(NULL) - pRD->t) > m_packtimeout )//超时，没响应
		{
			pRD->OnTimeOut();
			Del(it->first);
		}
	}
}