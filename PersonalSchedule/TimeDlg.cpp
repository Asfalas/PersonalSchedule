// TimeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "TimeDlg.h"
#include "afxdialogex.h"
#include "AdoAccess.h"
#include "TimepopDlg.h"
#include "CtimepopDlg.h"

// CTimeDlg �Ի���
int m_Row, m_Col;
CString m_str;
IMPLEMENT_DYNAMIC(CTimeDlg, CDialogEx)

CTimeDlg::CTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG5, pParent)
	, m_tcontent(_T(""))
{
}

CTimeDlg::~CTimeDlg()
{
}

void CTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TLIST, m_tlist);
	//  DDX_Text(pDX, IDC_EDIT_HIDE, m_hide);
	DDX_Control(pDX, IDC_EDIT_HIDE, m_hide);
	//  DDX_Control(pDX, IDC_EDIT2, m_tcontent);
	DDX_Text(pDX, IDC_EDIT2, m_tcontent);
}


BEGIN_MESSAGE_MAP(CTimeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_TNEW, &CTimeDlg::OnBnClickedTnew)
	ON_NOTIFY(NM_CLICK, IDC_TLIST, &CTimeDlg::OnNMClickTlist)
	ON_BN_CLICKED(IDC_TCHANGE, &CTimeDlg::OnBnClickedTchange)
	ON_BN_CLICKED(IDC_TDEL, &CTimeDlg::OnBnClickedTdel)
	ON_NOTIFY(NM_DBLCLK, IDC_TLIST, &CTimeDlg::OnNMDblclkTlist)
	ON_EN_KILLFOCUS(IDC_EDIT_HIDE, &CTimeDlg::OnEnKillfocusEditHide)
	ON_NOTIFY(NM_KILLFOCUS, IDC_TLIST, &CTimeDlg::OnNMKillfocusTlist)
END_MESSAGE_MAP()


// CTimeDlg ��Ϣ�������
BOOL CTimeDlg::OnInitDialog()
{
	GetDlgItem(IDC_TDEL)->EnableWindow(false);
	GetDlgItem(IDC_TCHANGE)->EnableWindow(false);
	CDialogEx::OnInitDialog();
	CRect rect;
	m_hide.ShowWindow(SW_HIDE);
	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
	m_tlist.GetClientRect(&rect);

	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
	m_tlist.SetExtendedStyle(m_tlist.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// Ϊ�б���ͼ�ؼ��������   
	m_tlist.InsertColumn(0, _T("����ʱ��"), LVCFMT_CENTER, rect.Width() / 4, 0);
	m_tlist.InsertColumn(1, _T("���ѱ���"), LVCFMT_CENTER, rect.Width() / 4, 1);
	m_tlist.InsertColumn(2, _T("���Ѽ��"), LVCFMT_CENTER, rect.Width() / 4, 2);
	m_tlist.InsertColumn(3, _T("��������"), LVCFMT_CENTER, rect.Width() / 4, 3);

	// ���б���ͼ�ؼ��в����б���������б������ı�   
	AddtoGrid();
	return TRUE;
}


void CTimeDlg::OnBnClickedTnew()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   
	CTimepopDlg tpDlg;           // ����Ի�����CTipDlg��ʵ��   
	nRes = tpDlg.DoModal();  // �����Ի���   
	if (IDCANCEL == nRes)     // �ж϶Ի����˳��󷵻�ֵ�Ƿ�ΪIDCANCEL���������return�������������ִ��   
		return;
	m_tlist.DeleteAllItems();
	AddtoGrid();
}


void CTimeDlg::AddtoGrid()
{
	CString week[7] = { _T("����"),_T("��һ") ,_T("�ܶ�") ,_T("����") ,_T("����"),_T("����"),_T("����") };
	AdoAccess m_ADOConn;       // ADOConn�����
	m_ADOConn.OnInitADOConn(); //�������ݿ�
	CString sql;
	int i = 0;
	sql = "select * from datetable where d_type<>'��������' order by d_date asc";                         //���ò�ѯ���
	m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //��ѯ
	while (!m_ADOConn.m_pRecordset->adoEOF)
	{
		//���б���ͼ�ؼ��в�����IDC_STATIC_ZZH_CONTENTIDC_STATIC_ZZH_TITLE
		m_tlist.InsertItem(i, _T(""));
		//���б���ͼ�ؼ��в�����
		CString time = m_ADOConn.m_pRecordset->GetCollect("d_date");
		int weekday = m_ADOConn.m_pRecordset->GetCollect("d_weekday");
		time = time.Mid(time.Find(_T(" ")) + 1);
		CString type = m_ADOConn.m_pRecordset->GetCollect("d_type");
		if (type == _T("ÿ������"))
			time = week[weekday + 1] + time;
		m_tlist.SetItemText(i, 0, (LPCTSTR)_bstr_t(time));
		m_tlist.SetItemText(i, 1, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_title")));
		m_tlist.SetItemText(i, 2, (LPCTSTR)_bstr_t(type));
		m_tlist.SetItemText(i, 3, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_content")));
		m_ADOConn.m_pRecordset->MoveNext(); //����¼��ָ���ƶ�����һ����¼
		i++;
	}
	m_ADOConn.ExitConnect(); //�Ͽ����ݿ�����
}


void CTimeDlg::OnNMClickTlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	LPNMITEMACTIVATE lpNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
	if (lpNMItemActivate != NULL)
	{
		nItem = lpNMItemActivate->iItem;
	}
	pos = m_tlist.GetSelectionMark();
	if (m_tlist.GetItemText(nItem, 0).IsEmpty())
	{
		GetDlgItem(IDC_TCHANGE)->EnableWindow(false);
		GetDlgItem(IDC_TDEL)->EnableWindow(false);
		m_tcontent = _T("");
		UpdateData(FALSE);
		return;
	}
	GetDlgItem(IDC_TCHANGE)->EnableWindow(true);
	GetDlgItem(IDC_TDEL)->EnableWindow(true);
	m_tcontent = _T("����ʱ�䣺") + m_tlist.GetItemText(nItem, 0) + "\r\n"  _T("���Ѽ����") + m_tlist.GetItemText(nItem, 2) + "\r\n" + _T("���ѱ��⣺") + m_tlist.GetItemText(nItem, 1) + "\r\n" +  _T("�������ݣ�") + m_tlist.GetItemText(nItem, 3);
	UpdateData(FALSE);
	*pResult = 0;
}


void CTimeDlg::OnBnClickedTchange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT_PTR nRes;             // ���ڱ���DoModal�����ķ���ֵ   
	CCtimepopDlg cpDlg(pos);           // ����Ի�����CTipDlg��ʵ��   
	nRes = cpDlg.DoModal();  // �����Ի���   
	if (IDCANCEL == nRes)     // �ж϶Ի����˳��󷵻�ֵ�Ƿ�ΪIDCANCEL���������return�������������ִ��   
		return;
	m_tlist.DeleteAllItems();
	AddtoGrid();
	GetDlgItem(IDC_TCHANGE)->EnableWindow(false);
	GetDlgItem(IDC_TDEL)->EnableWindow(false);
}


