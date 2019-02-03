// ParticleEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ParticleEdit.h"
#include "ParticleEditDlg.h"
#include "ParseFile.h"
#include "ximage/ximage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GetItemInt(v,id) \
v=::GetDlgItemInt(m_hWnd,id,NULL,FALSE)

#define GetItemFloat(v,id) \
char buf[260] = {0};\
::GetDlgItemTextA(m_hWnd,id,buf,259);\
v = atof(buf)

#define GetItemString(v,id) \
char buf[260] = {0};\
::GetDlgItemTextA(m_hWnd,id,buf,259);\
v=buf

char g_buf[260] = {0};

#define  SetItemFloat(value,id) \
sprintf_s(g_buf,259,"%.2f",value);\
::SetDlgItemText(m_hWnd,id,g_buf)

#define SetItemInt(value,id) \
::SetDlgItemInt(m_hWnd,id,value,TRUE)

#define SetItemString(value,id) \
::SetDlgItemText(m_hWnd,id,value);

#define GEnableWindow(id,enable) \
hWnd = ::GetDlgItem(m_hWnd,id); \
::EnableWindow(hWnd,enable)

#define SelectItem(id) \
HWND hWnd;\
hWnd = ::GetDlgItem(m_hWnd,id);\
::SendMessage(hWnd,BM_CLICK,0,0)
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CParticleEditDlg 对话框




CParticleEditDlg::CParticleEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParticleEditDlg::IDD, pParent)
	,m_fSysLifeTime(0.0f)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CParticleEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHCONTINUE, m_ctrlContinue);
	DDX_Control(pDX, IDC_CHECKONCE, m_ctrlOnce);
	DDX_Control(pDX, IDC_CHECKDR, m_ctrlDirectionR);
	DDX_Control(pDX, IDC_CHECKTWOEND, m_ctrlTwoE);
	DDX_Control(pDX, IDC_CHSPREAD, m_ctrlPSpread);
	DDX_Control(pDX, IDC_COMBORUNMODE, m_ctrlRunMode);
}

