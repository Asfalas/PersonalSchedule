
// PersonalSchedule.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPersonalScheduleApp: 
// �йش����ʵ�֣������ PersonalSchedule.cpp
//

class CPersonalScheduleApp : public CWinApp
{
public:
	CPersonalScheduleApp();

	// ��д
public:
	virtual BOOL InitInstance();

	// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPersonalScheduleApp theApp;
