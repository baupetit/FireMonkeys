#ifndef FLUID_GPU_H
#define FLUID_GPU_H

#include <GL/glew.h>
#include <GL/glut.h>


/** Taille de la grille **/
#define TAILLE_GRILLE 10

#include "Vecteur.h"


class Fluid_GPU{
private:

    /** Identificateur du shader champs de vitesse */
    GLuint _speed_program;    
    float testtt;
    /** Texture : Champs de viteses */
    GLuint _speedField;
    /** Position de la variable ChampsDeVitesse dans le Shader */
    GLuint _speedFieldLocation;
    
    
public:
    Fluid_GPU();
    ~Fluid_GPU();
    
    /** Initialisation du champs de vitesse */
    void initialiserSpeedField();
    /** Résoudre le champs de vitesse */
    void resolutionSpeedField();
    /** Afficher le champs de vitesse */
    void displaySpeedField();
    /** Afficher une texture 3D */
    void display_3DTexture(int details, int textureid);
};

#endif
