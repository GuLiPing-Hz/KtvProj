// UpgradePsi.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "string.h"
#include "../ParticleEdit/hgehelp/ParticleInfo.h"


struct OldstructParticleSystemInfo
{
	CGLSprite*		sprite;    // blend mode //ǰ��16λ�Ժ���ܸ���
	//�Ƿ�һ��
	bool					bOneShot;
	//�Ƿ���ɢ
	bool					bSpread;
	//////////////////////////////////////////////////////////////////////////glp
	//����������
	int					nEmitter;
	//�������ɿ�
	CGRect			pointCreateRect;
	float					circleInsideA;
	float					circleOutsideA;
	float					circleInsideB;
	float					circleOutsideB;

	//ԭ������
	float					fOriginX;
	float					fOriginY;
	//////////////////////////////////////////////////////////////////////////
	//һ�뷢���������
	int		nEmission;
	//����ϵͳ������
	float		fLifetime;
	//����ϵͳ��������С ���ֵ
	float		fParticleLifeMin;
	float		fParticleLifeMax;
	//����
	float		fDirection;
	//���ŽǶ�
	float		fSpread;
	//�Ƿ����
	bool		bRelative;
	//�Ƿ��Ƿ�Ϊ����
	bool		btwoEnds;
	//�ٶ���Сֵ ���ֵ
	float		fSpeedMin;
	float		fSpeedMax;
	//�滻����ֵ ��������
	hgeVector2 fForceMin;
	hgeVector2 fForceMax;
	//˳ʱ����ʱ��
	float		fRadialAccelMin;
	float		fRadialAccelMax;
	float		fTangentialAccelMin;
	float		fTangentialAccelMax;
	//��С�Ŀ���
	float		fSizeStart;
	float		fSizeEnd;
	float		fSizeVarStart;
	//������ת
	float		fSpinStart;
	float		fSpinEnd;
	float		fSpinVarStart;
	//��ɫ�仯
	hgeColor	colColorStart; // + alpha
	hgeColor	colColorEnd;
	float			fColorVar;
	//͸���ȿɱ���
	float			fAlphaVar;

	//������Ϣ
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
	newInfo.sprite = oldInfo.sprite;    // blend mode //ǰ��16λ�Ժ���ܸ���
	//�Ƿ�һ��
	newInfo.bOneShot = oldInfo.bOneShot;
	//�Ƿ���ɢ
	newInfo.bSpread = oldInfo.bSpread;
	//////////////////////////////////////////////////////////////////////////glp
	//����������
	newInfo.nEmitter = oldInfo.nEmitter;
	//�������ɿ�
	newInfo.pointCreateRect = oldInfo.pointCreateRect;
	newInfo.circleInsideA = oldInfo.circleInsideA;
	newInfo.circleOutsideA = oldInfo.circleOutsideA;
	newInfo.circleInsideB = oldInfo.circleInsideB;
	newInfo.circleOutsideB = oldInfo.circleOutsideB;

	//ԭ������
	newInfo.fOriginX = oldInfo.fOriginX;
	newInfo.fOriginY = oldInfo.fOriginY;
	//////////////////////////////////////////////////////////////////////////
	//һ�뷢���������
	newInfo.nEmission = oldInfo.nEmission;
	//����ϵͳ������
	newInfo.fLifetime = oldInfo.fLifetime;
	//����ϵͳ��������С ���ֵ
	newInfo.fParticleLifeMin = oldInfo.fParticleLifeMin;
	newInfo.fParticleLifeMax = oldInfo.fParticleLifeMax;
	//����
	newInfo.fDirection = oldInfo.fDirection;
	//���ŽǶ�
	newInfo.fSpread = fabs(oldInfo.fSpread);
	//�Ƿ����
	newInfo.bRelative = oldInfo.bRelative;
	//�Ƿ��Ƿ�Ϊ����
	newInfo.btwoEnds = oldInfo.btwoEnds;
	//�ٶ���Сֵ ���ֵ
	newInfo.fSpeedMin = oldInfo.fSpeedMin;
	newInfo.fSpeedMax = oldInfo.fSpeedMax;
	//�滻����ֵ ��������
	newInfo.fForceMin = oldInfo.fForceMin;
	newInfo.fForceMax = oldInfo.fForceMax;
	//˳ʱ����ʱ��
	newInfo.fRadialAccelMin = oldInfo.fRadialAccelMin;
	newInfo.fRadialAccelMax = oldInfo.fRadialAccelMax;
	newInfo.fTangentialAccelMin = oldInfo.fTangentialAccelMin;
	newInfo.fTangentialAccelMax = oldInfo.fTangentialAccelMax;

	//��С�Ŀ���
	newInfo.fSizeStart = oldInfo.fSizeStart;
	newInfo.fSizeEnd = oldInfo.fSizeEnd;
	newInfo.fSizeVarStart = fabs(oldInfo.fSizeVarStart);
	//������ת
	newInfo.fSpinStart = oldInfo.fSpinStart;
	newInfo.fSpinEnd = oldInfo.fSpinEnd;
	newInfo.fSpinVarStart = fabs(oldInfo.fSpinVarStart);
	//��ɫ�仯
	newInfo.colColorStart = oldInfo.colColorStart;
	newInfo.colColorEnd = oldInfo.colColorEnd;
	newInfo.fColorVar = fabs(oldInfo.fColorVar);
	//͸���ȿɱ���
	newInfo.fAlphaVar = fabs(oldInfo.fAlphaVar);

	//������Ϣ
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

