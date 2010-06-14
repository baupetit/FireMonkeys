#include "Fluid_GPU.h"

#include <iostream>
#include <cstdlib>
using namespace std;



Fluid_GPU::Fluid_GPU(){
	// Taille de la grille
	/*
	_grille_width  = TAILLE_GRILLE;
	_grille_height = TAILLE_GRILLE;
	_grille_depth  = TAILLE_GRILLE;
	*/

	_grille_width  = 100;
	_grille_height = 100;
	_grille_depth  = 100 ;


	s = NULL;
    shader_affichage = NULL;
    
    echelle.x = 1.0;
    echelle.y = 1.0;
    echelle.z = 1.0;
    
    
    int tailleGrille = _grille_width;    
	//Génération du bruit de perlin.
	p = new Perlin( 3 , tailleGrille , 7, tailleGrille+2 , tailleGrille+2 , tailleGrille+2 , 0.9f );
	p->Perlin::init();
	p->Perlin::init1D();

	perl = p->genererNoise();
	perl_temps = p->genererNoise1D();

	for (int i = 0 ; i< 100 ; i++)
	cout << "x= " << perl_temps[i].x << "y= " << perl_temps[i].y  << "z= " << perl_temps[i].z << endl;

	//attenuation du bruit en dégradé de haut en bas.
	for (int k=0; k< (tailleGrille+2); k++){
		for (int j=0; j< (tailleGrille+2); j++){
			for (int i=0; i< (tailleGrille+2); i++){
				perl[i+j*(tailleGrille+2)+k*(tailleGrille+2)*(tailleGrille+2)] = ((float)j/(float)((tailleGrille+2))) * perl[i+j*(tailleGrille+2)+k*(tailleGrille+2)*(tailleGrille+2)] ;
				
				
			}
		}
	}

    // Couleur
       float fColorMatch[][3]={
         {0.0014, 0.0000, 0.0065},
         {0.0022, 0.0001, 0.0105},
         {0.0042, 0.0001, 0.0201},
         {0.0076, 0.0002, 0.0362},
         {0.0143, 0.0004, 0.0679},
         {0.0232, 0.0006, 0.1102},
         {0.0435, 0.0012, 0.2074},
         {0.0776, 0.0022, 0.3713},
         {0.1344, 0.0040, 0.6456},
         {0.2148, 0.0073, 1.0391},
         {0.2839, 0.0116, 1.3856},
         {0.3285, 0.0168, 1.6230},
         {0.3483, 0.0230, 1.7471},
         {0.3481, 0.0298, 1.7826},
         {0.3362, 0.0380, 1.7721},
         {0.3187, 0.0480, 1.7441},
         {0.2908, 0.0600, 1.6692},
         {0.2511, 0.0739, 1.5281},
         {0.1954, 0.0910, 1.2876},
         {0.1421, 0.1126, 1.0419},
         {0.0956, 0.1390, 0.8130},
         {0.0580, 0.1693, 0.6162},
         {0.0320, 0.2080, 0.4652},
         {0.0147, 0.2586, 0.3533},
         {0.0049, 0.3230, 0.2720},
         {0.0024, 0.4073, 0.2123},
         {0.0093, 0.5030, 0.1582},
         {0.0291, 0.6082, 0.1117},
         {0.0633, 0.7100, 0.0782},
         {0.1096, 0.7932, 0.0573},
         {0.1655, 0.8620, 0.0422},
         {0.2257, 0.9149, 0.0298},
         {0.2904, 0.9540, 0.0203},
         {0.3597, 0.9803, 0.0134},
         {0.4334, 0.9950, 0.0087},
         {0.5121, 1.0000, 0.0057},
         {0.5945, 0.9950, 0.0039},
         {0.6784, 0.9786, 0.0027},
         {0.7621, 0.9520, 0.0021},
         {0.8425, 0.9154, 0.0018},
         {0.9163, 0.8700, 0.0017},
         {0.9786, 0.8163, 0.0014},
         {1.0263, 0.7570, 0.0011},
         {1.0567, 0.6949, 0.0010},
         {1.0622, 0.6310, 0.0008},
         {1.0456, 0.5668, 0.0006},
         {1.0026, 0.5030, 0.0003},
         {0.9384, 0.4412, 0.0002},
         {0.8544, 0.3810, 0.0002},
         {0.7514, 0.3210, 0.0001},
         {0.6424, 0.2650, 0.0000},
         {0.5419, 0.2170, 0.0000},
         {0.4479, 0.1750, 0.0000},
         {0.3608, 0.1382, 0.0000},
         {0.2835, 0.1070, 0.0000},
         {0.2187, 0.0816, 0.0000},
         {0.1649, 0.0610, 0.0000},
         {0.1212, 0.0446, 0.0000},
         {0.0874, 0.0320, 0.0000},
         {0.0636, 0.0232, 0.0000},
         {0.0468, 0.0170, 0.0000},
         {0.0329, 0.0119, 0.0000},
         {0.0227, 0.0082, 0.0000},
         {0.0158, 0.0057, 0.0000},
         {0.0114, 0.0041, 0.0000},
         {0.0081, 0.0029, 0.0000},
         {0.0058, 0.0021, 0.0000},
         {0.0041, 0.0015, 0.0000},
         {0.0029, 0.0010, 0.0000},
         {0.0020, 0.0007, 0.0000},
         {0.0014, 0.0005, 0.0000},
         {0.0010, 0.0004, 0.0000},
         {0.0007, 0.0002, 0.0000},
         {0.0005, 0.0002, 0.0000},
         {0.0003, 0.0001, 0.0000},
         {0.0002, 0.0001, 0.0000},
         {0.0002, 0.0001, 0.0000},
         {0.0001, 0.0000, 0.0000},
         {0.0001, 0.0000, 0.0000},
         {0.0001, 0.0000, 0.0000},
         {0.0000, 0.0000, 0.0000}};

    float *color = new float[253];
    float *temp = color;
    for (int i = 0; i < 81; i ++){
        *temp = fColorMatch[i][0];
        temp++; 
        *temp = fColorMatch[i][1];
        temp++; 
        *temp = fColorMatch[i][2];
        temp++; 
    }
    
	glBindTexture(GL_TEXTURE_1D, _color_texture);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,81, 0, GL_RGB, GL_FLOAT, color);
    

	

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
	s->densitiesStepWithTemp(0.1);
	s->velocitiesStepWithTemp(0.1);
}

