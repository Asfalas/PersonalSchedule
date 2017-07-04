#pragma once
#include "ATLComTime.h"
#include "afxwin.h"


// CTimepopDlg 对话框

class CTimepopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTimepopDlg)

public:
	CTimepopDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTimepopDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWTIME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	COleDateTime m_ttime;
	//	BOOL m_once;
	//	CButton m_perday;
	//	BOOL m_perday;
	//	BOOL m_perweek;
	int m_radio;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioOnce();
	afx_msg void OnBnClickedRadioPerday();
	afx_msg void OnBnClickedRadioPerweek();
	CString m_ttitle;
	COleDateTime m_tdate;
	CString m_tcontent;
};
