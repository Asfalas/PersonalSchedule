#pragma once
#include "ATLComTime.h"
#include "afxwin.h"


// CMemoDlg 对话框

class CMemoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMemoDlg)

public:
	CMemoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMemoDlg();
	int pos;
	CString my_date;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEMO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	BOOL OnInitDialog();

	void AddtoGrid(CString date);

	CString InitTime();

	DECLARE_MESSAGE_MAP()
public:
	CString m_mcontent;
	COleDateTime m_mdate;
	CComboBox m_mtitle;
	CString m_mcurrent;
	afx_msg void OnBnClickedMadd();
	BOOL CheckTitle(CString title, int place);
	afx_msg void OnBnClickedMchange();
	afx_msg void OnBnClickedMdel();
	afx_msg void OnDtnDatetimechangeMdate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeMtitle();
};
