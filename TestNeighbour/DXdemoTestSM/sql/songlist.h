#ifndef MAIN_H
#define MAIN_H

#include <list>
#include "MusicSQLObject.h"

#define PAGE_SIZE	20

typedef struct _tMusicInfo
{
	int	nMusicId;
	std::string	strMusicName;
	bool isScored;
} _tMusicInfo;

// �ɹ������� ���㣻 ʧ�ܣ������� 
int GetKTVMusicList( char * path, std::list<_tMusicInfo> & );

#endif