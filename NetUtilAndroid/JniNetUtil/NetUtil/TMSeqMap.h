#ifndef TMSEQMAP__H__
#define TMSEQMAP__H__

/*
	注释添加以及修改于 2014-4-3 
	mender: glp

	封装一个能对超时进行处理并保存数据的map
*/

#include "wrap/EventHandler.h"
#include "wrap/SeqMap.h"
#include "ReserveData.h"


class TMSeqMap : public NetworkUtil::TMEventHandler , public NetworkUtil::SeqMap_ThreadSafe<ReserveDataV*>
{
public:
	TMSeqMap(NetworkUtil::Reactor *pReactor,int packtimeout = 60 )
		: NetworkUtil::TMEventHandler(pReactor)
		, m_packtimeout(packtimeout)
	{}

	void onTimeOut();
private:
	int m_packtimeout;//包超时的数值,一般是1秒
};

#endif//TMSEQMAP__H__
