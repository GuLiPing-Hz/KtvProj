// DXdemoTestSMDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DXdemoTestSM.h"
#include "DXdemoTestSMDlg.h"
#include <locale.h>
#include "sql/MusicSQLObject.h"
#include "Opengl.h"
#include "Grapha.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd = NULL;
bool g_bRunning = false;

#define MUSICFILEIDR L"\\\\192.168.81.83\\mfile"

void gbk2unicode(const char* gbk,std::wstring& unicode)
{
	static wchar_t wTemp[1024] = {0};
	memset(wTemp,0,1024);

	//utf8 -> unicode
	int n = ::MultiByteToWideChar( CP_ACP, 0, gbk, strlen(gbk), wTemp, 1023 );
	wTemp[n] = 0;
	unicode = wTemp;
}

void utf2unicode(const char* utf,std::wstring& unicode)
{
	static wchar_t wTemp[1024] = {0};
	memset(wTemp,0,1024);

	//utf8 -> unicode
	int n = ::MultiByteToWideChar( CP_UTF8, 0, utf, strlen(utf), wTemp, 1023 );
	wTemp[n] = 0;
	unicode = wTemp;
}

void unicode2utf(const wchar_t* unicode,std::string& utf)
{
	static char utf8[1024] = {0};

	//unicode -> utf8
	int n = ::WideCharToMultiByte( CP_UTF8, 0, unicode, wcslen(unicode), utf8, 1023,NULL,NULL );
	utf8[n] = 0;
	utf = utf8;
}

void utf2gbk( const char * utf, std::string & mbc )
{
	static wchar_t wTemp[1024] = {0};
	static char cTemp[1024] = {0};

	memset(wTemp,0,1024);
	memset(cTemp,0,1024);

	//utf8 -> unicode
	::MultiByteToWideChar( CP_UTF8, 0, utf, strlen(utf), wTemp, 1023 );
	//unicode -> gbk
	::WideCharToMultiByte( CP_ACP, 0, wTemp, wcslen(wTemp), cTemp, 1023, 0, 0 );

	mbc = cTemp;
}
// CDXdemoTestSMDlg 对话框

CDXdemoTestSMDlg::CDXdemoTestSMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDXdemoTestSMDlg::IDD, pParent)
	, m_ncurValue(0)
	, m_bRunning(true)
	, m_curTime()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_smtMgr.Create();
	char *tmp_buf = "GL";
	m_smtMgr.PostMessage(MSG_CREATESHAREMEMORY,(void*)tmp_buf,0);
	timeBeginPeriod(1);
}

CDXdemoTestSMDlg::~CDXdemoTestSMDlg()
{
	m_bRunning = false;
	ReleaseSemaphore(m_hSemphore,1,NULL);
	CloseHandle(m_hSemphore);
	OnBnClickedClosePlayer();
	m_smtMgr.Release();
	timeEndPeriod(1);
}

void CDXdemoTestSMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_MESSAGE, m_ctrlMessage);
	//DDX_Control(pDX, IDC_EDIT1, m_ctrlNextSongName);
	DDX_Control(pDX, IDC_STATIC_SONGLIST, m_ctrlSongList);
	DDX_Control(pDX, IDC_PROGRESSDB, m_progressDb);
}