BEGIN_MESSAGE_MAP(CParticleEditDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHCONTINUE, &CParticleEditDlg::OnBnClickedChcontinue)
	ON_EN_CHANGE(IDC_ESYSLIFETIME, &CParticleEditDlg::OnEnChangeEsyslifetime)
	ON_EN_CHANGE(IDC_EEMISSION, &CParticleEditDlg::OnEnChangeEemission)
	ON_EN_CHANGE(IDC_ELIFEMIN, &CParticleEditDlg::OnEnChangeElifemin)
	ON_EN_CHANGE(IDC_ELIFEMAX, &CParticleEditDlg::OnEnChangeElifemax)
	ON_BN_CLICKED(IDC_RADIOADD, &CParticleEditDlg::OnBnClickedRadioadd)
	ON_BN_CLICKED(IDC_RADIOBLEND, &CParticleEditDlg::OnBnClickedRadioblend)
	ON_EN_CHANGE(IDC_ECENTERX, &CParticleEditDlg::OnEnChangeEcenterx)
	ON_EN_CHANGE(IDC_ECENTERY, &CParticleEditDlg::OnEnChangeEcentery)
	ON_EN_CHANGE(IDC_EDIRECTION, &CParticleEditDlg::OnEnChangeEdirection)
	ON_EN_CHANGE(IDC_ESPREAD, &CParticleEditDlg::OnEnChangeEspread)
	ON_EN_CHANGE(IDC_ESSPEEDMIN, &CParticleEditDlg::OnEnChangeEsspeedmin)
	ON_EN_CHANGE(IDC_ESSPEEDMAX, &CParticleEditDlg::OnEnChangeEsspeedmax)
	ON_EN_CHANGE(IDC_EGRAVITYMIN, &CParticleEditDlg::OnEnChangeEgravitymin)
	ON_EN_CHANGE(IDC_EGRAVITYMAX, &CParticleEditDlg::OnEnChangeEgravitymax)
	ON_EN_CHANGE(IDC_EXFORCEMIN, &CParticleEditDlg::OnEnChangeExforcemin)
	ON_EN_CHANGE(IDC_EXFORCEMAX, &CParticleEditDlg::OnEnChangeExforcemax)
	ON_EN_CHANGE(IDC_ERACCELERATIONMIN, &CParticleEditDlg::OnEnChangeEraccelerationmin)
	ON_EN_CHANGE(IDC_ERACCELERATIONMAX, &CParticleEditDlg::OnEnChangeEraccelerationmax)
	ON_EN_CHANGE(IDC_ETACCMIN, &CParticleEditDlg::OnEnChangeEtaccmin)
	ON_EN_CHANGE(IDC_ETACCMAX, &CParticleEditDlg::OnEnChangeEtaccmax)
	ON_BN_CLICKED(IDC_REMINTPOINT, &CParticleEditDlg::OnBnClickedRemintpoint)
	ON_BN_CLICKED(IDC_REMINTRECT, &CParticleEditDlg::OnBnClickedRemintrect)
	ON_BN_CLICKED(IDC_REMINTCIRCLE, &CParticleEditDlg::OnBnClickedRemintcircle)
	ON_BN_CLICKED(IDC_REMINTRING, &CParticleEditDlg::OnBnClickedRemintring)
	ON_BN_CLICKED(IDC_REMINTMASK, &CParticleEditDlg::OnBnClickedRemintmask)
	ON_EN_CHANGE(IDC_EX, &CParticleEditDlg::OnEnChangeEx)
	ON_EN_CHANGE(IDC_EY, &CParticleEditDlg::OnEnChangeEy)
	ON_EN_CHANGE(IDC_EWIDTH, &CParticleEditDlg::OnEnChangeEwidth)
	ON_EN_CHANGE(IDC_EHEIGHT, &CParticleEditDlg::OnEnChangeEheight)
	ON_EN_CHANGE(IDC_EINA, &CParticleEditDlg::OnEnChangeEina)
	ON_EN_CHANGE(IDC_EINB, &CParticleEditDlg::OnEnChangeEinb)
	ON_EN_CHANGE(IDC_EOUTA, &CParticleEditDlg::OnEnChangeEouta)
	ON_EN_CHANGE(IDC_EOUTB, &CParticleEditDlg::OnEnChangeEoutb)
	ON_BN_CLICKED(IDC_CHSPREAD, &CParticleEditDlg::OnBnClickedChspread)
	ON_EN_CHANGE(IDC_EPSSIZE, &CParticleEditDlg::OnEnChangeEpssize)
	ON_EN_CHANGE(IDC_EPESIZE, &CParticleEditDlg::OnEnChangeEpesize)
	ON_EN_CHANGE(IDC_EPSIZEVAR, &CParticleEditDlg::OnEnChangeEpsizevar)
	ON_EN_CHANGE(IDC_EPSSPIN, &CParticleEditDlg::OnEnChangeEpsspin)
	ON_EN_CHANGE(IDC_EPESPIN, &CParticleEditDlg::OnEnChangeEpespin)
	ON_EN_CHANGE(IDC_EPSPINVAR, &CParticleEditDlg::OnEnChangeEpspinvar)
	ON_EN_CHANGE(IDC_EPSALPHA, &CParticleEditDlg::OnEnChangeEpsalpha)
	ON_EN_CHANGE(IDC_EPEALPHA, &CParticleEditDlg::OnEnChangeEpealpha)
	ON_EN_CHANGE(IDC_EPALPHAVAR, &CParticleEditDlg::OnEnChangeEpalphavar)
	ON_EN_CHANGE(IDC_EPSR, &CParticleEditDlg::OnEnChangeEpsr)
	ON_EN_CHANGE(IDC_EPSG, &CParticleEditDlg::OnEnChangeEpsg)
	ON_EN_CHANGE(IDC_EPSB, &CParticleEditDlg::OnEnChangeEpsb)
	ON_EN_CHANGE(IDC_EPER, &CParticleEditDlg::OnEnChangeEper)
	ON_EN_CHANGE(IDC_EPEG, &CParticleEditDlg::OnEnChangeEpeg)
	ON_EN_CHANGE(IDC_EPEB, &CParticleEditDlg::OnEnChangeEpeb)
	ON_EN_CHANGE(IDC_EPCOLORVAR, &CParticleEditDlg::OnEnChangeEpcolorvar)
	ON_BN_CLICKED(IDC_BFINDTEXFILE, &CParticleEditDlg::OnBnClickedBfindtexfile)
	ON_BN_CLICKED(IDC_BFINDMASK, &CParticleEditDlg::OnBnClickedBfindmask)
	ON_EN_CHANGE(IDC_ETEXTUREP, &CParticleEditDlg::OnEnChangeEtexturep)
	ON_EN_CHANGE(IDC_EMASKFILE, &CParticleEditDlg::OnEnChangeEmaskfile)
	ON_BN_CLICKED(IDC_CHECKONCE, &CParticleEditDlg::OnBnClickedCheckonce)
	ON_BN_CLICKED(IDC_CHECKDR, &CParticleEditDlg::OnBnClickedCheckdr)
	ON_BN_CLICKED(IDC_CHECKTWOEND, &CParticleEditDlg::OnBnClickedChecktwoend)
	ON_BN_CLICKED(IDC_RADIOSHADE, &CParticleEditDlg::OnBnClickedRadioshade)
	ON_BN_CLICKED(IDC_RADIOOSCILLATE, &CParticleEditDlg::OnBnClickedRadiooscillate)
	ON_EN_CHANGE(IDC_EDITSTEPALPHA, &CParticleEditDlg::OnEnChangeEditstepalpha)
	ON_EN_CHANGE(IDC_EDITSTEPR, &CParticleEditDlg::OnEnChangeEditstepr)
	ON_EN_CHANGE(IDC_EDITSTEPG, &CParticleEditDlg::OnEnChangeEditstepg)
	ON_EN_CHANGE(IDC_EDITSTEPB, &CParticleEditDlg::OnEnChangeEditstepb)
	ON_EN_CHANGE(IDC_EDITSTEPVAR, &CParticleEditDlg::OnEnChangeEditstepvar)
	ON_BN_CLICKED(IDC_BUTTONLOADPLIST, &CParticleEditDlg::OnBnClickedButtonloadplist)
	ON_EN_CHANGE(IDC_EDITSTEPALPHAVAR, &CParticleEditDlg::OnEnChangeEditstepalphavar)
	ON_EN_CHANGE(IDC_EPSIZEVAR2, &CParticleEditDlg::OnEnChangeEpsizevar2)
	ON_EN_CHANGE(IDC_EPSPINVAR2, &CParticleEditDlg::OnEnChangeEpspinvar2)
	ON_EN_CHANGE(IDC_EDITSRADIUS, &CParticleEditDlg::OnEnChangeEditsradius)
	ON_EN_CHANGE(IDC_EDITSRADIUSVAR, &CParticleEditDlg::OnEnChangeEditsradiusvar)
	ON_EN_CHANGE(IDC_EDITERADIUS, &CParticleEditDlg::OnEnChangeEditeradius)
	ON_EN_CHANGE(IDC_EDITERADIUSVAR, &CParticleEditDlg::OnEnChangeEditeradiusvar)
	ON_EN_CHANGE(IDC_EDITROTATE, &CParticleEditDlg::OnEnChangeEditrotate)
	ON_EN_CHANGE(IDC_EDITROTATEVAR, &CParticleEditDlg::OnEnChangeEditrotatevar)
	ON_CBN_SELCHANGE(IDC_COMBORUNMODE, &CParticleEditDlg::OnCbnSelchangeComborunmode)
END_MESSAGE_MAP()


