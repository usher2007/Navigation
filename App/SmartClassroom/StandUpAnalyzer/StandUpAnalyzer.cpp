// StandUpAnalyzer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "StandUpAnalyzer.h"


CStandUpAnalyzer* CStandUpAnalyzer::m_pInstance = NULL;


CStandUpAnalyzer* CStandUpAnalyzer::GetInstance()
{
	if(m_pInstance != NULL)
	{
		return m_pInstance;
	}
	else
	{
		m_pInstance = new CStandUpAnalyzer;
		return m_pInstance;
	}
}
// This is the constructor of a class that has been exported.
// see StandUpAnalyzer.h for the class definition
CStandUpAnalyzer::CStandUpAnalyzer()
{
	m_StandUpInfo[0].clear();
	m_StandUpInfo[1].clear();
	return;
}

HRESULT CStandUpAnalyzer::AnalyzePosition( int index, StandUpRowInfo info )
{
	m_StandUpInfo[index] = info;
	controlPTZCamera();
	return S_OK;
}

HRESULT CStandUpAnalyzer::controlPTZCamera()
{
	if(m_StandUpInfo[0].empty() || m_StandUpInfo[1].empty())
	{
		return E_FAIL;
	}

	int rowNum = 0, colNum = 0;
	StandUpInfoIter infoIter;
	for(infoIter = m_StandUpInfo[0].begin(); infoIter != m_StandUpInfo[0].end(); ++infoIter)
	{
		StandUpInfoIter secondIter;
		if((secondIter = m_StandUpInfo[1].find(infoIter->first)) != m_StandUpInfo[1].end())
		{
			StandUpInfo s0 = infoIter->second;
			StandUpInfo s1 = secondIter->second;
			if(s0.slope > 2.5 && s1.slope > 2.5) // Stand Up
			{
				double ratio = s0.weight / s1.weight;
				if(ratio < 0.5)
				{
					colNum = 1;
				}
				else if(ratio < 1.5)
				{
					colNum = 2;
				}
				else
				{
					colNum = 3;
				}
				rowNum = infoIter->first;
				m_standUpStatus[rowNum] = 1;
			}
			else if(s0.slope < -2.5 && s1.slope < -2.5) // sit down
			{
				m_standUpStatus[rowNum] = 0;
			}
		}
		

	}

	int statusSum = 0;
	for(int i=0; i<9; ++i)
	{
		statusSum += m_standUpStatus[i];
	}
	if(statusSum == 1)
	{
		// Turn camera to specific position according to rowNum and colNum
	}
	else 
	{
		// Turn camera to full screen position.
	}
}
