
/*!*****************************************************************************
*
*       \file       :      main.cpp
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      26 mai 2010
*       \version    :      0.1
*
*******************************************************************************/


#include "Viewer.h"

int main(int argc, char **argv)
{
	Viewer window(string("GPU Fluid - Ensimag 2010"),
	              800, 800,    
	              200, 100);
	window.start();
	return(0);
}
