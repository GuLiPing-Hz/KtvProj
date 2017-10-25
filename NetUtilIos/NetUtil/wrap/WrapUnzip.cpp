#include "WrapUnzip.h"
#include <string.h>

#ifdef WIN32
#define R_OK  4  /* Read */
#define W_OK  2  /* Write */
#define X_OK  1  /* Execute */
#define F_OK  0  /* Existence */
#include <io.h>
#include <direct.h>

#include <windows.h>
#else
//#include <strings.h>
#include <unistd.h>
//#include <ctype.h>
#include <time.h>
#include <utime.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#define WRITEBUFFERSIZE (8192)

bool CUnZip::unZipFile(const char* file,const char* outDir,const char* password)
{
	unzFile zfile = unzOpen(file);
	if(!zfile)
		return false;

	createDir(outDir);

	unz_global_info info;
	if ( unzGetGlobalInfo( zfile, &info ) != UNZ_OK )
		return false;

	if(unzGoToFirstFile( zfile ) != UNZ_OK)
		return false;

	int size_buf = WRITEBUFFERSIZE;
	void* buf = (void*)new char[size_buf];
	if (!buf)
		return false;

	int numError = 0;

	do
	{
		unz_file_info uzfileInfo = {0};
		char filenameInzip[260] = {0};
		if(unzGetCurrentFileInfo(zfile,&uzfileInfo,filenameInzip,sizeof(filenameInzip),NULL,0,NULL,0) !=UNZ_OK )
			return false;
		char* filenameWithoutPath = filenameInzip;
		char* p = filenameWithoutPath;
		while ((*p) != '\0')
		{
			if (((*p)=='/') || ((*p)=='\\'))
				filenameWithoutPath = p+1;
			p++;
		}

		if ((*filenameWithoutPath)=='\0')
		{
			char temDir[260] = {0};
			sprintf(temDir,"%s/%s",outDir,filenameInzip);
			createDir(temDir);
		}
		else
		{
			char writeFile[260] = {0};
			sprintf(writeFile,"%s/%s",outDir,filenameInzip);

			if(unzOpenCurrentFilePassword(zfile,password) != UNZ_OK)
				return false;

			FILE* fp = fopen(writeFile,"rb");
			if(fp)
			{
				fclose(fp);
				remove(writeFile);
			}

			FILE* fout = fopen(writeFile,"wb");
			if(!fout)
			{
				char pathDir[260] = {0};
				strcpy(pathDir,writeFile);
				char* p1 = strrchr(pathDir,'/');
				char* p2 = strrchr(pathDir,'\\');
				char* p = NULL;
				if(p1&&p2)
					p = p1>p2?p1:p2;
				else if(p1 == NULL)
					p = p2;
				else if(p2 == NULL)
					p = p1;
				if(p)
					*p = 0;
				createDir(pathDir);
				//再尝试打开
				fout = fopen(writeFile,"wb");
			}

			if(!fout)
				return false;

			int err;
			do
			{
				err = unzReadCurrentFile(zfile,buf,size_buf);
				if (err<0)
					break;

				if (err>0)
				{
					if (fwrite(buf,err,1,fout)!=1)
					{
						err=UNZ_ERRNO;
						break;
					}
				}
			}while (err>0);

			fclose(fout);

			if (err==0)
				changeFileDate(writeFile,uzfileInfo.dosDate,uzfileInfo.tmu_date);
			else
				numError ++;
		}

		unzCloseCurrentFile(zfile);

	}while (unzGoToNextFile(zfile) == UNZ_OK);


	if(buf)
		delete (char*)buf;

	unzClose( zfile );
	return !numError;
}

/* changeFileDate : change the date/time of a file
    filename : the filename of the file where date/time must be modified
    dosdate : the new date at the MSDos format (4 bytes)
    tmu_date : the SAME new date at the tm_unz format */
void CUnZip::changeFileDate(const char *filename,uLong dosdate,tm_unz tmu_date)
{
#ifdef WIN32
  HANDLE hFile;
  FILETIME ftm,ftLocal,ftCreate,ftLastAcc,ftLastWrite;

  hFile = CreateFileA(filename,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
  GetFileTime(hFile,&ftCreate,&ftLastAcc,&ftLastWrite);
  DosDateTimeToFileTime((WORD)(dosdate>>16),(WORD)dosdate,&ftLocal);
  LocalFileTimeToFileTime(&ftLocal,&ftm);
  SetFileTime(hFile,&ftm,&ftLastAcc,&ftm);
  CloseHandle(hFile);
#else
  struct utimbuf ut;
  struct tm newdate;
  newdate.tm_sec = tmu_date.tm_sec;
  newdate.tm_min=tmu_date.tm_min;
  newdate.tm_hour=tmu_date.tm_hour;
  newdate.tm_mday=tmu_date.tm_mday;
  newdate.tm_mon=tmu_date.tm_mon;
  if (tmu_date.tm_year > 1900)
      newdate.tm_year=tmu_date.tm_year - 1900;
  else
      newdate.tm_year=tmu_date.tm_year ;
  newdate.tm_isdst=-1;

  ut.actime=ut.modtime=mktime(&newdate);
  utime(filename,&ut);
#endif
}

void CUnZip::createDir(const char* pDir,bool isDir)
{
	char dirName[260] = {0};
	if(pDir)
		strcpy(dirName,pDir);
	else
		return ;

	for(int i=0;i<strlen(dirName);i++)
	{
		if(dirName[i] == '\\')
			dirName[i] = '/';
	}
	int len = strlen(dirName);
	if(isDir)
	{
		if(dirName[len-1] != '/')
		{
			dirName[len] = '/';
			len += 1;
		}
	}

	for(int i=1; i<len; i++)
	{
		if(dirName[i]=='/')
		{
			dirName[i] = 0;
#ifdef WIN32
			if(_access(dirName,F_OK))
			{
				_mkdir(dirName);
			}
#else
			if(  access(dirName,F_OK)!=0   )
			{
				if(mkdir(dirName, 0755)==-1)
					return;
			}
#endif
			dirName[i] = '/';
		}
	}
}
