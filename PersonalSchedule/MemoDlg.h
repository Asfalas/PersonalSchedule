#pragma once


// CMemoDlg �Ի���

class CMemoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMemoDlg)

public:
	CMemoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMemoDlg();
	int pos;
	CString my_date;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEMO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	BOOL OnInitDialog();

	void AddtoGrid(CString date);

	CString InitTime();

	DECLARE_MESSAGE_MAP()
};
