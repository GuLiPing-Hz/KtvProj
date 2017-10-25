#include "ServerData.h"
#include "XmlFileDef.h"

#define  DEF_STRROOT "ServerData"
#define  strVersionTable  "GroupVersion"
#define  strGroupTable  "RoomGroup"
#define  strRoomTable  "RoomData"

CServerData::CServerData(const char* path)
	:CLocalStorage(path,XG_SERVER_XML,DEF_STRROOT)
{
	buildStorage();
}

CServerData::~CServerData()
{

}

void CServerData::setVersion( int lobby_idx, int version )
{
	if( 0 == version )
		return;

	TiXmlElement * p = findElement( m_pVersionElement, "lobby_id", lobby_idx );
	if( NULL == p )
	{
		p = new TiXmlElement( "item" );
		if(!p)
			return ;
		m_pVersionElement->LinkEndChild( p );
	}
	p->SetAttribute("lobby_id",lobby_idx);
	p->SetAttribute("group_version",version);

	saveFile();
}

int CServerData::getVersion( int lobby_idx )
{
	TiXmlElement * p = findElement( m_pVersionElement, "lobby_id", lobby_idx );
	if( p )
	{
		const char * value = NULL;
		value = p->Attribute( "group_version" );
		return atoi( value );
	}
	return 0;
}

void CServerData::addGroupData( int group_idx, const char * group_name, char disable, int lobby_idx )
{
	TiXmlElement * p = findElement( m_pGroupElement, "group_idx", group_idx );
	if( NULL == p )
	{
		p = new TiXmlElement( "item" );
		if(!p)
			return ;
		m_pGroupElement->LinkEndChild( p );
	}

	p->SetAttribute("group_id",group_idx);
	p->SetAttribute("group_name",group_name);
	p->SetAttribute("disable",disable);
	p->SetAttribute("lobby_id",lobby_idx);
}

void CServerData::getGroupData( int& count, ResultData_GroupInfo * info, int lobby_idx )
{
	count = 0;

	const char * v = NULL;

	TiXmlElement * p = m_pGroupElement?m_pGroupElement->FirstChildElement():NULL;
	while( NULL != p )
	{
		v = p->Attribute( "lobby_id" );
		if( lobby_idx == atoi( v ) )
		{
			v = p->Attribute( "group_id" );
			info->group_id = atoi( v );
			v = p->Attribute( "group_name" );
			strcpy( info->group_name, v );
			v = p->Attribute( "disable" );
			info->disable = atoi( v );

			count++;
			info++;
		}

		p = p->NextSiblingElement();
	}
}

void CServerData::addRoomData( int room_idx, int group_idx, int lobby_idx )
{
	TiXmlElement * p = findElement( m_pRoomElement, "room_id", room_idx );
	if( NULL == p )
	{
		p = new TiXmlElement( "item" );
		if(!p)
			return ;
		m_pRoomElement->LinkEndChild( p );
	}
	p->SetAttribute("room_id",room_idx);
	p->SetAttribute("group_id",group_idx);
	p->SetAttribute("lobby_id",lobby_idx);
}

int CServerData::getGroupRoomList( VECTROOMLIST & room_list, int group_idx, int lobby_idx )
{
	room_list.clear();
	const char * v = NULL;
	TiXmlElement * p = m_pRoomElement?m_pRoomElement->FirstChildElement():NULL;

	int count = 0;
	while( NULL != p )
	{
		v = p->Attribute( "group_id" );
		if( group_idx == atoi( v ) )
		{
			v = p->Attribute( "lobby_id" );
			if( lobby_idx == atoi( v ) )
			{
				v = p->Attribute( "room_id" );
				room_list.push_back( atoi( v ) );
				count++;
			}
		}

		p = p->NextSiblingElement();
	}
	return count;
}

void CServerData::buildStorage()
{
	m_pVersionElement = findFirstElement( strVersionTable );
	if( NULL == m_pVersionElement )
	{
		m_pVersionElement = new TiXmlElement( strVersionTable );
		if(!m_pVersionElement)
			return ;
		linkEndChild( m_pVersionElement );
		saveFile();
	}

	m_pGroupElement = findFirstElement( strGroupTable );
	if( NULL == m_pGroupElement )
	{
		m_pGroupElement = new TiXmlElement( strGroupTable );
		if(!m_pGroupElement)
			return ;
		linkEndChild( m_pGroupElement );
		saveFile();
	}

	m_pRoomElement = findFirstElement( strRoomTable );
	if( NULL == m_pRoomElement )
	{
		m_pRoomElement = new TiXmlElement( strRoomTable ); 
		if(!m_pRoomElement)
			return ;
		linkEndChild( m_pRoomElement );
		saveFile();
	}
}
