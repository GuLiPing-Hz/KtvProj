#include "stdafx.h"
#include "Grapha.h"
#include "util.h"
// #include "RenderEngine.h"


static DWORD_PTR        g_MyId = 0xACDCACDC;

#ifdef _GWINXP
int g_bCountToCreateListener = 0;
bool g_bNeedToReleaseListener = false;
#endif

CGraph* CGraph::s_graph = NULL;

CGraph*	CGraph::getSingleton()
{
	if (!s_graph)
	{
		s_graph = new CGraph;
	}
	return s_graph;
}

void	CGraph::releaseGraphInstance()
{
	if (s_graph)
	{
		delete s_graph;
		s_graph = NULL;
	}
}

CGraph::CGraph(void)
:m_pBuilder(NULL)
,m_pGraph(NULL)
,m_pSourceFilter(NULL)
,m_pDemultiplexer(NULL)
,m_pFFdsVDecoder(NULL)
,m_pFFdsADecoder1(NULL)
,m_pFFdsADecoder2(NULL)
,m_pAudioSwitcher(NULL)
,m_pAudioRender(NULL)
,m_pVMR9(NULL)
,m_pFileSourceFilter(NULL)
,m_pBasicAudio(NULL)
,m_pMediaControl(NULL)
,m_pMediaSeeking(NULL)
,m_pMediaEventEx(NULL)
,m_pStreamSelect(NULL)
,m_pOSAudioSwitch(NULL)
,m_pOSChangePitch(NULL)
,m_pAllocator(NULL)
,m_nStream(true)
// ,m_lpIVMRSurfAllocNotify(NULL)
,m_hWnd(NULL)
,m_binitGraph(false)
,m_bUseSameFilter(false)
,m_pAvisplitter(NULL)
{
	
}

CGraph::~CGraph()
{
	errorUninit();
}

void CGraph::errorUninit()
{
	m_bUseSameFilter = false;//彻底释放
	SAFE_RELEASE(m_pVMR9);
	//SAFE_RELEASE(m_pAllocator);
	gCloseGraph();
}

void CGraph::uinitGraph()
{
	RemoveAllFilter(m_pGraph);
	SAFE_RELEASE(m_pFileSourceFilter);
	SAFE_RELEASE(m_pSourceFilter);

	
	if (!m_bUseSameFilter)
	{
		SAFE_RELEASE(m_pBasicAudio);
		SAFE_RELEASE(m_pMediaSeeking);
		SAFE_RELEASE(m_pMediaControl);
		if (m_pMediaEventEx)
		{
			m_pMediaEventEx->SetNotifyWindow(NULL,0,0);
		}
		SAFE_RELEASE(m_pMediaEventEx);

		SAFE_RELEASE(m_pGraph);
		SAFE_RELEASE(m_pBuilder);

		SAFE_RELEASE(m_pStreamSelect);
		SAFE_RELEASE(m_pOSAudioSwitch);
		SAFE_RELEASE(m_pOSChangePitch);

		//SAFE_RELEASE(m_pVMR9);
		if (m_pAllocator)//
		{
			ulong nCount;
			nCount = m_pAllocator->Release();
			m_pAllocator = NULL;
		}
		SAFE_RELEASE(m_pDemultiplexer);
		SAFE_RELEASE(m_pAvisplitter);
		SAFE_RELEASE(m_pFFdsVDecoder);
		SAFE_RELEASE(m_pFFdsADecoder1);
		SAFE_RELEASE(m_pFFdsADecoder2);
		SAFE_RELEASE(m_pAudioSwitcher);
		SAFE_RELEASE(m_pAudioRender);
	// 	if (m_pgraph)
	// 	{
	// 		m_pgraph->Release();
	// 		m_pgraph = NULL;
	// 	}
	}
}

