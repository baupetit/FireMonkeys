#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Vecteur.h"

class BoundingBox {
public :
	Vecteur3D lowerCorner;
	Vecteur3D upperCorner;


	BoundingBox();
	BoundingBox( const Vecteur3D& lc, 
		     const Vecteur3D& uc );
	BoundingBox( const BoundingBox &AABB );
	~BoundingBox();
	
	BoundingBox& operator=( const BoundingBox& AABB );
	
	void translate( const Vecteur3D& tr );
	
	friend bool intersect( const BoundingBox& lhs, 
			       const BoundingBox& rhs );
};

#endif
