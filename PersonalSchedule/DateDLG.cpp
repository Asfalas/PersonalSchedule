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