HRESULT CGraph::initGraph(const wchar_t* filename,const CGSize &winsize,CRenderEngine* pallocator)
{
	_bstr_t path = filename;//"F:\\work\\video\\Data\\mpg\\950052=●=950052=●=突然的自我=●=伍佰.mpg";
	//w riteLog("[DXLOG CGraph::initGraph] %s",filename);
	if( ! path.length() )
	{
		LOG_ERN0("path error");
		return E_INVALIDARG;
	}
	bool bAvi = false;
	if (wcsstr(filename,L".avi"))
	{
		bAvi = true;
	}

	HRESULT hr;

	if (!m_binitGraph)
	{ 
		//hr = CoCreateInstance(CLSID_CaptureGraphBuilder2,NULL,CLSCTX_INPROC_SERVER,IID_ICaptureGraphBuilder2,(void**)&m_pBuilder);
		hr = CoCreateInstance(CLSID_FilterGraph,NULL,/*CLSCTX_ALL*/CLSCTX_INPROC_SERVER,IID_IGraphBuilder,(void**)&m_pGraph);
		//The Filter Graph Manager is provided by an in-process DLL, so the execution context is CLSCTX_INPROC_SERVER.
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] : CLSID_FilterGraph error :%x",hr);
			LOG_ERS2("CLSID_FilterGraph HRESULT",hr);
			return hr;
		}
		//设置图表
		//hr = m_pBuilder->SetFiltergraph(m_pgraph);
		hr = m_pGraph->QueryInterface(IID_IBasicAudio,(void**)&m_pBasicAudio);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] : IID_IBasicAudio error :%x",hr);
			LOG_ERS2("IID_IBasicAudio HRESULT",hr);
			return hr;
		}
		hr = m_pGraph->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] : IID_IMediaControl error :%x",hr);
			LOG_ERS2("IID_IMediaControl HRESULT",hr);
			return hr;
		}
		hr = m_pGraph->QueryInterface(IID_IMediaEventEx,(void**)&m_pMediaEventEx);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] : IID_IMediaEventEx error :%x",hr);
			LOG_ERS2("IID_IMediaEventEx HRESULT",hr);
			return hr;
		}
		hr = m_pGraph->QueryInterface(IID_IMediaSeeking,(void**)&m_pMediaSeeking);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] : IID_IMediaSeeking error :%x",hr);
			LOG_ERS2("IID_IMediaSeeking HRESULT",hr);
			return hr;
		}

		if (m_pMediaSeeking)
		{
			FAIL_RET(m_pMediaSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME));
		}
		if (m_pMediaEventEx)
		{
			FAIL_RET(m_pMediaEventEx->SetNotifyWindow((OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0));
		}
// 		hr = CoCreateInstance(CLSID_FILESourceAsync, NULL, CLSCTX_INPROC_SERVER, __uuidof(IBaseFilter), (void**)&m_pSourceFilter);
// 		hr = m_pSourceFilter->QueryInterface(IID_IFileSourceFilter,(void**)&m_pIfileSourceFilter);
		//w riteLog("2");
		hr = CoCreateInstance(CLSID_MPEG_Demultiplexer, NULL, CLSCTX_INPROC_SERVER, __uuidof(IBaseFilter), (void**)&m_pDemultiplexer);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] : CLSID_MPEG_Demultiplexer error :%x",hr);
			LOG_ERS2("CLSID_MPEG_Demultiplexer HRESULT",hr);
			return hr;
		}
		//w riteLog("3");
		hr = CoCreateInstance(CLSID_AVI_Demultiplexer,NULL,CLSCTX_INPROC_SERVER,__uuidof(IBaseFilter), (void**)&m_pAvisplitter);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] :  error :%x",hr);
			LOG_ERS2("CLSID_AVI_Demultiplexer HRESULT",hr);
			return hr;
		}
		//w riteLog("4");
		hr = CoCreateInstance(CLSID_FFDshowVideoDecoder, NULL, CLSCTX_INPROC_SERVER, __uuidof(IBaseFilter), (void**)&m_pFFdsVDecoder);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] : CLSID_FFDshowVideoDecoder error :%x",hr);
			LOG_ERS2("CLSID_FFDshowVideoDecoder HRESULT",hr);
			return hr;
		}
		//w riteLog("5");
		hr = CoCreateInstance(CLSID_FFDShowAudioDecoder, NULL, CLSCTX_INPROC_SERVER, __uuidof(IBaseFilter), (void**)&m_pFFdsADecoder1);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] : CLSID_FFDShowAudioDecoder error :%x",hr);
			LOG_ERS2("CLSID_FFDShowAudioDecoder1 HRESULT",hr);
			return hr;
		}
		//w riteLog("6");
		hr = CoCreateInstance(CLSID_FFDShowAudioDecoder, NULL, CLSCTX_INPROC_SERVER, __uuidof(IBaseFilter), (void**)&m_pFFdsADecoder2);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] :  error :%x",hr);
			LOG_ERS2("CLSID_FFDShowAudioDecoder2 HRESULT",hr);
			return hr;
		}
		//w riteLog("7");
		hr = CoCreateInstance(CLSID_GL_AudioSwitcher, NULL, CLSCTX_INPROC_SERVER, __uuidof(IBaseFilter), (void**)&m_pAudioSwitcher);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] : CLSID_GL_AudioSwitcher error :%x",hr);
			LOG_ERS2("CLSID_GL_AudioSwitcher HRESULT",hr);
			return hr;
		}
		hr = m_pAudioSwitcher->QueryInterface(IID_IAMStreamSelect,(void**)&m_pStreamSelect);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] :  error :%x",hr);
			LOG_ERS2("IID_IAMStreamSelect HRESULT",hr);
			return hr;
		}
		hr = m_pAudioSwitcher->QueryInterface(IID_IOS_AudioSwitch,(void**)&m_pOSAudioSwitch);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] : IID_IOS_AudioSwitch error :%x",hr);
			LOG_ERS2("IID_IOS_AudioSwitch HRESULT",hr);
			return hr;
		}
		hr = m_pAudioSwitcher->QueryInterface(IID_IOS_ChangePitch,(void**)&m_pOSChangePitch);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] : IID_IOS_ChangePitch error :%x",hr);
			LOG_ERS2("IID_IOS_ChangePitch HRESULT",hr);
			return hr;
		}
		//w riteLog("8");
		hr = CoCreateInstance(CLSID_DSoundRender, NULL, CLSCTX_INPROC_SERVER, __uuidof(IBaseFilter), (void**)&m_pAudioRender);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] : CLSID_DSoundRender error :%x",hr);
			LOG_ERS2("CLSID_DSoundRender HRESULT",hr);
			return hr;
		}
		//w riteLog("9");
		if (m_bUseSameFilter)//只创建一次filter
		{
			m_binitGraph = true;
		}
 	}

	hr = m_pGraph->AddSourceFilter(path,L"File Source (Async.)",&m_pSourceFilter);
	if (FAILED(hr))
	{
		//w riteLog("[DXLOG CGraph::initGraph] : SourceFileter add error :%x",hr);
		LOG_ERS2("SourceFileter Add HRESULT",hr);
		return hr;
	}
	//w riteLog("ax File Source (Async.) pass");
	if (bAvi)
	{
		FAIL_RET(m_pGraph->AddFilter(m_pAvisplitter,L"AVI Splitter"));
		//w riteLog("10");
	}
	else
	{
		FAIL_RET(m_pGraph->AddFilter(m_pDemultiplexer,L"MPEG Demultiplexer"));
	}
	
	FAIL_RET(m_pGraph->AddFilter(m_pFFdsVDecoder,L"FFDS Video Decoder"));
	FAIL_RET(m_pGraph->AddFilter(m_pFFdsADecoder1,L"FFDS Audio Decoder1"));
	FAIL_RET(m_pGraph->AddFilter(m_pFFdsADecoder2,L"FFDS Audio Decoder2"));//先提供单音轨的demo
	FAIL_RET(m_pGraph->AddFilter(m_pAudioSwitcher,L"Audio Switcher"));
	FAIL_RET(m_pGraph->AddFilter(m_pAudioRender,L"DSound Render"));
	//w riteLog("11");
	if (!m_pVMR9)
	{
		hr = CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER, __uuidof(IBaseFilter), (void**)&m_pVMR9);
		if (FAILED(hr))
		{
			//w riteLog("[DXLOG CGraph::initGraph] : CLSID_VideoMixingRenderer9 error :%x",hr);
			LOG_ERS2("CLSID_VideoMixingRenderer9 HRESULT",hr);
			return hr;
		}

