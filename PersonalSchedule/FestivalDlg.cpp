// FestivalDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "FestivalDlg.h"
#include "afxdialogex.h"
#include "AdoAccess.h"


// CFestivalDlg �Ի���

IMPLEMENT_DYNAMIC(CFestivalDlg, CDialogEx)

CFestivalDlg::CFestivalDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, m_htime(COleDateTime::GetCurrentTime())
	, m_htitle(_T(""))
	, m_hcontent(_T(""))
	, m_hdetail(_T(""))
{

}

CFestivalDlg::~CFestivalDlg()
{
}

void CFestivalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_HDATE, m_htime);
	DDX_Text(pDX, IDC_HTITLE, m_htitle);
	DDX_Text(pDX, IDC_HCONTENT, m_hcontent);
	DDX_Control(pDX, IDC_HLIST, m_hlist);
	DDX_Text(pDX, IDC_HDETAIL, m_hdetail);
}


BEGIN_MESSAGE_MAP(CFestivalDlg, CDialogEx)
	ON_BN_CLICKED(IDC_HNEW, &CFestivalDlg::OnBnClickedHnew)
	ON_BN_CLICKED(IDC_HDEL, &CFestivalDlg::OnBnClickedHdel)
	ON_NOTIFY(NM_CLICK, IDC_HLIST, &CFestivalDlg::OnNMClickHlist)
	ON_BN_CLICKED(IDC_HCHANGE, &CFestivalDlg::OnBnClickedHchange)
	ON_BN_CLICKED(IDC_HCLEAR, &CFestivalDlg::OnBnClickedHclear)
END_MESSAGE_MAP()


// CFestivalDlg ��Ϣ�������
BOOL CFestivalDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect;

	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_hlist.GetClientRect(&rect);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_hlist.SetExtendedStyle(m_hlist.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Ϊ�б���ͼ�ؼ��������   
	m_hlist.InsertColumn(0, _T("����"), LVCFMT_CENTER, rect.Width() / 4, 0);
	m_hlist.InsertColumn(1, _T("����"), LVCFMT_CENTER, rect.Width() / 4, 1);
	m_hlist.InsertColumn(2, _T("����"), LVCFMT_CENTER, rect.Width() / 4, 2);
	m_hlist.InsertColumn(3, _T("����"), LVCFMT_CENTER, rect.Width() / 4, 3);

	// ���б���ͼ�ؼ��в����б���������б������ı�   
	AdoAccess m_ADOConn;       // ADOConn�����
	m_ADOConn.OnInitADOConn(); //�������ݿ�
	CString sql;
	int i = 0;
	sql = "select * from holidaytable order by h_date asc";                         //���ò�ѯ���
	m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //��ѯ
	while (!m_ADOConn.m_pRecordset->adoEOF)
	{
		//���б���ͼ�ؼ��в�����IDC_STATIC_ZZH_CONTENTIDC_STATIC_ZZH_TITLE
		m_hlist.InsertItem(i, _T(""));
		//���б���ͼ�ؼ��в�����
		CString date = m_ADOConn.m_pRecordset->GetCollect("h_date");
		m_hlist.SetItemText(i, 0, (LPCTSTR)_bstr_t(date.Mid(5)));
		m_hlist.SetItemText(i, 1, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("h_title")));
		m_hlist.SetItemText(i, 2, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("h_content")));
		m_hlist.SetItemText(i, 3, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("h_type")));
		m_ADOConn.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
		i++;
	}
	m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
	GetDlgItem(IDC_HDETAIL)->EnableWindow(FALSE);
	return TRUE;
}

void CFestivalDlg::OnBnClickedHnew()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int year = m_htime.GetYear();
	int mounth = m_htime.GetMonth();
	int day = m_htime.GetDay();
	CString yearTime;
	CString mounthTime;
	CString dayTime;
	yearTime.Format(_T("%d"), year);
	mounthTime.Format(_T("%d"), mounth);
	dayTime.Format(_T("%d"), day);
	CString timeFinal;
	timeFinal.Format(_T("%s-%s-%s"), yearTime, mounthTime, dayTime);
	AdoAccess database;
	if (m_htitle.IsEmpty() || m_hcontent.IsEmpty())
	{
		MessageBox(_T("���������ݣ�"));
		return;
	}
	database.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from holidaytable order by h_date asc";
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		database.m_pRecordset->AddNew(); //�������
		database.m_pRecordset->PutCollect("h_date", (_bstr_t)timeFinal);
		database.m_pRecordset->PutCollect("h_title", (_bstr_t)m_htitle);
		database.m_pRecordset->PutCollect("h_content", (_bstr_t)m_hcontent);
		database.m_pRecordset->PutCollect("h_type", (_bstr_t)"�û�����");
		database.m_pRecordset->Update();
		database.ExitConnect();
	}
	catch (_com_error &e) {
		AfxMessageBox(_T("����ʧ��"));
		CString str;
		str.Format(_T("%s    %s"), (LPCTSTR)e.Description(), \
			(LPCTSTR)e.ErrorMessage());
		AfxMessageBox(str);
		return;
	}
	
	MessageBox(_T("��ӳɹ�"));
	m_hlist.DeleteAllItems();
	AddtoGrid();
	m_hdetail = _T("");
	m_htitle = _T("");
	m_hcontent = _T("");
	UpdateData(FALSE);
}


