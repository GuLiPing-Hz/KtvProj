/*
	注释添加以及修改于 2014-5-7
	author: glp

	
*/

#ifndef NOTIFYMSG__H__
#define NOTIFYMSG__H__

#include "LocalStorage.h"

class CNotifyMsg : public CLocalStorage
{
	CNotifyMsg(CNotifyMsg&);
	CNotifyMsg& operator=(const CNotifyMsg&);
public:
	CNotifyMsg(const char* path);
	virtual ~CNotifyMsg(void);

	virtual void buildStorage();

	int getCurMsgId();
	void setCurMsgId(int id);
private:
	TiXmlElement	*	m_pIdElement;
};

#endif//NOTIFYMSG__H__