// 		IVMRFilterConfig9 *filterConfig = NULL;
// 		FAIL_RET( m_pVMR9->QueryInterface(IID_IVMRFilterConfig9, (void**)&filterConfig) );
// 
// 		FAIL_RET( filterConfig->SetRenderingMode( VMR9Mode_Renderless  ) );//VMR9Mode_Windowless
// 
// 		//FAIL_RET( filterConfig->SetNumberOfStreams(1) );
// 		filterConfig->Release();
	}
	///m_pIfileSourceFilter->Load(path,NULL);

	//hr = m_pgraph->AddFilter(m_pSourceFilter,L"File Source (Async.)");
// 	if (FAILED(hr = setAllocatorPresenter( m_pVMR9, winsize,pallocator )))
// 	{
// 		LOG_ERS2("setAllocatorPresenter HRESULT",hr);
// 		m_pAllocator = NULL;
// 		return hr;
// 	}
	if( FAILED(hr = m_pGraph->AddFilter(m_pVMR9, L"Video Mixing Renderer 9")) )
	{
		m_pAllocator = NULL;
		return hr;
	}

// 	hr = m_pGraph->RenderFile( path, NULL);
// 	return hr;
//	}
	//w riteLog("13");
	//FAIL_RET( SetAllocatorPresenter( m_pVMR9, winsize,pallocator ) );//+1