// CParticleEditDlg 消息处理程序
#define FORCE_MODE 0
#define RADIUS_MODE 1
BOOL CParticleEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	MAPKEYVALUE mapKey;
	bool b = CParseFile::ParseFilePlist("C:\\Users\\Administrator\\Desktop\\11.plist",mapKey);
	structParticleSystemInfo info = {0};
	CParseFile::ChangeToPsi(mapKey,info);

	m_ctrlRunMode.AddString("力运动模式");//FORCE_MODE
	m_ctrlRunMode.AddString("半径运动模式");//RADIUS_MODE

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CParticleEditDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CParticleEditDlg::OnPaint()
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
HCURSOR CParticleEditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
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
	ofn.lpstrTitle          = TEXT("Select a picture file ...");
	ofn.Flags               = OFN_HIDEREADONLY;
	ofn.nFileOffset         = 0;
	ofn.nFileExtension      = 0;
	ofn.lpstrDefExt         = TEXT("PNG");
	ofn.lCustData           = 0L;
	ofn.lpfnHook            = NULL;
	ofn.lpTemplateName  = NULL; 

	if (GetOpenFileName (&ofn))  // user specified a file
	{
		return std::string( szBuffer );
	}

	return "";
}
#define TEXT_B_ALPHA \
bOscillate?"粒子最小Alpha":"粒子起始Alpha"
#define TEXT_E_ALPHA \
bOscillate?"粒子最大Alpha":"粒子结束Alpha"
#define TEXT_B_RED \
bOscillate?"粒子最小R":"粒子起始R"
#define TEXT_B_GREEN \
bOscillate?"粒子最小G":"粒子起始G"
#define TEXT_B_BLUE \
bOscillate?"粒子最小B":"粒子起始B"
#define TEXT_E_RED \
bOscillate?"粒子最大R":"粒子结束R"
#define TEXT_E_GREEN \
bOscillate?"粒子最大G":"粒子结束G"
#define TEXT_E_BLUE \
bOscillate?"粒子最大B":"粒子结束B"

void CParticleEditDlg::SetRunMode()
{
	HWND hWnd;
	BOOL b = !! state.ps[state.nPreset]->m_info.bRadiusMode;
	BOOL _b = ! b;
	GEnableWindow(IDC_ESSPEEDMIN,_b);
	GEnableWindow(IDC_ESSPEEDMAX,_b);
	GEnableWindow(IDC_EGRAVITYMIN,_b);
	GEnableWindow(IDC_EGRAVITYMAX,_b);
	GEnableWindow(IDC_EXFORCEMIN,_b);
	GEnableWindow(IDC_EXFORCEMAX,_b);
	GEnableWindow(IDC_ERACCELERATIONMIN,_b);
	GEnableWindow(IDC_ERACCELERATIONMAX,_b);
	GEnableWindow(IDC_ETACCMIN,_b);
	GEnableWindow(IDC_ETACCMAX,_b);

	GEnableWindow(IDC_REMINTPOINT,_b);
	GEnableWindow(IDC_REMINTRECT,_b);
	GEnableWindow(IDC_REMINTCIRCLE,_b);
	GEnableWindow(IDC_REMINTRING,_b);
	GEnableWindow(IDC_REMINTMASK,_b);
	GEnableWindow(IDC_CHSPREAD,_b);
	GEnableWindow(IDC_EX,_b);
	GEnableWindow(IDC_EY,_b);
	GEnableWindow(IDC_EWIDTH,_b);
	GEnableWindow(IDC_EHEIGHT,_b);
	GEnableWindow(IDC_EINA,_b);
	GEnableWindow(IDC_EINB,_b);
	GEnableWindow(IDC_EOUTA,_b);
	GEnableWindow(IDC_EOUTB,_b);

	GEnableWindow(IDC_EDITSRADIUS,b);
	GEnableWindow(IDC_EDITSRADIUSVAR,b);
	GEnableWindow(IDC_EDITERADIUS,b);
	GEnableWindow(IDC_EDITERADIUSVAR,b);
	GEnableWindow(IDC_EDITROTATE,b);
	GEnableWindow(IDC_EDITROTATEVAR,b);
}
void CParticleEditDlg::SetColorText()
{
	BOOL bOscillate = !! state.ps[state.nPreset]->m_info.bColorOscillate;
	HWND hWnd;

	if(bOscillate)
	{
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorMin.a,IDC_EPSALPHA);
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorMax.a,IDC_EPEALPHA);

		SetItemFloat(state.ps[state.nPreset]->m_info.colColorMin.r,IDC_EPSR);
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorMin.g,IDC_EPSG);
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorMin.b,IDC_EPSB);
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorMax.r,IDC_EPER);
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorMax.g,IDC_EPEG);
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorMax.b,IDC_EPEB);
	}
	else
	{
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorStart.a,IDC_EPSALPHA);
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorEnd.a,IDC_EPEALPHA);

		SetItemFloat(state.ps[state.nPreset]->m_info.colColorStart.r,IDC_EPSR);
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorStart.g,IDC_EPSG);
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorStart.b,IDC_EPSB);
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorEnd.r,IDC_EPER);
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorEnd.g,IDC_EPEG);
		SetItemFloat(state.ps[state.nPreset]->m_info.colColorEnd.b,IDC_EPEB);
	}
	SetItemString(TEXT_B_ALPHA,IDC_STATICBALPHA);
	SetItemString(TEXT_E_ALPHA,IDC_STATICEALPHA);
	SetItemString(TEXT_B_RED,IDC_STATICBRED);
	SetItemString(TEXT_E_RED,IDC_STATICERED);
	SetItemString(TEXT_B_GREEN,IDC_STATICBGREEN);
	SetItemString(TEXT_E_GREEN,IDC_STATICEGREEN);
	SetItemString(TEXT_B_BLUE,IDC_STATICBBLUE);
	SetItemString(TEXT_E_BLUE,IDC_STATICEBLUE);

	GEnableWindow(IDC_EDITSTEPALPHA,bOscillate);
	GEnableWindow(IDC_EDITSTEPALPHAVAR,bOscillate);
	GEnableWindow(IDC_EDITSTEPR,bOscillate);
	GEnableWindow(IDC_EDITSTEPG,bOscillate);
	GEnableWindow(IDC_EDITSTEPB,bOscillate);
	GEnableWindow(IDC_EDITSTEPVAR,bOscillate);
}

