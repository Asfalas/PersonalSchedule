// RemindDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "RemindDlg.h"
#include "afxdialogex.h"
#include "AdoAccess.h"

// CRemindDlg �Ի���

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
	ON_BN_CLICKED(IDOK, &CRemindDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRemindDlg ��Ϣ�������

BOOL CRemindDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_rtime = remind.time;
	m_rtitle = remind.title;
	m_rcontent = remind.content;
	m_later.AddString((LPCTSTR)_bstr_t(_T("�Ժ�����")));
	m_later.AddString((LPCTSTR)_bstr_t(_T("15����")));
	m_later.AddString((LPCTSTR)_bstr_t(_T("30����")));
	m_later.AddString((LPCTSTR)_bstr_t(_T("1Сʱ")));
	m_later.SetCurSel(0);
	UpdateData(FALSE);
	return TRUE;
}


void CRemindDlg::OnEnChangeRtime()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CRemindDlg::OnCbnSelchangeLater()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strWeb;
	int nSel;
	nSel = m_later.GetCurSel();
	//AfxMessageBox(m_rtime);
	m_rtime = CalTime(m_rtime, nSel);
	//AfxMessageBox(m_rtime);
	AdoAccess database;       // ADOConn�����
	database.OnInitADOConn(); //�������ݿ�
	_bstr_t sql;
	sql = "select * from latertable";
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		database.m_pRecordset->AddNew(); //�������
		database.m_pRecordset->PutCollect("l_time", (_bstr_t)m_rtime);
		database.m_pRecordset->PutCollect("l_title", (_bstr_t)m_rtitle);
		database.m_pRecordset->PutCollect("l_content", (_bstr_t)m_rcontent);
		database.m_pRecordset->Update();
		database.ExitConnect();
	}
	catch (...)
	{
		MessageBox(_T("����ʧ��"));
		return;
	}
	CDialogEx::OnOK();
}


CString CRemindDlg::CalTime(CString time, int sel)
{
	int hour, minute, second;
	CString hourtime, minutetime, secondtime, date, timefinal;
	date = time.Left(time.Find(_T(" ")));
	time = time.Right(time.GetLength() - m_rtime.Find(_T(" ")) - 1);
	hourtime = time.Left(time.Find(_T(":")));
	time = time.Right(time.GetLength() - hourtime.GetLength() - 1);
	minutetime = time.Left(time.Find(_T(":")));
	secondtime = time.Right(time.GetLength() - minutetime.GetLength() - 1);
	hour = _ttoi(hourtime);
	minute = _ttoi(minutetime);
	second = _ttoi(secondtime);
	switch (sel)
	{
	case 0:
		if (minute + 1 > 60)
			hour = (++hour) % 24;
		minute = (++minute) % 60;
		break;
	case 1:
		if (minute + 15 > 60)
			hour = (++hour) % 24;
		minute = (minute + 15) % 60;
		break;
	case 2:
		if (minute + 30 > 60)
			hour = (++hour) % 24;
		minute = (minute + 30) % 60;
		break;
	case 3:
		hour += 1;
		break;
	default:
		break;
	}
	hourtime.Format(_T("%d"), hour);
	hourtime.Format(_T("%d"),hour);
	minutetime.Format(_T("%d"), minute);
	timefinal = date + " " + hourtime + ":" + minutetime + ":" + secondtime;
	return timefinal;
}


void CRemindDlg::OnBnClickedOk()
{
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from latertable where l_time='" + (_bstr_t)m_rtime + "'";
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		if (!database.m_pRecordset->adoEOF)
		{
			database.m_pRecordset->Delete(adAffectCurrent);
			database.m_pRecordset->Update();
		}
		database.ExitConnect();
	}
	catch (...)
	{
		MessageBox(_T("����ʧ��"));
		return;
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}
