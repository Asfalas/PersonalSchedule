// CtimepopDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "CtimepopDlg.h"
#include "afxdialogex.h"
#include "AdoAccess.h"

// CCtimepopDlg �Ի���

IMPLEMENT_DYNAMIC(CCtimepopDlg, CDialogEx)

CCtimepopDlg::CCtimepopDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CTIME, pParent)
	, m_cdate(COleDateTime::GetCurrentTime())
	, m_ctime(COleDateTime::GetCurrentTime())
	, m_radio1(0)
	, m_ctitle(_T(""))
	, m_ccontent(_T(""))
{

}

CCtimepopDlg::CCtimepopDlg(int pos, CWnd * pParent)
	: CDialogEx(IDD_CTIME, pParent)
	, m_cdate(COleDateTime::GetCurrentTime())
	, m_ctime(COleDateTime::GetCurrentTime())
	, m_radio1(0)
	, m_ctitle(_T(""))
	, m_ccontent(_T(""))
	, pos(pos)
{
}

CCtimepopDlg::~CCtimepopDlg()
{
}

void CCtimepopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_cdate);
	//  DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_ctime);
	//  DDX_Text(pDX, IDC_EDIT2, m_ctitle);
	//  DDX_Text(pDX, IDC_EDIT3, m_ccontent);
	DDX_DateTimeCtrl(pDX, IDC_CDATE, m_cdate);
	DDX_DateTimeCtrl(pDX, IDC_CTIME, m_ctime);
	DDX_Radio(pDX, IDC_RADIO1, m_radio1);
	DDX_Text(pDX, IDC_CTITLE, m_ctitle);
	DDX_Text(pDX, IDC_CCONTENT, m_ccontent);
}


BEGIN_MESSAGE_MAP(CCtimepopDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCtimepopDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CCtimepopDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CCtimepopDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CCtimepopDlg::OnBnClickedRadio3)
END_MESSAGE_MAP()


// CCtimepopDlg ��Ϣ�������
BOOL CCtimepopDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect;

	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from datetable where d_type<>'��������' order by d_date asc";
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		database.m_pRecordset->Move(pos, vtMissing);
		m_ctitle = database.m_pRecordset->GetCollect("d_title");
		m_ccontent = database.m_pRecordset->GetCollect("d_content");
		database.ExitConnect();
	}
	catch (...)
	{
		MessageBox(_T("����ʧ��"));
		return TRUE;
	}
	//MessageBox(_T("ɾ���ɹ�"));
	UpdateData(FALSE);
	return TRUE;
}

void CCtimepopDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_ctitle == _T("") || m_ccontent == _T(""))
	{
		AfxMessageBox(_T("�������ݲ���Ϊ��"));
		return;
	}

	COleDateTime sys_time;
	sys_time = COleDateTime::GetCurrentTime();

	int hour = m_ctime.GetHour();
	int minute = m_ctime.GetMinute();
	int second = m_ctime.GetSecond();
	int year = m_cdate.GetYear();
	int month = m_cdate.GetMonth();
	int day = m_cdate.GetDay();
	int weekday = m_cdate.GetDayOfWeek();

	CString hourTime, minuteTime, secondTime, yearTime, mounthTime, dayTime;

	yearTime.Format(_T("%d"), year);
	mounthTime.Format(_T("%d"), month);
	dayTime.Format(_T("%d"), day);
	hourTime.Format(_T("%d"), hour);
	minuteTime.Format(_T("%d"), minute);
	secondTime.Format(_T("%d"), second);
	CString timeFinal, m_type;
	timeFinal.Format(_T("%s-%s-%s %s:%s:%s"), yearTime, mounthTime, dayTime, hourTime, minuteTime, secondTime);
	switch (m_radio1)
	{
	case 0:
		m_type = _T("һ������");
		break;
	case 1:
		m_type = _T("ÿ������");
		break;
	case 2:
		m_type = _T("ÿ������");
		break;
	default:
		break;
	}
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from datetable where d_type<>'��������' order by d_date asc";
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		database.m_pRecordset->Move(pos, vtMissing);
		database.m_pRecordset->PutCollect("d_date", (_bstr_t)timeFinal);
		database.m_pRecordset->PutCollect("d_title", (_bstr_t)m_ctitle);
		database.m_pRecordset->PutCollect("d_content", (_bstr_t)m_ccontent);
		database.m_pRecordset->PutCollect("d_type", (_bstr_t)m_type);
		database.m_pRecordset->PutCollect("d_weekday", (_bstr_t)weekday);
		database.m_pRecordset->PutCollect("d_remind", (_bstr_t)"FALSE");
		database.m_pRecordset->Update();
		database.ExitConnect();
	}
	catch (...)
	{
		MessageBox(_T("����ʧ��"));
		return;
	}
	MessageBox(_T("�����ɹ�"));
	CDialogEx::OnOK();
}


void CCtimepopDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_radio1 = 0;
}


void CCtimepopDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_radio1 = 1;
}


void CCtimepopDlg::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_radio1 = 2;
}
