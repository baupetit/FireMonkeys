#ifndef NOEUD_H
#define NOEUD_H

/*!*****************************************************************************
*
*       \file       :      Noeud.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      26 mai 2010
*       \version    :      0.1
*
*******************************************************************************/

#include "Vecteur.h"

class Noeud{
public:
    Vecteur3D position;
    Vecteur3D echelle;
    Vecteur3D anglesEuler;
};


#endif
