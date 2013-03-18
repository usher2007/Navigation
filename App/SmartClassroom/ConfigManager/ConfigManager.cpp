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
	if(m_teacherEnt.presetLocDict.find(locId) == m_teacherEnt.presetLocDict.end())
	{
		m_teacherEnt.presetLocIds.push_back(locId);
		m_teacherEnt.numOfPresetLoc++;
	}
	LocRange tmpLoc;
	tmpLoc.left = leftRange;
	tmpLoc.right = rightRange;
	m_teacherEnt.presetLocDict[locId] = tmpLoc;
	return S_OK;
}

HRESULT CConfigManager::SetTeacherFullScreen( int locId )
{
	m_teacherEnt.fullScreenLocId = locId > 0 ? locId : m_teacherEnt.fullScreenLocId;
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
	m_teacherEnt.pixRangeOverlap = pixOverlap >= 0 ? pixOverlap : m_teacherEnt.pixRangeOverlap;
	m_teacherEnt.roomWidth = classroomWidth > 0 ? classroomWidth : m_teacherEnt.roomWidth;
	m_teacherEnt.cameraDistance = cameraDistance > 0 ? cameraDistance : m_teacherEnt.cameraDistance;
	m_teacherEnt.leastHumanGap = leastHumanGap > 0 ? leastHumanGap : m_teacherEnt.leastHumanGap;
	m_teacherEnt.humanWidth = humanWidth > 0 ? humanWidth : m_teacherEnt.humanWidth;
	m_teacherEnt.fgLowThresh = fgLowThresh >= 0 ? fgLowThresh : m_teacherEnt.fgLowThresh;
	m_teacherEnt.fgUpThresh = fgUpThresh >= 0 ? fgUpThresh : m_teacherEnt.fgUpThresh;
	m_teacherEnt.fgHistThresh = fgHistThresh > 0 ? fgHistThresh : m_teacherEnt.fgHistThresh;
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
	m_teacherEnt.disappearFrameThresh = disappearFrameThresh > 0 ? disappearFrameThresh : m_teacherEnt.disappearFrameThresh;
	m_teacherEnt.centerWeightThresh = centerWeightThresh > 0 ? centerWeightThresh : m_teacherEnt.centerWeightThresh;
	m_teacherEnt.gbmLearningRate = gbmLearningRate > 0 ? gbmLearningRate : m_teacherEnt.gbmLearningRate;
	m_teacherEnt.trackingInterval = trackingInterval > 0 ? trackingInterval : m_teacherEnt.trackingInterval;

	return S_OK;
}

HRESULT CConfigManager::SetBlindZone(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	BlindZone bZone;

	bZone.x[0] = x1;
	bZone.x[1] = x2;
	bZone.x[2] = x3;
	bZone.x[3] = x4;

	bZone.y[0] = y1;
	bZone.y[1] = y2;
	bZone.y[2] = y3;
	bZone.y[3] = y4;

	m_teacherEnt.blindZones.push_back(bZone);
	return S_OK;
}

