#include "Fluid.h"

#include <iostream>
#include <unistd.h>
#include <cstdlib>


#include <cmath>
#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif


using namespace std;

static inline float getTempVal( int i, int j, int T ){
	float val;
	if( i < T/2 ) val = i ;
	else          val = T - i ;
	if( j < T/2 ) val *= j ;
	else          val *= T - j ;
	return val * ((random()+1)/(float)RAND_MAX)*4;
}

Fluid::Fluid()
{
        tailleGrille = 30;
        
        s = new Solver(tailleGrille);

	
	tempIndex = new TempToRGB(128,200);
	
	for( int i = 7 ; i < tailleGrille-6 ; ++i ){
		for( int j = 7 ; j < tailleGrille-6 ; ++j ){
			s->setDensity( i ,5, j, 1.0f );        
			s->setTemperature( i ,5, j, (getTempVal(i,j,tailleGrille))/13);        
		}
	}

	//s->setDensity( 3*tailleGrille /4 ,5, 3*tailleGrille/4, 1 );        
	//s->setTemperature( 3*tailleGrille /4 ,5, 3*tailleGrille/4, 50 );        

        initialiserRenduGPU();
        
        
        position.x = 1;
        position.y = 1;
        position.z = -1;
}

Fluid::~Fluid(){
	delete s;
	delete matriceRGBA;
}



void Fluid::Afficher_Face_Camera(Vecteur3D& positionCamera ){

    Mise_A_Jour();
    renduFumeeGPUFaceCamera(positionCamera);
}    
    
void Fluid::Afficher(){

    Mise_A_Jour();
    renduFumeeGPU();

	////////////////////////////////////////////////////////////////////////////
	//// DENSITE
	// dessin de la grille de densite
	/*	
		  
	const float *dens = s->getDensities();

	float c;
	
	glPointSize( 4.0f );
	glBegin( GL_POINTS );
	//glColor3f( 1,0,0 );
	//glVertex3f( 0, 0 , 0 );
	
	for( int k = 1; k<N ; k++ ){
		for( int j = 1; j<N ; j++ ){
			for( int i = 1; i<N ; i++ ){
				c = dens[IX(i,j,k)];
				glColor4f( c,c,c, 0.5 );
				
				glVertex3f( i/(float)N , j/(float)N , k/(float)N );
				
			}
		}
	}
	
	glEnd();
	*/
	  
	////////////////////////////////////////////////////////////////////////////
    
    
    
	////////////////////////////////////////////////////////////////////////////
	//// VITESSE
	// dessin de la grille de vitesse
	/*
	glClearColor( 1. , 0. , 1. , 0. );
	
	const float *u = s->getVelocityU();
	const float *v = s->getVelocityV();
	const float *w = s->getVelocityW();
	const int N = s->getSize();
	float u1,v1,w1;

	glPointSize( 4.0f );
	glBegin( GL_POINTS );
	//glColor3f( 1,0,0 );
	//glVertex3f( 0, 0 , 0 );

	for( int k = 1; k<N ; k++ ){
		for( int j = 1; j<N ; j++ ){
			for( int i = 1; i<N ; i++ ){
				u1 = u[IX(i,j,k)];
				v1 = v[IX(i,j,k)];
				w1 = w[IX(i,j,k)];	
				
				glColor4f( u1,v1,w1, 0.5 );
				
				glVertex3f( i/(float)N , j/(float)N , k/(float)N );
				// cout << "d["<<i<<","<<j<<","<<k<<"] = " << c << endl ;
			}
		}
	}
	glEnd();
	*/
    
    
		
}


	
void Fluid::renduFumeeGPU(){
    majMatriceFumeeEnMatriceRGBA();
    matriceRGBACarreeToTexture3D(matriceRGBA, tailleGrille + 2 , _id_texture_fumee);
    dessinerPlansDansTexture3D(_id_texture_fumee, 50);
}



void Fluid::renduFlammeGPU(){
}


	
void Fluid::renduFumeeGPUFaceCamera(Vecteur3D& positionCamera ){
    majMatriceFumeeEnMatriceRGBA();
    matriceRGBACarreeToTexture3D(matriceRGBA, tailleGrille + 2 , _id_texture_fumee);
    dessinerPlansDansTexture3DFaceALaCamera(_id_texture_fumee, 50, positionCamera);
}



