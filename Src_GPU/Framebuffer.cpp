#include "Framebuffer.h"





Framebuffer::Framebuffer(){
    // Creation de la texture 1
    glGenTextures(1,&texture_buf1);
    glBindTexture(GL_TEXTURE_3D, texture_buf1);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexImage3D(GL_TEXTURE_3D,0,GL_RGB,
                 TAILLE_GRILLE,TAILLE_GRILLE,TAILLE_GRILLE,
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
                 TAILLE_GRILLE,TAILLE_GRILLE,TAILLE_GRILLE,
                 0, GL_RGB, GL_FLOAT, NULL);
                                  
    // Unbind
    glBindTexture(GL_TEXTURE_3D,0);
    
    // Initialisation du pingpong
    pingpong = PING;

}
    
Framebuffer::~Framebuffer(){
}
    
void Framebuffer::Swap_Active_Buffer(){
}

void Framebuffer::Bind_Buffer_Courant(){
}
    
void Framebuffer::Unbind_Buffer(){
}
    
GLuint Framebuffer::Get_texture_courante(){
}
    
GLuint Framebuffer::Get_buffer_courant(){
}
    
void Framebuffer::Initialiser_texture_buf(const int id_texture, 
                                          const float *tab, 
                                          const int taille){
}
