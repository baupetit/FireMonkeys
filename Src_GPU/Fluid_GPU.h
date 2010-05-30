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
    /** Texture : Champs de viteses 
		Besoin de 2 textures pour le ping-ponging avec les calculs
    */
    GLuint _speedField_1;
	GLuint _speedField_2;
    /** Position des variables ChampsDeVitesse prec et cour dans le Shader */
    GLuint _speedFieldLocation_Prec;
    GLuint _speedFieldLocation_Cour;
	/** FBOs pour les textures que l'on utilise pour le calcul de champ de vitesse */
	GLuint _FBO_speed_1;
	GLuint _FBO_speed_2;
    

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
