#pragma once
#include "ATLComTime.h"
#include "afxcmn.h"


// CFestivalDlg �Ի���

class CFestivalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFestivalDlg)

public:
	CFestivalDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFestivalDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	int pos;
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	COleDateTime m_htime;
	CString m_htitle;
	CString m_hcontent;
	CListCtrl m_hlist;
	CString m_hdetail;
	afx_msg void OnBnClickedHnew();
	void AddtoGrid();
	afx_msg void OnBnClickedHdel();
	afx_msg void OnNMClickHlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedHchange();
	afx_msg void OnBnClickedHclear();
};
