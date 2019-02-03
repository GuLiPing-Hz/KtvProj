/*
** Haaf's Game Engine 1.81
** Copyright (C) 2008, Relish Games
** hge.relishgames.com
**
** Particle systems editor
*/
#include "stdafx.h"
#include "ParticleEditDlg.h"
#include "particleed.h"
#include <windows.h>
#include <string>
#include <vector>
#include "ximage/ximage.h"

extern CParticleEditDlg* g_pDlg;
extern bool bvectPreset[9];

using namespace std;

bool HandleKeys(int key)
{
	if(!state.ps[state.nPreset] && (key==HGEK_LBUTTON||key==HGEK_SPACE))
		return false;

	switch(key)
	{
		case 0:
			return false;

		case HGEK_ESCAPE:
			cmdSavePreset(state.nPreset);
			return true;

		case HGEK_TAB:
			state.bIFace = !state.bIFace;
			break;

		case HGEK_LBUTTON:
			if(state.mx>=168 && state.mx<=631) state.ps[state.nPreset]->Fire();
			break;

		case HGEK_SPACE:
			state.ps[state.nPreset]->Fire();
			break;

		case HGEK_1:
		case HGEK_2:
		case HGEK_3:
		case HGEK_4:
		case HGEK_5:
		case HGEK_6:
		case HGEK_7:
		case HGEK_8:
		case HGEK_9: cmdChangePreset(key-HGEK_1); break;
	}

	return false;
}

