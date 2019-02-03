// Opengl.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Opengl.h"
#include <math.h>
#include <locale.h>
#include "util.h"
// #include "DX/RenderEngine.h"
#include "GLListDef.h"

//#include "CWavePlayer.h"

//extern			HWND													g_hWnd;
//extern			CWavePlayer										g_Player;
// extern			bool														g_bGLThread;//控制绘画线程函数


CGPoint CGPointMake(float x,float y,float z)
{
	CGPoint point;
	point.x = x;
	point.y = y;
	point.z = z;
	return point;
}

CGSize CGSizeMake(int x,int y)
{
	CGSize cgSize;
	cgSize.width = x;
	cgSize.height = y;
	return cgSize;
}

CGRect CGRectMake(float x,float y,int w,int h)
{
	CGRect rect;
	rect.origin.x = x;
	rect.origin.y = y;
	rect.size.width = w;
	rect.size.height = h;
	return rect;
}

CGRect CGRectMake(const CGPoint& point,const CGSize& size)
{
	CGRect rect;
	rect.origin = point;
	rect.size = size;
	return rect;
}



CGRect CGRectIntersection(const CGRect& a,const CGRect& b)
{
	float Lmax = max(a.origin.x,b.origin.x);
	float Rmin = min(a.origin.x+a.size.width,b.origin.x+b.size.width);
	float Tmax = max(a.origin.y,b.origin.y);
	float Bmin = min(a.origin.y+a.size.height,b.origin.y+b.size.height);

	if (Lmax>=Rmin||Tmax>=Bmin)
	{
		return CGRectMake(0.0f,0.0f,0,0);
	}
	int w = (int)(Rmin-Lmax);
	int h = (int)(Bmin-Tmax);
	return CGRectMake(Lmax,Tmax,w,h);
}

void Str2Wstr(IN const std::string str,OUT std::wstring &wstr)
{
// 	std::wstring tmp_ws(str.begin(),str.end());
// 	wstr = tmp_ws;
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "chs"); 
	const char* _Source = str.c_str();
	size_t _Dsize = str.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	wstr = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());

}
void Wstr2Str(IN const std::wstring wstr,OUT std::string &str)
{
// 	std::string tmp_s(wstr.begin(),wstr.end());
// 	str = tmp_s;
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "chs"); 
	const wchar_t* _Source = wstr.c_str();
	size_t _Dsize = 2 * wstr.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	str = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());
}

void GetOnlyDifferentFont(std::wstring &font_all)
{
}

void initLogFile(const char* logFile)
{
	if (logFile)
	{
		strcpy(LOGPATH,logFile);
	}
}

void writeLog(const wchar_t *msg)
{
	if (msg == NULL)
	{
		return;
	}
	char buf[1024] = {0};
	WideCharToMultiByte(CP_ACP,0,msg,-1,buf,1023,NULL,NULL);

	SYSTEMTIME st;
	GetLocalTime(&st);

	FILE * fp = fopen(LOGPATH,"a");
	if (fp == NULL)
	{
		return;
	}
	fprintf(fp,"[%02d:%02d:%02d.%05d],%s\n",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds,buf);
	fclose(fp);
}
//////////////////////////////////////////////////////////////////////////

void  Ini_SetInt(const char *szIniFile,const char *section, const char *name, int value)
{
	char buf[256];

	if(szIniFile) {
		sprintf(buf,"%d",value);
		WritePrivateProfileString(section, name, buf, szIniFile);
	}
}


int  Ini_GetInt(const char *szIniFile,const char *section, const char *name, int def_val)
{
	char buf[256];

	if(szIniFile) {
		if(GetPrivateProfileString(section, name, "", buf, sizeof(buf), szIniFile))
		{ return atoi(buf); }
		else { return def_val; }
	}
	return def_val;
}


void  Ini_SetFloat(const char *szIniFile,const char *section, const char *name, float value)
{
	char buf[256];

	if(szIniFile) {
		sprintf(buf,"%f",value);
		WritePrivateProfileString(section, name, buf, szIniFile);
	}
}


float  Ini_GetFloat(const char *szIniFile,const char *section, const char *name, float def_val)
{
	char buf[256];

	if(szIniFile) {
		if(GetPrivateProfileString(section, name, "", buf, sizeof(buf), szIniFile))
		{ return (float)atof(buf); }
		else { return def_val; }
	}
	return def_val;
}


void  Ini_SetString(const char *szIniFile,const char *section, const char *name, const char *value)
{
	if(szIniFile) WritePrivateProfileString(section, name, value, szIniFile);
}


char*  Ini_GetString(const char *szIniFile,const char *section, const char *name, const char *def_val)
{
	static char szIniString[256] = {0};
	ZeroMemory(szIniString,256);
	if(szIniFile) GetPrivateProfileString(section, name, def_val, szIniString, sizeof(szIniString), szIniFile);
	else strcpy(szIniString, def_val);
	return szIniString;
}







