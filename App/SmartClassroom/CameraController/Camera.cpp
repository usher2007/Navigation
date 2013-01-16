#include "stdafx.h"
#include "Camera.h"

Location::Location()
	:m_nLocId(0), commandLength(0), horizontalPos(0), verticalPos(0)
{
	memset(command, 0x00, 1024);
}

Location::Location(int id)
	: m_nLocId(id), horizontalPos(0), verticalPos(0)
{
	Location();
}

Location::Location(int id, const unsigned char* cmd, int commandLen)
	: m_nLocId(id), commandLength(commandLen), horizontalPos(0), verticalPos(0)
{
	memcpy(command, cmd, commandLength);
}

int Location::GetId()
{
	return m_nLocId;
}

int Location::SetPosition(double hPos, double vPos, double zm)
{
	horizontalPos = hPos;
	verticalPos = vPos;
	zoom = zm;
	return 0;
}

int Location::GetPosition(double& hPos, double& vPos, double& zm)
{
	hPos = horizontalPos;
	vPos = verticalPos;
	zm = zoom;
	return 0;
}

int Location::GetCommand(unsigned char* cmd, int& cmdLen)
{
	if(commandLength <= 0)
	{
		return -1;
	}
	memcpy(cmd, command, commandLength);
	cmdLen = commandLength;
	return 0;
}

int Location::SetCommand(unsigned char* cmd, const int cmdLen)
{
	memset(command, 0x00, 1024);
	memcpy(command, cmd, cmdLen);
	commandLength = cmdLen;
	return 0;
}

// Camera
Camera::Camera()
	: m_nComNum(1), m_nBaudRate(9600), m_nPreLocNum(0)
{

}

Camera::Camera(int commNum, int baudRate)
	: m_nComNum(commNum), m_nBaudRate(baudRate), m_nPreLocNum(0)
{

}

int Camera::Open()
{
	CString com;
	com.Format(_T("COM%d"), m_nComNum);
	m_hSeries = CreateFile(com, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if(m_hSeries == INVALID_HANDLE_VALUE)
	{
		::MessageBoxA(NULL, "∂Àø⁄¥Úø™ ß∞‹", NULL,NULL);
		return -1;
	}

	SetupComm(m_hSeries, 1024, 1024);

	DCB dcb;
	GetCommState(m_hSeries, &dcb);
	dcb.BaudRate = m_nBaudRate;
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	if(!SetCommState(m_hSeries, &dcb))
	{
		::MessageBoxA(NULL, "¥Æø⁄◊¥Ã¨…Ë÷√ ß∞‹", NULL, NULL);
		return -1;
	}

	if(!SetCommMask(m_hSeries, EV_RXCHAR | EV_TXEMPTY))
	{
		::MessageBoxA(NULL, "¥Æø⁄—⁄¬Î…Ë÷√ ß∞‹", NULL, NULL);
		return -1;
	}

	COMMTIMEOUTS CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = MAXWORD;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 100;
	CommTimeouts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(m_hSeries, &CommTimeouts);
	PurgeComm(m_hSeries, PURGE_RXCLEAR | PURGE_TXCLEAR);
	// TODO: Create a thread to read comm

	return 0;
}

int Camera::AddPreSetLocation(Location& loc)
{
	// TODO: 
	// 1.SET PRE SET POSITION 
	// 2.STORE THE RECALL CODE
	unsigned char SetPrePosCmd[1024], RecallPrePosCmd[1024];
	int locId = loc.GetId();
	if(locId < 0)
	{
		return -1;
	}
	memcpy(SetPrePosCmd, PrefixOfSetPrePos, 1024);
	SetPrePosCmd[5] = (unsigned char)(locId);
	SetPrePosCmd[6] = (unsigned char)(SetPrePosCmd[1]+SetPrePosCmd[2]+SetPrePosCmd[3]+SetPrePosCmd[4]+SetPrePosCmd[5]);
	sendCommand(SetPrePosCmd, RegularCmdLength);

	memcpy(RecallPrePosCmd, PrefixOfRecallPrePos, 1024);
	RecallPrePosCmd[5] = (unsigned char)(locId);
	RecallPrePosCmd[6] = (unsigned char)(RecallPrePosCmd[1] + RecallPrePosCmd[2] + RecallPrePosCmd[3] + RecallPrePosCmd[4] + RecallPrePosCmd[5]);
	loc.SetCommand(RecallPrePosCmd, RegularCmdLength);
	presetLocations[locId] = loc;
	m_nPreLocNum = presetLocations.size();
	return 0;
}

int Camera::RecallSpecificLocation(int locId)
{
	Location specificLoc;
	if(presetLocations.find(locId) == presetLocations.end())
	{
		return -1;
	}
	specificLoc = presetLocations[locId];
	unsigned char currentCommand[1024];
	int cmdLength = 0;
	if(specificLoc.GetCommand(currentCommand, cmdLength) < 0)
	{
		return -1;
	}

	return sendCommand(currentCommand, cmdLength);
}

int Camera::TurnLeft()
{
	return sendCommand(TurnLeftCmd, RegularCmdLength);
}

int Camera::TurnRight()
{
	return sendCommand(TurnRightCmd, RegularCmdLength);
}

int Camera::TurnUp()
{
	return sendCommand(TurnUpCmd, RegularCmdLength);
}

int Camera::TurnDown()
{
	return sendCommand(TurnDownCmd, RegularCmdLength);
}

int Camera::ZoomIn()
{
	return sendCommand(ZommInCmd, RegularCmdLength);
}

int Camera::ZoomOut()
{
	return sendCommand(ZoomOutCmd, RegularCmdLength);
}

int Camera::Stop()
{
	return sendCommand(StopCmd, RegularCmdLength);
}

// Private Methods
//int Camera::findLocById(int locId, Location& loc)
//{
//	if(presetLocations.empty())
//	{
//		return -1;
//	}
//	std::vector<Location>::iterator locIter;
//	for(locIter = presetLocations.begin(); locIter!=presetLocations.end(); ++locIter)
//	{
//		if(locIter->GetId() == locId)
//		{
//			loc = *locIter;
//			return 0;
//		}
//	}
//	return -1;
//}

int Camera::sendCommand(const unsigned char* cmd, int cmdLen)
{
	COMSTAT ComStat;
	DWORD dwErrorFlag;
	ClearCommError(m_hSeries, &dwErrorFlag, &ComStat);
	DWORD nWrite;
	if(!WriteFile(m_hSeries, cmd, cmdLen, &nWrite, NULL))
	{
		::MessageBoxA(NULL, "∑¢ÀÕ ß∞‹", NULL, NULL);
		return -1;
	}
	FlushFileBuffers(m_hSeries);
	return 0;
}