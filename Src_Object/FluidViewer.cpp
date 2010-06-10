#include "FluidViewer.h"

#include "temp.h"


#include <iostream>
#include <unistd.h>


#include <cmath>
#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif
using namespace std;


#include "Sphere.h"
#include "Voxel.h"

FluidViewer::FluidViewer()
        :Viewer("Solver Viewer", 800, 600)
{   
	Voxel defBois( true, 
		   false, 
		   10000.0,
		   0,
		   Vecteur3D(0,0,0), 
		   0.0010,
		   0.0010,
		   100,
		   1.0,
		   1.0, 
		   Vecteur3I(0,0,0));

	Voxel defMetal( true, 
		   false, 
		   10,
		   10000,
		   Vecteur3D(0,0,0), 
		   0.0,
		   0.0,
		   0,
		   0.0010,
		   10, 
		   Vecteur3I(0,0,0));
<<<<<<< HEAD
		   
	Voxel defCire( true, 
		   false, 
		   10,
		   50,
		   Vecteur3D(0,0,0), 
		   0.0010,
		   0.0,
		   0,
		   1000.1,
		   1000.1, 
		   Vecteur3I(0,0,0));
		   
	Sphere *sphereBois   = new Sphere( defBois,0.8, Vecteur4D( 0,1,0,0.8 ), 10, 10);
	Sphere *sphereMetal  = new Sphere( defMetal,0.8, Vecteur4D( 1,0,0,0.8 ), 10, 10);
	Sphere *sphereCire   = new Sphere( defCire,0.8, Vecteur4D( 0,0,1,0.8 ), 10, 10);
	
	sphereBois->translate( Vecteur3D(-5, -5, 0 ));
=======


	Sphere *sphereBois   = new Sphere( def,1, Vecteur4D( 0,1,0,0.8 ), 10, 10);
	Sphere *sphereMetal  = new Sphere( def,1, Vecteur4D( 1,0,0,0.8 ), 10, 10);
	Sphere *sphereCire   = new Sphere( def,1, Vecteur4D( 0,0,1,0.8 ), 10, 10);
	sphereBois->translate( Vecteur3D(-5, -5, -5 ));
>>>>>>> de118257fbdbc219943e4abca08dee4d49745581
	sphereMetal->translate(Vecteur3D( -5, 0, 0 ));
	sphereCire->translate(Vecteur3D( -5, 5, 0 ));
	
	_objs.push_back(sphereBois);           
	_objs.push_back(sphereMetal);           
	_objs.push_back(sphereCire);           
	_fluid = new Fluid(_objs);
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



    // deplacement de l'objet
    _it_objs = _objs.begin();
    if (selectBois)
    {
        (*_it_objs)->translate( Vecteur3D (Viewer::sourisDX, -Viewer::sourisDY, 0.0));
        (*_it_objs)->updateVitesse();
    }
    if (selectCire)
    {
        _it_objs++;
        (*_it_objs)->translate( Vecteur3D (Viewer::sourisDX, -Viewer::sourisDY, 0.0));
        (*_it_objs)->updateVitesse();
    }
    if (selectMetal)
    {
        _it_objs++;
        _it_objs++;
        (*_it_objs)->translate( Vecteur3D (Viewer::sourisDX, -Viewer::sourisDY, 0.0));
        (*_it_objs)->updateVitesse();
    }
    

    // affichage objets
    for(_it_objs = _objs.begin(); _it_objs != _objs.end(); ++_it_objs){
    	(*_it_objs)->Afficher_Entite(0);
    }
    
    // affichage et maj fluide
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









