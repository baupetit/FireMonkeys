#include "objects.h"

#include <iostream>
#include <algorithm>
#include <iterator>

//----------------------------------------------------------------------
// Quad
//----------------------------------------------------------------------

/** 
 * Quad draw Function
 * 
 */
void Quad::draw()
{
  // Draws a z plane
  glBegin(GL_QUADS);
  glNormal3f(0,0,1);
  glColor3f(0,0,0); glVertex3f(-1,-1,0);
  glColor3f(1,0,0); glVertex3f(1,-1,0);
  glColor3f(1,1,0); glVertex3f(1,1,0);
  glColor3f(0,1,0); glVertex3f(-1,1,0);
  glEnd();
}

//----------------------------------------------------------------------
// Spiral
//----------------------------------------------------------------------

/** 
 * Spiral draw function
 * 
 */
void Spiral::draw()
{
  // Draws a spiral
  const float nbSteps = 200.0;
  glBegin(GL_QUAD_STRIP);
  for (float i=0; i<nbSteps; ++i)
    {
      float ratio = i/nbSteps;
      float angle = 21.0*ratio;
      float c = cos(angle);
      float s = sin(angle);
      float r1 = 1.0 - 0.8*ratio;
      float r2 = 0.8 - 0.8*ratio;
      float alt = ratio - 0.5;
      const float nor = .5;
      const float up = sqrt(1.0-nor*nor);
      glColor3f(1-ratio, .2 , ratio);
      glNormal3f(nor*c, up, nor*s);
      glVertex3f(r1*c, alt, r1*s);
      glVertex3f(r2*c, alt+0.05, r2*s);
    }
  glEnd();
}

//----------------------------------------------------------------------
// Cube
//----------------------------------------------------------------------

/** 
 * Cube constructor
 * 
 */
Cube::Cube()
{
  // vertex coordinates
  static const GLfloat ps[8][3] = {{-1,-1,-1},{-1,1,-1},{1,1,-1},{1,-1,-1},
				   {-1,-1, 1},{-1,1, 1},{1,1, 1},{1,-1, 1}};

  // triangles vertices
  static const GLint ts[12][3] = {
    // bottom triangles
    {1,2,0},{3,0,2},
    // side triangles
    {1,5,6},{2,1,6},{2,6,3},{3,6,7},{3,7,0},{0,7,4},{0,4,1},{1,4,5},
    // top triangles
    {4,7,5},{7,6,5}
  };
  
  // normals components
  static const GLfloat ns[12][3] = {
    // bottom triangles
    {0,0,-1},{0,0,-1},
    // side triangles
    {0,1,0},{0,1,0},{1,0,0},{1,0,0},{0,-1,0},{0,-1,0},{-1,0,0},{-1,0,0},
    // top triangles
    {0,0,1},{0,0,1}
  };

  // Fill vertices, triangles and normalsT vectors
  int i;
  for (i=0; i<8 ; ++i)  _vertices.push_back(qglviewer::Vec(ps[i]));
  for (i=0; i<12; ++i) _triangles.push_back(std::vector<int>(ts[i], ts[i+1]));
  for (i=0; i<12; ++i)  _normalsT.push_back(qglviewer::Vec(ns[i]));
  

  // Debug information
  bool debug = false;

  if (debug) {
    
    using namespace std;  // so that writing "std::" is no more necessary
    
    cout << "Display Cube information" << endl;
    
    // display components of the first point
    cout << _vertices[0] << endl;
    
    // display indices of the first triangle
    copy(_triangles[0].begin(), _triangles[0].end(), 
	 ostream_iterator<int>(cout, "\t"));
    cout << endl;
    // Ref STL: std::copy ->
    // http://www.sgi.com/tech/stl/copy.html

    // Ref STL: std::ostream_iterator<T> ->
    // http://www.sgi.com/tech/stl/ostream_iterator.html

    // display components of the firt normal
    cout << _normalsT[0] << endl;
  }

}

/** 
 * Cube draw function
 * 
 */
void Cube::draw()
{
  int t; // iterator on each triangle;
  int i; // iterator on each vertex indice;

  
  // scale it so that it is a unit cube
  glPushMatrix();
  glScalef(0.5, 0.5, 0.5);

  // for each triangle
  for(t=0; t<12; ++t) {
    
    //printf("triangle %d\n", t);
    
    // start drawing triangles
    glBegin(GL_TRIANGLES);

    // send normals to OpenGL
    glNormal3fv(_normalsT[t]);
    
    //std::cout << "normal: " << _normalsT[t] << std::endl;

    // for each vertex index for the current triangle
    for(i=0; i<3; i++) {
  
      // send vertices components to OpenGL
      glVertex3fv(_vertices[_triangles[t][i]]);

      //std::cout << "vertex: " << _vertices[_triangles[t][i]] << std::endl;
    
    } // end for each vertex
    
    // stop drawing triangles
    glEnd();
 
  } // end for each triangle

  glPopMatrix();

}


//----------------------------------------------------------------------
// Sphere
//----------------------------------------------------------------------

/** 
 * Sphere constructor
 * 
 */
