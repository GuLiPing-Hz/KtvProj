#pragma once
#include <strmif.h>

// DirectShow Header Files
#include <dshow.h>
#include <d3d9.h>
#include <vmr9.h>

#include "vmrutil.h"
#include <winnt.h>
#include "Opengl.h"
#include <InitGuid.h>
#include "OS_AudioSwitch.h"
#include "OS_ChangePitch.h"

class CRenderEngine;
class CGraph//用于管理KTV mv的播放包含视频，音频的graph
{
private:
	CGraph(void);
	virtual ~CGraph(void);
	
	static CGraph *s_graph;

public:
	static CGraph*		getSingleton();
	static void			releaseGraphInstance();

	HRESULT	gStartGraph(HWND window,const wchar_t * fileName,const CGSize &winsize,CRenderEngine* pallocator,long nIndex);

	HRESULT gCloseGraph(HWND hWnd = NULL);
	void errorUninit();

	/////////////////////////-----------0,成功；-1失败------------//////////////////////////////////
	//播放控制
	int startPlayer(bool bFirstAudio/*long nIndex*/);

	int stopPlayer();

	int pausePlayer();

	int resumePlayer();
	//设置当前位置(毫秒)
	int setCurPosition(ulong nposition_ms);
	//设置起始结束位置(毫秒)
	int setStartStopPosition(ulong nstart_ms,ulong nstop_ms);
	//获取当前位置(毫秒)
	int getPosition(ulong &nposition_ms);
	//获取媒体总长度:毫秒
	int getDuration(ulong &nduration_ms);
//////////////////////////////////////////////////////////////////////////
	//切换音轨会顿一下
	void switchAudio();
	//改善音轨切换体验
	void switchAudioEx(bool bFirstAudio);
	//改变音调
	void changeCurPitch(const int nPitch);
	//播放消息
	HRESULT getGraphEvent(LONG& evCode,LONG& evParam1,LONG evParam2);
	HRESULT freeGraphEvent(LONG evCode,LONG evParam1,LONG evParam2);
	//获得音量
	int getVolume(long &lVolume);
	//设置音量 -10000到0之间
	int setVolume(const long lVolume);
private:
	//HRESULT SetAllocatorPresenter( IBaseFilter* filter, HWND window,CAllocator* pallocator );
	HRESULT setAllocatorPresenter( IBaseFilter* filter, const CGSize &winsize,CRenderEngine* pallocator );
	HRESULT initGraph(const wchar_t* filename,const CGSize &winsize,CRenderEngine* pallocator);
	void			uinitGraph();

private:
	ICaptureGraphBuilder2*	m_pBuilder;
	IGraphBuilder*					m_pGraph;

	IBaseFilter* 						m_pSourceFilter;
	IBaseFilter* 						m_pDemultiplexer ;
	IBaseFilter*						m_pAvisplitter;
	IBaseFilter* 						m_pFFdsVDecoder ;
	IBaseFilter* 						m_pFFdsADecoder1 ;
	IBaseFilter* 						m_pFFdsADecoder2 ;
	IBaseFilter* 						m_pAudioSwitcher ;
	IBaseFilter* 						m_pAudioRender;
	IBaseFilter*						m_pVMR9;//Video Mixing Render

	IFileSourceFilter*							m_pFileSourceFilter;
	IBasicAudio	*								m_pBasicAudio;
	IMediaControl*							m_pMediaControl;
	IMediaSeeking*							m_pMediaSeeking;
	IMediaEventEx*							m_pMediaEventEx;
	IAMStreamSelect*						m_pStreamSelect;
	IOS_AudioSwitch*						m_pOSAudioSwitch;
	IOS_ChangePitch*						m_pOSChangePitch;
	IVMRSurfaceAllocator9*				m_pAllocator;//
	//IVMRSurfaceAllocatorNotify9* m_lpIVMRSurfAllocNotify;

	bool												m_nStream;
	bool												m_bUseSameFilter;
	bool												m_binitGraph;
	HWND											m_hWnd;

public:
	// 初始化音轨
	void initAudioTrack(long nIndex);
};
