#include <GL/glew.h> // must be included before any OpenGL related file
#include <qapplication.h>
#include <GL/glut.h>
#include "scene.h"
#include "viewer.h"

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);

  // Init Glut - just to use glut 3D objects
  glutInit(&argc,argv);
  
  // Instantiate the viewer.
  Viewer v;

  // Create a scene, giving a pointer to the associated viewer.
  Scene s(&v);

  // Connect the viewer and the scene
  v.setScene(&s);

  // Make the viewer window visible on screen.
  // It calls the init functions
  v.show();

#if QT_VERSION < 0x040000
  // Set the viewer as the application main widget.
  application.setMainWidget(&v);
#endif

  // Run main loop.
  return application.exec();
}