void CParticleEditDlg::SetGui(const char* dir)
{
	m_fSysLifeTime = state.ps[state.nPreset]->m_info.fLifetime;
	if (m_fSysLifeTime+1.0f < 0.001f)
	{
		//m_fSysLifeTime = 5.0f;
		SetItemFloat(m_fSysLifeTime,IDC_ESYSLIFETIME);
		m_ctrlContinue.SetCheck(1);
		state.ps[state.nPreset]->Fire();
	}
	else
	{
		SetItemFloat(m_fSysLifeTime,IDC_ESYSLIFETIME);
		m_ctrlContinue.SetCheck(0);
	}

	m_ctrlOnce.SetCheck(state.ps[state.nPreset]->m_info.bOneShot);
	SetItemInt(state.ps[state.nPreset]->m_info.nEmission,IDC_EEMISSION);
	SetItemFloat(state.ps[state.nPreset]->m_info.fParticleLifeMin,IDC_ELIFEMIN);
	SetItemFloat(state.ps[state.nPreset]->m_info.fParticleLifeMax,IDC_ELIFEMAX);
	// 	m_sTextureFile = GetFilePath();
	// 	SetDlgItemText(IDC_ETEXTUREP,m_sTextureFile.c_str());
	if(!gSprParticle[state.nPreset])
	{
		SelectItem(IDC_RADIOADD);
	}
	else
	{
		if(gSprParticle[state.nPreset]->GetBlendMode() & BLEND_ALPHABLEND)
		{
			SelectItem(IDC_RADIOBLEND);
		}
		else
		{
			SelectItem(IDC_RADIOADD);
		}
	}

	float fSpread = state.ps[state.nPreset]->m_info.fDirection * 180.0f/M_PI;
	SetItemFloat(fSpread,IDC_EDIRECTION);
	m_ctrlDirectionR.SetCheck(state.ps[state.nPreset]->m_info.bRelative);
	fSpread = state.ps[state.nPreset]->m_info.fSpread *180.f/M_PI;
	SetItemFloat(fSpread,IDC_ESPREAD);

	//力运动模式
	m_ctrlTwoE.SetCheck(state.ps[state.nPreset]->m_info.btwoEnds);
	SetItemFloat(state.ps[state.nPreset]->m_info.fSpeedMin,IDC_ESSPEEDMIN);
	SetItemFloat(state.ps[state.nPreset]->m_info.fSpeedMax,IDC_ESSPEEDMAX);
	SetItemFloat(state.ps[state.nPreset]->m_info.fForceMin.y,IDC_EGRAVITYMIN);
	SetItemFloat(state.ps[state.nPreset]->m_info.fForceMax.y,IDC_EGRAVITYMAX);
	SetItemFloat(state.ps[state.nPreset]->m_info.fForceMin.x,IDC_EXFORCEMIN);
	SetItemFloat(state.ps[state.nPreset]->m_info.fForceMax.x,IDC_EXFORCEMAX);
	SetItemFloat(state.ps[state.nPreset]->m_info.fRadialAccelMin,IDC_ERACCELERATIONMIN);
	SetItemFloat(state.ps[state.nPreset]->m_info.fRadialAccelMax,IDC_ERACCELERATIONMAX);
	SetItemFloat(state.ps[state.nPreset]->m_info.fTangentialAccelMin,IDC_ETACCMIN);
	SetItemFloat(state.ps[state.nPreset]->m_info.fTangentialAccelMax,IDC_ETACCMAX);
	if (state.ps[state.nPreset]->m_info.nEmitter == EMITTER_POINT)
	{
		SelectItem(IDC_REMINTPOINT);
	}
	else if (state.ps[state.nPreset]->m_info.nEmitter == EMITTER_RECT)
	{
		SelectItem(IDC_REMINTRECT);
	}
	else if (state.ps[state.nPreset]->m_info.nEmitter == EMITTER_CIRCLE)
	{
		SelectItem(IDC_REMINTCIRCLE);
	}
	else if (state.ps[state.nPreset]->m_info.nEmitter == EMITTER_RING)
	{
		SelectItem(IDC_REMINTRING);
	}
	else
	{
		SelectItem(IDC_REMINTMASK);
	}

	SetItemFloat(state.ps[state.nPreset]->m_info.pointCreateRect.origin.x,IDC_EX);
	SetItemFloat(state.ps[state.nPreset]->m_info.pointCreateRect.origin.y,IDC_EY);
	SetItemInt(state.ps[state.nPreset]->m_info.pointCreateRect.size.width,IDC_EWIDTH);
	SetItemInt(state.ps[state.nPreset]->m_info.pointCreateRect.size.height,IDC_EHEIGHT);
	SetItemFloat(state.ps[state.nPreset]->m_info.circleInsideA,IDC_EINA);
	SetItemFloat(state.ps[state.nPreset]->m_info.circleInsideB,IDC_EINB);
	SetItemFloat(state.ps[state.nPreset]->m_info.circleOutsideA,IDC_EOUTA);
	SetItemFloat(state.ps[state.nPreset]->m_info.circleOutsideB,IDC_EOUTB);
	m_ctrlPSpread.SetCheck(state.ps[state.nPreset]->m_info.bSpread);

	//半径运动模型
	SetItemFloat(state.ps[state.nPreset]->m_info.fRadiusStart,IDC_EDITSRADIUS);
	SetItemFloat(state.ps[state.nPreset]->m_info.fRadiusStartVar,IDC_EDITSRADIUSVAR);
	SetItemFloat(state.ps[state.nPreset]->m_info.fRadiusEnd,IDC_EDITERADIUS);
	SetItemFloat(state.ps[state.nPreset]->m_info.fRadiusEndVar,IDC_EDITERADIUSVAR);
	SetItemFloat(state.ps[state.nPreset]->m_info.fRotatePerSecond,IDC_EDITROTATE);
	SetItemFloat(state.ps[state.nPreset]->m_info.fRotatePerSecondVar,IDC_EDITROTATEVAR);

	//粒子其他属性
	SetItemFloat(state.ps[state.nPreset]->m_info.fOriginX,IDC_ECENTERX);
	SetItemFloat(state.ps[state.nPreset]->m_info.fOriginY,IDC_ECENTERY);
	SetItemFloat(state.ps[state.nPreset]->m_info.fSizeStart,IDC_EPSSIZE);
	SetItemFloat(state.ps[state.nPreset]->m_info.fSizeEnd,IDC_EPESIZE);
	SetItemFloat(state.ps[state.nPreset]->m_info.fSizeVarStart,IDC_EPSIZEVAR);
	SetItemFloat(state.ps[state.nPreset]->m_info.fSizeVarEnd,IDC_EPSIZEVAR2);
	SetItemFloat(state.ps[state.nPreset]->m_info.fSpinStart,IDC_EPSSPIN);
	SetItemFloat(state.ps[state.nPreset]->m_info.fSpinEnd,IDC_EPESPIN);
	SetItemFloat(state.ps[state.nPreset]->m_info.fSpinVarStart,IDC_EPSPINVAR);
	SetItemFloat(state.ps[state.nPreset]->m_info.fSpinVarEnd,IDC_EPSPINVAR2);

	SetItemFloat(state.ps[state.nPreset]->m_info.fAlphaVar,IDC_EPALPHAVAR);
	SetItemFloat(state.ps[state.nPreset]->m_info.fColorVar,IDC_EPCOLORVAR);

	SetItemString(state.ps[state.nPreset]->m_info.textureFile,IDC_ETEXTUREP);
	SetItemString(state.ps[state.nPreset]->m_info.maskFile,IDC_EMASKFILE);
	SetItemString((dir?dir:""),IDC_S_TEXTUREFILE_DIR);

	SetItemFloat(state.ps[state.nPreset]->m_info.colColorStep.a,IDC_EDITSTEPALPHA);
	SetItemFloat(state.ps[state.nPreset]->m_info.fStepAlphaVar,IDC_EDITSTEPALPHAVAR);
	SetItemFloat(state.ps[state.nPreset]->m_info.colColorStep.r,IDC_EDITSTEPR);
	SetItemFloat(state.ps[state.nPreset]->m_info.colColorStep.g,IDC_EDITSTEPG);
	SetItemFloat(state.ps[state.nPreset]->m_info.colColorStep.b,IDC_EDITSTEPB);
	SetItemFloat(state.ps[state.nPreset]->m_info.fStepColorVar,IDC_EDITSTEPVAR);

	if(state.ps[state.nPreset]->m_info.bColorOscillate)
	{
		SelectItem(IDC_RADIOOSCILLATE);
	}
	else
	{
		SelectItem(IDC_RADIOSHADE);
	}
	//SetColorText();
	if(state.ps[state.nPreset]->m_info.bRadiusMode)
		m_ctrlRunMode.SetCurSel(RADIUS_MODE);
	else
		m_ctrlRunMode.SetCurSel(FORCE_MODE);
	SetRunMode();
}

