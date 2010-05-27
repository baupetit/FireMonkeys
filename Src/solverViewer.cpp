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

        tailleSolver = 30;
        
        s = new Solver(tailleSolver);
        initialiserRenduGPU();
        
        matriceRGBA = new float[4*(tailleSolver+2)*(tailleSolver+2)*(tailleSolver+2)];
}

SolverViewer::~SolverViewer(){
	delete s;
	delete matriceRGBA;
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

    ////////////////////////////////////////////////////////////////////////////
    //// DENSITE
	// dessin de la grille de densite
	/*
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
				cout << "i,j,k : "<< i << " , " << j << " , " << k << "densite : " << c << endl;
				glColor4f( c,c,c, 0.5 );
				
				glVertex3f( i/(float)N , j/(float)N , k/(float)N );
				// cout << "d["<<i<<","<<j<<","<<k<<"] = " << c << endl ;
			}
		}
	}
	glEnd();
	*/
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
    
    renduFumeeGPU();
    
	
	s->densitiesStep( 0.2 , dt/10 );
	s->velocitiesStep( 0.1 , dt/10);

        	
        	
	
	frame += 1;
	if( elapsed > 1. ){
		cout << " FPS : " << frame << endl;
		elapsed -= 1.;
		frame = 0 ;

		s->setDensity( 5 , 5 , 5 , 10 );


		//s->setVelocity( N/2,N/2,N/2,0,0.0002,0);
		
	}
	
}





	
void SolverViewer::renduFumeeGPU(){
    majMatriceFumeeEnMatriceRGBA();
    matriceRGBACarreeToTexture3D(matriceRGBA, tailleSolver + 2 , _id_texture_fumee);
    dessinerPlansDansTexture3D(_id_texture_fumee, 5);
}



void SolverViewer::renduFlammeGPU(){
}


void SolverViewer::majMatriceFumeeEnMatriceRGBA(){

    // Creation de la texture
    float *pointeurMatriceRGBA = matriceRGBA;
    const float *pointeurMatriceACopier = s->getDensities();
    for (int i = 0; i < (tailleSolver+2)*(tailleSolver+2)*(tailleSolver+2); i ++){
        // R
        *pointeurMatriceRGBA = *pointeurMatriceACopier;
        pointeurMatriceRGBA++;
        // G
        *pointeurMatriceRGBA = *pointeurMatriceACopier;
        pointeurMatriceRGBA++;
        // B
        *pointeurMatriceRGBA = *pointeurMatriceACopier;
        pointeurMatriceRGBA++;
        // A
        *pointeurMatriceRGBA = *pointeurMatriceACopier;
        pointeurMatriceRGBA++;
        // MAJ
        pointeurMatriceACopier++;
    }

}
	

void SolverViewer::matriceRGBACarreeToTexture3D(const float *matrice, int cote, GLuint id_texture){

    // Chargement en mémoire
    glBindTexture(GL_TEXTURE_3D, id_texture);
    
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    
    glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,cote,cote,cote,
                 0, GL_RGBA, GL_FLOAT, matrice);
                 
}



void SolverViewer::initialiserRenduGPU(){
    glGenTextures(1,&_id_texture_flamme);
    glGenTextures(1,&_id_texture_fumee);
}





void SolverViewer::dessinerPlansDansTexture3D(GLuint id_texture, int nb_plans){

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );
    glAlphaFunc(GL_GREATER,0.1f);
    glEnable(GL_ALPHA_TEST);
    
	glEnable(GL_TEXTURE_3D);
    glActiveTexture(id_texture);

    GLfloat verts[4][3] = { { 0.0, 0.0, 0.5}, {0.0, 1.0, 0.5}, {1.0, 1.0, 0.5}, {1.0, 0.0, 0.5}};
    

	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2]);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]);
		
		glTexCoord3d(verts[1][2], verts[1][1], verts[1][2]);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2]);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2]);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2]);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2]);
		
		glTexCoord3d(verts[3][2], verts[3][1], verts[3][2]);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2]);
		
		glTexCoord3d(verts[0][2], verts[0][1], verts[0][2]);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]);		
	glEnd();
	
	for (int i = 1; i <= nb_plans/2; i++){
	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/nb_plans);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]+ (float)i/nb_plans);
		
		glTexCoord3d(verts[1][2], verts[1][1], verts[1][2] + (float)i/nb_plans);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2] + (float)i/nb_plans);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2] + (float)i/nb_plans);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/nb_plans);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2] + (float)i/nb_plans);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] + (float)i/nb_plans);
		
		glTexCoord3d(verts[3][2], verts[3][1], verts[3][2] + (float)i/nb_plans);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2] + (float)i/nb_plans);
		
		glTexCoord3d(verts[0][2], verts[0][1], verts[0][2] + (float)i/nb_plans);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2] + (float)i/nb_plans);	
	glEnd();
	}
	
	
	for (int i = 1; i <= nb_plans/2; i++){
	glBegin(GL_TRIANGLES);	
		glTexCoord3d(verts[0][0], verts[0][1], verts[0][2] - (float)i/nb_plans);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2]- (float)i/nb_plans);
		
		glTexCoord3d(verts[1][2], verts[1][1], verts[1][2] - (float)i/nb_plans);
		glVertex3d(verts[1][0], verts[1][1], verts[1][2] - (float)i/nb_plans);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2] - (float)i/nb_plans);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] - (float)i/nb_plans);
		
		glTexCoord3d(verts[2][2], verts[2][1], verts[2][2] - (float)i/nb_plans);
		glVertex3d(verts[2][0], verts[2][1], verts[2][2] - (float)i/nb_plans);
		
		glTexCoord3d(verts[3][2], verts[3][1], verts[3][2] - (float)i/nb_plans);
		glVertex3d(verts[3][0], verts[3][1], verts[3][2] - (float)i/nb_plans);
		
		glTexCoord3d(verts[0][2], verts[0][1], verts[0][2] - (float)i/nb_plans);
		glVertex3d(verts[0][0], verts[0][1], verts[0][2] - (float)i/nb_plans);	
	glEnd();
	}
}








