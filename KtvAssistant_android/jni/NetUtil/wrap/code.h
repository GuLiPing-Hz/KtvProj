/*

	by glp
	字符编码转换，
	只封装了GBK UTF-8之间的转换
	返回0 成功
	返回负数 失败
*/
#ifndef CHAR__CODE__H__
#define CHAR__CODE__H__

int GBKToUTF8(/*out*/char* dst,/*out*/int* len_out,/*in*/const char * src, /*in*/int len_in);
int UTF8ToGBK(/*out*/char* dst,/*out*/int* len_out,/*in*/const char * src, /*in*/int len_in);

#endif//CHAR__CODE__H__

