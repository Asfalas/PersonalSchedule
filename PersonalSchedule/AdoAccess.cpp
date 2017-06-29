#include "stdafx.h"
#include "AdoAccess.h"


void AdoAccess::OnInitADOConn()
{
	::CoInitialize(NULL);
	try {
		m_pConnection.CreateInstance("ADODB.Connection");
		_bstr_t strConnect = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=PSchedule.mdb";
		m_pConnection->Open(strConnect, _T(""), _T(""), adModeUnknown);
		AfxMessageBox(_T("连接成功"));
	}
	catch (_com_error &e) {
		AfxMessageBox(_T("链接失败"));
		CString str;
		str.Format(_T("%s    %s"), (LPCTSTR)e.Description(), \
			(LPCTSTR)e.ErrorMessage());
		AfxMessageBox(str);
	}
}

void AdoAccess::ExitConnect()
{
	if (m_pRecordset != NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
	::CoUninitialize();
}

_RecordsetPtr& AdoAccess::GetRecordSet(_bstr_t bstrSQL)
{
	try
	{
		if (m_pConnection == NULL)
			OnInitADOConn();
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(_variant_t(bstrSQL), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockBatchOptimistic, adCmdText);

	}
	catch (_com_error &e)
	{
		AfxMessageBox(_T("链接失败"));
		CString str;
		str.Format(_T("%s    %s"), (LPCTSTR)e.Description(), \
			(LPCTSTR)e.ErrorMessage());
		AfxMessageBox(str);
	}
	return m_pRecordset;
}

BOOL AdoAccess::ExecuteSQL(_bstr_t bstrSQL)
{
	try
	{
		//是否已连接数据库
		if (m_pConnection == NULL)
			OnInitADOConn();
		m_pConnection->Execute(bstrSQL, NULL, adCmdText);
		return true;
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());							//显示错误信息
		return false;
	}
}

AdoAccess::AdoAccess()
{
}


AdoAccess::~AdoAccess()
{
}
