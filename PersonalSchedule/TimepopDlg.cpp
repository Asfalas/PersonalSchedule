// TimepopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "TimepopDlg.h"
#include "afxdialogex.h"
#include "AdoAccess.h"

// CTimepopDlg 对话框

IMPLEMENT_DYNAMIC(CTimepopDlg, CDialogEx)

CTimepopDlg::CTimepopDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEWTIME, pParent)
	, m_ttime(COleDateTime::GetCurrentTime())
	, m_radio(0)
	, m_ttitle(_T(""))
	, m_tdate(COleDateTime::GetCurrentTime())
	, m_tcontent(_T(""))
{

}


CTimepopDlg::~CTimepopDlg()
{
}

void CTimepopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_TTIME, m_ttime);
	DDX_Radio(pDX, IDC_RADIO_ONCE, m_radio);
	DDV_MinMaxInt(pDX, m_radio, 0, 2);
	DDX_Text(pDX, IDC_TCONTENT, m_ttitle);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_tdate);
	DDX_Text(pDX, IDC_EDIT1, m_tcontent);
}


BEGIN_MESSAGE_MAP(CTimepopDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTimepopDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_ONCE, &CTimepopDlg::OnBnClickedRadioOnce)
	ON_BN_CLICKED(IDC_RADIO_PERDAY, &CTimepopDlg::OnBnClickedRadioPerday)
	ON_BN_CLICKED(IDC_RADIO_PERWEEK, &CTimepopDlg::OnBnClickedRadioPerweek)
END_MESSAGE_MAP()


// CTimepopDlg 消息处理程序


void CTimepopDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_ttitle == _T("")|| m_tcontent == _T(""))
	{
		AfxMessageBox(_T("提醒内容不能为空"));
		return;
	}

	COleDateTime sys_time;
	sys_time = COleDateTime::GetCurrentTime();

	int hour = m_ttime.GetHour();
	int minute = m_ttime.GetMinute();
	int second = m_ttime.GetSecond();
	int year = m_tdate.GetYear();
	int month = m_tdate.GetMonth();
	int day = m_tdate.GetDay();
	int weekday = m_tdate.GetDayOfWeek();

	CString hourTime, minuteTime, secondTime, yearTime, mounthTime, dayTime;

	yearTime.Format(_T("%d"), year);
	mounthTime.Format(_T("%d"), month);
	dayTime.Format(_T("%d"), day);
	hourTime.Format(_T("%d"), hour);
	minuteTime.Format(_T("%d"), minute);
	secondTime.Format(_T("%d"), second);
	CString timeFinal,m_type;
	timeFinal.Format(_T("%s-%s-%s %s:%s:%s"), yearTime, mounthTime, dayTime,hourTime,minuteTime,secondTime);
	switch (m_radio)
	{
		case 0:
			m_type = _T("一次提醒");
			break;
		case 1:
			m_type = _T("每天提醒");
			break;
		case 2: 
			m_type = _T("每周提醒");
			break;
		default:
			break;
	}
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from datetable";
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		database.m_pRecordset->AddNew(); //添加新行
		database.m_pRecordset->PutCollect("d_date", (_bstr_t)timeFinal);
		database.m_pRecordset->PutCollect("d_title", (_bstr_t)m_ttitle);
		database.m_pRecordset->PutCollect("d_content", (_bstr_t)m_tcontent);
		database.m_pRecordset->PutCollect("d_type", (_bstr_t)m_type);
		database.m_pRecordset->PutCollect("d_weekday", (_bstr_t)weekday);
		database.m_pRecordset->PutCollect("d_remind", (_bstr_t)"FALSE");
		database.m_pRecordset->Update();
		database.ExitConnect();
	}
	catch (...)
	{
		MessageBox(_T("操作失败"));
		return;
	}
	MessageBox(_T("操作成功"));
	CDialogEx::OnOK();
}


void CTimepopDlg::OnBnClickedRadioOnce()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radio = 0;
}


void CTimepopDlg::OnBnClickedRadioPerday()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radio = 1;
}


void CTimepopDlg::OnBnClickedRadioPerweek()
{
	// TODO: 在此添加控件通知处理程序代码
	m_radio = 2;
}