void Fluid::renduFlammeGPUFaceCamera(Vecteur3D& positionCamera ){
}


void Fluid::majMatriceFumeeEnMatriceRGBA(){

    // Creation de la texture
    float *pointeurMatriceRGBA = matriceRGBA;
    const float *pointeurMatriceACopier = s->getDensities();
    const float *pointeurMatriceACopier2 = s->getSmokes();
    const float *pointeurMatriceACopier3 = s->getTemperatures();
    float R,G,B;
    for (int i = 0; i < (tailleGrille+2)*(tailleGrille+2)*(tailleGrille+2); i ++){
	    tempIndex->getRGB( (*pointeurMatriceACopier3)*4000 , &R, &G, &B );
	    // R
	    *pointeurMatriceRGBA = B;
	    pointeurMatriceRGBA++;
	    // G
	    *pointeurMatriceRGBA = G;
	    pointeurMatriceRGBA++;
	    // B
	    *pointeurMatriceRGBA = R;
	    pointeurMatriceRGBA++;
	    // A
	    *pointeurMatriceRGBA = *pointeurMatriceACopier * 4 - *pointeurMatriceACopier3*2;
	    pointeurMatriceRGBA++;
	    // MAJ
	    pointeurMatriceACopier++;
	    pointeurMatriceACopier3++;
    }    
}
	

void Fluid::matriceRGBACarreeToTexture3D(const float *matrice, int cote, GLuint id_texture){

    // Chargement en mémoire
    glBindTexture(GL_TEXTURE_3D, id_texture);
    
/*
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    */
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,cote,cote,cote,
                 0, GL_BGRA, GL_FLOAT, matrice);
                 
}




void Fluid::initialiserRenduGPU(){
    glGenTextures(1,&_id_texture_flamme);
    glGenTextures(1,&_id_texture_fumee);
    matriceRGBA = new float[4*(tailleGrille+2)*(tailleGrille+2)*(tailleGrille+2)];
}




