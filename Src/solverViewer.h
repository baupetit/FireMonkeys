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
	
	void renduFumeeGPU();
	void renduFlammeGPU();
	void majMatriceFumeeEnMatriceRGBA();
	void matriceRGBACarreeToTexture3D(const float *matrice, int cote, GLuint id_texture);
	void initialiserRenduGPU();
	void dessinerPlansDansTexture3D(GLuint id_texture, int nb_plans);
private :
    GLuint _id_texture_flamme;
    GLuint _id_texture_fumee;
    int tailleSolver;
    float *matriceRGBA;
};

#endif
