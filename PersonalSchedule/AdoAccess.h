class AdoAccess
{
public:
	_ConnectionPtr m_pConnection;//���ݿ�
	_RecordsetPtr m_pRecordset;//����
	_CommandPtr m_pCommand;//��¼

	void OnInitADOConn();
	void ExitConnect();
	_RecordsetPtr & GetRecordSet(_bstr_t bstrSQL);
	BOOL ExecuteSQL(_bstr_t bstrSQL);
	AdoAccess();
	~AdoAccess();
};

