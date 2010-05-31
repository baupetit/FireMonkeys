#include "Framebuffer.h"

#include <GL/gl.h>





Framebuffer::Framebuffer(int _taille_grille, Texture3D& texture){
    // Creation de la texture 1
    glGenTextures(1,&texture_buf1);
    glBindTexture(GL_TEXTURE_3D, texture_buf1);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexImage3D(GL_TEXTURE_3D,0,GL_RGB,
                 taille_grille,taille_grille,taille_grille,
                 0, GL_RGB, GL_FLOAT, NULL);
                
    // Creation de la texture 2
    glGenTextures(1,&texture_buf2);
    glBindTexture(GL_TEXTURE_3D, texture_buf2);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexImage3D(GL_TEXTURE_3D,0,GL_RGB,
                 taille_grille,taille_grille,taille_grille,
                 0, GL_RGB, GL_FLOAT, NULL);
                                  
    // Creation du frame buffer 1
	glGenFramebuffers(1, &_FBO_speed_1);                       
                                  
    // Unbind
    glBindTexture(GL_TEXTURE_3D,0);
    
    // Initialisation du pingpong
    pingpong = PING;
    
    // Taille de la grille
    taille_grille = _taille_grille;
}
    
Framebuffer::~Framebuffer(){
}
    
void Framebuffer::Bind_Buffer(){

	//FBO#1 pour la texture speedfield1
	glGenFramebuffers(1, &_FBO_speed_1);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _FBO_speed_1);
	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, _speedField_1, 0 , 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    
}
    
void Framebuffer::Unbind_Buffer(){
}
    
GLuint Framebuffer::Get_texture(){
}
    
void Framebuffer::Initialiser_texture_buf(const Texture3D& texture, 
                                          const float *tab, 
                                          const int taille){
}
