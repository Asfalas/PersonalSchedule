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
{

}

CDateDLG::~CDateDLG()
{
}

BOOL CDateDLG::OnInitDialog()
{
	//m_jr_Grid.SetExtendedStyle(LVS_EX_FLATSB
	//	| LVS_EX_FULLROWSELECT
	//	| LVS_EX_HEADERDRAGDROP
	//	| LVS_EX_ONECLICKACTIVATE
	//	| LVS_EX_GRIDLINES);
	//m_jr_Grid.InsertColumn(0, _T("�ճ�ʱ��"), LVCFMT_LEFT, 80, 0);
	//m_jr_Grid.InsertColumn(1, _T("�ճ̱���"), LVCFMT_LEFT, 150, 1);
	//m_jr_Grid.InsertColumn(2, _T("�ճ�����"), LVCFMT_LEFT, 280, 2);

	return TRUE;
}

void CDateDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDateDLG, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDateDLG::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDateDLG ��Ϣ�������


void CDateDLG::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
