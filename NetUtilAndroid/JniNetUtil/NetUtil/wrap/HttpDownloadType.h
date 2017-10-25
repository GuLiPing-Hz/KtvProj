//
//  DownloadType.h
//  NetUtil
//

#ifndef NetUtil_DownloadType_h
#define NetUtil_DownloadType_h

//@param f ：进度0.0f-1.0f
//@param speed：速度，（bytes/millisecond）;
typedef void (* ONPROGRESS)(float f,double speed);
#define CHAR_LEN 260
    
typedef struct _DownloadInfo
{
    bool				download;				//是否下载到文件，false是保存到内存
    //download 为true的时候使用下面的属性
    char				fileName[CHAR_LEN];		// 文件名(包含路径)（必须指定）
    char				unzipDir[CHAR_LEN];		//如果是.zip，文件，可指定解压目录，如果不指定，则使用zip所在的目录解压
    char				tmpFileName[CHAR_LEN];	// 临时文件名（自动填充）
    long				fileSize;				// 文件大小（自动填充）
    long				tmpFileSize;			// 临时文件大小（自动填充）
    char				MD5[CHAR_LEN];			// 文件MD5值（可选指定）
    bool				mandatory;				// 是否强制重新下载文件（必须指定）
    //download为false的时候使用下面的属性,把内容保存到内存中
    unsigned char*			saveBuf;				//保存内容,外部需要自己维护此块内存，Http请求获取的内容会存放到这里。外部分配内存必须用malloc方法
    unsigned char*			saveTmpBuf;             //用于异步保存内容，
    int				saveBufLen;             //内存大小，内部最后会修改成实际大小
}DownloadInfo;

//4K
#define MAX_URL_LEN 4096
typedef struct _TaskDownload
{
    char			url[MAX_URL_LEN];//下载的Url
    DownloadInfo	info;//下载任务信息
    ONPROGRESS      onProgress;//进度回调函数
    typedef void (* ONFINISH)(_TaskDownload* downloadInfo,bool success,void* userData);
    ONFINISH        onFinish;//结果返回函数，
    void*           userData;//用户数据用于在OnFinish的时候传递
}TaskDownload;

#endif//NetUtil_DownloadType_h
