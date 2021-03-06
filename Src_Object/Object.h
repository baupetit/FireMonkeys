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
class Solver;

// define pour valuation
#define _BDL_ 0
#define _BDR_ 4
#define _BUL_ 3
#define _BUR_ 7
#define _FDL_ 1
#define _FDR_ 5
#define _FUL_ 2
#define _FUR_ 6


#define _Grille_Ind(i,j,k) ((i)+((j)*grilleSize.x)+((k)*grilleSize.x*grilleSize.y))

class Object : public BasicEntite {
	friend class Solver;
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
		Vecteur3D res = AABB.lowerCorner;
		res.x += c.x * SolverParam::getSpaceDiv();
		res.y += c.y * SolverParam::getSpaceDiv();
		res.z += c.z * SolverParam::getSpaceDiv();
		return res ;
	}

	inline void setVoisinBound( int i, int j, int k ){

		if ( grille[_Grille_Ind(i,j,k)].plein ) {
			if( !grille[_Grille_Ind(i-1,j,k)].plein ) {
				grille[_Grille_Ind(i-1,j,k)].frontiere = true ;
				grille[_Grille_Ind(i-1,j,k)].repulsion.x = repulse(i-1,j,k).x;
				grille[_Grille_Ind(i-1,j,k)].repulsion.y = repulse(i-1,j,k).y;
				grille[_Grille_Ind(i-1,j,k)].repulsion.z = repulse(i-1,j,k).z;
			

			}    
			if( !grille[_Grille_Ind(i+1,j,k)].plein ) {
				grille[_Grille_Ind(i+1,j,k)].frontiere = true ;
				grille[_Grille_Ind(i+1,j,k)].repulsion.x = repulse(i+1,j,k).x;
				grille[_Grille_Ind(i+1,j,k)].repulsion.y = repulse(i+1,j,k).y;
				grille[_Grille_Ind(i+1,j,k)].repulsion.z = repulse(i+1,j,k).z;
				/*
				  grille[_Grille_Ind(i+1,j,k)].repulsion.x = +1.0;
				  grille[_Grille_Ind(i+1,j,k)].repulsion.y = 0.0;
				  grille[_Grille_Ind(i+1,j,k)].repulsion.z = 0.0;
				*/
			}    
			if( !grille[_Grille_Ind(i,j-1,k)].plein ) {
				grille[_Grille_Ind(i,j-1,k)].frontiere = true ;
				grille[_Grille_Ind(i,j-1,k)].repulsion.x = repulse(i,j-1,k).x;
				grille[_Grille_Ind(i,j-1,k)].repulsion.y = repulse(i,j-1,k).y;
				grille[_Grille_Ind(i,j-1,k)].repulsion.z = repulse(i,j-1,k).z;
				/*
				  grille[_Grille_Ind(i,j-1,k)].repulsion.x = 0.0;
				  grille[_Grille_Ind(i,j-1,k)].repulsion.y = -1.0;
				  grille[_Grille_Ind(i,j-1,k)].repulsion.z = 0.0;
				*/
			}    
			if( !grille[_Grille_Ind(i,j+1,k)].plein ) {
				grille[_Grille_Ind(i,j+1,k)].frontiere = true ;
				grille[_Grille_Ind(i,j+1,k)].repulsion.x = repulse(i,j+1,k).x;
				grille[_Grille_Ind(i,j+1,k)].repulsion.y = repulse(i,j+1,k).y;
				grille[_Grille_Ind(i,j+1,k)].repulsion.z = repulse(i,j+1,k).z;
				/*
				  grille[_Grille_Ind(i,j+1,k)].repulsion.x = 0.0;
				  grille[_Grille_Ind(i,j+1,k)].repulsion.y = 1.0;
				  grille[_Grille_Ind(i,j+1,k)].repulsion.z = 0.0;
				*/
			}    
			if( !grille[_Grille_Ind(i,j,k-1)].plein ) {
				grille[_Grille_Ind(i,j,k-1)].frontiere = true ;
				grille[_Grille_Ind(i,j,k-1)].repulsion.x = repulse(i,j,k-1).x;
				grille[_Grille_Ind(i,j,k-1)].repulsion.y = repulse(i,j,k-1).y;
				grille[_Grille_Ind(i,j,k-1)].repulsion.z = repulse(i,j,k-1).z;
				/*
				  grille[_Grille_Ind(i,j,k-1)].repulsion.x = 0.0;
				  grille[_Grille_Ind(i,j,k-1)].repulsion.y = 0.0;
				  grille[_Grille_Ind(i,j,k-1)].repulsion.z = -1.0;
				*/
			}    
			if( !grille[_Grille_Ind(i,j,k+1)].plein ) {
				grille[_Grille_Ind(i,j,k+1)].frontiere = true ;
				grille[_Grille_Ind(i,j,k+1)].repulsion.x = repulse(i,j,k+1).x;
				grille[_Grille_Ind(i,j,k+1)].repulsion.y = repulse(i,j,k+1).y;
				grille[_Grille_Ind(i,j,k+1)].repulsion.z = repulse(i,j,k+1).z;
				/*
				  grille[_Grille_Ind(i,j,k+1)].repulsion.x = 0.0;
				  grille[_Grille_Ind(i,j,k+1)].repulsion.y = 0.0;
				  grille[_Grille_Ind(i,j,k+1)].repulsion.z = 1.0;
				*/
			}   
		}

	}


	Vecteur3D repulse (int i, int j, int k);

	
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
				
		v.corner[_BDL_] = orig+BDL*(SolverParam::getSpaceDiv()/2);
		v.corner[_FDL_] = orig+FDL*(SolverParam::getSpaceDiv()/2);
		v.corner[_FUL_] = orig+FUL*(SolverParam::getSpaceDiv()/2);
		v.corner[_BUL_] = orig+BUL*(SolverParam::getSpaceDiv()/2);
		v.corner[_BDR_] = orig+BDR*(SolverParam::getSpaceDiv()/2);
		v.corner[_FDR_] = orig+FDR*(SolverParam::getSpaceDiv()/2);
		v.corner[_FUR_] = orig+FUR*(SolverParam::getSpaceDiv()/2);
		v.corner[_BUR_] = orig+BUR*(SolverParam::getSpaceDiv()/2);
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
	
	void voxelConsome( Voxel *v );
	
	/* herited methods */
	virtual void Afficher( float dt );
	virtual void Afficher_Face_Camera(Vecteur3D& positionCamera,
					  Vecteur3D& directionCamera,
					  float dt ) ; 
			
};

#endif
