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
	int SetCommand(unsigned char* cmd, const int cmdLen);
private:
	int m_nLocId;
	unsigned char command[1024];
	int commandLength;
	double horizontalPos;
	double verticalPos;
	double zoom;
};
//Need to be detailing
class Camera
{
public:
	Camera();
	Camera(int commNum, int baudRate);

	int Open();
	int AddPreSetLocation(Location& loc);
	// Control
	int TurnToSpecificLocation(int locId);
	int TurnLeft();
	int TurnRight();
	int TurnUp();
	int TurnDown();
	int ZoomIn();
	int ZoomOut();
	int Stop();

	const Location& getCurrentLocation();
private:
	Location currentLocation;
	HANDLE m_hSeries;
	int m_nComNum;
	int m_nBaudRate;
	std::hash_map<int, Location> presetLocations;
	//std::vector<Location> presetLocations;
	int m_nPreLocNum;

private:
	//int findLocById(int locId, Location& loc);
	int sendCommand(const unsigned char* cmd, int cmdLen);
};