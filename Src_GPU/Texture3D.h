#ifndef TEXTURE3D_H
#define TEXTURE3D_H

/*!*****************************************************************************
*
*       \file       :      Texture3D.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      31 mai 2010
*       \version    :      0.1
*
*******************************************************************************/


#include <GL/glew.h>
#include <GL/glut.h>


class Texture3D{

private:
    /** Id de la texture */
    GLuint texture_id;
    
    /** Params */
    int _width;
    int _height;
    int _depth;
    
    
public:
    /** Constructeur par défaut*/
    Texture3D();
    
    /** Destructeur */
    ~Texture3D();
    
    /** 
        Charge la matrice dans la texture 
        @param la matrice
        @param width
        @param height
        @param depth
    */
    void charger_matrice(const float *matrice, 
                         const int width, 
                         const int height, 
                         const int depth);
    
    /** Renvoie l'id de la texture */
    GLuint get_texture_id() const;
    
    /** Bind la texture avec les bons paramètres*/
    void bindTexture(GLuint TEXTUREPOSITION = GL_TEXTURE0);
    
    /** Unbind la texture*/
    static void unbindTexture();
    
    
    static void bindTexture(GLuint texture_id, GLuint TEXTUREPOSITION);
    
    static void setFilter(GLuint PARAM);
};

#endif
