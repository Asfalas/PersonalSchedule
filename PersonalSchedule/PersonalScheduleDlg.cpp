
// PersonalScheduleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "PersonalScheduleDlg.h"
#include "afxdialogex.h"
#include "DateDLG.h"
#include "AdoAccess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//BOOL isAutoStart = FALSE;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPersonalScheduleDlg �Ի���



CPersonalScheduleDlg::CPersonalScheduleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PERSONALSCHEDULE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPersonalScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPersonalScheduleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_AUTOSTART, &CPersonalScheduleDlg::OnAutostart)
	ON_COMMAND(ID_EXIT, &CPersonalScheduleDlg::OnExit)
	ON_COMMAND(ID_32790, &CPersonalScheduleDlg::OnExit)
	ON_BN_CLICKED(IDC_BUTTON2, &CPersonalScheduleDlg::OnBnClickedButton2)
	ON_MESSAGE(WM_SYSTEMTRAY, &CPersonalScheduleDlg::OnSystemtray)
	ON_COMMAND(ID_32789, &CPersonalScheduleDlg::OnMinimize)
	ON_COMMAND(ID_32788, &CPersonalScheduleDlg::OnMinShow)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPersonalScheduleDlg ��Ϣ�������

BOOL CPersonalScheduleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//��ť��ʼ��
	InitButton();

	//�����������ļ���ʼ��
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����
	BOOL ifFind = finder.FindFile(_T("./autorun.ini"));
	if (!ifFind)
	{
		::WritePrivateProfileString(_T("Autorun Info"), _T("isAutoRun"), _T("FALSE"), _T("./autorun.ini"));
		CMenu* menu = GetMenu()->GetSubMenu(0);
		menu->CheckMenuItem(ID_AUTOSTART, MF_UNCHECKED);
	}
	else
	{
		CString isAutorun;
		GetPrivateProfileString(_T("Autorun Info"), _T("isAutoRun"), _T(""), isAutorun.GetBuffer(MAX_PATH), MAX_PATH, _T("./autorun.ini"));
		if (isAutorun == "FALSE")
		{
			CMenu* menu = GetMenu()->GetSubMenu(0);
			menu->CheckMenuItem(ID_AUTOSTART, MF_UNCHECKED);
		}
		else 
		{
			CMenu* menu = GetMenu()->GetSubMenu(0);
			menu->CheckMenuItem(ID_AUTOSTART, MF_CHECKED);
		}
	}
	//ϵͳ����ʵ��
	NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	//NotifyIcon.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon.hIcon = m_hIcon;  //�����Ǿ�Ҳ����
	NotifyIcon.hWnd = m_hWnd;
	lstrcpy(NotifyIcon.szTip, _T("�����ճ̹���ϵͳ"));
	NotifyIcon.uCallbackMessage = WM_SYSTEMTRAY;
	NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &NotifyIcon);   //���ϵͳ����
	
	//��ʱ���趨
	OnTimer(0);
	Init_time = InitTime();
	SetTimer(1, 60000, NULL);
	//AfxMessageBox(_T("111"));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPersonalScheduleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CPersonalScheduleDlg::InitButton() 
{
	//��ť��ʼ��
	HBITMAP   hBitmap[5];
	hBitmap[0] = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP2));
	((CWnd *)GetDlgItem(IDC_BUTTON2))->SetWindowPos(NULL, 0, 0, 65, 65, SWP_NOZORDER | SWP_NOMOVE);
	((CButton *)GetDlgItem(IDC_BUTTON2))->SetBitmap(hBitmap[0]);
	hBitmap[1] = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1));
	((CWnd *)GetDlgItem(IDC_BUTTON3))->SetWindowPos(NULL, 0, 0, 65, 65, SWP_NOZORDER | SWP_NOMOVE);
	((CButton *)GetDlgItem(IDC_BUTTON3))->SetBitmap(hBitmap[1]);
	hBitmap[2] = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP3));
	((CWnd *)GetDlgItem(IDC_BUTTON4))->SetWindowPos(NULL, 0, 0, 65, 65, SWP_NOZORDER | SWP_NOMOVE);
	((CButton *)GetDlgItem(IDC_BUTTON4))->SetBitmap(hBitmap[2]);
	hBitmap[3] = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP4));
	((CWnd *)GetDlgItem(IDC_BUTTON5))->SetWindowPos(NULL, 0, 0, 65, 65, SWP_NOZORDER | SWP_NOMOVE);
	((CButton *)GetDlgItem(IDC_BUTTON5))->SetBitmap(hBitmap[3]);
	hBitmap[4] = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP5));
	((CWnd *)GetDlgItem(IDC_BUTTON6))->SetWindowPos(NULL, 0, 0, 65, 65, SWP_NOZORDER | SWP_NOMOVE);
	((CButton *)GetDlgItem(IDC_BUTTON6))->SetBitmap(hBitmap[4]);
}