void CTimeDlg::OnBnClickedTdel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	m_tlist.DeleteAllItems();
	AddtoGrid();
}


void CTimeDlg::OnNMDblclkTlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

//	//AfxMessageBox(_T("111"));
//	
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//	CRect rc;
//	
//	m_Row = pNMListView->iItem;//���ѡ�е���  
//	m_Col = pNMListView->iSubItem;//���ѡ����  
//	m_str = m_tlist.GetItemText(m_Row, m_Col);
//	pos = m_tlist.GetSelectionMark();
//
////	if (pNMListView->iSubItem != 0) //���ѡ���������;  
////	{
//	m_tlist.GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);//��������RECT��  
//	m_hide.SetParent(&m_tlist);//ת������Ϊ�б���е�����  
//	m_hide.MoveWindow(rc);//�ƶ�Edit��RECT���ڵ�λ��;  
//	m_hide.SetWindowText(m_tlist.GetItemText(m_Row, m_Col));//���������е�ֵ����Edit�ؼ��У�  
//	m_hide.ShowWindow(SW_SHOW);//��ʾEdit�ؼ���  
//	m_hide.SetFocus();//����Edit����  
//	m_hide.ShowCaret();//��ʾ���  
//	m_hide.SetSel(-1);//������ƶ������  
////	}
	*pResult = 0;
}


void CTimeDlg::OnEnKillfocusEditHide()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CString tem;
	//m_hide.GetWindowText(tem);    //�õ��û�������µ�����  
	//m_tlist.SetItemText(m_Row, m_Col, tem);   //���ñ༭���������  
	//CString m_time,m_title,m_type;

	//m_time = m_tlist.GetItemText(m_Row, 0);
	//m_title= m_tlist.GetItemText(m_Row, 1);
	//m_type = m_tlist.GetItemText(m_Row, 2);

	//AdoAccess data;
	//if (m_title.IsEmpty() || m_type.IsEmpty()||m_type.IsEmpty())
	//{
	//	MessageBox(_T("������Ϣ����Ϊ�գ�"));
	//	m_tlist.SetItemText(m_Row, m_Col, m_str);
	//	m_hide.ShowWindow(SW_HIDE);
	//	return;
	//}

	//data.OnInitADOConn();
	//_bstr_t sql;
	//sql = "select * from datetable where d_type<>'��������' order by d_date asc";
	//data.m_pRecordset.CreateInstance(__uuidof(Recordset));
	//data.m_pRecordset->Open(sql, data.m_pConnection.GetInterfacePtr(), adOpenDynamic,
	//	adLockOptimistic, adCmdText);
	//try
	//{
	//	data.m_pRecordset->Move((long)pos, vtMissing);
	//	data.m_pRecordset->PutCollect("d_date", (_bstr_t)m_time);
	//	data.m_pRecordset->PutCollect("d_title", (_bstr_t)m_title);
	//	data.m_pRecordset->PutCollect("d_type", (_bstr_t)m_type);
	//	data.m_pRecordset->Update();
	//	data.ExitConnect();
	//}
	//catch (_com_error &e)
	//{
	//	AfxMessageBox(_T("����ʧ��"));
	//	CString str;
	//	str.Format(_T("%s    %s"), (LPCTSTR)e.Description(), \
	//		(LPCTSTR)e.ErrorMessage());
	//	AfxMessageBox(str);
	//	data.ExitConnect();
	//}
	//MessageBox(_T("�޸ĳɹ�"));
	//m_tlist.DeleteAllItems();
	//AddtoGrid();
	//m_hide.ShowWindow(SW_HIDE);                //Ӧ�ر༭��  
}


void CTimeDlg::OnNMKillfocusTlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//GetDlgItem(IDC_TCHANGE)->EnableWindow(false);
	//GetDlgItem(IDC_TDEL)->EnableWindow(false);
	*pResult = 0;
}
