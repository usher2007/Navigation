// MgdLib.cpp : Defines the exported functions for the DLL application.
#include "stdafx.h"
#include "MgdLib.h"
using namespace std;
namespace MgdLib {  
	mgClass::mgClass()  
	{   
	}  
	mgClass::!mgClass()  
	{  
		delete m_pImpObj;  
	}  
	mgClass::~mgClass()  
	{  
		this->!mgClass();  
	}  
	void mgClass::SetFileName(System::String^ imageName)
	{
		const char* imageNameC = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(imageName)).ToPointer();
		string& imageNameStd = (string)imageNameC;
		m_pImpObj = new MyImage(imageNameStd);
	}
	void mgClass::ShowImage()
	{
		m_pImpObj->ShowImage();
	}
} 