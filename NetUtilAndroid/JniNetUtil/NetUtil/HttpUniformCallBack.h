/*
 * HttpUniformCallBack.h
 *
 *  Created on: 2014年10月9日
 *      Author: glp
 */

#ifndef HTTPUNIFORMCALLBACK_H_
#define HTTPUNIFORMCALLBACK_H_

#include "wrap/EventHandler.h"
#include <list>

namespace NetworkUtil{
	class Reactor;
};

typedef enum _eHttpCmd
{
	FINISH_IMAGE,
	FINISH_DATABASE
};

typedef struct _HttpCallBackMsg
{
	int msg;
	void* data;
}HttpCallBackMsg;
typedef std::list<HttpCallBackMsg> LISTHTTPCBM;
class CriticalSectionWrapper;
class HttpUniformCallBack: public NetworkUtil::IdleEventHandler {
public:
	HttpUniformCallBack(NetworkUtil::Reactor *pReactor);
	virtual ~HttpUniformCallBack();

	virtual void onRun();

	void postMessage(int msg,void* data);
private:
	CriticalSectionWrapper* m_pCSMSG;
	LISTHTTPCBM m_listHttpCBM;
};

#endif /* HTTPUNIFORMCALLBACK_H_ */