BEGIN_MESSAGE_MAP(CDXdemoTestSMDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BNB_TEST, &CDXdemoTestSMDlg::OnBnClickedBnbTest)
	ON_BN_CLICKED(IDC_TestNeighborEx, &CDXdemoTestSMDlg::OnBnClickedTestneighborex)
	ON_BN_CLICKED(IDC_BTEST_DBSQL, &CDXdemoTestSMDlg::OnBnClickedBtestDbsql)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTESTDAUDIO, &CDXdemoTestSMDlg::OnBnClickedBtestdaudio)
	ON_BN_CLICKED(IDC_BUTTON1, &CDXdemoTestSMDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &CDXdemoTestSMDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDXdemoTestSMDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CDXdemoTestSMDlg 消息处理程序

void GetFileSetFromDir(CStringW dir, CStringW pattern, SETSTRING &files,bool needDir=true)
{
	if(dir.IsEmpty())
		return ;

	WIN32_FIND_DATAW FindFileData;

	HANDLE hFind = FindFirstFileW((dir+L"\\*.*"), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return;
	do {
		if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			//printf("+[%s]",find.cFileName);
			if(FindFileData.cFileName[0] != L'.')
			{
				wchar_t pszTemp[MAX_PATH]={0};
				swprintf(pszTemp,L"%s\\%s",dir,FindFileData.cFileName);
				CStringW tmpDir = pszTemp;
				GetFileSetFromDir(tmpDir,pattern,files);
			}

		}
		else
		{
			if ( pattern.IsEmpty() || (wcsstr(FindFileData.cFileName,pattern) != NULL))
			{
				wchar_t wideChar[260] = {0};
				wcscpy(wideChar,FindFileData.cFileName);
				int len = wcslen(wideChar) + 1;
				_wcslwr_s(wideChar, len);
				CStringW str_file;
				if(needDir)
					str_file = (dir + L"\\" + wideChar);
				else
					str_file = wideChar;
				files.insert(str_file);
			}
		}

	}
	while (FindNextFileW(hFind, &FindFileData) != 0);
	FindClose(hFind);
}

void GetFileListFromDir(CStringW dir, CStringW pattern, LISTSONGALL &files,CStringW& d_song)
{
	WIN32_FIND_DATAW FindFileData;

	HANDLE hFind = FindFirstFileW((dir+L"\\*.*"), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return;
	do {
		if(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			//printf("+[%s]",find.cFileName);
			if(FindFileData.cFileName[0] != L'.')
			{
				wchar_t pszTemp[MAX_PATH]={0};
				swprintf(pszTemp,L"%s\\%s",dir,FindFileData.cFileName);
				CStringW tmpDir = pszTemp;
				GetFileListFromDir(tmpDir,pattern,files,d_song);
			}

		}
		else
		{
			if ( pattern.IsEmpty() || wcsstr(FindFileData.cFileName,pattern))
			{
				static int song_id = 0;
				song_id ++;
				CStringW str_file = (dir + L"\\" + FindFileData.cFileName);
				Songlist item;
				item.temp = str_file;
				item.bScore = false;
				files.push_back(item);
				wchar_t tmp_fileName[260] = {0};
				swprintf_s(tmp_fileName,259,L"[%06d]%s",song_id,FindFileData.cFileName);
				d_song = d_song+tmp_fileName + L"\r\n";
			}
		}

	}
	while (FindNextFileW(hFind, &FindFileData) != 0);
	FindClose(hFind);
}

void  CDXdemoTestSMDlg::Ini_SetInt(const char *szIniFile,const char *section, const char *name, int value)
{
	char buf[256];

	if(szIniFile)
	{
		sprintf(buf,"%d",value);
		WritePrivateProfileString(section, name, buf, szIniFile);
	}
}


int  CDXdemoTestSMDlg::Ini_GetInt(const char *szIniFile,const char *section, const char *name, int def_val)
{
	char buf[256];

	if(szIniFile) 
	{
		if(GetPrivateProfileString(section, name, "", buf, sizeof(buf), szIniFile))
		{
			return atoi(buf); 
		}
		else
		{
			return def_val;
		}
	}
	return def_val;
}

BOOL CDXdemoTestSMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);			// 设置小图标

	// TODO: 在此添加额外的初始化代码
	g_hWnd = m_hWnd;

	CString songList;
	CString strPath = "";
	char buf[260] = {0};
	GetCurrentDirectory(259,buf);
	m_strBear = m_strBear+buf+"\\smooth.txt";
	sprintf_s(buf,259,"%s\\tm.ini",buf);
	//Ini_SetInt(buf,"CANBEAR","MAXMS",5000);
	m_uCanBear = Ini_GetInt(buf,"CANBEAR","MAXMS",5000);
// 	DWORD nDrives=GetLogicalDrives();
// 	printf("%08x\n",nDrives);
 	CString CurDir = "\\\\192.168.3.218\\e";
	//GetFileListFromDir(CurDir,".mpg",m_songlist,songList);
	CurDir = "\\\\192.168.3.218\\f";
	//GetFileListFromDir(CurDir,".mpg",m_songlist,songList);

	m_uCur = 0;
	m_uTotal = m_songlist.size();
	
	int cur_display_w = ::GetSystemMetrics(SM_CXSCREEN);
	int cur_display_h = ::GetSystemMetrics(SM_CYSCREEN);
	RECT rect;
	::GetWindowRect(m_hWnd,&rect);
	//rect.left += cur_display_w+100;
	//rect.right += cur_display_w+100; 
	//MoveWindow(&rect);
	m_hSemphore = ::CreateSemaphore(NULL,0,10,"WaitPlay");
	SetDlgItemText(IDC_STATIC_SONGLIST,songList);

	m_progressDb.SetRange(0,1000);
	m_progressDb.SetPos(0);
	m_progressDb.SetStep(1);

	//GetFileSetFromDir(MUSICFILEIDR,L"",m_setFiles);
	//GetFileSetFromDir(L"J:\\",L"",m_setFiles);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDXdemoTestSMDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CDXdemoTestSMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDXdemoTestSMDlg::OnBnClickedSwitchsong()
{
	if (!m_songlist.empty())
	{
		m_curSongItem = m_songlist.front();
		m_songlist.pop_front();
		//m_curSongName = "\\\\192.168.2.131\\f\\193\\960139=●=倒带=●=蔡依林.mpg";
		//m_curSongName = "file://192.168.2.131/f/193/960139=●=倒带=●=蔡依林.mpg";
		wchar_t * pSongName = new wchar_t[260];
		wcscpy(pSongName,m_curSongItem.strMusicName.c_str());
		int * pstaff_pos_height = new int;
		*pstaff_pos_height = 0;
		*pstaff_pos_height = ((30<<24)|(1<<23)|(2<<19)|(3<<15)|(4<<11)|(0<<10)|(0<<9)|(20<<1));
		char* hchar = (char*)pstaff_pos_height;
		//*pstaff_pos_height = 300;
		wchar_t * pSingerName = new wchar_t[260];
		wcscpy(pSingerName,L"歌手名");
		wchar_t * pSongShortName = new wchar_t[260];
		wcscpy(pSongShortName,L"歌曲名11");
		wchar_t * pSingerPic = new wchar_t[260];
		wcscpy(pSingerPic,L"C:\\Users\\Administrator\\Documents\\Visual Studio 2005\\Projects\\DXdemoTestSM\\debug\\singer.jpg");
		m_smtMgr.PostMessage(MSG_SINGER_PIC,(void*)pSingerPic,0);
		int* eType = new int;
		*eType = 2;
		m_smtMgr.PostMessage(MSG_PLAY_GRADE,(void*)eType,0);
		//发送歌曲信息
		m_smtMgr.PostMessage(MSG_SONG_INFO,(void*)pSingerName,(void*)pSongShortName);
		//起始结束时间
 		//int *p_song_start = new int;
 		//int *p_song_end = new int;
		//*p_song_start = 100;
		//*p_song_end = 120;
 		//m_smtMgr.PostMessage(MSG_SONG_STARTEND,(void*)p_song_start,(void*)p_song_end);
// 		char* pNextSong = new char[260];
// 		strcpy(pNextSong,"ktv2");
// 		m_smtMgr.PostMessage(MSG_NEXTSONG,(void*)pNextSong,0);
		m_smtMgr.PostMessage(MSG_SWITCH_SONG,(void*)pSongName,(void*)pstaff_pos_height);
		char* pNextSong1 = new char[260];
		strcpy(pNextSong1,"3002第一场雪");
		m_smtMgr.PostMessage(MSG_NEXTSONG,(void*)pNextSong1,0);
		SetDlgItemText(IDC_SONG_STATE,"歌曲正在播放或暂停中");
	}
	//控制灯光，音量等
}

LRESULT CDXdemoTestSMDlg::OnScoreStarNum(WPARAM wParam,LPARAM lParam)
{
	int* nScore = (int*)wParam;
	int* nStarNum = (int*)lParam;
	if (nScore && nStarNum)
	{
		float aveScore = *nScore/10.0f;
		char strScore[260] = {0};
		sprintf(strScore,"总分:%.1f\n\r星星值:%d",aveScore,*nStarNum);
		SetDlgItemText(STATIC_FINAL_SCORE,strScore);
	}

	if (nStarNum)
		delete nStarNum;
	if (nScore)
		delete nScore;

	return 0;
}


LRESULT CDXdemoTestSMDlg::OnEndSong(WPARAM wParam,LPARAM lParam)
{
	SetDlgItemText(IDC_SONG_STATE,"歌曲已播放完毕");//包括评分结算画面
	return 0;
}

void CDXdemoTestSMDlg::OnBnClickedResing()
{
	if (m_curSongItem.strMusicName != L"")
	{
		//for ( int i=0;i<20000;i++ )
		{
			wchar_t * pSongName = new wchar_t[260];
			int *pint = new int;
			*pint = 0;
			*pint = ((30<<24)|(1<<23)|(2<<19)|(3<<15)|(4<<11)|(0<<10)|(0<<9)|(20<<1));
			//*pint = 300;
			wcscpy(pSongName,m_curSongItem.strMusicName.c_str());
			m_smtMgr.PostMessage(MSG_SWITCH_SONG,(void*)pSongName,(void*)pint);
			//不用发歌曲信息
			//Sleep(2500);
		}
	}
}

void CDXdemoTestSMDlg::OnBnClickedGrade()
{
	static int nStart = 2;
	if (nStart == 3)
	{
		nStart = 0;
	}
	int* pInt = new int;
	*pInt = nStart++;
	m_smtMgr.PostMessage(MSG_PLAY_GRADE,(void*)pInt,0);
}

void CDXdemoTestSMDlg::OnBnClickedAccompany()
{
	m_smtMgr.PostMessage(MSG_PLAY_ACCOMPANY,0,0);
}

void CDXdemoTestSMDlg::OnBnClickedClosePlayer()
{
	m_smtMgr.PostMessage(MSG_DXPLAYER_CLOSE,0,0);
}

void CDXdemoTestSMDlg::OnBnClickedMute()
{
	//int *pInt = new int;
	//*pInt = -10000;
	//m_smtMgr.PostMessage(MSG_SET_VOLUME,(void*)pInt,0);
	m_smtMgr.PostMessage(MSG_MUTE_OPEN,0,0);
}



void CDXdemoTestSMDlg::OnStnClickedFinalScore()
{
	// TODO: Add your control notification handler code here
}

void CDXdemoTestSMDlg::OnBnClickedMaxVolume()
{
	m_smtMgr.PostMessage(MSG_MUTE_CLOSE,0,0);
}

void CDXdemoTestSMDlg::OnBnClickedAddVolume()
{
	m_ncurValue += 1;
	if (m_ncurValue >= 5)
	{
		m_ncurValue = 5;
	}
	int *pInt = NULL;//new int;
	//*pInt = m_ncurValue;
	m_smtMgr.PostMessage(MSG_SET_ADD_TONE,(void*)pInt,0);
}

void CDXdemoTestSMDlg::OnBnClickedSubVolume()
{
	m_ncurValue -= 1;
	if (m_ncurValue <= -5)
	{
		m_ncurValue = -5;
	}
	int *pInt = NULL;//new int;
	//*pInt = m_ncurValue;
	m_smtMgr.PostMessage(MSG_SET_SUB_TONE,(void*)pInt,0);
}

void CDXdemoTestSMDlg::OnBnClickedPublicNotify()
{
}

void CDXdemoTestSMDlg::OnBnClickedFirewarning()
{
	static bool b = false;
	b = !b;
	if (b)
	{
		m_smtMgr.PostMessage(MSG_FIREWARNING,0,0);
	}
	else
	{
		m_smtMgr.PostMessage(MSG_NOFIREWARNING,0,0);
	}
}


void CDXdemoTestSMDlg::OnBnClickedResing2()
{
	if (!m_curSongItem.strMusicName.empty())
	{
		//for ( int i=0;i<20000;i++ )
		{
			wchar_t * pSongName = new wchar_t[260];
			int *pint = new int;
			*pint = 0;
			*pint = ((30<<24)|(1<<23)|(2<<19)|(3<<15)|(4<<11)|(0<<10)|(1<<9)|(20<<1));
			//*pint = 300;
			wcscpy(pSongName,m_curSongItem.strMusicName.c_str());
			m_smtMgr.PostMessage(MSG_SWITCH_SONG,(void*)pSongName,(void*)pint);
			//不用发歌曲信息
			//Sleep(2500);
		}
	}
}

void CDXdemoTestSMDlg::OnBnClickedBapplaud()
{
	int* pInt = new int;
	*pInt = 0;
	m_smtMgr.PostMessage(MSG_SOUND,(void*)pInt,0);
}

void CDXdemoTestSMDlg::OnBnClickedBcheer()
{
	int* pInt = new int;
	*pInt = 1;
	m_smtMgr.PostMessage(MSG_SOUND,(void*)pInt,0);
}

void CDXdemoTestSMDlg::OnBnClickedBnextsong()
{
}



void CDXdemoTestSMDlg::OnBnClickedPlayResume()
{
	m_smtMgr.PostMessage(MSG_PLAY_CONTROL_RESUME,0,0);
}

void CDXdemoTestSMDlg::OnBnClickedBplayPause()
{
	m_smtMgr.PostMessage(MSG_PLAY_CONTROL_PAUSE,0,0);
}

bool ParseSongInfo(const std::wstring singer,std::wstring& r_singer1,std::wstring& r_singer2)
{
	char tmpSinger[260] = {0};
	char* pStr = NULL;
	std::wstring tmpwSinger1;
	std::wstring tmpwSinger2;
	std::wstring::size_type f = singer.find(L"／");
	if(f!=std::wstring::npos)
	{
		tmpwSinger1 = singer.substr(0,f);
		tmpwSinger2 = singer.substr(f+1);

		wchar_t lowercase_singer1[256] = {0};
		wchar_t lowercase_singer2[256] = {0};

		swprintf(lowercase_singer1,L"（%s）",tmpwSinger1.c_str());
		swprintf(lowercase_singer2,L"（%s）",tmpwSinger2.c_str());

		int len = wcslen(lowercase_singer1)+1;
		_wcslwr_s(lowercase_singer1, len);//转成小写
		r_singer1 = lowercase_singer1;
		len = wcslen(lowercase_singer2) + 1;
		_wcslwr_s(lowercase_singer2, len);
		r_singer2 = lowercase_singer2;
		return true;
	}
	else
	{
		r_singer1 = L"";
		r_singer2 = L"";
		return false;
	}
}

bool CDXdemoTestSMDlg::CheckFile(const wchar_t* file)
{
	
	if(m_setFiles.find(file) != m_setFiles.end())
		return true;
	return false;

// 	HANDLE hF = CreateFileW( file, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
// 	if(hF == INVALID_HANDLE_VALUE)
// 		return false;
// 	else
// 		CloseHandle(hF);
// 	return true;
}

DWORD CDXdemoTestSMDlg::ThreadTest(LPVOID pParam)
{
	g_bRunning = true;

	static bool b = true;

	CDXdemoTestSMDlg* pDlg = (CDXdemoTestSMDlg*)pParam;
	LISTSONGALL::const_iterator i = pDlg->m_songlist.begin();
	int j=0;
	FILE* fp = _wfopen(L"ResultSongCheck.txt",L"w");
	if (fp == NULL)
		return 0 ;

	UINT nTimer = ::SetTimer(pDlg->GetSafeHwnd(),TIMER_RUNTIME,1000,NULL);
	SetDlgItemTextW(pDlg->GetSafeHwnd(),IDC_SONG_STATE,L"正在检查歌曲文件...");

	int nError = 0;
	bool bNoError = true;
	CStringW toLog;
	std::string utfLog;
	wchar_t bufLog[300] = {0};
	while(i != pDlg->m_songlist.end()&&pDlg->m_bRunning)
	{
		bNoError = true;
		pDlg->m_ctrlSongList.LineScroll(1);
		wchar_t file[260] = {0};
		swprintf(file,L"%s.mp4",(*i).temp);
		if(!pDlg->CheckFile(file))
		{
			//swprintf(bufLog,L"                ERROR-MP4 File : file open error or no such file[%s]\n",file);
			//toLog += bufLog;
			unicode2utf(file,utfLog);
			fprintf(fp,"                ERROR-MP4 File : file open error or no such file[%s]\n",utfLog.c_str());
			fflush(fp);//对utf8编码的日志，需要及时fflush，否则等系统自己flush的时候字符编码格式会不对。
			bNoError = false;
		}
		
		if ((*i).bScore)//如果是评分歌曲
		{
			wchar_t mdmName[260] = {0};
			wchar_t datName[260] = {0};

			if((*i).bAntiphonal)
			{
				std::wstring singer1;
				std::wstring singer2;
				if(ParseSongInfo((*i).strSingerName,singer1,singer2))
				{
					swprintf(mdmName,L"%s%s.mdm",(*i).temp,singer1.c_str());
					if(!pDlg->CheckFile(mdmName))
					{
						//swprintf(bufLog,L"                ERROR-MP4 File : file open error or no such file[%s]\n",mdmName);
						//toLog += bufLog;
						unicode2utf(mdmName,utfLog);
						fprintf(fp,"                ERROR-MDM File : file open error or no such file[%s]\n",utfLog.c_str());
						fflush(fp);
						bNoError = false;
					}
					swprintf(mdmName,L"%s%s.mdm",(*i).temp,singer2.c_str());
					if(!pDlg->CheckFile(mdmName))
					{
						//swprintf(bufLog,L"                ERROR-MP4 File : file open error or no such file[%s]\n",mdmName);
						//toLog += bufLog;
						unicode2utf(mdmName,utfLog);
						fprintf(fp,"                ERROR-MDM File : file open error or no such file[%s]\n",utfLog.c_str());
						fflush(fp);
						bNoError = false;
					}

					swprintf(datName,L"%s%s.dat",(*i).temp,singer1.c_str());
					if(!pDlg->CheckFile(datName))
					{
						//swprintf(bufLog,L"                ERROR-MP4 File : file open error or no such file[%s]\n",datName);
						//toLog += bufLog;
						unicode2utf(datName,utfLog);
						fprintf(fp,"                ERROR-DAT File : file open error or no such file[%s]\n",utfLog.c_str());
						fflush(fp);
						bNoError = false;
					}
					swprintf(datName,L"%s%s.dat",(*i).temp,singer2.c_str());
					if(!pDlg->CheckFile(datName))
					{
						//swprintf(bufLog,L"                ERROR-MP4 File : file open error or no such file[%s]\n",datName);
						//toLog += bufLog;
						unicode2utf(datName,utfLog);
						fprintf(fp,"                ERROR-DAT File : file open error or no such file[%s]\n",utfLog.c_str());
						fflush(fp);
						bNoError = false;
					}
				}
				else
				{
					unicode2utf((*i).temp,utfLog);
					fprintf(fp,"                ERROR-bAntiphonal File : %s\n",utfLog.c_str());
					fflush(fp);
					bNoError = false;
				}
			}
			else
			{
				swprintf(mdmName,L"%s.mdm",(*i).temp);
				if(!pDlg->CheckFile(mdmName))
				{
					//swprintf(bufLog,L"                ERROR-MP4 File : file open error or no such file[%s]\n",mdmName);
					//toLog += bufLog;
					unicode2utf(mdmName,utfLog);
					fprintf(fp,"                ERROR-MDM File : file open error or no such file[%s]\n",utfLog.c_str());
					fflush(fp);
					bNoError = false;
				}
				swprintf(datName,L"%s.dat",(*i).temp);
				if(!pDlg->CheckFile(datName))
				{
					//swprintf(bufLog,L"                ERROR-MP4 File : file open error or no such file[%s]\n",datName);
					//toLog += bufLog;
					unicode2utf(datName,utfLog);
					fprintf(fp,"                ERROR-DAT File : file open error or no such file[%s]\n",utfLog.c_str());
					fflush(fp);
					bNoError = false;
				}
			}
		}

		if(!bNoError)
		{
			//swprintf(bufLog,L"[%06d] : ERROR_OCCUR : [%s]\n",++nError,(*i).temp);
			//toLog += bufLog;
			unicode2utf((*i).temp,utfLog);
			fprintf(fp,"[%06d] ERROR OCCUR %s: [%s]\n",++nError,(*i).bAntiphonal?"Antiphonal":"",utfLog.c_str());
			fflush(fp);
		}

// 		if(toLog.GetLength() > 5000)
// 		{
// 			unicode2utf(toLog,utfLog);
// 			fprintf(fp,"%s",utfLog.c_str());
// 			toLog = L"";
// 		}
		j++;
		i++;

		int nCur = j*1000/pDlg->m_songlist.size();
		pDlg->m_progressDb.SetPos(nCur);
	}

	CStringW tip;
	tip.Format(L"检查歌曲文件完毕,一共%d首，异常的有%d首",pDlg->m_songlist.size(),nError);
	SetDlgItemTextW(pDlg->GetSafeHwnd(),IDC_SONG_STATE,tip);
	::KillTimer(pDlg->GetSafeHwnd(),nTimer);
	fflush(fp);
	fclose(fp);
	g_bRunning = false;
	return 0;
}

// bool g_bRunning = false;
DWORD CDXdemoTestSMDlg::ThreadTestAudio(LPVOID pParam)
{
	g_bRunning = true;
	CoInitialize(NULL);
	char buf [260] = {0};
	GetCurrentDirectory(259,buf);
	char log_name[260] = {0};
	char tmp_file1[260] = {0};
	char tmp_file2[260] = {0};
	static bool b = true;

	sprintf_s(log_name,259,"%s\\AudioTest.txt",buf);
	FILE* fp = fopen(log_name,"w");
	if (fp == NULL)
	{
		return 0 ;
	}

	CDXdemoTestSMDlg* pDlg = (CDXdemoTestSMDlg*)pParam;
	LISTSONGALL::iterator i = pDlg->m_songlist.begin();
	UINT nTimer = ::SetTimer(pDlg->m_hWnd,TIMER_RUNTIME,1000,NULL);
	while(i != pDlg->m_songlist.end()&&pDlg->m_bRunning)
	{
		pDlg->m_ctrlSongList.LineScroll(1);
		HANDLE hF = CreateFileW( (*i).strMusicName.c_str(), GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if(hF == INVALID_HANDLE_VALUE)
		{
			FILE* fp = fopen(log_name,"a+");
			if (fp == NULL)
			{
				return 0 ;
			}
			fprintf(fp," fileName open error or no such file[%s]\n",(*i).strMusicName.c_str());
			fclose(fp);
		}
		else
		{
			static CGraph* pGraph = CGraph::getSingleton();
			pGraph->errorUninit();
			HRESULT hr = pGraph->gStartGraph(pDlg->GetSafeHwnd(),(*i).strMusicName.c_str(),CGSize(1280,720),NULL,0);
			if (FAILED(hr))
			{
				fprintf(fp,"[%s] have only one audio line ,please report to 51Mike!\n",(*i).strMusicName.c_str());
			}
		}

	}
	::KillTimer(pDlg->m_hWnd,nTimer);
	CoUninitialize();
	g_bRunning = false;
}

DWORD CDXdemoTestSMDlg::ThreadTest1(LPVOID pParam)
{
	g_bRunning = true;
	CoInitialize(NULL);
	char buf [260] = {0};
	GetCurrentDirectory(259,buf);
	char log_name[260] = {0};
	sprintf_s(log_name,259,"%s\\neighbortest.txt",buf);

	CDXdemoTestSMDlg* pDlg = (CDXdemoTestSMDlg*)pParam;
	LISTSONGALL::iterator i = pDlg->m_songlist.begin();
	int j = 0;
	UINT nTimer = ::SetTimer(pDlg->m_hWnd,TIMER_RUNTIME,1000,NULL);
	while(i != pDlg->m_songlist.end()&&pDlg->m_bRunning)
	{
		pDlg->m_ctrlSongList.LineScroll(1);
		HANDLE hF = CreateFileW( (*i).strMusicName.c_str(), GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if(hF == INVALID_HANDLE_VALUE)
		{
			FILE* fp = fopen(log_name,"a+");
			if (fp == NULL)
			{
				return 0 ;
			}
			fprintf(fp," fileName open error or no such file[%s]\n",(*i).strMusicName.c_str());
			fclose(fp);
		}
		else
		{
			pDlg->m_pMediaControl = NULL;
			if (pDlg->m_pMediaEventEx)
			{
				pDlg->m_pMediaEventEx->SetNotifyWindow((OAHWND)NULL,0,0);
			}
			pDlg->m_pMediaEventEx = NULL;
			pDlg->m_pMediaSeeking = NULL;
			pDlg->m_pGraph = NULL;
			CloseHandle(hF);
			HRESULT hr = CoCreateInstance(CLSID_FilterGraph,NULL,/*CLSCTX_ALL*/CLSCTX_INPROC_SERVER,IID_IGraphBuilder,(void**)&(pDlg->m_pGraph.p));
			hr = pDlg->m_pGraph->QueryInterface(IID_IMediaControl, (void**)&(pDlg->m_pMediaControl.p));
			if (FAILED(hr))
			{
				DbgBreak([DXLOG CGraph::initGraph] : IID_IMediaControl error);
				return -1;
			}
			hr = pDlg->m_pGraph->QueryInterface(IID_IMediaEventEx,(void**)&(pDlg->m_pMediaEventEx.p));
			if (FAILED(hr))
			{
				DbgBreak([DXLOG CGraph::initGraph] : IID_IMediaEventEx error);
				return -1;
			}
			hr = pDlg->m_pGraph->QueryInterface(IID_IMediaSeeking,(void**)&(pDlg->m_pMediaSeeking.p));
			if (FAILED(hr))
			{
				DbgBreak([DXLOG CGraph::initGraph] : IID_IMediaSeeking error);
				return -1;
			}

			if (pDlg->m_pMediaEventEx)
			{
				hr = pDlg->m_pMediaEventEx->SetNotifyWindow((OAHWND)pDlg->m_hWnd, WM_GRAPHNOTIFY, 0);
				if (FAILED(hr))
				{
					DbgBreak(SetNotifyWindow error);
					return -1;
				}
			}
			wchar_t tmp_file [260] = {0};
			wcscpy(tmp_file,(*i).strMusicName.c_str());
			_bstr_t File = tmp_file;
			pDlg->m_pGraph->RenderFile(File,NULL);
			pDlg->m_pMediaControl->Run();
			pDlg->m_uStartMS = timeGetTime();
			WaitForSingleObject(pDlg->m_hSemphore,INFINITE);
		}
		i++;
		j++;
	}
	::KillTimer(pDlg->m_hWnd,nTimer);
	g_bRunning = false;
	CoUninitialize();
	return 0;
}

LRESULT CDXdemoTestSMDlg::OnPlayEnd(WPARAM wParam,LPARAM lParam)
{
	LONG evCode;
	LONG evParam1;
	LONG evParam2;
	if (m_pMediaEventEx)
	{
		while(SUCCEEDED(m_pMediaEventEx->GetEvent(&evCode,(LONG_PTR*)&evParam1,(LONG_PTR*)&evParam2,0)))
		{
			m_pMediaEventEx->FreeEventParams(evCode,evParam1,evParam2);
			switch (evCode)
			{
			case EC_USERABORT:
			case EC_COMPLETE:
				{//播放结束
					::ReleaseSemaphore(m_hSemphore,1,NULL);
					return 1;
				}
			case EC_BUFFERING_DATA:
				{
					if (evParam1 == 0)
					{//结束缓冲
						return 2;
					}
					else if(evParam1 == 1)
					{//开始缓冲
						return 3;
					}
				}
			case EC_END_OF_SEGMENT:
				{
					return 4;
				}
			}
		}
	}
	return -1;
}

void CDXdemoTestSMDlg::OnBnClickedBnbTest()
{
	if (g_bRunning)
		return ;
	DWORD threadID;
	HANDLE hThread = ::CreateThread(NULL,0,&CDXdemoTestSMDlg::ThreadTest,(LPVOID)this,0,&threadID);
	if (hThread)
		CloseHandle(hThread);
}

void CDXdemoTestSMDlg::OnBnClickedTestneighborex()
{
	if (g_bRunning)
	{
		return ;
	}

	DWORD threadID;
	m_curTime = 0;
	HANDLE hThread = ::CreateThread(NULL,0,&CDXdemoTestSMDlg::ThreadTest1,(LPVOID)this,0,&threadID);
	if (hThread)
	{
		CloseHandle(hThread);
	}
}

#define PAGE_SIZE	8000

int CDXdemoTestSMDlg::GetKTVMusicList( const char* path, std::list<_tMusicInfo> & lstMusicInfo)
{
	std::string strPath = path;
	XGSQLITE::CMusicSQLObject * pMusicObject = new XGSQLITE::CMusicSQLObject( strPath );

	if( !pMusicObject )
		return 0;

	SetDlgItemText(IDC_SONG_STATE,"正在读取数据库歌曲信息...");

	int nPageCount = 0, nMusicCount = 0;

	pMusicObject->ClearSearchCondition();
	//pMusicObject->SetSearchConditionLangType(6);//6-韩语
	nMusicCount = pMusicObject->GetMusicCount();
	nPageCount = nMusicCount / PAGE_SIZE + (nMusicCount % PAGE_SIZE ? 1 : 0);

	for( int i = 1; i <= nPageCount; ++i )
	{
		std::list<XGSQLITE::CMusicInfo*> db_song_list;
		pMusicObject->GetMusicInfo( i, PAGE_SIZE, db_song_list );

		std::list<XGSQLITE::CMusicInfo*>::iterator iter = db_song_list.begin();
		for( int j = 1; iter != db_song_list.end(); ++iter,++j )
		{
			_tMusicInfo info;
			info.nMusicId = (*iter)->m_music_id;
			std::wstring tmp_fileName;
			std::wstring tmp_singName;
			utf2unicode((*iter)->m_music_name,tmp_fileName);
			utf2unicode((*iter)->m_music_singer,tmp_singName);
			info.strMusicName = tmp_fileName;
			info.strSingerName = tmp_singName;
			info.bScore = (*iter)->m_music_isScore;
			info.bAntiphonal = !! (*iter)->m_music_isChorus;
			lstMusicInfo.push_back( info );

			int nCur = (i*PAGE_SIZE+j)*1000/nMusicCount;
			m_progressDb.SetPos(nCur);
		}

		//Sleep(1);
		for (iter=db_song_list.begin();iter!=db_song_list.end();iter++)
		{
			if (*iter)
				SAFE_DELETE(*iter);
		}

	}

	delete( pMusicObject );

	char tip[260] = {0};
	sprintf(tip,"读取数据库歌曲信息完毕,一共%d首",nMusicCount);
	SetDlgItemText(IDC_SONG_STATE,tip);
	return nMusicCount;
}

const wchar_t* GetFileTemplateBySongId(int id,bool isGrade,bool isDouble,const wchar_t* song,const wchar_t* singer,bool isFromDb)
{
	static wchar_t dir[260] = {0};
	int dirId = id/10000+1;

	if(!song || !singer)
		return NULL;
	
	wchar_t lowercase_song[256] = {0};
	wchar_t lowercase_singer[256] = {0};

	wcscpy(lowercase_song,song);
	int len = wcslen(lowercase_song) + 1;
	_wcslwr_s(lowercase_song, len);
	wcscpy(lowercase_singer,singer);
	len = wcslen(lowercase_singer) + 1;
	_wcslwr_s(lowercase_singer, len);
	//int fileId = dirId%2 == 0?2:1;
	const wchar_t* fileId = dirId%2 == 0?L"file2":L"file1";
	if(isFromDb)
		swprintf(dir,L"%s\\9158_MPG_%d\\%06d=●=%s=●=%s",MUSICFILEIDR,/*fileId,*/dirId,id,lowercase_song,lowercase_singer);
	else
		swprintf(dir,L"%06d=●=%s=●=%s",id,lowercase_song,lowercase_singer);
	return dir;
}

bool readWord(const char** ppbufferIn,char* buffOut,int& size)
{
	if(!ppbufferIn || !(*ppbufferIn) || !buffOut || size==0)
		return false;

	char split = 0x09;
	int i = 0;
	while(**ppbufferIn != split && **ppbufferIn != 0)
	{
		buffOut[i] = **ppbufferIn;
		(*ppbufferIn) ++;
		i++;

		if(i >= size)
			return false;
	}
	buffOut[i] = 0;
	size = i+1;
	(*ppbufferIn) ++;//跳过分隔符
	return true;
}

bool parseLine(const char* line,_tMusicInfo & lstMusicInfo)
{
	if(!line || line[0]==0)
		return false;

	char buffer[300] = {0};
	const char* pCur = line;

	int size = sizeof(buffer);
	if(readWord(&pCur,buffer,size))//编号
		lstMusicInfo.nMusicId = atoi(buffer);
	else
		return false;

	size = sizeof(buffer);
	readWord(&pCur,buffer,size);//nbh

	size = sizeof(buffer);
	if(readWord(&pCur,buffer,size))//歌名
		 utf2unicode(buffer,lstMusicInfo.strMusicName);
	else
		return false;

	size = sizeof(buffer);
	readWord(&pCur,buffer,size);//yz

	size = sizeof(buffer);
	readWord(&pCur,buffer,size);//qz

	size = sizeof(buffer);
	readWord(&pCur,buffer,size);//theme

	size = sizeof(buffer);
	readWord(&pCur,buffer,size);//bihua

	size = sizeof(buffer);
	readWord(&pCur,buffer,size);//zs

	size = sizeof(buffer);
	readWord(&pCur,buffer,size);//zt

	size = sizeof(buffer);
	if(readWord(&pCur,buffer,size))//歌星
		utf2unicode(buffer,lstMusicInfo.strSingerName);
	else
		return false;

	size = sizeof(buffer);
	readWord(&pCur,buffer,size);//pygx

	size = sizeof(buffer);
	readWord(&pCur,buffer,size);//gxtype

	size = sizeof(buffer);
	readWord(&pCur,buffer,size);//lrbc

	size = sizeof(buffer);
	readWord(&pCur,buffer,size);//sdate

	size = sizeof(buffer);
	readWord(&pCur,buffer,size);//time

	size = sizeof(buffer);
	readWord(&pCur,buffer,size);//ishd

	size = sizeof(buffer);
	if(readWord(&pCur,buffer,size))//是否评分
		lstMusicInfo.bScore = !!atoi(buffer);
	else
		return false;

	size = sizeof(buffer);
	if(readWord(&pCur,buffer,size))//是否对唱
		lstMusicInfo.bAntiphonal = !!atoi(buffer);
	else 
		return false;

	return true;
}

bool readSongInfoFile(const char* file,std::list<_tMusicInfo> & lstMusicInfo)
{
	if(!file || file[0] == 0 )
		return false;

	FILE* fp = fopen(file,"r");
	if(!fp)
		return false;

	char buf[1024] = {0};
	bool isFirstLine = true;
	//fread
	while(fgets(buf,1023,fp))//读取一行内容
	{
		_tMusicInfo info = {0};
		if(!isFirstLine && parseLine(buf,info))//第一行需要跳过
			lstMusicInfo.push_back(info);

		if(isFirstLine)
			isFirstLine = false;
	}

	return !lstMusicInfo.empty();
}

DWORD WINAPI CDXdemoTestSMDlg::ThreadDb(LPVOID pParam)
{
	CDXdemoTestSMDlg* pDlg = (CDXdemoTestSMDlg*)pParam;
	if(!pDlg || g_bRunning)
	{
		return -1;
	}

	char curr_dir [260] = {0};
	GetCurrentDirectory(259,curr_dir);
	std::list<_tMusicInfo> db_songlist;

	if(pDlg->m_isFromDB)
	{
		sprintf_s(curr_dir,259,"%s\\song.db",curr_dir);
		pDlg->GetKTVMusicList(curr_dir,db_songlist);
	}
	else
	{
		if(!readSongInfoFile(pDlg->m_songFile.c_str(),db_songlist))
			return -3;
	}

	int nSize = db_songlist.size();
	if (nSize == 0)
		return -2;

	SetDlgItemTextW(pDlg->GetSafeHwnd(),IDC_SONG_STATE,L"正在遍历目录的文件...");
	wchar_t wDir[260] = {0};
	MultiByteToWideChar(CP_ACP,0,pDlg->m_songDir.c_str(),pDlg->m_songDir.size(),wDir,259);
	GetFileSetFromDir(wDir,L"",pDlg->m_setFiles,pDlg->m_isFromDB);
// 	if(m_setFiles.empty())
// 		return ;
	
	wchar_t tip[260] = {0};
	swprintf(tip,L"转换歌曲信息(共%d首)...",nSize);
	SetDlgItemTextW(pDlg->GetSafeHwnd(),IDC_SONG_STATE,tip);

	g_bRunning = true;
	pDlg->m_songlist.clear();
	CStringW display_song;
	std::list<_tMusicInfo>::const_iterator i = db_songlist.begin();
	for(int j = 1;i!=db_songlist.end();i++,j++)
	{
		Songlist item;
		item.bAntiphonal = (*i).bAntiphonal;
		item.bScore = (*i).bScore;
		item.nMusicId = (*i).nMusicId;
		item.strMusicName = (*i).strMusicName;
		item.strSingerName = (*i).strSingerName;
		item.temp = GetFileTemplateBySongId(item.nMusicId,item.bScore,item.bAntiphonal,item.strMusicName.c_str(),item.strSingerName.c_str(),pDlg->m_isFromDB);
		pDlg->m_songlist.push_back(item);

		wchar_t buf[300] = {0};
		swprintf(buf,L"%d : %06d-%s-%s\r\n",j,item.nMusicId,item.strMusicName.c_str(),item.strSingerName.c_str());
		display_song = display_song + buf;
		int nCur = j*1000/nSize;
		pDlg->m_progressDb.SetPos(nCur);
	}

	wchar_t buf[300] = {0};
	swprintf(buf,L"转换数据库歌曲信息完毕!共%d首",nSize);
	SetDlgItemTextW(pDlg->GetSafeHwnd(),IDC_SONG_STATE,buf);

	SetDlgItemTextW(pDlg->GetSafeHwnd(),IDC_STATIC_SONGLIST,display_song);
	g_bRunning = false;

	pDlg->OnBnClickedBnbTest();

	return 0;
}

void CDXdemoTestSMDlg::OnBnClickedBtestDbsql()
{
	if (g_bRunning)
		return ;
	DWORD threadID;
	m_isFromDB = true;
	HANDLE hThread = ::CreateThread(NULL,0,&CDXdemoTestSMDlg::ThreadDb,(LPVOID)this,0,&threadID);
	if (hThread)
		CloseHandle(hThread);
}

void CDXdemoTestSMDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_RUNTIME)
	{
		m_curTime ++;
		char buf[260] = {0};
		if (m_curTime>=3600)
		{
			unsigned long uhour = m_curTime / 3600;
			unsigned long tmpminutes = m_curTime-uhour*3600;
			unsigned long uminutes = tmpminutes / 60; 
			unsigned long useconds = tmpminutes-uminutes*60;
			sprintf_s(buf,259,"用时(大约):%02d时%02d分%02d秒",uhour,uminutes,useconds);
		}
		else if(m_curTime >= 60)
		{
			unsigned long uminutes = m_curTime / 60; 
			unsigned long useconds = m_curTime - uminutes*60;
			sprintf_s(buf,259,"用时(大约):%02d分%02d秒",uminutes,useconds);
		}
		else
		{
			sprintf_s(buf,259,"用时(大约):%02d秒",m_curTime);
		}
		SetDlgItemText(IDC_STATIC_TTIME,buf);

		LONGLONG tmp_last;
		if (m_pMediaSeeking)
		{
			if (SUCCEEDED(m_pMediaSeeking->GetCurrentPosition(&tmp_last)))
			{
				m_uLastMS = timeGetTime() - m_uStartMS;
				UINT tmp_cur = (UINT)(tmp_last/10000);
				if (fabs((float)(m_uLastMS-tmp_cur)) >m_uCanBear)
				{
					FILE* fp = fopen(m_strBear,"a+");
					if (fp)
					{
						fprintf_s(fp,"当前播放不流畅 : [%s],与定时器相差5秒\n",m_curSongItem.strMusicName.c_str());
						fclose(fp);
						if (m_pMediaControl)
						{
							m_pMediaControl->Stop();
						}
						::ReleaseSemaphore(m_hSemphore,1,NULL);//切歌
					}
				}
			}
		}
	}
		
	CDialog::OnTimer(nIDEvent);
}

void CDXdemoTestSMDlg::OnBnClickedBtestdaudio()
{
	if (g_bRunning)
	{
		return ;
	}

	DWORD threadID;
	m_curTime = 0;
	HANDLE hThread = ::CreateThread(NULL,0,&CDXdemoTestSMDlg::ThreadTestAudio,(LPVOID)this,0,&threadID);
	if (hThread)
	{
		CloseHandle(hThread);
	}
}

std::string GetFilePath(HWND hwnd,const char* filter)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	char  szBuffer[MAX_PATH];
	szBuffer[0] = NULL;

	ofn.lStructSize         = sizeof(OPENFILENAME);
	ofn.hwndOwner           = hwnd;
	ofn.hInstance           = NULL;
	ofn.lpstrFilter         = filter;
	ofn.nFilterIndex        = 1;
	ofn.lpstrCustomFilter   = NULL;
	ofn.nMaxCustFilter      = 0;
	ofn.lpstrFile           = szBuffer;
	ofn.nMaxFile            = MAX_PATH;
	ofn.lpstrFileTitle      = NULL;
	ofn.nMaxFileTitle       = 0;
	ofn.lpstrInitialDir     = NULL;
	ofn.lpstrTitle          = TEXT("Select a file ...");
	ofn.Flags               = OFN_HIDEREADONLY;
	ofn.nFileOffset         = 0;
	ofn.nFileExtension      = 0;
	ofn.lpstrDefExt         = TEXT("TXT");
	ofn.lCustData           = 0L;
	ofn.lpfnHook            = NULL;
	ofn.lpTemplateName  = NULL; 

	if (GetOpenFileName (&ofn))  // user specified a file
	{
		return std::string( szBuffer );
	}

	return "";
}

