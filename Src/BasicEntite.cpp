#include "BasicEntite.h"

#include <GL/glut.h>
#include <GL/gl.h>

void BasicEntite::Afficher_Entite(){   
 
    glPushMatrix();
    glMatrixMode (GL_MODELVIEW);
    glTranslatef(position.x, position.y, position.z);
    
    glRotated (anglesEuler.x, 1, 0, 0);
    glRotated (anglesEuler.y, 0, 1, 0);
    glRotated (anglesEuler.z, 0, 0, 1);
    
    glScalef(echelle.x, echelle.y, echelle.z);

    Afficher();    
    
    glPopMatrix();
}
