#include "LocalStorage.h"
#include <assert.h>
#include <stdio.h>

CLocalStorage::CLocalStorage(const char* path,const char* file,const char* root)
:m_pRoot(NULL)
{
	if(path)
		sprintf(m_filePath,"%s/%s",path,file);
	else
		strcpy(m_filePath,file);
    if( !m_xmlDoc.LoadFile(m_filePath) )
	{
        if(root && root[0] != '\0')
        {
            m_pRoot = new TiXmlElement( root );
            m_xmlDoc.LinkEndChild( m_pRoot );//交由xml来释放
            m_xmlDoc.SaveFile( m_filePath );
        }
	}
	else
		m_pRoot = m_xmlDoc.FirstChildElement();
}


CLocalStorage::~CLocalStorage(void)
{
}

TiXmlElement * CLocalStorage::findFirstElement( const char * element_name )
{
	if(!checkLoad())
        return NULL;
	return element_name?m_pRoot->FirstChildElement(element_name):NULL;
}

TiXmlElement * CLocalStorage::findElement( TiXmlElement * element, const char * name, const char * value )
{
	const char * v = NULL;

	TiXmlElement * p = element?element->FirstChildElement():NULL;
	while( NULL != p )
	{
		v = p->Attribute( name );
		if( NULL != v && 0 == strcmp( v, value ) )
		{
			return p;
		}
		p = p->NextSiblingElement();
	}

	return NULL;
}

TiXmlElement * CLocalStorage::findElement( TiXmlElement * element, const char * name, int value )
{
	char buf[16] = {0};
	sprintf( buf, "%d", value );
	return findElement( element, name, buf );
}

bool CLocalStorage::linkEndChild( TiXmlElement * element, TiXmlElement * node/* = NULL*/ )
{
	if( ! node )
		node = m_pRoot;
	if(element)
		if( ! node->LinkEndChild( element ) )
			return false;

	return true;
}

void CLocalStorage::clearElement( TiXmlElement * element )
{
	TiXmlElement * pNext = NULL;
	TiXmlElement * p = element?element->FirstChildElement():NULL;
	while( NULL != p )
	{
		pNext = p->NextSiblingElement();
		element->RemoveChild( p );
		p = pNext;
	}
}

