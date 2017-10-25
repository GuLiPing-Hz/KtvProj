#include "logsystem.h"


#include <stdio.h>
#include <time.h>
#include <string.h>
#include <string>
#include <stdarg.h>


static char s_logpath[260];
static char s_filename[256];
static std::string s_logname;
static const char* ls_getlogname();

void ls_setpath(char* logpath)
{
	memset(s_logpath, 0, sizeof(s_logpath));
	strncpy(s_logpath, logpath, sizeof(s_logpath));
}

void ls_setfilename(char* filename)
{
	memset(s_filename, 0, sizeof(s_filename));
	strncpy(s_filename, filename, sizeof(s_filename));
}

const char* ls_getlogname()
{
	time_t t = time(0);
	struct tm* st = localtime(&t);

	char ft[32];
	strftime(ft, sizeof(ft)-1, ".%Y-%m-%d", st);

	s_logname = std::string(s_logpath) + std::string(s_filename)+std::string(ft) + std::string(".log");

	return s_logname.c_str();
}

void ls_write(char * f, int l, char* format, ...)
{
	FILE *file = fopen(ls_getlogname(), "ab");
	if (file != NULL)
	{
		//time
		time_t t = time(0);
		struct tm* st = localtime(&t);
		static char formattime[32];
		strftime(formattime, sizeof(formattime)-1, "[%Y-%m-%d %H:%M:%S] ", st);
		fwrite(formattime, strlen(formattime), 1, file);

		// buf
		va_list valist; 
		va_start(valist, format);
		static char buf[16384] = {0};
		int len = vsprintf(buf, format, valist);
		va_end(valist);
		fwrite(buf, len, 1, file);

		// file line
		char* pos = strrchr(f, '\\');
		static char filename[256];
		if (pos != NULL)
			strncpy(filename, pos+1, sizeof(filename));
		else
			strncpy(filename, f, sizeof(filename));
		static char fl[256] = {0};
		sprintf(fl, " (%s:%d)", filename, l);
		fwrite(fl, strlen(fl), 1, file);

		// \r\n
		fwrite("\r\n", strlen("\r\n"), 1, file);
		fclose(file);
	}
}
