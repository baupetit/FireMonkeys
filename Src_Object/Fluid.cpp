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

Fluid::Fluid(list<Object *> obj)
	:obj(obj)
{
    tailleGrille = 20;
    
    s = new Solver(tailleGrille);
    
	tempIndex = new TempToRGB(256,50);
	
	// le bruit
		//Génération du bruit de perlin.
	p = new Perlin( 3 , tailleGrille , 7, tailleGrille+2 , tailleGrille+2 , tailleGrille+2 , 0.9f );
	p->Perlin::init();
	p->Perlin::init1D();

	perl = p->genererNoise();
	perl_temps = p->genererNoise1D();

	//attenuation du bruit en dégradé de haut en bas.
	for (int k=0; k< (tailleGrille+2); k++){
		for (int j=0; j< (tailleGrille+2); j++){
			for (int i=0; i< (tailleGrille+2); i++){
				perl[i+j*(tailleGrille+2)+k*(tailleGrille+2)*(tailleGrille+2)] = 
				     ((float)j/(float)((tailleGrille+2))) 
				     * perl[i+j*(tailleGrille+2)+k
				     *(tailleGrille+2)*(tailleGrille+2)] ;
				
				
			}
		}
	}
	

	
	
	
	
	
		
	// la flamme
	int mid = tailleGrille/2;
	
	s->setDensity( mid ,5, mid, 10.0f );   
	s->setDensity( mid ,6, mid, 10.0f );   
	s->setDensity( mid ,4, mid, 10.0f );
	s->setDensity( mid+1 ,5, mid, 10.0f );   
	s->setDensity( mid-1 ,5, mid, 10.0f );   
	s->setDensity( mid ,5, mid+1, 10.0f );   
	s->setDensity( mid ,5, mid-1, 10.0f );   
	
	s->setTemperature( mid ,5, mid, 60*((random()+1)/(float)RAND_MAX)*4/1);   
	s->setTemperature( mid ,6, mid, 42*((random()+1)/(float)RAND_MAX)*4/1);   
	s->setTemperature( mid ,6, mid, 25*((random()+1)/(float)RAND_MAX)*4/1);   
	s->setTemperature( mid+1 ,5, mid, 24*((random()+1)/(float)RAND_MAX)*4/1);   
	s->setTemperature( mid-1 ,5, mid, 38*((random()+1)/(float)RAND_MAX)*4/1);   
	s->setTemperature( mid ,5, mid+1, 12*((random()+1)/(float)RAND_MAX)*4/1);   
	s->setTemperature( mid ,5, mid-1, 55*((random()+1)/(float)RAND_MAX)*4/1);   
	
	initialiserRenduGPU();

	position.x = 0;
	position.y = 0;
	position.z = 0;
	
	float spaceDiv = SolverParam::getSpaceDiv();
	AABB = BoundingBox(Vecteur3D( -tailleGrille*spaceDiv/2, -tailleGrille*spaceDiv/2, -tailleGrille*spaceDiv/2 ) ,
			   Vecteur3D(  tailleGrille*spaceDiv/2,  tailleGrille*spaceDiv/2,  tailleGrille*spaceDiv/2 ) );

}

Fluid::~Fluid(){
	delete s;
	delete matriceRGBA_fire;
	delete matriceRGBA_smoke;
}

void Fluid::Afficher_Face_Camera(Vecteur3D& positionCamera, Vecteur3D& directionCamera, float dt ){
	Mise_A_Jour(0.1);
/*
	glPointSize( 2.0f );
	glDisable(GL_LIGHTING);

	int N = s->getSize();
	const int* voxel = s->getFilledInfo();

	glBegin(GL_POINTS);
	for( int k = 0 ; k < N+2 ; ++k ){
		for( int j = 0 ; j < N+2 ; ++j ){
			for( int i = 0 ; i < N+2 ; ++i ){
				Vecteur3D p = s->cellToPoint( i , j , k );
				float val = voxel[IX(i,j,k)];
	  
				if( val != 0 ){
					glColor3f( 1,0,0 );
					glVertex3f( p.x, p.y, p.z );
				} else 
					glColor3f( 0,0,1 );

			}
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
*/
	//renduFumeeGPUFaceCamera(positionCamera, directionCamera);
	//renduFlammeGPUFaceCamera(positionCamera, directionCamera);
	renduFlammeETFumeeGPUFaceCamera(400, positionCamera, directionCamera, dt);
}    
    
