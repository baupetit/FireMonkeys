#include "Fluid_GPU.h"

#include <iostream>
using namespace std;



Fluid_GPU::Fluid_GPU(){
	// Taille de la grille
	/*
	_grille_width  = TAILLE_GRILLE;
	_grille_height = TAILLE_GRILLE;
	_grille_depth  = TAILLE_GRILLE;
	*/

	_grille_width  = 60;
	_grille_height = 60;
	_grille_depth  = 60;
   
	s = NULL;
    shader_affichage = NULL;
    
    echelle.x = 1.0;
    echelle.y = 1.0;
    echelle.z = 1.0;
    
    

}

Fluid_GPU::~Fluid_GPU(){
}

void Fluid_GPU::initialiserFluid(){
	cout << "Nouveau FluideGPU" << endl;
    
	// Solver
	delete s;
	s = new Solver_GPU(_grille_width, _grille_height, _grille_depth);
	
	// Shader
	delete shader_affichage;
	shader_affichage = new Shader("./Shaders/vertex_shader_qui_ne_fait_rien.vert",
	                                 "./Shaders/affichage.frag");
	                                 
}

void Fluid_GPU::resolutionFluid(){
	s->densitiesStepWithTemp(0.012);
	s->velocitiesStepWithTemp(0.012);
}

void Fluid_GPU::Afficher(){
	// MAJ
	resolutionFluid();
    
    

	// Affichage
	afficherFlamme();
}

void Fluid_GPU::Afficher_Face_Camera(Vecteur3D& positionCamera, Vecteur3D& orientationCamera){
}

void Fluid_GPU::afficherFlamme(){
	// Feu
	dessinerPlansDansTexture3D(s->getDensities(),50);
}

void Fluid_GPU::afficherFumee(){
}

