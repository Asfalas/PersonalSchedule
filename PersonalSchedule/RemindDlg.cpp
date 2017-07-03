// RemindDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "RemindDlg.h"
#include "afxdialogex.h"
#include "AdoAccess.h"

// CRemindDlg 对话框

IMPLEMENT_DYNAMIC(CRemindDlg, CDialogEx)

CRemindDlg::CRemindDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REMIND, pParent)
	, m_rtime(_T(""))
	, m_rtitle(_T(""))
	, m_rcontent(_T(""))
	, m_rcurrent(_T(""))
{

}

CRemindDlg::CRemindDlg(const CRemind& remind, CWnd * pParent)
	: CDialogEx(IDD_REMIND, pParent)
	, m_rtime(_T(""))
	, m_rtitle(_T(""))
	, m_rcontent(_T(""))
	, m_rcurrent(_T(""))
	, remind(remind)
{
}

CRemindDlg::~CRemindDlg()
{
}

void CRemindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RTIME, m_rtime);
	DDX_Text(pDX, IDC_RTITLE, m_rtitle);
	DDX_Text(pDX, IDC_RCONTENT, m_rcontent);
	DDX_CBString(pDX, IDC_LATER, m_rcurrent);
	DDX_Control(pDX, IDC_LATER, m_later);
}


BEGIN_MESSAGE_MAP(CRemindDlg, CDialogEx)
	ON_EN_CHANGE(IDC_RTIME, &CRemindDlg::OnEnChangeRtime)
	ON_CBN_SELCHANGE(IDC_LATER, &CRemindDlg::OnCbnSelchangeLater)
END_MESSAGE_MAP()


// CRemindDlg 消息处理程序

BOOL CRemindDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_rtime = remind.time;
	m_rtitle = remind.title;
	m_rcontent = remind.content;
	m_later.AddString((LPCTSTR)_bstr_t(_T("稍后提醒")));
	m_later.AddString((LPCTSTR)_bstr_t(_T("15分钟")));
	m_later.AddString((LPCTSTR)_bstr_t(_T("30分钟")));
	m_later.AddString((LPCTSTR)_bstr_t(_T("1小时")));
	
	CalTime(m_rtime.Right(m_rtime.GetLength()- m_rtime.Find(_T(" "))),0);
	m_later.SetCurSel(0);
	UpdateData(FALSE);
	return TRUE;
}


void CRemindDlg::OnEnChangeRtime()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CRemindDlg::OnCbnSelchangeLater()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strWeb;
	int nSel;
	// 获取组合框控件的列表框中选中项的索引   
	nSel = m_later.GetCurSel();
	// 根据选中项索引获取该项字符串   
	
	//m_later.GetLBText(nSel, strWeb);
	
	AdoAccess database;       // ADOConn类对象
	database.OnInitADOConn(); //连接数据库
	_bstr_t sql;
	sql = "select * from latertable";
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		database.m_pRecordset->AddNew(); //添加新行
		database.m_pRecordset->PutCollect("l_time", (_bstr_t)m_rtime);
		database.m_pRecordset->PutCollect("l_title", (_bstr_t)m_rtitle);
		database.m_pRecordset->PutCollect("l_content", (_bstr_t)m_rcontent);
		database.m_pRecordset->Update();
		database.ExitConnect();
	}
	catch (...)
	{
		MessageBox(_T("操作失败"));
		return;
	}
	UpdateData(FALSE);
	//SetDlgItemText(IDC_COMBO1, strWeb);
}

CString CRemindDlg::CalTime(CString time,int sel) 
{
	int hour, minute, second;
	CString hourtime, minutetime, secondtime;
	hourtime = time.Left(time.Find(_T(":")));
	minutetime = time.Right(time.GetLength() - time.Find(_T(":"))-1);
	minutetime = minutetime.Left(minutetime.Find(_T(":")));
	secondtime = time.Right(time.GetLength() - 2 - hourtime.GetLength() - minutetime.GetLength());
	hour = _ttoi(hourtime);
	minute = _ttoi(minutetime);
	second = _ttoi(secondtime);
	switch (sel) 
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;

	}
	return CString();
}