#include "Object.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

Object::Object(){
	defVox = Voxel();
	grille = 0;
}

Object::Object( Voxel defVox )
	:defVox(defVox)
{
	grille = 0;
}

/* destructor */
Object::~Object(){
	if( grille ) delete [] grille;
}

/* heat diffusion */
void Object::diffuserTemperature( float dt ){
}

/* herited methods */
void Object::Afficher_Face_Camera(Vecteur3D& positionCamera,
				  Vecteur3D& directionCamera,
				  float dt ) {
	Afficher( dt );
}

void Object::Afficher( float dt ){
	int i,j,k,l ;
	
	glColor4f( color.x, color.y, color.z, color.w );
	glBegin( GL_TRIANGLES );
	for( k=0; k<grilleSize.z; ++k){
		for( j=0; j<grilleSize.y; ++j){
			for( i=0; i<grilleSize.x; ++i){
				Voxel v = grille[_Grille_Ind(i,j,k)];
				for( l = 0 ; l < v.nb_faces; ++l ){
					v.faces[l].drawTriangle();
				}
			}
		}
	}
	glEnd();
}
