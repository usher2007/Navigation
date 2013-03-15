#include "stdafx.h"
#include "Utils.h"

int getIntFromCEdit( CEdit *ctrlEdit )
{
	int num;
	CString param;
	param = getCStringFromCEdit(ctrlEdit);
	if(param.IsEmpty())
	{
		return -1;
	}
	num = _ttoi(param);
	return num;
}

CString getCStringFromCEdit( CEdit *ctrlEdit )
{
	int paramLen;
	CString param;
	paramLen = ctrlEdit->LineLength(0);
	ctrlEdit->GetLine(0, param.GetBuffer(paramLen), paramLen);
	param.ReleaseBuffer(paramLen);
	return param;
}

double getDoubleFromCEdit(CEdit *ctrlEdit)
{
	double ret = -1.0;
	CString param;
	param = getCStringFromCEdit(ctrlEdit);
	ret = _ttof(param);
	if(ret == 0.0)
	{
		ret = -1.0;
	}
	return ret;
}