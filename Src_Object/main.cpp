
/*!*****************************************************************************
*
*       \file       :      main.cpp
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      26 mai 2010
*       \version    :      0.1
*
*******************************************************************************/


#include "FluidViewer.h"
#include "ObjectViewer.h"
#include "SolverParam.h"

int main(int argc, char **argv)
{
	SolverParam::initSolverParam();
	FluidViewer window;
	//ObjectViewer window;
	window.start();
	return 0;
}
