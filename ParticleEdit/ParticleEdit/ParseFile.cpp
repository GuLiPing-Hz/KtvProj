#include "ParseFile.h"
#include "xml/tinyxml.h"

#define MAX(a,b) (((a)>(b)) ? (a) : (b))
#define MIN(a,b) (((a)<(b)) ? (a) : (b))
#define FABSF(a) (((a)<0.0f) ?-(a) : (a))

CParseFile::CParseFile()
{

}
CParseFile::~CParseFile()
{
	m_mapKeyValue.clear();
}

bool CParseFile::ParseFilePlist(const char* _PlistFile,MAPKEYVALUE& _Map)
{
	if (!_PlistFile)
		return false;

	if (!strstr(_PlistFile,".plist"))
		return false;

	TiXmlDocument doc;
	if(!doc.LoadFile(_PlistFile))
		return false;

	TiXmlElement* pElement = doc.FirstChildElement();//plist element
	//Ŀǰֻ֧��1.0�汾
	if(!pElement || strcmp(pElement->Value(),"plist")!=0 || strcmp(pElement->Attribute("version"),"1.0")!=0)
		return false;

	pElement = pElement->FirstChildElement();//dict element
	if(!pElement || strcmp(pElement->Value(),"dict")!=0)
		return false;

	pElement = pElement->FirstChildElement();//���ݿ�ʼ

	_Map.clear();//���֮ǰ������
	do 
	{
		std::string key = pElement->GetText();//key
		pElement = pElement->NextSiblingElement();
		if(!pElement)
			return false;
		VarValue _value;
		_value.value= pElement->GetText();//value

		_Map[key] = _value;
	} while (pElement=pElement->NextSiblingElement());//��ȡ��һ��Key

	return true;
}

#define FINDITEM(x)

inline void FindMapFloat(const MAPKEYVALUE& _Map,const char* name,float& v,float def=0.0f)
{
	MAPKEYVALUE::const_iterator it = _Map.find(name);
	if(it!=_Map.end())
		v = it->second.asFloat();
	else
		v = def;
}
inline void FindMapString(const MAPKEYVALUE& _Map,const char* name,char* v)
{
	MAPKEYVALUE::const_iterator it = _Map.find(name);
	if(it!=_Map.end())
		strcpy(v , it->second.asString());
	else
		v[0] = 0;
}

