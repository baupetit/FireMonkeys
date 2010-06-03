#ifndef _VIEWER_H_
#define _VIEWER_H_

#include <GL/glew.h> // must be included before any OpenGL related file
#include <QGLViewer/qglviewer.h>
#include "scene.h"

class Viewer : public QGLViewer
{

public:
  Viewer();
  void setScene(Scene* scene);
  virtual void init();

private:
  Scene* _scene;
};


#endif