void CParticleEditDlg::OnBnClickedChcontinue()
{
	state.ps[state.nPreset]->Stop(true);
	if (m_ctrlContinue.GetCheck())
	{
		state.ps[state.nPreset]->m_info.fLifetime=-1.0f;
		state.ps[state.nPreset]->Fire();
	}
	else
	{
		state.ps[state.nPreset]->m_info.fLifetime=m_fSysLifeTime;
		state.ps[state.nPreset]->Fire();
	}
}

void CParticleEditDlg::OnEnChangeEsyslifetime()
{
	GetItemFloat(m_fSysLifeTime,IDC_ESYSLIFETIME);
	state.ps[state.nPreset]->m_info.fLifetime = m_fSysLifeTime;
	m_ctrlContinue.SetCheck(0);
	state.ps[state.nPreset]->Stop();
}

void CParticleEditDlg::OnEnChangeEemission()
{
	GetItemInt(state.ps[state.nPreset]->m_info.nEmission,IDC_EEMISSION);
}

void CParticleEditDlg::OnEnChangeElifemin()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fParticleLifeMin,IDC_ELIFEMIN);
}

void CParticleEditDlg::OnEnChangeElifemax()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fParticleLifeMax,IDC_ELIFEMAX);
}

void CParticleEditDlg::OnBnClickedRadioadd()
{
	state.ps[state.nPreset]->m_info.sprite->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_ZWRITE);
}

void CParticleEditDlg::OnBnClickedRadioblend()
{
	state.ps[state.nPreset]->m_info.sprite->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE);
}

void CParticleEditDlg::OnEnChangeEcenterx()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fOriginX,IDC_ECENTERX);
}

void CParticleEditDlg::OnEnChangeEcentery()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fOriginY,IDC_ECENTERY);
}

void CParticleEditDlg::OnEnChangeEdirection()
{
	float fDirection;
	GetItemFloat(fDirection,IDC_EDIRECTION);
	state.ps[state.nPreset]->m_info.fDirection = fDirection / 180.f * M_PI;
}

void CParticleEditDlg::OnEnChangeEspread()
{
	float fSpread;
	GetItemFloat(fSpread,IDC_ESPREAD);
	fSpread = fSpread<0.0f?-fSpread:fSpread;
	state.ps[state.nPreset]->m_info.fSpread = fSpread / 180.f * M_PI;
}

void CParticleEditDlg::OnEnChangeEsspeedmin()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fSpeedMin,IDC_ESSPEEDMIN);
}

void CParticleEditDlg::OnEnChangeEsspeedmax()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fSpeedMax,IDC_ESSPEEDMAX);
}

void CParticleEditDlg::OnEnChangeEgravitymin()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fForceMin.y,IDC_EGRAVITYMIN);
}

void CParticleEditDlg::OnEnChangeEgravitymax()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fForceMax.y,IDC_EGRAVITYMAX);
}

void CParticleEditDlg::OnEnChangeExforcemin()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fForceMin.x,IDC_EXFORCEMIN);
}

void CParticleEditDlg::OnEnChangeExforcemax()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fForceMax.x,IDC_EXFORCEMAX);
}

void CParticleEditDlg::OnEnChangeEraccelerationmin()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fRadialAccelMin,IDC_ERACCELERATIONMIN);
}

void CParticleEditDlg::OnEnChangeEraccelerationmax()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fRadialAccelMax,IDC_ERACCELERATIONMAX);
}

void CParticleEditDlg::OnEnChangeEtaccmin()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fTangentialAccelMin,IDC_ETACCMIN);
}

