#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include "SolverParam.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

class Sphere : public Object {
protected :
	float radius ;
	
	/* ogl attribs */
	GLuint drawList ;
	Vecteur4D color ;
	
	virtual void generateVoxels() ;
	void         generateDisplayList(int nb_plans, int nb_quarts);

	inline bool isInside( Vecteur3D p ){
		Vecteur3D dist = position - p;
		if(norme( dist ) < radius) {
			return true ;
		}
		return false ;
	}

public :
	Sphere( float radius, Vecteur4D color, 
		int nb_plans, int nb_quarts);
	~Sphere();
	
	virtual void Afficher(float dt);

};

#endif
