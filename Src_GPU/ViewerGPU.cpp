#include "ViewerGPU.h"

#include <cmath>
#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif


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
}
void ViewerGPU::rendu(){


	////////////////////////////////////////////////////////////////////////////
	//// OBJETS
	glPushMatrix();
    glMatrixMode (GL_MODELVIEW);
		
	//_fluid->Afficher_Entite_Face_Camera(positionCamera, directionCamera);
   	_fluid->Afficher_Entite();
   	
	glPopMatrix();
	
    
	////////////////////////////////////////////////////////////////////////////
}
