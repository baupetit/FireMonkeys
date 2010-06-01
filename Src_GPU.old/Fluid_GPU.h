#ifndef FLUID_GPU_H
#define FLUID_GPU_H

#include <GL/glew.h>
#include <GL/glut.h>


/** Taille de la grille **/
#define TAILLE_GRILLE 4

#include "Vecteur.h"


class Fluid_GPU{
private:
    GLuint _speed_program;
    
    /** Texture : Champs de viteses */
    GLuint _speedField;
    
    
public:
    Fluid_GPU();
    ~Fluid_GPU();
    void initialiserSpeedField();
    void resolutionSpeedField();
    void displaySpeedField();
    void display_3DTexture(int details, int textureid);
};

#endif
