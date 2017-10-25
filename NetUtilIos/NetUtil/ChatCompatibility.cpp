#include "ChatCompatibility.h"
#include <string.h>
#include "xml/tinyxml.h"

#define CHAT_VERSION "2.0.2"

int CChatCompatibility::convertXml2Normal(char* out,int& len,const char* in)
{
	if(!in || !out)
		return -4;

	int curLen = 0;
	out[0] = 0;

	TiXmlDocument document;
	if(!document.Parse(in))
		return -1;

	TiXmlElement* pMsgElement = document.FirstChildElement("msg");
	if(!pMsgElement)
		return -2;
	else
	{
		const char* pVersion = pMsgElement->Attribute("version");
		if(pVersion == NULL || strcmp(pVersion,CHAT_VERSION) != 0)
			return -2;
	}

	TiXmlElement* pElement = pMsgElement->FirstChildElement();
	do
	{
		if(pElement == NULL)
			break;
		//不对字体进行解析
		if(strcmp(pElement->Value(),"font") == 0)
			continue;
		if (strcmp(pElement->Value(),"txt") == 0)
		{
			const char* pTxt = pElement->GetText();
			if(pTxt)
			{
				int nTxtLen = strlen(pTxt);
				if(curLen + nTxtLen > len)
					return -3;
				strcat(&out[curLen],pTxt);
				curLen += nTxtLen;
			}
		}
		else if(strcmp(pElement->Value(),"img") == 0)
		{
			const char* pImg = pElement->Attribute("image");
			if(pImg)
			{
				char buf[50] = {0};
				sprintf(buf,"[a/%s]",pImg+6);
				int nImgLen = strlen(buf);
				if(curLen + nImgLen > len)
					return -3;
				strcat(&out[curLen],buf);
				curLen += nImgLen;
			}
		}
	}while((pElement = pElement->NextSiblingElement()));

	return 0;
}

