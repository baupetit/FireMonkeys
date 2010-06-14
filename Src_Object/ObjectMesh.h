#ifndef OBJECTMESH_H
#define OBJECTMESH_H

// object charge depuis une mesh
#include "Object.h"
#include "mesh.h"
#include <string>
using namespace std;

class ObjectMesh : public Object {
protected :
	void setValuation( Voxel& v ){
		for( int i = 0 ; i < 8 ; ++ i) {
			D3Vector p ;
			p(0) = v.corner[i].x ;p(1) = v.corner[i].y ;p(2) = v.corner[i].z ;
			v.valuation[i] = obj->phiEval( p , true ); // use trilinear interp
		}
	}
	
	inline bool isInside( Vecteur3D pos ){
		D3Vector p ;
		p(0) = pos.x ;p(1) = pos.y ;p(2) = pos.z ;
		return obj->phiEval( p, true ) < 0.0 ;
	}

	virtual void generateVoxels();
	
	Mesh * obj;
public:
	ObjectMesh( Voxel defVox,
		    string filename, Vecteur4D color );
	~ObjectMesh();
};

#endif
