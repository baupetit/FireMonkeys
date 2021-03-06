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
};

#endif
