#pragma once
#include "..\PureCPPSLib\PureCPPSLib.h"
#using <system.dll>
using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace System::Globalization;

namespace MgdLib {  
	public ref class mgClass  
	{  
	public:  
		mgClass();  
	protected:  
		!mgClass();  
	public:  
		~mgClass();  
		void ShowImage();
		void SetFileName(System::String^ imageName);
	private:  
		MyImage* m_pImpObj;  
	};  
}