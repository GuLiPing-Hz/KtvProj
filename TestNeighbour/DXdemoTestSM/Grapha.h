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
class CGraph//���ڹ���KTV mv�Ĳ��Ű�����Ƶ����Ƶ��graph
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

	/////////////////////////-----------0,�ɹ���-1ʧ��------------//////////////////////////////////
	//���ſ���
	int startPlayer(bool bFirstAudio/*long nIndex*/);

	int stopPlayer();

	int pausePlayer();

	int resumePlayer();
	//���õ�ǰλ��(����)
	int setCurPosition(ulong nposition_ms);
	//������ʼ����λ��(����)
	int setStartStopPosition(ulong nstart_ms,ulong nstop_ms);
	//��ȡ��ǰλ��(����)
	int getPosition(ulong &nposition_ms);
	//��ȡý���ܳ���:����
	int getDuration(ulong &nduration_ms);
//////////////////////////////////////////////////////////////////////////
	//�л�������һ��
	void switchAudio();
	//���������л�����
	void switchAudioEx(bool bFirstAudio);
	//�ı�����
	void changeCurPitch(const int nPitch);
	//������Ϣ
	HRESULT getGraphEvent(LONG& evCode,LONG& evParam1,LONG evParam2);
	HRESULT freeGraphEvent(LONG evCode,LONG evParam1,LONG evParam2);
	//�������
	int getVolume(long &lVolume);
	//�������� -10000��0֮��
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
	// ��ʼ������
	void initAudioTrack(long nIndex);
};
