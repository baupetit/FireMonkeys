#include "load_texture.h"
#include <iostream>

/** 
 * Load and image file and return a valid OpenGL texture id.
 * 
 * @param imgFilename 
 * 
 * @return 
 */
GLuint loadTexture(std::string imgFilename)
{
  // load image
  const QImage img(imgFilename.c_str());
  
  if (img.isNull()) {
    std::cerr << "Error: Can't load image: " << imgFilename << std::endl;
    exit(-1);
  }
  
  // convert it to OpenGL format
  const QImage& imgTex = QGLWidget::convertToGLFormat(img);
  
  // ask for a valid name for a texture
  GLuint textureId;
  glGenTextures( 1, &textureId );
  
  // prepare the OpenGL texture
  glBindTexture(GL_TEXTURE_2D, textureId);
  
  // give its parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // and its texels
  glTexImage2D(GL_TEXTURE_2D, 0, 3, imgTex.width(), imgTex.height(), 0, 
	       GL_RGBA, GL_UNSIGNED_BYTE, imgTex.bits() );
  
  return textureId;
}
