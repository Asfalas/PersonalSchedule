
// PersonalScheduleDlg.cpp : 实现文件
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
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CPersonalScheduleDlg 对话框


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


// CPersonalScheduleDlg 消息处理程序

BOOL CPersonalScheduleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//按钮初始化
	InitButton();

	//开机自启动文件初始化
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用
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
	//系统托盘实现
	NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	//NotifyIcon.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	NotifyIcon.hIcon = m_hIcon;  //上面那句也可以
	NotifyIcon.hWnd = m_hWnd;
	lstrcpy(NotifyIcon.szTip, _T("个人日程管理系统"));
	NotifyIcon.uCallbackMessage = WM_SYSTEMTRAY;
	NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &NotifyIcon);   //添加系统托盘
	
	//list control 设定
	CRect rect;
	m_schedule.GetClientRect(&rect);
	// 为列表视图控件添加全行选中和栅格风格   
	m_schedule.SetExtendedStyle(m_schedule.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加三列   
	m_schedule.InsertColumn(0, _T("日程时间"), LVCFMT_CENTER, rect.Width() / 3, 0);
	m_schedule.InsertColumn(1, _T("日程标题"), LVCFMT_CENTER, rect.Width() / 3, 1);
	m_schedule.InsertColumn(2, _T("日程内容"), LVCFMT_CENTER, rect.Width() / 3, 2);
	AddtoGrid();
	//定时器设定
	OnTimer(0);
	SetTimer(0, 60000, NULL);
	SetTimer(1, 1000, NULL);
	//AfxMessageBox(_T("111"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CPersonalScheduleDlg::AddtoGrid()
{
	AdoAccess m_ADOConn;       // ADOConn类对象
	m_ADOConn.OnInitADOConn(); //连接数据库
	CString sql;
	int i = 0;
	sql.Format(_T("select * from datetable order by d_date asc"));                         //设置查询语句
	m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //查询
	while (!m_ADOConn.m_pRecordset->adoEOF)
	{
		//向列表视图控件中插入行IDC_STATIC_ZZH_CONTENTIDC_STATIC_ZZH_TITLE
		m_schedule.InsertItem(i, _T(""));
		//向列表视图控件中插入列
		m_schedule.SetItemText(i, 0, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_date")));
		m_schedule.SetItemText(i, 1, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_title")));
		m_schedule.SetItemText(i, 2, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_content")));
		m_ADOConn.m_pRecordset->MoveNext(); //将记录集指针移动到下一条记录
		i++;
	}
	m_ADOConn.ExitConnect(); //断开数据库连接
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
	//按钮初始化
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
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPersonalScheduleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	case  WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭”     
	{
		LPPOINT    lpoint = new    tagPOINT;
		::GetCursorPos(lpoint);//得到鼠标位置     
		CMenu    menu;
		menu.CreatePopupMenu();//声明一个弹出式菜单     
							   //增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已     
							   //隐藏），将程序结束。     
		menu.AppendMenu(MF_STRING, WM_DESTROY, _T("关闭"));
		//确定弹出式菜单的位置     
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
		//资源回收     
		HMENU    hmenu = menu.Detach();
		menu.DestroyMenu();
		delete    lpoint;
	}
	break;
	case    WM_LBUTTONDBLCLK://双击左键的处理     
	{
		this->ShowWindow(SW_SHOW);//简单的显示主窗口完事儿     
	}
	break;
	}
	return 0;
}


void CPersonalScheduleDlg::SetAutoRun(BOOL bAutoRun)
{
	HKEY hKey;
	CString strRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");//找到系统的启动项  
	if (bAutoRun)
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) //打开启动项       
		{
			TCHAR szModule[_MAX_PATH];
			GetModuleFileName(NULL, szModule, _MAX_PATH);//得到本程序自身的全路径
			CString a = szModule;
			RegSetValueEx(hKey, _T("PersonalSchedule"), 0, REG_SZ, (const BYTE*)(LPCSTR)szModule, a.GetLength()); //添加一个子Key,并设置值，"Client"是应用程序名字（不加后缀.exe）  
			RegCloseKey(hKey); //关闭注册表  
		}
		else
		{
			AfxMessageBox(_T("系统参数错误,不能随系统启动"));
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
	// TODO: 在此添加命令处理程序代码
	::Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}


//定期提醒
void CPersonalScheduleDlg::OnBnClickedButton2()
{
	KillTimer(0);
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   
	CDateDLG tipDlg;           // 构造对话框类CTipDlg的实例   
	nRes = tipDlg.DoModal();  // 弹出对话框 
	OnTimer(0);
	SetTimer(0, 6000, NULL);
	if (IDCANCEL == nRes)     // 判断对话框退出后返回值是否为IDCANCEL，如果是则return，否则继续向下执行   
		return;
}


//WindowProc中增加的代码  
LRESULT CPersonalScheduleDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class  
	switch (message) //判断消息类型  
	{
	case WM_SYSTEMTRAY:
		//如果是用户定义的消息   
		if (lParam == WM_LBUTTONDBLCLK)

		{
			//鼠标双击时主窗口出现   
			if (AfxGetApp()->m_pMainWnd->IsWindowVisible()) //判断窗口当前状态  
			{
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE); //隐藏窗口  
			}
			else
			{
				AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW); //显示窗口  
			}

		}
		else if (lParam == WM_RBUTTONDOWN)
		{ //鼠标右键单击弹出选单   
			CMenu menu;
			menu.LoadMenu(IDR_MENU2); //载入事先定义的选单   
			CMenu *pMenu = menu.GetSubMenu(0);
			CPoint pos;
			GetCursorPos(&pos);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, AfxGetMainWnd());
		}
		break;
	case WM_SYSCOMMAND:
		//如果是系统消息   
		if (wParam == SC_MINIMIZE)
		{
			//接收到最小化消息时主窗口隐藏   
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
			return 0;
		}
		if (wParam == SC_CLOSE)
		{
			::Shell_NotifyIcon(NIM_DELETE, &NotifyIcon); //关闭时删除系统托盘图标  
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}




void CPersonalScheduleDlg::OnMinimize()
{
	// TODO: 在此添加命令处理程序代码
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
}


void CPersonalScheduleDlg::OnMinShow()
{
	// TODO: 在此添加命令处理程序代码
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
}



void CPersonalScheduleDlg::OnMusic()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   
	CMusicDlg mDlg;           // 构造对话框类CTipDlg的实例   
	nRes = mDlg.DoModal();  // 弹出对话框   
	if (IDCANCEL == nRes)     // 判断对话框退出后返回值是否为IDCANCEL，如果是则return，否则继续向下执行   
		return;
}


