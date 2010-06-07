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



class Solver_GPU {

private :
	/** Shader de resolution de feu */
	Shader *shader_add_sources;
	Shader *shader_linear_solve;
	Shader *shader_advect_cool;
	Shader *shader_advect_speed;
	Shader *shader_buoy;
	Shader *shader_div;
	Shader *shader_linear_solve_pression;
	Shader *shader_velocity_correction;
    
	/** Dans quel cas sommmes nous */
	int pingpong;
    
	/** Buffer */
	Framebuffer *buffer;
    
	/** Tailles*/
	int _grille_width;
	int _grille_height;
	int _grille_depth;
    
	/** Matrices */
	Texture3D *_grille_feu_sources;
	Texture3D *_grille_feu_courante;
	Texture3D *_grille_feu_dest;
	Texture3D *_grille_vitesse_sources;
	Texture3D *_grille_vitesse_courante;
	Texture3D *_grille_vitesse_dest;
	Texture3D *_grille_pression_courante;
	Texture3D *_grille_pression_dest;
    
public :
    // CONSTRUCTEURS 

	/**
	 * @brief : constructor
	 */
	Solver_GPU( int width, int height, int depth );

	/**
	 * @brief : destructor
	 */
	~Solver_GPU();



    //// ACCESSEURS    
    
    /** Retourne l'id de la texture de densite */
	const GLuint getDensities() const ;
	
    /** Retourne l'id de la texture de destination du calcul de densite */
	const GLuint getDestDensities() const ;
	
    /** Retourne l'id de la texture de sources */
    const GLuint getSources() const ;

    /** Retounr d'id de la texture de vitesse */
	const GLuint getSpeed() const ;

    /** Retounr d'id de la texture de pression */
	const GLuint getPression() const ;

    //// DENSITIES STEP
    
	/** Effectue l'etape de densité */
	void densitiesStepWithTemp ( float dt );

    /** Ajout des sources de fumee / feu / chaleur */
    void addSource(float dt, Texture3D& textureCible, Texture3D& textureSource );
    void addBuoyancy(float dt);

    /** Resolution lineaire */
    void linearSolve ( int b, float dt, Vecteur3D& a, Texture3D* source, Texture3D* dest);
    void linearSolve_speed ( int b, float dt);
    void linearSolvePression ( int b, float dt);
    
	/** Phase de diffusion */
	void diffuse ( float dt);
	void diffuse_speed ( float dt);
	
	/** Phase d'advection (avec refroidissement ) */
    void advect_cool ( float dt );
    void advect_speed ( float dt );


    /** Echange deux textures */
    void swapGrilles(Texture3D** t1, Texture3D** t2);
    

    //// VELOCITIES STEP
    
	/** Effectue l'etape des vitesses */
	void velocitiesStepWithTemp( float dt );

    void project();

    void velocityCorrection();


    
};

#endif
