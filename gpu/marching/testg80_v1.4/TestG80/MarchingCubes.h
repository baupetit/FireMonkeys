#ifndef MarchingCubes_h
#define MarchingCubes_h

#include "Vector.h"

using namespace NemoGraphics;

class GridCell{
public:
	float val[8];
	Vector3f pos[8];
};

void RenderMarchCube(float *data, Vector3i size, Vector3i gridsize, float isolevel);

int Polygonise(GridCell &grid, float isolevel, Vector3f *triangles);

#endif