#include "Texture3D.h"






#include <iostream>
using namespace std;

Texture3D::Texture3D(){
    glGenTextures(1,&texture_id);
    cout << "Nouvelle texture3D, id : " << texture_id << endl;
     
    glEnableClientState(GL_TEXTURE_3D);
    glEnable(GL_TEXTURE_3D);

    glBindTexture(GL_TEXTURE_3D,texture_id);                
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
 
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
 
}

void Texture3D::charger_matrice(const float *matrice,
                                const int width, 
                                const int height, 
                                const int depth){

    
    // params
    _width  =  width; 
    _height =  height;
    _depth  =  depth;

    // bind
    bindTexture();
    
    // load
    glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,width,height,depth,
                 0, GL_RGBA, GL_FLOAT, matrice);
    
    
}


Texture3D::~Texture3D(){
}


GLuint Texture3D::get_texture_id() const {
    return texture_id;
}


void Texture3D::bindTexture(GLuint POSITION){
    glEnable(GL_TEXTURE_3D);
    glActiveTexture(POSITION);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_3D,texture_id); 
}

void Texture3D::bindTexture(GLuint texture_id, GLuint POSITION){
    glEnable(GL_TEXTURE_3D);
    glActiveTexture(POSITION);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_3D,texture_id);                    
}

void Texture3D::unbindTexture(){
    glBindTexture(GL_TEXTURE_3D,0);
}



void Texture3D::setFilter(GLuint PARAM){
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, PARAM);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, PARAM);  
}
