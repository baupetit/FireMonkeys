#ifndef STEELBALL_H
#define STEELBALL_H

#include "Object.h"

#include <GL/gl.h>

class SteelBall : public Object {
protected :
	float radius ;
	
	GLuint drawList ;

};

#endif
