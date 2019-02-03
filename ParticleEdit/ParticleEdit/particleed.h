/*
** Haaf's Game Engine 1.8
** Copyright (C) 2007, Relish Games
** hge.relishgames.com
**
** Particle systems editor
*/


#ifndef PARTICLEED_H
#define PARTICLEED_H


#include "hge.h"
#include "hgehelp/hgeanim.h"
#include "hgehelp/hgefont.h"
#include "hgehelp/hgeparticlesys.h"
#include "hgehelp/hgegui.h"
#include "hgehelp/hgeguictrls.h"


#define PARTICLE_SIZE			32

#define ButtonGetState(id)		((hgeGUIButton*)gui->GetCtrl(id))->GetState()
#define ButtonSetState(id,b)	((hgeGUIButton*)gui->GetCtrl(id))->SetState(b)
#define SliderGetValue(id)		((hgeGUISlider*)gui->GetCtrl(id))->GetValue()
#define SliderSetValue(id,f)	((hgeGUISlider*)gui->GetCtrl(id))->SetValue(f)
#define GetTextCtrl(id)			((hgeGUIText*)gui->GetCtrl(id))

extern int PSI_START;
extern HGE *hge;

struct PEditorState
{
	bool				bHelp;
	bool				bBBox;
	bool				bIFace;
	int					nPreset;
	float				mx, my;
	HTEXTURE			texBG;
	CGLSprite			*sprBG;
	hgeParticleSystem*	ps[9];
	//std::vector<hgeParticleSystem*> vect_ps;
};

extern hgeFont		*fnt;
extern hgeGUI		*gui;
extern HTEXTURE		texGui, texParticle;

extern CGLSprite	*sprLeftPane1, *sprLeftPane2, /**sprLeftPane3,*/ *sprRightPane1, *sprRightPane2;
extern CGLSprite	*sprCursor, *sprColor, *sprBBox,*sprTex;
extern hgeAnimation	*sprParticles[9];
class CCritSec {

	// make copy constructor and assignment operator inaccessible

	CCritSec(const CCritSec &refCritSec);
	CCritSec &operator=(const CCritSec &refCritSec);

	CRITICAL_SECTION m_CritSec;

public:
	DWORD   m_currentOwner;
	DWORD   m_lockCount;
	BOOL    m_fTrace;        // Trace this one

public:
	CCritSec() {
		InitializeCriticalSection(&m_CritSec);
	};

	~CCritSec() {
		DeleteCriticalSection(&m_CritSec);
	};

	void Lock() {
		EnterCriticalSection(&m_CritSec);
	};

	void Unlock() {
		LeaveCriticalSection(&m_CritSec);
	};
};

class CAutoLock {

	// make copy constructor and assignment operator inaccessible

	CAutoLock(const CAutoLock &refAutoLock);
	CAutoLock &operator=(const CAutoLock &refAutoLock);

protected:
	CCritSec * m_pLock;

public:
	CAutoLock(CCritSec * plock)
	{
		m_pLock = plock;
		m_pLock->Lock();
	};

	~CAutoLock() {
		m_pLock->Unlock();
	};
};
extern CCritSec g_csLock;

extern PEditorState state;

extern HTEXTURE		gTexParticle[9];
extern CGLSprite*		gSprParticle[9];

bool HandleKeys(int key);
bool DoCommands(int id);

void cmdChangePreset(int n);
void cmdSavePreset(int n);
void cmdLoadFromFile(int n);
void cmdLoadPreset();
void cmdChangeColor(int id);


#define CMD_EXIT				1
#define CMD_HELP				2
#define CMD_PRESET1				3
#define CMD_PRESET2				4
#define CMD_PRESET3				5
#define CMD_PRESET4				6
#define CMD_PRESET5				7
#define CMD_PRESET6				8
#define CMD_PRESET7				9
#define CMD_PRESET8				10
#define CMD_PRESET9				11
#define CMD_BOUNDINGBOX			12
#define CMD_NPARTICLES			13
#define CMD_FPS					14

