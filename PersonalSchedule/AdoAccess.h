class AdoAccess
{
public:
	_ConnectionPtr m_pConnection;//Êý¾Ý¿â
	_RecordsetPtr m_pRecordset;//ÃüÁî
	_CommandPtr m_pCommand;//¼ÇÂ¼

	void OnInitADOConn();
	void ExitConnect();
	_RecordsetPtr & GetRecordSet(_bstr_t bstrSQL);
	BOOL ExecuteSQL(_bstr_t bstrSQL);
	AdoAccess();
	~AdoAccess();
};

