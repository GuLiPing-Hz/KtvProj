#pragma once
#include "ThreadMgr.h"
#include "ShareMemory.h"

enum MSG_SHAEREMEMORY
{
	//�򿪹����ڴ� wparam (char*) lparam(NULL)
	MSG_CREATESHAREMEMORY = MSG_INVALID+1,
	//�ر� wparam(NULL) lparam(NULL)
	MSG_DXPLAYER_CLOSE,
	//�и� wparam(char*) lparam(NULL)
	MSG_SWITCH_SONG,
	//����ͼƬ ����������������Ҫ��CMD_DXPLAYER_SONGINFO֮ǰ���ͣ�
	//wparam����ͼƬ(����·��,��׺)
	MSG_SINGER_PIC,
	//����/��ͣ  wparam(NULL) lparam(NULL)
	MSG_PLAY_CONTROL_RESUME,
	MSG_PLAY_CONTROL_PAUSE,
	//���ֿ��� wparam(NULL) lparam(NULL)
	MSG_PLAY_GRADE,
	//���࿪�� wparam(NULL) lparam(NULL)
	MSG_PLAY_ACCOMPANY,
	//������Ϣ
	//������Ϣ ������wparam(char*) ������lparam(char*)
	MSG_SONG_INFO,
	MSG_SONG_STARTEND,
	//�������� ���� wparam(int)
	MSG_SET_VOLUME,
	//��ȡ����
	MSG_GET_VOLUME,
	MSG_SET_ADD_TONE,
	MSG_SET_SUB_TONE,
	MSG_GET_TONE,
	MSG_MUTE_OPEN,
	MSG_MUTE_CLOSE,
	MSG_MESSAGE,
	MSG_FIREWARNING,
	MSG_NOFIREWARNING,
	MSG_NEXTSONG,
	MSG_SOUND
};

class CShareMemoryThreadMgr : public CThreadMgr
{
public:
	CShareMemoryThreadMgr(void);
	~CShareMemoryThreadMgr(void);

	static CShareMemoryThreadMgr & getSingleton();

public:

public:
	// ��Ϣ����
	virtual void ProcessMessage( int id, void * wParam, void * lParam );

	// ���д���
	virtual void ProcessIdle();

protected:
	// ��ȡShareMemory
	void ReadShareMemory();

private:
	CShareMemory	m_C2PShareMemory;
	CShareMemory	m_P2CShareMemory;
	bool			m_bC2PShareMemory;
	bool			m_bP2CShareMemory;
	HANDLE			m_hC2PEvent;
	HANDLE			m_hP2CEvent;

	PROCESS_INFORMATION m_procinfo; //���̵���Ϣ
};
