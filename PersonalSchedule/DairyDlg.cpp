// DairyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "DairyDlg.h"
#include "afxdialogex.h"
#include "AdoAccess.h"

// CDairyDlg �Ի���

IMPLEMENT_DYNAMIC(CDairyDlg, CDialogEx)

CDairyDlg::CDairyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DAIRY, pParent)
	, m_ddate(COleDateTime::GetCurrentTime())
	, m_dcontent(_T(""))
	, m_current(_T(""))
{

}

CDairyDlg::~CDairyDlg()
{
}

void CDairyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DCONTENT, m_dcontent);
	DDX_DateTimeCtrl(pDX, IDC_DDATE, m_ddate);
	DDX_Control(pDX, IDC_DTITLE, m_dtitle);
	DDX_CBString(pDX, IDC_DTITLE, m_current);
}


BEGIN_MESSAGE_MAP(CDairyDlg, CDialogEx) 
	ON_BN_CLICKED(IDC_DADD, &CDairyDlg::OnBnClickedDadd)
	ON_BN_CLICKED(IDC_DCHANGE, &CDairyDlg::OnBnClickedDchange)
	ON_BN_CLICKED(IDC_DDEL, &CDairyDlg::OnBnClickedDdel)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DDATE, &CDairyDlg::OnDtnDatetimechangeDdate)
	ON_CBN_SELCHANGE(IDC_DTITLE, &CDairyDlg::OnCbnSelchangeDtitle)
END_MESSAGE_MAP()


// CDairyDlg ��Ϣ�������
BOOL CDairyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString date;
	date = InitTime();
	AddtoGrid(date);
	return TRUE;  // return TRUE  unless you set the focus to a control   
}


void CDairyDlg::AddtoGrid(CString date)
{
	m_dtitle.ResetContent();
	AdoAccess m_ADOConn;       // ADOConn�����
	m_ADOConn.OnInitADOConn(); //�������ݿ�
	CString sql;
	sql.Format(_T("select * from dairytable where d_date='"+date+"'"));                         //���ò�ѯ���
	m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //��ѯ
	if (!m_ADOConn.m_pRecordset->adoEOF)
	{
		m_current = m_ADOConn.m_pRecordset->GetCollect("d_title");
		m_dcontent = m_ADOConn.m_pRecordset->GetCollect("d_content");
	}
	else
	{
		m_current = _T("");
		m_dcontent = _T("");
	}
	while (!m_ADOConn.m_pRecordset->adoEOF)
	{
		m_dtitle.AddString((LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_title")));
		m_ADOConn.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
	}
	m_dtitle.SetCurSel(0);
	m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
	UpdateData(FALSE);
}


CString CDairyDlg::InitTime()
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
	my_date = sys_timeFinal;
	return sys_timeFinal;
}


BOOL CDairyDlg::CheckTitle(CString title,int place) 
{
	AdoAccess m_ADOConn;       // ADOConn�����
	m_ADOConn.OnInitADOConn(); //�������ݿ�
	CString sql;
	sql.Format(_T("select * from dairytable where d_date='" + my_date + "'"));                         //���ò�ѯ���
	m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //��ѯ
	while (!m_ADOConn.m_pRecordset->adoEOF)
	{
		if (pos == place)
		{
			m_ADOConn.m_pRecordset->MoveNext();
			continue;
		}
		if (title == ((LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_title")))||title.IsEmpty())
		{
			m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
			return false;
		}
		m_ADOConn.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
	}
	m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
	return true;
}


void CDairyDlg::OnBnClickedDadd()
{
	UpdateData(TRUE);
	if (!CheckTitle(m_current,-1))
	{
		AfxMessageBox(_T("���ոñ����ѱ���ӻ��������Ϊ��"));
		AddtoGrid(my_date);
		return;
	}
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from dairytable";
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		database.m_pRecordset->AddNew(); //�������
		database.m_pRecordset->PutCollect("d_date", (_bstr_t)my_date);
		database.m_pRecordset->PutCollect("d_title", (_bstr_t)m_current);
		database.m_pRecordset->PutCollect("d_content", (_bstr_t)m_dcontent);
		database.m_pRecordset->Update();
		database.ExitConnect();
	}
	catch (...)
	{
		MessageBox(_T("����ʧ��"));
		return;
	}
	MessageBox(_T("��ӳɹ�"));
	AddtoGrid(my_date);
	UpdateData(FALSE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDairyDlg::OnBnClickedDchange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (!CheckTitle(m_current, -1))
	{
		AfxMessageBox(_T("���ոñ����ѱ����ڻ��������Ϊ��"));
		AddtoGrid(my_date);
		return;
	}
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = _T("select * from dairytable where d_date = '"+my_date+"'");
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		database.m_pRecordset->Move(pos, vtMissing);
		database.m_pRecordset->PutCollect("d_date", (_bstr_t)my_date);
		database.m_pRecordset->PutCollect("d_title", (_bstr_t)m_current);
		database.m_pRecordset->PutCollect("d_content", (_bstr_t)m_dcontent);
		database.m_pRecordset->Update();
		database.ExitConnect();
	}
	catch (...)
	{
		MessageBox(_T("ɾ��ʧ��"));
		return;
	}
	MessageBox(_T("�޸ĳɹ�"));
	AddtoGrid(my_date);
	UpdateData(FALSE);
}


void CDairyDlg::OnBnClickedDdel()
{
	UpdateData(TRUE);
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = _T("select * from dairytable where d_date = '" + my_date + "'");
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		database.m_pRecordset->Move(pos, vtMissing);
		database.m_pRecordset->Delete(adAffectCurrent);
		database.m_pRecordset->Update();
		database.ExitConnect();
	}
	catch (...)
	{
		MessageBox(_T("����ʧ��"));
		return;
	}
	MessageBox(_T("ɾ���ɹ�"));
	AddtoGrid(my_date);
	UpdateData(FALSE);
}


void CDairyDlg::OnDtnDatetimechangeDdate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int year = m_ddate.GetYear();
	int mounth = m_ddate.GetMonth();
	int day = m_ddate.GetDay();
	CString yearTime;
	CString mounthTime;
	CString dayTime;
	yearTime.Format(_T("%d"), year);
	mounthTime.Format(_T("%d"), mounth);
	dayTime.Format(_T("%d"), day);
	my_date.Format(_T("%s/%s/%s"), yearTime, mounthTime, dayTime);
	AddtoGrid(my_date);
	*pResult = 0;
}


void CDairyDlg::OnCbnSelchangeDtitle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strWeb;
	int nSel;
	// ��ȡ��Ͽ�ؼ����б����ѡ���������   
	nSel = m_dtitle.GetCurSel();
	pos = nSel;
	// ����ѡ����������ȡ�����ַ���   
	m_dtitle.GetLBText(nSel, strWeb);
	AdoAccess m_ADOConn;       // ADOConn�����
	m_ADOConn.OnInitADOConn(); //�������ݿ�
	CString sql;
	sql.Format(_T("select * from dairytable where d_date='" + my_date + "'"));                         //���ò�ѯ���
	m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //��ѯ
	m_ADOConn.m_pRecordset->Move(pos, vtMissing);
	m_dcontent = m_ADOConn.m_pRecordset->GetCollect("d_content");
	m_current = m_ADOConn.m_pRecordset->GetCollect("d_title");
	m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
	UpdateData(FALSE);
	//SetDlgItemText(IDC_COMBO1, strWeb);
}
