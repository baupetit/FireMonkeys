#include "Fluid.h"

#include <iostream>
#include <unistd.h>
#include <cstdlib>


#include "loadprogram.h"

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
        tailleGrille = 40;
        s = new Solver(tailleGrille);
	
	
	tempIndex = new TempToRGB(256,50);
	
/*
	for( int i = 7 ; i < tailleGrille-6 ; ++i ){
		for( int j = 7 ; j < tailleGrille-6 ; ++j ){
			s->setDensity( i ,5, j, 10.0f );        
			s->setTemperature( i ,5, j, (getTempVal(i,j,tailleGrille))/5);        
		}
	}
*/

	int mid = tailleGrille/2;
	s->setDensity( mid ,5, mid, 100.0f );   
	s->setDensity( mid ,6, mid, 100.0f );   
	s->setDensity( mid ,4, mid, 100.0f );   
	s->setDensity( mid+1 ,5, mid, 100.0f );   
	s->setDensity( mid-1 ,5, mid, 100.0f );   
	s->setDensity( mid ,5, mid+1, 100.0f );   
	s->setDensity( mid ,5, mid-1, 100.0f );   
	s->setTemperature( mid ,5, mid, 10*60*((random()+1)/(float)RAND_MAX)*4/1);   
	s->setTemperature( mid ,6, mid, 10*42*((random()+1)/(float)RAND_MAX)*4/1);   
	s->setTemperature( mid ,6, mid, 10*25*((random()+1)/(float)RAND_MAX)*4/1);   
	s->setTemperature( mid+1 ,5, mid, 10*24*((random()+1)/(float)RAND_MAX)*4/1);   
	s->setTemperature( mid-1 ,5, mid, 10*38*((random()+1)/(float)RAND_MAX)*4/1);   
	s->setTemperature( mid ,5, mid+1, 10*12*((random()+1)/(float)RAND_MAX)*4/1);   
	s->setTemperature( mid ,5, mid-1, 10*55*((random()+1)/(float)RAND_MAX)*4/1);   

	

	for( int i = 7 ; i < tailleGrille-6 ; ++i ){
		for( int j = 7 ; j < tailleGrille-6 ; ++j ){
			s->setVelocity(3,i,j, 0.20,0.0,0.0);
		}
	}      

    initialiserRenduGPU();
    _multitex_program = LoadProgram("./Shaders/multitex.vert","./Shaders/multitex.frag");
    cout << "Fluid_Multitex : Shader chargé  " << _multitex_program << endl;
    
	glGenTextures(1,&_id_texture_fumee);
    glBindTexture(GL_TEXTURE_3D, _id_texture_fumee);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexImage3D(GL_TEXTURE_3D,0,GL_RGB,
                 tailleGrille,tailleGrille,tailleGrille,
                 0, GL_RGB, GL_FLOAT, NULL);
	cout << "Texture fumee cree:  " << _id_texture_fumee << endl;	


	glGenTextures(1,&_id_texture_flamme);
    glBindTexture(GL_TEXTURE_3D, _id_texture_flamme);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexImage3D(GL_TEXTURE_3D,0,GL_RGB,
                 tailleGrille,tailleGrille,tailleGrille,
                 0, GL_RGB, GL_FLOAT, NULL);
	cout << "Texture flamme cree: " << _id_texture_flamme << endl;	


    position.x = 0;
    position.y = 0.5;
    position.z = 0;
    
    echelle.x = 10;
    echelle.y = 10;
    echelle.z = 10;
}

Fluid::~Fluid(){
	delete s;
	delete matriceRGBA_fire;
	delete matriceRGBA_smoke;
}




void Fluid::Afficher_Face_Camera(Vecteur3D& positionCamera, Vecteur3D& directionCamera ){

    Mise_A_Jour();
	renduFumeeGPUFaceCamera(positionCamera, directionCamera);
    renduFlammeGPUFaceCamera(positionCamera, directionCamera);
	//renduFlammeETFumeeGPUFaceCamera(positionCamera, directionCamera);
}    
    
