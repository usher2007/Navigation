// ConfigManager.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ConfigManager.h"
#include <fstream>

// This is the constructor of a class that has been exported.
// see ConfigManager.h for the class definition
CConfigManager::CConfigManager()
{
	LoadConfigFile();

	m_studentEnt.id = 1;
	m_laserPointEnt.id = 1;
	return;
}
CConfigManager::~CConfigManager()
{
	dumpTeacherConfig();
}

HRESULT CConfigManager::LoadConfigFile()
{
	HRESULT hr = loadTeacherConfig();
	if(FAILED(hr))
	{

	}
	return S_OK;
}

HRESULT CConfigManager::DumpConfigFile()
{
	HRESULT hr = dumpTeacherConfig();
	if(FAILED(hr))
	{

	}
	return S_OK;
}

HRESULT CConfigManager::SetTeacherPresetLoc( int locId, int leftRange, int rightRange )
{
	LocRange tmpLoc;
	tmpLoc.left = leftRange;
	tmpLoc.right = rightRange;
	m_teacherEnt.presetLocDict[locId] = tmpLoc;
	return S_OK;
}

HRESULT CConfigManager::SetTeacherFullScreen( int locId )
{
	m_teacherEnt.fullScreenLocId = locId;
	return S_OK;
}


HRESULT CConfigManager::SetTeaShowTracking(BOOL bShowTracking)
{
	m_teacherEnt.bShowTracking = bShowTracking;
	return S_OK;
}

HRESULT CConfigManager::SetTeaDetailParams(int pixOverlap, double classroomWidth, double cameraDistance, int leastHumanGap, 
	int humanWidth, int fgLowThresh, int fgUpThresh, double fgHistThresh)
{
	m_teacherEnt.pixRangeOverlap = pixOverlap;
	m_teacherEnt.roomWidth = classroomWidth;
	m_teacherEnt.cameraDistance = cameraDistance;
	m_teacherEnt.leastHumanGap = leastHumanGap;
	m_teacherEnt.humanWidth = humanWidth;
	m_teacherEnt.fgLowThresh = fgLowThresh;
	m_teacherEnt.fgUpThresh = fgUpThresh;
	m_teacherEnt.fgHistThresh = fgHistThresh;
	return S_OK;
}

HRESULT CConfigManager::SetTeaTrackingArea(int beginX, int beginY, int beginW, int beginH, int stopX, int stopY, int stopW, int stopH)
{
	m_teacherEnt.beginTrackArea.clear();
	m_teacherEnt.beginTrackArea.reserve(4);
	m_teacherEnt.beginTrackArea[0] = beginX;
	m_teacherEnt.beginTrackArea[1] = beginY;
	m_teacherEnt.beginTrackArea[2] = beginW;
	m_teacherEnt.beginTrackArea[3] = beginH;

	m_teacherEnt.stopTrackArea.clear();
	m_teacherEnt.stopTrackArea.reserve(4);
	m_teacherEnt.stopTrackArea[0] = stopX;
	m_teacherEnt.stopTrackArea[1] = stopY;
	m_teacherEnt.stopTrackArea[2] = stopW;
	m_teacherEnt.stopTrackArea[3] = stopH;
	
	return S_OK;
}

HRESULT CConfigManager::SetTeaCommonParams(int disappearFrameThresh, int centerWeightThresh, double gbmLearningRate, int trackingInterval)
{
	m_teacherEnt.disappearFrameThresh = disappearFrameThresh;
	m_teacherEnt.centerWeightThresh = centerWeightThresh;
	m_teacherEnt.gbmLearningRate = gbmLearningRate;
	m_teacherEnt.trackingInterval = trackingInterval;

	return S_OK;
}

HRESULT CConfigManager::GetTeaEnvParams(double& roomWidth, double& cameraDistance)
{
	roomWidth = m_teacherEnt.roomWidth;
	cameraDistance = m_teacherEnt.cameraDistance;
	return S_OK;
}

HRESULT CConfigManager::GetTeaPresetLocDict( PresetLocDict** locDict )
{
	//locDict.clear();
	/*PresetLocDictIter pIter = m_teacherEnt.presetLocDict.begin();
	for(; pIter!=m_teacherEnt.presetLocDict.end(); ++pIter)
	{
		locDict[pIter->first] = pIter->second;
	}*/
	*locDict = &m_teacherEnt.presetLocDict;
	return S_OK;
}

int CConfigManager::GetTeaId()
{
	return m_teacherEnt.id;
}

int CConfigManager::GetTeaFullScreenLocId()
{
	return m_teacherEnt.fullScreenLocId;
}

