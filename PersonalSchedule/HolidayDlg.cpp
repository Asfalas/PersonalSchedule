// HolidayDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "HolidayDlg.h"
#include "afxdialogex.h"


// CHolidayDlg 对话框

IMPLEMENT_DYNAMIC(CHolidayDlg, CDialogEx)

CHolidayDlg::CHolidayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CHolidayDlg::~CHolidayDlg()
{
}

void CHolidayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHolidayDlg, CDialogEx)
END_MESSAGE_MAP()


// CHolidayDlg 消息处理程序
