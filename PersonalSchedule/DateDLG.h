#pragma once
#include "ATLComTime.h"
#include "afxcmn.h"


// CDateDLG 对话框

class CDateDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CDateDLG)

public:
	CDateDLG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDateDLG();
	BOOL OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	COleDateTime m_time;
	CString m_title;
	CString m_content;
	CListCtrl m_jr_Grid;
	CString m_detail;
	int pos;
	afx_msg void OnBnClickedNew();
	void AddtoGrid();
	afx_msg void OnBnClickedDel();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedChange();
	afx_msg void OnBnClickedClear();
};
