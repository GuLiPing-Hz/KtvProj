#ifndef PARSEFILE__H__
#define PARSEFILE__H__
#include <map>
#include <string>
#include "hgehelp/ParticleInfo.h"

class VarValue
{
public:
	std::string value;

	float asFloat() const{
		return (float)atof(value.c_str());
	}
	int asInt() const{
		return atoi(value.c_str());
	}
	const char* asString() const{
		return value.c_str();
	}
};

typedef std::map<std::string,VarValue> MAPKEYVALUE;
//@ע��ר�����ڽ���ʹ��plist�ļ���ʽ���������ϵͳ���ļ�
class CParseFile
{
public:
	CParseFile();
	virtual ~CParseFile();

	static bool ParseFilePlist(const char* _PlistFile,MAPKEYVALUE& _Map);
	static bool ChangeToPsi(const MAPKEYVALUE& _Map,structParticleSystemInfo& info);

	bool parseFilePlist(const char* _PlistFile);
	bool chageToPsi(structParticleSystemInfo& info);
private:
	MAPKEYVALUE m_mapKeyValue;
};

#endif//PARSEFILE__H__
