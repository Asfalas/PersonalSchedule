
// PersonalScheduleDlg.h : ͷ�ļ�
//

#pragma once


// CPersonalScheduleDlg �Ի���
class CPersonalScheduleDlg : public CDialogEx
{
// ����
public:
	CPersonalScheduleDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PERSONALSCHEDULE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void SetAutoRun(BOOL bAutoRun);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnAutostart();
	afx_msg void OnExit();
	afx_msg void OnBnClickedButton2();
};
