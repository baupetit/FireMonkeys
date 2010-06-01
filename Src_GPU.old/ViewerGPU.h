#ifndef VIEWERGPU_H
#define VIEWERGPU_H

/*!*****************************************************************************
*
*       \file       :      ViewerGPU.h
*
*       \author     :      Benjamin Aupetit, Champeau Julien, Arnaud Emilien
*       \date       :      26 mai 2010
*       \version    :      0.1
*
*******************************************************************************/


#include "Viewer.h"


#include "Fluid_GPU.h"


class ViewerGPU : public Viewer{

private:
    // Systeme de fluide associé
    Fluid_GPU *_fluid;

public :
    ViewerGPU(const string & title, int width, int height,
	       int initPositionX = 0, int initPositionY = 0);
    ~ViewerGPU();
        
    void initScene();
    void rendu();
    

};


#endif
