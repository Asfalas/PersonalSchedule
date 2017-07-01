// MusicDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PersonalSchedule.h"
#include "MusicDlg.h"
#include "afxdialogex.h"
#include <MMSystem.h>  
#include <Digitalv.h>  

#pragma comment(lib, "Winmm.lib")  

HWND m_hWnd;
DWORD DeviceId;
MCI_OPEN_PARMS mciopenparms;


// CMusicDlg 对话框


IMPLEMENT_DYNAMIC(CMusicDlg, CDialogEx)

CMusicDlg::CMusicDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_int(0)
{

}

CMusicDlg::~CMusicDlg()
{
}

void CMusicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_slider);
	DDX_Text(pDX, IDC_vol, m_int);
}


BEGIN_MESSAGE_MAP(CMusicDlg, CDialogEx)
	ON_BN_CLICKED(IDC_filechoice, &CMusicDlg::OnBnClickedfilechoice)
	ON_BN_CLICKED(IDC_play, &CMusicDlg::OnBnClickedplay)
	ON_BN_CLICKED(IDC_pause, &CMusicDlg::OnBnClickedpause)
	ON_BN_CLICKED(IDC_stop, &CMusicDlg::OnBnClickedstop)
//	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR1, &CMusicDlg::OnThemechangedScrollbar1)
ON_WM_HSCROLL()
ON_BN_CLICKED(IDOK, &CMusicDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CMusicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_slider.SetScrollRange(1, 100);
	// 设置水平滚动条的初始位置为20   
	m_slider.SetScrollPos(20);
	// 在编辑框中显示20   
	SetDlgItemInt(IDC_SCROLLBAR1, 20);
	m_int = 20;
	UpdateData(false);
	if (!InitMusic())
	{
		GetDlgItem(IDC_play)->EnableWindow(false); //文件没有读取时所有按钮不可选
		GetDlgItem(IDC_pause)->EnableWindow(false);
		GetDlgItem(IDC_stop)->EnableWindow(false);
		GetDlgItem(IDC_vol)->EnableWindow(false);
		GetDlgItem(IDC_SCROLLBAR1)->EnableWindow(false);
	}
	return TRUE;
}

BOOL CMusicDlg::InitMusic() {
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件，这样就保证了我们更改后的设置每次都可用
	BOOL ifFind = finder.FindFile(_T("./music.ini"));
	if (!ifFind) {
		::WritePrivateProfileString(_T("Music Info"), _T("m_path"), _T(""), _T("./music.ini"));
		::WritePrivateProfileString(_T("Music Info"), _T("m_name"), _T(""), _T("./music.ini"));
		::WritePrivateProfileString(_T("Music Info"), _T("m_vol"), _T(""), _T("./music.ini"));
		return false;
	}
	else
	{
		GetPrivateProfileString(_T("Music Info"), _T("m_path"), _T(""), m_path.GetBuffer(MAX_PATH), MAX_PATH, _T("./music.ini"));
		GetPrivateProfileString(_T("Music Info"), _T("m_name"), _T(""), m_name.GetBuffer(MAX_PATH), MAX_PATH, _T("./music.ini"));
		GetPrivateProfileString(_T("Music Info"), _T("m_vol"), _T(""), m_vol.GetBuffer(MAX_PATH), MAX_PATH, _T("./music.ini"));
		if ((m_path=="")|| (m_name == "") || (m_vol == ""))
			return false;
	}
	SetDlgItemText(IDC_filename, m_path);
	return true;
}

// CMusicDlg 消息处理程序
//加载  
void Load(HWND hWnd, CString strFilepath)
{
	m_hWnd = hWnd;
	mciSendCommand(DeviceId, MCI_CLOSE, 0, 0); //在加载文件前先清空上一次播放的设备  
	mciopenparms.lpstrElementName = (LPCWSTR)strFilepath; //将音乐文件路径传给设备  
	DWORD dwReturn;
	if (dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT, (DWORD)(LPVOID)&mciopenparms))
	{
		//如果打开玩家失败,将出错信息储存在buffer,并显示出错警告  
		char buffer[256] = {0};
		mciGetErrorString(dwReturn, (LPWSTR)buffer, 256); //获取错误码对应的错误信息  
		MessageBox(hWnd, (LPWSTR)buffer, _T("出错警告！"), MB_ICONHAND | MB_ICONERROR | MB_ICONSTOP); //弹出错误信息提示对话框  
	}
	DeviceId = mciopenparms.wDeviceID;
	//打开文件成功就关联文件到设备  
}

//播放  
void CMusicDlg::play()
{
	Load(this->m_hWnd, m_path);
	MCI_PLAY_PARMS mciplayparms;
	mciplayparms.dwCallback = (DWORD)m_hWnd;
	mciplayparms.dwFrom = 0; //每次播放都是从0开始播放  
	mciSendCommand(DeviceId, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (DWORD)(LPVOID)&mciplayparms);
	MCI_DGV_SETAUDIO_PARMS setvolume; //设置音量的参数结构体  
	setvolume.dwCallback = NULL; //  
	setvolume.dwItem = MCI_DGV_SETAUDIO_VOLUME; //动作是设置音量  
	setvolume.dwValue = _ttoi(m_vol); //音量值是vol  
	mciSendCommand(DeviceId, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD)(LPVOID)&setvolume);
}

void pause()
{
	mciSendCommand(DeviceId, MCI_PAUSE, 0, 0);
}

