#pragma once
#include "ATLComTime.h"
#include "afxcmn.h"


// CDateDLG �Ի���

class CDateDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CDateDLG)

public:
	CDateDLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDateDLG();
	BOOL OnInitDialog();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	COleDateTime m_time;
	CString m_title;
	CString m_content;
	CListCtrl m_jr_Grid;
	CString m_detail;
	afx_msg void OnBnClickedNew();
	void AddtoGrid();
};
