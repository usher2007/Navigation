#include "stdafx.h"
#include "Utils.h"

int getIntFromCEdit( CEdit *ctrlEdit )
{
	int num;
	CString param;
	param = getCStringFromCEdit(ctrlEdit);
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
	double ret = 0.0;
	CString param;
	param = getCStringFromCEdit(ctrlEdit);
	ret = _ttof(param);
	return ret;
}