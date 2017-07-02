
// PersonalScheduleDlg.h : ͷ�ļ�
//

#pragma once

#define   WM_SYSTEMTRAY WM_USER+5
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
	NOTIFYICONDATA NotifyIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	void InitButton();
	CString Init_time;
	CString  InitTime();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSystemtray(WPARAM wParam, LPARAM lParam);
	void SetAutoRun(BOOL bAutoRun);
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnBnClickedOk();
	afx_msg void OnAutostart();
	afx_msg void OnExit();
	afx_msg void OnBnClickedButton2();
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnMinimize();
	afx_msg void OnMinShow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMusic();
	afx_msg void OnHoliday();
	afx_msg void OnHelp();
	afx_msg void OnBnClickedButton3();
};
