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
    /** 
        Constructeur par défaut
        @param titre de la fenetre
        @param largeur de la fenetre
        @param hauteur de la fenetre
        @positionX initiale de la fenetre
        @positionY initiale de la fenetre
    */
    ViewerGPU(const string & title, 
              int width, 
              int height,
	          int initPositionX = 0, 
	          int initPositionY = 0);
	       
	/** Destructeur */
    ~ViewerGPU();
        
    /** Initialise la scène */
    void initScene();
    
    /** Lance le rendu de la scène */
    void rendu();
    

};


#endif
