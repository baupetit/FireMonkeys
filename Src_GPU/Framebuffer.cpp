#include "Framebuffer.h"

#include <GL/gl.h>


#include <iostream>
using namespace std;


Framebuffer::Framebuffer(int wwidth, int hheight, int ddepth){
	glGenFramebuffersEXT(1, &_buffer_id);
	cout << "Nouveau Framebuffer, id : " << _buffer_id << endl;
	_grille_width  = wwidth;
	_grille_height = hheight;
	_grille_depth  = ddepth;
}
    
Framebuffer::~Framebuffer(){
}
 
    
void Framebuffer::bind_Buffer(){
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _buffer_id);
}


void Framebuffer::attacher_layers_de_la_texture(GLuint texture_id, int numero_layer, int nb_layer){
 
    for(int i = 0; i < nb_layer; i ++){
    	
        glFramebufferTexture3DEXT( GL_FRAMEBUFFER_EXT, 
                               GL_COLOR_ATTACHMENT0_EXT + i,
                               GL_TEXTURE_3D, 
                               texture_id, 
                               0, 
                               numero_layer+i );
	}
    
}

void Framebuffer::detacher_texture(){
    GLint maxbuffers;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxbuffers);

    for(int i = 0; i < (int)maxbuffers; i ++){
    
    	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, 
    	                       GL_COLOR_ATTACHMENT0_EXT + i, 
    	                       GL_TEXTURE_3D, 
    	                       0, 
    	                       0 , 
    	                       0);    
    	                       
    
	}
}
    
void Framebuffer::unbind_Buffer(){
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
}
 

GLuint Framebuffer::get_id_buffer() const{
    return _buffer_id;
}
 
 
 
void Framebuffer::prepareDrawIntoBufferAttachment(int nb_attachements){

    
    GLenum mrt[] = { GL_COLOR_ATTACHMENT0_EXT,
                     GL_COLOR_ATTACHMENT1_EXT,
                     GL_COLOR_ATTACHMENT2_EXT,
                     GL_COLOR_ATTACHMENT3_EXT,
                     GL_COLOR_ATTACHMENT4_EXT,
                     GL_COLOR_ATTACHMENT5_EXT,
                     GL_COLOR_ATTACHMENT6_EXT,
                     GL_COLOR_ATTACHMENT7_EXT,
                     GL_COLOR_ATTACHMENT8_EXT,
                     GL_COLOR_ATTACHMENT9_EXT,
                     GL_COLOR_ATTACHMENT10_EXT,
                     GL_COLOR_ATTACHMENT12_EXT,
                     GL_COLOR_ATTACHMENT13_EXT,
                     GL_COLOR_ATTACHMENT14_EXT,
                     GL_COLOR_ATTACHMENT15_EXT };
                     
    glDrawBuffers(nb_attachements, mrt);    
}




   
void Framebuffer::traiterDessinDansBuffer(GLuint source_id,
                                          GLuint cible_id){
  
    // attribs
    glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT);

    
    // depth
    glDisable(GL_DEPTH_TEST);
    
    // bind !
    bind_Buffer();
        
    // viewport    
    glViewport(0,0,_grille_width,_grille_height);
	glClear(GL_COLOR_BUFFER_BIT);

    // traite le calcul    
    GLint maxbuffers;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxbuffers);
        
    
    int pos = 0;
    
        
    prepareDrawIntoBufferAttachment((int)maxbuffers);
        
    while ( _grille_depth - pos > (int)maxbuffers ){
        
        attacher_layers_de_la_texture(cible_id, pos, (int)maxbuffers);
            
        // dessin    
        dessinerCarre(source_id, pos/float(_grille_depth));
        
        pos += 8;
       } 
       
    
    detacher_texture();
    attacher_layers_de_la_texture(cible_id, pos, _grille_depth - pos);
    prepareDrawIntoBufferAttachment(_grille_depth - pos);
    
    dessinerCarre(source_id, pos/float(_grille_depth));
         
    // unbind !   
    unbind_Buffer();
    
    // etat depth
    glEnable(GL_DEPTH_TEST);
    
    // pop
    glPopAttrib();
}



void Framebuffer::dessinerCarre(int id_texture, float decalage){
 
    // pos du dessin
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,1,0,1,0,1);
    
    // texture
    Texture3D::bindTexture(id_texture);
    
               
    // affichage            
    glBegin(GL_QUADS);        
    
        glTexCoord3f(0.0,0.0,0.0 + decalage);
        glVertex3f(0.0,0.0,0.0);
        
        glTexCoord3f(0.0,1.0,0.0 + decalage);
        glVertex3f(0.0,1.0,0.0 );
        
        glTexCoord3f(1.0,1.0,0.0 + decalage);
        glVertex3f(1.0,1.0,0.0);
        
        glTexCoord3f(1.0,0.0,0.0 + decalage);
        glVertex3f(1.0,0.0,0.0);
        
    glEnd();    
    
    // pos
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
   
}
