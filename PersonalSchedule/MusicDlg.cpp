// MusicDlg.cpp : ʵ���ļ�
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


// CMusicDlg �Ի���


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
	// ����ˮƽ�������ĳ�ʼλ��Ϊ20   
	m_slider.SetScrollPos(20);
	// �ڱ༭������ʾ20   
	SetDlgItemInt(IDC_SCROLLBAR1, 20);
	m_int = 20;
	UpdateData(false);
	if (!InitMusic())
	{
		GetDlgItem(IDC_play)->EnableWindow(false); //�ļ�û�ж�ȡʱ���а�ť����ѡ
		GetDlgItem(IDC_pause)->EnableWindow(false);
		GetDlgItem(IDC_stop)->EnableWindow(false);
		GetDlgItem(IDC_vol)->EnableWindow(false);
		GetDlgItem(IDC_SCROLLBAR1)->EnableWindow(false);
	}
	return TRUE;
}

BOOL CMusicDlg::InitMusic() {
	CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ��������ͱ�֤�����Ǹ��ĺ������ÿ�ζ�����
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

// CMusicDlg ��Ϣ�������
//����  
void Load(HWND hWnd, CString strFilepath)
{
	m_hWnd = hWnd;
	mciSendCommand(DeviceId, MCI_CLOSE, 0, 0); //�ڼ����ļ�ǰ�������һ�β��ŵ��豸  
	mciopenparms.lpstrElementName = (LPCWSTR)strFilepath; //�������ļ�·�������豸  
	DWORD dwReturn;
	if (dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT, (DWORD)(LPVOID)&mciopenparms))
	{
		//��������ʧ��,��������Ϣ������buffer,����ʾ������  
		char buffer[256] = {0};
		mciGetErrorString(dwReturn, (LPWSTR)buffer, 256); //��ȡ�������Ӧ�Ĵ�����Ϣ  
		MessageBox(hWnd, (LPWSTR)buffer, _T("�����棡"), MB_ICONHAND | MB_ICONERROR | MB_ICONSTOP); //����������Ϣ��ʾ�Ի���  
	}
	DeviceId = mciopenparms.wDeviceID;
	//���ļ��ɹ��͹����ļ����豸  
}

//����  
void CMusicDlg::play()
{
	Load(this->m_hWnd, m_path);
	MCI_PLAY_PARMS mciplayparms;
	mciplayparms.dwCallback = (DWORD)m_hWnd;
	mciplayparms.dwFrom = 0; //ÿ�β��Ŷ��Ǵ�0��ʼ����  
	mciSendCommand(DeviceId, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (DWORD)(LPVOID)&mciplayparms);
	MCI_DGV_SETAUDIO_PARMS setvolume; //���������Ĳ����ṹ��  
	setvolume.dwCallback = NULL; //  
	setvolume.dwItem = MCI_DGV_SETAUDIO_VOLUME; //��������������  
	setvolume.dwValue = _ttoi(m_vol); //����ֵ��vol  
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
	//�����ֹͣ��ťʱ,�����е���Ϣ�������  
}

DWORD setvolume(DWORD vol)
{
	MCI_DGV_SETAUDIO_PARMS setvolume; //���������Ĳ���
	setvolume.dwCallback = NULL; //
	setvolume.dwItem = MCI_DGV_SETAUDIO_VOLUME; //��������������
	setvolume.dwValue = vol*10; //����ֵ��vol
	mciSendCommand(DeviceId, MCI_SETAUDIO, MCI_DGV_SETAUDIO_ITEM | MCI_DGV_SETAUDIO_VALUE, (DWORD)(LPVOID)&setvolume);
	return 0;
}
 
void CMusicDlg::OnBnClickedfilechoice()
{
	CString szFileFilter =_T("mp3�ļ�(*.mp3)|*.mp3|"
		"wma�ļ�(*.wma)|*.wma|"
		"wav�ļ�(*.wav)|*.wav|"
		"�����ļ�(*.*)|*.*|");
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
	GetDlgItem(IDC_play)->EnableWindow(true); //�ļ���ȡ�ɹ�ʱ���а�ť��ɿ�ѡ  
	GetDlgItem(IDC_pause)->EnableWindow(true);
	GetDlgItem(IDC_stop)->EnableWindow(true);
	GetDlgItem(IDC_vol)->EnableWindow(true);
	GetDlgItem(IDC_SCROLLBAR1)->EnableWindow(true);
}


void CMusicDlg::OnBnClickedplay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	play();
	SetDlgItemText(IDC_pause, _T("��ͣ"));
}


void CMusicDlg::OnBnClickedstop()
{
	stop();
	SetDlgItemText(IDC_pause, _T("��ͣ"));
	GetDlgItem(IDC_play)->EnableWindow(false); //������stop��ʱ��,���ź���ͣ����ѡ  
	GetDlgItem(IDC_pause)->EnableWindow(false);
}


void CMusicDlg::OnBnClickedpause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTemp;
	GetDlgItemText(IDC_pause, strTemp); //��ȡ��ť��״̬  
	if (strTemp.Compare(_T("��ͣ")) == 0)
	{
		pause();
		SetDlgItemText(IDC_pause, _T("�ָ�"));
	}

	if (strTemp.Compare(_T("�ָ�")) == 0)
	{
		resume();
		SetDlgItemText(IDC_pause, _T("��ͣ"));
	}
}



//void CMusicDlg::OnThemechangedScrollbar1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// �ù���Ҫ��ʹ�� Windows XP ����߰汾��
//	// ���� _WIN32_WINNT ���� >= 0x0501��
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	UpdateData(TRUE);
//	m_int = m_slider.GetScrollPos()/10;
//	m_slider.SetScrollPos(m_int * 10);
//	UpdateData(FALSE);
//	*pResult = 0;
//}


void CMusicDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// TODO: Add your message handler code here and/or call default  
	int pos = m_slider.GetScrollPos();    // ��ȡˮƽ��������ǰλ��  

	switch (nSBCode)
	{
		// ����������һ�У���pos��1  
	case SB_LINELEFT:
		pos -= 1;
		break;
		// ������ҹ���һ�У���pos��1  
	case SB_LINERIGHT:
		pos += 1;
		break;
		// ����������һҳ����pos��10  
	case SB_PAGELEFT:
		pos -= 10;
		break;
		// ������ҹ���һҳ����pos��10  
	case SB_PAGERIGHT:
		pos += 10;
		break;
		// �������������ˣ���posΪ1  
	case SB_LEFT:
		pos = 1;
		break;
		// ������������Ҷˣ���posΪ100  
	case SB_RIGHT:
		pos = 100;
		break;
		// ����϶������������ָ��λ�ã���pos��ֵΪnPos��ֵ  
	case SB_THUMBPOSITION:
		pos = nPos;
		break;
		// �����m_horiScrollbar.SetScrollPos(pos);ִ��ʱ��ڶ��ν���˺���������ȷ��������λ�ã����һ�ֱ�ӵ�default��֧�������ڴ˴����ñ༭������ʾ��ֵ  
	default:
		setvolume(pos); 
		CString p;
		p.Format(_T("%d"), pos);
		::WritePrivateProfileString(_T("Music Info"), _T("m_vol"),p, _T("./music.ini"));
		SetDlgItemInt(IDC_vol, pos);
		return;
	}

	// ���ù�����λ��  
	m_slider.SetScrollPos(pos);
	setvolume(pos);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


LRESULT CMusicDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class  
	switch (message) //�ж���Ϣ����  
	{
	case WM_SYSCOMMAND:
		//�����ϵͳ��Ϣ   
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	stop();
	CDialogEx::OnOK();
}