int CConfigManager::GetTeaPixRangeOverlap()
{
	return m_teacherEnt.pixRangeOverlap;
}

BOOL CConfigManager::IsTeaShowTracking()
{
	return m_teacherEnt.bShowTracking;
}

HRESULT CConfigManager::GetTeaBeginTrackingArea(int& x, int& y, int& width, int& height)
{
	if(m_teacherEnt.beginTrackArea.size() == 4)
	{
		x = m_teacherEnt.beginTrackArea[0];
		y = m_teacherEnt.beginTrackArea[1];
		width = m_teacherEnt.beginTrackArea[2];
		height = m_teacherEnt.beginTrackArea[3];
		return S_OK;
	}
	return E_FAIL;
}

HRESULT CConfigManager::GetTeaStopTrackingArea(int& x, int& y, int& width, int& height)
{
	if(m_teacherEnt.stopTrackArea.size() == 4)
	{
		x = m_teacherEnt.stopTrackArea[0];
		y = m_teacherEnt.stopTrackArea[1];
		width = m_teacherEnt.stopTrackArea[2];
		height = m_teacherEnt.stopTrackArea[3];
		return S_OK;
	}
	return E_FAIL;
}

int CConfigManager::GetTeaLeastHumanGap()
{
	return m_teacherEnt.leastHumanGap;
}

int CConfigManager::GetTeaHumanWidth()
{
	return m_teacherEnt.humanWidth;
}

int CConfigManager::GetTeaDisFrameThresh()
{
	return m_teacherEnt.disappearFrameThresh;
}

int CConfigManager::GetTeaCenterWeightThresh()
{
	return m_teacherEnt.centerWeightThresh;
}

int CConfigManager::GetTeaFgLowThresh()
{
	return m_teacherEnt.fgLowThresh;
}

int CConfigManager::GetTeaFgUpThresh()
{
	return m_teacherEnt.fgUpThresh;
}

double CConfigManager::GetTeaFgHistThresh()
{
	return m_teacherEnt.fgHistThresh;
}

double CConfigManager::GetTeaGBMLearningRate()
{
	return m_teacherEnt.gbmLearningRate;
}

int CConfigManager::GetTeaTrackingInterval()
{
	return m_teacherEnt.trackingInterval;
}

HRESULT CConfigManager::loadTeacherConfig()
{
	std::ifstream teacherIn(TeacherConfigFile);
	std::string configLine, paramName, paramValue;
	while(!teacherIn.eof())
	{
		getline(teacherIn, configLine);
		getParamNameAndVal(configLine, paramName, paramValue);
		setTeaParametersFromFile(paramName, paramValue);
	}
	teacherIn.close();
	return S_OK;
}

HRESULT CConfigManager::getParamNameAndVal(const std::string& paramLine, std::string& paramName, std::string& paramValue)
{
	std::size_t nDelimiterPos = paramLine.find(NAMEVALUEDELIMITER);
	paramName = paramLine.substr(0,nDelimiterPos);
	paramValue = paramLine.substr(nDelimiterPos+1, paramLine.length()-nDelimiterPos-1);
	return S_OK;
}