void CParticleEditDlg::OnEnChangeEtaccmax()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fTangentialAccelMax,IDC_ETACCMAX);
}

void CParticleEditDlg::OnBnClickedRemintpoint()
{
	state.ps[state.nPreset]->m_info.nEmitter = EMITTER_POINT;

	HWND hWnd = NULL;
	GEnableWindow(IDC_EX,FALSE);
	GEnableWindow(IDC_EY,FALSE);
	GEnableWindow(IDC_EWIDTH,FALSE);
	GEnableWindow(IDC_EHEIGHT,FALSE);
	GEnableWindow(IDC_EINA,FALSE);
	GEnableWindow(IDC_EINB,FALSE);
	GEnableWindow(IDC_EOUTA,FALSE);
	GEnableWindow(IDC_EOUTB,FALSE);
}

void CParticleEditDlg::OnBnClickedRemintrect()
{
	state.ps[state.nPreset]->m_info.nEmitter = EMITTER_RECT;

	HWND hWnd = NULL;
	GEnableWindow(IDC_EX,TRUE);
	GEnableWindow(IDC_EY,TRUE);
	GEnableWindow(IDC_EWIDTH,TRUE);
	GEnableWindow(IDC_EHEIGHT,TRUE);
	GEnableWindow(IDC_EINA,FALSE);
	GEnableWindow(IDC_EINB,FALSE);
	GEnableWindow(IDC_EOUTA,FALSE);
	GEnableWindow(IDC_EOUTB,FALSE);
}

void CParticleEditDlg::OnBnClickedRemintcircle()
{
	state.ps[state.nPreset]->m_info.nEmitter = EMITTER_CIRCLE;

	HWND hWnd = NULL;
	GEnableWindow(IDC_EX,FALSE);
	GEnableWindow(IDC_EY,FALSE);
	GEnableWindow(IDC_EWIDTH,FALSE);
	GEnableWindow(IDC_EHEIGHT,FALSE);
	GEnableWindow(IDC_EINA,TRUE);
	GEnableWindow(IDC_EINB,TRUE);
	GEnableWindow(IDC_EOUTA,FALSE);
	GEnableWindow(IDC_EOUTB,FALSE);
}

void CParticleEditDlg::OnBnClickedRemintring()
{
	state.ps[state.nPreset]->m_info.nEmitter = EMITTER_RING;

	HWND hWnd = NULL;
	GEnableWindow(IDC_EX,FALSE);
	GEnableWindow(IDC_EY,FALSE);
	GEnableWindow(IDC_EWIDTH,FALSE);
	GEnableWindow(IDC_EHEIGHT,FALSE);
	GEnableWindow(IDC_EINA,TRUE);
	GEnableWindow(IDC_EINB,TRUE);
	GEnableWindow(IDC_EOUTA,TRUE);
	GEnableWindow(IDC_EOUTB,TRUE);
}

void CParticleEditDlg::OnBnClickedRemintmask()
{
	state.ps[state.nPreset]->m_info.nEmitter = EMITTER_MASK;

	HWND hWnd = NULL;
	GEnableWindow(IDC_EX,TRUE);
	GEnableWindow(IDC_EY,TRUE);
	GEnableWindow(IDC_EWIDTH,FALSE);
	GEnableWindow(IDC_EHEIGHT,FALSE);
	GEnableWindow(IDC_EINA,FALSE);
	GEnableWindow(IDC_EINB,FALSE);
	GEnableWindow(IDC_EOUTA,FALSE);
	GEnableWindow(IDC_EOUTB,FALSE);
}

void CParticleEditDlg::OnEnChangeEx()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.pointCreateRect.origin.x,IDC_EX);
}

void CParticleEditDlg::OnEnChangeEy()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.pointCreateRect.origin.y,IDC_EY);
}

void CParticleEditDlg::OnEnChangeEwidth()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.pointCreateRect.size.width,IDC_EWIDTH);
}

void CParticleEditDlg::OnEnChangeEheight()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.pointCreateRect.size.height,IDC_EHEIGHT);
}

void CParticleEditDlg::OnEnChangeEina()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.circleInsideA,IDC_EINA);
}

void CParticleEditDlg::OnEnChangeEinb()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.circleInsideB,IDC_EINB);
}

void CParticleEditDlg::OnEnChangeEouta()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.circleOutsideA,IDC_EOUTA);
}

void CParticleEditDlg::OnEnChangeEoutb()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.circleOutsideB,IDC_EOUTB);
}

void CParticleEditDlg::OnBnClickedChspread()
{
	state.ps[state.nPreset]->m_info.bSpread = m_ctrlPSpread.GetCheck();
}

void CParticleEditDlg::OnEnChangeEpssize()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fSizeStart,IDC_EPSSIZE);
}

void CParticleEditDlg::OnEnChangeEpesize()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fSizeEnd,IDC_EPESIZE);
}

void CParticleEditDlg::OnEnChangeEpsizevar()
{
	float fVal;
	GetItemFloat(fVal,IDC_EPSIZEVAR);
	state.ps[state.nPreset]->m_info.fSizeVarStart = fVal<0.0f?-fVal:fVal;
}

void CParticleEditDlg::OnEnChangeEpsspin()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fSpinStart,IDC_EPSSPIN);
}

void CParticleEditDlg::OnEnChangeEpespin()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fSpinEnd,IDC_EPESPIN);
}

void CParticleEditDlg::OnEnChangeEpspinvar()
{
	float fVal;
	GetItemFloat(fVal,IDC_EPSPINVAR);
	state.ps[state.nPreset]->m_info.fSpinVarStart = fVal<0.0f?-fVal:fVal;
}

void CParticleEditDlg::OnEnChangeEpsalpha()
{
	if(state.ps[state.nPreset]->m_info.bColorOscillate)
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorMin.a,IDC_EPSALPHA);
	}
	else
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorStart.a,IDC_EPSALPHA);
	}
}

void CParticleEditDlg::OnEnChangeEpealpha()
{
	if(state.ps[state.nPreset]->m_info.bColorOscillate)
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorMax.a,IDC_EPEALPHA);
	}
	else
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorEnd.a,IDC_EPEALPHA);
	}
}

