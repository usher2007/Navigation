#ifndef SKELETON_READER
#define SKETETON_READER

#include <fstream>

class SkeletonReader
{
public:
	SkeletonReader(void);
	SkeletonReader(const char *fileName);
	void ReadNextFrame(double *rows, double *cols);
private:
	std::ifstream skeletonFile;
	int frameCount;
	int numOfJoint;
};
#endif