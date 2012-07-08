// SampleDepthMapSktIO.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DepthMapSktBinFileIO.h"

//This is a sample program to load a depth video file (*.bin). 
//Authors: Zicheng Liu, Jiang Wang.
//A depth video file consists of a sequence of depth maps. Each frame corresponds to one depth map.
//A depth map is a matrix of depth values (see CDepthMapSkt in DepthMapSkt.h). For each pixel, its skeleton ID
//is also stored.
//I purposely did not create a data structure to hold all the frames of the depth video because the implementation
//of such data structure is better left to the application developers.
int _tmain(int argc, _TCHAR* argv[])
{
	char depthFileName[] = "D:\\code\\Data\\cheer up\\a08_s01_e02_depth.bin";
	FILE * fp = fopen(depthFileName, "rb");

	if(fp == NULL)
		return 1;

	
	int nofs = 0; //number of frames conatined in the file (each file is a video sequence of depth maps)
	int ncols = 0;
	int nrows = 0;
	ReadDepthMapSktBinFileHeader(fp, nofs, ncols, nrows);

	printf("number of frames=%i\n", nofs);

	//read each frame
	int f; //frame index
	for(f=0; f<nofs; f++)
	{
		CDepthMapSkt depthMap;
		depthMap.SetSize(ncols, nrows); //it allocates space
		//the data will be stored in <depthMap>
		ReadDepthMapSktBinFileNextFrame(fp, ncols, nrows, depthMap);

		//check to see what has been loaded for DEBUG purpose:
		int nNonZeroPoints = depthMap.NumberOfNonZeroPoints();
		float avg = depthMap.AvgNonZeroDepth();
		printf("frame[%i], ncols=%i, nrows=%i, count=%i, avg=%f\n", f, depthMap.GetNCols(), depthMap.GetNRows(), nNonZeroPoints, avg );
	}

	fclose(fp);
	fp=NULL;

	return 0;
}