void Fluid::dessinerPlansDansTexture3DFaceALaCamera(GLuint id_texture, int nb_plans,
	                                     Vecteur3D& positionCamera){
	       
    glPushMatrix();
    
    glDisable(GL_LIGHTING);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );
    glAlphaFunc(GL_GREATER,0.0f);
    glEnable(GL_ALPHA_TEST);    
	glEnable(GL_TEXTURE_3D);
    glActiveTexture(id_texture);
    
    
    Vecteur3D Boitev0 = Vecteur3D(-0.5, -0.5, 0);
    Vecteur3D Boitev1 = Vecteur3D(-0.5, 0.5, 0);
    Vecteur3D Boitev2 = Vecteur3D(0.5, 0.5, 0);
    Vecteur3D Boitev3 = Vecteur3D(0.5, -0.5, 0);
    
    Vecteur3D direction = position - positionCamera;
    position.afficher();
    positionCamera.afficher();
    direction.afficher();
    
    float distOZY = sqrt ( direction.z*direction.z + direction.y*direction.y);
    float distOXZ = sqrt ( direction.z*direction.z + direction.x*direction.x);
    float anglePlanOZY = asinf(direction.y/distOZY);
    float anglePlanOXZ = asinf(direction.x/distOXZ);    
    
    
    cout << "angle 1 : " << anglePlanOZY << endl;      
    Boitev0.rorationAutourAxeX(anglePlanOZY);
    Boitev1.rorationAutourAxeX(anglePlanOZY);
    Boitev2.rorationAutourAxeX(anglePlanOZY);
    Boitev3.rorationAutourAxeX(anglePlanOZY);
    
    
    cout << "angle 2 : " << anglePlanOXZ << endl;
    Boitev0.rorationAutourAxeY(anglePlanOXZ);
    Boitev1.rorationAutourAxeY(anglePlanOXZ);
    Boitev2.rorationAutourAxeY(anglePlanOXZ);
    Boitev3.rorationAutourAxeY(anglePlanOXZ);
    
  
    
    GLfloat verts[4][3] = { {Boitev0.x, Boitev0.y, Boitev0.z}, 
                            {Boitev1.x, Boitev1.y, Boitev1.z}, 
                            {Boitev2.x, Boitev2.y, Boitev2.z}, 
                            {Boitev3.x, Boitev3.y, Boitev3.z}};
	
	glBegin(GL_TRIANGLES);	
	    glVertex3d(verts[0][0], verts[0][1], verts[0][2]);
	    glVertex3d(verts[1][0], verts[1][1], verts[1][2]);
	    glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
	    
	    glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
	    glVertex3d(verts[3][0], verts[3][1], verts[3][2]);
	    glVertex3d(verts[0][0], verts[0][1], verts[0][2]);
	    
	    
	    
	
	/*
	for (int i = 0; i < 0; i++){
		glTexCoord3d(verts[0][0] + N.x * (float)i/nb_plans, 
		             verts[0][1] + N.y * (float)i/nb_plans, 
		             verts[0][2] + N.z * (float)i/nb_plans);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]+ (float)i/nb_plans);
		
		glTexCoord3d(verts[1][0] + N.x * (float)i/nb_plans, 
		             verts[1][1] + N.y * (float)i/nb_plans, 
		             verts[1][2] + N.z * (float)i/nb_plans);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2] + (float)i/nb_plans);
				
		glTexCoord3d(verts[2][0] + N.x * (float)i/nb_plans, 
		             verts[2][1] + N.y * (float)i/nb_plans, 
		             verts[2][2] + N.z * (float)i/nb_plans);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/nb_plans);
		
		glTexCoord3d(verts[2][0] + N.x * (float)i/nb_plans, 
		             verts[2][1] + N.y * (float)i/nb_plans, 
		             verts[2][2] + N.z * (float)i/nb_plans);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/nb_plans);
		
		glTexCoord3d(verts[3][0] + N.x * (float)i/nb_plans, 
		             verts[3][1] + N.y * (float)i/nb_plans, 
		             verts[3][2] + N.z * (float)i/nb_plans);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2] + (float)i/nb_plans);
		
		glTexCoord3d(verts[0][0] + N.x * (float)i/nb_plans, 
		             verts[0][1] + N.y * (float)i/nb_plans, 
		             verts[0][2] + N.z * (float)i/nb_plans);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/nb_plans);	
	}
	*/
	glEnd();
	
	
	glDisable(GL_TEXTURE_3D);
    glDisable( GL_BLEND );    
    glEnable(GL_LIGHTING);
    
    glPopMatrix();
}





void Fluid::dessinerPlansDansTexture3D(GLuint id_texture, int nb_plans){


	
    glPushMatrix();
    //glLoadIdentity();    
	//glOrtho(0, 1, 1, 0, 0, 1);
    //glTranslatef(position.x, position.y, position.z);
	


    glDisable(GL_LIGHTING);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );
    glAlphaFunc(GL_GREATER,0.0f);
    glEnable(GL_ALPHA_TEST);
    
	glEnable(GL_TEXTURE_3D);
    glActiveTexture(id_texture);


    GLfloat verts[4][3] = { { 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 0.0, 0.0}};
    

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
	
	
	for (int i = 1; i <= 2 * nb_plans; i++){
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/nb_plans);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]+ (float)i/nb_plans);
		
		glTexCoord3d(verts[1][0], verts[1][1], verts[1][2] + (float)i/nb_plans);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2] + (float)i/nb_plans);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/nb_plans);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/nb_plans);
		
		glTexCoord3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/nb_plans);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/nb_plans);
		
		glTexCoord3d(verts[3][0], verts[3][1], verts[3][2] + (float)i/nb_plans);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2] + (float)i/nb_plans);
		
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/nb_plans);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/nb_plans);	
	}
	glEnd();
	
	
	glDisable(GL_TEXTURE_3D);
    glDisable( GL_BLEND );    
    glEnable(GL_LIGHTING);
    
    glPopMatrix();
	
}


void Fluid::Mise_A_Jour(){
	s->velocitiesStepWithTemp(0.0, 1, 2.0,0.1);
	s->densitiesStepWithTemp(0.00001,0.001,0.0001, 1, 0.4, 0.01, 0.1);
}








