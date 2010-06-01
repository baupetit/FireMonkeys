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
    GLuint _buffer_id;
    
    /** Texture associée au buffer*/
    Texture3D *_texture_associee;
        
    /** Largeur de la grille */
    int _grille_width;
    
    /** Hauteur de la grille */
    int _grille_height;
    
    /** Profondeur de la grille */
    int _grille_depth;
    
    
public:
    /** Constructeur par défaut */
    Framebuffer();
    
    /** Destructeur */
    ~Framebuffer();
    
    /** Charge le buffer courant */
    void bind_Buffer();
    
    /** Attache au buffer les layers de la texture associée */
    void attacher_layers_de_la_texture(int numero_layer, int nb_layer);
    
    /** Detache du buffer la texture associée */
    void detacher_texture();
    
    /** Decharge le buffer courant */
    void unbind_Buffer();
    
    /** Retourne la texture associee*/
    GLuint get_id_texture() const;
    
    /** Retourne l'id du buffer*/
    GLuint get_id_buffer() const;
    
    /** Initialise la texture associee avec le tableau donné */
    void initialiser_texture_buf(const float *tab, 
                                 const int grille_width, 
                                 const int grille_height, 
                                 const int grille_depth);                                 
    
};
#endif
