// MgdLib.cpp : Defines the exported functions for the DLL application.
#include "stdafx.h"
#include "MgdLib.h"
namespace MgdLib {  
	mgClass::mgClass()  
	{  
		m_pImpObj = new PureCClass();  
	}  
	mgClass::!mgClass()  
	{  
		delete m_pImpObj;  
	}  
	mgClass::~mgClass()  
	{  
		this->!mgClass();  
	}  
	String ^ mgClass::FirstName::get()  
	{  
		return gcnew String(m_pImpObj->getFirstName());  
	}  
	void mgClass::FirstName::set(String^ str)  
	{  
		pin_ptr<const WCHAR> wch = PtrToStringChars(str);  
		m_pImpObj->setFirstName(((std::wstring)wch).c_str());  
	}  
	String ^ mgClass::LastName::get()  
	{  
		return gcnew String(m_pImpObj->getLastName());  
	}  
	void mgClass::LastName::set(String^ str)  
	{  
		pin_ptr<const WCHAR> wch = PtrToStringChars(str);  
		m_pImpObj->setLastName(((std::wstring)wch).c_str());  
	}  
	String ^ mgClass::JoinName ()  
	{  
		return gcnew String(m_pImpObj->joinName());  
	}  
} 