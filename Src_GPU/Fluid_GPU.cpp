#include "Fluid_GPU.h"

#include <iostream>
using namespace std;


#define ID(i,j,k) i+j*TAILLE_GRILLE+k*TAILLE_GRILLE*TAILLE_GRILLE


Fluid_GPU::Fluid_GPU(){
    _speed_program = 0;
    initialiserSpeedField();
}

Fluid_GPU::~Fluid_GPU(){}

void Fluid_GPU::initialiserFluid(){

    //// FRAGMENT PROGRAMME : _SPEED_PROGRAM
    cout << "Fluid_GPU : Initialisation\n";

    // Chargement du programme
    cout << "Fluid_GPU : Chargement du shader \n";
    _speed_program = LoadProgram("./Shaders/Vitesse.vert","./Shaders/Vitesse.frag");
    cout << "Fluid_GPU : Shader chargé \n";
    
    // Creation du champs de vitesse vide
    Vecteur3D *texture = new Vecteur3D[TAILLE_GRILLE*TAILLE_GRILLE*TAILLE_GRILLE];
    for (int k = 0; k < TAILLE_GRILLE; k++){
        for (int j = 0; j < TAILLE_GRILLE; j++){
            for (int i = 0; i < TAILLE_GRILLE; i++){    
                texture[ID(i,j,k)].x = 0.0f;
                texture[ID(i,j,k)].y = 0.5f;
                texture[ID(i,j,k)].z = 0.0f;
            }
        }
    } 
    
    
    // Quelques valeurs initiales
    for (int k = 0; k < 1; k++){
        for (int j = 0; j < TAILLE_GRILLE; j++){
            for (int i = 0; i < TAILLE_GRILLE ; i++){    
                texture[ID(i,j,k)].x = 0.0f;
                texture[ID(i,j,k)].y = 1.0f;
                texture[ID(i,j,k)].z = 0.0f;
            }
        }
    } 
    
	/** Initialisation des 2 textures de calculs avec la matrice 3D initiale */
    // Creation de la texture 3D sur la carte pour speedfiled1
    glGenTextures(1,&_speedField_1);
    glBindTexture(GL_TEXTURE_3D, _speedField_1);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexImage3D(GL_TEXTURE_3D,0,GL_RGB,
                 TAILLE_GRILLE,TAILLE_GRILLE,TAILLE_GRILLE,
                 0, GL_RGB, GL_FLOAT, texture);
    glBindTexture(GL_TEXTURE_3D,0);

 	// Creation de la texture 3D sur la carte pour speedfiled2
    glGenTextures(1,&_speedField_2);
    glBindTexture(GL_TEXTURE_3D, _speedField_2);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexImage3D(GL_TEXTURE_3D,0,GL_RGB,
                 TAILLE_GRILLE,TAILLE_GRILLE,TAILLE_GRILLE,
                 0, GL_RGB, GL_FLOAT, texture);
    glBindTexture(GL_TEXTURE_3D,0);

	//FBO#1 pour la texture speedfield1
	glGenFramebuffers(1, &_FBO_speed_1);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _FBO_speed_1);
	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, _speedField_1, 0 , 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	//FBO#2 pour la texture speedfield1
	glGenFramebuffers(1, &_FBO_speed_2);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _FBO_speed_2);
	glFramebufferTexture3D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_3D, _speedField_2, 0 , 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    

	/* Create a depth buffer for our framebuffer */
	glGenRenderbuffers( 1, &_renderbuffer_1 );
	glBindRenderbuffer( GL_RENDERBUFFER, _renderbuffer_1  );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
 						   TAILLE_GRILLE, TAILLE_GRILLE );


	/* Create a depth buffer for our framebuffer */
	glGenRenderbuffers( 1, &_renderbuffer_2 );
	glBindRenderbuffer( GL_RENDERBUFFER, _renderbuffer_2  );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
						   TAILLE_GRILLE, TAILLE_GRILLE );


	/* Attach the texture and depth buffer to the framebuffer */

	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, _FBO_speed_1 );
	glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER,
	   GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderbuffer_1 );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, _FBO_speed_2 );
	glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER,
	   GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderbuffer_2 );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

	glEnable( GL_DEPTH_TEST );


    // On lie les variables uniforme du shader
//  _speedFieldLocation_Prec = glGetUniformLocation ( _speed_program, "TextureSpeedField_Prec");
//    glUniform1i(_speedFieldLocation_Prec,_speedField_1);
//    _speedFieldLocation_Cour = glGetUniformLocation ( _speed_program, "TextureSpeedField_Cour");
//    glUniform1i(_speedFieldLocation_Cour,_speedField_2);
    
    
}


