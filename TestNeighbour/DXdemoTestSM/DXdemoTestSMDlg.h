// DXdemoTestSMDlg.h : 头文件
//

#pragma once

#include <list>
#include <vector>
#include <set>
#include <string>
#include "progress/processcommunicate.h"
#include "progress/ShareMemoryThreadMgr.h"
#include "afxwin.h"
#include <tchar.h>
#include <comutil.h>
#include <MMSystem.h>
#include "afxcmn.h"


typedef struct __tMusicInfo
{
	int			nMusicId;
	std::wstring	strMusicName;
	std::wstring strSingerName;
	bool         bScore;
	bool			bAntiphonal;
} _tMusicInfo;

#define TIMER_RUNTIME 1000

typedef struct _SongList : public _tMusicInfo
{
	CStringW temp;
}Songlist;


typedef std::list<Songlist> LISTSONGALL;
typedef std::set<CStringW> SETSTRING;
// CDXdemoTestSMDlg 对话框
class CDXdemoTestSMDlg : public CDialog
{
// 构造
public:
	CDXdemoTestSMDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CDXdemoTestSMDlg();
// 对话框数据
	enum { IDD = IDD_DXDEMOTESTSM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON				   m_hIcon;
	Songlist			   m_curSongItem;
	unsigned int		   m_uCur;
	unsigned int		   m_uTotal;

	unsigned long	   m_curTime;
	int					   m_ncurValue;
	CShareMemoryThreadMgr m_smtMgr;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	std::string m_songFile;
	std::string m_songDir;
	bool m_isFromDB;
	bool CheckFile(const wchar_t* file);
public:
	afx_msg void OnBnClickedSwitchsong();
	afx_msg LRESULT OnEndSong(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnScoreStarNum(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedResing();
	afx_msg void OnBnClickedGrade();
	afx_msg void OnBnClickedAccompany();
	afx_msg void OnBnClickedClosePlayer();
	afx_msg void OnBnClickedMute();
	afx_msg void OnStnClickedFinalScore();
	afx_msg void OnBnClickedMaxVolume();
	afx_msg void OnBnClickedAddVolume();
	afx_msg void OnBnClickedSubVolume();
	afx_msg void OnBnClickedPublicNotify();
	CEdit m_ctrlMessage;
	afx_msg void OnBnClickedFirewarning();
	afx_msg void OnBnClickedResing2();
	afx_msg void OnBnClickedBapplaud();
	afx_msg void OnBnClickedBcheer();
	CEdit m_ctrlNextSongName;
	afx_msg void OnBnClickedBnextsong();
	afx_msg void OnBnClickedBplayPause();
	afx_msg void OnBnClickedPlayResume();
	afx_msg LRESULT OnPlayEnd(WPARAM wParam,LPARAM lParam);
private:
	int GetKTVMusicList( const char* path, std::list<_tMusicInfo> & lstMusicInfo);
	void  Ini_SetInt(const char *szIniFile,const char *section, const char *name, int value);
	int	 Ini_GetInt(const char *szIniFile,const char *section, const char *name, int def_val);
	static DWORD WINAPI ThreadTest(LPVOID pParam);
	static DWORD WINAPI ThreadTest1(LPVOID pParam);
	static DWORD WINAPI ThreadTestAudio(LPVOID pParam);
	static DWORD WINAPI ThreadDb(LPVOID pParam);
public:
	SETSTRING			m_setFiles;
	bool						m_bRunning;
	LISTSONGALL		m_songlist;
	HANDLE				m_hSemphore;
	UINT					m_uStartMS;
	UINT					m_uLastMS;
	UINT					m_uCanBear;
	CString				m_strBear;
	CComPtr<IGraphBuilder>					m_pGraph;
	CComPtr<IMediaControl>				m_pMediaControl;
	CComPtr<IMediaEventEx>				m_pMediaEventEx;
	CComPtr<IMediaSeeking>				m_pMediaSeeking;
public:
	afx_msg void OnBnClickedBnbTest();
	afx_msg void OnBnClickedTestneighborex();
	CEdit m_ctrlSongList;
	afx_msg void OnBnClickedBtestDbsql();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtestdaudio();
	CProgressCtrl m_progressDb;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
/*CAMThread*/
