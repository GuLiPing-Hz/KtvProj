/*
	注释时间:2014-4-25
	author: glp
	解压zip文件类的封装,提供类函数，方便使用
*/
#ifndef UNZIP__H__
#define UNZIP__H__

#include "../zip/unzip.h"


class CUnZip
{
private:
	CUnZip();
	virtual ~CUnZip();

public:
	//解压文件
	/*
	默认采用覆盖的方式解压，目录结构会按照zip包中的目录结构解压到指定文件夹下

	@param file : 指定的zip文件名，全路径文件名，或者相对路径
	@param outDir：指定zip文件解压的目录
	@param password：指定zip解压的密码，无可以使用NULL
	*/
	static bool unZipFile(const char* file,const char* outDir,const char* password);
	//创建目录
	/*
	@param dirName：指定创建的目录，isDir是true的时候，如果最后的字符串不是'\\'或者'/'，会自动添加一个'/'，然后逐层创建目录
	isDir是false的时候不会自动添加'/'字符，也会逐层创建目录
	@param isDir：是否是文件名
	*/
	static void createDir(const char* pDir,bool isDir=true);
	//修改文件修改时间
	/*
	@param filename：指定需要修改时间的文件，需要包含路径
	@param dosdate：指定的时间 windows
	@param tmu_date：指定的时间 linux
	*/
	static void changeFileDate(const char *filename,uLong dosdate,tm_unz tmu_date);

};

#endif // UNZIP__H__
