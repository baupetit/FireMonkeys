#ifndef _SCENE_H_
#define _SCENE_H_

#include <GL/glew.h> // must be included before any OpenGL related file
#include <qobject.h>
#include <QGLViewer/qglviewer.h>
#include "glsl.h"

#include "objects.h"

class Scene : public QObject
{
  Q_OBJECT
  
public :
  Scene(const QGLViewer* const v);
  
  void init();
  
public slots:
  void draw();
  void animate();
  
private:
  // Shader Stuff
  glShaderManager _SM;		/**< Shader Manager from glsl.h      */
  glShader*       _shader;	/**< single vertex + fragment shader */
  
  // Light0 Properties
  float _Al[4];			/**< Ambiant  */
  float _Dl[4];			/**< Diffuse  */
  float _Sl[4];			/**< Specular */
  float _Pl[4];			/**< Position */

  float _lightAngle;

  // 3D Objects
  Sphere _sphere;
  
  // Textures
  GLuint _textureDay;
  //GLuint _textureNight;
};

#endif