void CPersonalScheduleDlg::OnHoliday()
{
	// TODO: 在此添加命令处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   
	CFestivalDlg hDlg;           // 构造对话框类CTipDlg的实例   
	nRes = hDlg.DoModal();  // 弹出对话框   
	if (IDCANCEL == nRes)     // 判断对话框退出后返回值是否为IDCANCEL，如果是则return，否则继续向下执行   
		return;
}


void CPersonalScheduleDlg::OnHelp()
{
	// TODO: 在此添加命令处理程序代码

	MessageBox(_T("个人日程管理系统 1.0 版\r\n         版权所有 (C)2017\r\n              -zhang"), _T("系统信息"), NULL);
}

//定时提醒
void CPersonalScheduleDlg::OnBnClickedButton3()
{
	KillTimer(1);
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   
	CTimeDlg tDlg;           // 构造对话框类CTipDlg的实例   
	nRes = tDlg.DoModal();  // 弹出对话框
	SetTimer(1, 1000, NULL);
	if (IDCANCEL == nRes)     // 判断对话框退出后返回值是否为IDCANCEL，如果是则return，否则继续向下执行   
		return;
}


void CPersonalScheduleDlg::OnBnClickedDairy()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   
	CDairyDlg dDlg;           // 构造对话框类CTipDlg的实例   
	nRes = dDlg.DoModal();  // 弹出对话框   
	if (IDCANCEL == nRes)     // 判断对话框退出后返回值是否为IDCANCEL，如果是则return，否则继续向下执行   
		return;
}


void CPersonalScheduleDlg::OnBnClickedMemo()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   
	CMemoDlg mDlg;           // 构造对话框类CTipDlg的实例   
	nRes = mDlg.DoModal();  // 弹出对话框   
	if (IDCANCEL == nRes)     // 判断对话框退出后返回值是否为IDCANCEL，如果是则return，否则继续向下执行   
		return;
}