void Fluid_GPU::Afficher(){
	// MAJ
	resolutionFluid();

	// Affichage
	afficherFlamme();
}

void Fluid_GPU::Afficher_Face_Camera(Vecteur3D& positionCamera, Vecteur3D& orientationCamera){

	// MAJ
	resolutionFluid();

	// Affichage
	dessinerPlansDansTexture3DFaceALaCamera(s->getDensities(),100, positionCamera, orientationCamera);
	
}

void Fluid_GPU::afficherFlamme(){
	// Feu
	dessinerPlansDansTexture3D(s->getDensities(),100);
}

void Fluid_GPU::afficherFumee(){
}

void Fluid_GPU::dessinerPlansDansTexture3DFaceALaCamera(GLuint id_texture, 
                                                        int nb_plans,
							                            Vecteur3D& positionCamera, 
							                            Vecteur3D& directionCamera){
	       
	float t = 0.2;
	int tailleGrille = _grille_width;
	       
	srand(NULL);
	tps1 += t;
	tps2 += t;
	matricePerlinCarreeToTexture3D(perl, tailleGrille + 2 , _id_texture_perlin);
	VecteurPerlinTempsToTexture1D(perl_temps, 100 , _id_texture_perlin_temps);

	       
	       
    //GLuint id0 = s->getPression();
    GLuint id0 = s->getDensities();
    //GLuint id0 = s->getSpeed();
    //GLuint id1 = s->getDestDensities();

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_3D , id0 );
	
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_3D , _id_texture_perlin );
	
	glActiveTexture( GL_TEXTURE2 );
	glBindTexture(GL_TEXTURE_1D , _id_texture_perlin_temps );
	
	glActiveTexture( GL_TEXTURE3 );
	glBindTexture(GL_TEXTURE_1D , _color_texture );
	
	
	glActiveTexture( GL_TEXTURE0 );
	
	
	Texture3D::setFilter(GL_LINEAR);        	
	
	

    shader_affichage -> Bind_Program();
    
    shader_affichage->lierLevel("texture_entree", 0);
    shader_affichage->lierLevel("Textureperlin", 1);
    shader_affichage->lierLevel("Temps", 2);
    shader_affichage->lierLevel("ColorTexture", 3);    
    shader_affichage->lierFloat("temps1", ((float) rand()) / RAND_MAX*tps2 );
    shader_affichage->lierFloat("temps2", ((float) rand()) / RAND_MAX*tps2 );
    shader_affichage->lierFloat("temps3", t);
    
	       
	       
	       
	       
	       
	       
	       
	       
	       
	       
	       
	       
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
	
	
    shader_affichage -> Unbind_Program();
    Texture3D::setFilter(GL_NEAREST);	
    
    
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

    //GLuint id0 = s->getPression();
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








void Fluid_GPU::matricePerlinCarreeToTexture3D(const Vecteur3D *matrice, int cote, GLuint id_texture){
	// Chargement en mémoire
	glBindTexture(GL_TEXTURE_3D, id_texture);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage3D(GL_TEXTURE_3D,0,GL_RGB,cote,cote,cote,
		     0, GL_RGB, GL_FLOAT, matrice);
}

void Fluid_GPU::VecteurPerlinTempsToTexture1D(const Vecteur3D *matrice, int cote, GLuint id_texture){

	// Chargement en mémoire
	glBindTexture(GL_TEXTURE_1D, id_texture);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,cote,
		     0, GL_RGB, GL_FLOAT, matrice);

}

