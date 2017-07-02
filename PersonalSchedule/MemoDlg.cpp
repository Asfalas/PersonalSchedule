// MemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "MemoDlg.h"
#include "afxdialogex.h"
#include "AdoAccess.h"

// CMemoDlg 对话框

IMPLEMENT_DYNAMIC(CMemoDlg, CDialogEx)

CMemoDlg::CMemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MEMO, pParent)
{

}

CMemoDlg::~CMemoDlg()
{
}

void CMemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMemoDlg, CDialogEx)
END_MESSAGE_MAP()


// CMemoDlg 消息处理程序
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
	//m_mtitle.ResetContent();
	//AdoAccess m_ADOConn;       // ADOConn类对象
	//m_ADOConn.OnInitADOConn(); //连接数据库
	//CString sql;
	//sql.Format(_T("select * from dairytable where d_date='" + date + "'"));                         //设置查询语句
	//m_ADOConn.m_pRecordset = m_ADOConn.GetRecordSet((_bstr_t)sql); //查询
	//if (!m_ADOConn.m_pRecordset->adoEOF)
	//{
	//	m_current = m_ADOConn.m_pRecordset->GetCollect("d_title");
	//	m_dcontent = m_ADOConn.m_pRecordset->GetCollect("d_content");
	//}
	//else
	//{
	//	m_current = _T("");
	//	m_dcontent = _T("");
	//}
	//while (!m_ADOConn.m_pRecordset->adoEOF)
	//{
	//	m_dtitle.AddString((LPCTSTR)_bstr_t(m_ADOConn.m_pRecordset->GetCollect("d_title")));
	//	m_ADOConn.m_pRecordset->MoveNext(); //将记录集指针移动到下一条记录
	//}
	//m_dtitle.SetCurSel(0);
	//m_ADOConn.ExitConnect(); //断开数据库连接
	//UpdateData(FALSE);
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