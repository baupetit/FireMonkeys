#ifndef BASICENTITE_H
#define BASICENTITE_H

/*!*****************************************************************************
*
*       \file       :      BasicEntite.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      26 mai 2010
*       \version    :      0.1
*
*******************************************************************************/

#include "Noeud.h"
#include <GL/glew.h>
#include <GL/glut.h>



class BasicEntite : public Noeud{
public:
    void Afficher_Entite();
    void Afficher_Entite_Face_Camera(Vecteur3D& positionCamera,Vecteur3D& directionCamera );
    virtual void Afficher() = 0;
    virtual void Afficher_Face_Camera(Vecteur3D& positionCamera,Vecteur3D& directionCamera ) = 0; 
};


#endif
