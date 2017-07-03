// TimeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "TimeDlg.h"
#include "afxdialogex.h"
#include "AdoAccess.h"
#include "TimepopDlg.h"
#include "CtimepopDlg.h"

// CTimeDlg 对话框
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


// CTimeDlg 消息处理程序
BOOL CTimeDlg::OnInitDialog()
{
	GetDlgItem(IDC_TDEL)->EnableWindow(false);
	GetDlgItem(IDC_TCHANGE)->EnableWindow(false);
	CDialogEx::OnInitDialog();
	CRect rect;
	m_hide.ShowWindow(SW_HIDE);
	// 获取编程语言列表视图控件的位置和大小   
	m_tlist.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_tlist.SetExtendedStyle(m_tlist.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加三列   
	m_tlist.InsertColumn(0, _T("提醒时间"), LVCFMT_CENTER, rect.Width() / 4, 0);
	m_tlist.InsertColumn(1, _T("提醒标题"), LVCFMT_CENTER, rect.Width() / 4, 1);
	m_tlist.InsertColumn(2, _T("提醒间隔"), LVCFMT_CENTER, rect.Width() / 4, 2);
	m_tlist.InsertColumn(3, _T("提醒内容"), LVCFMT_CENTER, rect.Width() / 4, 3);

	// 在列表视图控件中插入列表项，并设置列表子项文本   
	AddtoGrid();
	return TRUE;
}


void CTimeDlg::OnBnClickedTnew()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   
	CTimepopDlg tpDlg;           // 构造对话框类CTipDlg的实例   
	nRes = tpDlg.DoModal();  // 弹出对话框   
	if (IDCANCEL == nRes)     // 判断对话框退出后返回值是否为IDCANCEL，如果是则return，否则继续向下执行   
		return;
	m_tlist.DeleteAllItems();
	AddtoGrid();
}


