#include "Fluid_GPU.h"
#include "loadprogram.h"

#include <iostream>
using namespace std;

Fluid_GPU::Fluid_GPU(){
    _speed_program = 0;
}

Fluid_GPU::~Fluid_GPU(){}

void Fluid_GPU::initialiserSpeedField(){

    //// FRAGMENT PROGRAMME : _SPEED_PROGRAM
    cout << "Fluid_GPU : Initialisation : Speed Field \n";

    // Chargement du programme
    cout << "Fluid_GPU : Chargement du shader \n";
    _speed_program = LoadProgram("./Shaders/Vitesse.vert","./Shaders/Vitesse.frag");
    cout << "Fluid_GPU : Shader chargé \n";
    
    // Creation du champs de vitesse vide
    Vecteur3D *texture = new Vecteur3D[TAILLE_GRILLE*TAILLE_GRILLE*TAILLE_GRILLE];
    for (int i = 0; i < (TAILLE_GRILLE*TAILLE_GRILLE*TAILLE_GRILLE); i++){
        texture[i].x = 0.0f;
        texture[i].y = 0.0f;
        texture[i].z = 0.0f;
    } 
    
    // Quelques valeurs initiales
    for (int i = 0; i < (TAILLE_GRILLE); i++){
        texture[i].x = 1.0f;
        texture[i].y = 0.0f;
        texture[i].z = 0.0f;
    } 
    
    // Creation de la texture 3D sur la carte
    glEnable(GL_TEXTURE3D);
    glGenTextures(1,&_speedField);
    glBindTexture(GL_TEXTURE_3D, _speedField);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexImage3D(GL_TEXTURE_3D,0,GL_RGB,
                 TAILLE_GRILLE,TAILLE_GRILLE,TAILLE_GRILLE,
                 0, GL_RGB, GL_FLOAT, texture);
    
    // Liaison de la texture avec le shader
    int position = glGetUniformLocationARB(_speed_program, "SpeedField");
    glUseProgramObjectARB(_speed_program);
    glUniform1iARB(position, _speedField);
    glUseProgramObjectARB(0);
}


void Fluid_GPU::resolutionSpeedField(){
    // cout << "Fluid_GPU : Resolution : Speed Field \n";
    glEnable(GL_TEXTURE_3D);
    glActiveTexture(_speedField);
    glBindTexture(GL_TEXTURE_3D, _speedField);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glUseProgram(_speed_program);
    int position = glGetUniformLocationARB(_speed_program, "SpeedField");
    glUniform1iARB(position, _speedField);
    GLfloat verts[4][3] = { { 0.0, 0.0, 0.5}, {0.0, 1.0, 0.5}, {1.0, 1.0, 0.5}, {1.0, 0.0, 0.5} };
	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2]);    
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]);
		
		glTexCoord3d(verts[1][2], verts[1][1], verts[1][2]);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2]);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2]);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2]);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
		
		glTexCoord3d(verts[3][2], verts[3][1], verts[3][2]);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2]);
		
		glTexCoord3d(verts[0][2], verts[0][1], verts[0][2]);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]);		
	glEnd();
    glUseProgram(0);
    //glCopyTexSubImage3D (_speedField,0,0,0,0,0,0,TAILLE_GRILLE,TAILLE_GRILLE);
}


void Fluid_GPU::displaySpeedField(){

    display_3DTexture(6, _speedField);
    
}

void Fluid_GPU::display_3DTexture(int details, int textureid){


    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );
    glAlphaFunc(GL_GREATER,0.1f);
    glEnable(GL_ALPHA_TEST);
    
	glEnable(GL_TEXTURE_3D);
    glActiveTexture(textureid);

    GLfloat verts[4][3] = { { 0.0, 0.0, 0.5}, {0.0, 1.0, 0.5}, {1.0, 1.0, 0.5}, {1.0, 0.0, 0.5} };

	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2]);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]);
		
		glTexCoord3d(verts[1][2], verts[1][1], verts[1][2]);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2]);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2]);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2]);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
		
		glTexCoord3d(verts[3][2], verts[3][1], verts[3][2]);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2]);
		
		glTexCoord3d(verts[0][2], verts[0][1], verts[0][2]);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]);		
	glEnd();
	
	for (int i = 1; i <= details/2; i++){
	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/details);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]+ (float)i/details);
		
		glTexCoord3d(verts[1][2], verts[1][1], verts[1][2] + (float)i/details);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2] + (float)i/details);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2] + (float)i/details);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/details);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2] + (float)i/details);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/details);
		
		glTexCoord3d(verts[3][2], verts[3][1], verts[3][2] + (float)i/details);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2] + (float)i/details);
		
		glTexCoord3d(verts[0][2], verts[0][1], verts[0][2] + (float)i/details);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/details);	
	glEnd();
	}
	
	
	for (int i = 1; i <= details/2; i++){
	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] - (float)i/details);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]- (float)i/details);
		
		glTexCoord3d(verts[1][2], verts[1][1], verts[1][2] - (float)i/details);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2] - (float)i/details);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2] - (float)i/details);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] - (float)i/details);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2] - (float)i/details);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] - (float)i/details);
		
		glTexCoord3d(verts[3][2], verts[3][1], verts[3][2] - (float)i/details);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2] - (float)i/details);
		
		glTexCoord3d(verts[0][2], verts[0][1], verts[0][2] - (float)i/details);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2] - (float)i/details);	
	glEnd();
	}

	glDisable(GL_TEXTURE_3D);
    glDisable( GL_BLEND );

}
