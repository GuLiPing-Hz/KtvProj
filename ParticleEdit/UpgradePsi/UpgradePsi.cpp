// UpgradePsi.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "string.h"
#include "../ParticleEdit/hgehelp/ParticleInfo.h"


struct OldstructParticleSystemInfo
{
	CGLSprite*		sprite;    // blend mode //前面16位以后可能更多
	//是否一次
	bool					bOneShot;
	//是否扩散
	bool					bSpread;
	//////////////////////////////////////////////////////////////////////////glp
	//发射器类型
	int					nEmitter;
	//粒子生成框
	CGRect			pointCreateRect;
	float					circleInsideA;
	float					circleOutsideA;
	float					circleInsideB;
	float					circleOutsideB;

	//原点坐标
	float					fOriginX;
	float					fOriginY;
	//////////////////////////////////////////////////////////////////////////
	//一秒发射多少粒子
	int		nEmission;
	//粒子系统的寿命
	float		fLifetime;
	//粒子系统的寿命最小 最大值
	float		fParticleLifeMin;
	float		fParticleLifeMax;
	//方向
	float		fDirection;
	//扩张角度
	float		fSpread;
	//是否相对
	bool		bRelative;
	//是否是分为两边
	bool		btwoEnds;
	//速度最小值 最大值
	float		fSpeedMin;
	float		fSpeedMax;
	//替换重力值 方向力量
	hgeVector2 fForceMin;
	hgeVector2 fForceMax;
	//顺时、逆时针
	float		fRadialAccelMin;
	float		fRadialAccelMax;
	float		fTangentialAccelMin;
	float		fTangentialAccelMax;
	//大小的控制
	float		fSizeStart;
	float		fSizeEnd;
	float		fSizeVarStart;
	//自身旋转
	float		fSpinStart;
	float		fSpinEnd;
	float		fSpinVarStart;
	//颜色变化
	hgeColor	colColorStart; // + alpha
	hgeColor	colColorEnd;
	float			fColorVar;
	//透明度可变量
	float			fAlphaVar;

	//新增信息
	char textureFile[260];
	char maskFile[260];
// 	bool bColorOscillate;
// 
// 	hgeColor colColorMin;
// 	hgeColor colColorMax;
// 	hgeColor colColorStep;
// 	float fStepColorVar;
// 	float fStepAlphaVar;
};

