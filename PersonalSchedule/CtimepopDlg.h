#pragma once
#include "ATLComTime.h"


// CCtimepopDlg �Ի���

class CCtimepopDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCtimepopDlg)

public:
	CCtimepopDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CCtimepopDlg(int pos, CWnd* pParent = NULL);
	virtual ~CCtimepopDlg();
	int pos;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTIME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	COleDateTime m_cdate;
//	COleDateTime m_ctime;
//	CString m_ctitle;
//	CString m_ccontent;
	COleDateTime m_cdate;
	COleDateTime m_ctime;
	int m_radio1;
	CString m_ctitle;
	CString m_ccontent;
	afx_msg
		BOOL OnInitDialog();
	void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
};