//		hr = m_pIfileSourceFilter->Load(path,NULL);
// 	REFERENCE_TIME nStart = 0;
// 	hr =m_pmediaSeeking->SetPositions(&nStart,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
	//FAIL_RET( SetAllocatorPresenter( m_pVMR9, winsize,pallocator ) );//+1

	if (bAvi)
	{
		if( FAILED(hr = TryConnectFilter(m_pGraph,m_pSourceFilter,m_pAvisplitter)))
		{
			LOG_ERS2("connect fileter m_pSourceFilter m_pAvisplitter HRESULT",hr);
			m_pAllocator = NULL;
			return hr;
		}
		if( FAILED(hr = TryConnectFilter(m_pGraph,m_pAvisplitter,m_pFFdsVDecoder)))
		{
			LOG_ERS2("connect fileter m_pAvisplitter m_pFFdsVDecoder HRESULT",hr);
			m_pAllocator = NULL;
			return hr;
		}
		if( FAILED(hr = TryConnectFilter(m_pGraph,m_pAvisplitter,m_pFFdsADecoder1)))
		{
			LOG_ERS2("connect fileter m_pAvisplitter m_pFFdsADecoder1 HRESULT",hr);
			m_pAllocator = NULL;
			return hr;
		}
		if( FAILED(hr = TryConnectFilter(m_pGraph,m_pAvisplitter,m_pFFdsADecoder2)))
		{
			LOG_ERS2("connect fileter m_pAvisplitter m_pFFdsADecoder2 HRESULT",hr);
			m_pAllocator = NULL;
			return hr;
		}
		//w riteLog("[DXLOG CGraph::initGraph] : avi connect well");
	}
	else
	{
		if( FAILED(hr = TryConnectFilter(m_pGraph,m_pSourceFilter,m_pDemultiplexer)))
		{
			LOG_ERS2("connect fileter m_pSourceFilter m_pDemultiplexer HRESULT",hr);
			m_pAllocator = NULL;
			return hr;
		}
		if( FAILED(hr = TryConnectFilter(m_pGraph,m_pDemultiplexer,m_pFFdsVDecoder)))
		{
			LOG_ERS2("connect fileter m_pDemultiplexer m_pFFdsVDecoder HRESULT",hr);
			m_pAllocator = NULL;
			return hr;
		}
		if( FAILED(hr = TryConnectFilter(m_pGraph,m_pDemultiplexer,m_pFFdsADecoder1)))
		{
			LOG_ERS2("connect fileter m_pDemultiplexer m_pFFdsADecoder1 HRESULT",hr);
			m_pAllocator = NULL;
			return hr;
		}
		if( FAILED(hr = TryConnectFilter(m_pGraph,m_pDemultiplexer,m_pFFdsADecoder2)))
		{
			LOG_ERS2("connect fileter m_pDemultiplexer m_pFFdsADecoder2 HRESULT",hr);
			m_pAllocator = NULL;
			return hr;
		}
		//w riteLog("[DXLOG CGraph::initGraph] : mpg connect well");
	}

	if (FAILED(hr = TryConnectFilter(m_pGraph,m_pFFdsVDecoder,m_pVMR9)))
	{
		LOG_ERS2("connect fileter m_pFFdsVDecoder m_pVMR9 HRESULT",hr);
		m_pAllocator = NULL;
		return hr;
	}

	if( FAILED(hr = TryConnectFilter(m_pGraph,m_pFFdsADecoder1,m_pAudioSwitcher)))
	{
		LOG_ERS2("connect fileter m_pFFdsADecoder1 m_pAudioSwitcher HRESULT",hr);
		m_pAllocator = NULL;
		return hr;
	}
	if( FAILED(hr = TryConnectFilter(m_pGraph,m_pFFdsADecoder2,m_pAudioSwitcher)))
	{
		LOG_ERS2("connect fileter m_pFFdsADecoder2 m_pAudioSwitcher! The song may have only one audio line,HRESULT",hr);
		m_pAllocator = NULL;
		return hr;
	}
	if( FAILED(hr = TryConnectFilter(m_pGraph,m_pAudioSwitcher,m_pAudioRender)))
	{
		LOG_ERS2("connect fileter m_pAudioSwitcher m_pAudioRender HRESULT",hr);
		m_pAllocator = NULL;
		return hr;
	}
	return hr;
}