bool GetNewInfo(int i,structParticleSystemInfo& newInfo)
{
	char oldName[260] = {0};
	structParticleSystemInfo/*OldstructParticleSystemInfo*/ oldInfo = {0};
	DWORD size;
	sprintf(oldName,"D:\\glp\\VSProject\\2005\\PlayerS\\Debug\\data\\psi\\particle%d.psi",i);
	HANDLE hF = CreateFile( oldName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if(hF == INVALID_HANDLE_VALUE) return false;
	ReadFile(hF, &oldInfo, sizeof(oldInfo), &size, NULL );
	CloseHandle(hF);
	memcpy(&newInfo,&oldInfo,sizeof(oldInfo));
	newInfo.sprite = (CGLSprite*)(((((DWORD_PTR)oldInfo.sprite)&0xffff0000)&2)?
		((BLEND_ALPHABLEND|BLEND_ZWRITE)<<16):((BLEND_ALPHAADD|BLEND_ZWRITE)<<16));

	return true;
	newInfo.sprite = oldInfo.sprite;    // blend mode //前面16位以后可能更多
	//是否一次
	newInfo.bOneShot = oldInfo.bOneShot;
	//是否扩散
	newInfo.bSpread = oldInfo.bSpread;
	//////////////////////////////////////////////////////////////////////////glp
	//发射器类型
	newInfo.nEmitter = oldInfo.nEmitter;
	//粒子生成框
	newInfo.pointCreateRect = oldInfo.pointCreateRect;
	newInfo.circleInsideA = oldInfo.circleInsideA;
	newInfo.circleOutsideA = oldInfo.circleOutsideA;
	newInfo.circleInsideB = oldInfo.circleInsideB;
	newInfo.circleOutsideB = oldInfo.circleOutsideB;

	//原点坐标
	newInfo.fOriginX = oldInfo.fOriginX;
	newInfo.fOriginY = oldInfo.fOriginY;
	//////////////////////////////////////////////////////////////////////////
	//一秒发射多少粒子
	newInfo.nEmission = oldInfo.nEmission;
	//粒子系统的寿命
	newInfo.fLifetime = oldInfo.fLifetime;
	//粒子系统的寿命最小 最大值
	newInfo.fParticleLifeMin = oldInfo.fParticleLifeMin;
	newInfo.fParticleLifeMax = oldInfo.fParticleLifeMax;
	//方向
	newInfo.fDirection = oldInfo.fDirection;
	//扩张角度
	newInfo.fSpread = fabs(oldInfo.fSpread);
	//是否相对
	newInfo.bRelative = oldInfo.bRelative;
	//是否是分为两边
	newInfo.btwoEnds = oldInfo.btwoEnds;
	//速度最小值 最大值
	newInfo.fSpeedMin = oldInfo.fSpeedMin;
	newInfo.fSpeedMax = oldInfo.fSpeedMax;
	//替换重力值 方向力量
	newInfo.fForceMin = oldInfo.fForceMin;
	newInfo.fForceMax = oldInfo.fForceMax;
	//顺时、逆时针
	newInfo.fRadialAccelMin = oldInfo.fRadialAccelMin;
	newInfo.fRadialAccelMax = oldInfo.fRadialAccelMax;
	newInfo.fTangentialAccelMin = oldInfo.fTangentialAccelMin;
	newInfo.fTangentialAccelMax = oldInfo.fTangentialAccelMax;

	//大小的控制
	newInfo.fSizeStart = oldInfo.fSizeStart;
	newInfo.fSizeEnd = oldInfo.fSizeEnd;
	newInfo.fSizeVarStart = fabs(oldInfo.fSizeVarStart);
	//自身旋转
	newInfo.fSpinStart = oldInfo.fSpinStart;
	newInfo.fSpinEnd = oldInfo.fSpinEnd;
	newInfo.fSpinVarStart = fabs(oldInfo.fSpinVarStart);
	//颜色变化
	newInfo.colColorStart = oldInfo.colColorStart;
	newInfo.colColorEnd = oldInfo.colColorEnd;
	newInfo.fColorVar = fabs(oldInfo.fColorVar);
	//透明度可变量
	newInfo.fAlphaVar = fabs(oldInfo.fAlphaVar);

	//新增信息
	strcpy(newInfo.textureFile,oldInfo.textureFile);
	strcpy(newInfo.maskFile,oldInfo.maskFile);
// 	newInfo.bColorOscillate = oldInfo.bColorOscillate;
// 
// 	newInfo.colColorMin = oldInfo.colColorMin;
// 	newInfo.colColorMax = oldInfo.colColorMax;
// 	newInfo.colColorStep = oldInfo.colColorStep;
// 	newInfo.fStepColorVar = oldInfo.fStepColorVar;
// 	newInfo.fStepAlphaVar = oldInfo.fStepAlphaVar;
	return true;
}

void WriteToFile(int i,const structParticleSystemInfo& newInfo)
{
	char newName[260] = {0};
	DWORD size;
	sprintf(newName,"C:\\Users\\Administrator\\Desktop\\newpsi\\particle%d.psi",i);
	HANDLE hF = CreateFile( newName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if(hF == INVALID_HANDLE_VALUE) return;
	WriteFile(hF, &newInfo, sizeof(structParticleSystemInfo), &size, NULL );
	CloseHandle(hF);
}

int _tmain(int argc, _TCHAR* argv[])
{

	const int total = 45;
	
	//int i = 1;
	//const char* pName = "particlebg2";
	for (int i=1;i<=total;i++)
	{
		structParticleSystemInfo newInfo = {0};
		GetNewInfo(i,newInfo);
		//strcpy(newInfo.textureFile,pName);
		WriteToFile(i,newInfo);
	}

	return 0;
}

