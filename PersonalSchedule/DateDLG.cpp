// DateDLG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "DateDLG.h"
#include "afxdialogex.h"
#include "AdoAccess.h"

// CDateDLG �Ի���

IMPLEMENT_DYNAMIC(CDateDLG, CDialogEx)

CDateDLG::CDateDLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_time(COleDateTime::GetCurrentTime())
	, m_title(_T(""))
	, m_content(_T(""))
	, m_detail(_T(""))
{

}

CDateDLG::~CDateDLG()
{
}

BOOL CDateDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect;

	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_jr_Grid.GetClientRect(&rect);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_jr_Grid.SetExtendedStyle(m_jr_Grid.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Ϊ�б���ͼ�ؼ��������   
	m_jr_Grid.InsertColumn(0, _T("����"), LVCFMT_CENTER, rect.Width() / 3, 0);
	m_jr_Grid.InsertColumn(1, _T("����"), LVCFMT_CENTER, rect.Width() / 3, 1);
	m_jr_Grid.InsertColumn(2, _T("����"), LVCFMT_CENTER, rect.Width() / 3, 2);

	// ���б���ͼ�ؼ��в����б���������б������ı�   
	AdoAccess dq_m_ADOConn;       // ADOConn�����
	dq_m_ADOConn.OnInitADOConn(); //�������ݿ�
	CString dq_sql;
	int dq_ii = 0;
	dq_sql="select * from datetable";                         //���ò�ѯ���
	dq_m_ADOConn.m_pRecordset = dq_m_ADOConn.GetRecordSet((_bstr_t)dq_sql); //��ѯ
	while (!dq_m_ADOConn.m_pRecordset->adoEOF)
	{
		//���б���ͼ�ؼ��в�����IDC_STATIC_ZZH_CONTENTIDC_STATIC_ZZH_TITLE
		m_jr_Grid.InsertItem(dq_ii, _T(""));
		//���б���ͼ�ؼ��в�����
		m_jr_Grid.SetItemText(dq_ii, 0, (LPCTSTR)_bstr_t(dq_m_ADOConn.m_pRecordset->GetCollect("d_date")));
		m_jr_Grid.SetItemText(dq_ii, 1, (LPCTSTR)_bstr_t(dq_m_ADOConn.m_pRecordset->GetCollect("d_title")));
		m_jr_Grid.SetItemText(dq_ii, 2, (LPCTSTR)_bstr_t(dq_m_ADOConn.m_pRecordset->GetCollect("d_content")));
		dq_m_ADOConn.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
		dq_ii++;
	}
	dq_m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
	return TRUE;
}

void CDateDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_time);
	DDX_Text(pDX, IDC_EDIT1, m_title);
	DDX_Text(pDX, IDC_EDIT2, m_content);
	DDX_Control(pDX, IDC_LIST1, m_jr_Grid);
	DDX_Text(pDX, IDC_EDIT3, m_detail);
}


BEGIN_MESSAGE_MAP(CDateDLG, CDialogEx)
	ON_BN_CLICKED(IDC_NEW, &CDateDLG::OnBnClickedNew)
END_MESSAGE_MAP()


// CDateDLG ��Ϣ�������


void CDateDLG::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDateDLG::OnBnClickedNew()
{
	UpdateData(TRUE);
	int year = m_time.GetYear();
	int mounth = m_time.GetMonth();
	int day = m_time.GetDay();
	CString yearTime;
	CString mounthTime;
	CString dayTime;
	yearTime.Format(_T("%d"), year);
	mounthTime.Format(_T("%d"), mounth);
	dayTime.Format(_T("%d"), day);
	CString timeFinal;
	timeFinal.Format(_T("%s-%s-%s"), yearTime, mounthTime, dayTime);
	AdoAccess database;
	if (m_title.IsEmpty() || m_content.IsEmpty())
	{
		MessageBox(_T("���������ݣ�"));
		return;
	}
	database.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from datetable";
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		database.m_pRecordset->AddNew(); //�������
		database.m_pRecordset->PutCollect("d_date", (_bstr_t)timeFinal);
		database.m_pRecordset->PutCollect("d_title", (_bstr_t)m_title);
		database.m_pRecordset->PutCollect("d_content", (_bstr_t)m_content);
		database.m_pRecordset->Update();
		database.ExitConnect();
	}
	catch (...)
	{
		MessageBox(_T("����ʧ��"));
		return;
	}
	MessageBox(_T("��ӳɹ�"));
	m_jr_Grid.DeleteAllItems();
	AddtoGrid();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDateDLG::AddtoGrid()
{
	AdoAccess dq_m_ADOConn;       // ADOConn�����
	dq_m_ADOConn.OnInitADOConn(); //�������ݿ�
	CString dq_sql;
	int dq_ii = 0;
	dq_sql.Format(_T("select * from datetable"));                         //���ò�ѯ���
	dq_m_ADOConn.m_pRecordset = dq_m_ADOConn.GetRecordSet((_bstr_t)dq_sql); //��ѯ
	while (!dq_m_ADOConn.m_pRecordset->adoEOF)
	{
		//���б���ͼ�ؼ��в�����IDC_STATIC_ZZH_CONTENTIDC_STATIC_ZZH_TITLE
		m_jr_Grid.InsertItem(dq_ii, _T(""));
		//���б���ͼ�ؼ��в�����
		m_jr_Grid.SetItemText(dq_ii, 0, (LPCTSTR)_bstr_t(dq_m_ADOConn.m_pRecordset->GetCollect("d_date")));
		m_jr_Grid.SetItemText(dq_ii, 1, (LPCTSTR)_bstr_t(dq_m_ADOConn.m_pRecordset->GetCollect("d_title")));
		m_jr_Grid.SetItemText(dq_ii, 2, (LPCTSTR)_bstr_t(dq_m_ADOConn.m_pRecordset->GetCollect("d_content")));
		dq_m_ADOConn.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
		dq_ii++;
	}
	dq_m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
}