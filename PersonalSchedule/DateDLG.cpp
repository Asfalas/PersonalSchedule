// DateDLG.cpp : 实现文件
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "DateDLG.h"
#include "afxdialogex.h"
#include "AdoAccess.h"

// CDateDLG 对话框

IMPLEMENT_DYNAMIC(CDateDLG, CDialogEx)

CDateDLG::CDateDLG(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_time(COleDateTime::GetCurrentTime())
	, m_title(_T(""))
	, m_content(_T(""))
{

}

CDateDLG::~CDateDLG()
{
}

BOOL CDateDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect;

	// 获取编程语言列表视图控件的位置和大小   
	m_jr_Grid.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_jr_Grid.SetExtendedStyle(m_jr_Grid.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加三列   
	m_jr_Grid.InsertColumn(0, _T("日期"), LVCFMT_CENTER, rect.Width() / 3, 0);
	m_jr_Grid.InsertColumn(1, _T("标题"), LVCFMT_CENTER, rect.Width() / 3, 1);
	m_jr_Grid.InsertColumn(2, _T("内容"), LVCFMT_CENTER, rect.Width() / 3, 2);

	// 在列表视图控件中插入列表项，并设置列表子项文本   
	AdoAccess dq_m_ADOConn;       // ADOConn类对象
	dq_m_ADOConn.OnInitADOConn(); //连接数据库
	CString dq_sql;
	int dq_ii = 0;
	dq_sql="select * from datetable";                         //设置查询语句
	dq_m_ADOConn.m_pRecordset = dq_m_ADOConn.GetRecordSet((_bstr_t)dq_sql); //查询
	while (!dq_m_ADOConn.m_pRecordset->adoEOF)
	{
		//向列表视图控件中插入行IDC_STATIC_ZZH_CONTENTIDC_STATIC_ZZH_TITLE
		m_jr_Grid.InsertItem(dq_ii, _T(""));
		//向列表视图控件中插入列
		m_jr_Grid.SetItemText(dq_ii, 0, (LPCTSTR)_bstr_t(dq_m_ADOConn.m_pRecordset->GetCollect("d_date")));
		m_jr_Grid.SetItemText(dq_ii, 1, (LPCTSTR)_bstr_t(dq_m_ADOConn.m_pRecordset->GetCollect("d_title")));
		m_jr_Grid.SetItemText(dq_ii, 2, (LPCTSTR)_bstr_t(dq_m_ADOConn.m_pRecordset->GetCollect("d_content")));
		dq_m_ADOConn.m_pRecordset->MoveNext(); //将记录集指针移动到下一条记录
		dq_ii++;
	}
	dq_m_ADOConn.ExitConnect(); //断开数据库连接
	return TRUE;
}

void CDateDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_time);
	DDX_Text(pDX, IDC_EDIT1, m_title);
	DDX_Text(pDX, IDC_EDIT2, m_content);
	DDX_Control(pDX, IDC_LIST1, m_jr_Grid);
}


BEGIN_MESSAGE_MAP(CDateDLG, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDateDLG::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDateDLG 消息处理程序


void CDateDLG::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}