void CParticleEditDlg::OnEnChangeEpalphavar()
{
	float fVal;
	GetItemFloat(fVal,IDC_EPALPHAVAR);
	state.ps[state.nPreset]->m_info.fAlphaVar = fVal<0.0f?-fVal:fVal;
}

void CParticleEditDlg::OnEnChangeEpsr()
{
	if(state.ps[state.nPreset]->m_info.bColorOscillate)
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorMin.r,IDC_EPSR);
	}
	else
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorStart.r,IDC_EPSR);
	}
}

void CParticleEditDlg::OnEnChangeEpsg()
{
	if(state.ps[state.nPreset]->m_info.bColorOscillate)
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorMin.g,IDC_EPSG);
	}
	else
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorStart.g,IDC_EPSG);
	}
}

void CParticleEditDlg::OnEnChangeEpsb()
{
	if(state.ps[state.nPreset]->m_info.bColorOscillate)
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorMin.b,IDC_EPSB);
	}
	else
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorStart.b,IDC_EPSB);
	}
}

void CParticleEditDlg::OnEnChangeEper()
{
	if(state.ps[state.nPreset]->m_info.bColorOscillate)
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorMax.r,IDC_EPER);
	}
	else
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorEnd.r,IDC_EPER);
	}
}

void CParticleEditDlg::OnEnChangeEpeg()
{
	if(state.ps[state.nPreset]->m_info.bColorOscillate)
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorMax.g,IDC_EPEG);
	}
	else
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorEnd.g,IDC_EPEG);
	}
}

void CParticleEditDlg::OnEnChangeEpeb()
{
	if(state.ps[state.nPreset]->m_info.bColorOscillate)
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorMax.b,IDC_EPEB);
	}
	else
	{
		GetItemFloat(state.ps[state.nPreset]->m_info.colColorEnd.b,IDC_EPEB);
	}
}

void CParticleEditDlg::OnEnChangeEpcolorvar()
{
	float fVal;
	GetItemFloat(fVal,IDC_EPCOLORVAR);
	state.ps[state.nPreset]->m_info.fColorVar = fVal<0.0f?-fVal:fVal;
}

void CParticleEditDlg::OnBnClickedBfindtexfile()
{
	std::string tmp = GetFilePath(m_hWnd,"PNG Files (.png)\0*.PNG\0All Files (*.*)\0*.*\0\0");
	if(tmp == "")
		return ;
	size_t pos = tmp.rfind('\\');
	std::string sTextureFile;
	if(pos == std::string::npos)
	{
		m_sTextureCurDir = ".";
		sTextureFile = tmp;
	}
	else
	{
		m_sTextureCurDir = tmp.substr(0,pos);
		sTextureFile = tmp.substr(pos+1,-1);
	}
	if(strstr(sTextureFile.c_str(),".png") && strcmp(sTextureFile.c_str(),m_sTextureFile.c_str()))
	{
		m_sTextureFile = sTextureFile;
		std::string file = m_sTextureCurDir+"\\"+m_sTextureFile;

		{
			CAutoLock lock(&g_csLock);
			state.ps[state.nPreset]->m_info.sprite = NULL;

		}
		if(gTexParticle[state.nPreset])
		{
			hge->Texture_Free(gTexParticle[state.nPreset]);
			gTexParticle[state.nPreset] = NULL;
		}
		unsigned int w;
		unsigned int h;
		gTexParticle[state.nPreset] = hge->Texture_Load(file.c_str(),w,h);
		if(gTexParticle[state.nPreset])
		{
			CGLSprite* pSprite = new CGLSprite(gTexParticle[state.nPreset],0,0,w,h);
			if(!pSprite)
				return ;
			pSprite->SetHotSpot(w/2,h/2);
			if(gSprParticle[state.nPreset])
			{
				delete gSprParticle[state.nPreset];
				gSprParticle[state.nPreset] = NULL;
			}
			gSprParticle[state.nPreset] = pSprite;
			state.ps[state.nPreset]->m_info.sprite = gSprParticle[state.nPreset];
		}
	}
	strcpy(state.ps[state.nPreset]->m_info.textureFile,sTextureFile.c_str());
	SetDlgItemText(IDC_S_TEXTUREFILE_DIR,m_sTextureCurDir.c_str());
	SetDlgItemText(IDC_ETEXTUREP,sTextureFile.c_str());
	//OnEnChangeEtexturep();
}

void CParticleEditDlg::OnBnClickedBfindmask()
{
	return ;
	std::string tmp = GetFilePath(m_hWnd,"PNG Files (.png)\0*.PNG\0All Files (*.*)\0*.*\0\0");
	if(tmp == "")
		return ;
	size_t pos = tmp.rfind('\\');
	std::string sMaskFile;
	if(pos == std::string::npos)
	{
		m_sTextureCurDir = ".";
		sMaskFile = tmp;
	}
	else
	{
		m_sTextureCurDir = tmp.substr(0,pos);
		sMaskFile = tmp.substr(pos+1,-1);
	}

	strcpy(state.ps[state.nPreset]->m_info.maskFile,sMaskFile.c_str());
	SetDlgItemText(IDC_S_TEXTUREFILE_DIR,m_sTextureCurDir.c_str());
	SetDlgItemText(IDC_EMASKFILE,sMaskFile.c_str());
	//OnEnChangeEmaskfile();
}

void CParticleEditDlg::OnEnChangeEtexturep()
{
	return ;
	std::string texFile;
	GetItemString(texFile,IDC_ETEXTUREP);
	if(strstr(texFile.c_str(),".png") && strcmp(texFile.c_str(),m_sTextureFile.c_str()))
	{
		m_sTextureFile = texFile;
		std::string file = m_sTextureCurDir+"\\"+m_sTextureFile;
		CxImage ximg;

		if(gTexParticle[state.nPreset])
		{
			hge->Texture_Free(gTexParticle[state.nPreset]);
			gTexParticle[state.nPreset] = NULL;
		}
		unsigned int w;
		unsigned int h;
		gTexParticle[state.nPreset] = hge->Texture_Load(file.c_str(),w,h);
		if(gTexParticle[state.nPreset])
		{

			CGLSprite* pSprite = new CGLSprite(gTexParticle[state.nPreset],0,0,w,h);
			if(!pSprite)
				return ;
			pSprite->SetHotSpot(w/2,h/2);
			if(gSprParticle[state.nPreset])
			{
				delete gSprParticle[state.nPreset];
				gSprParticle[state.nPreset] = NULL;
			}
			gSprParticle[state.nPreset] = pSprite;
			state.ps[state.nPreset]->m_info.sprite = gSprParticle[state.nPreset];
		}
	}
}

