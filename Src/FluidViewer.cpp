#include "FluidViewer.h"

#include "temp.h"


#include <iostream>
#include <unistd.h>


#include <cmath>
#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif


using namespace std;

FluidViewer::FluidViewer()
        :Viewer("Solver Viewer", 800, 600)
{
       _fluid = new Fluid();
}

FluidViewer::~FluidViewer(){
	delete _fluid;
}

void FluidViewer::rendu(){
	////////////////////////////////////////////////////////////////////////////
	//// TIME
	// 
	static struct timespec old = get_real_time();
	static long frame  = 0 ;
	static float elapsed = 0 ;

	struct timespec cur = get_real_time();
	old = sub_timespec( &cur , &old );
	float dt = timespec_to_double( &old );
	old = cur ; 
	elapsed += dt ;

	////////////////////////////////////////////////////////////////////////////
	//// AXIS
	// 
	glBegin( GL_LINES );
	// x
	glColor3f( 1,0,0);
	glVertex3f( 0,0,0);
	glVertex3f( 1,0,0 );
	// y
	glColor3f( 0,1,0);
	glVertex3f( 0,0,0);
	glVertex3f( 0,1,0 );
	// z
	glColor3f( 0,0,1);
	glVertex3f( 0,0,0);
	glVertex3f( 0,0,1 );
	glEnd();

	////////////////////////////////////////////////////////////////////////////
	//// OBJETS
	glPushMatrix();
	glMatrixMode (GL_MODELVIEW);
	Vecteur3D positionCamera;
	positionCamera.x = _distToOrigin*cos(_cameraAngle);
	positionCamera.y = _cameraHeight;
	positionCamera.z = _distToOrigin*sin(_cameraAngle);
	Vecteur3D directionCamera;
	directionCamera.x = -positionCamera.x;
	directionCamera.y = 0;
	directionCamera.z = -positionCamera.z;
	_fluid->Afficher_Entite_Face_Camera(positionCamera, directionCamera,dt);
	glPopMatrix();
	
	////////////////////////////////////////////////////////////////////////////
	//// TIME
	// 
	frame += 1;
	if( elapsed > 1. ){
		cout << " FPS : " << frame << endl;
		elapsed -= 1.;
		frame = 0 ;
	}
}












