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
    virtual void Afficher() = 0;
};


#endif