HRESULT CGraph::gStartGraph(HWND window,const wchar_t * fileName,const CGSize &winsize,CRenderEngine* pallocator,long nIndex)
{
	/*gCloseGraph(m_hWnd);*/
	m_hWnd = window;

	HRESULT hr = E_FAIL;

	hr = initGraph(fileName,winsize,pallocator);

	UNREFERENCED_PARAMETER(nIndex);
	//////////////////////////////////////////////////////////////////////////
	return hr;
}

HRESULT CGraph::gCloseGraph(HWND hWnd)
{//433
	if( m_pMediaControl != NULL ) 
	{
		OAFilterState state;
		do {//1427 26-8
			m_pMediaControl->Stop();
			//1427-89
			m_pMediaControl->GetState(0, & state );
		} while( state != State_Stopped ) ;
	}

#ifdef _GWINXP
	if (g_bNeedToReleaseListener)
	{
#endif
		//SAFE_RELEASE(m_pallocator);
		if (m_pAllocator)
		{
// 			ulong nCount = m_pAllocator->Release();
// 			w riteLog("Allocator count : %d",nCount);
		}
#ifdef _GWINXP
		g_bNeedToReleaseListener = false;
	}
#endif
	//SAFE_RELEASE(m_pallocator);只有一个allocator
	uinitGraph();
	return S_OK;
}

