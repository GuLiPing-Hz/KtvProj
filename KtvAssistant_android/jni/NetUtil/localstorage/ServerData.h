/*
	注释添加以及修改于 2014-5-7
	author: glp
*/
#ifndef SERVERDATA__H__
#define SERVERDATA__H__
#include "LocalStorage.h"
#include "../ResultData.h"
#include <vector>

typedef std::vector< int > VECTROOMLIST;
class CServerData : public CLocalStorage
{
public:
	CServerData(const char* path);
	virtual ~CServerData();
	//CLocalStorage
	virtual void buildStorage();

	void		setVersion( /*in*/int lobby_idx,/*in*/ int version );
	int         getVersion(/*in*/ int lobby_idx );
	
	void		addGroupData(/*in*/ int group_idx,/*in*/const char * group_name,/*in*/ char disable,/*in*/ int lobby_idx );
	void		addRoomData(/*in*/ int room_idx,/*in*/ int group_idx,/*in*/ int lobby_idx );
	//must be called after you have called the add##XXX function
	void		addEnd(){saveFile();}
	//获取指定lobby id 的分组信息
	void		getGroupData( /*out*/int& count, /*out*/ResultData_GroupInfo * info,/*in*/  int lobby_idx );
	//取得指定group，lobby的roomlist
	int         getGroupRoomList( /*out*/VECTROOMLIST & room_list,/*in*/ int group_idx,/*in*/  int lobby_idx );

	void		clearVersion(){clearElement(m_pVersionElement);}
	void		clearGroup(){clearElement(m_pGroupElement);}
	void		clearRoom(){clearElement(m_pRoomElement);}
private:
	TiXmlElement	*	m_pVersionElement;
	TiXmlElement	*	m_pGroupElement;
	TiXmlElement	*	m_pRoomElement;
};

#endif//SERVERDATA__H__