bool CParseFile::ChangeToPsi(const MAPKEYVALUE& _Map,structParticleSystemInfo& info)
{
	memset(&info,0,sizeof(info));

	FindMapFloat(_Map,"angle",info.fDirection);
	info.fDirection =MATH_DEG_TO_RAD(info.fDirection);
	//info.fDirection = (_Map[std::string("angle")]).asFloat();
	
	FindMapFloat(_Map,"angleVariance",info.fSpread);
	info.fSpread = MATH_DEG_TO_RAD(FABSF(info.fSpread));

	//����Ĭ����ͻ��
	info.sprite = (CGLSprite*)(DWORD_PTR) ((BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE)<<16);
	FindMapFloat(_Map,"duration",info.fLifetime);//-1 һֱ����
	float life,lifeVar;
	FindMapFloat(_Map,"particleLifespan",life);
	FindMapFloat(_Map,"particleLifespanVariance",lifeVar);
	lifeVar = FABSF(lifeVar);
	info.fParticleLifeMin = MAX(0,life-lifeVar);
	info.fParticleLifeMax = MAX(0,life+lifeVar);

	//λ��
	float posX,posY;
	FindMapFloat(_Map,"sourcePositionx",posX);
	FindMapFloat(_Map,"sourcePositiony",posY);
	info.fOriginX = WIN_WIDTH/2.0f+posX;
	info.fOriginY = WIN_HEIGHT/2.0f-posY;
	//Ĭ�Ͼ��ο���
	float varX;
	FindMapFloat(_Map,"sourcePositionVariancex",varX);
	varX = FABSF(varX);
	float varY;
	FindMapFloat(_Map,"sourcePositionVariancey",varY);
	varY = FABSF(varY);
	info.nEmitter = EMITTER_RECT;
	info.pointCreateRect.origin.x = info.fOriginX-varX;
	info.pointCreateRect.origin.y = info.fOriginY-varY;
	info.pointCreateRect.size.width = 2*varX;
	info.pointCreateRect.size.height = 2*varY;
	//��λʱ�䷢��
	float fEmission;
	FindMapFloat(_Map,"emissionRate",fEmission);
	info.nEmission = (int)fEmission;

	//��ɫ�仯
	FindMapFloat(_Map,"startColorAlpha",info.colColorStart.a);
	FindMapFloat(_Map,"startColorBlue",info.colColorStart.b);
	FindMapFloat(_Map,"startColorGreen",info.colColorStart.g);
	FindMapFloat(_Map,"startColorRed",info.colColorStart.r);

	FindMapFloat(_Map,"finishColorAlpha",info.colColorEnd.a);
	FindMapFloat(_Map,"finishColorBlue",info.colColorEnd.b);
	FindMapFloat(_Map,"finishColorGreen",info.colColorEnd.g);
	FindMapFloat(_Map,"finishColorRed",info.colColorEnd.r);
	//�ɱ䶼��Ϊ0����Ϊ���ݻ���������
	//����Alpha�ɱ䣬��ɫ�ɱ䶼��ʱ����ȡ��

	//����
	//FindMapFloat(_Map,"rotationStart",info.fSpinStart);
	info.fSpinStart = 0.0f;//MATH_DEG_TO_RAD(info.fSpinStart);//��ʼ��ת��plist��û������
	//FindMapFloat(_Map,"rotationStartVariance",info.fSpinVarStart);
	info.fSpinVarStart = 0.0f;//MATH_DEG_TO_RAD(FABSF(info.fSpinVarStart));
	FindMapFloat(_Map,"rotationEnd",info.fSpinEnd);
	info.fSpinEnd = info.fSpinEnd/100.0f;
	FindMapFloat(_Map,"rotationEndVariance",info.fSpinVarEnd);
	info.fSpinVarEnd = (FABSF(info.fSpinVarEnd))/100.0f;

	//��С �������߶Դ�С����ⲻͬ������û�д��ڵı�����ϵ������ֱ�Ӱ��ǱߵĴ�С����1.0f������Ϊ���䴦��
	//FindMapFloat(_Map,"startParticleSize",info.fSizeStart);
	//FindMapFloat(_Map,"startParticleSizeVariance",info.fSizeVarStart);
	//info.fSizeVarStart = FABSF(info.fSizeVarStart);
	//FindMapFloat(_Map,"finishParticleSize",info.fSizeEnd);
	info.fSizeStart = 1.0f;
	info.fSizeEnd = -1.0f;
	if(info.fSizeEnd == -1.0f)
	{
		info.fSizeEnd = info.fSizeStart;
		info.fSizeVarStart = 0.0f;
		info.fSizeVarEnd = 0.0f;
	}
// 	else
// 	{
// 		FindMapFloat(_Map,"finishParticleSizeVariance",info.fSizeVarEnd);
// 		info.fSizeVarEnd = FABSF(info.fSizeVarEnd);
// 	}

	//��ģʽ
	//���귽����ٶ�
	FindMapFloat(_Map,"gravityx",info.fForceMin.x);
	info.fForceMax.x = info.fForceMin.x;
	FindMapFloat(_Map,"gravityy",info.fForceMin.y);
	info.fForceMax.y = info.fForceMin.y;

	//������ٶ�
	
	float radialA;
	FindMapFloat(_Map,"radialAcceleration",radialA);
	float radialAV;
	FindMapFloat(_Map,"radialAccelVariance",radialAV);
	radialAV = FABSF(radialAV);
	info.fRadialAccelMin = radialA-radialAV;
	info.fRadialAccelMax = radialA+radialAV;

	//������ٶ�
	float tangentialA;
	FindMapFloat(_Map,"tangentialAcceleration",tangentialA);
	float tangentialAV;
	FindMapFloat(_Map,"tangentialAccelVariance",tangentialAV);
	tangentialAV = FABSF(tangentialAV);
	info.fTangentialAccelMin = (tangentialA-tangentialAV)/10.0f;
	info.fTangentialAccelMax = (tangentialA+tangentialAV)/10.0f;

	float speed;
	FindMapFloat(_Map,"speed",speed);
	float speedVar;
	FindMapFloat(_Map,"speedVariance",speedVar);
	speedVar = FABSF(speedVar);
	info.fSpeedMin = speed - speedVar;
	info.fSpeedMax = speed + speedVar;

	//�뾶ģʽ
	FindMapFloat(_Map,"maxRadius",info.fRadiusStart);
	FindMapFloat(_Map,"maxRadiusVariance",info.fRadiusStartVar);
	info.fRadiusStartVar = FABSF(info.fRadiusStartVar);
	FindMapFloat(_Map,"minRadius",info.fRadiusEnd);
	if(info.fRadiusEnd == -1.0f)
	{
		info.fRadiusEnd = info.fRadiusStart;
		info.fRadiusStartVar = 0.0f;
		info.fRadiusEndVar = 0.0f;
	}
	else
	{
		FindMapFloat(_Map,"minRadiusVariance",info.fRadiusEndVar);
		info.fRadiusEndVar = FABSF(info.fRadiusStartVar);
	}
	FindMapFloat(_Map,"rotatePerSecond",info.fRotatePerSecond);
	info.fRotatePerSecond = MATH_DEG_TO_RAD(info.fRotatePerSecond);
	FindMapFloat(_Map,"rotatePerSecondVariance",info.fRotatePerSecondVar);
	info.fRotatePerSecondVar = MATH_DEG_TO_RAD(FABSF(info.fRotatePerSecondVar));

	FindMapString(_Map,"textureFileName",info.textureFile);

	return true;
}

bool CParseFile::parseFilePlist(const char* _PlistFile)
{
	return ParseFilePlist(_PlistFile,m_mapKeyValue);
}

bool CParseFile::chageToPsi(structParticleSystemInfo& info)
{
	return ChangeToPsi(m_mapKeyValue,info);
}
