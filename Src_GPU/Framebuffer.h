#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/*!*****************************************************************************
*
*       \file       :      Framebuffer.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      31 mai 2010
*       \version    :      0.1
*
*******************************************************************************/


#include <GL/glew.h>
#include <GL/glut.h>

#include "Texture3D.h"    
    
class Framebuffer{
private:
    /** Identifiant du buffer */
    GLuint buffer_id;
    
    /** Texture associée au buffer*/
    Texture3D *texture_associee;
        
    /** Taille de la grille */
    int taille_grille;
    
    
public:
    /** Constructeur par défaut */
    Framebuffer();
    
    /** Destructeur */
    ~Framebuffer();
    
    /** Charge le buffer courant */
    void Bind_Buffer();
    
    /** Decharge le buffer courant */
    void Unbind_Buffer();
    
    /** Retourne la texture associee*/
    GLuint Get_texture(){
        return texture_associee;
    
    /** Initialise la texture id avec le tableau donné */
    void Initialiser_texture_buf(const Texture3D& texture, const float *tab, const int taille);
    
};
#endif
