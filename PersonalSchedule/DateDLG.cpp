// DateDLG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "DateDLG.h"
#include "afxdialogex.h"


// CDateDLG �Ի���

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
	//m_jr_Grid.SetExtendedStyle(LVS_EX_FLATSB
	//	| LVS_EX_FULLROWSELECT
	//	| LVS_EX_HEADERDRAGDROP
	//	| LVS_EX_ONECLICKACTIVATE
	//	| LVS_EX_GRIDLINES);
	//m_jr_Grid.InsertColumn(0, _T("�ճ�ʱ��"), LVCFMT_LEFT, 80, 0);
	//m_jr_Grid.InsertColumn(1, _T("�ճ̱���"), LVCFMT_LEFT, 150, 1);
	//m_jr_Grid.InsertColumn(2, _T("�ճ�����"), LVCFMT_LEFT, 280, 2);
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
	//m_jr_Grid.InsertItem(0, _T("Java"));
	//m_jr_Grid.SetItemText(0, 1, _T("1"));
	//m_jr_Grid.SetItemText(0, 2, _T("1"));
	//m_jr_Grid.InsertItem(1, _T("C"));
	//m_jr_Grid.SetItemText(1, 1, _T("2"));
	//m_jr_Grid.SetItemText(1, 2, _T("2"));
	//m_jr_Grid.SetItemText(2, 1, _T("3"));
	//m_jr_Grid.SetItemText(2, 2, _T("6"));
	//m_jr_Grid.InsertItem(3, _T("C++"));
	//m_jr_Grid.SetItemText(3, 1, _T("4"));
	//m_jr_Grid.SetItemText(3, 2, _T("3"));
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


// CDateDLG ��Ϣ�������


void CDateDLG::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
