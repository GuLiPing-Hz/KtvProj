/*
	注释添加以及修改于 2014-5-7
	author: glp

	
*/

#ifndef LOCALSTORAGE__H__
#define LOCALSTORAGE__H__

#include "../xml/tinyxml.h"

class CLocalStorage
{
    CLocalStorage(CLocalStorage&);
    CLocalStorage& operator=(const CLocalStorage&);
public:
	CLocalStorage(const char* path,const char* file,const char* root=NULL);
	virtual ~CLocalStorage(void);

	TiXmlElement *	findFirstElement( const char * element_name );
	//从element底下寻找名字为name 值为value的TiXmlElement
	TiXmlElement *  findElement( TiXmlElement * element, const char * name, const char * value );
	//从element底下寻找名字为name 值为value的TiXmlElement
	TiXmlElement *  findElement( TiXmlElement * element, const char * name, int value );
	//把element加入到node中
	bool			linkEndChild( TiXmlElement * element, TiXmlElement * node = NULL);
	//清空element
	void			clearElement( TiXmlElement * element );
	//保存文件
	void			saveFile(){m_xmlDoc.SaveFile();}
    //检查是否加载成功
    bool            checkLoad(){return !!m_pRoot;}
	//实现需要自己的存储的xmlElement
	virtual void buildStorage(){};

	TiXmlDocument           m_xmlDoc;
	TiXmlElement*           m_pRoot;
	char					m_filePath[260];
};

#endif//LOCALSTORAGE__H__
