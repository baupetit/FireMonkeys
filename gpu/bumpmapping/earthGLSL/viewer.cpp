#include "viewer.h"


Viewer::Viewer()
  : _scene(NULL)
{
  
}

void Viewer::setScene(Scene* scene)
{
  _scene = scene;
}

void Viewer::init()
{
  if (!_scene) return;

  _scene->init();
}
