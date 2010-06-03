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


#include "Shader.h" 
#include "Framebuffer.h"
#include "Vecteur.h"
#include "Texture3D.h"


#define LARGEUR    20
#define HAUTEUR    20
#define PROFONDEUR 20

#define PING 1
#define PONG 2

#define ID(i,j,k) ((i)+(LARGEUR)*(j)+((LARGEUR)*(HAUTEUR))*(k))

class Solver_GPU {

private :
	/** Shader de resolution de feu */
	Shader *shader_linear_solve;
    
	/** Dans quel cas sommmes nous */
	int pingpong;
    
	/** Buffer */
	Framebuffer *buffer;
    
	/** Tailles*/
	int _grille_width;
	int _grille_height;
	int _grille_depth;
    
	/** Matrices */
	Texture3D *_grille_temp;
	Texture3D *_grille_feu_courante;
	Texture3D *_grille_feu_dest;
    
public :
	/**
	 * @brief : constructor
	 */
	Solver_GPU( int width, int height, int depth );

	/**
	 * @brief : destructor
	 */
	~Solver_GPU();

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
	const GLuint getDensities() const ;
	const GLuint getDestDensities() const ;
    const GLuint getTemp() const ;
	
	

	/**
	 * @brief : return the smoke density matrix
	 */
	const GLuint getSmokes() const ;

	/**
	 * @brief : return the density matrix
	 */
	const GLuint getTemperatures() const ;

	/**
	 * @brief : return the grid size
	 */
	int getWidth() const ;
	int getHeight() const ;
	int getDepth() const ;


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



    
	/** Dessine un carre */
	void dessinerCarre();
    
	/** Phase de diffusion */
	void diffuse ( float diff, 
		           float dt );





    void linearSolve ( int b, float a1, float a2, float a3 );

    void swapGrilles(Texture3D** t1, Texture3D** t2);
    
};

#endif
