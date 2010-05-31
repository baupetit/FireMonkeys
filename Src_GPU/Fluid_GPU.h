#ifndef FLUID_GPU_H
#define FLUID_GPU_H

/*!*****************************************************************************
*
*       \file       :      Fluid_GPU.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      31 mai 2010
*       \version    :      0.1
*
*******************************************************************************/

#include <GL/glew.h>
#include <GL/glut.h>


/** Taille de la grille **/
#define TAILLE_GRILLE 10

#include "Vecteur.h"
#include "BasicEntite.h"
#include "Shader.h" 
#include "Framebuffer.h"

class Fluid_GPU : public BasicEntite{

private:

    /** Identificateur du shader qui calcule l'advection */
    Shader *advection;
    
    /** Gestionnaire de buffer */
    Framebuffer *buffer;
    
    /** Taille de la grille ( initialisée à TAILLE_GRILLE ) */
    int taille_grille;

public:
    /** Constructeur */
    Fluid_GPU();
    
    /** Destructeur */
    ~Fluid_GPU();
    
    /** Initialisation de la résolution */
    void initialiserFluid();
    
    /** Résoudre le champs de vitesse */
    void resolutionFluid();
    
    /** Affiche le modèle */
    void Afficher();
    
    /** Affiche le modèle face à la camera*/
    void Afficher_Face_Camera(Vecteur3D& positionCamera, Vecteur3D& orientationCamera);
    
    /** Afficher les flammes*/
    void afficherFlamme();
    
    /** Afficher la fummée*/
    void afficherFumee();
        
    /** Dessine des plans dans la texture3D */
	void dessinerPlansDansTexture3D(GLuint id_texture, int nb_plans);
	                                
	/** Dessine des plans dans la texture3D face a la camera*/
	void dessinerPlansDansTexture3DFaceALaCamera(GLuint id_texture, int nb_plans,
	                     Vecteur3D& positionCamera, Vecteur3D& directionCamera);
	
};

#endif
