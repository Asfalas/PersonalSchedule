#pragma once
#include "afxwin.h"
#include "Remind.h"

// CRemindDlg 对话框

class CRemindDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRemindDlg)

public:
	CRemindDlg(CWnd* pParent = NULL);   // 标准构造函数
	CRemindDlg(const CRemind& remind,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRemindDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMIND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CRemind remind;
	DECLARE_MESSAGE_MAP()
public:
	CString m_rtime;
	CString m_rtitle;
	CString m_rcontent;
	CString m_rcurrent;
	CComboBox m_later;
	afx_msg
		BOOL OnInitDialog();
	void OnEnChangeRtime();
	afx_msg void OnCbnSelchangeLater();
	CString CalTime(CString time, int sel);
};