void CPersonalScheduleDlg::CheckDate() 
{
	CString sys_dateFinal, path, name, vol;
	sys_dateFinal = InitDate();
	//AfxMessageBox(_T("test"));
	AdoAccess data;				 // ADOConn类对象
	data.OnInitADOConn();		 //连接数据库
	CString date, title, content, isRemind;
	_bstr_t sql;
	sql = "select * from datetable";                         //设置查询语句
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
			MessageBox(_T("你今天的日程为:\r\n标题：" + title + "\r\n内容：" + content), _T("日程提醒"), MB_ICONQUESTION);
			data.m_pRecordset->PutCollect("d_remind", (_bstr_t)"TRUE");
			data.m_pRecordset->Update();
			SetTimer(0, 60000, NULL);
		}
		data.m_pRecordset->MoveNext(); //将记录集指针移动到下一条记录
	}
	stop(myDevice);
	data.ExitConnect(); //断开数据库连接
}


void CPersonalScheduleDlg::CheckTime()
{
	CString sys_timeFinal;
	sys_timeFinal = InitTime();
	COleDateTime s_time;
	s_time = COleDateTime::GetCurrentTime();
	int sys_weekday = s_time.GetDayOfWeek();
	//AfxMessageBox(_T("test"));
	AdoAccess data;				 // ADOConn类对象
	data.OnInitADOConn();		 //连接数据库
	CString time, title, content, isRemind,type;
	int weekday;
	_bstr_t sql;
	sql = "select * from datetable where d_type<>'定期提醒'";                         //设置查询语句
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
			if (type == "一次提醒")
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
					MessageBox(time + _T("定时提醒:\r\n标题：" + title + "\r\n内容：" + content), _T("日程提醒"), MB_ICONQUESTION);
					data.m_pRecordset->PutCollect("d_remind", (_bstr_t)"TRUE");
					data.m_pRecordset->Update();
					SetTimer(1, 1000, NULL);
				}
			}
			if (type == "每天提醒" || (type == "每周提醒"&&sys_weekday == weekday))
			{
				if (!flag)
				{
					myDevice = play();
					flag = true;
				}
				KillTimer(1);
				MessageBox(time.Mid(time.Find(_T(" ")) + 1) + _T("定时提醒:\r\n标题：" + title + "\r\n内容：" + content), _T("日程提醒"), MB_ICONQUESTION);
				data.m_pRecordset->PutCollect("d_remind", (_bstr_t)"TRUE");
				data.m_pRecordset->Update();
				SetTimer(1, 1000, NULL);
			}
		}
		data.m_pRecordset->MoveNext(); //将记录集指针移动到下一条记录
	}
	stop(myDevice);
	data.ExitConnect(); //断开数据库连接
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
	MCI_OPEN_PARMS openParms;//MCI_OPEN命令需要参数结构体  
	openParms.lpstrDeviceType = _T("MPEGvideo");//MP3的文件设备ID为MPEGvideo  
	openParms.lpstrElementName = path;//MP3文件的存放路径  
									  //发送命令  
	mciSendCommand(
		NULL,//打开设备不需要设备ID  
		MCI_OPEN,//MCI_OPEN,此命令表示打开设备  
		MCI_OPEN_ELEMENT | MCI_OPEN_TYPE | MCI_WAIT,
		(DWORD)(LPVOID(&openParms)));//传递MCI_OPEN_PARMS参数  
	WORD m_wDeviceID = openParms.wDeviceID;
	//2、播放设备  
	MCI_PLAY_PARMS playParms;//MCI_PLAY命令需要的参数结构体  
	mciSendCommand(
		m_wDeviceID,//需要的设备ID  
		MCI_PLAY,//此命令播放设备  
		MCI_NOTIFY,//等待播放文件结束，然后返回  
		(DWORD)(LPVOID(&playParms)));//传递MCI_PLAY_PARMS参数  

	MCI_DGV_SETAUDIO_PARMS setvolume; //设置音量的参数
	setvolume.dwCallback = NULL; //
	setvolume.dwItem = MCI_DGV_SETAUDIO_VOLUME; //动作是设置音量
	setvolume.dwValue = _ttoi(vol) * 10; //音量值是vol
	mciSendCommand(m_wDeviceID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD)(LPVOID)&setvolume);

	return m_wDeviceID;
}

void stop(DWORD DeviceId)
{
	mciSendCommand(DeviceId, MCI_STOP, 0, 0);
	mciSendCommand(DeviceId, MCI_CLOSE, 0, 0);
}