void Fluid::Afficher(float dt){
	Mise_A_Jour(dt);
	renduFlammeGPU();
	renduFumeeGPU();
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
	glEnable(GL_TEXTURE_3D);
	glActiveTexture(_id_texture_fumee);
	dessinerPlansDansTexture3DFaceALaCamera(100, positionCamera, directionCamera);
}

void Fluid::renduFlammeGPUFaceCamera(Vecteur3D& positionCamera, Vecteur3D& directionCamera ){
	majMatriceFlammeEnMatriceRGBA();
	matriceRGBACarreeToTexture3D(matriceRGBA_fire, tailleGrille + 2 , _id_texture_flamme);
	glEnable(GL_TEXTURE_3D);
	glActiveTexture(_id_texture_flamme);
	dessinerPlansDansTexture3DFaceALaCamera(100, positionCamera, directionCamera);
}

void Fluid::renduFlammeETFumeeGPUFaceCamera( int nb_plans, Vecteur3D& positionCamera, Vecteur3D& directionCamera ,float t ){
/*	majMatriceFlammeEnMatriceRGBA();
	majMatriceFumeeEnMatriceRGBA();


	matriceRGBACarreeToTexture3D(matriceRGBA_smoke, tailleGrille + 2 , _id_texture_fumee);
	matriceRGBACarreeToTexture3D(matriceRGBA_fire, tailleGrille + 2 , _id_texture_flamme);

	glBindTexture(GL_TEXTURE_3D, 0);	
	
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_3D , _id_texture_flamme );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_3D , _id_texture_fumee );

	renderer->Bind_Program();
	GLuint location_tex1 = glGetUniformLocation( renderer->getProgramId(), "Texture0" );
	glUniform1i( location_tex1 , 0 );
       	GLuint location_tex2 = glGetUniformLocation( renderer->getProgramId(), "Texture1" );
	glUniform1i( location_tex2 , 1 );

	// Dessin
	dessinerPlansDansTexture3DFaceALaCamera(nb_plans, positionCamera, directionCamera);
	
	renderer->Unbind_Program();

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_3D , 0 );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_3D , 0 );
*/
	srand(NULL);
	tps1 += t;
	tps2 += t;
	//tps3 = fmod(tps3 + t*((float) rand()) / RAND_MAX,2*M_PI);
	majMatriceFlammeEnMatriceRGBA();
	majMatriceFumeeEnMatriceRGBA();

	matriceRGBACarreeToTexture3D(matriceRGBA_smoke, tailleGrille + 2 , _id_texture_fumee);
	matriceRGBACarreeToTexture3D(matriceRGBA_fire, tailleGrille + 2 , _id_texture_flamme);
	matricePerlinCarreeToTexture3D(perl, tailleGrille + 2 , _id_texture_perlin);
	VecteurPerlinTempsToTexture1D(perl_temps, 100 , _id_texture_perlin_temps);

	glBindTexture(GL_TEXTURE_3D, 0);	
	
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_3D , _id_texture_flamme );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_3D , _id_texture_fumee );
	glActiveTexture( GL_TEXTURE2 );
	glBindTexture(GL_TEXTURE_3D , _id_texture_perlin );
	glActiveTexture( GL_TEXTURE3 );
	glBindTexture(GL_TEXTURE_1D , _id_texture_perlin_temps );

	glActiveTexture( GL_TEXTURE0 );
	renderer->Bind_Program();
	GLuint location_tex1 = glGetUniformLocation( renderer->getProgramId(), "Texture0" );
	glUniform1i( location_tex1 , 0 );
    GLuint location_tex2 = glGetUniformLocation( renderer->getProgramId(), "Texture1" );
	glUniform1i( location_tex2 , 1 );
    GLuint location_texperl = glGetUniformLocation( renderer->getProgramId(), "Textureperlin" );
	glUniform1i( location_texperl , 2 );
	GLuint location_perlintemps = glGetUniformLocation( renderer->getProgramId(), "Temps" );
	glUniform1i( location_perlintemps , 3 );
	GLuint location_temps1 = glGetUniformLocation( renderer->getProgramId(), "temps1" );
	glUniform1f( location_temps1 , ((float) rand()) / RAND_MAX*tps1 );
	GLuint location_temps2 = glGetUniformLocation( renderer->getProgramId(), "temps2" );
	glUniform1f( location_temps2 , ((float) rand()) / RAND_MAX*tps2 );
	GLuint location_temps3 = glGetUniformLocation( renderer->getProgramId(), "temps3" );
	glUniform1f( location_temps3 , t );
	// Dessin
	dessinerPlansDansTexture3DFaceALaCamera(nb_plans, positionCamera, directionCamera);
	
	renderer->Unbind_Program();

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_3D , 0 );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture(GL_TEXTURE_3D , 0 );
	glActiveTexture( GL_TEXTURE2 );
	glBindTexture(GL_TEXTURE_3D , 0 );
	glActiveTexture( GL_TEXTURE3 );
	glBindTexture(GL_TEXTURE_1D , 0 );
	

}

