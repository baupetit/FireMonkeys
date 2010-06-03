
#include "scene.h"
#include <math.h>
#include <GL/glut.h>
#include "load_texture.h"

using namespace std;

Scene::Scene(const QGLViewer* const v)
{
  // Connect the viewer signal to our draw slot.
  connect(v, SIGNAL(drawNeeded()), this, SLOT(draw()));
  connect(v, SIGNAL(animateNeeded()), this, SLOT(animate()));
}


void Scene::init()
{

  // Light
  float Al[4] = {1.f, 1.f, 1.f, 0.0f };     // ambient
  float Dl[4] = {1.f, 1.f, 1.f, 0.0f };     // diffuse
  float Sl[4] = {0.9f, 0.9f, 0.9f, 0.0f };  // specular
  float Pl[4] = {-10.0, 0.0, 0.0, 1.0 };    // position
  memcpy(_Al, Al, sizeof(Al));
  memcpy(_Dl, Dl, sizeof(Dl));
  memcpy(_Sl, Sl, sizeof(Sl));

  // Put the light on a circle of radius rad
  _lightAngle = M_PI;
  float rad = 100.;
  Pl[0] = rad*cos(_lightAngle);
  Pl[2] = rad*sin(_lightAngle);
  memcpy(_Pl, Pl, sizeof(Pl));

  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  glDisable(GL_COLOR_MATERIAL); // in order to use usual material properties

  // Load Textures
  _textureDay   = loadTexture("Images/Earth__land_ocean_ice_2048.jpg");
  //_textureNight = loadTexture("Images/Earth__land_ocean_ice_lights_2048.jpg");

  // Initialization of the shaders
  
  initGLExtensions();      // requiered before using ShaderManager object

  _shader = _SM.loadfromFile("earthDay.vert", "earthDay.frag");
  //_shader = _SM.loadfromFile("earthDayAndBlack.vert", "earthDayAndBlack.frag");
  //_shader = _SM.loadfromFile("earthDayAndNight.vert", "earthDayAndNight.frag");
  //_shader = _SM.loadfromFile("earthSeaMask.vert", "earthSeaMask.frag");
  //_shader = _SM.loadfromFile("earthSunReflection.vert", "earthSunReflection.frag");

  if (_shader==0) cout<<"Error Loading, compiling or linking shader"<<endl;

  
}

// Draw the scene
void Scene::draw()
{
  // Light
  glLightfv(GL_LIGHT0, GL_AMBIENT , _Al );	
  glLightfv(GL_LIGHT0, GL_DIFFUSE , _Dl );	
  glLightfv(GL_LIGHT0, GL_SPECULAR, _Sl );	
  glLightfv(GL_LIGHT0, GL_POSITION, _Pl );
  glEnable(GL_LIGHT0);
  
  // Textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _textureDay);
  //glActiveTexture(GL_TEXTURE1);
  //glBindTexture(GL_TEXTURE_2D, _textureNight);
  
  if (_shader) {
    _shader->begin();
    
    _shader->setUniform1i("textureDay",0);
    //_shader->setUniform1i("textureNight",1);
  }
 
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glRotatef(-50,1,0,0);
  glRotatef(90,0,0,1);

  _sphere.draw(_shader);
  
  glPopMatrix();

  if (_shader) _shader->end();

}

/** 
 * Animate the scene
 * 
 */
void Scene::animate()
{
  // Make the Sun rotate around Earth!
  _lightAngle += 2.*M_PI/(25.*10.);
  float rad = 100;
  _Pl[0] = rad*cos(_lightAngle);
  _Pl[2] = rad*sin(_lightAngle);
  glLightfv(GL_LIGHT0, GL_POSITION, _Pl );    

  //std::cout << "lightAngle : " << _lightAngle << std::endl;
}