#define CMD_SYS_LIFETIME		15
#define CMD_SYS_TLIFETIME		16
#define CMD_SYS_LIFECONT		17
#define CMD_SYS_EMISSION		18
#define CMD_SYS_TEMISSION		19
#define CMD_SYS_PARLIFETIMEMIN	20
#define CMD_SYS_PARLIFETIMEMAX	21
#define CMD_SYS_PARLIFETIMELOCK	22
#define CMD_SYS_BLENDADDITIVE	23
#define CMD_SYS_BLENDBLEND		24
#define CMD_SYS_TEXTURE			25

#define CMD_PM_DIRECTION		26
#define CMD_PM_TDIRECTION		27
#define CMD_PM_RELATIVE			28
#define CMD_PM_SPREAD			29
#define CMD_PM_TSPREAD			30

#define CMD_PM_STARTSPEEDMIN	31
#define CMD_PM_STARTSPEEDMAX	32
#define CMD_PM_STARTSPEEDLOCK	33

#define CMD_PM_GRAVITYMIN		34
#define CMD_PM_GRAVITYMAX		35
#define CMD_PM_GRAVITYLOCK		36

#define CMD_PM_RADIALMIN		37
#define CMD_PM_RADIALMAX		38
#define CMD_PM_RADIALLOCK		39

#define CMD_PM_TANGENTIALMIN	40
#define CMD_PM_TANGENTIALMAX	41
#define CMD_PM_TANGENTIALLOCK	42

#define CMD_PA_SIZESTART		43
#define CMD_PA_SIZEEND			44
#define CMD_PA_SIZEVAR			45
#define CMD_PA_SIZELOCK			46

#define CMD_PA_SPINSTART		47
#define CMD_PA_SPINEND			48
#define CMD_PA_SPINVAR			49
#define CMD_PA_SPINLOCK			50

#define CMD_PA_ALPHASTART	51
#define CMD_PA_ALPHAEND		52
#define CMD_PA_ALPHAVAR		53
#define CMD_PA_ALPHALOCK		54

#define CMD_PA_RSTART			55
#define CMD_PA_GSTART			56
#define CMD_PA_BSTART			57
#define CMD_PA_REND				58
#define CMD_PA_GEND			59
#define CMD_PA_BEND				60
#define CMD_PA_RGBVAR		61
//glp
#define  CMD_SYS_ONESHOT			62

#define  CMD_EMISSION_POINT		63

#define  CMD_RECT_X						64
#define  CMD_RECT_Y						65
#define  CMD_RECT_WIDTH				66
#define  CMD_RECT_HEIGHT				67
#define  CMD_EMISSION_RECT		68	

#define  CMD_CIRCLE_IN_A				69
#define  CMD_CIRCLE_IN_B				70
#define  CMD_EMISSION_CIRCLE		71	

#define  CMD_CIRCLE_OUT_A			72
#define  CMD_CIRCLE_OUT_B			73
#define  CMD_EMISSION_RING		74

#define CMD_ORIGIN_POINT_X		75
#define CMD_ORIGIN_POINT_Y		76

#define CMD_PRESET1_CHECK			77
#define CMD_PRESET2_CHECK			78
#define CMD_PRESET3_CHECK			79
#define CMD_PRESET4_CHECK			80
#define CMD_PRESET5_CHECK			81
#define CMD_PRESET6_CHECK			82
#define CMD_PRESET7_CHECK			83
#define CMD_PRESET8_CHECK			84
#define CMD_PRESET9_CHECK			85

#define CMD_FIX_START_SPEED		86
#define TEXT_CIRCLE_IN_A				87
#define TEXT_CIRCLE_IN_B				88
#define TEXT_CIRCLE_OUT_A			89
#define TEXT_CIRCLE_OUT_B			90

int glp();
#endif
