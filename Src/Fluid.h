#ifndef FLUID_H
#define FLUID_H

/*!*****************************************************************************
*
*       \file       :      Fluid.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      26 mai 2010
*       \version    :      0.1
*
*******************************************************************************/

#include "solver.h"
#include "BasicEntite.h"

#include <GL/glut.h>
#include <GL/gl.h>

class Fluid : public BasicEntite {

private :
    /** Solver 3D */
	Solver *s;
	/** Taille de la résolution */
    int tailleGrille;
	/** Faut il afficher la fumee */
	bool afficher_fumee;
	/** Faut il afficher la flamme */
	bool afficher_flamme;	
	/** id de texture3d pour la flamme */
    GLuint _id_texture_flamme;
	/** id de texture3d pour la fumee */
    GLuint _id_texture_fumee;
    /** matrice RGBA pour charger la texture 3D */
    float *matriceRGBA;
    
public :
	Fluid();
	~Fluid();
	
private:
    /** Initialise les textures */
	void initialiserRenduGPU();
	/** Affiche la fumee */
	void renduFumeeGPU();
	/** Affiche la flamme */
	void renduFlammeGPU();
	/** Affiche la fumee face a la camera*/
	void renduFumeeGPUFaceCamera(Vecteur3D& positionCamera );
	/** Affiche la flamme face a la camera*/
	void renduFlammeGPUFaceCamera(Vecteur3D& positionCamera );
	/** Permet de charger le tableau de la fumée dans la matrice RGBA */
	void majMatriceFumeeEnMatriceRGBA();
	/** Permet de charger la matrice RGBA de la classe dans une texture 3D */
	void matriceRGBACarreeToTexture3D(const float *matrice, int cote, GLuint id_texture);
	/** Dessine des plans dans la texture3D */
	void dessinerPlansDansTexture3D(GLuint id_texture, int nb_plans);
	                                
	/** Dessine des plans dans la texture3D face a la camera*/
	void dessinerPlansDansTexture3DFaceALaCamera(GLuint id_texture, int nb_plans,
	                                     Vecteur3D& positionCamera);
	
public :
	void Afficher();
	void Afficher_Face_Camera(Vecteur3D& positionCamera );
	void Mise_A_Jour();
};


#endif
