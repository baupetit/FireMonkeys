#ifndef MARCHINGCUBE_H
#define MARCHINGCUBE_H

#include "Voxel.h"
#include "Vecteur.h"

//given a grid cell, returns the set of triangles that approximates the region where val == 0.
int Polygonise( Voxel &vox );

Vecteur3D VertexInterp(const Vecteur3D &p1, const Vecteur3D &p2, float valp1, float valp2);

#endif
