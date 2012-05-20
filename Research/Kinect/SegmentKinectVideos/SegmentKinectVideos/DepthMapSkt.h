#ifndef CLASS_DEPTH_MAP
#define CLASS_DEPTH_MAP

#include <stdint.h>
typedef unsigned char uint8_t;
typedef unsigned char uchar;
//a depthmap is a matrix of depth values. A depth value is invalid if it is equal to 0 (i.e. the sensor has no data on this pixel).
class CDepthMapSkt
{
public:
	CDepthMapSkt(void);
	CDepthMapSkt(int ncols, int nrows, uchar* raw_data);
	~CDepthMapSkt(void);

	int GetNRows() const {return m_nrows;}
	int GetNCols() const {return m_ncols;}
	void GetCoords(int r, int c, float retCoord[3]) const;
	void SetSize(int ncols, int nrows); //allocate space

	void SetItem(int r, int c, float val);
	float GetItem(int r, int c) const;
	void SetSkeletonID(int r, int c, uint8_t val);
	uint8_t GetSkeletonID(int r, int c) const;
	void CopyDepthMapTo(CDepthMapSkt & dstMap) const;
	void convertToChar (uchar* dst) const; //assume the memory has been located
	void Scale(float s); //scale the depth
	void ScaleSizeNN(float scaleFactor); //ncols = floor(scaleFactor * ncols), nrows = floor(scaleFactor * nrows)
	void ScaleSizeNN(float scaleFactorWidth, float scaleFactorHeight);
    void convertToInt( uint8_t* dst ) const;
	int NumberOfNonZeroPoints() const;
	float AvgNonZeroDepth() const;
	float* GetPtr();

	int NumPointsWithSkeletonID(uint8_t val) const;
protected:
	int m_ncols; //ncols
	int m_nrows; //nrows
	float * m_depthVals; //nrows * ncols
	uint8_t* m_skIDVals; //nrows * ncols
	float m_nonZeroMaxVals;
	float m_nonZeroMinVals;
	int m_nonZeroCount;
	float m_nonZeroSum;
};
#endif
