#include "FluidViewer.h"

#include "temp.h"


#include <iostream>
#include <unistd.h>

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
	_fluid->Afficher();
	
    
	////////////////////////////////////////////////////////////////////////////
	//// TIME
	// 
    
	frame += 1;
	if( elapsed > 1. ){
		cout << " FPS : " << frame << endl;
		elapsed -= 1.;
		frame = 0 ;
	    /*
		if( toto < 10 ){
			s->setDensity( N/2 , 5 , N/2 , 0.5 );
			s->setVelocity( N/2,1,N/2,0,100,0);
			toto +=1;
		}	
	    */		
	}
}












