#ifndef FLUID_GPU_H
#define FLUID_GPU_H

#include <GL/glew.h>
#include <GL/glut.h>


/** Taille de la grille **/
#define TAILLE_GRILLE 10

#include "Vecteur.h"
#include "Framebuffer.h"
#include "Shader.h" 
#include "BasicEntite.h"

class Fluid_GPU : public BasicEntite{

private:

    /** Identificateur du shader qui calcule l'advection */
    Shader *advection;
    
    /** Gestionnaire de buffer */
    Framebuffer *buffer;

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
    
    /** Afficher les flammes*/
    void afficherFlamme();
    
    /** Afficher la fummée*/
    void afficherFumee();
    
    /** Afficher la fummée*/
    void afficherObjets();
    
    /** Dessine des plans dans la texture3D */
	void dessinerPlansDansTexture3D(GLuint id_texture, int nb_plans);
	                                
	/** Dessine des plans dans la texture3D face a la camera*/
	void dessinerPlansDansTexture3DFaceALaCamera(GLuint id_texture, int nb_plans,
	                     Vecteur3D& positionCamera, Vecteur3D& directionCamera);
	
};

#endif
