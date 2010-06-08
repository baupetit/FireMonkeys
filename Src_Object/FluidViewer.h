#ifndef FLUIDVIEWER_H
#define FLUIDVIEWER_H

/*!*****************************************************************************
*
*       \file       :      FluidViewer.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      26 mai 2010
*       \version    :      0.1
*
*******************************************************************************/

#include "Viewer.h"
#include "Fluid.h"
#include "Object.h"
#include <list>

class FluidViewer : public Viewer {
private :
	Fluid *_fluid;
	list<Object *> _objs;
	list<Object *>::iterator _it_objs;
public :
	FluidViewer();
	~FluidViewer();

	void rendu();
};

#endif
