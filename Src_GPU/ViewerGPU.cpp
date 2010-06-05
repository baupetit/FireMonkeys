#include "ViewerGPU.h"

#include <cmath>
#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif

#include <unistd.h>

#include "temp.h"


ViewerGPU::ViewerGPU(const string & title, int width, int height,
	           int initPositionX, int initPositionY) 
	       : Viewer(title, width, height, initPositionX, initPositionY) {
    initScene();	       
}


ViewerGPU::~ViewerGPU(){
    delete _fluid;
}
	
	


void ViewerGPU::initScene(){
	cout << "Initialisation de la Scene " << endl;
	_fluid = new Fluid_GPU();
	_fluid -> initialiserFluid();
	cout << "Fin de l'initialisation de la Scene " << endl;
	
}


void ViewerGPU::rendu(){


	////////////////////////////////////////////////////////////////////////////
	//// TIME
	static struct timespec old = get_real_time();
	static long frame  = 0 ;
	static float elapsed = 0 ;

	struct timespec cur = get_real_time();
	old = sub_timespec( &cur , &old );
	float dt = timespec_to_double( &old );
	old = cur ; 
	elapsed += dt ;


	frame += 1;
	if( elapsed > 1. ){
		cout << " FPS : " << frame << endl;
		elapsed -= 1.;
		frame = 0 ;
	}
	

	////////////////////////////////////////////////////////////////////////////
	//// OBJETS
	glPushMatrix();
    glMatrixMode (GL_MODELVIEW);
		
	//_fluid->Afficher_Entite_Face_Camera(positionCamera, directionCamera);
   	_fluid->Afficher_Entite();
   	
	glPopMatrix();
	
    
	////////////////////////////////////////////////////////////////////////////
}
