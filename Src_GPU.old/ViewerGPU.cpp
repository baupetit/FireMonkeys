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
	_fluid -> initialiserSpeedField();
}
void ViewerGPU::rendu(){
	if ( _fluid!= 0 ) _fluid -> resolutionSpeedField();
	//if ( _fluid!= 0 ) _fluid -> displaySpeedField();	
}