void CTimeDlg::AddtoGrid()
{
	CString week[7] = { _T("周日"),_T("周一") ,_T("周二") ,_T("周三") ,_T("周四"),_T("周五"),_T("周六") };
	AdoAccess m_ADOConn;       // ADOConn类对象
	m_ADOConn.OnInitADOConn(); //连接数据库
	CString sql;
	int i = 0;
	sql = "select * from datetable where d_type<>'定期提醒' order by d_date asc";                         //设置查询语句
	m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //查询
	while (!m_ADOConn.m_pRecordset->adoEOF)
	{
		//向列表视图控件中插入行IDC_STATIC_ZZH_CONTENTIDC_STATIC_ZZH_TITLE
		m_tlist.InsertItem(i, _T(""));
		//向列表视图控件中插入列
		CString time = m_ADOConn.m_pRecordset->GetCollect("d_date");
		int weekday = m_ADOConn.m_pRecordset->GetCollect("d_weekday");
		time = time.Mid(time.Find(_T(" ")) + 1);
		CString type = m_ADOConn.m_pRecordset->GetCollect("d_type");
		if (type == _T("每周提醒"))
			time = week[weekday + 1] + time;
		m_tlist.SetItemText(i, 0, (LPCTSTR)_bstr_t(time));
		m_tlist.SetItemText(i, 1, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_title")));
		m_tlist.SetItemText(i, 2, (LPCTSTR)_bstr_t(type));
		m_tlist.SetItemText(i, 3, (LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_content")));
		m_ADOConn.m_pRecordset->MoveNext(); //将记录集指针移动到下一条记录
		i++;
	}
	m_ADOConn.ExitConnect(); //断开数据库连接
}


void CTimeDlg::OnNMClickTlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
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
	m_tcontent = _T("提醒时间：") + m_tlist.GetItemText(nItem, 0) + "\r\n"  _T("提醒间隔：") + m_tlist.GetItemText(nItem, 2) + "\r\n" + _T("提醒标题：") + m_tlist.GetItemText(nItem, 1) + "\r\n" +  _T("提醒内容：") + m_tlist.GetItemText(nItem, 3);
	UpdateData(FALSE);
	*pResult = 0;
}


void CTimeDlg::OnBnClickedTchange()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;             // 用于保存DoModal函数的返回值   
	CCtimepopDlg cpDlg(pos);           // 构造对话框类CTipDlg的实例   
	nRes = cpDlg.DoModal();  // 弹出对话框   
	if (IDCANCEL == nRes)     // 判断对话框退出后返回值是否为IDCANCEL，如果是则return，否则继续向下执行   
		return;
	m_tlist.DeleteAllItems();
	AddtoGrid();
	GetDlgItem(IDC_TCHANGE)->EnableWindow(false);
	GetDlgItem(IDC_TDEL)->EnableWindow(false);
}


void CTimeDlg::OnBnClickedTdel()
{
	// TODO: 在此添加控件通知处理程序代码
	AdoAccess database;
	database.OnInitADOConn();
	_bstr_t sql;
	sql = "select * from datetable where d_type<>'定期提醒' order by d_date asc";
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
		MessageBox(_T("操作失败"));
		return;
	}
	MessageBox(_T("删除成功"));
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
//	m_Row = pNMListView->iItem;//获得选中的行  
//	m_Col = pNMListView->iSubItem;//获得选中列  
//	m_str = m_tlist.GetItemText(m_Row, m_Col);
//	pos = m_tlist.GetSelectionMark();
//
////	if (pNMListView->iSubItem != 0) //如果选择的是子项;  
////	{
//	m_tlist.GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);//获得子项的RECT；  
//	m_hide.SetParent(&m_tlist);//转换坐标为列表框中的坐标  
//	m_hide.MoveWindow(rc);//移动Edit到RECT坐在的位置;  
//	m_hide.SetWindowText(m_tlist.GetItemText(m_Row, m_Col));//将该子项中的值放在Edit控件中；  
//	m_hide.ShowWindow(SW_SHOW);//显示Edit控件；  
//	m_hide.SetFocus();//设置Edit焦点  
//	m_hide.ShowCaret();//显示光标  
//	m_hide.SetSel(-1);//将光标移动到最后  
////	}
	*pResult = 0;
}


void CTimeDlg::OnEnKillfocusEditHide()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString tem;
	//m_hide.GetWindowText(tem);    //得到用户输入的新的内容  
	//m_tlist.SetItemText(m_Row, m_Col, tem);   //设置编辑框的新内容  
	//CString m_time,m_title,m_type;

	//m_time = m_tlist.GetItemText(m_Row, 0);
	//m_title= m_tlist.GetItemText(m_Row, 1);
	//m_type = m_tlist.GetItemText(m_Row, 2);

	//AdoAccess data;
	//if (m_title.IsEmpty() || m_type.IsEmpty()||m_type.IsEmpty())
	//{
	//	MessageBox(_T("基础信息不能为空！"));
	//	m_tlist.SetItemText(m_Row, m_Col, m_str);
	//	m_hide.ShowWindow(SW_HIDE);
	//	return;
	//}

	//data.OnInitADOConn();
	//_bstr_t sql;
	//sql = "select * from datetable where d_type<>'定期提醒' order by d_date asc";
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
	//	AfxMessageBox(_T("链接失败"));
	//	CString str;
	//	str.Format(_T("%s    %s"), (LPCTSTR)e.Description(), \
	//		(LPCTSTR)e.ErrorMessage());
	//	AfxMessageBox(str);
	//	data.ExitConnect();
	//}
	//MessageBox(_T("修改成功"));
	//m_tlist.DeleteAllItems();
	//AddtoGrid();
	//m_hide.ShowWindow(SW_HIDE);                //应藏编辑框  
}


void CTimeDlg::OnNMKillfocusTlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//GetDlgItem(IDC_TCHANGE)->EnableWindow(false);
	//GetDlgItem(IDC_TDEL)->EnableWindow(false);
	*pResult = 0;
}
