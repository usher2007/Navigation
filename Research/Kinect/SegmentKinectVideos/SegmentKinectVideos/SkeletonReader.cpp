#include "SkeletonReader.h"

SkeletonReader::SkeletonReader(const char *fileName)
{
	skeletonFile.open(fileName);
	skeletonFile>>frameCount;
	skeletonFile>>numOfJoint;
}

void SkeletonReader::ReadNextFrame(double *rows, double *cols)
{
	int rowCount;
	skeletonFile>>rowCount;
	int tmp1;
	int tmp2;
	double col;
	double row;
	char tmp[100];
	for(int i=0; i<rowCount; i++)
	{
		if(i%2==0)
		{
			skeletonFile.getline(tmp, 100);
			bool ret = skeletonFile.getline(tmp,100);
			continue;
		}
		skeletonFile>>col>>row>>tmp1>>tmp2;
		rows[(i-1)/2] = row;
		cols[(i-1)/2] = col;	
	}
	return;
}