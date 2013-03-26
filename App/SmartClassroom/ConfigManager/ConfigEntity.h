#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <string>

const std::string TeacherConfigFile                   = "..\\Config\\Teacher.conf";
const char ARRAYDELIMITER                             = ',';
const char DICTIONARYDELIMITER                        = ';';
const char NAMEVALUEDELIMITER                         = '=';
const std::string ID                                  = "ID";
const std::string PROTOCOL                            = "PROTOCOL";
const std::string CLASSROOMWIDTH                      = "CLS_RM_WIDTH";
const std::string CAMERADISTANCE                      = "CAM_DIST";
const std::string FULLSCREENLOCID                     = "FULL_SCR_LOC_ID";
const std::string PIXOVERLLAP                         = "PIX_LOC_OVERLAP";
const std::string SHOWTRACKRESULT                     = "SHOW_TRACK_RES";
const std::string NUMOFPRESETLOC                      = "PRESET_LOC_NUM";
const std::string PRESETLOCIDS                        = "PRESET_LOC_IDS";
const std::string PRESETLOCPIXRANGES                  = "PRESET_LOC_PIX_RANGES";
const std::string PRESETVLOC                          = "PRESET_VLOC";
const std::string PRESETFSCRVLOC                      = "PRESET_FULL_SCR_VLOC";
const std::string CAMERAVELOCITY                      = "CAMERA_VELOCITY";
const std::string BEGINTRACKINGAREA                   = "BEGIN_TRACKING_AREA";
const std::string STOPTRACKINGAREA                    = "STOP_TRACKING_AREA";
const std::string LEASTHUAMNGAP                       = "LEAST_HUMAN_GAP";
const std::string HUMANWIDTH                          = "HUMAN_WIDTH";
const std::string DISAPPEARFRAMETHRESH                = "DIS_FRAME_THRESH";
const std::string CENTERWEIGHTTHRESH                  = "CENTER_WEIGHT_THRESH";
const std::string FGLOWTHRESH                         = "FG_LOW_THRESH";
const std::string FGUPTHRESH                          = "FG_UP_THRESH";
const std::string FGHISTTHRESH                        = "FG_HIST_THRESH";
const std::string GBMLEARNINGRATE                     = "GBM_LEARNING_RATE";
const std::string TRACKINGINTERVAL                    = "TRACKING_INTERVAL";
const std::string BLINDZONE                           = "BLIND_ZONE";
typedef struct LocRange
{
	int left;
	int right;
} LocRange;

typedef std::map<int, LocRange> PresetLocDict;
typedef std::map<int, LocRange>::iterator PresetLocDictIter;

typedef struct BlindZone
{
	int x[4];
	int y[4];
} BlindZone;

typedef std::vector<BlindZone> BlindZoneList;
typedef std::vector<BlindZone>::iterator BlindZoneIter;

typedef struct LocationCode
{
	unsigned char Pos[8];
	unsigned char Focal[4];
} LocationCode;
typedef std::map<int, LocationCode> VLocCodeDict;
typedef std::map<int, LocationCode>::iterator VLocCodeIter;

class TeacherEntity
{
public:
	int id;                                    // Global Uinque ID
	int cameraProtocol;                        // Pelco-D(0) or VISCA(1)
	int fullScreenLocId;                       // Location ID represent full-screen, usually 0
	double roomWidth;                          // The Room's width, in meters
	double cameraDistance;                     // The distance between camera and rostrum
	int numOfPresetLoc;                        // Count of preset locations
	std::vector<int> presetLocIds;             // Array of preset location ids, max 10
	PresetLocDict presetLocDict;               // The preseted camera locations
	VLocCodeDict viscaPresetLocDict;           // The location code array used for protocol VISCA
	LocationCode viscaFullScreen;              // The full screen code for protocol VISCA
	int cameraVelocity;
	int pixRangeOverlap;                       // The overlap width between two locations, in pixels
	BOOL bShowTracking;                        // Flag denote showing tracking result or not

	std::vector<int> beginTrackArea;           // The rect to begin tracking, (x,y,width,height)
	std::vector<int> stopTrackArea;            // Out this rect, stop tracking, (x,y,width,height)
	int leastHumanGap;                         // The least width of gap between humans, in pixel
	int humanWidth;                            // The width of human, in pixel
	int disappearFrameThresh;                  // If an object disappeared more than this thresh times, it is truly disappeared.
	int centerWeightThresh;                    // If an object's barycenter's weight is bigger than this thresh, it is a human
	int fgLowThresh;                           // Foreground low thresh, used in the GBM training
	int fgUpThresh;                            // Foreground up thresh, used in the GBM training
	double fgHistThresh;                       // Foreground hist thresh
	double gbmLearningRate;                    // GBM's learning rate, used in the GBM training
	int trackingInterval;                      // every trackingInterval frames do once tracking

	BlindZoneList blindZones;                  // Blind Zone of the teacher view
};

class StudentEntity
{
public:
	int id;
};

class LaserPointEntity
{
public:
	int id;
};