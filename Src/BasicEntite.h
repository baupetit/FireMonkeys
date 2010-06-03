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

class BasicEntite : public Noeud{
public:
    void Afficher_Entite(float dt);
	void Afficher_Entite_Face_Camera(Vecteur3D& positionCamera,Vecteur3D& directionCamera,float dt );
    virtual void Afficher(float dt) = 0;
	virtual void Afficher_Face_Camera(Vecteur3D& positionCamera,Vecteur3D& directionCamera,float dt ) = 0; 
};


#endif
