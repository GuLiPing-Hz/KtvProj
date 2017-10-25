/*

	by glp
	字符编码转换，
	只封装了GBK UTF-8之间的转换

*/

#include "code.h"
#include <stdio.h> 
#include <string.h> 
#include <wchar.h>
#ifdef _WIN32
#include<windows.h>
	
int GBKToUTF8(/*out*/char* dst,/*out*/int* len_out,/*in*/const char * src, /*in*/int len_in)
{
	  if(src == NULL)    return -1; 

      int nRetLen = 0; 

      //获取转换到Unicode编码后所需要的字符空间长度 
      nRetLen = MultiByteToWideChar(CP_ACP, 0,src, -1, NULL, NULL);         

      WCHAR *lpUnicodeStr = new WCHAR[nRetLen + 1];        
       //为Unicode字符串空间 

      //转换到Unicode编码 
      nRetLen = MultiByteToWideChar(CP_ACP, 0,src, -1, lpUnicodeStr, nRetLen); 

      if(!nRetLen) 
      { 
              delete []lpUnicodeStr; 
              return -1; 
      } 

      //获取转换到UTF8编码后所需要的字符空间长度 
      nRetLen = WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr,-1, NULL, NULL, NULL, NULL);         

      CHAR *lpUTF8Str = new CHAR[nRetLen + 1]; 

      nRetLen = WideCharToMultiByte(CP_UTF8, 0, lpUnicodeStr,-1, lpUTF8Str, nRetLen, NULL, NULL);       
      //转换到UTF8编码 

      if(!nRetLen) 
      { 
              delete []lpUnicodeStr; 
              delete []lpUTF8Str; 
              return -2; 
      } 
  
  	if(dst && *len_out >= nRetLen)
  		strncpy(dst,lpUTF8Str,nRetLen);
  	if(len_out)
  		*len_out = nRetLen;

      delete []lpUTF8Str; 
      delete []lpUnicodeStr; 
      return 0; 
}
	
int UTF8ToGBK(/*out*/char* dst,/*out*/int* len_out,/*in*/const char * src, /*in*/int len_in) 
{ 
      if(src == NULL)    return -1; 

      int nRetLen = 0; 

      //获取转换到Unicode编码后所需要的字符空间长度 
      nRetLen = MultiByteToWideChar(CP_UTF8, 0,src, -1, NULL, NULL);         

      wchar_t *lpUnicodeStr = new wchar_t[nRetLen + 1];        
       //为Unicode字符串空间 

      //转换到Unicode编码 
      nRetLen = MultiByteToWideChar(CP_UTF8, 0,src, -1, lpUnicodeStr, nRetLen); 

      if(!nRetLen) 
      { 
              delete []lpUnicodeStr; 
              return -1; 
      } 

      //获取转换到GBK编码后所需要的字符空间长度 
      nRetLen = WideCharToMultiByte(CP_ACP, 0, lpUnicodeStr,-1, NULL, NULL, NULL, NULL);         

      char *lpGBKStr = new char[nRetLen + 1]; 

      nRetLen = WideCharToMultiByte(CP_ACP, 0, lpUnicodeStr,-1, lpGBKStr, nRetLen, NULL, NULL);       
      //转换到GBK编码 

      if(!nRetLen) 
      { 
              delete []lpUnicodeStr; 
              delete []lpGBKStr; 
              return -2; 
      } 
  
  	if(dst && *len_out >= nRetLen)
  		strncpy(dst,lpGBKStr,nRetLen);
  	if(len_out)
  		*len_out = nRetLen;

      delete []lpGBKStr; 
      delete []lpUnicodeStr; 
      return 0; 
}
#else
#include <iconv.h> 
	
int GBKToUTF8(/*out*/char* dst,/*out*/int* len_out,/*in*/const char * src, /*in*/int len_in)
{
	iconv_t cd; 

     cd = iconv_open("UTF-8","GBK"); 

     if((iconv_t)-1 == cd) { 
             return -1; 
     } 

     retsize = iconv(cd, &src, (size_t *)&len_in, &dst, (size_t *)len_out); 

     if((size_t)-1 == retsize) { 
             return -2;    
     } 

     iconv_close(cd); 
     
     return 0;
}
int UTF8ToGBK(/*out*/char* dst,/*out*/int* len_out,/*in*/const char * src, /*in*/int len_in) 
{
	iconv_t cd; 

     cd = iconv_open("GBK","UTF-8"); 

     if((iconv_t)-1 == cd) { 
             return -1; 
     } 

     retsize = iconv(cd, &src, (size_t *)&len_in, &dst, (size_t *)len_out); 

     if((size_t)-1 == retsize) { 
             return -1;    
     } 

     iconv_close(cd); 
     
     return 0;
}
#endif





