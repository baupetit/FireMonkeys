#ifndef SOLVERVIEWER_H
#define SOLVERVIEWER_H

/*!*****************************************************************************
*
*       \file       :      solverViewer.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      26 mai 2010
*       \version    :      0.1
*
*******************************************************************************/

#include "Viewer.h"
#include "solver.h"

class SolverViewer : public Viewer {
private :
	Solver *s;
public :
	SolverViewer();
	~SolverViewer();

	virtual void rendu();
};

#endif