void resume()
{
	mciSendCommand(DeviceId, MCI_RESUME, 0, 0);
}

void stop()
{
	mciSendCommand(DeviceId, MCI_STOP, 0, 0);
	mciSendCommand(DeviceId, MCI_CLOSE, 0, 0);
	//当点击停止按钮时,将所有的信息都清除掉  
}

DWORD setvolume(DWORD vol)
{
	MCI_DGV_SETAUDIO_PARMS setvolume; //设置音量的参数
	setvolume.dwCallback = NULL; //
	setvolume.dwItem = MCI_DGV_SETAUDIO_VOLUME; //动作是设置音量
	setvolume.dwValue = vol*10; //音量值是vol
	mciSendCommand(DeviceId, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD)(LPVOID)&setvolume);
	return 0;
}
 
void CMusicDlg::OnBnClickedfilechoice()
{
	CString szFileFilter =_T("mp3文件(*.mp3)|*.mp3|"
		"wma文件(*.wma)|*.wma|"
		"wav文件(*.wav)|*.wav|"
		"所有文件(*.*)|*.*|");
	CFileDialog dlg(true, NULL, NULL, OFN_HIDEREADONLY, (LPCTSTR)szFileFilter);
	if (dlg.DoModal() == IDOK)
	{
		CString strFilepath = dlg.GetPathName();
		CString strFilename = dlg.GetFileName();
		::WritePrivateProfileString(_T("Music Info"), _T("m_path"), strFilepath, _T("./music.ini"));
		::WritePrivateProfileString(_T("Music Info"), _T("m_name"), strFilename, _T("./music.ini"));
		SetDlgItemText(IDC_filename, strFilename);
		Load(this->m_hWnd, strFilepath);
	}
	GetDlgItem(IDC_play)->EnableWindow(true); //文件读取成功时所有按钮变成可选  
	GetDlgItem(IDC_pause)->EnableWindow(true);
	GetDlgItem(IDC_stop)->EnableWindow(true);
	GetDlgItem(IDC_vol)->EnableWindow(true);
	GetDlgItem(IDC_SCROLLBAR1)->EnableWindow(true);
}


void CMusicDlg::OnBnClickedplay()
{
	// TODO: 在此添加控件通知处理程序代码
	play();
	SetDlgItemText(IDC_pause, _T("暂停"));
}


void CMusicDlg::OnBnClickedstop()
{
	stop();
	SetDlgItemText(IDC_pause, _T("暂停"));
	GetDlgItem(IDC_play)->EnableWindow(false); //当按下stop的时候,播放和暂停不可选  
	GetDlgItem(IDC_pause)->EnableWindow(false);
}


void CMusicDlg::OnBnClickedpause()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	GetDlgItemText(IDC_pause, strTemp); //获取按钮的状态  
	if (strTemp.Compare(_T("暂停")) == 0)
	{
		pause();
		SetDlgItemText(IDC_pause, _T("恢复"));
	}

	if (strTemp.Compare(_T("恢复")) == 0)
	{
		resume();
		SetDlgItemText(IDC_pause, _T("暂停"));
	}
}



//void CMusicDlg::OnThemechangedScrollbar1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// 该功能要求使用 Windows XP 或更高版本。
//	// 符号 _WIN32_WINNT 必须 >= 0x0501。
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//	m_int = m_slider.GetScrollPos()/10;
//	m_slider.SetScrollPos(m_int * 10);
//	UpdateData(FALSE);
//	*pResult = 0;
//}


void CMusicDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// TODO: Add your message handler code here and/or call default  
	int pos = m_slider.GetScrollPos();    // 获取水平滚动条当前位置  

	switch (nSBCode)
	{
		// 如果向左滚动一列，则pos减1  
	case SB_LINELEFT:
		pos -= 1;
		break;
		// 如果向右滚动一列，则pos加1  
	case SB_LINERIGHT:
		pos += 1;
		break;
		// 如果向左滚动一页，则pos减10  
	case SB_PAGELEFT:
		pos -= 10;
		break;
		// 如果向右滚动一页，则pos加10  
	case SB_PAGERIGHT:
		pos += 10;
		break;
		// 如果滚动到最左端，则pos为1  
	case SB_LEFT:
		pos = 1;
		break;
		// 如果滚动到最右端，则pos为100  
	case SB_RIGHT:
		pos = 100;
		break;
		// 如果拖动滚动块滚动到指定位置，则pos赋值为nPos的值  
	case SB_THUMBPOSITION:
		pos = nPos;
		break;
		// 下面的m_horiScrollbar.SetScrollPos(pos);执行时会第二次进入此函数，最终确定滚动块位置，并且会直接到default分支，所以在此处设置编辑框中显示数值  
	default:
		setvolume(pos); 
		CString p;
		p.Format(_T("%d"), pos);
		::WritePrivateProfileString(_T("Music Info"), _T("m_vol"),p, _T("./music.ini"));
		SetDlgItemInt(IDC_vol, pos);
		return;
	}

	// 设置滚动块位置  
	m_slider.SetScrollPos(pos);
	setvolume(pos);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


LRESULT CMusicDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class  
	switch (message) //判断消息类型  
	{
	case WM_SYSCOMMAND:
		//如果是系统消息   
		if (wParam == SC_CLOSE)
		{
			stop();
		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CMusicDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	stop();
	CDialogEx::OnOK();
}


