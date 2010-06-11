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
	if ( grille[_Grille_Ind(i,j,k)].plein && i-1>=0) {
		if( !grille[_Grille_Ind(i-1,j,k)].plein ) {
		    grille[_Grille_Ind(i-1,j,k)].frontiere = true ;
					
		    grille[_Grille_Ind(i-1,j,k)].repulsion.x = repulse(i-1,j,k).x;
		    grille[_Grille_Ind(i-1,j,k)].repulsion.y = repulse(i-1,j,k).y;
		    grille[_Grille_Ind(i-1,j,k)].repulsion.z = repulse(i-1,j,k).z;
			
		}    
		if( !grille[_Grille_Ind(i+1,j,k)].plein && i+1<=grilleSize.x){
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
		if( !grille[_Grille_Ind(i,j-1,k)].plein && j-1>=0) {
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
		if( !grille[_Grille_Ind(i,j+1,k)].plein && j+1<=grilleSize.y) {
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
		if( !grille[_Grille_Ind(i,j,k-1)].plein && k-1>=0 ) {
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
		if( !grille[_Grille_Ind(i,j,k+1)].plein && k+1<=grilleSize.z) {
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

	Vecteur3D repulse (int i, int j, int k){
	//condition i j k est donnée pour une frontiere, on cherche tous les frontieres distantes de 1
		
		Vecteur3D vec = Vecteur3D(0,0,0);
		if (grille[_Grille_Ind(i  ,j  ,k  )].frontiere){
		//comparaison aux 9 devant	
		if (grille[_Grille_Ind(i  ,j  ,k-1)].plein)  vec += Vecteur3D(0,0,1); 		
		if (grille[_Grille_Ind(i  ,j-1,k-1)].plein)  vec += Vecteur3D(0,1,1);
		if (grille[_Grille_Ind(i  ,j+1,k-1)].plein)  vec += Vecteur3D(0,-1,1);
		if (grille[_Grille_Ind(i-1,j  ,k-1)].plein)  vec += Vecteur3D(1,0,1); 
		if (grille[_Grille_Ind(i-1,j-1,k-1)].plein)  vec += Vecteur3D(1,1,1);
		if (grille[_Grille_Ind(i-1,j+1,k-1)].plein)  vec += Vecteur3D(1,-1,1);
		if (grille[_Grille_Ind(i+1,j  ,k-1)].plein)  vec += Vecteur3D(-1,0,1);
		if (grille[_Grille_Ind(i+1,j-1,k-1)].plein)  vec += Vecteur3D(-1,1,1);
		if (grille[_Grille_Ind(i+1,j+1,k-1)].plein)  vec += Vecteur3D(-1,-1,1);

		//comparaison aux 9 derriere		
		if (grille[_Grille_Ind(i  ,j  ,k+1)].plein)  vec += Vecteur3D(0,0,-1); 		
		if (grille[_Grille_Ind(i  ,j-1,k+1)].plein)  vec += Vecteur3D(0,1,-1);
		if (grille[_Grille_Ind(i  ,j+1,k+1)].plein)  vec += Vecteur3D(0,-1,-1);
		if (grille[_Grille_Ind(i-1,j  ,k+1)].plein)  vec += Vecteur3D(1,0,-1); 
		if (grille[_Grille_Ind(i-1,j-1,k+1)].plein)  vec += Vecteur3D(1,1,-1);
		if (grille[_Grille_Ind(i-1,j+1,k+1)].plein)  vec += Vecteur3D(1,-1,-1);
		if (grille[_Grille_Ind(i+1,j  ,k+1)].plein)  vec += Vecteur3D(-1,0,-1);
		if (grille[_Grille_Ind(i+1,j-1,k+1)].plein)  vec += Vecteur3D(-1,1,-1);
		if (grille[_Grille_Ind(i+1,j+1,k+1)].plein)  vec += Vecteur3D(-1,-1,-1);

		//comparaison aux 8 du plan
		//if (grille[_Grille_Ind(i  ,j  ,k)].plein)  vec += Vecteur3D(0,0,-1); 		
		if (grille[_Grille_Ind(i  ,j-1,k)].plein)  vec += Vecteur3D(0,1,0);
		if (grille[_Grille_Ind(i  ,j+1,k)].plein)  vec += Vecteur3D(0,-1,0);
		if (grille[_Grille_Ind(i-1,j  ,k)].plein)  vec += Vecteur3D(1,0,0); 
		if (grille[_Grille_Ind(i-1,j-1,k)].plein)  vec += Vecteur3D(1,1,0);
		if (grille[_Grille_Ind(i-1,j+1,k)].plein)  vec += Vecteur3D(1,-1,0);
		if (grille[_Grille_Ind(i+1,j  ,k)].plein)  vec += Vecteur3D(-1,0,0);
		if (grille[_Grille_Ind(i+1,j-1,k)].plein)  vec += Vecteur3D(-1,1,0);
		if (grille[_Grille_Ind(i+1,j+1,k)].plein)  vec += Vecteur3D(-1,-1,0);	
		/*
		vec.x = (vec.x/(sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z)));
		vec.y = (vec.y/(sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z)));
		vec.z = (vec.z/(sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z)));
		*/
		
		//pivotage de Pi/2
		Vecteur3D vec_vect_y = Vecteur3D(-vec.z,0.0,vec.x);
		vec.x = (vec.x*vec_vect_y.x+vec.y*vec_vect_y.y+vec.z*vec_vect_y.z)*vec_vect_y.x - vec.x*vec.y;
		vec.y = (vec.x*vec_vect_y.x+vec.y*vec_vect_y.y+vec.z*vec_vect_y.z)*vec_vect_y.y + vec.x*vec.x + vec.z*vec.z ;
		vec.z = (vec.x*vec_vect_y.x+vec.y*vec_vect_y.y+vec.z*vec_vect_y.z)*vec_vect_y.z - vec.z*vec.y;

		vec.x = (vec.x/(sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z)));
		vec.y = (vec.y/(sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z)));
		vec.z = (vec.z/(sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z)));

		}
		//cout << "pour i j k = " << i << " " << j << " " << k << " " << "x= " << vec.x << " y= " << vec.y << " z= " << vec.z << endl;
	
		return vec;

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
