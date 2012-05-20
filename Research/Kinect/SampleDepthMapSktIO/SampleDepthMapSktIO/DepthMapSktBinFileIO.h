#ifndef _DEPTH_MAP_SKT_BIN_FILE_IO_H
#define _DEPTH_MAP_SKT_BIN_FILE_IO_H

#include "DepthMapSkt.h"

int ReadDepthMapSktBinFileHeader(FILE * fp, int &retNumFrames, int &retNCols, int &retNRows);

//the caller needs to allocate space for <retDepthMap>
int ReadDepthMapSktBinFileNextFrame(FILE * fp, int numCols, int numRows, CDepthMapSkt & retDepthMap);

//<fp> must be opened with "wb"
int WriteDepthMapSktBinFileHeader(FILE * fp, int nFrames, int nCols, int nRows);

//<fp> must be opened with "wb"
int WriteDepthMapSktBinFileNextFrame(FILE * fp, const CDepthMapSkt & depthMap);

#endif