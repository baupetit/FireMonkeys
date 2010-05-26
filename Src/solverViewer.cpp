#include "solverViewer.h"
#include <GL/glut.h>
#include <GL/gl.h>

SolverViewer::SolverViewer()
	:Viewer("Solver Viewer", 640, 480)
{
	s = new Solver(40);
}

SolverViewer::~SolverViewer(){
	delete s;
}

void SolverViewer::rendu(){
	// dessin de la grille
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
	glClearColor( 0. , 0. , 0. , 0. );

	const float *dens = s->getDensities();
	const int N = s->getSize();
	float c;

	glPointSize( 2.0f );
	glBegin( GL_POINTS );
	for( int k = 1; k<N ; k++ ){
		for( int j = 1; j<N ; j++ ){
			for( int i = 1; i<N ; i++ ){
				c = dens[IX(i,j,k)];
				glColor3f( c,c,c );
				glVertex3f( (float)i/N , (float)j/N , (float)k/N );
			}
		}
	}
	glEnd();
	
	glutSwapBuffers();
}