std::string GetDir(HWND hWnd)
{
	std::string sFolderPath;
	BROWSEINFO bi;
	char Buffer[MAX_PATH] = {0};
	//初始化入口参数bi开始
	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;//此参数如为NULL则不能显示对话框
	bi.lpszTitle = "选择歌库";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;
	//初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		sFolderPath = Buffer;
	}
	return sFolderPath;
}     

void CDXdemoTestSMDlg::OnBnClickedButton1()
{
	m_songFile = GetFilePath(GetSafeHwnd(),"TXT Files (.txt)\0*.TXT\0All Files (*.*)\0*.*\0\0");
	SetDlgItemText(IDC_EDIT_SONGINFO,m_songFile.c_str());
}

void CDXdemoTestSMDlg::OnBnClickedButton5()
{
	m_songDir = GetDir(GetSafeHwnd());
	SetDlgItemText(IDC_EDIT_SONG_DIR,m_songDir.c_str());
}

void CDXdemoTestSMDlg::OnBnClickedButton6()
{
	if (g_bRunning)
		return ;
	DWORD threadID;
	m_isFromDB = false;
	HANDLE hThread = ::CreateThread(NULL,0,&CDXdemoTestSMDlg::ThreadDb,(LPVOID)this,0,&threadID);
	if (hThread)
		CloseHandle(hThread);
}
