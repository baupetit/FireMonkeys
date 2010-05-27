#include "Fluid.h"



#include <iostream>
#include <unistd.h>

using namespace std;

Fluid::Fluid()
{
        tailleGrille = 20;
        
        s = new Solver(tailleGrille);
        s->setVelocity( tailleGrille/2,1,tailleGrille/2,0,100,0);
	s->setDensity( tailleGrille /2 , tailleGrille/2, tailleGrille/2, 10 );        
        initialiserRenduGPU();
}

Fluid::~Fluid(){
	delete s;
	delete matriceRGBA;
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


void Fluid::majMatriceFumeeEnMatriceRGBA(){

    // Creation de la texture
    float *pointeurMatriceRGBA = matriceRGBA;
    const float *pointeurMatriceACopier = s->getDensities();
    for (int i = 0; i < (tailleGrille+2)*(tailleGrille+2)*(tailleGrille+2); i ++){
        // R
        *pointeurMatriceRGBA = 0.5;
        pointeurMatriceRGBA++;
        // G
        *pointeurMatriceRGBA = 0.5;
        pointeurMatriceRGBA++;
        // B
        *pointeurMatriceRGBA = 0.5;
        pointeurMatriceRGBA++;
        // A
        *pointeurMatriceRGBA = *pointeurMatriceACopier;
        pointeurMatriceRGBA++;
        // MAJ
        pointeurMatriceACopier++;
    }    
}
	

void Fluid::matriceRGBACarreeToTexture3D(const float *matrice, int cote, GLuint id_texture){

    // Chargement en mémoire
    glBindTexture(GL_TEXTURE_3D, id_texture);
    

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
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





void Fluid::dessinerPlansDansTexture3D(GLuint id_texture, int nb_plans){
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
	
}


void Fluid::Mise_A_Jour(){

	s->densitiesStep(0., 0.01);
	s->velocitiesStep(0., 0.01);
}








