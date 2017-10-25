#include "jniloghelp.h"

#ifdef WIN32

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
void Printf(char* format, ...)
{
	va_list args;
	va_start(args, format);
	char buf[256];
	vsprintf(buf, format, args);
	OutputDebugStringA(buf);
	va_end(args);
}

#endif
