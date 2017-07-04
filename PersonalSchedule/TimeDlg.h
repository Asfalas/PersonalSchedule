#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTimeDlg 对话框

class CTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTimeDlg)

public:
	CTimeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTimeDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	BOOL OnInitDialog();
	int pos;
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_tlist;
	afx_msg void OnBnClickedTnew();
	void AddtoGrid();
	afx_msg void OnNMClickTlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedTchange();
	afx_msg void OnBnClickedTdel();
	afx_msg void OnNMDblclkTlist(NMHDR *pNMHDR, LRESULT *pResult);
	//	CString m_hide;
	CEdit m_hide;
	afx_msg void OnEnKillfocusEditHide();
	//	CEdit m_tcontent;
	CString m_tcontent;
	afx_msg void OnNMKillfocusTlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedTclear();
};
