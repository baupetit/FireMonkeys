#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vecteur.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

struct Triangle {
	Vecteur3D s1;
	Vecteur3D s2;
	Vecteur3D s3;

	inline void drawTriangle() const {
		glVertex3f( s1.x, s1.y, s1.z );
		glVertex3f( s2.x, s2.y, s2.z );
		glVertex3f( s3.x, s3.y, s3.z );
	}
};

#endif