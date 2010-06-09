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

#include "Triangle.h"

#define _Grille_Ind(i,j,k) ((i)+((j)*grilleSize.x)+((k)*grilleSize.x*grilleSize.y))

class Object : public BasicEntite {
protected :
	// voxelisation
	Voxel defVox;
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

	inline Vecteur3D cellToPoint( Vecteur3I c ){
		Vecteur3D res = AABB.lowerCorner ;
		res.x += c.x * SolverParam::getSpaceDiv();
		res.y += c.y * SolverParam::getSpaceDiv();
		res.z += c.z * SolverParam::getSpaceDiv();
		return res ;
	}

	inline void setVoisinBound( int i, int j, int k ){
		if( !grille[_Grille_Ind(i-1,j,k)].plein ) grille[_Grille_Ind(i-1,j,k)].frontiere = true ;
		if( !grille[_Grille_Ind(i+1,j,k)].plein ) grille[_Grille_Ind(i+1,j,k)].frontiere = true ;
		if( !grille[_Grille_Ind(i,j-1,k)].plein ) grille[_Grille_Ind(i,j-1,k)].frontiere = true ;
		if( !grille[_Grille_Ind(i,j+1,k)].plein ) grille[_Grille_Ind(i,j+1,k)].frontiere = true ;
		if( !grille[_Grille_Ind(i,j,k-1)].plein ) grille[_Grille_Ind(i,j,k-1)].frontiere = true ;
		if( !grille[_Grille_Ind(i,j,k+1)].plein ) grille[_Grille_Ind(i,j,k+1)].frontiere = true ;
	}
	
	inline void setCornerCell( Voxel& v ){
		Vecteur3D orig = cellToPoint( v.pos );
		Vecteur3D BDL( -1, -1, -1 );
		Vecteur3D BDR(  1, -1, -1 );
		Vecteur3D BUL( -1,  1, -1 );
		Vecteur3D BUR(  1,  1, -1 );
		Vecteur3D FDL( -1, -1,  1 );
		Vecteur3D FDR(  1, -1,  1 );
		Vecteur3D FUL( -1,  1,  1 );
		Vecteur3D FUR(  1,  1,  1 );
				
		v.corner[0] = orig+BDL*(SolverParam::getSpaceDiv()/2);
		v.corner[1] = orig+FDL*(SolverParam::getSpaceDiv()/2);
		v.corner[2] = orig+FUL*(SolverParam::getSpaceDiv()/2);
		v.corner[3] = orig+BUL*(SolverParam::getSpaceDiv()/2);
		v.corner[4] = orig+BDR*(SolverParam::getSpaceDiv()/2);
		v.corner[5] = orig+FDR*(SolverParam::getSpaceDiv()/2);
		v.corner[6] = orig+FUR*(SolverParam::getSpaceDiv()/2);
		v.corner[7] = orig+BUR*(SolverParam::getSpaceDiv()/2);
	}

	virtual void setValuation( Voxel& v ) = 0;
public:
	Vecteur4D color ;
	
	/* constructor */
	Object();
	Object(Voxel defVox);

	/* destructor */
	virtual ~Object();
	

	inline Voxel getInfo( Vecteur3D point ){
		Vecteur3I coord = pointToCell( point );
		return grille[_Grille_Ind(coord.x,coord.y,coord.z)] ;
	}

	void diffuserTemperature( float dt );

	/* herited methods */
	virtual void Afficher( float dt );
	virtual void Afficher_Face_Camera(Vecteur3D& positionCamera,
					  Vecteur3D& directionCamera,
					  float dt ) ; 
};

#endif