HRESULT CConfigManager::setTeaParametersFromFile( const std::string& paramName, std::string& paramValue )
{
	if(paramName.compare(ID) == 0)
	{
		m_teacherEnt.id = atoi(paramValue.c_str());
	}
	else if(paramName.compare(CLASSROOMWIDTH) == 0)
	{
		m_teacherEnt.roomWidth = atof(paramValue.c_str());
	}
	else if(paramName.compare(CAMERADISTANCE) == 0)
	{
		m_teacherEnt.cameraDistance = atof(paramValue.c_str());
	}
	else if(paramName.compare(FULLSCREENLOCID) == 0)
	{
		m_teacherEnt.fullScreenLocId = atoi(paramValue.c_str());
	}
	else if(paramName.compare(PIXOVERLLAP) == 0)
	{
		m_teacherEnt.pixRangeOverlap = atoi(paramValue.c_str());
	}
	else if(paramName.compare(SHOWTRACKRESULT) == 0)
	{
		m_teacherEnt.bShowTracking = (atoi(paramValue.c_str()) == 1);
	}
	else if(paramName.compare(NUMOFPRESETLOC) == 0)
	{
		m_teacherEnt.numOfPresetLoc = atoi(paramValue.c_str());
	}
	else if(paramName.compare(PRESETLOCIDS) == 0)
	{
		processArrayParameters(paramName, paramValue, m_teacherEnt.numOfPresetLoc);
	}
	else if(paramName.compare(PRESETLOCPIXRANGES) == 0)
	{
		int nDelimiPos = 0, i = 0;
		for(i=1; i<m_teacherEnt.numOfPresetLoc; ++i)
		{
			if(i!=1)
			{
				nDelimiPos++;
			}
			int nextDelimPos = paramValue.find(DICTIONARYDELIMITER, nDelimiPos);
			std::string ranges = paramValue.substr(nDelimiPos, nextDelimPos-nDelimiPos);
			int rangeDelimPos = ranges.find(ARRAYDELIMITER);
			LocRange tmpRange;
			tmpRange.left = atoi(ranges.substr(0, rangeDelimPos).c_str());
			tmpRange.right = atoi(ranges.substr(rangeDelimPos+1, ranges.length()-rangeDelimPos-1).c_str());
			m_teacherEnt.presetLocDict[m_teacherEnt.presetLocIds[i-1]] = tmpRange;
			nDelimiPos = nextDelimPos;
		}
		std::string ranges = paramValue.substr(nDelimiPos+1, paramValue.length()-nDelimiPos-1);
		int rangeDelimPos = ranges.find(ARRAYDELIMITER);
		LocRange tmpRange;
		tmpRange.left = atoi(ranges.substr(0, rangeDelimPos).c_str());
		tmpRange.right = atoi(ranges.substr(rangeDelimPos+1, ranges.length()-rangeDelimPos-1).c_str());
		m_teacherEnt.presetLocDict[m_teacherEnt.presetLocIds[i-1]] = tmpRange;
	}
	else if(paramName.compare(BEGINTRACKINGAREA) == 0)
	{
		processArrayParameters(paramName, paramValue, 4);
	}
	else if(paramName.compare(STOPTRACKINGAREA) == 0)
	{
		processArrayParameters(paramName, paramValue, 4);
	}
	else if(paramName.compare(LEASTHUAMNGAP) == 0)
	{
		m_teacherEnt.leastHumanGap = atoi(paramValue.c_str());
	}
	else if(paramName.compare(HUMANWIDTH) == 0)
	{
		m_teacherEnt.humanWidth = atoi(paramValue.c_str());
	}
	else if(paramName.compare(DISAPPEARFRAMETHRESH) == 0)
	{
		m_teacherEnt.disappearFrameThresh = atoi(paramValue.c_str());
	}
	else if(paramName.compare(CENTERWEIGHTTHRESH) == 0)
	{
		m_teacherEnt.centerWeightThresh = atoi(paramValue.c_str());
	}
	else if(paramName.compare(FGLOWTHRESH) == 0)
	{
		m_teacherEnt.fgLowThresh = atoi(paramValue.c_str());
	}
	else if(paramName.compare(FGUPTHRESH) == 0)
	{
		m_teacherEnt.fgUpThresh = atoi(paramValue.c_str());
	}
	else if(paramName.compare(FGHISTTHRESH) == 0)
	{
		m_teacherEnt.fgHistThresh = atof(paramValue.c_str());
	}
	else if(paramName.compare(GBMLEARNINGRATE) == 0)
	{
		m_teacherEnt.gbmLearningRate = atof(paramValue.c_str());
	}
	else if(paramName.compare(TRACKINGINTERVAL) == 0)
	{
		m_teacherEnt.trackingInterval = atoi(paramValue.c_str());
	}
	else // No this parameter
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CConfigManager::processArrayParameters( const std::string& paramName, std::string& paramValue, int arrayLen )
{
	std::vector<int> *resultArray = NULL;
	if(paramName.compare(PRESETLOCIDS) == 0)
	{
		resultArray = &(m_teacherEnt.presetLocIds);
	}
	else if(paramName.compare(BEGINTRACKINGAREA) == 0)
	{
		resultArray = &(m_teacherEnt.beginTrackArea);
	}
	else if(paramName.compare(STOPTRACKINGAREA) == 0)
	{
		resultArray = &(m_teacherEnt.stopTrackArea);
	}
	if(resultArray != NULL)
	{
		int nDelimPos = 0, i = 0;
		for(i=1; i<arrayLen; ++i)
		{
			if(i!=1)
			{
				nDelimPos++;
			}
			int nextDelimPos = paramValue.find(ARRAYDELIMITER, nDelimPos);
			std::string arrayElem = paramValue.substr(nDelimPos, nextDelimPos-nDelimPos);
			resultArray->push_back(atoi(arrayElem.c_str()));
			nDelimPos = nextDelimPos;
		}
		std::string arrayElem = paramValue.substr(nDelimPos+1, paramValue.length()-nDelimPos-1);
		resultArray->push_back(atoi(arrayElem.c_str()));
	}
	return S_OK;
}


HRESULT CConfigManager::dumpTeacherConfig()
{
	std::ofstream teacherOut(TeacherConfigFile);
	if(teacherOut.fail())
	{
		return E_FAIL;
	}
	teacherOut<<ID<<NAMEVALUEDELIMITER<<m_teacherEnt.id<<'\n';
	teacherOut<<CLASSROOMWIDTH<<NAMEVALUEDELIMITER<<m_teacherEnt.roomWidth<<'\n';
	teacherOut<<CAMERADISTANCE<<NAMEVALUEDELIMITER<<m_teacherEnt.cameraDistance<<'\n';
	teacherOut<<FULLSCREENLOCID<<NAMEVALUEDELIMITER<<m_teacherEnt.fullScreenLocId<<'\n';
	teacherOut<<PIXOVERLLAP<<NAMEVALUEDELIMITER<<m_teacherEnt.pixRangeOverlap<<'\n';
	teacherOut<<SHOWTRACKRESULT<<NAMEVALUEDELIMITER<<((int)m_teacherEnt.bShowTracking)<<'\n';
	teacherOut<<NUMOFPRESETLOC<<NAMEVALUEDELIMITER<<m_teacherEnt.numOfPresetLoc<<'\n';

	teacherOut<<PRESETLOCIDS<<NAMEVALUEDELIMITER;
	std::vector<int>::iterator intIter;
	for(intIter = m_teacherEnt.presetLocIds.begin(); intIter != m_teacherEnt.presetLocIds.end(); ++intIter)
	{
		if(intIter != m_teacherEnt.presetLocIds.begin())
		{
			teacherOut<<ARRAYDELIMITER;
		}
		teacherOut<<(*intIter);
	}
	teacherOut<<'\n';

	teacherOut<<PRESETLOCPIXRANGES<<NAMEVALUEDELIMITER;
	PresetLocDictIter locDictIter;
	for(locDictIter = m_teacherEnt.presetLocDict.begin(); locDictIter != m_teacherEnt.presetLocDict.end(); ++locDictIter)
	{
		if(locDictIter != m_teacherEnt.presetLocDict.begin())
		{
			teacherOut<<DICTIONARYDELIMITER;
		}
		teacherOut<<(*locDictIter).second.left<<ARRAYDELIMITER<<(*locDictIter).second.right;
	}
	teacherOut<<'\n';

	teacherOut<<BEGINTRACKINGAREA<<NAMEVALUEDELIMITER<<m_teacherEnt.beginTrackArea[0]<<ARRAYDELIMITER<<
		m_teacherEnt.beginTrackArea[1]<<ARRAYDELIMITER<<m_teacherEnt.beginTrackArea[2]<<ARRAYDELIMITER<<m_teacherEnt.beginTrackArea[3]<<'\n';
	teacherOut<<STOPTRACKINGAREA<<NAMEVALUEDELIMITER<<m_teacherEnt.stopTrackArea[0]<<ARRAYDELIMITER<<
		m_teacherEnt.stopTrackArea[1]<<ARRAYDELIMITER<<m_teacherEnt.stopTrackArea[2]<<ARRAYDELIMITER<<m_teacherEnt.stopTrackArea[3]<<'\n';

	teacherOut<<LEASTHUAMNGAP<<NAMEVALUEDELIMITER<<m_teacherEnt.leastHumanGap<<'\n';
	teacherOut<<HUMANWIDTH<<NAMEVALUEDELIMITER<<m_teacherEnt.humanWidth<<'\n';
	teacherOut<<DISAPPEARFRAMETHRESH<<NAMEVALUEDELIMITER<<m_teacherEnt.disappearFrameThresh<<'\n';
	teacherOut<<CENTERWEIGHTTHRESH<<NAMEVALUEDELIMITER<<m_teacherEnt.centerWeightThresh<<'\n';
	teacherOut<<FGLOWTHRESH<<NAMEVALUEDELIMITER<<m_teacherEnt.fgLowThresh<<'\n';
	teacherOut<<FGUPTHRESH<<NAMEVALUEDELIMITER<<m_teacherEnt.fgUpThresh<<'\n';
	teacherOut<<FGHISTTHRESH<<NAMEVALUEDELIMITER<<m_teacherEnt.fgHistThresh<<'\n';
	teacherOut<<GBMLEARNINGRATE<<NAMEVALUEDELIMITER<<m_teacherEnt.gbmLearningRate<<'\n';
	teacherOut<<TRACKINGINTERVAL<<NAMEVALUEDELIMITER<<m_teacherEnt.trackingInterval<<'\n';
	teacherOut<<std::endl;
	teacherOut.close();
	return S_OK;
}