void Fluid_GPU::dessinerPlansDansTexture3DFaceALaCamera(GLuint id_texture, 
                                                        int nb_plans,
							Vecteur3D& positionCamera, 
							Vecteur3D& directionCamera){
	       
	Vecteur3D directionInitiale = Vecteur3D (0,0,1);
    
	Vecteur3D N = positionCamera - position;
	N.normaliser();

	// Calcul de l'angle Oxz
	float angleOxz = atan2(N.z, N.x) - atan2(directionInitiale.z, directionInitiale.x);
	while ( angleOxz >= M_PI ) angleOxz -= 2*M_PI;
	while ( angleOxz <= -M_PI ) angleOxz += 2*M_PI;

	// Calcul de l'angle Oyz
	Vecteur3D distance = - positionCamera + position;
	float angleOyz = asin(distance.y / norme(distance));
	while ( angleOyz >= M_PI ) angleOyz -= 2*M_PI;
	while ( angleOyz <= -M_PI ) angleOyz += 2*M_PI;

	// Vecteur directeurs    
	Vecteur3D Boitev0 = Vecteur3D(-0.5, -0.5, -0.5);
	Vecteur3D Boitev1 = Vecteur3D(-0.5, +0.5, -0.5);
	Vecteur3D Boitev2 = Vecteur3D(+0.5, +0.5, -0.5);
	Vecteur3D Boitev3 = Vecteur3D(+0.5, -0.5, -0.5);
	// Rotation autour de X pour garder le cube face à la camera 
	Boitev0.rotationAutourAxeX(angleOyz);
	Boitev1.rotationAutourAxeX(angleOyz);
	Boitev2.rotationAutourAxeX(angleOyz);
	Boitev3.rotationAutourAxeX(angleOyz);
	// Rotation autour de Y pour garder le cube face à la camera     
	Boitev0.rotationAutourAxeY(angleOxz);
	Boitev1.rotationAutourAxeY(angleOxz);
	Boitev2.rotationAutourAxeY(angleOxz);
	Boitev3.rotationAutourAxeY(angleOxz);
    
    
	// Coordonnées de textures      
	Vecteur3D Tex0 = Vecteur3D(-0.0, -0.0, -0.0);
	Vecteur3D Tex1 = Vecteur3D(-0.0, +1.0, -0.0);
	Vecteur3D Tex2 = Vecteur3D(+1.0, +1.0, -0.0);
	Vecteur3D Tex3 = Vecteur3D(+1.0, -0.0, -0.0);       
	// Deplacement
	Vecteur3D centre = Vecteur3D(0.5,0.5,0.5);
	Tex0 -= centre; 
	Tex1 -= centre; 
	Tex2 -= centre; 
	Tex3 -= centre; 
	// Rotation 1
	Tex0.rotationAutourAxeX(angleOyz);
	Tex1.rotationAutourAxeX(angleOyz);
	Tex2.rotationAutourAxeX(angleOyz);
	Tex3.rotationAutourAxeX(angleOyz);
	// Rotation 2
	Tex0.rotationAutourAxeY(angleOxz);
	Tex1.rotationAutourAxeY(angleOxz);
	Tex2.rotationAutourAxeY(angleOxz);
	Tex3.rotationAutourAxeY(angleOxz);
	// Deplacement
	Tex0 += centre; 
	Tex1 += centre; 
	Tex2 += centre; 
	Tex3 += centre; 
    
    
	// Direction de création des plans
	Vecteur3D profondeur = produitVectoriel(Boitev2-Boitev1 , Boitev0-Boitev1);
	Boitev0.normaliser();
	Boitev1.normaliser();
	Boitev2.normaliser();
	Boitev3.normaliser();
	profondeur.normaliser();
    
	// Activation de la texture
	glDisable(GL_LIGHTING);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );
	glAlphaFunc(GL_GREATER,0.0f);
	glEnable(GL_ALPHA_TEST);    
	glEnable(GL_TEXTURE_3D);
	glActiveTexture(id_texture);
    
	// Vecteur de coordonnées
	Vecteur3D coord;
	Vecteur3D decalageTex;
	Vecteur3D decalageVert;
	N = profondeur;
	
	// Affichage
	glBegin(GL_QUADS);
	for (int i = 0; i < nb_plans; i++){
		decalageTex  = - float(i)/nb_plans * N;//Vecteur3D(0,0,+ float(i)/nb_plans);// * N;
		decalageVert = - float(i)/nb_plans * N;
        
        
		coord = (Tex0 + decalageTex);        glTexCoord3d(coord.x, coord.y, coord.z);                
		coord = Boitev0 + decalageVert;      glVertex3d(coord.x, coord.y, coord.z);
        
		coord = (Tex1 + decalageTex);        glTexCoord3d(coord.x, coord.y, coord.z);        
		coord = Boitev1 + decalageVert;      glVertex3d(coord.x, coord.y, coord.z);
        
		coord = (Tex2 + decalageTex);        glTexCoord3d(coord.x, coord.y, coord.z);
		coord = Boitev2 + decalageVert;      glVertex3d(coord.x, coord.y, coord.z);
                
		coord = (Tex3 + decalageTex);        glTexCoord3d(coord.x, coord.y, coord.z);
		coord = Boitev3 + decalageVert;      glVertex3d(coord.x, coord.y, coord.z);
	}
	glEnd();
    
	// Desactivation des elements
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_ALPHA_TEST);    
	glDisable( GL_BLEND );	
}

void Fluid_GPU::dessinerPlansDansTexture3D(GLuint id_texture, int nb_plans){
    
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
 
 
	glDisable(GL_LIGHTING);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );
	glAlphaFunc(GL_GREATER,0.0f);
	glEnable(GL_ALPHA_TEST);
	
	GLfloat verts[4][3] = { { 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 0.0, 0.0}};

    GLuint id0 = s->getDensities();
    //GLuint id0 = s->getSpeed();
    //GLuint id1 = s->getDestDensities();
        
    Texture3D::bindTexture(id0,0);
    Texture3D::setFilter(GL_LINEAR);

    
    shader_affichage -> Bind_Program();
    shader_affichage->lierLevel("texture_entree", 0);



	glBegin(GL_QUADS);	
	
	for (int i = 0; i < nb_plans; i++){
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/nb_plans);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]+ (float)i/nb_plans);
		
		glTexCoord3d(verts[1][0], verts[1][1], verts[1][2] + (float)i/nb_plans);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2] + (float)i/nb_plans);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/nb_plans);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/nb_plans);
		
		glTexCoord3d(verts[3][0], verts[3][1], verts[3][2] + (float)i/nb_plans);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2] + (float)i/nb_plans);
		
	}
	
	glEnd();
	
    shader_affichage -> Unbind_Program();

	
    Texture3D::setFilter(GL_NEAREST);
    
    glEnable(GL_LIGHTING);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
	glPopMatrix();	
}
