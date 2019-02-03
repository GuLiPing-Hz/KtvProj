/*
	ע��ʱ��:2014-4-25
	author: glp
	����ϵͳ������
*/
#ifndef PARTICLEINFO__H__
#define PARTICLEINFO__H__

#include "hgesprite.h"
#include "hgevector.h"
#include "hgecolor.h"
#include "../GLDef.h"

#define MAX_PARTICLES	1000
#define MAX_PSYSTEMS	50
//����ʱ�����������Ϣ
struct structParticle
{
	//λ��
	hgeVector2		vecLocation;

	//�˶�ģʽ------------------------begin
	bool bRadiusMode;
	//��ģʽ
	//�ٶ�
	hgeVector2		vecVelocity;
	//����
	hgeVector2		fForce;
	float		fRadialAccel;
	float		fTangentialAccel;
	//�뾶ģʽ
	float fAngle;//��λ����
	float fRadius;
	float fRadiusDelta;//ÿ��仯����
	float fRotatePerSecond;
	//�˶�ģʽ------------------------end

	//������ת����
	float		fSpin;
	float		fSpinDelta;
	//��С����
	float		fSize;
	float		fSizeDelta;

	//��ɫ����
	bool bColorOscillate;//true ��ɫ��ģʽ��false ��ɫ����ģʽ
	bool bAddr;//���������Ǽӻ��Ǽ�
	bool bAddg;
	bool bAddb;
	bool bAdda;
	hgeColor	colColor;		// + alpha
	hgeColor	colColorDelta;
	hgeColor colColorMin;
	hgeColor colColorMax;

	//����
	float		fAge;//��ǰ����
	float		fTerminalAge;//��������

	float fAlpha;//used for mask��most mode is 1.0f
	float z;
};
//�ļ�����Ҫ�����������Ϣ 
struct structParticleSystemInfo
{
	CGLSprite*		sprite;    // blend mode //ǰ��16λ�Ժ���ܸ���
	//�Ƿ�һ��
	bool					bOneShot;
	//�Ƿ���ɢ
	bool					bSpread;
	//////////////////////////////////////////////////////////////////////////glp
	//����������
	int					nEmitter;//plist modeAĬ��ʹ�þ��ο� modeBʹ�õ㣬���Ҳ���ɢ
	//�������ɿ�
	CGRect			pointCreateRect;//(_sourcePosition.x- _posVar.x * CCRANDOM_MINUS1_1(),_sourcePosition.y- _posVar.y * CCRANDOM_MINUS1_1())
	float					circleInsideA;
	float					circleOutsideA;
	float					circleInsideB;
	float					circleOutsideB;

	//ԭ������
	float					fOriginX;//plist Ĭ������λ�ã�640,320�����ܸ�������ϵ��Ҫת��һ�¡�
	float					fOriginY;//0
	//////////////////////////////////////////////////////////////////////////
	//һ�뷢���������
	int		nEmission;
	//����ϵͳ������
	float		fLifetime;
	//����ϵͳ��������С ���ֵ
	float		fParticleLifeMin;//plist�� MAX��0��_life-|_lifeVar|�� ~ _life+|_lifeVar| ��СΪ0
	float		fParticleLifeMax;//
	//����
	float		fDirection;//����
	float		fSpread;//���� == fDirectionVar
	//�Ƿ����
	bool		bRelative;
	//�Ƿ��Ƿ�Ϊ����
	bool		btwoEnds;

	//**************************************��ģʽ BEGIN********************
	//�ٶ���Сֵ ���ֵ
	//plist�ļ���speed-|speedVar| ~ speed+|speedVar|
	float		fSpeedMin;
	float		fSpeedMax;
	//�滻����ֵ ��������  /1.0f
	//plist�ļ��� 
	hgeVector2 fForceMin;//
	hgeVector2 fForceMax;
	//����������ٶ� /1.0f
	//plist�ļ���radialAccel-|radialAccelVar| ~ radialAccel+|radialAccelVar|
	//tangentialAccel-|tangentialAccelVar| ~ tangentialAccel+|tangentialAccelVar|
	float		fRadialAccelMin;
	float		fRadialAccelMax;
	float		fTangentialAccelMin;
	float		fTangentialAccelMax;
	//**************************************��ģʽ END********************

	//��С�Ŀ���  /1.0f
	float		fSizeStart;//_startSize-|_startSizeVar| ~ _startSize+|_startSizeVar| ȷ������0
	float		fSizeEnd;//-1�Ļ����Ǹ�fSizeStartһ�� _endSize-|_endSizeVar| ~ _endSize+|_endSizeVar| ȷ������0
	float		fSizeVarStart;//ȷ��>=0  
	float		fSizeVarEnd;//ȷ��>=0
	//������ת /300.0f
	float		fSpinStart;//_startSpin-|_startSpinVar| ~ _startSpin+|_startSpinVar|
	float		fSpinEnd;//_endSpin-|_endSpinVar| ~ _endSpin+|_endSpinVar|
	float		fSpinVarStart;//ȷ��>=0 
	float		fSpinVarEnd;//ȷ��>=0
	//��ɫ�仯
	hgeColor	colColorStart; // + alpha clampf(_startColor.r + _startColorVar.r * CCRANDOM_MINUS1_1(), 0, 1);
	hgeColor	colColorEnd;
	float			fColorVar;//ȷ��>=0
	//͸���ȿɱ���
	float			fAlphaVar;//ȷ��>=0

	//������Ϣ
	char textureFile[260];
	char maskFile[260];
	bool bColorOscillate;

	hgeColor colColorMin;
	hgeColor colColorMax;
	hgeColor colColorStep;
	float fStepColorVar;//ȷ��>=0
	float fStepAlphaVar;//ȷ��>=0

	bool bRadiusMode;//�Ƿ��ǰ뾶ģʽ
	//**************************************�뾶ģʽ BEGIN********************
	float fRadiusStart;
	float fRadiusStartVar;//ȷ��>=0
	float fRadiusEnd;
	float fRadiusEndVar;//ȷ��>=0
	float fRotatePerSecond;
	float fRotatePerSecondVar;//ȷ��>=0
	//**************************************�뾶ģʽ END********************
};

#endif//PARTICLEINFO__H__