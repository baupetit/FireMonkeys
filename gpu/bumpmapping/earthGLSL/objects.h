#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#include <GL/glew.h> // must be included before any OpenGL related file
#include <vector>
#include <QGLViewer/vec.h>
#include "glsl.h"

// Documentation!
// --------------
//
// 3D positions and vectors are stored in qglviewer::Vec data structures
// Ref: qglviewer::Vec ->
// http://artis.imag.fr/Members/Gilles.Debunne/QGLViewer/refManual/classqglviewer_1_1Vec.html


/** 
 * Simple quad colored object
 * 
 */
class Quad
{
 public:
  void draw();
};

/** 
 * Traditional QGLViewer Spiral
 * 
 */
class Spiral
{
 public:
  void draw();
};

/** 
 * Simple cube object
 * 
 */
class Cube {

 protected:
  std::vector<qglviewer::Vec>      _vertices;  /**< 3D positions */
  std::vector<std::vector<int> >   _triangles; /**< vertex indices */
  std::vector<qglviewer::Vec>      _normalsT;  /**< 3D normals */

 public:
  Cube();
  //~Cube(){};
  void draw();

};


/** 
 * Sphere object
 * 
 * @param radius 
 * @param nPhi 
 * @param nTheta 
 */
class Sphere {
  
protected:
  std::vector<std::vector<int> >   _triangles;  /**< vertex indices */
  std::vector<qglviewer::Vec>      _vertices;   /**< 3D positions */
  std::vector<qglviewer::Vec>      _normalsV;   /**< 3D normals */
  std::vector<qglviewer::Vec>      _tangentsV;  /**< 3D tangent to surface */
  std::vector<std::vector<float> > _texCoordsV; /**< 2D texture coordinates */

public:
  Sphere(float radius=1., int nPhi=20, int nTheta=40);
  void draw(glShader* shader=NULL);
};


/** 
 * Torus object
 * 
 * @param nPhi 
 * @param nTheta 
 * @param outerRadius 
 * @param innerRadius 
 */
class Torus {
  
protected:
  std::vector<std::vector<int> >   _triangles;  /**< vertex indices */
  std::vector<qglviewer::Vec>      _vertices;   /**< 3D positions */
  std::vector<qglviewer::Vec>      _normalsV;   /**< 3D normals */
  std::vector<qglviewer::Vec>      _tangentsV;  /**< 3D tangent to surface */
  std::vector<std::vector<float> > _texCoordsV; /**< 2D texture coordinates */
  
public:
  Torus(int nPhi=30, int nTheta=40, float outerRadius=0.66, float innerRadius=0.34);
  void draw(glShader* shader=NULL);
};



#endif