bool DoCommands(int id)
{
	float fVal;
	switch(id)
	{
		// Presets & stuff
	
		case 0:	return false;
		case CMD_EXIT: cmdSavePreset(state.nPreset); return true;
		case CMD_HELP: state.bHelp=ButtonGetState(CMD_HELP); break;
		case CMD_BOUNDINGBOX: state.bBBox=ButtonGetState(CMD_BOUNDINGBOX); break;

		case CMD_PRESET1:
		case CMD_PRESET2:
		case CMD_PRESET3:
		case CMD_PRESET4:
		case CMD_PRESET5:
		case CMD_PRESET6:
		case CMD_PRESET7:
		case CMD_PRESET8:
		case CMD_PRESET9: cmdChangePreset(id-CMD_PRESET1); break;

		case CMD_PRESET1_CHECK:
			{
				if (bvectPreset[0])
				{
					bvectPreset[0] = false;
					ButtonSetState(CMD_PRESET1_CHECK,false);
				}
				else
				{
					bvectPreset[0] = true;
					ButtonSetState(CMD_PRESET1_CHECK,true);
				}
				break;
			}
		case CMD_PRESET2_CHECK:
			{
				if (bvectPreset[1])
				{
					bvectPreset[1] = false;
					ButtonSetState(CMD_PRESET2_CHECK,false);
				}
				else
				{
					bvectPreset[1] = true;
					ButtonSetState(CMD_PRESET2_CHECK,true);
				}
				break;
			}
		case CMD_PRESET3_CHECK:
			{
				if (bvectPreset[2])
				{
					bvectPreset[2] = false;
					ButtonSetState(CMD_PRESET3_CHECK,false);
				}
				else
				{
					bvectPreset[2] = true;
					ButtonSetState(CMD_PRESET3_CHECK,true);
				}
				break;
			}
		case CMD_PRESET4_CHECK:
			{
				if (bvectPreset[3])
				{
					bvectPreset[3] = false;
					ButtonSetState(CMD_PRESET4_CHECK,false);
				}
				else
				{
					bvectPreset[3] = true;
					ButtonSetState(CMD_PRESET4_CHECK,true);
				}
				break;
			}
		case CMD_PRESET5_CHECK:
			{
				if (bvectPreset[4])
				{
					bvectPreset[4] = false;
					ButtonSetState(CMD_PRESET5_CHECK,false);
				}
				else
				{
					bvectPreset[4] = true;
					ButtonSetState(CMD_PRESET5_CHECK,true);
				}
				break;
			}
		case CMD_PRESET6_CHECK:
			{
				if (bvectPreset[5])
				{
					bvectPreset[5] = false;
					ButtonSetState(CMD_PRESET6_CHECK,false);
				}
				else
				{
					bvectPreset[5] = true;
					ButtonSetState(CMD_PRESET6_CHECK,true);
				}
				break;
			}
		case CMD_PRESET7_CHECK:
			{
				if (bvectPreset[6])
				{
					bvectPreset[6] = false;
					ButtonSetState(CMD_PRESET7_CHECK,false);
				}
				else
				{
					bvectPreset[6] = true;
					ButtonSetState(CMD_PRESET7_CHECK,true);
				}
				break;
			}
		case CMD_PRESET8_CHECK:
			{
				if (bvectPreset[7])
				{
					bvectPreset[7] = false;
					ButtonSetState(CMD_PRESET8_CHECK,false);
				}
				else
				{
					bvectPreset[7] = true;
					ButtonSetState(CMD_PRESET8_CHECK,true);
				}
				break;
			}
		case CMD_PRESET9_CHECK:
			{
				if (bvectPreset[8])
				{
					bvectPreset[8] = false;
					ButtonSetState(CMD_PRESET9_CHECK,false);
				}
				else
				{
					bvectPreset[8] = true;
					ButtonSetState(CMD_PRESET9_CHECK,true);
				}
				break;
			}

		// System parameters

		case CMD_SYS_LIFETIME:
			state.ps[state.nPreset]->m_info.fLifetime=SliderGetValue(CMD_SYS_LIFETIME);
			GetTextCtrl(CMD_SYS_TLIFETIME)->printf("%.1f",state.ps[state.nPreset]->m_info.fLifetime);
			ButtonSetState(CMD_SYS_LIFECONT, false);
			state.ps[state.nPreset]->Stop();
			break;
		
		case CMD_SYS_LIFECONT:
			if(ButtonGetState(CMD_SYS_LIFECONT))
			{
				state.ps[state.nPreset]->m_info.fLifetime=-1.0f;
				state.ps[state.nPreset]->Fire();
			}
			else
			{
				state.ps[state.nPreset]->m_info.fLifetime=SliderGetValue(CMD_SYS_LIFETIME);
				state.ps[state.nPreset]->Stop();
			}
			break;

		case CMD_SYS_ONESHOT:
			{
				if (state.ps[state.nPreset]->m_info.bOneShot)
				{
					state.ps[state.nPreset]->m_info.bOneShot = false;
					ButtonSetState(CMD_SYS_ONESHOT,false);
				}
				else
				{
					state.ps[state.nPreset]->m_info.bOneShot = true;
					ButtonSetState(CMD_SYS_ONESHOT,true);
				}
				break;
			}
		case CMD_SYS_EMISSION:
			state.ps[state.nPreset]->m_info.nEmission=int(SliderGetValue(CMD_SYS_EMISSION));
			GetTextCtrl(CMD_SYS_TEMISSION)->printf("%d",state.ps[state.nPreset]->m_info.nEmission);
			break;

		case CMD_SYS_PARLIFETIMELOCK:
			if(ButtonGetState(CMD_SYS_PARLIFETIMELOCK))
			{
				SliderSetValue(CMD_SYS_PARLIFETIMEMAX, SliderGetValue(CMD_SYS_PARLIFETIMEMIN));
				state.ps[state.nPreset]->m_info.fParticleLifeMax=state.ps[state.nPreset]->m_info.fParticleLifeMin;
			}
			break;

		case CMD_SYS_PARLIFETIMEMIN:
			state.ps[state.nPreset]->m_info.fParticleLifeMin=SliderGetValue(CMD_SYS_PARLIFETIMEMIN);
			if(ButtonGetState(CMD_SYS_PARLIFETIMELOCK))
			{
				SliderSetValue(CMD_SYS_PARLIFETIMEMAX, state.ps[state.nPreset]->m_info.fParticleLifeMin);
				state.ps[state.nPreset]->m_info.fParticleLifeMax=state.ps[state.nPreset]->m_info.fParticleLifeMin;
			}
			break;
		
		case CMD_SYS_PARLIFETIMEMAX:
			state.ps[state.nPreset]->m_info.fParticleLifeMax=SliderGetValue(CMD_SYS_PARLIFETIMEMAX);
			if(ButtonGetState(CMD_SYS_PARLIFETIMELOCK))
			{
				SliderSetValue(CMD_SYS_PARLIFETIMEMIN, state.ps[state.nPreset]->m_info.fParticleLifeMax);
				state.ps[state.nPreset]->m_info.fParticleLifeMin=state.ps[state.nPreset]->m_info.fParticleLifeMax;
			}
			break;

		case CMD_SYS_TEXTURE: 
			//sprParticles[state.nPreset]->SetFrame(sprParticles[state.nPreset]->GetFrame()+1); break;
			break;
		case CMD_SYS_BLENDADDITIVE:
			ButtonSetState(CMD_SYS_BLENDADDITIVE, true);
			ButtonSetState(CMD_SYS_BLENDBLEND, false);
			state.ps[state.nPreset]->m_info.sprite->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_ZWRITE);
			break;

		case CMD_SYS_BLENDBLEND:
			ButtonSetState(CMD_SYS_BLENDADDITIVE, false);
			ButtonSetState(CMD_SYS_BLENDBLEND, true);
			state.ps[state.nPreset]->m_info.sprite->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE);
			break;

		// Particle movement

		case CMD_PM_DIRECTION:
			state.ps[state.nPreset]->m_info.fDirection=SliderGetValue(CMD_PM_DIRECTION);
			GetTextCtrl(CMD_PM_TDIRECTION)->printf("%d",int(state.ps[state.nPreset]->m_info.fDirection*180/M_PI));
			break;
		
		case CMD_PM_RELATIVE:
			state.ps[state.nPreset]->m_info.bRelative=ButtonGetState(CMD_PM_RELATIVE);
			break;

		case CMD_PM_SPREAD:
			fVal=SliderGetValue(CMD_PM_SPREAD);
			state.ps[state.nPreset]->m_info.fSpread = fVal<0.0f?-fVal:fVal;
			GetTextCtrl(CMD_PM_TSPREAD)->printf("%d",int(state.ps[state.nPreset]->m_info.fSpread*180/M_PI));
			break;

		case CMD_PM_STARTSPEEDLOCK:
			if(ButtonGetState(CMD_PM_STARTSPEEDLOCK))
			{
				SliderSetValue(CMD_PM_STARTSPEEDMAX, SliderGetValue(CMD_PM_STARTSPEEDMIN));
				state.ps[state.nPreset]->m_info.fSpeedMax=state.ps[state.nPreset]->m_info.fSpeedMin;
			}
			break;

		case CMD_FIX_START_SPEED:
			{
				if (state.ps[state.nPreset]->m_info.btwoEnds)
				{
					ButtonSetState(CMD_FIX_START_SPEED,false);
				}
				else
				{
					ButtonSetState(CMD_FIX_START_SPEED,true);
				}
				state.ps[state.nPreset]->m_info.btwoEnds = !state.ps[state.nPreset]->m_info.btwoEnds;
				break;
			}

		case CMD_PM_STARTSPEEDMIN:
			state.ps[state.nPreset]->m_info.fSpeedMin=SliderGetValue(CMD_PM_STARTSPEEDMIN);
			if(ButtonGetState(CMD_PM_STARTSPEEDLOCK))
			{
				SliderSetValue(CMD_PM_STARTSPEEDMAX, state.ps[state.nPreset]->m_info.fSpeedMin);
				state.ps[state.nPreset]->m_info.fSpeedMax=state.ps[state.nPreset]->m_info.fSpeedMin;
			}
			break;
		
		case CMD_PM_STARTSPEEDMAX:
			state.ps[state.nPreset]->m_info.fSpeedMax=SliderGetValue(CMD_PM_STARTSPEEDMAX);
			if(ButtonGetState(CMD_PM_STARTSPEEDLOCK))
			{
				SliderSetValue(CMD_PM_STARTSPEEDMIN, state.ps[state.nPreset]->m_info.fSpeedMax);
				state.ps[state.nPreset]->m_info.fSpeedMin=state.ps[state.nPreset]->m_info.fSpeedMax;
			}
			break;
		case CMD_PM_RADIALLOCK:
			if(ButtonGetState(CMD_PM_RADIALLOCK))
			{
				SliderSetValue(CMD_PM_RADIALMAX, SliderGetValue(CMD_PM_RADIALMIN));
				state.ps[state.nPreset]->m_info.fRadialAccelMax=state.ps[state.nPreset]->m_info.fRadialAccelMin;
			}
			break;

		case CMD_PM_RADIALMIN:
			state.ps[state.nPreset]->m_info.fRadialAccelMin=SliderGetValue(CMD_PM_RADIALMIN);
			if(ButtonGetState(CMD_PM_RADIALLOCK))
			{
				SliderSetValue(CMD_PM_RADIALMAX, state.ps[state.nPreset]->m_info.fRadialAccelMin);
				state.ps[state.nPreset]->m_info.fRadialAccelMax=state.ps[state.nPreset]->m_info.fRadialAccelMin;
			}
			break;
		
		case CMD_PM_RADIALMAX:
			state.ps[state.nPreset]->m_info.fRadialAccelMax=SliderGetValue(CMD_PM_RADIALMAX);
			if(ButtonGetState(CMD_PM_RADIALLOCK))
			{
				SliderSetValue(CMD_PM_RADIALMIN, state.ps[state.nPreset]->m_info.fRadialAccelMax);
				state.ps[state.nPreset]->m_info.fRadialAccelMin=state.ps[state.nPreset]->m_info.fRadialAccelMax;
			}
			break;

		case CMD_PM_TANGENTIALLOCK:
			if(ButtonGetState(CMD_PM_TANGENTIALLOCK))
			{
				SliderSetValue(CMD_PM_TANGENTIALMAX, SliderGetValue(CMD_PM_TANGENTIALMIN));
				state.ps[state.nPreset]->m_info.fTangentialAccelMax=state.ps[state.nPreset]->m_info.fTangentialAccelMin;
			}
			break;

		case CMD_PM_TANGENTIALMIN:
			state.ps[state.nPreset]->m_info.fTangentialAccelMin=SliderGetValue(CMD_PM_TANGENTIALMIN);
			if(ButtonGetState(CMD_PM_TANGENTIALLOCK))
			{
				SliderSetValue(CMD_PM_TANGENTIALMAX, state.ps[state.nPreset]->m_info.fTangentialAccelMin);
				state.ps[state.nPreset]->m_info.fTangentialAccelMax=state.ps[state.nPreset]->m_info.fTangentialAccelMin;
			}
			break;
		
		case CMD_PM_TANGENTIALMAX:
			state.ps[state.nPreset]->m_info.fTangentialAccelMax=SliderGetValue(CMD_PM_TANGENTIALMAX);
			if(ButtonGetState(CMD_PM_TANGENTIALLOCK))
			{
				SliderSetValue(CMD_PM_TANGENTIALMIN, state.ps[state.nPreset]->m_info.fTangentialAccelMax);
				state.ps[state.nPreset]->m_info.fTangentialAccelMin=state.ps[state.nPreset]->m_info.fTangentialAccelMax;
			}
			break;