HRESULT CConfigManager::ClearBlindZones()
{
	if(m_teacherEnt.blindZones.empty())
	{
		return S_OK;
	}
	m_teacherEnt.blindZones.clear();
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

HRESULT CConfigManager::GetTeaVLocCodes(VLocCodeDict** vLocCodeList)
{
	*vLocCodeList = &m_teacherEnt.viscaPresetLocDict;
	return S_OK;
}

HRESULT CConfigManager::GetTeaFullScreenVLocCode(LocationCode **vLocFullScreen)
{
	*vLocFullScreen = &m_teacherEnt.viscaFullScreen;
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

int CConfigManager::GetTeaCameraProtocol()
{
	return m_teacherEnt.cameraProtocol;
}

HRESULT CConfigManager::GetBlindZoneList(BlindZoneList **bZoneList)
{
	*bZoneList = &(m_teacherEnt.blindZones);
	return S_OK;
}

HRESULT CConfigManager::SyncViscaCode( int locId, unsigned char *pos, unsigned char *focal )
{
	if(locId == 0)
	{
		memcpy(m_teacherEnt.viscaFullScreen.Pos, pos, 8);
		memcpy(m_teacherEnt.viscaFullScreen.Focal, focal, 4);
		return S_OK;
	}
	else if(locId > 0)
	{
		LocationCode locCode;
		memcpy(locCode.Pos, pos+6, 8);
		memcpy(locCode.Focal, focal+4, 4);
		m_teacherEnt.viscaPresetLocDict[locId] = locCode;
		return S_OK;
	}
	return E_FAIL;

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
	else if(paramName.compare(PROTOCOL) == 0)
	{
		m_teacherEnt.cameraProtocol = atoi(paramValue.c_str());
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
	else if(paramName.compare(PRESETVLOC) == 0)
	{
		int nDelimPos = 0, i = 0;
		for(i=1; i<m_teacherEnt.numOfPresetLoc; ++i)
		{
			if(i!=1)
			{
				nDelimPos++;
			}
			int nextDelimPos = paramValue.find(ARRAYDELIMITER, nDelimPos);
			std::string arrayElem = paramValue.substr(nDelimPos, nextDelimPos-nDelimPos);
			// TODO: PROCESS THE ELEMENTS
			LocationCode vLocCode;
			for(int j=0; j<12; j++)
			{
				std::string code = arrayElem.substr(j, 1);
				char *stopstring;
				unsigned char vcode = (unsigned char)strtol(code.c_str(), &stopstring, 16);
				if(j<8)
				{
					vLocCode.Pos[j] = vcode;
				}
				else
				{
					vLocCode.Focal[j%8] = vcode;
				}
			}
			m_teacherEnt.viscaPresetLocDict[m_teacherEnt.presetLocIds[i-1]] = vLocCode;
			nDelimPos = nextDelimPos;
		}
		std::string arrayElem = paramValue.substr(nDelimPos+1, paramValue.length()-nDelimPos-1);
		LocationCode vLocCode;
		for(int j=0; j<12; j++)
		{
			std::string code = arrayElem.substr(j, 1);
			char *stopstring;
			unsigned char vcode = (unsigned char)strtol(code.c_str(), &stopstring, 16);
			if(j<8)
			{
				vLocCode.Pos[j] = vcode;
			}
			else
			{
				vLocCode.Focal[j%8] = vcode;
			}
		}
		m_teacherEnt.viscaPresetLocDict[m_teacherEnt.presetLocIds[i-1]] = vLocCode;
	}
	else if(paramName.compare(PRESETFSCRVLOC) == 0)
	{
		for(int i=0; i<12; ++i)
		{
			std::string code = paramValue.substr(i,1);
			char* stopString;
			unsigned char vcode = (unsigned char)strtol(code.c_str(), &stopString, 16);
			if(i<8)
			{
				m_teacherEnt.viscaFullScreen.Pos[i] = vcode;
			}
			else
			{
				m_teacherEnt.viscaFullScreen.Focal[i%8] = vcode;
			}
		}
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
	else if(paramName.compare(BLINKZONE) == 0)
	{
		int nDelimiPos = 0, i = 0;
		BlindZone bZone;
		for(i=1; i<4; ++i)
		{
			if(i != 1)
			{
				nDelimiPos++;
			}
			int nextDelimPos = paramValue.find(DICTIONARYDELIMITER, nDelimiPos);
			std::string pointStr = paramValue.substr(nDelimiPos, nextDelimPos-nDelimiPos);
			int pointDelimPos = pointStr.find(ARRAYDELIMITER);
			int x = atoi(pointStr.substr(0, pointDelimPos).c_str());
			int y = atoi(pointStr.substr(pointDelimPos+1, pointStr.length()-pointDelimPos-1).c_str());
			bZone.x[i-1] = x;
			bZone.y[i-1] = y;
			nDelimiPos = nextDelimPos;
		}
		std::string pointStr = paramValue.substr(nDelimiPos+1, paramValue.length()-nDelimiPos-1);
		int pointDelimPos = pointStr.find(ARRAYDELIMITER);
		int x = atoi(pointStr.substr(0, pointDelimPos).c_str());
		int y = atoi(pointStr.substr(pointDelimPos+1, pointStr.length()-pointDelimPos-1).c_str());
		bZone.x[3] = x;
		bZone.y[3] = y;
		m_teacherEnt.blindZones.push_back(bZone);
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
	teacherOut<<PROTOCOL<<NAMEVALUEDELIMITER<<m_teacherEnt.cameraProtocol<<'\n';
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

	teacherOut<<PRESETVLOC<<NAMEVALUEDELIMITER;
	VLocCodeIter vLocDictIter;
	for(vLocDictIter=m_teacherEnt.viscaPresetLocDict.begin(); vLocDictIter!=m_teacherEnt.viscaPresetLocDict.end(); ++vLocDictIter)
	{
		if(vLocDictIter != m_teacherEnt.viscaPresetLocDict.begin())
		{
			teacherOut<<ARRAYDELIMITER;
		}
		for(int i=0; i<12; ++i)
		{
			int code = 0;
			if(i >= 8)
			{
				int j = i % 8;
				code = (*vLocDictIter).second.Focal[j];
			}
			else
			{
				code = (*vLocDictIter).second.Pos[i];
			}
			teacherOut<<int2char(code, 16);
		}
	}
	teacherOut<<'\n';

	teacherOut<<PRESETFSCRVLOC<<NAMEVALUEDELIMITER;
	LocationCode fullScrCode = m_teacherEnt.viscaFullScreen;
	for(int i=0; i<12; ++i)
	{
		int code = 0;
		if(i >= 8)
		{
			int j = i % 8;
			code = fullScrCode.Focal[j];
		}
		else
		{
			code = fullScrCode.Pos[i];
		}
		teacherOut<<int2char(code, 16);
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
	if(!(m_teacherEnt.blindZones.empty()))
	{
		BlindZoneIter bZoneIter;
		for(bZoneIter=m_teacherEnt.blindZones.begin(); bZoneIter!=m_teacherEnt.blindZones.end(); ++bZoneIter)
		{
			teacherOut<<BLINKZONE<<NAMEVALUEDELIMITER;
			for(int i=0; i<4; i++)
			{
				if(i != 0)
				{
					teacherOut<<DICTIONARYDELIMITER;
				}
				teacherOut<<(bZoneIter->x[i])<<ARRAYDELIMITER<<(bZoneIter->y[i]);
			}
			teacherOut<<'\n';
		}
	}
	teacherOut<<std::endl;
	teacherOut.close();
	return S_OK;
}

char CConfigManager::int2char(int code, int scale)
{
	char res = '0';
	if(scale == 16)
	{
		if(code < 0 || code > 15)
		{
			return res;
		}
		switch (code)
		{
		case 10:
			res = 'A';
			break;
		case 11:
			res = 'B';
			break;
		case 12:
			res = 'C';
			break;
		case 13:
			res = 'D';
			break;
		case 14:
			res = 'E';
			break;
		case 15:
			res = 'F';
			break;
		default:
			res = '0' + code;
		}
		return res;
	}
	else if(scale == 10)
	{
		return ('0' + code);
	}
	return res;
}