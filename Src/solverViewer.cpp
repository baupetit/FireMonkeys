#include "solverViewer.h"

#include "temp.h"

#include <GL/glut.h>
#include <GL/gl.h>

#include <iostream>
#include <unistd.h>

using namespace std;

SolverViewer::SolverViewer()
        :Viewer("Solver Viewer", 640, 480)
{
	int N = 20;
        s = new Solver(N);
	s->setVelocity( N/2,1,N/2,0,100,0);

}

SolverViewer::~SolverViewer(){
	delete s;
}

void SolverViewer::rendu(){
	
	static struct timespec old = get_real_time();
	static long frame  = 0 ;
	static float elapsed = 0 ;
	static int toto= 0;

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
	glVertex3f( 10,0,0 );
	// y
	glColor3f( 0,1,0);
	glVertex3f( 0,0,0);
	glVertex3f( 0,10,0 );
	// z
	glColor3f( 0,0,1);
	glVertex3f( 0,0,0);
	glVertex3f( 0,0,10 );
	glEnd();

	////////////////////////////////////////////////////////////////////////////
	//// DENSITE
	// dessin de la grille de densite
	glClearColor( 1. , 0. , 1. , 0. );
		  
	const float *dens = s->getDensities();
	const int N = s->getSize();
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
	////////////////////////////////////////////////////////////////////////////
		
	s->densitiesStep( 0.0, dt );
	s->velocitiesStep( 0.0 , dt);

	frame += 1;
	if( elapsed > 1. ){
		cout << " FPS : " << frame << endl;
		elapsed -= 1.;
		frame = 0 ;
		if( toto < 10 ){
			s->setDensity( N/2 , 5 , N/2 , 0.5 );
			s->setVelocity( N/2,1,N/2,0,100,0);
			toto +=1;
		}			
	}
}
