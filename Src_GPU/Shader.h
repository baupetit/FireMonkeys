#ifndef SHADER_H
#define SHADER_H

/*!*****************************************************************************
*
*       \file       :      Shader.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      31 mai 2010
*       \version    :      0.1
*
*******************************************************************************/

#include <GL/glew.h>
#include <GL/glut.h>

#include "Vecteur.h"


#include <iostream>
using namespace std;

class Shader{

private:
    /** Id du program */
    GLuint program;
    
public:
    /** Constructeur de base */
    Shader(string fichier_vert_shader, string fichier_frag_shader);
    
    /** Destructeur */
    ~Shader();
    
    /** Chager le shader */
    void Bind_Program();
    
    /** Déchager le shader */
    void Unbind_Program();
        
    /** Lier texture */
    void lierTexture(string nom_texture, GLuint texture_id, GLuint level);
    
    /** Lier vecteur */
    void lierVecteur(string nom_vecteur, Vecteur3D& vect);
    
    /** Lier flottant */
    void lierFloat(string nom_vecteur, float flouttounet);
    
    /** Renvoie d'id du program */
    GLuint getProgramId() const{
        return program;
    }    
};
#endif