void Fluid::majMatriceFlammeEnMatriceRGBA(){
	// Creation de la texture
	Vecteur4D *pointeurMatriceRGBA = matriceRGBA_fire;
	const float *pointeurMatriceACopier = s->getDensities();
	const float *pointeurMatriceACopier3 = s->getTemperatures();
	float R,G,B;
	float seuil = 0.001;
	
	for (int i = 0; i < (tailleGrille+2)*(tailleGrille+2)*(tailleGrille+2); i ++){
		tempIndex->getRGB( (1+(*pointeurMatriceACopier3))*1555 , &R, &G, &B );
		(*pointeurMatriceRGBA).x = R;		// R 
		(*pointeurMatriceRGBA).y = G;		// G
	   	(*pointeurMatriceRGBA).z = B;		// B
		// A
		if (*pointeurMatriceACopier < seuil) 
			(*pointeurMatriceRGBA).w = 0;
		else  	
			(*pointeurMatriceRGBA).w = 1/(*pointeurMatriceACopier3 * 13);
		// MAJ
		pointeurMatriceRGBA++;
		pointeurMatriceACopier++;
		pointeurMatriceACopier3++;
	}    
}
	
void Fluid::majMatriceFumeeEnMatriceRGBA(){
	// Creation de la texture
	Vecteur4D *pointeurMatriceRGBA = matriceRGBA_smoke;
	const float *pointeurMatriceACopier2 = s->getSmokes();
	for (int i = 0; i < (tailleGrille+2)*(tailleGrille+2)*(tailleGrille+2); i ++){
		(*pointeurMatriceRGBA).x = 0.4;		                  // R
		(*pointeurMatriceRGBA).y = 0.4;		                  // G
		(*pointeurMatriceRGBA).z = 0.4;		                  // B
		(*pointeurMatriceRGBA).w = *pointeurMatriceACopier2  /6;  // A
		// MAJ
		pointeurMatriceRGBA++;
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
		     0, GL_RGBA, GL_FLOAT, matrice);
}

void Fluid::initialiserRenduGPU(){
	// Creation des textures
	glGenTextures(1,&_id_texture_flamme);
	glGenTextures(1,&_id_texture_fumee);
	matriceRGBA_smoke = new Vecteur4D[(tailleGrille+2)*(tailleGrille+2)*(tailleGrille+2)];
	matriceRGBA_fire  = new Vecteur4D[(tailleGrille+2)*(tailleGrille+2)*(tailleGrille+2)];
	
	// init du shader de melange
	renderer = new Shader("./Shaders/multitex.vert","./Shaders/multitex.frag");
	GLuint location_tex1 = glGetUniformLocation( renderer->getProgramId(), "Texture0" );
	glUniform1i( location_tex1 , 0 );
       	GLuint location_tex2 = glGetUniformLocation( renderer->getProgramId(), "Texture1" );
	glUniform1i( location_tex2 , 1 );
}

