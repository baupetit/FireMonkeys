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
    
    glBindTexture(GL_TEXTURE_3D, texture_id);
    
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    
    glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,width,height,depth,
                 0, GL_RGBA, GL_FLOAT, matrice);
    
    
}


Texture3D::~Texture3D(){
}
