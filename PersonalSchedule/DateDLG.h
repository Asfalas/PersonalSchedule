#pragma once


// CDateDLG �Ի���

class CDateDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CDateDLG)

public:
	CDateDLG(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDateDLG();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
