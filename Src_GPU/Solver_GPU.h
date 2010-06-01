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


#define LARGEUR    20
#define HAUTEUR    20
#define PROFONDEUR 20

#define ID(i,j,k) ((i)+(LARGEUR)*(j)+((LARGEUR)*(HAUTEUR))*(k))

class Solver_GPU {

private :

    Shader *shader_feu;
    
    /** Gestionnaire de buffer */
    Framebuffer *buffer_feu_1;
    Framebuffer *buffer_feu_2;
    int buffer_courant;
    
    /** Tailles*/
    int _grille_width;
    int _grille_height;
    int _grille_depth;
    
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
	void densitiesStep( float diff , float dt );

	/**
	 * @brief : compute velocities evolution over a dt timestep
	 * using visc as the viscosity of the fluid
	 */
	void velocitiesStep( float visc , float dt );


	/**
	 * @brief : move densities over a dt timestep with a certain
	 * diffusion factor with Temperature
	 */
	void densitiesStepWithTemp ( float diffFire,
				     float diffSmoke,
				     float diffTemp,
				     float cool, 
					 float consume,
				     float sub, 
				     float fireTosmoke, 
				     float dt );

	/**
	 * @brief : compute velocities evolution over a dt timestep
	 * using visc as the viscosity of the fluid with Temperature
	 */
	void velocitiesStepWithTemp( float visc , 
				     float buoy, 
				     float vc_eps, 
				     float dt );


	/**
	 * @brief : return the fire density matrix
	 */
	const GLuint getDensities() const ;

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






    Framebuffer& getBufferFeuCourant () const ;
    Framebuffer& getBufferFeuDestination() const ;
    void swapBufferCourant();
    void dessinerCarre();
    
    
    
    void diffuse ( Shader& calcul_shader, 
               string nom_texture, 
               Framebuffer originBuffer,
               Framebuffer destBuffer,
               float diff, 
               float dt );
    
};

#endif