void Fluid::Afficher(){

    Mise_A_Jour();
    renduFlammeGPU();
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
    matriceRGBACarreeToTexture3D(matriceRGBA_smoke, tailleGrille + 2 , _id_texture_fumee);
    dessinerPlansDansTexture3D(_id_texture_fumee, 10);
}



void Fluid::renduFlammeGPU(){
    majMatriceFlammeEnMatriceRGBA();
    matriceRGBACarreeToTexture3D(matriceRGBA_fire, tailleGrille + 2 , _id_texture_flamme);
    dessinerPlansDansTexture3D(_id_texture_flamme, 30);
}


	
void Fluid::renduFumeeGPUFaceCamera(Vecteur3D& positionCamera, Vecteur3D& directionCamera ){
    majMatriceFumeeEnMatriceRGBA();
    matriceRGBACarreeToTexture3D(matriceRGBA_smoke, tailleGrille + 2 , _id_texture_fumee);
    dessinerPlansDansTexture3DFaceALaCamera(_id_texture_fumee, 70, positionCamera, directionCamera);
}



void Fluid::renduFlammeGPUFaceCamera(Vecteur3D& positionCamera, Vecteur3D& directionCamera ){
    majMatriceFlammeEnMatriceRGBA();
    matriceRGBACarreeToTexture3D(matriceRGBA_fire, tailleGrille + 2 , _id_texture_flamme);
    dessinerPlansDansTexture3DFaceALaCamera(_id_texture_flamme,70, positionCamera, directionCamera);
}

void Fluid::renduFlammeETFumeeGPUFaceCamera(Vecteur3D& positionCamera, Vecteur3D& directionCamera ){
    majMatriceFlammeEnMatriceRGBA();
	majMatriceFumeeEnMatriceRGBA();
    //matriceRGBACarreeToTexture3D(matriceRGBA_fire, tailleGrille + 2 , _id_texture_flamme);
	//matriceRGBACarreeToTexture3D(matriceRGBA_smoke, tailleGrille + 2 , _id_texture_fumee);
	glBindTexture(GL_TEXTURE_3D, _id_texture_flamme);
	glTexImage3D(GL_TEXTURE_3D,0,GL_RGB,
                 tailleGrille,tailleGrille,tailleGrille,
                 0, GL_RGB, GL_FLOAT, matriceRGBA_fire);


	glBindTexture(GL_TEXTURE_3D, _id_texture_fumee);	
	glTexImage3D(GL_TEXTURE_3D,0,GL_RGB,
                 tailleGrille,tailleGrille,tailleGrille,
                 0, GL_RGB, GL_FLOAT, matriceRGBA_smoke);
  
	//passage des parametres des textures en uniforme
	
	_fumee_id_multitex = glGetUniformLocation ( _multitex_program, "fumee");
   //	glUniform1i(_fumee_id_multitex, _id_texture_fumee);
	_fire_id_multitex = glGetUniformLocation ( _multitex_program, "fire");
   //	glUniform1i(_fire_id_multitex, _id_texture_flamme);
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(_multitex_program);
	glUniform1i(_fire_id_multitex, _id_texture_flamme);
	glUniform1i(_fumee_id_multitex, _id_texture_fumee);

	glBegin(GL_QUADS);
		glTexCoord2d(0,0);						// Begin Drawing Quads
		glVertex2f(0,0);
		glTexCoord2d(0,1);				// First Vertex		(   0,   0 )
		glVertex2f(0,1);
		glTexCoord2d(1,1);				// Second Vertex	(   0, 480 )
		glVertex2f(1,1);				// Third Vertex		( 640, 480 )
		glTexCoord2d(1, 0);
		glVertex2f(1,0);				// Fourth Vertex	( 640,   0 )
	glEnd();



    glUseProgram(0);
 
	

}

