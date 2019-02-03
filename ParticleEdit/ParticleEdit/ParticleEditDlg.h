// ParticleEditDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "resource.h"
#include <string>
#include <string.h>

// CParticleEditDlg 对话框
class CParticleEditDlg : public CDialog
{
// 构造
public:
	CParticleEditDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PARTICLEEDIT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual void OnOK(){
		return ;
	}
public:
	void SetColorText();
	void SetRunMode();
	void SetGui(const char* dir);
// 实现
protected:
	HICON m_hIcon;

	std::string m_sTextureCurDir;
	std::string m_sMaskFile;
	std::string m_sTextureFile;
	float m_fSysLifeTime;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChcontinue();
	CButton m_ctrlContinue;
	CButton m_ctrlOnce;
	CButton m_ctrlDirectionR;
	CButton m_ctrlTwoE;
	CButton m_ctrlPSpread;
	CComboBox m_ctrlRunMode;
	afx_msg void OnEnChangeEsyslifetime();
	afx_msg void OnEnChangeEemission();
	afx_msg void OnEnChangeElifemin();
	afx_msg void OnEnChangeElifemax();
	afx_msg void OnBnClickedRadioadd();
	afx_msg void OnBnClickedRadioblend();
	afx_msg void OnEnChangeEcenterx();
	afx_msg void OnEnChangeEcentery();
	afx_msg void OnEnChangeEdirection();
	afx_msg void OnEnChangeEspread();
	afx_msg void OnEnChangeEsspeedmin();
	afx_msg void OnEnChangeEsspeedmax();
	afx_msg void OnEnChangeEgravitymin();
	afx_msg void OnEnChangeEgravitymax();
	afx_msg void OnEnChangeExforcemin();
	afx_msg void OnEnChangeExforcemax();
	afx_msg void OnEnChangeEraccelerationmin();
	afx_msg void OnEnChangeEraccelerationmax();
	afx_msg void OnEnChangeEtaccmin();
	afx_msg void OnEnChangeEtaccmax();
	afx_msg void OnBnClickedRemintpoint();
	afx_msg void OnBnClickedRemintrect();
	afx_msg void OnBnClickedRemintcircle();
	afx_msg void OnBnClickedRemintring();
	afx_msg void OnBnClickedRemintmask();
	afx_msg void OnEnChangeEx();
	afx_msg void OnEnChangeEy();
	afx_msg void OnEnChangeEwidth();
	afx_msg void OnEnChangeEheight();
	afx_msg void OnEnChangeEina();
	afx_msg void OnEnChangeEinb();
	afx_msg void OnEnChangeEouta();
	afx_msg void OnEnChangeEoutb();
	afx_msg void OnBnClickedChspread();
	afx_msg void OnEnChangeEpssize();
	afx_msg void OnEnChangeEpesize();
	afx_msg void OnEnChangeEpsizevar();
	afx_msg void OnEnChangeEpsspin();
	afx_msg void OnEnChangeEpespin();
	afx_msg void OnEnChangeEpspinvar();
	afx_msg void OnEnChangeEpsalpha();
	afx_msg void OnEnChangeEpealpha();
	afx_msg void OnEnChangeEpalphavar();
	afx_msg void OnEnChangeEpsr();
	afx_msg void OnEnChangeEpsg();
	afx_msg void OnEnChangeEpsb();
	afx_msg void OnEnChangeEper();
	afx_msg void OnEnChangeEpeg();
	afx_msg void OnEnChangeEpeb();
	afx_msg void OnEnChangeEpcolorvar();
	afx_msg void OnBnClickedBfindtexfile();
	afx_msg void OnBnClickedBfindmask();
	afx_msg void OnEnChangeEtexturep();
	afx_msg void OnEnChangeEmaskfile();
	afx_msg void OnBnClickedCheckonce();
	afx_msg void OnBnClickedCheckdr();
	afx_msg void OnBnClickedChecktwoend();
	afx_msg void OnBnClickedRadioshade();
	afx_msg void OnBnClickedRadiooscillate();
	afx_msg void OnEnChangeEditstepalpha();
	afx_msg void OnEnChangeEditstepr();
	afx_msg void OnEnChangeEditstepg();
	afx_msg void OnEnChangeEditstepb();
	afx_msg void OnEnChangeEditstepvar();
	afx_msg void OnBnClickedButtonloadplist();
	afx_msg void OnEnChangeEditstepalphavar();
	afx_msg void OnEnChangeEpsizevar2();
	afx_msg void OnEnChangeEpspinvar2();
	afx_msg void OnEnChangeEditsradius();
	afx_msg void OnEnChangeEditsradiusvar();
	afx_msg void OnEnChangeEditeradius();
	afx_msg void OnEnChangeEditeradiusvar();
	afx_msg void OnEnChangeEditrotate();
	afx_msg void OnEnChangeEditrotatevar();
	afx_msg void OnCbnSelchangeComborunmode();
};
