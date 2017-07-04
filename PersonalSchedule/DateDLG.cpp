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
	m_jr_Grid.GetClientRect(&rect);
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_jr_Grid.SetExtendedStyle(m_jr_Grid.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Ϊ�б���ͼ�ؼ��������   
	m_jr_Grid.InsertColumn(0, _T("����"), LVCFMT_CENTER, rect.Width() / 3, 0);
	m_jr_Grid.InsertColumn(1, _T("����"), LVCFMT_CENTER, rect.Width() / 3, 1);
	m_jr_Grid.InsertColumn(2, _T("����"), LVCFMT_CENTER, rect.Width() / 3, 2);
	AddtoGrid();
	GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
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
	ON_BN_CLICKED(IDC_DEL, &CDateDLG::OnBnClickedDel)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDateDLG::OnNMClickList1)
	ON_BN_CLICKED(IDC_CHANGE, &CDateDLG::OnBnClickedChange)
	ON_BN_CLICKED(IDC_CLEAR, &CDateDLG::OnBnClickedClear)
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
		database.m_pRecordset->PutCollect("d_remind", (_bstr_t)"FALSE");
		database.m_pRecordset->PutCollect("d_type", (_bstr_t)"��������");
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
	m_detail = _T("");
	m_title = _T("");
	m_content = _T("");
	UpdateData(FALSE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDateDLG::AddtoGrid()
{
	AdoAccess m_ADOConn;       // ADOConn�����
	m_ADOConn.OnInitADOConn(); //�������ݿ�
	CString sql;
	int i = 0;
	sql.Format(_T("select * from datetable where d_type='��������' order by d_date asc"));                         //���ò�ѯ���
	m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //��ѯ
	while (!m_ADOConn.m_pRecordset->adoEOF)
	{
		//���б���ͼ�ؼ��в�����IDC_STATIC_ZZH_CONTENTIDC_STATIC_ZZH_TITLE
		m_jr_Grid.InsertItem(i, _T(""));
		//���б���ͼ�ؼ��в�����
		m_jr_Grid.SetItemText(i, 0, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_date")));
		m_jr_Grid.SetItemText(i, 1, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_title")));
		m_jr_Grid.SetItemText(i, 2, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_content")));
		m_ADOConn.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
		i++;
	}
	m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
}

void CDateDLG::OnBnClickedDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from datetable where d_type='��������' order by d_date asc";
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
	m_jr_Grid.DeleteAllItems();
	AddtoGrid();
	m_detail = _T("");
	m_title = _T("");
	m_content = _T("");
	UpdateData(FALSE);
}

void CDateDLG::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int dq_nItem = -1;
	LPNMITEMACTIVATE lpNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
	if (lpNMItemActivate != NULL)
	{
		dq_nItem = lpNMItemActivate->iItem;
	}
	pos = m_jr_Grid.GetSelectionMark();
	m_detail = _T("���⣺") + m_jr_Grid.GetItemText(dq_nItem, 1) + "\r\n" + _T("���ݣ�") + m_jr_Grid.GetItemText(dq_nItem, 2);
	m_title = m_jr_Grid.GetItemText(pos, 1);
	m_content = m_jr_Grid.GetItemText(pos, 2);
	UpdateData(FALSE);
}

void CDateDLG::OnBnClickedChange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�õ�ѡ���ʱ��
	UpdateData(TRUE);
	AdoAccess data;
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
	if (m_title.IsEmpty() || m_content.IsEmpty())
	{
		MessageBox(_T("������Ϣ����Ϊ�գ�"));
		return;
	}

	data.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from datetable where d_type='��������' order by d_date asc";
	data.m_pRecordset.CreateInstance(__uuidof(Recordset));
	data.m_pRecordset->Open(sql, data.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		data.m_pRecordset->Move((long)pos, vtMissing);
		data.m_pRecordset->PutCollect("d_date", (_bstr_t)timeFinal);
		data.m_pRecordset->PutCollect("d_title", (_bstr_t)m_title);
		data.m_pRecordset->PutCollect("d_content", (_bstr_t)m_content);
		data.m_pRecordset->PutCollect("d_remind", (_bstr_t)"FALSE");
		data.m_pRecordset->Update();
		data.ExitConnect();
	}
	catch (_com_error &e)
	{
		AfxMessageBox(_T("����ʧ��"));
		CString str;
		str.Format(_T("%s    %s"), (LPCTSTR)e.Description(), \
			(LPCTSTR)e.ErrorMessage());
		AfxMessageBox(str);
		data.ExitConnect();
	}
	MessageBox(_T("�޸ĳɹ�"));
	m_jr_Grid.DeleteAllItems();
	AddtoGrid();
}

void CDateDLG::OnBnClickedClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;

	// ��ʾ��Ϣ�Ի���   
	nRes = MessageBox(_T("��ȷ��Ҫ������������ݽ��޷��ָ�"), _T("��Ϣ����"), MB_OKCANCEL | MB_ICONQUESTION);
	// �ж���Ϣ�Ի��򷵻�ֵ�����ΪIDCANCEL��return�������������ִ��   
	if (IDCANCEL == nRes)
		return;
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from datetable where d_type='��������'";
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	while (!database.m_pRecordset->adoEOF)
	{
		try
		{
			database.m_pRecordset->Delete(adAffectCurrent);
			database.m_pRecordset->Update();
			database.m_pRecordset->MoveNext();

		}
		catch (...)
		{
			MessageBox(_T("����ʧ��"));
			return;
		}
	}
	database.ExitConnect();
	MessageBox(_T("��ճɹ�"));
	m_jr_Grid.DeleteAllItems();
	AddtoGrid();
	m_detail = _T("");
	m_title = _T("");
	m_content = _T("");
	UpdateData(FALSE);
}
