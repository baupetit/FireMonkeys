#include "Texture3D.h"






#include <iostream>
using namespace std;

Texture3D::Texture3D(){
    glGenTextures(1,&texture_id);
    cout << "Nouvelle texture3D, id : " << texture_id << endl;
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


void Texture3D::bindTexture(){

    glEnableClientState(GL_TEXTURE_3D);
    glEnable(GL_TEXTURE_3D);
    
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    
    glEnable(GL_TEXTURE_3D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    glActiveTexture(GL_TEXTURE0);                            
    glBindTexture(GL_TEXTURE_3D,texture_id);                
}

void Texture3D::bindTexture(GLuint texture_id){

    glEnableClientState(GL_TEXTURE_3D);
    glEnable(GL_TEXTURE_3D);
    
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    
    glEnable(GL_TEXTURE_3D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    glActiveTexture(GL_TEXTURE0);            
    glBindTexture(GL_TEXTURE_3D,texture_id);                
}

void Texture3D::unbindTexture(){
    glBindTexture(GL_TEXTURE_3D,0);
    glDisable(GL_TEXTURE_3D);
}
