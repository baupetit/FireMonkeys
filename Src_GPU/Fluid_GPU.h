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
    
    /** Afficher les flammes*/
    void afficherFlamme();
    
    /** Afficher la fummée*/
    void afficherFumee();
    
    /** Afficher la fummée*/
    void afficherObjets();
    
    /** Afficher une texture 3D face à la camera*/
    void display_3DTexture(int details, int textureid);
};

#endif