void Fluid_GPU::resolutionSpeedField(){
	//on lie la texture courante avec la variable uniforme (texturespeedfield) du shader
	_speedFieldLocation_Prec = glGetUniformLocation ( _speed_program, "TextureSpeedField_Prec");
    glUniform1i(_speedFieldLocation_Prec,_speedField_1);

	//on se place dans le FBO 2 on va donc dessiner dans la texture2
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _FBO_speed_2);
	//glClearColor( 1.0,0.0,0.0,0.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//rendu du calcul	
	glUseProgram(_speed_program);
	//la texture est dessiné dans le FBO 2
    displaySpeedField(); 
    glUseProgram(0);
	// generation texture
	glGenerateMipmap( GL_TEXTURE_CUBE_MAP );
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	int details = 10;
	glEnable(GL_TEXTURE_3D);
	glActiveTexture(_speedField_2);
    glBindTexture(GL_TEXTURE_3D,_speedField_2);
    

    GLfloat verts[4][3] = { { 0.0, 0.0, 0.5}, {0.0, 1.0, 0.5}, {1.0, 1.0, 0.5}, {1.0, 0.0, 0.5} };

	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2]);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]);
		
		glTexCoord3d(verts[1][0], verts[1][1], verts[1][2]);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2]);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2]);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2]);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
		
		glTexCoord3d(verts[3][0], verts[3][1], verts[3][2]);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2]);
		
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2]);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]);		
	glEnd();
	
	for (int i = 1; i < details/2; i++){
	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/details);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]+ (float)i/details);
		
		glTexCoord3d(verts[1][0], verts[1][1], verts[1][2] + (float)i/details);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2] + (float)i/details);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/details);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/details);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/details);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/details);
		
		glTexCoord3d(verts[3][0], verts[3][1], verts[3][2] + (float)i/details);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2] + (float)i/details);
		
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/details);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/details);	
	glEnd();
	}
	
	
	for (int i = 1; i < details/2; i++){
	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] - (float)i/details);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]- (float)i/details);
		
		glTexCoord3d(verts[1][0], verts[1][1], verts[1][2] - (float)i/details);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2] - (float)i/details);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2] - (float)i/details);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] - (float)i/details);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2] - (float)i/details);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] - (float)i/details);
		
		glTexCoord3d(verts[3][0], verts[3][1], verts[3][2] - (float)i/details);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2] - (float)i/details);
		
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] - (float)i/details);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2] - (float)i/details);	
	glEnd();
	}

	glDisable(GL_TEXTURE_3D);

    

	


}


void Fluid_GPU::displaySpeedField(){

    display_3DTexture(20, _speedField_2);
    
}

void Fluid_GPU::display_3DTexture(int details, int textureid){


    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );
    glAlphaFunc(GL_GREATER,0.0f);
    glEnable(GL_ALPHA_TEST);
    
	glEnable(GL_TEXTURE_3D);
    glActiveTexture(textureid);
    glBindTexture(GL_TEXTURE_3D,textureid);

    GLfloat verts[4][3] = { { 0.0, 0.0, 0.5}, {0.0, 1.0, 0.5}, {1.0, 1.0, 0.5}, {1.0, 0.0, 0.5} };

	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2]);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]);
		
		glTexCoord3d(verts[1][0], verts[1][1], verts[1][2]);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2]);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2]);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2]);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
		
		glTexCoord3d(verts[3][0], verts[3][1], verts[3][2]);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2]);
		
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2]);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]);		
	glEnd();
	
	for (int i = 1; i < details/2; i++){
	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/details);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]+ (float)i/details);
		
		glTexCoord3d(verts[1][0], verts[1][1], verts[1][2] + (float)i/details);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2] + (float)i/details);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/details);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/details);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/details);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/details);
		
		glTexCoord3d(verts[3][0], verts[3][1], verts[3][2] + (float)i/details);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2] + (float)i/details);
		
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/details);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/details);	
	glEnd();
	}
	
	
	for (int i = 1; i < details/2; i++){
	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] - (float)i/details);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]- (float)i/details);
		
		glTexCoord3d(verts[1][0], verts[1][1], verts[1][2] - (float)i/details);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2] - (float)i/details);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2] - (float)i/details);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] - (float)i/details);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2] - (float)i/details);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] - (float)i/details);
		
		glTexCoord3d(verts[3][0], verts[3][1], verts[3][2] - (float)i/details);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2] - (float)i/details);
		
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] - (float)i/details);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2] - (float)i/details);	
	glEnd();
	}

	glDisable(GL_TEXTURE_3D);
    glDisable( GL_BLEND );
    glBindTexture(GL_TEXTURE_3D,0);

}
