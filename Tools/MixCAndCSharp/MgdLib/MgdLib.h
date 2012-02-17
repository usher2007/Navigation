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
		property String^ FirstName  
		{  
			String ^ get ();  
			void set (String ^str);  
		}  
		property String^ LastName  
		{  
			String ^ get ();  
			void set (String ^str);  
		}  
		String^ JoinName();  
	private:  
		PureCClass* m_pImpObj;  
	};  
}