#include "Noeud.h"

Vecteur3D   Noeud::getPosition(){
	return position ;
}
void        Noeud::setPosition( const Vecteur3D& newPos ){
	position = newPos ;
}

Vecteur3D   Noeud::getAnglesEuler(){
	return anglesEuler ;
}
void        Noeud::setAnglesEuler( const Vecteur3D& newAngle ){
	anglesEuler = newAngle;
}

BoundingBox Noeud::getAABB(){
	return AABB ;
}
void        Noeud::setAABB( const BoundingBox& AABB ){
	this->AABB = AABB ;
}

Vecteur3D Noeud::getVitesse(){
	return vitesse;
}
void Noeud::setVitesse( const Vecteur3D& v ){
	vitesse = v;
}

/* others usefull stuff */
void        Noeud::translate( const Vecteur3D& tr ){
	AABB.translate( tr );
	position += tr ;
}
void        Noeud::rotateX( float angle ){
	anglesEuler.x += angle ;
}
void        Noeud::rotateY( float angle ){
	anglesEuler.y += angle ;
}
void        Noeud::rotateZ( float angle ){
	anglesEuler.z += angle ;
}
void        Noeud::rotateXYZ( float angleX, float angleY,
			      float angleZ ){
	anglesEuler.x += angleX ;
	anglesEuler.y += angleY ;
	anglesEuler.z += angleZ ;
}
void        Noeud::updateVitesse(){
    vitesse = position - prevPosition;
    prevPosition = position;
}


