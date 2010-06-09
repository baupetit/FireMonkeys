#ifndef SOLVER_H
#define SOLVER_H

/**
 *@file   : solver.h
 *@author : FireMonkeys
 *@brief  : This file contains the solver specification
 */

/**
 * @class : Solver
 * @brief : This class is used to modelise the fluid motion in the environnement
 */

#define IX(i,j,k) ((i)+(N+2)*(j)+((N+2)*(N+2))*(k))

#include "Object.h"
#include "BoundingBox.h"
#include "Vecteur.h"
#include "Voxel.h"
#include <list>

class Solver {
public :
	/**
	 * @brief : constructor
	 */
	Solver( int N );

	/**
	 * @brief : destructor
	 */
	~Solver();

	/**
	 * @brief : move densities over a dt timestep with a certain
	 * diffusion factor
	 */
	void densitiesStep( float dt );

	/**
	 * @brief : compute velocities evolution over a dt timestep
	 * using visc as the viscosity of the fluid
	 */
	void velocitiesStep( float dt );


	/**
	 * @brief : move densities over a dt timestep with a certain
	 * diffusion factor with Temperature
	 */
	void densitiesStepWithTemp ( float dt );

	/**
	 * @brief : compute velocities evolution over a dt timestep
	 * using visc as the viscosity of the fluid with Temperature
	 */
	void velocitiesStepWithTemp( float dt );


	/**
	 * @brief : return the fire density matrix
	 */
	const float* getDensities() const ;

	/**
	 * @brief : return the smoke density matrix
	 */
	const float* getSmokes() const ;

	/**
	 * @brief : return the density matrix
	 */
	const float* getTemperatures() const ;

	/**
	 * @brief : return the velocityU matrix
	 */
	const float* getVelocityU() const;
	/**
	 * @brief : return the velocityV matrix
	 */
	const float* getVelocityV()const;
	/**
	 * @brief : return the velocityW matrix
	 */
	const float* getVelocityW()const;
	
	/**
	 * @brief : return the grid size
	 */
	int getSize() const ;

	/**
	 * @brief : set density in the given cell
	 */ 
	void setDensity( int i , int j , int k , float dens );

	/**
	 * @brief : set temperature at the given cell
	 */
	void setTemperature( int i, int j, int k, float temp );

	/**
	 * @brief : set the velocity in the given cell
	 */
	void setVelocity( int i, int j , int k , float u, float v, float w );
	
	/**
	 * @brief : clear filled information
	 */
	void clearFilledInfo();

    /**
     * Mise à jour des infos dans la grille des voxels
     */
    void updateInfo( Object& o);

    
	inline Vecteur3D cellToPoint( int i, int j, int k){
		Vecteur3D res = AABB.lowerCorner;
		res.x += (i-1) * SolverParam::getSpaceDiv();
		res.y += (j-1) * SolverParam::getSpaceDiv();
		res.z += (k-1) * SolverParam::getSpaceDiv();
		return res ;
	}

    inline Vecteur3I objectCellToSolverCell(Vecteur3I& objectCell, Vecteur3D& objectLowerCorner ){
        Vecteur3I res = objectCell;
        res.x += (int)  ( (objectLowerCorner.x - AABB.lowerCorner.x) / SolverParam::getSpaceDiv() );
        res.y += (int)  ( (objectLowerCorner.y - AABB.lowerCorner.y) / SolverParam::getSpaceDiv() );
        res.z += (int)  ( (objectLowerCorner.z - AABB.lowerCorner.z) / SolverParam::getSpaceDiv() );
    }


protected :
	/* size of the grid */
	int _N ;
	
	/* densities */
	float *_d, *_d0;
	float *_f, *_f0;

	/* temperature */
	float *_T, *_T0;

	/* velocities */
	float *_v, *_v0;
	float *_u, *_u0;
	float *_w, *_w0;

	/* Sources */
	float *_srcd;
	float *_srcT;
	float *_srcu;
	float *_srcv;
	float *_srcw;

	/* Voxels infos */
	Voxel *_filled;

    /** Boite englobante */
	BoundingBox AABB;



    
	inline bool isSolidCell( int i, int j, int k ){
		int N = _N ;
		return _filled[IX(i,j,k)].plein;
	}
	
	inline Vecteur3D getObstacleVelocity( int i, int j, int k ){
		int N = _N ;
		return _filled[IX(i,j,k)].repulsion;
	}
	





	void addSource ( int N, float *x , float *s , float dt );	
    void addSource3 ( int N, 
                      float *x1 , float *s1 ,
                      float *x2 , float *s2 ,
                      float *x3 , float *s3 ,
                      float dt );                         
	void combustion( int N, float *x , float *f, float *T, 
			 float consumeK, float fireToSmoke,
			 float dt );
	void addBuoyancy( int N, float *T, float *v, float buoy, float dt);
	void setBoundaries ( int N, int b, float *x );
	void setBoundariesB0 ( int N, float *x );
	void linearSolve ( int N, int b, float * x, float * x0, float a, float c );
	void diffuse ( int N, int b, float * x, float * x0, float diff, float dt );
	void diffuseFireAndSmoke ( int N, int b1, int b2, int b3,
				   float * x, float * x0, 
				   float * f, float * f0, 
				   float * t, float * t0, 
				   float diffFire, float diffSmoke, float diffTemp, 
				   float dt );
	void advect ( int N, int b, float * d, float * d0, float * u, float * v, float *w, float dt );
	void advect_cool ( int N, int b, 
			   float * d, float * d0, 
			   float * u, float * v, float *w, 
			   float dt );
	void calcDiv( int N , float *u, float *v, float *w, float *div, float *p );
	void correctVel( int N, float *u, float * v, float *w, float * p);
	void project ( int N, float * u, float * v, float *w, float * p, float * div );
	void vorticity_confinement( int N, float *u, float *v, float *w, 
				    float *u0, float *v0, float *w0, float *T0, 
				    float vc_eps, float dt);
	




};

#endif
