
#include "NotifyMsg.h"
#include "XmlFileDef.h"

#define DEF_ROOT_NOTIFY "notify"
#define DEF_ELEMENT_MSGID "msgid"

#define DEF_ATTR_ID "ATTR_ID"

CNotifyMsg::CNotifyMsg(const char* path)
:CLocalStorage(path,XG_NOTIFY_XML,DEF_ROOT_NOTIFY)
{
	buildStorage();
}
CNotifyMsg::~CNotifyMsg(void)
{

}

//实现需要自己的存储的xmlElement
void CNotifyMsg::buildStorage()
{
	m_pIdElement = findFirstElement(DEF_ELEMENT_MSGID);
	if( NULL == m_pIdElement )
	{
		m_pIdElement = new TiXmlElement( DEF_ELEMENT_MSGID );
		if(!m_pIdElement)
			return ;
		linkEndChild( m_pIdElement );
		saveFile();
	}
}

int CNotifyMsg::getCurMsgId()
{
	int ret = 0;
	if(!m_pIdElement)
		return ret;

	const char* pStr = m_pIdElement->Attribute(DEF_ATTR_ID);
	if(!pStr)
		return ret;

	ret = atoi(pStr);
	return ret;
}
void CNotifyMsg::setCurMsgId(int id)
{
	if(!m_pIdElement)
		return;
	m_pIdElement->SetAttribute(DEF_ATTR_ID,id);
	saveFile();
}
