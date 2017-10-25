#ifndef LOGSYSTEM__H__
#define LOGSYSTEM__H__

void ls_setpath(char* logpath);
void ls_setfilename(char* filename);
void ls_write(char * f, int l, char* format, ...);

//#define //TRACE_INFO((...) ls_write(__FILE__, __LINE__, __VA_ARGS__)

// 新文件系统
//#include "../base/Log.h"   //GenericLog
#define TRACE_INFO

#endif//LOGSYSTEM__H__
