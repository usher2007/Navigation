#include "PureCPPSLib.h"

PureCClass::PureCClass()  
{}  
PureCClass::~PureCClass(void)  
{}  
LPCTSTR PureCClass::getFirstName()  
{  
	return (LPCTSTR)m_FirstName;  
}  
void PureCClass::setFirstName(LPCTSTR fistName)  
{  
	m_FirstName = fistName;  
}  
LPCTSTR PureCClass::getLastName()  
{  
	return (LPCTSTR)m_LastName;  
}  
void PureCClass::setLastName(LPCTSTR lastName)  
{  
	m_LastName = lastName;  
}  
LPCTSTR PureCClass::joinName()  
{  
	m_fullName = m_FirstName + _T(" ") +m_LastName;  
	return (LPCTSTR)(m_fullName);  
} 