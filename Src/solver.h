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
	void densitiesStep( float diff , float dt );

	/**
	 * @brief : compute velocities evolution over a dt timestep
	 * using visc as the viscosity of the fluid
	 */
	void velocitiesStep( float visc , float dt );

	/**
	 * @brief : return the density matrix
	 */
	const float* getDensities() const ;

	/**
	 * @brief : return the grid size
	 */
	int getSize() const ;

	/**
	 * @brief : set density in the given cell
	 */ 
	void setDensity( int i , int j , int k , float dens );

	/**
	 * @brief : set the velocity in the given cell
	 */
	void setVelocity( int i, int j , int k , float u, float v, float w );


protected :
	/* size of the grid */
	int _N ;
	
	/* densities */
	float *_d, *_d0;

	/* velocities */
	float *_v, *_v0;
	float *_u, *_u0;
	float *_w, *_w0;
};

#endif
