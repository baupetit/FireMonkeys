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
        tailleGrille = 20;
        
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
        
        
        position.x = 0;
        position.y = 0.5;
        position.z = 0;
        
        echelle.x = 10;
        echelle.y = 10;
        echelle.z = 10;
}

Fluid::~Fluid(){
	delete s;
	delete matriceRGBA;
}



void Fluid::Afficher_Face_Camera(Vecteur3D& positionCamera, Vecteur3D& directionCamera ){

    Mise_A_Jour();
    renduFumeeGPUFaceCamera(positionCamera, directionCamera);
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
    dessinerPlansDansTexture3D(_id_texture_fumee, 20);
}



void Fluid::renduFlammeGPU(){
}


	
void Fluid::renduFumeeGPUFaceCamera(Vecteur3D& positionCamera, Vecteur3D& directionCamera ){
    majMatriceFumeeEnMatriceRGBA();
    matriceRGBACarreeToTexture3D(matriceRGBA, tailleGrille + 2 , _id_texture_fumee);
    dessinerPlansDansTexture3DFaceALaCamera(_id_texture_fumee, 20, positionCamera, directionCamera);
}



void Fluid::renduFlammeGPUFaceCamera(Vecteur3D& positionCamera, Vecteur3D& directionCamera ){
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
	    *pointeurMatriceRGBA = *pointeurMatriceACopier* 4 - *pointeurMatriceACopier3*2;
	    pointeurMatriceRGBA++;
	    // MAJ
	    pointeurMatriceACopier++;
	    pointeurMatriceACopier3++;
    }    
    
    ///////////////////////////////////////////////
    // A SUPPRIMER
    ///////////////////////////////////////////////
    /*
    
    pointeurMatriceRGBA = matriceRGBA;
    for (int i = (tailleGrille+2)*(tailleGrille+2)*5; i < (tailleGrille+2)*(tailleGrille+2)*(tailleGrille-5); i ++){
	    // R
	    *pointeurMatriceRGBA = 1.0;//B;
	    pointeurMatriceRGBA++;
	    // G
	    *pointeurMatriceRGBA = 1.0;//G;
	    pointeurMatriceRGBA++;
	    // B
	    *pointeurMatriceRGBA = 0.0;//R;
	    pointeurMatriceRGBA++;
	    // A
	    *pointeurMatriceRGBA = 1.0;// *pointeurMatriceACopier ;// * 4 - *pointeurMatriceACopier3*2;
	    pointeurMatriceRGBA++;
    } 
    
    */ 
    ///////////////////////////////////////////////
}

	

void Fluid::matriceRGBACarreeToTexture3D(const float *matrice, int cote, GLuint id_texture){

    // Chargement en mémoire
    glBindTexture(GL_TEXTURE_3D, id_texture);
    
    
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    
    
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
	                                     Vecteur3D& positionCamera, Vecteur3D& directionCamera){
	       

       
    Vecteur3D directionInitiale = Vecteur3D (0,0,1);
    /*
    cout << "directionInitiale ";
    directionInitiale.afficher();
    */
    
    Vecteur3D N = positionCamera - position;
    N.normaliser();
    /*
    cout << "N ";
    N.afficher();
    */
    
    // Calcul de l'angle Oxz
    float angleOxz = atan2(N.z, N.x) - atan2(directionInitiale.z, directionInitiale.x);
    while ( angleOxz >= M_PI ) angleOxz -= 2*M_PI;
    while ( angleOxz <= -M_PI ) angleOxz += 2*M_PI;
    /*
    cout << "Angle Oxz " << angleOxz << endl;
    */
    
    // Calcul de l'angle Oyz
    Vecteur3D distance = - positionCamera + position;
    float angleOyz = asin(distance.y / norme(distance));
    while ( angleOyz >= M_PI ) angleOyz -= 2*M_PI;
    while ( angleOyz <= -M_PI ) angleOyz += 2*M_PI;
    /*
    cout << "Angle Oyz " << angleOyz << endl;
    */
    

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
    
    Tex0 += centre; 
    Tex1 += centre; 
    Tex2 += centre; 
    Tex3 += centre; 
    
    
    // Direction des autres faces
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
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < nb_plans; i++){
        decalageTex  = - float(i)/nb_plans * N;//Vecteur3D(0,0,+ float(i)/nb_plans);// * N;
        decalageVert = - float(i)/nb_plans * N;
        
        
        coord = (Tex0 + decalageTex);
        /*
        cout << "Tex 0 : ";
        coord.afficher();
        */
        glTexCoord3d(coord.x, coord.y, coord.z);                
        coord = Boitev0 + decalageVert;
        /*
        cout << "Point 0 : ";
        coord.afficher();
        */
        glVertex3d(coord.x, coord.y, coord.z);
        
        coord = (Tex1 + decalageTex);
        /*
        cout << "Tex 1 : ";
        coord.afficher();
        */
        glTexCoord3d(coord.x, coord.y, coord.z);        
        coord = Boitev1 + decalageVert;
        /*
        cout << "Point 1 : ";
        coord.afficher();
        */
        glVertex3d(coord.x, coord.y, coord.z);
        
        coord = (Tex2 + decalageTex);  
        /*
        cout << "Tex 2 : ";
        coord.afficher();
        */
        glTexCoord3d(coord.x, coord.y, coord.z);
        coord = Boitev2 + decalageVert;
        /*
        cout << "Point 2 : ";
        coord.afficher();
        */
        glVertex3d(coord.x, coord.y, coord.z);
          
        
        coord = (Tex2 + decalageTex);
        glTexCoord3d(coord.x, coord.y, coord.z);
        coord = Boitev2 + decalageVert;
        glVertex3d(coord.x, coord.y, coord.z);
        
        
        coord = (Tex3 + decalageTex);
        glTexCoord3d(coord.x, coord.y, coord.z);
        coord = Boitev3 + decalageVert;
        glVertex3d(coord.x, coord.y, coord.z);
        
        
        coord = (Tex0 + decalageTex);
        glTexCoord3d(coord.x, coord.y, coord.z);
        coord = Boitev0 + decalageVert;
        glVertex3d(coord.x, coord.y, coord.z);
    }
		
	glEnd();
	
	// Desactivation des elements
	glDisable(GL_TEXTURE_3D);
    glDisable( GL_BLEND );    
    glEnable(GL_LIGHTING);
    
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








