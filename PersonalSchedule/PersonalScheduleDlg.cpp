
// PersonalScheduleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "PersonalScheduleDlg.h"
#include "afxdialogex.h"
#include "DateDLG.h"
#include "AdoAccess.h"
#include "MusicDlg.h"
#include <MMSystem.h>  
#include <Digitalv.h>  
#include "FestivalDlg.h"
#include "TimeDlg.h"
#include "DairyDlg.h"
#include "MemoDlg.h"

#pragma comment(lib, "Winmm.lib")  
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DWORD play();
void stop(DWORD DeviceId);
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
	, m_maindetail(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPersonalScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAINDETAIL, m_maindetail);
	DDX_Control(pDX, IDC_MAINLIST, m_schedule);
}

BEGIN_MESSAGE_MAP(CPersonalScheduleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_AUTOSTART, &CPersonalScheduleDlg::OnAutostart)
	ON_COMMAND(ID_EXIT, &CPersonalScheduleDlg::OnExit)
	ON_COMMAND(ID_32790, &CPersonalScheduleDlg::OnExit)
	ON_BN_CLICKED(IDC_BUTTON2, &CPersonalScheduleDlg::OnBnClickedButton2)
	ON_COMMAND(ID_DATE, &CPersonalScheduleDlg::OnBnClickedButton2)
	ON_MESSAGE(WM_SYSTEMTRAY, &CPersonalScheduleDlg::OnSystemtray)
	ON_COMMAND(ID_32789, &CPersonalScheduleDlg::OnMinimize)
	ON_COMMAND(ID_32788, &CPersonalScheduleDlg::OnMinShow)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, &CPersonalScheduleDlg::OnMusic)
	ON_COMMAND(ID_MUSIC, &CPersonalScheduleDlg::OnMusic)
	ON_COMMAND(ID_HOLIDAY, &CPersonalScheduleDlg::OnHoliday)
	ON_COMMAND(ID_HELP, &CPersonalScheduleDlg::OnHelp)
	ON_BN_CLICKED(IDC_BUTTON3, &CPersonalScheduleDlg::OnBnClickedButton3)
	ON_COMMAND(ID_TIME, &CPersonalScheduleDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CPersonalScheduleDlg::OnBnClickedDairy)
	ON_COMMAND(ID_DAIRY, &CPersonalScheduleDlg::OnBnClickedDairy)
	ON_BN_CLICKED(IDC_BUTTON6, &CPersonalScheduleDlg::OnBnClickedMemo)
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
	ifFind = finder.FindFile(_T("./flag.ini"));
	if(!ifFind)
		::WritePrivateProfileString(_T("Database Update Info"), _T("isUpdate"), _T("FALSE"), _T("./flag.ini"));
	//ϵͳ����ʵ��
	NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	//NotifyIcon.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon.hIcon = m_hIcon;  //�����Ǿ�Ҳ����
	NotifyIcon.hWnd = m_hWnd;
	lstrcpy(NotifyIcon.szTip, _T("�����ճ̹���ϵͳ"));
	NotifyIcon.uCallbackMessage = WM_SYSTEMTRAY;
	NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &NotifyIcon);   //���ϵͳ����
	
	//list control �趨
	CRect rect;
	m_schedule.GetClientRect(&rect);
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_schedule.SetExtendedStyle(m_schedule.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Ϊ�б���ͼ�ؼ��������   
	m_schedule.InsertColumn(0, _T("�ճ�ʱ��"), LVCFMT_CENTER, rect.Width() / 3, 0);
	m_schedule.InsertColumn(1, _T("�ճ̱���"), LVCFMT_CENTER, rect.Width() / 3, 1);
	m_schedule.InsertColumn(2, _T("�ճ�����"), LVCFMT_CENTER, rect.Width() / 3, 2);
	AddtoGrid();
	//��ʱ���趨
	OnTimer(0);
	SetTimer(0, 60000, NULL);
	SetTimer(1, 1000, NULL);
	//AfxMessageBox(_T("111"));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CPersonalScheduleDlg::AddtoGrid()
{
	AdoAccess m_ADOConn;       // ADOConn�����
	m_ADOConn.OnInitADOConn(); //�������ݿ�
	CString sql;
	int i = 0;
	sql.Format(_T("select * from datetable order by d_date asc"));                         //���ò�ѯ���
	m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //��ѯ
	while (!m_ADOConn.m_pRecordset->adoEOF)
	{
		//���б���ͼ�ؼ��в�����IDC_STATIC_ZZH_CONTENTIDC_STATIC_ZZH_TITLE
		m_schedule.InsertItem(i, _T(""));
		//���б���ͼ�ؼ��в�����
		m_schedule.SetItemText(i, 0, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_date")));
		m_schedule.SetItemText(i, 1, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_title")));
		m_schedule.SetItemText(i, 2, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_content")));
		m_ADOConn.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
		i++;
	}
	m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
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
	int hour = time.GetHour();
	int minute = time.GetMinute();
	int second = time.GetSecond();
	CString hourTime;
	CString minuteTime;
	CString secondTime;
	hourTime.Format(_T("%d"), hour);
	minuteTime.Format(_T("%d"), minute);
	secondTime.Format(_T("%d"), second);
	sys_timeFinal.Format(_T("%s:%s:%s"), hourTime, minuteTime, secondTime);
	return sys_timeFinal;
}


CString CPersonalScheduleDlg::InitDate()
{
	COleDateTime date;
	date = COleDateTime::GetCurrentTime();
	CString sys_dateFinal;
	int year = date.GetYear();
	int mounth = date.GetMonth();
	int day = date.GetDay();
	CString yearTime;
	CString mounthTime;
	CString dayTime;
	yearTime.Format(_T("%d"), year);
	mounthTime.Format(_T("%d"), mounth);
	dayTime.Format(_T("%d"), day);
	sys_dateFinal.Format(_T("%s/%s/%s"), yearTime, mounthTime, dayTime);
	return sys_dateFinal;
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
			CString a = szModule;
			RegSetValueEx(hKey, _T("PersonalSchedule"), 0, REG_SZ, (const BYTE*)(LPCSTR)szModule, a.GetLength()); //���һ����Key,������ֵ��"Client"��Ӧ�ó������֣����Ӻ�׺.exe��  
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
			RegDeleteValue(hKey, _T("PersonalSchedule"));
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


//��������
void CPersonalScheduleDlg::OnBnClickedButton2()
{
	KillTimer(0);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   
	CDateDLG tipDlg;           // ����Ի�����CTipDlg��ʵ��   
	nRes = tipDlg.DoModal();  // �����Ի��� 
	OnTimer(0);
	SetTimer(0, 6000, NULL);
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



void CPersonalScheduleDlg::OnMusic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   
	CMusicDlg mDlg;           // ����Ի�����CTipDlg��ʵ��   
	nRes = mDlg.DoModal();  // �����Ի���   
	if (IDCANCEL == nRes)     // �ж϶Ի����˳��󷵻�ֵ�Ƿ�ΪIDCANCEL���������return�������������ִ��   
		return;
}


void CPersonalScheduleDlg::OnHoliday()
{
	// TODO: �ڴ���������������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   
	CFestivalDlg hDlg;           // ����Ի�����CTipDlg��ʵ��   
	nRes = hDlg.DoModal();  // �����Ի���   
	if (IDCANCEL == nRes)     // �ж϶Ի����˳��󷵻�ֵ�Ƿ�ΪIDCANCEL���������return�������������ִ��   
		return;
}


void CPersonalScheduleDlg::OnHelp()
{
	// TODO: �ڴ���������������

	MessageBox(_T("�����ճ̹���ϵͳ 1.0 ��\r\n         ��Ȩ���� (C)2017\r\n              -zhang"), _T("ϵͳ��Ϣ"), NULL);
}

//��ʱ����
void CPersonalScheduleDlg::OnBnClickedButton3()
{
	KillTimer(1);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   
	CTimeDlg tDlg;           // ����Ի�����CTipDlg��ʵ��   
	nRes = tDlg.DoModal();  // �����Ի���
	SetTimer(1, 1000, NULL);
	if (IDCANCEL == nRes)     // �ж϶Ի����˳��󷵻�ֵ�Ƿ�ΪIDCANCEL���������return�������������ִ��   
		return;
}


void CPersonalScheduleDlg::OnBnClickedDairy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   
	CDairyDlg dDlg;           // ����Ի�����CTipDlg��ʵ��   
	nRes = dDlg.DoModal();  // �����Ի���   
	if (IDCANCEL == nRes)     // �ж϶Ի����˳��󷵻�ֵ�Ƿ�ΪIDCANCEL���������return�������������ִ��   
		return;
}


void CPersonalScheduleDlg::OnBnClickedMemo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   
	CMemoDlg mDlg;           // ����Ի�����CTipDlg��ʵ��   
	nRes = mDlg.DoModal();  // �����Ի���   
	if (IDCANCEL == nRes)     // �ж϶Ի����˳��󷵻�ֵ�Ƿ�ΪIDCANCEL���������return�������������ִ��   
		return;
}


void CPersonalScheduleDlg::CheckDate() 
{
	CString sys_dateFinal, path, name, vol;
	sys_dateFinal = InitDate();
	//AfxMessageBox(_T("test"));
	AdoAccess data;				 // ADOConn�����
	data.OnInitADOConn();		 //�������ݿ�
	CString date, title, content, isRemind;
	_bstr_t sql;
	sql = "select * from datetable";                         //���ò�ѯ���
	data.m_pRecordset.CreateInstance(__uuidof(Recordset));
	data.m_pRecordset->Open(sql, data.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	BOOL flag = false;
	DWORD myDevice = NULL;
	while (!data.m_pRecordset->adoEOF)
	{
		date = data.m_pRecordset->GetCollect("d_date");
		title = data.m_pRecordset->GetCollect("d_title");
		content = data.m_pRecordset->GetCollect("d_content");
		isRemind = data.m_pRecordset->GetCollect("d_remind");
		if (date == sys_dateFinal&&isRemind == _T("FALSE"))
		{
			if (!flag)
			{
				myDevice = play();
				flag = true;
			}
			KillTimer(0);
			MessageBox(_T("�������ճ�Ϊ:\r\n���⣺" + title + "\r\n���ݣ�" + content), _T("�ճ�����"), MB_ICONQUESTION);
			data.m_pRecordset->PutCollect("d_remind", (_bstr_t)"TRUE");
			data.m_pRecordset->Update();
			SetTimer(0, 60000, NULL);
		}
		data.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
	}
	stop(myDevice);
	data.ExitConnect(); //�Ͽ����ݿ�����
}


void CPersonalScheduleDlg::CheckTime()
{
	CString sys_timeFinal;
	sys_timeFinal = InitTime();
	COleDateTime s_time;
	s_time = COleDateTime::GetCurrentTime();
	int sys_weekday = s_time.GetDayOfWeek();
	//AfxMessageBox(_T("test"));
	AdoAccess data;				 // ADOConn�����
	data.OnInitADOConn();		 //�������ݿ�
	CString time, title, content, isRemind,type;
	int weekday;
	_bstr_t sql;
	sql = "select * from datetable where d_type<>'��������'";                         //���ò�ѯ���
	data.m_pRecordset.CreateInstance(__uuidof(Recordset));
	data.m_pRecordset->Open(sql, data.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	BOOL flag = false;
	DWORD myDevice = NULL;
	while (!data.m_pRecordset->adoEOF)
	{
		time = data.m_pRecordset->GetCollect("d_date");
		title = data.m_pRecordset->GetCollect("d_title");
		content = data.m_pRecordset->GetCollect("d_content");
		isRemind = data.m_pRecordset->GetCollect("d_remind");
		weekday = data.m_pRecordset->GetCollect("d_weekday");
		type = data.m_pRecordset->GetCollect("d_type");
		if (time.Mid(time.Find(_T(" ")) + 1) == sys_timeFinal&&isRemind == _T("FALSE"))
		{ 
			if (type == "һ������")
			{
				CString date = InitDate();
				if (date == time.Left(time.Find(_T(" "))))
				{
					if (!flag)
					{
						myDevice = play();
						flag = true;
					}
					KillTimer(1);
					MessageBox(time + _T("��ʱ����:\r\n���⣺" + title + "\r\n���ݣ�" + content), _T("�ճ�����"), MB_ICONQUESTION);
					data.m_pRecordset->PutCollect("d_remind", (_bstr_t)"TRUE");
					data.m_pRecordset->Update();
					SetTimer(1, 1000, NULL);
				}
			}
			if (type == "ÿ������" || (type == "ÿ������"&&sys_weekday == weekday))
			{
				if (!flag)
				{
					myDevice = play();
					flag = true;
				}
				KillTimer(1);
				MessageBox(time.Mid(time.Find(_T(" ")) + 1) + _T("��ʱ����:\r\n���⣺" + title + "\r\n���ݣ�" + content), _T("�ճ�����"), MB_ICONQUESTION);
				data.m_pRecordset->PutCollect("d_remind", (_bstr_t)"TRUE");
				data.m_pRecordset->Update();
				SetTimer(1, 1000, NULL);
			}
		}
		data.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
	}
	stop(myDevice);
	data.ExitConnect(); //�Ͽ����ݿ�����
}

void CPersonalScheduleDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 0:
		CheckDate();
		break;
	case 1:
		CheckTime();
		break;
	default:
		break;
	}
	return;
}

DWORD play()
{
	CString path, vol;
	GetPrivateProfileString(_T("Music Info"), _T("m_path"), _T(""), path.GetBuffer(MAX_PATH), MAX_PATH, _T("./music.ini"));
	GetPrivateProfileString(_T("Music Info"), _T("m_vol"), _T(""), vol.GetBuffer(MAX_PATH), MAX_PATH, _T("./music.ini"));
	MCI_OPEN_PARMS openParms;//MCI_OPEN������Ҫ�����ṹ��  
	openParms.lpstrDeviceType = _T("MPEGvideo");//MP3���ļ��豸IDΪMPEGvideo  
	openParms.lpstrElementName = path;//MP3�ļ��Ĵ��·��  
									  //��������  
	mciSendCommand(
		NULL,//���豸����Ҫ�豸ID  
		MCI_OPEN,//MCI_OPEN,�������ʾ���豸  
		MCI_OPEN_ELEMENT | MCI_OPEN_TYPE | MCI_WAIT,
		(DWORD)(LPVOID(&openParms)));//����MCI_OPEN_PARMS����  
	WORD m_wDeviceID = openParms.wDeviceID;
	//2�������豸  
	MCI_PLAY_PARMS playParms;//MCI_PLAY������Ҫ�Ĳ����ṹ��  
	mciSendCommand(
		m_wDeviceID,//��Ҫ���豸ID  
		MCI_PLAY,//��������豸  
		MCI_NOTIFY,//�ȴ������ļ�������Ȼ�󷵻�  
		(DWORD)(LPVOID(&playParms)));//����MCI_PLAY_PARMS����  

	MCI_DGV_SETAUDIO_PARMS setvolume; //���������Ĳ���
	setvolume.dwCallback = NULL; //
	setvolume.dwItem = MCI_DGV_SETAUDIO_VOLUME; //��������������
	setvolume.dwValue = _ttoi(vol) * 10; //����ֵ��vol
	mciSendCommand(m_wDeviceID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD)(LPVOID)&setvolume);

	return m_wDeviceID;
}

void stop(DWORD DeviceId)
{
	mciSendCommand(DeviceId, MCI_STOP, 0, 0);
	mciSendCommand(DeviceId, MCI_CLOSE, 0, 0);
}