//////////////////////////////////////////////////////////////////////////
			//Particle Emission glp
		case CMD_EMISSION_POINT:
			{
				if (!(state.ps[state.nPreset]->m_info.nEmitter & EMITTER_POINT))
				{
					state.ps[state.nPreset]->m_info.nEmitter = EMITTER_POINT;
					ButtonSetState(CMD_EMISSION_POINT,true);
					ButtonSetState(CMD_EMISSION_RECT,false);
					ButtonSetState(CMD_EMISSION_CIRCLE,false);
					ButtonSetState(CMD_EMISSION_RING,false);
				}
				break;
			}
		case CMD_EMISSION_RECT:
			{
				if (!(state.ps[state.nPreset]->m_info.nEmitter & EMITTER_RECT))
				{
					state.ps[state.nPreset]->m_info.nEmitter = EMITTER_RECT;
					ButtonSetState(CMD_EMISSION_RECT,true);
					ButtonSetState(CMD_EMISSION_POINT,false);
					ButtonSetState(CMD_EMISSION_CIRCLE,false);
					ButtonSetState(CMD_EMISSION_RING,false);
				}
				break;
			}
		case CMD_EMISSION_CIRCLE:
			{
				if (!(state.ps[state.nPreset]->m_info.nEmitter & EMITTER_CIRCLE))
				{
					state.ps[state.nPreset]->m_info.nEmitter = EMITTER_CIRCLE;
					ButtonSetState(CMD_EMISSION_CIRCLE,true);
					ButtonSetState(CMD_EMISSION_POINT,false);
					ButtonSetState(CMD_EMISSION_RECT,false);
					ButtonSetState(CMD_EMISSION_RING,false);
				}
				break;
			}
		case CMD_EMISSION_RING:
			{
				if (!(state.ps[state.nPreset]->m_info.nEmitter & EMITTER_RING))
				{
					state.ps[state.nPreset]->m_info.nEmitter = EMITTER_RING;
					ButtonSetState(CMD_EMISSION_RING,true);
					ButtonSetState(CMD_EMISSION_POINT,false);
					ButtonSetState(CMD_EMISSION_RECT,false);
					ButtonSetState(CMD_EMISSION_CIRCLE,false);
				}
				break;
			}
		case CMD_RECT_X:
			{
// 				if (!(state.ps->m_info.nEmitter & EMITTER_RECT))
// 				{
// 					break;
// 				}
				state.ps[state.nPreset]->m_info.pointCreateRect.origin.x = SliderGetValue(CMD_RECT_X);
				break;
			}
		case CMD_RECT_Y:
			{
				state.ps[state.nPreset]->m_info.pointCreateRect.origin.y = SliderGetValue(CMD_RECT_Y);
				break;
			}
		case CMD_RECT_WIDTH:
			{
				state.ps[state.nPreset]->m_info.pointCreateRect.size.width = SliderGetValue(CMD_RECT_WIDTH);
				break;
			}
		case CMD_RECT_HEIGHT:
			{
				state.ps[state.nPreset]->m_info.pointCreateRect.size.height = SliderGetValue(CMD_RECT_HEIGHT);
				break;
			}
		case CMD_CIRCLE_IN_A:
			{
				state.ps[state.nPreset]->m_info.circleInsideA = SliderGetValue(CMD_CIRCLE_IN_A);
				break;
			}
		case TEXT_CIRCLE_IN_A:
			{
				state.ps[state.nPreset]->m_info.circleInsideA += 1;
				SliderSetValue(CMD_CIRCLE_IN_A,state.ps[state.nPreset]->m_info.circleInsideA);
				break;
			}
		case CMD_CIRCLE_IN_B:
			{
				state.ps[state.nPreset]->m_info.circleInsideB = SliderGetValue(CMD_CIRCLE_IN_B);
				break;
			}
		case TEXT_CIRCLE_IN_B:
			{
				state.ps[state.nPreset]->m_info.circleInsideB += 1;
				SliderSetValue(CMD_CIRCLE_IN_B,state.ps[state.nPreset]->m_info.circleInsideB);
				break;
			}
		case CMD_CIRCLE_OUT_A:
			{
				state.ps[state.nPreset]->m_info.circleOutsideA = SliderGetValue(CMD_CIRCLE_OUT_A);
				break;
			}
		case TEXT_CIRCLE_OUT_A:
			{
				state.ps[state.nPreset]->m_info.circleOutsideA += 1;
				SliderSetValue(CMD_CIRCLE_OUT_A,state.ps[state.nPreset]->m_info.circleOutsideA);
				break;
			}
		case CMD_CIRCLE_OUT_B:
			{
				state.ps[state.nPreset]->m_info.circleOutsideB = SliderGetValue(CMD_CIRCLE_OUT_B);
				break;
			}
		case TEXT_CIRCLE_OUT_B:
			{
				state.ps[state.nPreset]->m_info.circleOutsideB += 1;
				SliderSetValue(CMD_CIRCLE_OUT_B,state.ps[state.nPreset]->m_info.circleOutsideB);
				break;
			}
		case CMD_ORIGIN_POINT_X:
			{
				state.ps[state.nPreset]->m_info.fOriginX = SliderGetValue(CMD_ORIGIN_POINT_X);
				break;
			}
		case CMD_ORIGIN_POINT_Y:
			{
				state.ps[state.nPreset]->m_info.fOriginY = SliderGetValue(CMD_ORIGIN_POINT_Y);
				break;
			}
		// Particle appearance
		
		case CMD_PA_SIZELOCK:
			if(ButtonGetState(CMD_PA_SIZELOCK))
			{
				SliderSetValue(CMD_PA_SIZEEND, SliderGetValue(CMD_PA_SIZESTART));
				state.ps[state.nPreset]->m_info.fSizeEnd=state.ps[state.nPreset]->m_info.fSizeStart;
			}
			break;

		case CMD_PA_SIZESTART:
			state.ps[state.nPreset]->m_info.fSizeStart=SliderGetValue(CMD_PA_SIZESTART)/PARTICLE_SIZE;
			if(ButtonGetState(CMD_PA_SIZELOCK))
			{
				SliderSetValue(CMD_PA_SIZEEND, state.ps[state.nPreset]->m_info.fSizeStart*PARTICLE_SIZE);
				state.ps[state.nPreset]->m_info.fSizeEnd=state.ps[state.nPreset]->m_info.fSizeStart;
			}
			break;
		
		case CMD_PA_SIZEEND:
			state.ps[state.nPreset]->m_info.fSizeEnd=SliderGetValue(CMD_PA_SIZEEND)/PARTICLE_SIZE;
			if(ButtonGetState(CMD_PA_SIZELOCK))
			{
				SliderSetValue(CMD_PA_SIZESTART, state.ps[state.nPreset]->m_info.fSizeEnd*PARTICLE_SIZE);
				state.ps[state.nPreset]->m_info.fSizeStart=state.ps[state.nPreset]->m_info.fSizeEnd;
			}
			break;

		case CMD_PA_SIZEVAR:
			fVal=SliderGetValue(CMD_PA_SIZEVAR);
			state.ps[state.nPreset]->m_info.fSizeVarStart = fVal<0.0f?-fVal:fVal;
			break;

		case CMD_PA_SPINLOCK:
			if(ButtonGetState(CMD_PA_SPINLOCK))
			{
				SliderSetValue(CMD_PA_SPINEND, SliderGetValue(CMD_PA_SPINSTART));
				state.ps[state.nPreset]->m_info.fSpinEnd=state.ps[state.nPreset]->m_info.fSpinStart;
			}
			break;

		case CMD_PA_SPINSTART:
			state.ps[state.nPreset]->m_info.fSpinStart=SliderGetValue(CMD_PA_SPINSTART);
			if(ButtonGetState(CMD_PA_SPINLOCK))
			{
				SliderSetValue(CMD_PA_SPINEND, state.ps[state.nPreset]->m_info.fSpinStart);
				state.ps[state.nPreset]->m_info.fSpinEnd=state.ps[state.nPreset]->m_info.fSpinStart;
			}
			break;
		
		case CMD_PA_SPINEND:
			state.ps[state.nPreset]->m_info.fSpinEnd=SliderGetValue(CMD_PA_SPINEND);
			if(ButtonGetState(CMD_PA_SPINLOCK))
			{
				SliderSetValue(CMD_PA_SPINSTART, state.ps[state.nPreset]->m_info.fSpinEnd);
				state.ps[state.nPreset]->m_info.fSpinStart=state.ps[state.nPreset]->m_info.fSpinEnd;
			}
			break;

		case CMD_PA_SPINVAR:
			fVal=SliderGetValue(CMD_PA_SPINVAR);
			state.ps[state.nPreset]->m_info.fSpinVarStart = fVal<0.0f?-fVal:fVal;
			break;

		case CMD_PA_ALPHALOCK:
			if(ButtonGetState(CMD_PA_ALPHALOCK))
			{
				SliderSetValue(CMD_PA_ALPHAEND, SliderGetValue(CMD_PA_ALPHASTART));
				state.ps[state.nPreset]->m_info.colColorEnd.a=state.ps[state.nPreset]->m_info.colColorStart.a;
			}
			break;

		case CMD_PA_ALPHASTART:
			state.ps[state.nPreset]->m_info.colColorStart.a=SliderGetValue(CMD_PA_ALPHASTART);
			if(ButtonGetState(CMD_PA_ALPHALOCK))
			{
				SliderSetValue(CMD_PA_ALPHAEND, state.ps[state.nPreset]->m_info.colColorStart.a);
				state.ps[state.nPreset]->m_info.colColorEnd.a=state.ps[state.nPreset]->m_info.colColorStart.a;
			}
			break;
		
		case CMD_PA_ALPHAEND:
			state.ps[state.nPreset]->m_info.colColorEnd.a=SliderGetValue(CMD_PA_ALPHAEND);
			if(ButtonGetState(CMD_PA_ALPHALOCK))
			{
				SliderSetValue(CMD_PA_ALPHASTART, state.ps[state.nPreset]->m_info.colColorEnd.a);
				state.ps[state.nPreset]->m_info.colColorStart.a=state.ps[state.nPreset]->m_info.colColorEnd.a;
			}
			break;

		case CMD_PA_ALPHAVAR:
			fVal=SliderGetValue(CMD_PA_ALPHAVAR);
			state.ps[state.nPreset]->m_info.fAlphaVar = fVal<0.0f?-fVal:fVal;
			break;

		case CMD_PA_RSTART:
		case CMD_PA_GSTART:
		case CMD_PA_BSTART:
		case CMD_PA_REND:
		case CMD_PA_GEND:
		case CMD_PA_BEND: cmdChangeColor(id); break;

		case CMD_PA_RGBVAR:
			fVal=SliderGetValue(CMD_PA_RGBVAR);
			state.ps[state.nPreset]->m_info.fColorVar = fVal<0.0f?-fVal:fVal;
			break;

	}

	return false;
}

