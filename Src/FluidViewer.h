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

class FluidViewer : public Viewer {
private :
	Fluid *_fluid;
public :
	FluidViewer();
	~FluidViewer();

	void rendu();
};

#endif