void CFestivalDlg::AddtoGrid()
{
	AdoAccess m_ADOConn;       // ADOConn�����
	m_ADOConn.OnInitADOConn(); //�������ݿ�
	CString sql;
	int i = 0;
	sql.Format(_T("select * from holidaytable order by h_date asc"));                         //���ò�ѯ���
	m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //��ѯ
	while (!m_ADOConn.m_pRecordset->adoEOF)
	{
		//���б���ͼ�ؼ��в�����IDC_STATIC_ZZH_CONTENTIDC_STATIC_ZZH_TITLE
		m_hlist.InsertItem(i, _T(""));
		//���б���ͼ�ؼ��в�����
		CString date = m_ADOConn.m_pRecordset->GetCollect("h_date");
		m_hlist.SetItemText(i, 0, (LPCTSTR)_bstr_t(date.Mid(5)));
		m_hlist.SetItemText(i, 1, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("h_title")));
		m_hlist.SetItemText(i, 2, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("h_content")));
		m_hlist.SetItemText(i, 3, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("h_type")));
		m_ADOConn.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
		i++;
	}
	m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
}


void CFestivalDlg::OnBnClickedHdel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from holidaytable order by h_date asc";
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		database.m_pRecordset->Move(pos, vtMissing);
		if (database.m_pRecordset->GetCollect("h_type") == (_variant_t)"ϵͳ����")
		{
			AfxMessageBox(_T("ϵͳ���ղ���ɾ��"));
			database.ExitConnect();
			return;
		}
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
	m_hlist.DeleteAllItems();
	AddtoGrid();
	m_hdetail = _T("");
	m_htitle = _T("");
	m_hcontent = _T("");
	UpdateData(FALSE);
}


void CFestivalDlg::OnNMClickHlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int dq_nItem = -1;
	LPNMITEMACTIVATE lpNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
	if (lpNMItemActivate != NULL)
	{
		dq_nItem = lpNMItemActivate->iItem;
	}
	pos = m_hlist.GetSelectionMark();
	m_hdetail = _T("���⣺") + m_hlist.GetItemText(dq_nItem, 1) + "\r\n" + _T("���ݣ�") + m_hlist.GetItemText(dq_nItem, 2);
	m_htitle = m_hlist.GetItemText(pos, 1);
	m_hcontent = m_hlist.GetItemText(pos, 2);
	UpdateData(FALSE);
	*pResult = 0;
}


void CFestivalDlg::OnBnClickedHchange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�õ�ѡ���ʱ��
	UpdateData(TRUE);
	AdoAccess data;
	int year = m_htime.GetYear();
	int mounth = m_htime.GetMonth();
	int day = m_htime.GetDay();
	CString yearTime;
	CString mounthTime;
	CString dayTime;
	yearTime.Format(_T("%d"), year);
	mounthTime.Format(_T("%d"), mounth);
	dayTime.Format(_T("%d"), day);
	CString timeFinal;
	timeFinal.Format(_T("%s-%s-%s"), yearTime, mounthTime, dayTime);
	if (m_htitle.IsEmpty() || m_hcontent.IsEmpty())
	{
		MessageBox(_T("������Ϣ����Ϊ�գ�"));
		return;
	}

	data.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from holidaytable order by h_date asc";
	data.m_pRecordset.CreateInstance(__uuidof(Recordset));
	data.m_pRecordset->Open(sql, data.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	try
	{
		data.m_pRecordset->Move((long)pos, vtMissing);
		if (data.m_pRecordset->GetCollect("h_type") == (_variant_t)"ϵͳ����")
		{
			AfxMessageBox(_T("ϵͳ���ղ����޸�"));
			data.ExitConnect();
			return;
		}
		data.m_pRecordset->PutCollect("h_date", (_bstr_t)timeFinal);
		data.m_pRecordset->PutCollect("h_title", (_bstr_t)m_htitle);
		data.m_pRecordset->PutCollect("h_content", (_bstr_t)m_hcontent);
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
	m_hlist.DeleteAllItems();
	AddtoGrid();
}


void CFestivalDlg::OnBnClickedHclear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from holidaytable";
	database.m_pRecordset.CreateInstance(__uuidof(Recordset));
	database.m_pRecordset->Open(sql, database.m_pConnection.GetInterfacePtr(), adOpenDynamic,
		adLockOptimistic, adCmdText);
	while (!database.m_pRecordset->adoEOF)
	{
		try
		{
			if (database.m_pRecordset->GetCollect("h_type") == (_variant_t)"ϵͳ����")
			{
				database.m_pRecordset->MoveNext();
				continue;
			}
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
	m_hlist.DeleteAllItems();
	AddtoGrid();
	m_hdetail = _T("");
	m_htitle = _T("");
	m_hcontent = _T("");
	UpdateData(FALSE);
}
