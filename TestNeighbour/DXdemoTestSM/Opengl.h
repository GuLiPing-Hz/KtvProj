#ifndef OPENGL__H__
#define  OPENGL__H__

#include "GLDef.h"
#include <string>
#include <vector>
#include "log/loghelp.h"

CGPoint CGPointMake(float x,float y,float z=0);

CGSize CGSizeMake(int x,int y);

CGRect CGRectMake(float x,float y,int w,int h);

CGRect CGRectMake(const CGPoint& point,const CGSize& size);

CGRect CGRectIntersection(const CGRect& a,const CGRect& b);//������Rect�ཻ����

//�����ļ�
void			Ini_SetInt(const char *szIniFile,const char *section, const char *name, int value);
int 			Ini_GetInt(const char *szIniFile,const char *section, const char *name, int def_val);
void			Ini_SetFloat(const char *szIniFile,const char *section, const char *name, float value);
float			Ini_GetFloat(const char *szIniFile,const char *section, const char *name, float def_val);
void			Ini_SetString(const char *szIniFile,const char *section, const char *name, const char *value);
char*		Ini_GetString(const char *szIniFile,const char *section, const char *name, const char *def_val);

void Str2Wstr(IN const std::string str,OUT std::wstring &wstr);
void Wstr2Str(IN const std::wstring wstr,OUT std::string &str);
void GetOnlyDifferentFont(IN OUT std::wstring &font_all);

void initLogFile(const char* logFile);
void writeLog(const wchar_t *msg);

#endif //OPENGL__H__
