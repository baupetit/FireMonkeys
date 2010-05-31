#include "Framebuffer.h"

#include <GL/gl.h>





Framebuffer::Framebuffer(){
    
    // Creation du frame buffer 1
	glGenFramebuffers(1, &_buffer_id);
	
	// Texture associee  
	_texture_associee = new Texture3D();
	
}
    
Framebuffer::~Framebuffer(){
    delete _texture_associee;
}
 
 
    
void Framebuffer::bind_Buffer(){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _buffer_id);
}

void Framebuffer::attacher_layers_de_la_texture(int numero_layer, int nb_layer){
    GLuint idtexture = get_id_texture();
    
    /* RED BOOK p 535 :
    void glFramebufferTexture3D(GLenum target, GLenum attachment,
                GLenum texturetarget, GLuint texture, GLint level,
                GLint layer);
    */

    for (int i = 0; i < nb_layer; i++){
        // j'attache les nb_layers au framebuffer
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer + i);    
	}
    
}

void Framebuffer::detacher_texture(){
    // attacher à 0 == détacher les textures
	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, 0, 0 , 0);    
}
    
void Framebuffer::unbind_Buffer(){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
    
GLuint Framebuffer::get_id_texture(){
    return _texture_associee->get_texture_id();
}
    
void Framebuffer::initialiser_texture_buf(const float *tab,
                                          const int grille_width, 
                                          const int grille_height, 
                                          const int grille_depth){
    _grille_width  = grille_width;
    _grille_height = grille_height;
    _grille_depth  = grille_depth;
    
    _texture_associee->charger_matrice(tab, grille_width, grille_height, grille_depth);
}