CString CPersonalScheduleDlg::InitTime()
{
	COleDateTime time;
	time = COleDateTime::GetCurrentTime();
	CString sys_timeFinal;
	int year = time.GetYear();
	int mounth = time.GetMonth();
	int day = time.GetDay();
	CString yearTime;
	CString mounthTime;
	CString dayTime;
	yearTime.Format(_T("%d"), year);
	mounthTime.Format(_T("%d"), mounth);
	dayTime.Format(_T("%d"), day);
	sys_timeFinal.Format(_T("%s/%s/%s"), yearTime, mounthTime, dayTime);
	return sys_timeFinal;
}
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPersonalScheduleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPersonalScheduleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CPersonalScheduleDlg::OnSystemtray(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
		return    1;
	switch (lParam)
	{
	case  WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա�     
	{
		LPPOINT    lpoint = new    tagPOINT;
		::GetCursorPos(lpoint);//�õ����λ��     
		CMenu    menu;
		menu.CreatePopupMenu();//����һ������ʽ�˵�     
							   //���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ���     
							   //���أ��������������     
		menu.AppendMenu(MF_STRING, WM_DESTROY, _T("�ر�"));
		//ȷ������ʽ�˵���λ��     
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		//��Դ����     
		HMENU    hmenu = menu.Detach();
		menu.DestroyMenu();
		delete    lpoint;
	}
	break;
	case    WM_LBUTTONDBLCLK://˫������Ĵ���     
	{
		this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶�     
	}
	break;
	}
	return 0;
}

void CPersonalScheduleDlg::SetAutoRun(BOOL bAutoRun)
{
	HKEY hKey;
	CString strRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");//�ҵ�ϵͳ��������  
	if (bAutoRun)
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) //��������       
		{
			TCHAR szModule[_MAX_PATH];
			GetModuleFileName(NULL, szModule, _MAX_PATH);//�õ������������ȫ·��  
			RegSetValueEx(hKey, _T("Client"), 0, REG_SZ, (const BYTE*)(LPCSTR)szModule, strlen((char*)szModule)); //���һ����Key,������ֵ��"Client"��Ӧ�ó������֣����Ӻ�׺.exe��  
			RegCloseKey(hKey); //�ر�ע���  
		}
		else
		{
			AfxMessageBox(_T("ϵͳ��������,������ϵͳ����"));
		}
	}
	else
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			RegDeleteValue(hKey, _T("Client"));
			RegCloseKey(hKey);
		}
	}
}