HRESULT CGraph::setAllocatorPresenter( IBaseFilter* filter, const CGSize &winsize,CRenderEngine* pallocator )
{
// 	if( filter == NULL )
// 	{
// 		return E_FAIL;
// 	}
// 
// 	HRESULT hr;
// 
// 	//CComPtr<IVMRSurfaceAllocatorNotify9> lpIVMRSurfAllocNotify;
// 	IVMRSurfaceAllocatorNotify9*	 lpIVMRSurfAllocNotify = NULL;
// 	FAIL_RET( filter->QueryInterface(IID_IVMRSurfaceAllocatorNotify9, reinterpret_cast<void**>(&lpIVMRSurfAllocNotify)) );//9
// 
// 	// create our surface allocator
// 	m_pAllocator = pallocator;
// 
// 	// let the allocator and the notify know about each other206
// 	hr = m_pAllocator->AdviseNotify(lpIVMRSurfAllocNotify) ;
// 	if (FAILED(hr))
// 	{
// 		LOG_ERN0("AdviseNotify failed");
// 		goto failed;
// 	}
// 	hr = lpIVMRSurfAllocNotify->AdviseSurfaceAllocator( g_MyId, m_pAllocator );
// 	if (FAILED(hr))
// 	{
// 		LOG_ERN0("AdviseSurfaceAllocator failed");
// 	}
// 
// failed:
// 	lpIVMRSurfAllocNotify->Release();
// 	return hr;
	return S_OK;
}

void CGraph::switchAudioEx(bool bFirstAudio)
{
	if (m_pOSAudioSwitch)
	{
		m_pOSAudioSwitch->SwitchAudio(bFirstAudio);
	}
}

void CGraph::changeCurPitch(const int nPitch)
{
	int tmp_nPitch = nPitch;
	tmp_nPitch = tmp_nPitch<-5?-5:(tmp_nPitch>5?5:tmp_nPitch);
	if (m_pOSChangePitch)
	{
		m_pOSChangePitch->ChangeCurPitch(tmp_nPitch);
	}
}

void CGraph::switchAudio()
{
	if (m_pStreamSelect)
	{
		DWORD nStreams=1;
		m_pStreamSelect->Count(&nStreams);
		if (nStreams>1)
		{
			if (m_nStream)
			{
				m_pStreamSelect->Enable(1,AMSTREAMSELECTENABLE_ENABLE);//
				m_nStream = false;
			}
			else
			{
				m_pStreamSelect->Enable(0,AMSTREAMSELECTENABLE_ENABLE);//0
				m_nStream = true;
			}

		}
	}
}


int CGraph::startPlayer(bool bFirstAudio/*long nIndex*/)
{
	if (m_pMediaControl)
	{
		if (FAILED(m_pMediaControl->Run()))
		{
			return -1;
		}
		//initAudioTrack(nIndex);
		switchAudioEx(bFirstAudio);
	}
	return 0;
}

int CGraph::stopPlayer()
{
	if( m_pMediaControl ) 
	{
		OAFilterState state;
		do {
			m_pMediaControl->Stop();
			m_pMediaControl->GetState(0, & state );
		} while( state != State_Stopped ) ;
	}
	return 0;
}

int CGraph::pausePlayer()
{
	if (m_pMediaControl)
	{
		OAFilterState state;
		m_pMediaControl->GetState(0,&state);
		if(state == State_Running)
		{
			if (FAILED(m_pMediaControl->Pause()))
			{
				return -1;
			}
		}
	}
	return 0;
}

int CGraph::resumePlayer()
{
	if (m_pMediaControl)
	{
		OAFilterState state;
		m_pMediaControl->GetState(0,&state);
		if (state == State_Paused)
		{
			if (FAILED(m_pMediaControl->Run()))
			{
				return -1;
			}
		}
	}
	return 0;
}

//设置当前位置(毫秒)
int CGraph::setCurPosition(ulong nposition_ms)
{
	if (m_pMediaSeeking)
	{
		LONGLONG nanosecond_unit = nposition_ms*10000;
		if (FAILED(m_pMediaSeeking->SetPositions(&nanosecond_unit, AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame, 
			0, AM_SEEKING_NoPositioning)))
		{
			return -1;
		}
	}
	return 0;
}

