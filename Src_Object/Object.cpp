#include "Object.h"

Object::Object(){
	grille = 0;
}

/* destructor */
Object::~Object(){
	if( grille ) delete [] grille;
}

/* herited methods */
void Object::Afficher_Face_Camera(Vecteur3D& positionCamera,
				  Vecteur3D& directionCamera,
				  float dt ) {
	Afficher( dt );
}


