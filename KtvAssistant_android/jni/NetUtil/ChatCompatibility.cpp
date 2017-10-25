#include "ChatCompatibility.h"
#include <string.h>
#include "xml/tinyxml.h"
#include "NetApp.h"

#define CHAT_VERSION "2.0.2"

int CChatCompatibility::ConvertXml2Normal(char* out,int& len,const char* in)
{
	if(!in || !out || len==0)
		return -4;

	int curLen = 0;
	out[0] = 0;

	TiXmlDocument document;
	document.Parse(in,NULL,TIXML_ENCODING_LEGACY);
	if(document.Error())//xml解析错误
		return -1;

	bool isRightChatVersion = true;
	TiXmlElement* pMsgElement = document.FirstChildElement("msg");
	if(!pMsgElement)
		isRightChatVersion = false;
	else
	{
		const char* pVersion = pMsgElement->Attribute("version");
		if(pVersion == NULL || strcmp(pVersion,CHAT_VERSION) != 0)
			isRightChatVersion = false;
	}

	if(isRightChatVersion)
	{
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
					if(strstr(pImg,"QQFace"))
					{
						char buf[50] = {0};
						sprintf(buf,"[a/%s]",pImg+6);//6=strlen("QQFace");
						int nImgLen = strlen(buf);
						if(curLen+nImgLen>=len)
							return -3;
						strcat(&out[curLen],buf);
						curLen += nImgLen;
					}
					else
					{
						if(strlen(out)+7>=len)// 7 = [image]
							return -3;

						strcat(&out[curLen],"[image]");
						curLen += 7;
					}
				}
			}
		}while((pElement = pElement->NextSiblingElement()));
	}
	else
	{
		strcpy(out,"聊天协议版本需要(V2.0.2),请更新程序~~");
	}
	

	int ret = 0;

	unsigned short* unicode = g_NetApp.m_pCharsetCodecGBK->convertToUnicode(out);
	char* utf8 = g_NetApp.m_pCharsetCodecUTF8->convertFromUnicode(unicode);

	if(utf8)
	{
		strcpy(out,utf8);
		delete utf8;
	}
	else
	{
		out[0] = 0;
		ret = -2;
	}
	if(unicode)
		delete unicode;

	return ret;
}

