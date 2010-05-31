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

#include <iostream>
using namespace std;

class Shader{

private:
    /** Id du program */
    Gluint program;
    
public:
    /** Constructeur de base */
    Shader(string fichier_vert_shader, string fichier_frag_shader);
    
    /** Destructeur */
    ~Shader();
    
    /** Chager le shader */
    void Bind_Program();
    
    /** Déchager le shader */
    void Unbind_Program();
    
};
#endif