Sphere::Sphere(float radius, int nPhi, int nTheta)
{
  float phiInc   = M_PI/(nPhi-1.);
  float thetaInc = 2.*M_PI/(nTheta-1.);
  
  float phi, theta;
  float x,y,z, nx,ny,nz, tx,ty,tz ,s,t;
  int i, j;
  
  for (j=0; j<nPhi; j++) {

    phi = -M_PI/2. + phiInc * j; 
      
    for (i=0; i<nTheta; i++) {
      
      theta = thetaInc * i;

      // position
      x = radius*cos(theta)*cos(phi);
      y = radius*sin(theta)*cos(phi);
      z = radius*sin(phi);
      
      // normal
      nx = cos(theta) * cos(phi);
      ny = sin(theta) * cos(phi);
      nz = sin(phi);

      // tangent
      tx = -y;
      ty = x;
      tz = 0;

      // texture coordinates
      s = i/float(nTheta-1.);
      t = j/float(nPhi-1.);
      std::vector<float> tc;
      tc.push_back(s);
      tc.push_back(t);

      _vertices.push_back  (qglviewer::Vec(x,y,z));
      _normalsV.push_back  (qglviewer::Vec(nx,ny,nz));
      _tangentsV.push_back (qglviewer::Vec(tx,ty,tz));
      _texCoordsV.push_back(tc);
    }    
  }
  
  for (j=0; j<nPhi-1; j++) {
    for (i=0; i<nTheta-1; i++) {
      
      std::vector<int> tri1, tri2;

      tri1.push_back(j*nTheta + i);
      tri1.push_back(((j+1)%nPhi)*nTheta + i);
      tri1.push_back(((j+1)%nPhi)*nTheta + (i+1)%nTheta);

      tri2.push_back(j*nTheta + i);
      tri2.push_back(((j+1)%nPhi)*nTheta + (i+1)%nTheta);
      tri2.push_back(j*nTheta + (i+1)%nTheta);
   
      _triangles.push_back(tri1);
      _triangles.push_back(tri2);      
    }
  }

  //std::cout << "nb triangles " << _triangles.size() << std::endl;

}

/** 
 * Sphere draw function
 * 
 */
void Sphere::draw(glShader* shader) 
{
  size_t t,i;
  
  for (t=0; t<_triangles.size(); ++t) {
    
    std::vector<int> indices = _triangles[t];
    
    glBegin(GL_TRIANGLES);

    for (i=0; i<indices.size(); ++i) {
      
      int index = indices[i];
      
      qglviewer::Vec& position = _vertices  [index];
      qglviewer::Vec& normal   = _normalsV  [index];
      qglviewer::Vec& tangent  = _tangentsV [index];
      std::vector<float>& texc = _texCoordsV[index];

      if (shader) shader->setVertexAttrib3f(1, tangent.x,tangent.y,tangent.z);
      glTexCoord2f(texc[0], texc[1]);
      glNormal3fv(normal);
      glVertex3fv(position);
      
    }

    glEnd();

  }
  
}

//----------------------------------------------------------------------
// Torus
//----------------------------------------------------------------------

/** 
 * Torus constructor
 * 
 */
Torus::Torus(int nPhi, int nTheta, float outerRadius, float innerRadius)
{
  float phiInc   = 2.*M_PI/(nPhi-1.);
  float thetaInc = 2.*M_PI/(nTheta-1.);
  
  float phi, theta;
  float x,y,z, nx,ny,nz, tx,ty,tz ,s,t;
  int i, j;
  
  for (j=0; j<nPhi; j++) {

    phi = phiInc * j; 
      
    for (i=0; i<nTheta; i++) {
      
      theta = thetaInc * i;

      // position
      x = cos(theta) * (innerRadius*cos(phi) + outerRadius);
      y = sin(theta) * (innerRadius*cos(phi) + outerRadius);
      z = innerRadius*sin(phi);
      
      // normal
      nx = cos(theta) * cos(phi);
      ny = sin(theta) * cos(phi);
      nz = sin(phi);

      // tangent
      tx = -y;
      ty = x;
      tz = 0;

      // texture coordinates
      s = i/float(nTheta-1.);
      t = j/float(nPhi-1.);
      std::vector<float> tc;
      tc.push_back(s);
      tc.push_back(t);

      _vertices.push_back  (qglviewer::Vec(x,y,z));
      _normalsV.push_back  (qglviewer::Vec(nx,ny,nz));
      _tangentsV.push_back (qglviewer::Vec(tx,ty,tz));
      _texCoordsV.push_back(tc);
    }    
  }
  
  for (j=0; j<nPhi-1; j++) {
    for (i=0; i<nTheta-1; i++) {
      
      std::vector<int> tri1, tri2;

      tri1.push_back(j*nTheta + i);
      tri1.push_back(((j+1)%nPhi)*nTheta + i);
      tri1.push_back(((j+1)%nPhi)*nTheta + (i+1)%nTheta);

      tri2.push_back(j*nTheta + i);
      tri2.push_back(((j+1)%nPhi)*nTheta + (i+1)%nTheta);
      tri2.push_back(j*nTheta + (i+1)%nTheta);
   
      _triangles.push_back(tri1);
      _triangles.push_back(tri2);      
    }
  }

  //std::cout << "nb triangles " << _triangles.size() << std::endl;

}

/** 
 * Torus draw function
 * 
 */
void Torus::draw(glShader* shader) 
{
  size_t t,i;
  
  for (t=0; t<_triangles.size(); ++t) {
    
    std::vector<int> indices = _triangles[t];
    
    glBegin(GL_TRIANGLES);

    for (i=0; i<indices.size(); ++i) {
      
      int index = indices[i];
      
      qglviewer::Vec& position = _vertices  [index];
      qglviewer::Vec& normal   = _normalsV  [index];
      qglviewer::Vec& tangent  = _tangentsV [index];
      std::vector<float>& texc = _texCoordsV[index];

      if (shader) shader->setVertexAttrib3f(1, tangent.x, tangent.y, tangent.z);
      glTexCoord2f(texc[0], texc[1]);
      glNormal3fv(normal);
      glVertex3fv(position);
      
    }

    glEnd();

  }
  
}
