#ifndef NOEUD_H
#define NOEUD_H

/*!*****************************************************************************
*
*       \file       :      Noeud.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      26 mai 2010
*       \version    :      0.1
*
*******************************************************************************/

#include "Vecteur.h"
#include "BoundingBox.h"

class Noeud{
protected :
	Vecteur3D position;
	//Vecteur3D echelle; on s'en sert pas
	Vecteur3D anglesEuler;
	
	BoundingBox AABB;
public:
	Noeud() :position(0,0,0),anglesEuler(0,0,0){}
	~Noeud() {}	
	
	/* getters and setters */
	Vecteur3D  getPosition();
	void       setPosition( const Vecteur3D& newPos );
	
	//Vecteur3D  getEchelle();
	//void       setEchelle( const Vecteur3D& newEchelle );
	
	Vecteur3D  getAnglesEuler();
	void       setAnglesEuler( const Vecteur3D& newAngle );

	BoundingBox getAABB();
	void        setAABB( const BoundingBox& AABB );
	

	/* others usefull stuff */
	void        translate( const Vecteur3D& tr );
	void        rotateX( float angle );
	void        rotateY( float angle );
	void        rotateZ( float angle );
	void        rotateXYZ( float angleX, float angleY,
			       float angleZ );
};


#endif
