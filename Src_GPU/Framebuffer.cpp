#include "Framebuffer.h"

#include <GL/gl.h>


#include <iostream>
using namespace std;


Framebuffer::Framebuffer(){
    
    // Creation du frame buffer 1
	glGenFramebuffers(1, &_buffer_id);
	cout << "Nouveau Framebuffer, id : " << _buffer_id << endl;
	
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

    if(nb_layer==1){
        // j'attache les nb_layers au framebuffer
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer);    
	}
    if(nb_layer==2){
        // j'attache les nb_layers au framebuffer
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer);
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+1);            
	}
    if(nb_layer==3){
        // j'attache les nb_layers au framebuffer
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer);
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+1);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+2);    
	}
    if(nb_layer==4){
        // j'attache les nb_layers au framebuffer
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer);
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+1);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+2);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+3);    
	}
    if(nb_layer==5){
        // j'attache les nb_layers au framebuffer
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer);
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+1);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+2);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+3);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+4);    
	}
    if(nb_layer==6){
        // j'attache les nb_layers au framebuffer
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer);
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+1);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+2);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+3);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+4);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT5,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+5);    
	}
    if(nb_layer==7){
        // j'attache les nb_layers au framebuffer
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer);
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+1);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+2);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+3);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+4);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT5,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+5);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT6,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+6);    
	}
    if(nb_layer==8){
        // j'attache les nb_layers au framebuffer
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer);
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+1);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+2);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+3);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+4);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT5,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+5);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT6,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+6);    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT7,
							GL_TEXTURE_3D, idtexture, 0 , numero_layer+7);        
	}
	if(nb_layer > 8){
	    cout << " Erreur : Impossible d'associer autant de layers en même temps.\n";
	    cout << "          8 layers max. ";
	}
    
}

void Framebuffer::detacher_texture(){
    // attacher à 0 == détacher les textures
	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, 0, 0 , 0);    
	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
							GL_TEXTURE_3D, 0, 0 , 0);    
	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
							GL_TEXTURE_3D, 0, 0 , 0);    
	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,
							GL_TEXTURE_3D, 0, 0 , 0);    
	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4,
							GL_TEXTURE_3D, 0, 0 , 0);    
	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT5,
							GL_TEXTURE_3D, 0, 0 , 0);    
	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT6,
							GL_TEXTURE_3D, 0, 0 , 0);    
	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT7,
							GL_TEXTURE_3D, 0, 0 , 0);    
}
    
void Framebuffer::unbind_Buffer(){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
    
GLuint Framebuffer::get_id_texture() const{
    return _texture_associee->get_texture_id();
}

GLuint Framebuffer::get_id_buffer() const{
    return _buffer_id;
}
    
void Framebuffer::initialiser_texture_buf(const float *tab,
                                          const int grille_width, 
                                          const int grille_height, 
                                          const int grille_depth){
    _grille_width  = grille_width;
    _grille_height = grille_height;
    _grille_depth  = grille_depth;
    
    cout << "plou" << endl;   
    _texture_associee->charger_matrice(tab, grille_width, grille_height, grille_depth);
    cout << "plou" << endl;
}
