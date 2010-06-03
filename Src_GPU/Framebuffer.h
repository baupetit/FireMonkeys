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



#include "Shader.h"
#include "Texture3D.h"

    
class Framebuffer{
private:
    /** Identifiant du buffer */
    GLuint _buffer_id;
        
    /** Largeur de la grille */
    int _grille_width;
    
    /** Hauteur de la grille */
    int _grille_height;
    
    /** Profondeur de la grille */
    int _grille_depth;
    
    
    
    
    
    
    
public:
    /** Constructeur par défaut */
    Framebuffer(int jambi, int jambo, int jamboo);
    
    /** Destructeur */
    ~Framebuffer();
                
    /** Retourne l'id du buffer*/
    GLuint get_id_buffer() const;
    
    void traiterDessinDansBuffer(Shader& shader,
                                 GLuint source_id,
                                 GLuint cible_id);
    
private:    

    /** Prepare le dessin dans les GL_COLOR_ATTACHMENTi associés */
    void prepareDrawIntoBufferAttachment(int nb_attachements);
    
    /** Charge le buffer courant */
    void bind_Buffer();
    
    /** Decharge le buffer courant */
    void unbind_Buffer();
    
    void dessinerCarre(int id_texture, float decalage);
    
    
    /** Attache au buffer les layers de la texture associée */
    void attacher_layers_de_la_texture(GLuint texture_id, int numero_layer, int nb_layer);
    
    /** Detache du buffer la texture associée */
    void detacher_texture();
    
};
#endif
