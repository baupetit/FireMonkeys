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
        s = new Solver(100);
	//s->setVelocity( 5,5,5,0,0.2,0);
}

SolverViewer::~SolverViewer(){
	delete s;
}

void SolverViewer::rendu(){
	
	static struct timespec old = get_real_time();
	static long frame  = 0 ;
	static float elapsed = 0 ;

	struct timespec cur = get_real_time();
	old = sub_timespec( &cur , &old );
	float dt = timespec_to_double( &old );
	old = cur ; 
	elapsed += dt ;

	// dessin de la grille
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
	//glClearColor( 0. , 0. , 0. , 0. );
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
				// cout << "d["<<i<<","<<j<<","<<k<<"] = " << c << endl ;
			}
		}
	}
	glEnd();
	
	s->densitiesStep( 1 , 0.15 );
	s->velocitiesStep( 0. , 0.15);

        	
	glutSwapBuffers();
	
	frame += 1;
	if( elapsed > 1. ){
		cout << " FPS : " << frame << endl;
		elapsed -= 1.;
		frame = 0 ;

		s->setDensity( N/2 , N/2 , N/2 , 10 );
		//s->setVelocity( N/2,N/2,N/2,0,0.0002,0);
		
	}
	
	sleep(1);
}
