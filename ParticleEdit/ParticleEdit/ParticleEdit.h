// ParticleEdit.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "particleed.h"

// CParticleEditApp:
// �йش����ʵ�֣������ ParticleEdit.cpp
//

class CParticleEditApp : public CWinApp
{
public:
	CParticleEditApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CParticleEditApp theApp;