void CParticleEditDlg::OnEnChangeEmaskfile()
{
	std::string maskFile;
	GetItemString(maskFile,IDC_ETEXTUREP);
	if(strstr(maskFile.c_str(),".png") && strcmp(maskFile.c_str(),m_sMaskFile.c_str()))
	{
		m_sMaskFile = maskFile;
		CxImage ximg;
		std::string file = m_sTextureCurDir+"\\"+maskFile;
		bool b = ximg.Load(maskFile.c_str(),CXIMAGE_FORMAT_PNG);
		ximg.Flip(false,true);
		if(b)
			state.ps[state.nPreset]->setMaskAlpha(ximg.AlphaGetPointer(),ximg.GetWidth(),ximg.GetHeight());
	}

}

void CParticleEditDlg::OnBnClickedCheckonce()
{
	state.ps[state.nPreset]->Stop(true);
	state.ps[state.nPreset]->m_info.bOneShot = m_ctrlOnce.GetCheck();
	state.ps[state.nPreset]->Fire();
}

void CParticleEditDlg::OnBnClickedCheckdr()
{
	state.ps[state.nPreset]->m_info.bRelative = m_ctrlDirectionR.GetCheck();
}

void CParticleEditDlg::OnBnClickedChecktwoend()
{
	state.ps[state.nPreset]->m_info.btwoEnds = m_ctrlTwoE.GetCheck();
}

void CParticleEditDlg::OnBnClickedRadioshade()
{
	state.ps[state.nPreset]->m_info.bColorOscillate = false;
	SetColorText();
}

void CParticleEditDlg::OnBnClickedRadiooscillate()
{
	state.ps[state.nPreset]->m_info.bColorOscillate = true;
	SetColorText();
}

void CParticleEditDlg::OnEnChangeEditstepalpha()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.colColorStep.a,IDC_EDITSTEPALPHA);
}

void CParticleEditDlg::OnEnChangeEditstepr()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.colColorStep.r,IDC_EDITSTEPR);
}

void CParticleEditDlg::OnEnChangeEditstepg()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.colColorStep.g,IDC_EDITSTEPG);
}

void CParticleEditDlg::OnEnChangeEditstepb()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.colColorStep.b,IDC_EDITSTEPB);
}

void CParticleEditDlg::OnEnChangeEditstepvar()
{
	float fVar;
	GetItemFloat(fVar,IDC_EDITSTEPVAR);
	state.ps[state.nPreset]->m_info.fStepColorVar = fVar<0.0f?-fVar:fVar;
}

void CParticleEditDlg::OnBnClickedButtonloadplist()
{
	std::string plistFile = GetFilePath(m_hWnd,"PLIST Files (.plist)\0*.plist\0All Files (*.*)\0*.*\0\0");
	if(plistFile == "")
		return ;

	CParseFile pf;
	bool b = pf.parseFilePlist(plistFile.c_str());
	if(!b)
	{
		MessageBox("Plist 解析失败，或者版本不兼容","Failed",MB_ICONERROR|MB_OK);
		return ;
	}


	CAutoLock lock(&g_csLock);
	b = pf.chageToPsi(state.ps[state.nPreset]->m_info);
	cmdLoadPreset();
}

void CParticleEditDlg::OnEnChangeEditstepalphavar()
{
	float fVal;
	GetItemFloat(fVal,IDC_EDITSTEPALPHAVAR);
	state.ps[state.nPreset]->m_info.fStepAlphaVar = fVal<0.0f?-fVal:fVal;
}

void CParticleEditDlg::OnEnChangeEpsizevar2()
{
	float fVal;
	GetItemFloat(fVal,IDC_EPSIZEVAR2);
	state.ps[state.nPreset]->m_info.fSizeVarEnd = fVal<0.0f?-fVal:fVal;
}

void CParticleEditDlg::OnEnChangeEpspinvar2()
{
	float fVal;
	GetItemFloat(fVal,IDC_EPSPINVAR2);
	state.ps[state.nPreset]->m_info.fSpinVarEnd = fVal<0.0f?-fVal:fVal;
}

void CParticleEditDlg::OnEnChangeEditsradius()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fRadiusStart,IDC_EDITSRADIUS);
}

void CParticleEditDlg::OnEnChangeEditsradiusvar()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fRadiusStartVar,IDC_EDITSRADIUSVAR);
}

void CParticleEditDlg::OnEnChangeEditeradius()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fRadiusEnd,IDC_EDITERADIUS);
}

void CParticleEditDlg::OnEnChangeEditeradiusvar()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fRadiusEndVar,IDC_EDITERADIUSVAR);
}

void CParticleEditDlg::OnEnChangeEditrotate()
{
	GetItemFloat(state.ps[state.nPreset]->m_info.fRotatePerSecond,IDC_EDITROTATE);
}

void CParticleEditDlg::OnEnChangeEditrotatevar()
{
	float fVal;
	GetItemFloat(fVal,IDC_EDITROTATEVAR);
	state.ps[state.nPreset]->m_info.fRotatePerSecondVar = fVal<0.0f?-fVal:fVal;
}

void CParticleEditDlg::OnCbnSelchangeComborunmode()
{
	int n = m_ctrlRunMode.GetCurSel();
	if( n != CB_ERR)
	{
		if(n == FORCE_MODE)
			state.ps[state.nPreset]->m_info.bRadiusMode = false;
		else if(n == RADIUS_MODE)
			state.ps[state.nPreset]->m_info.bRadiusMode = true;
		SetRunMode();
	}
}
