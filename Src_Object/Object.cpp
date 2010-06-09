#include "Object.h"

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
