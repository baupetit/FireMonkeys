#ifndef UtilsGL_h
#define UtilsGL_h

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#ifdef WIN32
#include <GL/wglew.h>
#endif

#include <GL/glu.h>

#include <iostream>

//#define USE_OPENGLUT

#ifdef USE_OPENGLUT
#include <GL/openglut.h>

#ifdef _WIN32
#pragma comment (lib,"openglut.lib")
#endif

#else
#include <GL/glut.h>
#endif

#include "Vector.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define INT_INDICE_BUG 32768
#define PI 3.1415926535897932384626433832795


namespace NemoGraphics {

inline std::string operator +(const std::string &str, const float f){
	char buff[4096];

	sprintf(buff, "%s%f", str.c_str(), f);

	return buff;
}

inline std::string operator +(const std::string &str, const int f){
	char buff[4096];

	sprintf(buff, "%s%d", str.c_str(), f);

	return buff;
}

inline int GLGetFormatSize(GLuint format){
	switch(format){
		case GL_RGBA8:
		case GL_RGBA:
		case GL_BGRA:
			return 4;
		case GL_RGB8:
		case GL_RGB:
			return 3;
		case GL_RGBA16F_ARB:
			return 8;
		case GL_RGB16F_ARB:
			return 6;
		case GL_RGBA32F_ARB:
			return 16;
		case GL_RGB32F_ARB:
			return 12;
		default:
			return -1;
	}
}

//get gl types from c++
template<class T>
inline int GLGetGLType(){
	return 0;
}

template<>
inline int GLGetGLType<GLfloat>(){
	return GL_FLOAT;
}

template<>
inline int GLGetGLType<GLhalf>(){
	return GL_HALF_FLOAT_NV;
}

template<>
inline int GLGetGLType<Vector4<GLfloat>>(){
	return GL_FLOAT;
}

template<>
inline int GLGetGLType<Vector4<GLhalf>>(){
	return GL_HALF_FLOAT_NV;
}


/////////Gestion des extensions
class GLExtensionsManager {
	static bool initialized;
public:
	static int initialize();
	static int checkSupported(const char *exts);
};

//////////////////////////////
inline void drawCube(Vector3f &p1, Vector3f &p2) {

glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(p1.x, p1.y,  p2.z);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( p2.x, p1.y,  p2.z);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( p2.x,  p2.y,  p2.z);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(p1.x,  p2.y,  p2.z);	// Top Left Of The Texture and Quad
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(p1.x, p1.y, p1.z);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(p1.x,  p2.y, p1.z);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( p2.x,  p2.y, p1.z);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( p2.x, p1.y, p1.z);	// Bottom Left Of The Texture and Quad
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(p1.x,  p2.y, p1.z);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(p1.x,  p2.y,  p2.z);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( p2.x,  p2.y,  p2.z);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( p2.x,  p2.y, p1.z);	// Top Right Of The Texture and Quad
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(p1.x, p1.y, p1.z);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( p2.x, p1.y, p1.z);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( p2.x, p1.y,  p2.z);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(p1.x, p1.y,  p2.z);	// Bottom Right Of The Texture and Quad
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( p2.x, p1.y, p1.z);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( p2.x,  p2.y, p1.z);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( p2.x,  p2.y,  p2.z);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( p2.x, p1.y,  p2.z);	// Bottom Left Of The Texture and Quad
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(p1.x, p1.y, p1.z);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(p1.x, p1.y,  p2.z);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(p1.x,  p2.y,  p2.z);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(p1.x,  p2.y, p1.z);	// Top Left Of The Texture and Quad
	glEnd();

}

};

#endif