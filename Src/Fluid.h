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
#include "tempToRGB.h"
#include "Shader.h"
#include "Perlin.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

class Fluid : public BasicEntite {

private :
	/** Solver 3D */
	Solver *s;
	Perlin *p;
	Vecteur3D *perl ;
	Vecteur3D *perl_temps ;
	/** Temperature indexes */
	TempToRGB *tempIndex;

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
	GLuint _id_texture_perlin;
	GLuint _id_texture_perlin_temps;
	/**densité générée*/
	float gen_dens;
	/** matrice RGBA pour charger la texture 3D */
	Vecteur4D *matriceRGBA_fire;
	Vecteur4D *matriceRGBA_smoke;
	
	float tps1;
	float tps2;
    float tps3;
	/**ID du programme shader pour la multitexture*/
	Shader *renderer;
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
	void renduFumeeGPUFaceCamera(Vecteur3D& positionCamera, 
	                             Vecteur3D& directionCamera );
	/** Affiche la flamme face a la camera*/
	void renduFlammeGPUFaceCamera(Vecteur3D& positionCamera, 
	                              Vecteur3D& directionCamera );
	/** Affiche la flamme et la fumee face a la camera*/
	void renduFlammeETFumeeGPUFaceCamera( int nb_plans,
					      Vecteur3D& positionCamera,
					      Vecteur3D& directionCamera,
						  float t );
	/** Permet de charger le tableau de la fumée dans la matrice RGBA */
	void majMatriceFumeeEnMatriceRGBA();
	/** Permet de charger le tableau de la flamme dans la matrice RGBA */
	void majMatriceFlammeEnMatriceRGBA();
	/** Permet de charger la matrice RGBA de la classe dans une texture 3D */
	void matriceRGBACarreeToTexture3D(const Vecteur4D *matrice, int cote, GLuint id_texture);
	void matricePerlinCarreeToTexture3D(const Vecteur3D *matrice, int cote, GLuint id_texture);
	/** Dessine des plans dans la texture3D */
	void dessinerPlansDansTexture3D(GLuint id_texture, int nb_plans);
	                                
	/** Dessine des plans dans la texture3D face a la camera*/
	void dessinerPlansDansTexture3DFaceALaCamera(int nb_plans,
	                     Vecteur3D& positionCamera, Vecteur3D& directionCamera);

	void VecteurPerlinTempsToTexture1D(const Vecteur3D *matrice, int cote, GLuint id_texture);
	
public :
	void Afficher( float dt );
	void Afficher_Face_Camera(Vecteur3D& positionCamera, Vecteur3D& directionCamera,float dt);
	void Mise_A_Jour( float dt );
};


#endif