void Fluid::dessinerPlansDansTexture3DFaceALaCamera(int nb_plans,
						    Vecteur3D& positionCamera, Vecteur3D& directionCamera)
{
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



	glPushMatrix();
	glScalef( tailleGrille*SolverParam::getSpaceDiv(),
		      tailleGrille*SolverParam::getSpaceDiv(),
		      tailleGrille*SolverParam::getSpaceDiv());
		  

	// Vecteur de coordonnées
	Vecteur3D coord;
	Vecteur3D decalageTex;
	Vecteur3D decalageVert;
	N = profondeur;
	
	// Affichage
	glBegin(GL_QUADS);
	for (int i = 0; i < nb_plans; i++){
		decalageTex  = - i/(float)nb_plans * N;//Vecteur3D(0,0,+ float(i)/nb_plans);// * N;
		decalageVert = - i/(float)nb_plans * N;

		coord = (Tex0 + decalageTex);		glTexCoord3d(coord.x, coord.y, coord.z);                
		coord = Boitev0 + decalageVert;		glVertex3d(coord.x, coord.y, coord.z);
		
		coord = (Tex1 + decalageTex);		glTexCoord3d(coord.x, coord.y, coord.z);        
		coord = Boitev1 + decalageVert;		glVertex3d(coord.x, coord.y, coord.z);
		
		coord = (Tex2 + decalageTex);  		glTexCoord3d(coord.x, coord.y, coord.z);
		coord = Boitev2 + decalageVert;		glVertex3d(coord.x, coord.y, coord.z);
                  
		coord = (Tex3 + decalageTex);		glTexCoord3d(coord.x, coord.y, coord.z);
		coord = Boitev3 + decalageVert;		glVertex3d(coord.x, coord.y, coord.z);
	}
	glEnd();
		
	glPopMatrix();
	
	// Desactivation des elements
	glDisable(GL_TEXTURE_3D);
	glDisable(GL_ALPHA_TEST);    
	glDisable( GL_BLEND );    
	glEnable(GL_LIGHTING);
}

void Fluid::dessinerPlansDansTexture3D(GLuint id_texture, int nb_plans){
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );
	glAlphaFunc(GL_GREATER,0.0f);
	glEnable(GL_ALPHA_TEST);
    
	glEnable(GL_TEXTURE_3D);
	glActiveTexture(id_texture);

	GLfloat verts[4][3] = { { 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 0.0, 0.0}};

	glBegin(GL_QUADS);	
	
	glTexCoord3d(verts[0][0], verts[0][1], verts[0][2]);	glVertex3d(verts[0][0], verts[0][1], verts[0][2]);
	glTexCoord3d(verts[1][0], verts[1][1], verts[1][2]);	glVertex3d(verts[1][0], verts[1][1], verts[1][2]);
	glTexCoord3d(verts[2][0], verts[2][1], verts[2][2]);	glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
	glTexCoord3d(verts[3][0], verts[3][1], verts[3][2]);	glVertex3d(verts[3][0], verts[3][1], verts[3][2]);
	
	for (int i = 1; i <= 2 * nb_plans; i++){
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
	
	glDisable(GL_TEXTURE_3D);
	glDisable( GL_BLEND );    
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void Fluid::Mise_A_Jour( float dt ){
    updateInfo(dt);
	s->velocitiesStepWithTemp(dt);
	s->densitiesStepWithTemp(dt);
}


void Fluid::updateInfo(float dt){
	s->clearFilledInfo();
    for(_it_objs = obj.begin(); _it_objs != obj.end(); ++_it_objs){
    	s->updateInfo(**_it_objs);
    	(**_it_objs).diffuserTemperature(dt);
    }
}
 






void Fluid::matricePerlinCarreeToTexture3D(const Vecteur3D *matrice, int cote, GLuint id_texture){
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

void Fluid::VecteurPerlinTempsToTexture1D(const Vecteur3D *matrice, int cote, GLuint id_texture){

	// Chargement en mémoire
	glBindTexture(GL_TEXTURE_1D, id_texture);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,cote,
		     0, GL_RGB, GL_FLOAT, matrice);

}