//设置起始结束位置(毫秒)
int CGraph::setStartStopPosition(ulong nstart_ms,ulong nstop_ms)
{
	if (m_pMediaSeeking)
	{
		LONGLONG nanosecond_start_unit = nstart_ms*10000;
		LONGLONG nanosecond_stop_unit = nstop_ms*10000;
		if (FAILED(m_pMediaSeeking->SetPositions(&nanosecond_start_unit, AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame, 
			&nanosecond_stop_unit, AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame)))
		{
			return -1;
		}
	}
	return 0;
}
//获取当前位置(毫秒)
int CGraph::getPosition(ulong &nposition_ms)
{
	if (m_pMediaSeeking)
	{
		LONGLONG nanosecond_unit;
		if (FAILED(m_pMediaSeeking->GetCurrentPosition(&nanosecond_unit)))
		{
			return -1;
		}
		nposition_ms = ulong(nanosecond_unit/10000);//毫秒
	}
	return 0;
}

int CGraph::getDuration(ulong &nduration_ms)
{
	if (m_pMediaSeeking)
	{
		LONGLONG nanosecond_unit;
		if (FAILED(m_pMediaSeeking->GetDuration(&nanosecond_unit)))
		{
			return -1;
		}
		nduration_ms = ulong(nanosecond_unit/10000);//毫秒
	}
	return 0;
}


int CGraph::getVolume(long &lVolume)
{
	if (m_pBasicAudio)
	{
		if (FAILED(m_pBasicAudio->get_Volume(&lVolume)))
		{
			return -1;
		}
	}
	return 0;
}

int CGraph::setVolume(const long lVolume)
{
	if (m_pBasicAudio)
	{
		if (FAILED(m_pBasicAudio->put_Volume(lVolume)))
		{
			return -1;
		}
	}
	return 0;
}

HRESULT CGraph::getGraphEvent(LONG& evCode,LONG& evParam1,LONG evParam2)
{
	HRESULT hr = E_FAIL;
	if (m_pMediaEventEx)
	{
		hr = m_pMediaEventEx->GetEvent(&evCode,(LONG_PTR*)&evParam1,(LONG_PTR*)&evParam2,0);
	}
	return hr;
}

HRESULT CGraph::freeGraphEvent(LONG evCode,LONG evParam1,LONG evParam2)
{
	HRESULT hr = E_FAIL;
	if (m_pMediaEventEx)
	{
		hr = m_pMediaEventEx->FreeEventParams(evCode,evParam1,evParam2);
	}
	return hr;
}

// int CGraph::getStatus(int &retCode)
// {
// 	LONG evCode;
// 	LONG evParam1;
// 	LONG evParam2;
// 	if (m_pMediaEventEx)
// 	{
// 		while(SUCCEEDED(m_pMediaEventEx->GetEvent(&evCode,(LONG_PTR*)&evParam1,(LONG_PTR*)&evParam2,0)))
// 		{
// 			switch (evCode)
// 			{
// 			case EC_COMPLETE:
// 				{//播放结束
// 					retCode = 1;
// 					goto back;
// 				}
// 			case EC_BUFFERING_DATA:
// 				{
// 					if (evParam1 == 0)
// 					{//结束缓冲
// 						retCode = 2;
// 						goto back;
// 					}
// 					else if(evParam1 == 1)
// 					{//开始缓冲
// 						retCode = 3;
// 						goto back;
// 					}
// 				}
// 			case EC_END_OF_SEGMENT:
// 				{
// 					retCode = 4;
// 					goto back;
// 				}
// 			}
// 			HRESULT hr = m_pMediaEventEx->FreeEventParams(evCode,evParam1,evParam2);
// 		}
// 	}
// 	retCode = 0;
// 	return 0;
// 
// back:
// 	HRESULT hr = m_pMediaEventEx->FreeEventParams(evCode,evParam1,evParam2);
// 	return 1;
// }

void CGraph::initAudioTrack(long nIndex)
{
	if (m_pStreamSelect)
	{
		DWORD nStreams=1;
		m_pStreamSelect->Count(&nStreams);
		if (nStreams>1)
		{
			m_pStreamSelect->Enable(nIndex,AMSTREAMSELECTENABLE_ENABLE);
			if (nIndex == 0)
			{
				m_nStream = true;
			}
			else
			{
				m_nStream = false;
			}
		}
	}
}
