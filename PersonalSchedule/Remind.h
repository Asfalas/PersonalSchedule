#pragma once
class CRemind
{
public:
	CString title, content, time, type;
	CRemind();
	CRemind(CString title, CString content, CString time);
	~CRemind();
};

