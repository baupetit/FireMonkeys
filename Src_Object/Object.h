#ifndef OBJECT_H
#define OBJECT_H
/*!*****************************************************************************
 *
 *       \file       :      Object.h
 *
 *       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
 *       \date       :      4 jun 2010
 *       \version    :      0.1
 *
 *******************************************************************************/
#include "BasicEntite.h"
#include "Voxel.h"
#include "Vecteur.h"
#include "SolverParam.h"
#include <cmath>

#define _Grille_Ind(i,j,k) ((i)+((j)*grilleSize.x)+((k)*grilleSize.x*grilleSize.y))

class Object : public BasicEntite {
protected :
	Voxel *grille;
	Vecteur3I grilleSize;
	
	virtual void generateVoxels() = 0;

	inline Vecteur3I pointToCell( Vecteur3D p ){
		Vecteur3I res;
		res.x = lround((p.x-AABB.lowerCorner.x) / SolverParam::getSpaceDiv()) ;
		res.y = lround((p.y-AABB.lowerCorner.y) / SolverParam::getSpaceDiv()) ;
		res.z = lround((p.z-AABB.lowerCorner.z) / SolverParam::getSpaceDiv()) ;
		return res;
	}

public:
	/* constructor */
	Object();
	
	/* destructor */
	virtual ~Object();
	

	inline Voxel getInfo( Vecteur3D point ){
		Vecteur3I coord = pointToCell( point );
		return grille[_Grille_Ind(coord.x,coord.y,coord.z)] ;
	}

	void diffuserTemperature( float dt );

	/* herited methods */
	virtual void Afficher( float dt ) = 0;
	virtual void Afficher_Face_Camera(Vecteur3D& positionCamera,
					  Vecteur3D& directionCamera,
					  float dt ) ; 
};

#endif
