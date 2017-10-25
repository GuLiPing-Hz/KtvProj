/*
 * HttpDownloadMgr.h
 *
 *  Created on: 2014-5-23
 *   CHttpDownloadMgr封装一个下载任务队列，以及管理一个下载Tcp线程
 *   维护DownloadInfo中saveBuf的内存，用户无需自己申请内存。
 */

#ifndef HTTPDOWNLOADMGR_H_
#define HTTPDOWNLOADMGR_H_

#include "HttpDownload.h"
#include "reactor.h"
#include "thread_wrapper.h"
#include <list>
#include "NUtility.h"


namespace NetworkUtil {
//只同时下载一个任务
#define MAX_DOWNLOAD 1

typedef std::list<TaskDownload> LISTTASK;
typedef std::set<std::string> SETURL;


class CHttpDownloadMgr
{
public:
	CHttpDownloadMgr();
	virtual ~CHttpDownloadMgr();

	static bool HttpDownload(ThreadObj pParam);
private:
	//-1 无空闲，其他为空闲可用的ID数组索引
	int checkIdle();
public:
	bool initDownload();
	/*
     增加一个Http任务
     @param task[in]：结构中可以指定进度回调函数
        可以指定完成回调函数，判断是否下载成功
        DownloadInfo 中需要指定fileName（绝对路径或相对路径），mandatory，如果有MD5也可以指定，文件大小在内部会填充进去，
        临时文件名也会根据一定的规则生成。
     @param priority[in]：是否优先下载

     @return：0，添加成功；-1，添加失败;-2，文件已经存在，不需要下载;-3,URL请求为空
     
     @注1：特别提醒，TaskDownload.DownloadInfo中的saveBuf，函数会判断如果不是下载到文件，则会自己申请内存空间
     ，并在finish的时候会释放内存，用户无需自己申请内存，只要使用即可。
     
     @注2:对于非强制下载的任务，如果发现要求下载的文件与传递的MD5(非“”)相一致的话，则会调用到OnFinish函数,认为下载完成
	*/
	int addTask(TaskDownload& task,bool priority=false);
	void doNext(bool success,CHttpDownload* worker);
private:
	ThreadWrapper*			m_pThread;
	LISTTASK						m_listTask;
	SETURL 						m_setUrl;
	TaskDownload			m_gCurTask;

	CHttpDownload*               m_gHttpDownload[MAX_DOWNLOAD];
	CriticalSectionWrapper*   m_pCS;
public:
	NetReactor						    m_gReactor;
};

} /* namespace NetworkUtil */
#endif /* HTTPDOWNLOADMGR_H_ */
