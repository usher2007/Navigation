#include "afxwin.h"

class PureCClass  
{  
public:  
	PureCClass();  
	~PureCClass(void);  
public:  
	LPCTSTR getFirstName();  
	void setFirstName(LPCTSTR fistName);  
	LPCTSTR getLastName();  
	void setLastName(LPCTSTR lastName);  
	LPCTSTR joinName();  
private:  
	CString m_FirstName;  
	CString m_LastName;  
	CString m_fullName;  
};  
