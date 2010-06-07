#include "BoundingBox.h"

BoundingBox::BoundingBox()
	:lowerCorner(0,0,0), upperCorner(0,0,0)
{}

BoundingBox::BoundingBox( const Vecteur3D& lc, 
			  const Vecteur3D& uc )
	:lowerCorner(lc), upperCorner(uc)
{}

BoundingBox::BoundingBox( const BoundingBox &AABB ){
	lowerCorner = AABB.lowerCorner ;
	upperCorner = AABB.upperCorner ;
}

BoundingBox::~BoundingBox() {}

BoundingBox& BoundingBox::operator=( const BoundingBox& AABB ){
	lowerCorner = AABB.lowerCorner ;
	upperCorner = AABB.upperCorner ;
	return *this;
}

void BoundingBox::translate( const Vecteur3D& tr ){
	lowerCorner += tr ;
	upperCorner += tr ;
}

bool intersect( const BoundingBox& lhs, 
		const BoundingBox& rhs ){
	if(    lhs.upperCorner.x < rhs.lowerCorner.x
	    || lhs.lowerCorner.x > rhs.upperCorner.x )
		return false ;
	if(    lhs.upperCorner.y < rhs.lowerCorner.y
	    || lhs.lowerCorner.y > rhs.upperCorner.y )
		return false ;
	if(    lhs.upperCorner.z < rhs.lowerCorner.z
	    || lhs.lowerCorner.z > rhs.upperCorner.z )
		return false ;
	
	return true ;
}
