// TestXHDll.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "../XHSMDll/XHHead.h"
#include "../XHSMDll/XHSMDllHelp.h"
#include "sm/processcommunicate.h"
#include "sm/SMHelp.h"
#include <string>

// CTestXHSMDllDlg 消息处理程序

CSMHelp* g_pHelp = NULL;

void MessageGlp(int cmd,void* pData)
{
	switch(cmd)
	{
	case XHH::CMD_HOST_SONG:
		{
			//OutputDebugStringA("MessageGlp CMD_HOST_SONG\n");
			//break;
			XHH::DoubleString* p = (XHH::DoubleString*)pData;
			std::string strMdm = p->str1;
			if (strMdm.size() == 0)
			{
				break;
			}
			int info = ((30<<24)|(1<<23)|(2<<19)|(3<<15)|(4<<11)|(0<<10)|(0<<9)|(16<<1)|1);
			//视频信息
			g_pHelp->PostVideoInfo(1,0,176,144,3,0);
			g_pHelp->PostVideoInfo(1,1,176,144,3,0);

			std::string::size_type b = strMdm.rfind(".dat");
			strMdm = strMdm.substr(0,b);
			std::string::size_type f = strMdm.rfind("=●=");
			std::string singer = strMdm.substr(f+3);
			strMdm = strMdm.substr(0,f);
			f = strMdm.rfind("=●=");
			std::string song = strMdm.substr(f);
			//发送歌曲信息
			g_pHelp->PostSongInfo(singer.c_str(),song.c_str());
			//起始结束时间
			//m_psmHelp->PostSongStartEnd(100,120);
			//m_psmHelp->PostNextSong("ktv2");
			g_pHelp->PostFromOther(true,false);
			g_pHelp->PostSwitchSong("D:\\glp\\work\\SVN\\ktv play\\mpg\\966338=●=青花瓷=●=周杰伦.mpg",p->str1,info);
			break;
		}
	case XHH::CMD_HOST_GRADE:
		{
			//OutputDebugStringA("MessageGlp CMD_HOST_GRADE\n");
			//break;
			XHH::OtherGrade* p = (XHH::OtherGrade*)pData;
			g_pHelp->PostOhterGrade(p->cur_tm,p->cur_pitch,p->sentence_index,p->realtime_score,p->accumulate_score,p->cur_db
				,p->sentence_score,p->sentence_switch);
			break;
		}
	case XHH::CMD_HOST_VIDEO:
		{
			//OutputDebugStringA("MessageGlp CMD_HOST_VIDEO\n");
			XHH::VideoInfo* p = (XHH::VideoInfo*)pData;
			g_pHelp->PostVideoInfo(p->operation,p->type,p->width,p->height,p->bytes_pixel,p->size);
			break;
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{

	CXHSMDllHelp::InitSMHelp(&MessageGlp);
	CXHSMDllHelp* pHelp = new CXHSMDllHelp("D:\\glp\\work\\SVN\\super\\branches\\bin\\release","xingguang.exe");
	g_pHelp = new CSMHelp("D:\\glp\\work\\SVN\\ktv play\\src\\DXDPdemo\\debug","player.exe");
	while(1)
	{
		Sleep(1);
	}
	if(pHelp)
	{
		delete pHelp;
	}
	return 0;
}