void cmdChangePreset(int n)
{
	//state.ps[state.nPreset]->Stop();
	ButtonSetState(CMD_PRESET1+state.nPreset, false);
	ButtonSetState(CMD_PRESET1+n, true);
	cmdSavePreset(state.nPreset);
	state.nPreset=n;
	cmdLoadFromFile(n);
}

void cmdSavePreset(int n)
{
	HANDLE hF;
	DWORD size;
	char filename[_MAX_PATH];

	if(!state.ps[state.nPreset]) return;

	ZeroMemory(filename, _MAX_PATH);
	GetModuleFileName(GetModuleHandle(NULL), filename, _MAX_PATH);
	string s(filename);
	s=s.substr(0,s.rfind('\\'));
	//s=s.substr(0,s.rfind('\\'))+"\\"+"particle"+char('1'+n)+".psi";
	sprintf(filename,"%s\\particle%d.psi",s.c_str(),n+PSI_START);
	//strcpy(filename, s.c_str());

	CGLSprite* pTmpSprite = state.ps[state.nPreset]->m_info.sprite;
	state.ps[state.nPreset]->m_info.sprite=(CGLSprite*)(0 | (pTmpSprite?(pTmpSprite->GetBlendMode()<<16):0));
	hF = CreateFile( filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if(hF == INVALID_HANDLE_VALUE) return;
	WriteFile(hF, &state.ps[state.nPreset]->m_info, sizeof(structParticleSystemInfo), &size, NULL );
 	CloseHandle(hF);
	state.ps[state.nPreset]->m_info.sprite=pTmpSprite;
}

void cmdLoadFromFile(int n)
{
	HANDLE hF;
	DWORD size;
	static char curDir[260] = {0};
	char filename[_MAX_PATH] = {0};

	if(!state.ps[state.nPreset]) return;

	GetModuleFileName(GetModuleHandle(NULL), curDir, _MAX_PATH);
	string s(curDir);
	s=s.substr(0,s.rfind('\\'));
	//s=s.substr(0,s.rfind('\\'))+"\\"+"particle"+char('1'+n)+".psi";
	sprintf(filename,"%s\\particle%d.psi",s.c_str(),n+PSI_START);
	//strcpy(filename, s.c_str());

	hF = CreateFile( filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if(hF == INVALID_HANDLE_VALUE) return;
	ReadFile(hF, &state.ps[state.nPreset]->m_info, sizeof(structParticleSystemInfo), &size, NULL );
	CloseHandle(hF);

	cmdLoadPreset();
}

void cmdLoadPreset()
{
	DWORD col1, col2;
	static char curDir[260] = {0};
	GetModuleFileName(GetModuleHandle(NULL), curDir, _MAX_PATH);
	std::string s(curDir);
	s=s.substr(0,s.rfind('\\'));

	CAutoLock lock(&g_csLock);
	//sprParticles[state.nPreset]->SetFrame((DWORD)state.ps[state.nPreset]->m_info.sprite & 0xFFFF);
	//sprParticles[state.nPreset]->SetBlendMode((DWORD)state.ps[state.nPreset]->m_info.sprite >> 16);
	unsigned int w,h;
	if(gTexParticle[state.nPreset])
		hge->Texture_Free(gTexParticle[state.nPreset]);
	std::string texFile = s+"\\"+state.ps[state.nPreset]->m_info.textureFile;
	FILE* fp = fopen(texFile.c_str(),"r");
	if(fp == NULL)
	{
		MessageBox(NULL,"纹理图片缺失",texFile.c_str(),MB_TOPMOST|MB_OK | MB_ICONERROR);
		exit(-1);
		return ;
	}
	fclose(fp);

	gTexParticle[state.nPreset] = hge->Texture_Load(texFile.c_str(),w,h);
	CGLSprite* pSprite = new CGLSprite(gTexParticle[state.nPreset],0,0,w,h);
	pSprite->SetHotSpot(w/2,h/2);
	pSprite->SetBlendMode((((DWORD)state.ps[state.nPreset]->m_info.sprite)>>16)&0xffff);
	if(gSprParticle[state.nPreset])
		delete gSprParticle[state.nPreset];
	gSprParticle[state.nPreset] = pSprite;
	state.ps[state.nPreset]->m_info.sprite=gSprParticle[state.nPreset];

	if(state.ps[state.nPreset]->m_info.nEmitter == EMITTER_MASK)
	{
		CxImage ximg;
		//应用程序目录
		bool b = ximg.Load(state.ps[state.nPreset]->m_info.maskFile,CXIMAGE_FORMAT_PNG);
		ximg.Flip(false,true);
		if(b)
			state.ps[state.nPreset]->setMaskAlpha(ximg.AlphaGetPointer(),ximg.GetWidth(),ximg.GetHeight());
	}

	if (g_pDlg)
	{
		g_pDlg->SetGui(s.c_str());
	}
	// System parameters
	
// 	if(state.ps[state.nPreset]->m_info.fLifetime==-1.0f)
// 	{
// 		SliderSetValue(CMD_SYS_LIFETIME, 5.0f);
// 		ButtonSetState(CMD_SYS_LIFECONT, true);
// 		GetTextCtrl(CMD_SYS_TLIFETIME)->printf("%.1f", 5.0f);
// 		state.ps[state.nPreset]->Fire();
// 	}
// 	else
// 	{
// 		SliderSetValue(CMD_SYS_LIFETIME, state.ps[state.nPreset]->m_info.fLifetime);
// 		ButtonSetState(CMD_SYS_LIFECONT, false);
// 		GetTextCtrl(CMD_SYS_TLIFETIME)->printf("%.1f", state.ps[state.nPreset]->m_info.fLifetime);
// 	}
// 
// 	if (state.ps[state.nPreset]->m_info.bOneShot)
// 	{
// 		ButtonSetState(CMD_SYS_ONESHOT,true);
// 	}
// 	else
// 	{
// 		ButtonSetState(CMD_SYS_ONESHOT,false);
// 	}
// 
// 	SliderSetValue(CMD_SYS_EMISSION, (float)state.ps[state.nPreset]->m_info.nEmission);
// 	GetTextCtrl(CMD_SYS_TEMISSION)->printf("%d", state.ps[state.nPreset]->m_info.nEmission);
// 
// 	SliderSetValue(CMD_SYS_PARLIFETIMEMIN, state.ps[state.nPreset]->m_info.fParticleLifeMin);
// 	SliderSetValue(CMD_SYS_PARLIFETIMEMAX, state.ps[state.nPreset]->m_info.fParticleLifeMax);
// 	ButtonSetState(CMD_SYS_PARLIFETIMELOCK, false);
// 
// 	if(sprParticles[state.nPreset]->GetBlendMode() & BLEND_ALPHABLEND)
// 	{
// 		ButtonSetState(CMD_SYS_BLENDADDITIVE, false);
// 		ButtonSetState(CMD_SYS_BLENDBLEND, true);
// 	}
// 	else
// 	{
// 		ButtonSetState(CMD_SYS_BLENDADDITIVE, true);
// 		ButtonSetState(CMD_SYS_BLENDBLEND, false);
// 	}

	// Particle movement

// 	SliderSetValue(CMD_PM_DIRECTION, state.ps[state.nPreset]->m_info.fDirection);
// 	GetTextCtrl(CMD_PM_TDIRECTION)->printf("%d",int(state.ps[state.nPreset]->m_info.fDirection*180/M_PI));
// 	ButtonSetState(CMD_PM_RELATIVE, state.ps[state.nPreset]->m_info.bRelative);
// 
// 	SliderSetValue(CMD_PM_SPREAD, state.ps[state.nPreset]->m_info.fSpread);
// 	GetTextCtrl(CMD_PM_TSPREAD)->printf("%d",int(state.ps[state.nPreset]->m_info.fSpread*180/M_PI));
// 
// 	SliderSetValue(CMD_PM_STARTSPEEDMIN, state.ps[state.nPreset]->m_info.fSpeedMin);
// 	SliderSetValue(CMD_PM_STARTSPEEDMAX, state.ps[state.nPreset]->m_info.fSpeedMax);
// 	ButtonSetState(CMD_PM_STARTSPEEDLOCK, false);
// 	if (state.ps[state.nPreset]->m_info.btwoEnds)
// 	{
// 		ButtonSetState(CMD_FIX_START_SPEED,true);
// 	}
// 	else
// 	{
// 		ButtonSetState(CMD_FIX_START_SPEED,false);
// 	}
// 
// 	SliderSetValue(CMD_PM_RADIALMIN, state.ps[state.nPreset]->m_info.fRadialAccelMin);
// 	SliderSetValue(CMD_PM_RADIALMAX, state.ps[state.nPreset]->m_info.fRadialAccelMax);
// 	ButtonSetState(CMD_PM_RADIALLOCK, false);
// 
// 	SliderSetValue(CMD_PM_TANGENTIALMIN, state.ps[state.nPreset]->m_info.fTangentialAccelMin);
// 	SliderSetValue(CMD_PM_TANGENTIALMAX, state.ps[state.nPreset]->m_info.fTangentialAccelMax);
// 	ButtonSetState(CMD_PM_TANGENTIALLOCK, false);
// 
//////////////////////////////////////////////////////////////////////////glp
	//Particle Emission
// 	if (state.ps[state.nPreset]->m_info.nEmitter & EMITTER_POINT)
// 	{
// 		ButtonSetState(CMD_EMISSION_POINT,true);
// 	}
// 	else if (state.ps[state.nPreset]->m_info.nEmitter & EMITTER_RECT)
// 	{
// 		ButtonSetState(CMD_EMISSION_RECT,true);
// 	}
// 	else if (state.ps[state.nPreset]->m_info.nEmitter & EMITTER_CIRCLE)
// 	{
// 		ButtonSetState(CMD_EMISSION_CIRCLE,true);
// 	}
// 	else if (state.ps[state.nPreset]->m_info.nEmitter & EMITTER_RING)
// 	{
// 		ButtonSetState(CMD_EMISSION_RING,true);
// 	}
// 	SliderSetValue(CMD_RECT_X,state.ps[state.nPreset]->m_info.pointCreateRect.origin.x);
// 	SliderSetValue(CMD_RECT_Y,state.ps[state.nPreset]->m_info.pointCreateRect.origin.y);
// 	SliderSetValue(CMD_RECT_WIDTH,state.ps[state.nPreset]->m_info.pointCreateRect.size.width);
// 	SliderSetValue(CMD_RECT_HEIGHT,state.ps[state.nPreset]->m_info.pointCreateRect.size.height);
// 
// 	SliderSetValue(CMD_CIRCLE_IN_A,state.ps[state.nPreset]->m_info.circleInsideA);
// 	SliderSetValue(CMD_CIRCLE_IN_B,state.ps[state.nPreset]->m_info.circleInsideB);
// 	SliderSetValue(CMD_CIRCLE_OUT_A,state.ps[state.nPreset]->m_info.circleOutsideA);
// 	SliderSetValue(CMD_CIRCLE_OUT_B,state.ps[state.nPreset]->m_info.circleOutsideB);
// 
// 	SliderSetValue(CMD_ORIGIN_POINT_X,state.ps[state.nPreset]->m_info.fOriginX);
// 	SliderSetValue(CMD_ORIGIN_POINT_Y,state.ps[state.nPreset]->m_info.fOriginY);
	//////////////////////////////////////////////////////////////////////////
	// Particle appearance

// 	SliderSetValue(CMD_PA_SIZESTART, state.ps[state.nPreset]->m_info.fSizeStart*PARTICLE_SIZE);
// 	SliderSetValue(CMD_PA_SIZEEND, state.ps[state.nPreset]->m_info.fSizeEnd*PARTICLE_SIZE);
// 	SliderSetValue(CMD_PA_SIZEVAR, state.ps[state.nPreset]->m_info.fSizeVar);
// 	ButtonSetState(CMD_PA_SIZELOCK, false);
// 
// 	SliderSetValue(CMD_PA_SPINSTART, state.ps[state.nPreset]->m_info.fSpinStart);
// 	SliderSetValue(CMD_PA_SPINEND, state.ps[state.nPreset]->m_info.fSpinEnd);
// 	SliderSetValue(CMD_PA_SPINVAR, state.ps[state.nPreset]->m_info.fSpinVar);
// 	ButtonSetState(CMD_PA_SPINLOCK, false);
// 
	SliderSetValue(CMD_PA_ALPHASTART, state.ps[state.nPreset]->m_info.colColorStart.a);
	SliderSetValue(CMD_PA_ALPHAEND, state.ps[state.nPreset]->m_info.colColorEnd.a);
	SliderSetValue(CMD_PA_ALPHAVAR, state.ps[state.nPreset]->m_info.fAlphaVar);
	ButtonSetState(CMD_PA_ALPHALOCK, false);

	SliderSetValue(CMD_PA_RSTART, state.ps[state.nPreset]->m_info.colColorStart.r);
	SliderSetValue(CMD_PA_GSTART, state.ps[state.nPreset]->m_info.colColorStart.g);
	SliderSetValue(CMD_PA_BSTART, state.ps[state.nPreset]->m_info.colColorStart.b);
	SliderSetValue(CMD_PA_REND, state.ps[state.nPreset]->m_info.colColorEnd.r);
	SliderSetValue(CMD_PA_GEND, state.ps[state.nPreset]->m_info.colColorEnd.g);
	SliderSetValue(CMD_PA_BEND, state.ps[state.nPreset]->m_info.colColorEnd.b);
	SliderSetValue(CMD_PA_RGBVAR, state.ps[state.nPreset]->m_info.fColorVar);

	col1=state.ps[state.nPreset]->m_info.colColorStart.GetHWColor() | 0xFF000000;
	col2=state.ps[state.nPreset]->m_info.colColorEnd.GetHWColor() | 0xFF000000;
	sprColor->SetColor(col1,0); sprColor->SetColor(col1,1);//设置四边形4个点的颜色
	sprColor->SetColor(col2,2); sprColor->SetColor(col2,3);

	static bool b = true;
	if(b)
	{
		ButtonSetState(CMD_PRESET1_CHECK,true);
		b = false;
	}
}

void cmdChangeColor(int id)
{
	DWORD col1, col2;
	float val=SliderGetValue(id);

	switch(id)
	{
		case CMD_PA_RSTART: state.ps[state.nPreset]->m_info.colColorStart.r=val; break;
		case CMD_PA_GSTART: state.ps[state.nPreset]->m_info.colColorStart.g=val; break;
		case CMD_PA_BSTART: state.ps[state.nPreset]->m_info.colColorStart.b=val; break;
		case CMD_PA_REND: state.ps[state.nPreset]->m_info.colColorEnd.r=val; break;
		case CMD_PA_GEND: state.ps[state.nPreset]->m_info.colColorEnd.g=val; break;
		case CMD_PA_BEND:  state.ps[state.nPreset]->m_info.colColorEnd.b=val; break;
	}

	col1=state.ps[state.nPreset]->m_info.colColorStart.GetHWColor() | 0xFF000000;
	col2=state.ps[state.nPreset]->m_info.colColorEnd.GetHWColor() | 0xFF000000;
	sprColor->SetColor(col1,0); sprColor->SetColor(col1,1);
	sprColor->SetColor(col2,2); sprColor->SetColor(col2,3);
}