void CPersonalScheduleDlg::OnAutostart()
{
	CString isAutorun;
	GetPrivateProfileString(_T("Autorun Info"), _T("isAutoRun"), _T(""), isAutorun.GetBuffer(MAX_PATH), MAX_PATH, _T("./autorun.ini"));
	if (isAutorun == "FALSE") 
	{
		SetAutoRun(TRUE);
		CMenu* menu = GetMenu()->GetSubMenu(0);
		menu->CheckMenuItem(ID_AUTOSTART, MF_CHECKED);
		::WritePrivateProfileString(_T("Autorun Info"), _T("isAutoRun"), _T("TRUE"), _T("./autorun.ini"));
	}
	else 
	{
		SetAutoRun(FALSE);
		CMenu* menu = GetMenu()->GetSubMenu(0);
		menu->CheckMenuItem(ID_AUTOSTART, MF_UNCHECKED);
		::WritePrivateProfileString(_T("Autorun Info"), _T("isAutoRun"), _T("FALSE"), _T("./autorun.ini"));
	}
}


void CPersonalScheduleDlg::OnExit()
{
	// TODO: �ڴ���������������
	::Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}


void CPersonalScheduleDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   
	CDateDLG tipDlg;           // ����Ի�����CTipDlg��ʵ��   
	nRes = tipDlg.DoModal();  // �����Ի���   
	if (IDCANCEL == nRes)     // �ж϶Ի����˳��󷵻�ֵ�Ƿ�ΪIDCANCEL���������return�������������ִ��   
		return;
}


//WindowProc�����ӵĴ���  
LRESULT CPersonalScheduleDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class  
	switch (message) //�ж���Ϣ����  
	{
	case WM_SYSTEMTRAY:
		//������û��������Ϣ   
		if (lParam == WM_LBUTTONDBLCLK)

		{
			//���˫��ʱ�����ڳ���   
			if (AfxGetApp()->m_pMainWnd->IsWindowVisible()) //�жϴ��ڵ�ǰ״̬  
			{
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE); //���ش���  
			}
			else
			{
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW); //��ʾ����  
			}

		}
		else if (lParam == WM_RBUTTONDOWN)
		{ //����Ҽ���������ѡ��   
			CMenu menu;
			menu.LoadMenu(IDR_MENU2); //�������ȶ����ѡ��   
			CMenu *pMenu = menu.GetSubMenu(0);
			CPoint pos;
			GetCursorPos(&pos);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, AfxGetMainWnd());
		}
		break;
	case WM_SYSCOMMAND:
		//�����ϵͳ��Ϣ   
		if (wParam == SC_MINIMIZE)
		{
			//���յ���С����Ϣʱ����������   
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			return 0;
		}
		if (wParam == SC_CLOSE)
		{
			::Shell_NotifyIcon(NIM_DELETE, &NotifyIcon); //�ر�ʱɾ��ϵͳ����ͼ��  
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CPersonalScheduleDlg::OnMinimize()
{
	// TODO: �ڴ���������������
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
}


void CPersonalScheduleDlg::OnMinShow()
{
	// TODO: �ڴ���������������
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
}


void CPersonalScheduleDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString sys_timeFinal;
	sys_timeFinal = InitTime();
	if (sys_timeFinal == Init_time)
		return;
	//AfxMessageBox(_T("test"));
	AdoAccess data;				 // ADOConn�����
	data.OnInitADOConn();		 //�������ݿ�
	CString sql,date,title,content,isRemind;
	sql = "select * from datetable";                         //���ò�ѯ���
	data.m_pRecordset = data.GetRecordSet((_bstr_t)sql); //��ѯ
	while (!data.m_pRecordset->adoEOF)
	{
		date=data.m_pRecordset->GetCollect("d_date");
		title = data.m_pRecordset->GetCollect("d_title");
		content = data.m_pRecordset->GetCollect("d_content");
		if (date == sys_timeFinal)
		{
			MessageBox(_T("�������ճ�Ϊ:\r\n���⣺" + title + "\r\n���ݣ�" + content), _T("��Ϣ��ʾ"), MB_OKCANCEL | MB_ICONQUESTION);

		}
		data.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
	}
	data.ExitConnect(); //�Ͽ����ݿ�����

}
