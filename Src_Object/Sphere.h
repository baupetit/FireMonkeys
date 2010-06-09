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
	
	virtual void generateVoxels() ;
	void         generateDisplayList(int nb_plans, int nb_quarts);

	inline bool isInside( Vecteur3D p ){
		Vecteur3D dist = position - p;
		if(norme( dist ) < radius) {
			return true ;
		}
		return false ;
	}
	
	inline void setValuation( Voxel& v ){
		for( int i = 0 ; i<8 ; ++i ){
			Vecteur3D dist = position - v.corner[i];
			v.valuation[i] = norme( dist ) - radius;
		}
	}
public :
	Sphere( Voxel defVox,
		float radius, Vecteur4D color, 
		int nb_plans, int nb_quarts);
	~Sphere();
	
	//virtual void Afficher(float dt);

};

#endif
