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

	Voxel defBois( true,  // plein 
			false, // frontiere
			200.0,// restant 
			0.0,// temperature 
			Vecteur3D(0,0,0),  // repulsion
			0.8,// temp de combustion
			1.0,// taux de conversion
			1.0,// taux de perte
			0.1,// diffusion
			1.0, // conductance
			Vecteur3I(0,0,0));
			
	Voxel defExplosif( true,  // plein 
			false, // frontiere
			1500.0,// restant 
			0.0,// temperature 
			Vecteur3D(0,0,0),  // repulsion
			0.5,// temp de combustion
			1.0,// taux de conversion
			1500.0,// taux de perte
			1.0,// diffusion
			1.0, // conductance
			Vecteur3I(0,0,0));

	Voxel defMetal( true,  // plein 
			false, // frontiere
			100.0,// restant 
			0.0,// temperature 
			Vecteur3D(0,0,0),  // repulsion
			0.8,// temp de combustion
			0.0,// taux de conversion
			0.0,// taux de perte
			1.0,// diffusion
			1.0, // conductance
			Vecteur3I(0,0,0));
	
	Voxel defCire( true,  // plein 
			false, // frontiere
			100.0,// restant 
			0.0,// temperature 
			Vecteur3D(0,0,0),  // repulsion
			0.8,// temp de combustion
			4.0,// taux de conversion
			0.0,// taux de perte
			1.0,// diffusion
			1.0, // conductance
			Vecteur3I(0,0,0));

		   
	Sphere *sphereBois   = new Sphere( defBois,0.2, Vecteur4D( 0.5,0.2,0,0.8 ), 10, 10);
	Sphere *sphereMetal  = new Sphere( defBois,0.2, Vecteur4D( 0.4,0.4,0.4,0.8 ), 10, 10);
	Sphere *sphereCire   = new Sphere( defExplosif,0.2, Vecteur4D( 0.6,0.6,0.0,0.8 ), 10, 10);
	
	sphereBois->translate( Vecteur3D(-5, -5, 0 ));
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









