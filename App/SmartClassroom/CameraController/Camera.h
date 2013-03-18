#pragma once
#include <iostream>
#include <hash_map>

//FF 01 00 04 28 00 2D
const static unsigned char TurnLeftCmd[1024] = {(unsigned char)0xFF, (unsigned char)0x01, (unsigned char)0x00, (unsigned char)0x04, (unsigned char)0x28, (unsigned char)0x00, (unsigned char)0x2D};
//FF 01 00 02 28 00 2B
const static unsigned char TurnRightCmd[1024] = {(unsigned char)0xFF, (unsigned char)0x01, (unsigned char)0x00, (unsigned char)0x02, (unsigned char)0x28, (unsigned char)0x00, (unsigned char)0x2B};
//FF 01 00 08 00 23 2C
const static unsigned char TurnUpCmd[1024] = {(unsigned char)0xFF, (unsigned char)0x01, (unsigned char)0x00, (unsigned char)0x08, (unsigned char)0x00, (unsigned char)0x23, (unsigned char)0x2C};
//FF 01 00 10 00 23 34
const static unsigned char TurnDownCmd[1024] = {(unsigned char)0xFF, (unsigned char)0x01, (unsigned char)0x00, (unsigned char)0x10, (unsigned char)0x00, (unsigned char)0x23, (unsigned char)0x34};
//FF 01 00 20 00 00 21
const static unsigned char ZommInCmd[1024] = {(unsigned char)0xFF, (unsigned char)0x01, (unsigned char)0x00, (unsigned char)0x20, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x21};
//FF 01 00 40 00 00 41
const static unsigned char ZoomOutCmd[1024] = {(unsigned char)0xFF, (unsigned char)0x01, (unsigned char)0x00, (unsigned char)0x40, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x41};
//FF 01 00 00 00 00 01
const static unsigned char StopCmd[1024] = {(unsigned char)0xFF, (unsigned char)0x01, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x01};
//FF 01 00 03 00 10 14-> 16th pre position
const static unsigned char PrefixOfSetPrePos[1024] = {(unsigned char)0xFF, (unsigned char)0x01, (unsigned char)0x00, (unsigned char)0x03, (unsigned char)0x00};
//FF 01 00 07 00 10 18-> 16th position recall code
const static unsigned char PrefixOfRecallPrePos[1024] = {(unsigned char)0xFF, (unsigned char)0x01, (unsigned char)0x00, (unsigned char)0x07, (unsigned char)0x00};

const static int RegularCmdLength = 7;


// 
// --- VISCA Protocol --
//
// 81 09 06 12 FF
const static unsigned char VQueryPosCmd[1024] = {(unsigned char)0x81, (unsigned char)0x09, (unsigned char)0x06, (unsigned char)0x12, (unsigned char)0xFF};
// 81 09 04 47 FF
const static unsigned char VQueryFocalCmd[1024] = {(unsigned char)0x81, (unsigned char)0x09, (unsigned char)0x04, (unsigned char)0x47, (unsigned char)0xFF};
// 81 01 06 02 05 05 0F 06 0A 00 00 00 00 00 FF -> Move to absolute position F6A0(Horizon) and 0000(Vertical).
const static unsigned char VPrefixOfTurnToAbsolutePos[1024] = {(unsigned char)0x81, (unsigned char)0x01, (unsigned char)0x06, (unsigned char)0x02, (unsigned char)0x05, (unsigned char)0x05, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0xFF};
// 81 01 04 47 03 00 00 00 FF -> Move to absolute focal 3000.
const static unsigned char VPrefixOfTurnToAbsoluteFocal[1024] = {(unsigned char)0x81, (unsigned char)0x01, (unsigned char)0x04, (unsigned char)0x47, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0x00, (unsigned char)0xFF};
const static int VQueryCmdLength = 5;
const static int VRecallPosCmdLength = 15;
const static int VRecallFocalCmdLength = 9;
const static int Pelco_D = 0;
const static int VISCA = 1;

//Need to be detailing
class Location
{
public:
	Location();
	Location(int id);
	Location(int id, const unsigned char* cmd, int commandLen);

	int GetId();
	int SetPosition(double hPos, double vPos, double zm);
	int GetPosition(double& hPos, double& vPos, double& zm);
	int GetCommand(unsigned char* cmd, int& cmdLen);
	int GetFocalCommand(unsigned char* cmd, int& cmdLen);
	int SetCommand(unsigned char* cmd, const int cmdLen);
	int SetFocalCommand(unsigned char* cmd, const int cmdLen);
private:
	int m_nLocId;
	unsigned char command[1024];
	unsigned char focalCmd[1024];
	int commandLength;
	int focalCmdLength;
	double horizontalPos;
	double verticalPos;
	double zoom;
};

typedef std::hash_map<int, Location> CameraLocDict;
typedef std::hash_map<int, Location>::iterator CameraLocIter;
//Need to be detailing
class Camera
{
public:
	Camera();
	Camera(int commNum, int baudRate);

	int SetProtocol(int protocol);
	int Open();
	int AddPreSetLocation(Location& loc, BOOL bRestoreFromConfig, const unsigned char *posCode, const unsigned char *focalCode);
	// Control
	int RecallSpecificLocation(int locId);
	int TurnLeft();
	int TurnRight();
	int TurnUp();
	int TurnDown();
	int ZoomIn();
	int ZoomOut();
	int Stop();

	const Location& getCurrentLocation();
	int GetLocationDict(CameraLocDict **locDict);
private:
	Location currentLocation;
	HANDLE m_hSeries;
	int m_nComNum;
	int m_nBaudRate;
	CameraLocDict presetLocations;
	//std::vector<Location> presetLocations;
	int m_nPreLocNum;
	int m_nProtocol;              // 0-PelcoD 1-VISCA

private:
	//int findLocById(int locId, Location& loc);
	int sendCommand(const unsigned char* cmd, int cmdLen);
};