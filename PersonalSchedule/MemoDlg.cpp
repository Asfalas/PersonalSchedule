// MemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "MemoDlg.h"
#include "afxdialogex.h"
#include "AdoAccess.h"

// CMemoDlg �Ի���

IMPLEMENT_DYNAMIC(CMemoDlg, CDialogEx)

CMemoDlg::CMemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MEMO, pParent)
	, m_mcontent(_T(""))
	, m_mdate(COleDateTime::GetCurrentTime())
	, m_mcurrent(_T(""))
{

}

CMemoDlg::~CMemoDlg()
{
}

void CMemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MCONTENT, m_mcontent);
	DDX_DateTimeCtrl(pDX, IDC_MDATE, m_mdate);
	DDX_Control(pDX, IDC_MTITLE, m_mtitle);
	DDX_CBString(pDX, IDC_MTITLE, m_mcurrent);
}


BEGIN_MESSAGE_MAP(CMemoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MADD, &CMemoDlg::OnBnClickedMadd)
	ON_BN_CLICKED(IDC_MCHANGE, &CMemoDlg::OnBnClickedMchange)
	ON_BN_CLICKED(IDC_MDEL, &CMemoDlg::OnBnClickedMdel)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_MDATE, &CMemoDlg::OnDtnDatetimechangeMdate)
	ON_CBN_SELCHANGE(IDC_MTITLE, &CMemoDlg::OnCbnSelchangeMtitle)
END_MESSAGE_MAP()


// CMemoDlg ��Ϣ�������
BOOL CMemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString date;
	date = InitTime();
	AfxMessageBox(date);
	AddtoGrid(date);
	return TRUE;  // return TRUE  unless you set the focus to a control   
}


void CMemoDlg::AddtoGrid(CString date)
{
	m_mtitle.ResetContent();
	AdoAccess m_ADOConn;       // ADOConn�����
	m_ADOConn.OnInitADOConn(); //�������ݿ�
	CString sql;
	sql.Format(_T("select * from memotable where m_date='" + date + "'"));                         //���ò�ѯ���
	m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //��ѯ
	if (!m_ADOConn.m_pRecordset->adoEOF)
	{
		m_mcurrent = m_ADOConn.m_pRecordset->GetCollect("m_title");
		m_mcontent = m_ADOConn.m_pRecordset->GetCollect("m_content");
	}
	else
	{
		m_mcurrent = _T("");
		m_mcontent = _T("");
	}
	while (!m_ADOConn.m_pRecordset->adoEOF)
	{
		m_mtitle.AddString((LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("m_title")));
		m_ADOConn.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
	}
	m_mtitle.SetCurSel(0);
	m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
	UpdateData(FALSE);
}


CString CMemoDlg::InitTime()
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


void CMemoDlg::OnBnClickedMadd()
{
	UpdateData(TRUE);
	if (!CheckTitle(m_mcurrent, -1))
	{
		AfxMessageBox(_T("���ոñ����ѱ���ӻ��������Ϊ��"));
		AddtoGrid(my_date);
		return;
	}
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from memotable";
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		database.m_pRecordset->AddNew(); //�������
		database.m_pRecordset->PutCollect("m_date", (_bstr_t)my_date);
		database.m_pRecordset->PutCollect("m_title", (_bstr_t)m_mcurrent);
		database.m_pRecordset->PutCollect("m_content", (_bstr_t)m_mcontent);
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


BOOL CMemoDlg::CheckTitle(CString title, int place)
{
	AdoAccess m_ADOConn;       // ADOConn�����
	m_ADOConn.OnInitADOConn(); //�������ݿ�
	CString sql;
	sql.Format(_T("select * from memotable where m_date='" + my_date + "'"));                         //���ò�ѯ���
	m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //��ѯ
	while (!m_ADOConn.m_pRecordset->adoEOF)
	{
		if (pos == place)
		{
			m_ADOConn.m_pRecordset->MoveNext();
			continue;
		}
		if (title == ((LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("m_title"))) || title.IsEmpty())
		{
			m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
			return false;
		}
		m_ADOConn.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
	}
	m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
	return true;
}


void CMemoDlg::OnBnClickedMchange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (!CheckTitle(m_mcurrent, -1))
	{
		AfxMessageBox(_T("���ոñ����ѱ����ڻ��������Ϊ��"));
		AddtoGrid(my_date);
		return;
	}
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = _T("select * from memotable where m_date = '" + my_date + "'");
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		database.m_pRecordset->Move(pos, vtMissing);
		database.m_pRecordset->PutCollect("m_date", (_bstr_t)my_date);
		database.m_pRecordset->PutCollect("m_title", (_bstr_t)m_mcurrent);
		database.m_pRecordset->PutCollect("m_content", (_bstr_t)m_mcontent);
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


void CMemoDlg::OnBnClickedMdel()
{
	UpdateData(TRUE);
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = _T("select * from memotable where m_date = '" + my_date + "'");
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


void CMemoDlg::OnDtnDatetimechangeMdate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int year = m_mdate.GetYear();
	int mounth = m_mdate.GetMonth();
	int day = m_mdate.GetDay();
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


void CMemoDlg::OnCbnSelchangeMtitle()
{
	CString strWeb;
	int nSel;
	// ��ȡ��Ͽ�ؼ����б����ѡ���������   
	nSel = m_mtitle.GetCurSel();
	pos = nSel;
	// ����ѡ����������ȡ�����ַ���   
	m_mtitle.GetLBText(nSel, strWeb);
	AdoAccess m_ADOConn;       // ADOConn�����
	m_ADOConn.OnInitADOConn(); //�������ݿ�
	CString sql;
	sql.Format(_T("select * from memotable where m_date='" + my_date + "'"));                         //���ò�ѯ���
	m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //��ѯ
	m_ADOConn.m_pRecordset->Move(pos, vtMissing);
	m_mcontent = m_ADOConn.m_pRecordset->GetCollect("m_content");
	m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
	UpdateData(FALSE);
}