void Fluid::majMatriceFlammeEnMatriceRGBA(){

    // Creation de la texture
    Vecteur4D *pointeurMatriceRGBA = matriceRGBA_fire;
    const float *pointeurMatriceACopier = s->getDensities();
    //const float *pointeurMatriceACopier2 = s->getSmokes();
    const float *pointeurMatriceACopier3 = s->getTemperatures();
    float R,G,B;
	float seuil = 0.001;
	
    for (int i = 0; i < (tailleGrille+2)*(tailleGrille+2)*(tailleGrille+2); i ++){
		
		
	    tempIndex->getRGB( (1+(*pointeurMatriceACopier3))*1555 , &R, &G, &B );
	    // R 
    	(*pointeurMatriceRGBA).x = B;
	    //pointeurMatriceRGBA++;
	    // G
	    (*pointeurMatriceRGBA).y = G;
	    //pointeurMatriceRGBA++;
	    // B
	   	(*pointeurMatriceRGBA).z = R;
		//pointeurMatriceRGBA++;
	    // A

		if (*pointeurMatriceACopier < seuil) {
			(*pointeurMatriceRGBA).w = 0;
		}
		else {
			//cout << "densité : " << *pointeurMatriceACopier  << "  température : " << *pointeurMatriceACopier3 << endl;
			(*pointeurMatriceRGBA).w = 1/(*pointeurMatriceACopier3*8);
			//cout << "A= " << *pointeurMatriceRGBA << endl;
		}

	    pointeurMatriceRGBA++;
	    // MAJ
	    pointeurMatriceACopier++;
	    pointeurMatriceACopier3++;
    }    
	
    
}

	
void Fluid::majMatriceFumeeEnMatriceRGBA(){

    // Creation de la texture
    Vecteur4D *pointeurMatriceRGBA = matriceRGBA_smoke;
    //const float *pointeurMatriceACopier  = s->getDensities();
    const float *pointeurMatriceACopier2 = s->getSmokes();
    //const float *pointeurMatriceACopier3 = s->getTemperatures();
    //float R,G,B;
    for (int i = 0; i < (tailleGrille+2)*(tailleGrille+2)*(tailleGrille+2); i ++){
	    // R
	    (*pointeurMatriceRGBA).x = 0.1;
	    //pointeurMatriceRGBA++;
	    // G
	    (*pointeurMatriceRGBA).y = 0.1;
	    //pointeurMatriceRGBA++; 
	    // B
	    (*pointeurMatriceRGBA).z = 0.1;
	    //pointeurMatriceRGBA++;
	    // A
	    (*pointeurMatriceRGBA).w = *pointeurMatriceACopier2  /3;
	    pointeurMatriceRGBA++;
	    // MAJ
	    pointeurMatriceACopier2++;
    }    
}

	

void Fluid::matriceRGBACarreeToTexture3D(const Vecteur4D *matrice, int cote, GLuint id_texture){

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
    //glGenTextures(1,&_id_texture_flamme);
    //glGenTextures(1,&_id_texture_fumee);
    matriceRGBA_smoke = new Vecteur4D[(tailleGrille+2)*(tailleGrille+2)*(tailleGrille+2)];
	matriceRGBA_fire  = new Vecteur4D[(tailleGrille+2)*(tailleGrille+2)*(tailleGrille+2)];
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
/*
    Boitev0.rotationAutourAxeY(angleOxz);
    Boitev1.rotationAutourAxeY(angleOxz);
    Boitev2.rotationAutourAxeY(angleOxz);
    Boitev3.rotationAutourAxeY(angleOxz);
 */   
    
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
	/*
    Tex0.rotationAutourAxeY(angleOxz);
    Tex1.rotationAutourAxeY(angleOxz);
    Tex2.rotationAutourAxeY(angleOxz);
    Tex3.rotationAutourAxeY(angleOxz);
    */
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
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
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
    glDisable(GL_ALPHA_TEST);    
    glDisable( GL_BLEND );    
    glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	
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
	s->velocitiesStepWithTemp(0.0, 0.5, 2.0,0.1);
	s->densitiesStepWithTemp(0.001,0.00001,0.01, 2, 1  , 0.4, 2, 0.1);

}








