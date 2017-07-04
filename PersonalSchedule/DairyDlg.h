#pragma once
#include "ATLComTime.h"
#include "afxwin.h"


// CDairyDlg 对话框

class CDairyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDairyDlg)

public:
	CDairyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDairyDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DAIRY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	BOOL OnInitDialog();
	int pos;
	DECLARE_MESSAGE_MAP()
public:
	CString my_date;
	afx_msg void OnCbnSelchangeCombo1();
	void AddtoGrid(CString date);
	CString InitTime();
	BOOL CheckTitle(CString title, int place);
	//afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDadd();
	afx_msg void OnBnClickedDchange();
	afx_msg void OnBnClickedDdel();
	CString m_dcontent;
	COleDateTime m_ddate;
	CComboBox m_dtitle;
	CString m_current;
	afx_msg void OnDtnDatetimechangeDdate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeDtitle();
	afx_msg void OnBnClickedDclear();
};
